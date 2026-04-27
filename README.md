
# cpp-utils

A lightweight cross-platform C++ utility library that provides reusable core components for concurrent and network-based applications.

`cpp-utils` currently includes:

* **Thread-safe Queue** for producer/consumer systems
* **TCP Socket wrapper** for Linux and Windows
* Simple integration with CMake projects

---

# Table of Contents

1. Overview
2. Features
3. Requirements
4. Installation
5. Build from Source
6. Project Structure
7. Queue Module
8. Socket Module
9. Examples
10. Best Practices
11. Limitations
12. Roadmap
13. License

---

# Overview

This library is designed for developers who need lightweight reusable utilities for:

* Backend services
* TCP client/server applications
* Multithreaded programs
* Internal tools
* Embedded Linux applications
* Cross-platform networking projects

The goal of this version is simplicity, portability, and easy integration.

---

# Features

## Queue Utility

* Thread-safe container
* Blocking wait for data
* Non-blocking retrieval
* Supports move semantics
* Graceful shutdown support
* Suitable for worker threads

## Socket Utility

* TCP socket abstraction
* Server and client support
* Linux socket API support
* Windows Winsock support
* Basic send / receive operations
* Accept incoming client connections

---

# Requirements

* C++11 or later
* GCC / Clang / MSVC
* Linux / Windows

Recommended:

* CMake 3.10+

---

# Installation

## Clone Repository

```bash
git clone https://github.com/jayasankar-jp/cpp-utils.git
cd cpp-utils
```

---

# Build from Source

```bash
mkdir build
cd build
cmake ..
make
```

---

# Include in Your Project

```cpp
#include <Queue.h>
#include <Socket.h>
```

If required:

```bash
-IIncludes
```

---

# Project Structure

```txt
cpp-utils/
├── Includes/
│   ├── Queue.h
│   └── Socket.h
├── src/
│   └── Socket.cpp
├── test/
│   └── test_socket.cpp
├── CMakeLists.txt
├── README.md
└── LICENSE
```

---

# Queue Module

## Overview

A generic thread-safe queue implemented using templates.

Useful for:

* Producer / consumer systems
* Logging pipelines
* Job dispatchers
* Worker threads
* Message passing

---

## Basic Usage

```cpp
Queue<int> q;
q.insert(10);
```

---

## Insert Copy

```cpp
int value = 25;
q.insert(value);
```

---

## Insert Move

```cpp
std::string msg = "hello";
Queue<std::string> q;
q.insert(std::move(msg));
```

---

## Blocking Read

```cpp
int data;
q.getElement(data);
```

Waits until data is available or queue is shutdown.

---

## Non-blocking Read

```cpp
int data;
if(q.getElement_no_wait(data))
{
    // success
}
```

---

## Shutdown Queue

```cpp
q.close();
```

All waiting threads will be released.

---

## Queue Example with Threads

```cpp
Queue<int> q;

std::thread producer([&]() {
    for(int i=0;i<5;i++)
        q.insert(i);
});

std::thread consumer([&]() {
    int value;
    while(q.getElement(value))
        std::cout << value << std::endl;
});

producer.join();
q.close();
consumer.join();
```

---

# Socket Module

## Overview

A lightweight TCP socket wrapper for client/server communication.

Supports:

* Create socket
* Bind port
* Listen for clients
* Accept connections
* Connect to remote server
* Send data
* Receive data

---

# Server Example

```cpp
Socket server;
Socket client;

server.mcfn_create(8080);
server.mcfn_bind();
server.mcfn_listin(5);

server.mcfn_accept(client);
```

---

# Client Example

```cpp
Socket client;

client.mcfn_create(8080);
client.mcfn_connect("127.0.0.1");
```

---

# Send Data

```cpp
client.mcfn_send("Hello Server");
```

---

# Receive Data

```cpp
char buffer[1024];
client.mcfn_recv(buffer, sizeof(buffer));
```

---

# Full Echo Server Example

```cpp
Socket server, client;

server.mcfn_create(8080);
server.mcfn_bind();
server.mcfn_listin(5);

server.mcfn_accept(client);

char buffer[1024];
client.mcfn_recv(buffer, sizeof(buffer));
client.mcfn_send(buffer);
```

---

# Full Echo Client Example

```cpp
Socket client;

client.mcfn_create(8080);
client.mcfn_connect("127.0.0.1");

client.mcfn_send("hello");

char buffer[1024];
client.mcfn_recv(buffer, sizeof(buffer));
```

---

# Best Practices

## Queue

* Always call `close()` before program shutdown when threads are waiting
* Prefer move insert for large objects
* Use dedicated worker threads

## Socket

* Check return values after every call
* Validate received data size
* Use timeout logic for production systems
* Close sockets after use

---

# Limitations in v1.0.0

* TCP only
* No UDP support
* No SSL/TLS
* No async I/O
* No advanced error objects
* No namespace wrapping
* Naming uses legacy method style

---

# Roadmap

## Planned Improvements

* Cleaner modern API names
* RAII socket cleanup
* UDP support
* TLS support
* Thread pool utility
* Logger module
* Timer utilities
* Config parser
* Better CMake package install

---

# Example Use Cases

* Chat server prototypes
* Internal tools
* Device communication systems
* Logging pipelines
* Worker thread queues
* Simple backend services

---

# Version

Current stable release:

**v1.0.0**

---

# Contributing

Contributions, bug reports, and feature requests are welcome.

GitHub Repository:

https://github.com/jayasankar-jp/cpp-utils

---

# License

MIT License

---

# Author

Created by Jayasankar JP
