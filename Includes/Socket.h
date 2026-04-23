#ifndef __SOCKET_LIB_H__
#define __SOCKET_LIB_H__
#ifdef _WIN32
#include <winsock2.h>
#include <ws2tcpip.h>
#pragma comment(lib, "ws2_32.lib")

using socket_t = SOCKET;
using socklen_t = int;

#else
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>

using socket_t = int;

#define INVALID_SOCKET (-1)
#define SOCKET_ERROR (-1)

#endif
#include <iostream>
#include <memory>
#include <mutex>
class Socket
{
    socket_t iL_socket_fd{};
    sockaddr_in address{};
    std::mutex mu;

public:
    Socket();
    ~Socket();
    socket_t mcfn_create(const int &port);
    int mcfn_bind();
    int mcfn_listin(const int &maxCon);
    int mcfn_accept(Socket &client);
    int mcfn_accept(std::shared_ptr<Socket> &client);
    int mcfn_connect(const std::string &IP = "");
    int mcfn_send(const std::string &buf);
    int mcfn_recv(std::string &buf, bool blocking = true);
};
#endif