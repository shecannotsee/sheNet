//
// Created by shecannotsee on 23-5-17.
//

#ifndef SHE_NET_SRC_EPOLL_WRAPPER_EPOLL_WRAPPER_H_
#define SHE_NET_SRC_EPOLL_WRAPPER_EPOLL_WRAPPER_H_

#include <vector>
#include <sys/epoll.h>

namespace sheNet {

class epoll_wrapper {
 public:
  // copy:off
  epoll_wrapper(const epoll_wrapper&) = delete;
  epoll_wrapper& operator=(const epoll_wrapper&) = delete;
  // move:off
  epoll_wrapper(epoll_wrapper&&) = default;
  epoll_wrapper& operator=(epoll_wrapper&&) = default;
  //destructors
  ~epoll_wrapper() = default;
  //constructors
  epoll_wrapper();
  explicit epoll_wrapper(int events_num);

 private:
  int epoll_container_id_;///< epoll容器
  std::vector<epoll_event> user_events_;///< epoll event的集合
  int timeout_set_;///< 用于设置epoll的超时时间,单位是毫秒,默认为-1表示永久阻塞
  int server_socket_fd_;///< local fd

 public:
  /**
   * @brief 将监听端口的fd进行存储
   * @param local_fd 进行监听的文件描述符
   */
  void add_server_fd(int local_fd);

  /**
   * @brief 添加那些已经通过 accept 建立连接的 client fd
   * @param fd 需要添加的文件描述符
   * @param trigger_mode 触发方式,默认为水平触发,可以通过传入 EPOLLET 进行边缘触发,但是不建议
   */
  void add_alive_fd(int fd, EPOLL_EVENTS trigger_mode = EPOLLIN) const;

  /**
   * @brief 调用 epoll_ctl,使用 EPOLL_CTL_DEL 操作将客户端的文件描述符从 epoll 实例中剔除
   * @param fd 需要剔除的文件描述符
   */
  void remove_alive_fd(int fd);


};

};// namespace sheNet

#endif //SHE_NET_SRC_EPOLL_WRAPPER_EPOLL_WRAPPER_H_
