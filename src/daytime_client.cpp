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
#include <boost/array.hpp>
#include <boost/asio.hpp>

using boost::asio::ip::tcp;

int main(int argc, char* argv[])
{
  
  try
  {
    // Hvis argc != 2, så printer den fejlen
    if (argc != 2)
    {
      std::cerr << "Usage: client <host>" << std::endl;
      return 1;
    }

    // Creates io_context class containing the I/O devices
    boost::asio::io_context io_context;

    // Creates resolver class
    tcp::resolver resolver(io_context);

    // The resolver class translates the query into endpoints
    tcp::resolver::results_type endpoints = resolver.resolve(argv[1], "daytime"); //laver endpoints ud fra de ip'er som ligger i io_context, og i dette tilfælde er det 1

    //
    tcp::socket socket(io_context); //laver et socket af vores kommunikations object
    boost::asio::connect(socket, endpoints); // socket connector til det endpoint som er lavet

    while(true)
    {
      boost::array<char, 128> buf;
      boost::system::error_code error;

      size_t len = socket.read_some(boost::asio::buffer(buf), error); // læser hvad serveren skriver og lægger det i "buf"

      if (error == boost::asio::error::eof)
        break; // Connection closed cleanly by peer.
      else if (error)
        throw boost::system::system_error(error); // Some other error.

      std::cout.write(buf.data(), len); // Udskriver hvad serveren har skrevet til den
    }
  }
  catch (std::exception& e)
  {
    std::cerr << e.what() << std::endl;
  }

  return 0;
}