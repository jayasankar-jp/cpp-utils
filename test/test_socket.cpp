#include <Socket.h>
#include <string.h>
#include <iostream>
#include <thread>
#include <Queue.h>
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
            while (true)
            {
                int da = CG_Client.mcfn_recv(data, false);
                if (da > 0)
                {
                    std::cout << data << " getting from client" << std::endl;
                }
                else if (da == -2)
                {
                    std::cout << "Disconnected" << std::endl;
                    break;
                }
            }
            // CG_Client.mcfn_recv(data, true);
            // std::cout << data << " getting from client";
            // CG_Client.mcfn_recv(data, true);
            // std::cout << data << " getting from client";
        }
        // }
    }
    else
    {
        CG_Socket.mcfn_connect("");
        std::string data;
        if (CG_Socket.mcfn_recv(data) == -2)
        {
            std::cout << "DISCONNECT" << std::endl;
            return 0;
        }
        CG_Socket.mcfn_send("Welcome back");

        CG_Socket.mcfn_send("Welcome back 2");
        std::cout << data << " getting from server";
    }
    return 0;
}