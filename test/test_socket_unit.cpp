#include <iostream>
#include <thread>
#include <cassert>
#include <chrono>
#include <string>
#include <Socket.h>

void test_socket_lifecycle()
{
    std::cout << "[TEST] Running test_socket_lifecycle..." << std::endl;
    Socket server;
    assert(server.mcfn_create(9898) != INVALID_SOCKET);
    assert(server.mcfn_bind() == 1);
    assert(server.mcfn_listin(5) == 1);

    Socket client;
    assert(client.mcfn_create(9898) != INVALID_SOCKET);


    std::thread server_thread([&server]() {
        Socket peer;
        if (server.mcfn_accept(peer, 1000) == 1)
        {
            std::string msg;
            if (peer.mcfn_recv(msg, true) > 0)
            {
                peer.mcfn_send("PONG:" + msg);
            }
            peer.mcfn_close();
        }
    });

    std::this_thread::sleep_for(std::chrono::milliseconds(50));
    assert(client.mcfn_connect("127.0.0.1") == 1);

    assert(client.mcfn_send("PING") == 1);

    std::string response;
    assert(client.mcfn_recv(response, true) > 0);
    assert(response == "PONG:PING");

    client.mcfn_close();
    server.mcfn_close();
    server_thread.join();

    std::cout << "[PASS] test_socket_lifecycle completed." << std::endl;
}

int main()
{
    std::cout << "========================================" << std::endl;
    std::cout << "         SOCKET UNIT & AUDIT TESTS      " << std::endl;
    std::cout << "========================================" << std::endl;

    test_socket_lifecycle();

    std::cout << "========================================" << std::endl;
    std::cout << "       ALL SOCKET TESTS PASSED!         " << std::endl;
    std::cout << "========================================" << std::endl;
    return 0;
}
