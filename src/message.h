//
// Created by shecannotsee on 2023/3/7.
//

#ifndef SHE_NET_SRC_MESSAGE_H_
#define SHE_NET_SRC_MESSAGE_H_

#include "socket.h"
#include <string>
#include <memory>

namespace sheNet {

class message {
 public:
  message() = delete;
  message(std::unique_ptr<sheNet::socket> socket);
  ~message() = default;
  // disable copy
  message(const message& x) = delete;
  message& operator=(const message& x) = delete;
  // open move
  message(message&&) = default;
  message& operator=(message&& x) = default;
 private:
  std::unique_ptr<sheNet::socket> socket_;
 public:
  std::string get();
  void send(std::string&&);

 private:
  void tcp_get();
  void tcp_send();
  void udp_get();
  void udp_send();




};

};// namespace sheNet

#endif //SHE_NET_SRC_MESSAGE_H_
