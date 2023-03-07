//
// client.cpp
// ~~~~~~~~~~
//
// Copyright (c) 2003-2019 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#include <iostream>
#include <fstream> // files
#include <boost/array.hpp>
#include <boost/asio.hpp>

using boost::asio::ip::tcp;

constexpr size_t image_size = 133*132 + 60; // Ændret for at hele katten bliver indlæst

void save_image(char* data, size_t len)
{
  std::ofstream image;
  image.open("copycat.jpg", std::ios::binary); // Opens the image in binary mode
  
  image.write(data, len); // writes the date to the image
}

int main(int argc, char* argv[])
{
  try
  {
    if (argc != 2) // tjekker om der er 2 adresser, hvis ikke, så kører den (burde nok være "argc = 1", da vi kun vil sende til 1 server
    {
      std::cerr << "Usage: client <host>" << std::endl;
      return 1;
    }

    boost::asio::io_context io_context;

    tcp::resolver resolver(io_context);
    tcp::resolver::results_type endpoints = resolver.resolve(argv[1], "daytime");

    tcp::socket socket(io_context);
    boost::asio::connect(socket, endpoints);

    while(true)
    {
      boost::array<char, image_size> buf;
      boost::system::error_code error;

      // read until buffer is full
      boost::asio::read(socket, boost::asio::buffer(buf));

      if (error == boost::asio::error::eof)
        break; // Connection closed cleanly by peer.
      else if (error)
        throw boost::system::system_error(error); // Some other error.

      save_image(buf.data(),image_size);
    }
  }
  catch (std::exception& e)
  {
    std::cerr << e.what() << std::endl;
  }

  return 0;
}