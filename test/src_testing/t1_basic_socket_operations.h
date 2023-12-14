//
// Created by shecannotsee on 23-12-14.
//

#ifndef T1_BASIC_SOCKET_OPERATIONS_H
#define T1_BASIC_SOCKET_OPERATIONS_H

#include <iostream>
#include <string>
#include <thread>

#include <basic_socket_operations/basic_socket_operations.h>
#include <four_tuple.h>

#include <she_net_exception/she_net_exception.h>

namespace src_testing::t1_basic_socket_operations {

std::string main() {
  using st/* socket tools*/ = she_net::basic_socket_operations;
  bool server_start = false;

  std::thread server([&]() {
    auto type = she_net::TRANSPORT_ADDRESS_TYPE::TCP_IPV4;
    she_net::four_tuple address;/* init */ {
      address.source_ip = "0.0.0.0";
      address.source_port = 9981;
    };
    int fd = st::socket(type);/* print */{
      std::cout << "server:socket create fd:" << fd << "\n";
    };
    st::bind(fd, address.source_ip, std::to_string(address.source_port), type);/* print */ {
      std::cout << "server:bind " + address.source_ip + ":" + std::to_string(address.source_port) + " success.\n";
    };
    st::listen(fd);/* print */ {
      std::cout << "server:listen success.\n";
    };
    /* print */ {
      std::cout << "server:wait to accept...\n";
    }
    while(true) {
      try {
        server_start = true;
        int client_fd = st::accept(fd);/* print */ {
          std::cout << "server:accept to client fd is " << client_fd << "\n";
        }
        if(client_fd!= -1) {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
          break;
        }
      }
      catch (const she_net::she_net_exception& exc) {
        std::cout << exc.what();
        break;
      }
    }
    st::shutdown(fd);
    std::cout << "server close.\n";
  });

  std::thread client([&](){
    while (server_start == false) {}
    auto type = she_net::TRANSPORT_ADDRESS_TYPE::TCP_IPV4;
    int fd = st::socket(type);/* print */{
      std::cout << "client:socket create fd:" << fd << "\n";
    };

    int used_port = -1;
    while (used_port == -1) {
      try {
        used_port = st::connect(fd, "192.168.1.47", "9981");/* print */ {
          std::cout << "client:connect success,local port is[" + std::to_string(used_port) + "]\n";
        };
      }
      catch (const she_net::she_net_exception& exc) {
        std::cout << exc.what();
        break;
      };
    }
    st::shutdown(fd);
    std::cout << "client close.\n";
  });


  server.join();
  client.join();

  return "t1_basic_socket_operations pass test";
}

}

#endif //T1_BASIC_SOCKET_OPERATIONS_H
