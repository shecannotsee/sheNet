//
// Created by shecannotsee on 24-2-2.
//

#ifndef LISTEN_H
#define LISTEN_H
#include <sys/socket.h>

#include <stdexcept>
#include <unordered_map>

#include "socket.h"
#include "socket_define.h"

namespace she_net {
namespace listen_param {
enum class error_info : int {
  port_already_in_use   = EADDRINUSE,
  socktfd_invalid       = EBADF,
  non_socket_fd         = ENOTSOCK,
  sockets_not_supported = EOPNOTSUPP,
};
const std::unordered_map<error_info, std::string> error_info_map = {
    {error_info::port_already_in_use, "port_already_in_use"},
    {error_info::socktfd_invalid, "socktfd_invalid"},
    {error_info::non_socket_fd, "non_socket_fd"},
    {error_info::sockets_not_supported, "sockets_not_supported"},
};

}  // namespace listen_param

template <typename type>
struct listen_param_check {
  constexpr listen_param_check() {
    constexpr bool error = (type::value == socket_param::type::stream || type::value == socket_param::type::datagrams);
    static_assert(error, "listen just support SOCK_STREAM and SOCK_SEQPACKET");
  }
};

class listen_t {
  static void handle_error(const int status) {
    if (status == 0) {
      // success
      return;
    } else if (status == -1) {
      const auto error_info = static_cast<listen_param::error_info>(errno);
      if (const auto it = listen_param::error_info_map.find(error_info); it != listen_param::error_info_map.end()) {
        const auto error_message = it->second;
        // error handling
        throw std::runtime_error("bind socket failed: " + error_message);
      } else {
        // Handle the case where error_info is not found in the map
        throw std::runtime_error("bind socket failed with unknown error");
      }
    } else {
      throw std::runtime_error("Error: bind socket returned " + std::to_string(status));
    }
  }

 public:
  template <typename type>
  static void exec(const socket_fd fd) noexcept {
    listen_param_check<type>();
    constexpr int backlog = 5;
    const auto status     = ::listen(fd, backlog);
    handle_error(status);
  }
};

}  // namespace she_net

#endif  // LISTEN_H
