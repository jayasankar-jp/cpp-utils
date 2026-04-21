#include <Socket.h>
#include <string.h>
#include <iostream>
#include <thread>
#define SERVER 0
#define CLINET 1
int mode = SERVER;
int port;
Socket CG_Socket;
Socket CG_Client;
int main(int argc, char **argv)
{
    if (argc == 2 && strcasecmp(argv[1], "-v") == 0)
    {
        std::cout << "Basic version" << std::endl;
        std::cout << "server.exe -> Act as server ./server.exe <port>" << std::endl;
        std::cout << "server.exe -> Act as client ./client.exe <port>" << std::endl;
        return 0;
    }
    if (strcasecmp(argv[0], "./server.exe") == 0)
    {
        std::cout << "Act as a server" << std::endl;
        if (argc < 2)
        {
            std::cout << "./server.exe <port>";
            return 0;
        }
        port = std::atoi(argv[1]);
    }
    else if (strcasecmp(argv[0], "./client.exe") == 0)
    {
        std::cout << "Act as a client" << std::endl;
        mode = CLINET;
        if (argc < 2)
        {
            std::cout << "./client.exe <port>";
            return 0;
        }
        port = std::atoi(argv[1]);
    }
    else
    {
        std::cout << "Wrong configuration rename server.exe or client.exe" << std::endl;
    }
    CG_Socket.mcfn_create(port);
    if (mode == SERVER)
    {
        CG_Socket.mcfn_bind();
        CG_Socket.mcfn_listin(5);
        // while(true){
        if (CG_Socket.mcfn_accept(CG_Client))
        {
            CG_Client.mcfn_send("Helo form server");
            std::string data;
            CG_Client.mcfn_recv(data,false);
            std::cout << data << " getting from client";
        }
        // }
    }
    else
    {
        CG_Socket.mcfn_connect("");
        std::string data;
        CG_Socket.mcfn_recv(data);
        // CG_Socket.mcfn_send("Welcome back");
        std::cout << data << " getting from server";
    }
    return 0;
}