#include "Socket.h"
#include "vector"
#include <memory>
int Socket::mcfn_create(const int &port)
{
    try
    {
        iL_socket_fd = socket(AF_INET, SOCK_STREAM, 0);
        address.sin_family = AF_INET;
        address.sin_port = htons(port);
        address.sin_addr.s_addr = INADDR_ANY;
        return 1;
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
int Socket::mcfn_accept(Socket &client)
{

    try
    {

        sockaddr_in receive_addr;

        socklen_t addr_len = sizeof(receive_addr);

        int clientSocket = accept(iL_socket_fd,
                                  (struct sockaddr *)&receive_addr,
                                  &addr_len);
        if (clientSocket < 0)
        {
            std::cerr << "Failed to accept";
            return -1;
        }
        client.address = receive_addr;
        client.iL_socket_fd = clientSocket;
        return 1;
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << '\n';
        return -2;
    }
}
int Socket::mcfn_accept(std::shared_ptr<Socket> &client)
{
    try
    {

        sockaddr_in receive_addr;

        socklen_t addr_len = sizeof(receive_addr);

        int clientSocket = accept(iL_socket_fd,
                                  (struct sockaddr *)&receive_addr,
                                  &addr_len);
        if (clientSocket < 0)
        {
            std::cerr << "Failed to accept";
            return -1;
        }
        client= std::make_shared<Socket>();

         client->address = receive_addr;
         client->iL_socket_fd = clientSocket;
        return 1;
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << '\n';
        return -2;
    }
}
int Socket::mcfn_connect(const std::string &IP)
{
    try
    {
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
// int Socket::mcfn_send(const std::string &buf)
// {
//     try
//     {
//         std::lock_guard<std::mutex> lg(mu);
//         std::string len = std::to_string(buf.length());
//         send(iL_socket_fd, len.c_str(), 4, 0);
//         char ack_buffer[4];
//         int bytes = recv(iL_socket_fd, ack_buffer, 4, 0);
//         if (std::string(ack_buffer) == "OK")
//         {
//             send(iL_socket_fd, buf.c_str(), sizeof(buf.c_str()), 0);
//         }
//         return 1;
//         // send(iL_socket_fd, buff.c_str(), msg.size(), 0)
//     }
//     catch (const std::exception &e)
//     {
//         std::cerr << e.what() << '\n';
//         return -1;
//     }
// }
int Socket::mcfn_send(const std::string &buf)
{
    try
    {
        std::lock_guard<std::mutex> lg(mu);

        // FIXED: always send 4-byte length (padded)
        char len_buffer[4] = {0};
        snprintf(len_buffer, sizeof(len_buffer), "%03d", (int)buf.size());

        send(iL_socket_fd, len_buffer, 4, 0);

        // FIXED: safe ACK read
        char ack_buffer[3] = {0};
        recv(iL_socket_fd, ack_buffer, 2, 0);

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

        return 1;
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << '\n';
        return -1;
    }
}
// int Socket::mcfn_recv(std::string &buf)
// {
//     try
//     {
//         std::lock_guard<std::mutex> lg(mu);
//         char len_buffer[4];
//         int bytes = recv(iL_socket_fd, len_buffer, 4, 0);
//         int length = std::stoi(len_buffer);
//         send(iL_socket_fd, "OK", 4, 0);
//         char recvBuffer[length];
//         bytes = recv(iL_socket_fd, recvBuffer, length, 0);
//         buf = recvBuffer;
//         return bytes;
//     }
//     catch (const std::exception &e)
//     {
//         std::cerr << e.what() << '\n';
//         return -1;
//     }
// }
// int Socket::mcfn_recv(std::string &buf)
// {
//     try
//     {
//         std::lock_guard<std::mutex> lg(mu);

//         // FIXED: receive length safely
//         char len_buffer[5] = {0}; // +1 for null
//         recv(iL_socket_fd, len_buffer, 4, 0);

//         int length = std::stoi(len_buffer);

//         // send ACK properly
//         send(iL_socket_fd, "OK", 2, 0);

//         // FIXED: dynamic buffer
//         std::vector<char> recvBuffer(length);

//         int total = 0;
//         while (total < length)
//         {
//             int bytes = recv(iL_socket_fd,
//                              recvBuffer.data() + total,
//                              length - total,
//                              0);

//             if (bytes <= 0) return -1;
//             total += bytes;
//         }

//         buf.assign(recvBuffer.begin(), recvBuffer.end());
//         return total;
//     }
//     catch (const std::exception &e)
//     {
//         std::cerr << e.what() << '\n';
//         return -1;
//     }
// }

int Socket::mcfn_recv(std::string &buf, bool blocking)
{
    try
    {
        std::lock_guard<std::mutex> lg(mu);

        // 🔹 OPTIONAL: non-blocking behavior using select()
        if (!blocking)
        {
            fd_set fds;
            FD_ZERO(&fds);
            FD_SET(iL_socket_fd, &fds);

            struct timeval tv{};
            tv.tv_sec = 0;
            tv.tv_usec = 0;

            int ret = select(iL_socket_fd + 1, &fds, NULL, NULL, &tv);

            if (ret <= 0)
                return 0; // no data, just return immediately
        }

        // 🔹 Step 1: Read length safely (loop)
        char len_buffer[5] = {0};
        int received = 0;

        while (received < 4)
        {
            int n = recv(iL_socket_fd, len_buffer + received, 4 - received, 0);
            if (n <= 0)
                return -1;
            received += n;
        }

        int length = std::stoi(len_buffer);

        // 🔹 Send ACK
        send(iL_socket_fd, "OK", 2, 0);

        // 🔹 Step 2: Read actual data
        std::vector<char> recvBuffer(length);
        int total = 0;

        while (total < length)
        {
            int bytes = recv(iL_socket_fd,
                             recvBuffer.data() + total,
                             length - total,
                             0);

            if (bytes <= 0)
                return -1;
            total += bytes;
        }

        buf.assign(recvBuffer.begin(), recvBuffer.end());
        return total;
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << '\n';
        return -1;
    }
}
Socket::~Socket()
{
    close(iL_socket_fd);
}