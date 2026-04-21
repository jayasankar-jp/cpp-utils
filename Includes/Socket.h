#ifndef __SOCKET_LIB_H__
#define __SOCKET_LIB_H__
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <iostream>
#include <memory>
#include <mutex>
    class Socket{
        int iL_socket_fd;
        sockaddr_in address ;
        std::mutex mu;
        public :
        ~Socket();
        int mcfn_create(const int & port);
        int mcfn_bind();
        int mcfn_listin(const int & maxCon);
        int mcfn_accept(Socket &client);
        int mcfn_accept(std::shared_ptr< Socket> &client);
        int mcfn_connect(const std::string & IP="");
        int mcfn_send(const std::string &buf);
        int mcfn_recv( std::string &buf,bool blocking=true);
        
    };
#endif 