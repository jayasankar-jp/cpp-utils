#include "Socket.h"
#include "vector"
#include <memory>

#ifdef _WIN32

#include <winsock2.h>
#include <windows.h>
// #include <winsock2.h>
#pragma comment(lib, "ws2_32.lib")

#else

#include <poll.h>
#include <unistd.h>
#include <fcntl.h>

#endif
#ifdef _WIN32
#define CLOSE_SOCKET closesocket
#else
#define CLOSE_SOCKET close
#endif
#ifndef _WIN32
#include <errno.h>
#endif
Socket::Socket() : iL_socket_fd(INVALID_SOCKET) {};
socket_t Socket::mcfn_create(const int &port)
{

    try
    {
        if (port != -1)
        {
            mei_port = port;
        }
#ifdef _WIN32
        WSADATA wsa;
        WSAStartup(MAKEWORD(2, 2), &wsa);
#endif
        iL_socket_fd = socket(AF_INET, SOCK_STREAM, 0);

#ifdef _WIN32

        SetHandleInformation((HANDLE)serverSocket,
                             HANDLE_FLAG_INHERIT,
                             0);

#else

        fcntl(iL_socket_fd, F_SETFD, FD_CLOEXEC);

#endif
        address.sin_family = AF_INET;
        address.sin_port = htons(mei_port);
        address.sin_addr.s_addr = INADDR_ANY;
        return iL_socket_fd;
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << '\n';
    }
    return -1;
}
int Socket::mcfn_bind()
{
    try
    {
        if (bind(iL_socket_fd, (struct sockaddr *)&address,
                 sizeof(address)) == 0)
        {
            std::cout << "successfully bind" << std::endl;
            return 1;
        }
        else
        {
            std::cerr << "bind filed" << std::endl;
            return 0;
        }
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << '\n';
    }
    return -1;
}
int Socket::mcfn_listin(const int &maxCon)
{
    try
    {
        return listen(iL_socket_fd, maxCon) == 0 ? 1 : 0;
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << '\n';
    }
    return -1;
}
// int Socket::mcfn_accept(Socket &client)
// {
//     try
//     {

//         sockaddr_in receive_addr;

//         socklen_t addr_len = sizeof(receive_addr);

//         int clientSocket = accept(iL_socket_fd,
//                                   (struct sockaddr *)&receive_addr,
//                                   &addr_len);
//         if (clientSocket < 0)
//         {
//             std::cerr << "Failed to accept";
//             return -1;
//         }
//         client.address = receive_addr;
//         client.iL_socket_fd = clientSocket;
//         return 1;
//     }
//     catch (const std::exception &e)
//     {
//         std::cerr << e.what() << '\n';
//         return -2;
//     }
// }
// int Socket::mcfn_accept(std::shared_ptr<Socket> &client)
// {
//     try
//     {

//         sockaddr_in receive_addr;

//         socklen_t addr_len = sizeof(receive_addr);

//         int clientSocket = accept(iL_socket_fd,
//                                   (struct sockaddr *)&receive_addr,
//                                   &addr_len);
//         if (clientSocket < 0)
//         {
//             std::cerr << "Failed to accept";
//             return -1;
//         }
//         client = std::make_shared<Socket>();

//         client->address = receive_addr;
//         client->iL_socket_fd = clientSocket;
//         return 1;
//     }
//     catch (const std::exception &e)
//     {
//         std::cerr << e.what() << '\n';
//         return -2;
//     }
// }

