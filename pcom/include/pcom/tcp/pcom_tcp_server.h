#ifndef PCOM_TCP_SERVER_H
#define PCOM_TCP_SERVER_H

#include <algorithm>
#include <cstdlib>
#include <deque>
#include <iostream>
#include <set>
#include <boost/bind.hpp>
#include <boost/circular_buffer.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/asio/deadline_timer.hpp>
#include <boost/asio/io_service.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/ip/udp.hpp>
#include <boost/asio/read_until.hpp>
#include <boost/asio/streambuf.hpp>
#include <boost/asio/write.hpp>
#include <thread>

using boost::asio::deadline_timer;
using boost::asio::ip::tcp;
using boost::asio::ip::udp;

namespace pcom{

  class subscriber{
  public:
    virtual ~subscriber() {}
    virtual void deliver(const boost::asio::const_buffer& item) = 0;
  };

  typedef boost::shared_ptr<subscriber> subscriber_ptr;

  class channel{
  public:
    void join(subscriber_ptr subscriber)
    {
      subscribers_.insert(subscriber);
    }

    void leave(subscriber_ptr subscriber)
    {
      subscribers_.erase(subscriber);
    }

    void deliver(const boost::asio::const_buffer& item)
    {
      std::for_each(subscribers_.begin(), subscribers_.end(), boost::bind(&subscriber::deliver, _1, boost::ref(item)));
    }

    void close(){
        //std::for_each(subscribers_.begin(), subscribers_.end(), [this](subscriber_ptr p){leave(p);});
    }
  private:
    std::set<subscriber_ptr> subscribers_;
  };

  class tcp_session : public subscriber, public boost::enable_shared_from_this<tcp_session>{

  public:
  tcp_session(boost::asio::io_service& io_service, channel& ch)
    : channel_(ch),
      socket_(io_service),
      input_deadline_(io_service),
      non_empty_output_queue_(io_service),
      output_deadline_(io_service)
  {
    /*input_deadline_.expires_at(boost::posix_time::pos_infin);
    output_deadline_.expires_at(boost::posix_time::pos_infin);

    // The non_empty_output_queue_ deadline_timer is set to pos_infin whenever
    // the output queue is empty. This ensures that the output actor stays
    // asleep until a message is put into the queue.
    non_empty_output_queue_.expires_at(boost::posix_time::pos_infin);*/
  }

  boost::asio::ip::tcp::socket& socket()
  {
    return socket_;
  }

  // Called by the server object to initiate the four actors.
  void start()
  {
    /*channel_.join(shared_from_this());

    start_read();

    input_deadline_.async_wait(
        boost::bind(&tcp_session::check_deadline,
        shared_from_this(), &input_deadline_));

    await_output();

    output_deadline_.async_wait(
        boost::bind(&tcp_session::check_deadline,
        shared_from_this(), &output_deadline_));*/
  }

private:
  void stop()
  {
    channel_.leave(shared_from_this());

    boost::system::error_code ignored_ec;
    socket_.close(ignored_ec);
    input_deadline_.cancel();
    non_empty_output_queue_.cancel();
    output_deadline_.cancel();
  }

  bool stopped() const
  {
    return !socket_.is_open();
  }

  void deliver(const boost::asio::const_buffer& item)
  {
    std::vector<unsigned char> data(boost::asio::buffer_size(item));
    boost::asio::buffer_copy(boost::asio::buffer(data), item);

    output_queue_.push_back(boost::asio::buffer(data));

    // Signal that the output queue contains messages. Modifying the expiry
    // will wake the output actor, if it is waiting on the timer.
    non_empty_output_queue_.expires_at(boost::posix_time::neg_infin);
  }

  void start_read()
  {
    // Set a deadline for the read operation.
    input_deadline_.expires_from_now(boost::posix_time::seconds(30));

    // Start an asynchronous operation to read a newline-delimited message.
    boost::asio::async_read_until(socket_, input_buffer_, '\n',
        boost::bind(&tcp_session::handle_read, shared_from_this(), _1));
  }

