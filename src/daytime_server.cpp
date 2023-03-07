//
// server.cpp
// ~~~~~~~~~~
//
// Copyright (c) 2003-2019 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#include <ctime>
#include <iostream>
#include <string>
#include <boost/asio.hpp>

using boost::asio::ip::tcp;

std::string make_daytime_string()
{
  using namespace std; // For time_t, time and ctime;
  time_t now = time(0);
  return ctime(&now);
}

int main()
{
  try
  {
    // Definerer et objekt der indeholder hvad vi kommunikerer med
    boost::asio::io_context io_context; 

    // Definerer "acceptor" objektet, og observerer port 13
    tcp::acceptor acceptor(io_context, tcp::endpoint(tcp::v4(), 13)); 

    
    while (true)
    {
      tcp::socket socket(io_context); // Socket der indeholder vores kommunikations muligheder
      acceptor.accept(socket); // Serveren acceptere kommunikationen med dette endpoint (clienten)

      std::string message = make_daytime_string(); // Laver en string der indeholder tiden som serveren holder styr på

      boost::system::error_code ignored_error; // laver en variabel der indeholder indexet på en fejltype
      boost::asio::write(socket, boost::asio::buffer(message), ignored_error); // Skriver beskeden til clienten, og prøver hele loopet igen hvis det fejler.
    }
  }
  catch (std::exception &e) // hvis programmet ovenfor slår fejl, og printer erroren nedenfor.
  {
    std::cerr << e.what() << std::endl;
  }

  return 0;
}