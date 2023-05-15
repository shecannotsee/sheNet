//
// Created by shecannotsee on 23-5-15.
//

#ifndef SHE_NET_TEST_M9_BASIC_SOCKET_OPERATIONS_H_
#define SHE_NET_TEST_M9_BASIC_SOCKET_OPERATIONS_H_

#include <thread>
#include <basic_socket_operations/basic_socket_operations.h>
#include <four_tuple.h>

namespace m9_basic_socket_operations {

void main() {
  using st/* socket tools*/ = sheNet::basic_socket_operations;

  std::thread server([]() {
    auto type = sheNet::TRANSPORT_ADDRESS_TYPE::TCP_IPV4;
    sheNet::four_tuple address;/* init */ {
    address.source_ip = "0.0.0.0";
    address.source_port = 9981;
  }
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
    int client_fd = st::accept(fd);/* print */ {
      std::cout << "server:client fd is " << client_fd << "\n";
    }
  });

  std::thread client([](){
    sleep(1);
    auto type = sheNet::TRANSPORT_ADDRESS_TYPE::TCP_IPV4;
    sheNet::four_tuple address;/* init */ {
    address.source_ip = "0.0.0.0";
    address.source_port = 9981;
  }
    int fd = st::socket(type);/* print */{
    std::cout << "client:socket create fd:" << fd << "\n";
  };
    int used_port = st::connect(fd,"192.168.1.47","9981");/* print */ {
      std::cout << "client:connect success,local port is["+std::to_string(used_port)+"]\n";
    };
  });


  server.join();
  client.join();
};

};// namespace m9_basic_socket_operations

#endif //SHE_NET_TEST_M9_BASIC_SOCKET_OPERATIONS_H_
