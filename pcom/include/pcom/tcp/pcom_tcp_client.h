#ifndef PCOM_TCP_CLIENT_H
#define PCOM_TCP_CLIENT_H

#include <iostream>
#include <boost/array.hpp>
#include <boost/asio.hpp>
#include <boost/circular_buffer.hpp>

namespace pcom{

  class client{

  public:

      client(boost::asio::io_service& io_service, std::string host, int port, boost::circular_buffer<std::string>& msg_buffer):
      io_service(io_service), socket(io_service), deadline_(io_service), heartbeat_timer_(io_service), msg_buffer_(msg_buffer)
      {

          boost::asio::ip::tcp::resolver resolver(io_service);
          boost::asio::ip::tcp::resolver::query query(host, std::to_string(port));
          boost::asio::ip::tcp::resolver::iterator endpoint_iterator = resolver.resolve(query);
          stopped_ = false;
          start_connect(endpoint_iterator);
          // Start the deadline actor. You will note that we're not setting any
          // particular deadline here. Instead, the connect and input actors will
          // update the deadline prior to each asynchronous operation.
          deadline_.async_wait(std::bind(&client::check_deadline, this));
     }

      void start_connect(boost::asio::ip::tcp::resolver::iterator endpoint_iter){
         if (endpoint_iter != boost::asio::ip::tcp::resolver::iterator()){
           std::cout << "Trying " << endpoint_iter->endpoint() << "...\n";
           // Set a deadline for the connect operation.
           deadline_.expires_from_now(boost::posix_time::seconds(60));
           // Start the asynchronous connect operation.
           socket.async_connect(endpoint_iter->endpoint(), std::bind(&client::handle_connect, this, std::placeholders::_1, endpoint_iter));
         }else{
           // There are no more endpoints to try. Shut down the client.
           stop();
         }
       }

      void stop(){
         stopped_ = true;
         socket.close();
         deadline_.cancel();
         heartbeat_timer_.cancel();
       }

      void handle_connect(const boost::system::error_code& ec,  boost::asio::ip::tcp::resolver::iterator endpoint_iterator){
          if (stopped_)
              return;
          if (!socket.is_open()){
             std::cout << "Connect timed out\n";
             // Try the next available endpoint.
             start_connect(++endpoint_iterator);
          }else if(ec){
              std::cout << "Connect error: " << ec.message() << "\n";
              // We need to close the socket used in the previous connection attempt
              // before starting a new one.
              socket.close();
              // Try the next available endpoint.
              start_connect(++endpoint_iterator);
          }else{
              start_read();
              start_write();
         }
      }

      void start_read(){
          // Set a deadline for the read operation.
          deadline_.expires_from_now(boost::posix_time::seconds(1));
          boost::asio::async_read_until(socket, input_buffer_, '\n', std::bind(&client::handle_read, this, std::placeholders::_1));
      }

      void handle_read(const boost::system::error_code& ec){
          if (stopped_)
               return;
          if (!ec){
              // Extract the newline-delimited message from the buffer.
              std::string line;
              std::istream is(&input_buffer_);
              std::getline(is, line);
              // Empty messages are heartbeats and so ignored.
              if (!line.empty())
              {
                msg_buffer_.push_back(line);
              }
              start_read();
          }else{
              stop();
          }
      }

      void start_write(){
          if (stopped_)
              return;
          // Start an asynchronous operation to send a heartbeat message.
          boost::asio::async_write(socket, boost::asio::buffer("\n", 1), std::bind(&client::handle_write, this, std::placeholders::_1));
      }

      void handle_write(const boost::system::error_code& ec){
         if (stopped_)
           return;
         if (!ec){
           // Wait 10 seconds before sending the next heartbeat.
           heartbeat_timer_.expires_from_now(boost::posix_time::seconds(10));
           heartbeat_timer_.async_wait(std::bind(&client::start_write, this));
         }else{
           std::cout << "Error on heartbeat: " << ec.message() << "\n";
           stop();
         }

      }

      void check_deadline(){
          if (stopped_)
              return;
          // Check whether the deadline has passed. We compare the deadline against
          // the current time since a new asynchronous operation may have moved the
          // deadline before this actor had a chance to run.
          if (deadline_.expires_at() <= boost::asio::deadline_timer::traits_type::now()){
            // The deadline has passed. The socket is closed so that any outstanding
            // asynchronous operations are cancelled.
            socket.close();
            // There is no longer an active deadline. The expiry is set to positive
            // infinity so that the actor takes no action until a new deadline is set.
            deadline_.expires_at(boost::posix_time::pos_infin);
          }
          // Put the actor back to sleep.
          deadline_.async_wait(std::bind(&client::check_deadline, this));
      }


  private:

      boost::asio::io_service& io_service;
      boost::asio::ip::tcp::socket socket;
      boost::asio::streambuf input_buffer_;
      boost::asio::deadline_timer deadline_;
      boost::asio::deadline_timer heartbeat_timer_;
      boost::circular_buffer<std::string>& msg_buffer_;
      bool stopped_;


  };

}

#endif
