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

// #define MAX_MSG_LEN 10000
#include <iostream>
#include <memory>
#include <mutex>
class Socket
{
    socket_t iL_socket_fd{};
    sockaddr_in address{};
    int mei_port{};
    std::string mes_IP{};
    std::mutex send_mu;
    std::mutex recv_mu;


public:
    Socket();
    ~Socket();
    Socket(const Socket &) = delete;
    Socket &operator=(const Socket &) = delete;

    socket_t mcfn_create(const int &port = -1);
    int mcfn_bind();
    int mcfn_listin(const int &maxCon);
    int mcfn_accept(Socket &client, int timeout_ms = 0);
    int mcfn_accept(std::shared_ptr<Socket> &client, int timeout_ms = 0);
    int mcfn_connect(const std::string &IP = "");
    int mcfn_send(const std::string &buf, size_t size = 0);
    int mcfn_recv(std::string &buf, bool blocking = true, size_t size = 0);
    ssize_t mcfn_sendDir(const std::string &buf, int flags = 0);
    int mcfn_recvDir(std::string &buf, size_t size, int flags = 0);
    ssize_t mcfn_sendDir(char *buffer, size_t size, int flags = 0);
    int mcfn_recvDir(char *buffer, size_t size, int flags = 0);
    int mcfn_recvUntil(std::string &buf, char delimiter, int flags);
    int mcfn_recvAll(std::string &buf);
    int mcfn_reconnect();
    int mcfn_close();
    socket_t mcfn_getSocketfd();
};
#endif