  void handle_read(const boost::system::error_code& ec)
  {
    if (stopped())
      return;

    if (!ec)
    {
      // Extract the newline-delimited message from the buffer.
      std::string msg;
      std::istream is(&input_buffer_);
      std::getline(is, msg);

      if (!msg.empty())
      {
        channel_.deliver(boost::asio::buffer(msg));
      }
      else
      {
        // We received a heartbeat message from the client. If there's nothing
        // else being sent or ready to be sent, send a heartbeat right back.
        if (output_queue_.empty())
        {
          // TODO: send something back
          std::vector<unsigned char> data;
          data.push_back('\n');
          output_queue_.push_back(boost::asio::buffer(data));

          // Signal that the output queue contains messages. Modifying the
          // expiry will wake the output actor, if it is waiting on the timer.
          non_empty_output_queue_.expires_at(boost::posix_time::neg_infin);
        }
      }

      start_read();
    }
    else
    {
      stop();
    }
  }

  void await_output()
  {
    if (stopped())
      return;

    if (output_queue_.empty())
    {
      // There are no messages that are ready to be sent. The actor goes to
      // sleep by waiting on the non_empty_output_queue_ timer. When a new
      // message is added, the timer will be modified and the actor will wake.
      non_empty_output_queue_.expires_at(boost::posix_time::pos_infin);
      non_empty_output_queue_.async_wait(
          boost::bind(&tcp_session::await_output, shared_from_this()));
    }
    else
    {
      start_write();
    }
  }

  void start_write()
  {
    // Set a deadline for the write operation.
    output_deadline_.expires_from_now(boost::posix_time::seconds(30));

    // Start an asynchronous operation to send a message.
    boost::asio::async_write(socket_,output_queue_.front(),
        boost::bind(&tcp_session::handle_write, shared_from_this(), _1));
  }

  void handle_write(const boost::system::error_code& ec)
  {
    if (stopped())
      return;

    if (!ec)
    {
      output_queue_.pop_front();

      await_output();
    }
    else
    {
      stop();
    }
  }

  void check_deadline(deadline_timer* deadline)
  {
    if (stopped())
      return;

    // Check whether the deadline has passed. We compare the deadline against
    // the current time since a new asynchronous operation may have moved the
    // deadline before this actor had a chance to run.
    if (deadline->expires_at() <= deadline_timer::traits_type::now())
    {
      // The deadline has passed. Stop the session. The other actors will
      // terminate as soon as possible.
      stop();
    }
    else
    {
      // Put the actor back to sleep.
      deadline->async_wait(
          boost::bind(&tcp_session::check_deadline,
          shared_from_this(), deadline));
    }
  }

  channel& channel_;
  tcp::socket socket_;
  boost::asio::streambuf input_buffer_;
  deadline_timer input_deadline_;
  std::deque<boost::asio::mutable_buffers_1> output_queue_;
  deadline_timer non_empty_output_queue_;
  deadline_timer output_deadline_;
};

typedef boost::shared_ptr<tcp_session> tcp_session_ptr;


class udp_broadcaster : public subscriber{

public:
  udp_broadcaster(boost::asio::io_service& io_service,
      const udp::endpoint& broadcast_endpoint)
    : socket_(io_service)
  {
    socket_.connect(broadcast_endpoint);
  }

private:
  void deliver(const boost::asio::const_buffer& msg)
  {
    boost::system::error_code ignored_ec;
    std::vector<unsigned char> data(boost::asio::buffer_size(msg));
    boost::asio::buffer_copy(boost::asio::buffer(data), msg);
    socket_.send(boost::asio::buffer(data), 0, ignored_ec);
  }

  udp::socket socket_;
};


class server{

public:
  server(boost::asio::io_service& io_service,
      const tcp::endpoint& listen_endpoint,
      const udp::endpoint& broadcast_endpoint)
    : io_service_(io_service),
      acceptor_(io_service, listen_endpoint)
  {
    subscriber_ptr bc(new udp_broadcaster(io_service_, broadcast_endpoint));
    channel_.join(bc);
    start_accept();
  }

  void start_accept()
  {
      std::cout<<"start accept"<< std::endl;
    tcp_session_ptr new_session(new tcp_session(io_service_, channel_));
    acceptor_.async_accept(new_session->socket(), boost::bind(&server::handle_accept, this, new_session, _1));
  }

  void handle_accept(tcp_session_ptr session,
      const boost::system::error_code& ec)
  {
    if (!ec)
    {
     std::cout<<"start "<< std::endl;
      session->start();
    }

    start_accept();
  }

public:

  channel channel_;

private:
  boost::asio::io_service& io_service_;
  tcp::acceptor acceptor_;
};


}

#endif
