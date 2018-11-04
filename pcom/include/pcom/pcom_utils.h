#ifndef PCOM_UTILS_H
#define PCOM_UTILS_H

#include <iostream>
#include <fstream>
#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/path.hpp>

namespace pcom{

template<typename T>
unsigned int get_size(T data){
  return boost::asio::buffer_size(boost::asio::buffer(data));
}

template<typename T>
void copy_buffer(const boost::asio::mutable_buffer& buffer, T& output){
   T const* tb = static_cast<T const*>(boost::asio::buffer_cast<const void*>(buffer));
   std::copy(tb->cbeing(), tb->cend(), output);
}


inline  bool is_directory(const std::string& file_path)
{
  try {
    // Create a Path object from given path string
  	boost::filesystem::path path(file_path);
  	// Check if path exists and is of a directory file
  	if (boost::filesystem::exists(path) && boost::filesystem::is_directory(path)){
      return true;
    }
  }
  catch (boost::filesystem::filesystem_error & e)
  {
  	std::cerr << e.what() << std::endl;
  }
  return false;
}

inline bool is_file(std::string file_path){
   return boost::filesystem::exists(file_path);
}

inline bool create_file(const std::string file_path){
  //if(!is_directory(file_path))
  //  return false;
  std::cout<< "create file: " << file_path << std::endl;
  std::ofstream tmp_file(file_path);
  tmp_file.close();
  return true;
}

inline void remove_file(std::string file_path){
  boost::filesystem::path path(file_path);
  boost::filesystem::remove(path);
}

inline void create_directory(const std::string& folder_path){
  boost::filesystem::path path(folder_path);
  boost::filesystem::create_directory(path);
}

}

#endif