int Socket::mcfn_accept(Socket &client, int timeout_ms)
{
    try
    {
        // timeout support
        if (timeout_ms > 0)
        {
#ifdef _WIN32

            fd_set readfds;
            FD_ZERO(&readfds);
            FD_SET(iL_socket_fd, &readfds);

            timeval tv;
            tv.tv_sec = timeout_ms / 1000;
            tv.tv_usec = (timeout_ms % 1000) * 1000;

            int ret = select(
                0,
                &readfds,
                nullptr,
                nullptr,
                &tv);

#else

            struct pollfd pfd;
            pfd.fd = iL_socket_fd;
            pfd.events = POLLIN;

            int ret = poll(&pfd, 1, timeout_ms);

#endif

            // timeout
            if (ret == 0)
            {
                return 0;
            }

            // select/poll error
            if (ret < 0)
            {
                return -1;
            }
        }

        sockaddr_in receive_addr;
        socklen_t addr_len = sizeof(receive_addr);

#ifdef _WIN32
        SOCKET clientSocket = accept(
#else
        int clientSocket = accept(
#endif
            iL_socket_fd,
            (struct sockaddr *)&receive_addr,
            &addr_len);

#ifdef _WIN32
        if (clientSocket == INVALID_SOCKET)
#else
        if (clientSocket < 0)
#endif
        {
            return -2;
        }

        client.address = receive_addr;
        client.iL_socket_fd = clientSocket;

        return 1;
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << '\n';
        return -3;
    }
}
int Socket::mcfn_accept(
    std::shared_ptr<Socket> &client,
    int timeout_ms )
{
    try
    {
        // timeout support
        if (timeout_ms > 0)
        {
#ifdef _WIN32

            fd_set readfds;
            FD_ZERO(&readfds);
            FD_SET(iL_socket_fd, &readfds);

            timeval tv;
            tv.tv_sec = timeout_ms / 1000;
            tv.tv_usec = (timeout_ms % 1000) * 1000;

            int ret = select(
                0,
                &readfds,
                nullptr,
                nullptr,
                &tv);

#else

            struct pollfd pfd;
            pfd.fd = iL_socket_fd;
            pfd.events = POLLIN;

            int ret = poll(&pfd, 1, timeout_ms);

#endif

            // timeout
            if (ret == 0)
            {
                return 0;
            }

            // select/poll error
            if (ret < 0)
            {
                return -1;
            }
        }

        sockaddr_in receive_addr;
        socklen_t addr_len = sizeof(receive_addr);

#ifdef _WIN32
        SOCKET clientSocket = accept(
#else
        int clientSocket = accept(
#endif
            iL_socket_fd,
            (struct sockaddr *)&receive_addr,
            &addr_len);

#ifdef _WIN32
        if (clientSocket == INVALID_SOCKET)
#else
        if (clientSocket < 0)
#endif
        {
            return -2;
        }

        client = std::make_shared<Socket>();

        client->address = receive_addr;
        client->iL_socket_fd = clientSocket;

        return 1;
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << '\n';
        return -3;
    }
}
int Socket::mcfn_connect(const std::string &IP)
{
    try
    {
        mes_IP = IP;
        if (IP != "")
        {
            if (inet_pton(AF_INET, IP.c_str(), &address.sin_addr) <= 0)
            {
                perror("Invalid address");
                return 0;
            }
        }
        if (connect(iL_socket_fd, (struct sockaddr *)&address, sizeof(address)) < 0)
        {
            perror("Connection failed");
            return 0;
        }
        return 1;
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << '\n';
        return -1;
    }
}
int Socket::mcfn_reconnect()
{
    try
    {
        // mes_IP = IP;
        if (mes_IP != "")
        {
            if (inet_pton(AF_INET, mes_IP.c_str(), &address.sin_addr) <= 0)
            {
                perror("Invalid address");
                return 0;
            }
        }
        if (connect(iL_socket_fd, (struct sockaddr *)&address, sizeof(address)) < 0)
        {
            perror("Connection failed");
            return 0;
        }
        return 1;
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << '\n';
        return -1;
    }
}

int Socket::mcfn_close()
{
    try
    {
        std::lock_guard<std::mutex> lg(mu);
        if (iL_socket_fd != INVALID_SOCKET)
        {
            CLOSE_SOCKET(iL_socket_fd);
            iL_socket_fd = INVALID_SOCKET;
        }
        return 1;
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << '\n';
        return -1;
    }
}
int Socket::mcfn_send(const std::string &buf, size_t size)
{
    try
    {
        std::lock_guard<std::mutex> lg(mu);
        if (size > 0)
        {
            // char buffer[10000];
            send(iL_socket_fd, buf.c_str(), size, 0);
        }
        else
        {
            // FIXED: always send 4-byte length (padded)
            char len_buffer[4] = {0};
            snprintf(len_buffer, sizeof(len_buffer), "%03d", (int)buf.size());

            int res = send(iL_socket_fd, len_buffer, 4, 0);
            if (res <= 0)
                return -1;
            // FIXED: safe ACK read
            char ack_buffer[3] = {0};
            int rec_ack = recv(iL_socket_fd, ack_buffer, 2, 0);
            if (rec_ack <= 0)
                return -1;
            if (std::string(ack_buffer) == "OK")
            {
                // FIXED: correct size + loop
                int total = 0;
                int len = buf.size();

                while (total < len)
                {
                    int sent = send(iL_socket_fd,
                                    buf.c_str() + total,
                                    len - total,
                                    0);
                    if (sent <= 0)
                        return -1;
                    total += sent;
                }
            }
        }

        return 1;
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << '\n';
        return -1;
    }
}
ssize_t Socket::mcfn_sendDir(char *buffer, size_t size, int flags)
{
    return send(iL_socket_fd, buffer, size, flags);
}
int Socket::mcfn_recvDir(char *buffer, size_t size, int flags)
{
    return recv(iL_socket_fd, buffer, size, flags);
}
ssize_t Socket::mcfn_sendDir(const std::string &buf, int flags)
{
    return send(iL_socket_fd, buf.data(), buf.size(), flags);
}
int Socket::mcfn_recvDir(std::string &buf, size_t size, int flags)
{
    std::vector<char> buffer(size);

    int res = recv(iL_socket_fd, buffer.data(), size, flags);

    if (res > 0)
    {
        buf.assign(buffer.data(), res);
    }
    else
    {
        buf.clear();
    }

    return res;
}
int Socket::mcfn_recvUntil(std::string &buf, char delimiter, int flags)
{
    buf.clear();

    char ch;

    while (true)
    {
        int res = recv(iL_socket_fd, &ch, 1, flags);

        if (res <= 0)
        {
            return res;
        }

        buf.push_back(ch);

        if (ch == delimiter)
        {
            break;
        }
    }

    return buf.size();
}
int Socket::mcfn_recvAll(std::string &buf)
{
    buf.clear();

    char temp[4096];

    while (true)
    {
        int res = recv(iL_socket_fd, temp, sizeof(temp), MSG_DONTWAIT);

        if (res > 0)
        {
            buf.append(temp, res);
        }
        else if (res == 0)
        {
            return 0;
        }
        else
        {
            if (errno == EAGAIN || errno == EWOULDBLOCK)
            {
                break;
            }

            return -1;
        }
    }

    return buf.size();
}
int Socket::mcfn_recv(std::string &buf, bool blocking, size_t size)
{
    std::lock_guard<std::mutex> lg(mu);

    buf.clear();

    // =====================================================
    // RAW RECEIVE MODE
    // =====================================================
    if (size > 0)
    {
        std::vector<char> buff(size);

        int res = recv(iL_socket_fd,
                       buff.data(),
                       (int)size,
                       0);

        if (res > 0)
            buf.assign(buff.data(), res);
        else if (res == 0)
            return -2; // disconnected
        else
        {
#ifdef _WIN32
            int err = WSAGetLastError();

            if (!blocking && err == WSAEWOULDBLOCK)
                return 0;

            return -1;
#else
            if (errno == EINTR)
                return mcfn_recv(buf, blocking, size);

            if (!blocking &&
                (errno == EAGAIN || errno == EWOULDBLOCK))
                return 0;

            return -1;
#endif
        }

        return res;
    }

    // =====================================================
    // PACKET MODE (4 BYTE HEADER + PAYLOAD)
    // =====================================================

    // Non-blocking pre-check
    if (!blocking)
    {
        fd_set readfds;
        FD_ZERO(&readfds);
        FD_SET(iL_socket_fd, &readfds);

        timeval tv{};
        tv.tv_sec = 0;
        tv.tv_usec = 0;

        int ret = select(iL_socket_fd + 1,
                         &readfds,
                         nullptr,
                         nullptr,
                         &tv);

        if (ret == 0)
            return 0; // no data

        if (ret < 0)
            return -1;
    }

    // -------------------------------------------------
    // Receive 4-byte header
    // -------------------------------------------------
    char len_buffer[5] = {0};
    int received = 0;

    while (received < 4)
    {
        int n = recv(iL_socket_fd,
                     len_buffer + received,
                     4 - received,
                     0);

        if (n > 0)
        {
            received += n;
        }
        else if (n == 0)
        {
            return -2; // disconnected
        }
        else
        {
#ifdef _WIN32
            int err = WSAGetLastError();

            if (!blocking && err == WSAEWOULDBLOCK)
                return 0;

            return -1;
#else
            if (errno == EINTR)
                continue;

            if (!blocking &&
                (errno == EAGAIN || errno == EWOULDBLOCK))
                return 0;

            return -1;
#endif
        }
    }

    // -------------------------------------------------
    // Parse length
    // -------------------------------------------------
    int length = 0;

    try
    {
        length = std::stoi(len_buffer);
    }
    catch (...)
    {
        return -3;
    }

    if (length <= 0)
        return -3;

    // -------------------------------------------------
    // ACK
    // -------------------------------------------------
    int ack = send(iL_socket_fd, "OK", 2, 0);

    if (ack == 0)
        return -2;

    if (ack < 0)
    {
#ifdef _WIN32
        int err = WSAGetLastError();

        if (!blocking && err == WSAEWOULDBLOCK)
            return 0;

        return -1;
#else
        if (!blocking &&
            (errno == EAGAIN || errno == EWOULDBLOCK))
            return 0;

        return -1;
#endif
    }

    // -------------------------------------------------
    // Receive payload
    // -------------------------------------------------
    std::vector<char> recvBuffer(length);
    int total = 0;

    while (total < length)
    {
        int n = recv(iL_socket_fd,
                     recvBuffer.data() + total,
                     length - total,
                     0);

        if (n > 0)
        {
            total += n;
        }
        else if (n == 0)
        {
            return -2; // disconnected
        }
        else
        {
#ifdef _WIN32
            int err = WSAGetLastError();

            if (!blocking && err == WSAEWOULDBLOCK)
                return 0;

            return -1;
#else
            if (errno == EINTR)
                continue;

            if (!blocking &&
                (errno == EAGAIN || errno == EWOULDBLOCK))
                return 0;

            return -1;
#endif
        }
    }

    buf.assign(recvBuffer.begin(), recvBuffer.end());

    return total;
}
socket_t Socket::mcfn_getSocketfd()
{
    return iL_socket_fd;
}
Socket::~Socket()
{

    if (iL_socket_fd != INVALID_SOCKET)
    {
        CLOSE_SOCKET(iL_socket_fd);
        iL_socket_fd = INVALID_SOCKET;
    }

#ifdef _WIN32
    WSACleanup();
#endif
}