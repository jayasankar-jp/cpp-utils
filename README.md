
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

## Option A: Quick One-Liner Commands (`wget` / `curl`)

### Quick Download & Build on Linux / Raspberry Pi:
```bash
# Download source tarball via wget, extract & build in one command
wget -qO- https://github.com/jayasankar-jp/cpp-utils/archive/refs/heads/main.tar.gz | tar -xz && \
cd cpp-utils-main && \
cmake -B build -DCMAKE_BUILD_TYPE=Release && \
cmake --build build -j$(nproc) && \
sudo cmake --install build
```

### Quick Download via `curl`:
```bash
curl -L https://github.com/jayasankar-jp/cpp-utils/archive/refs/heads/main.tar.gz | tar -xz
cd cpp-utils-main
cmake -B build && cmake --build build
```

---

## Option B: Download Pre-built Packages (GitHub Actions)

Pre-built binaries, headers, and CMake config packages for **Linux distributions**, **Raspberry Pi**, and **Windows** are automatically archived on every commit/release with version and OS-based names:

| Target Platform / Architecture | Package Archive Name | Format |
| :--- | :--- | :--- |
| **Ubuntu 22.04 (x86_64)** | `cpp-utils-v1.0.0-ubuntu-22.04-x86_64.tar.gz` | Tarball |
| **Ubuntu 24.04 (x86_64)** | `cpp-utils-v1.0.0-ubuntu-24.04-x86_64.tar.gz` | Tarball |
| **Debian Latest (x86_64)** | `cpp-utils-v1.0.0-debian-latest-x86_64.tar.gz` | Tarball |
| **Fedora Latest (x86_64)** | `cpp-utils-v1.0.0-fedora-latest-x86_64.tar.gz` | Tarball |
| **Alpine Linux (musl)** | `cpp-utils-v1.0.0-alpine-latest-x86_64.tar.gz` | Tarball |
| **Arch Linux (x86_64)** | `cpp-utils-v1.0.0-archlinux-latest-x86_64.tar.gz` | Tarball |
| **Raspberry Pi 32-bit (`armhf`)** | `cpp-utils-v1.0.0-raspberrypi-armv7.tar.gz` | Tarball |
| **Raspberry Pi 64-bit (`aarch64`)** | `cpp-utils-v1.0.0-raspberrypi-aarch64.tar.gz` | Tarball |
| **Windows MSVC (x64)** | `cpp-utils-v1.0.0-windows-latest-Release.zip` | Zip |

### How to Install a Pre-built Package:

```bash
# Example: Extracting pre-built Linux tarball system-wide
sudo tar -xzvf cpp-utils-v1.0.0-ubuntu-22.04-x86_64.tar.gz -C /usr/local
```

---


## Option B: Build & Install from Source

### 1. Linux Installation (Ubuntu, Debian, Fedora, Alpine, Arch)

```bash
# Install build prerequisites (Ubuntu/Debian example)
sudo apt update && sudo apt install -y cmake build-essential

# Clone repository
git clone https://github.com/jayasankar-jp/cpp-utils.git
cd cpp-utils

# Configure & Build
cmake -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build -j$(nproc)

# Run tests
ctest --test-dir build --output-on-failure

# Install system-wide (Installs to /usr/local/include & /usr/local/lib)
sudo cmake --install build
```

---

### 2. Raspberry Pi Installation (Raspberry Pi OS / ARM32 & ARM64)

```bash
# Install build tools on Raspberry Pi OS
sudo apt update && sudo apt install -y cmake g++ make

# Clone repository
git clone https://github.com/jayasankar-jp/cpp-utils.git
cd cpp-utils

# Configure & Build
cmake -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build -j$(nproc)

# Run tests
ctest --test-dir build --output-on-failure

# Install system-wide on Raspberry Pi
sudo cmake --install build
```

---

### 3. Windows Installation (MSVC / Visual Studio or MinGW)

#### Method 1: Using Command Prompt / PowerShell (MSVC)

```cmd
:: Open Developer Command Prompt for Visual Studio
git clone https://github.com/jayasankar-jp/cpp-utils.git
cd cpp-utils

:: Configure
cmake -B build -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX="C:/Program Files/internalLib"

:: Build
cmake --build build --config Release

:: Run tests
ctest --test-dir build -C Release --output-on-failure

:: Install (Run Command Prompt as Administrator)
cmake --install build --config Release
```

#### Method 2: Custom Directory (No Admin required)

```cmd
cmake -B build -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX="C:/libs/cpp-utils"
cmake --build build --config Release
cmake --install build --config Release
```

---

### Installed Files Structure

When installed, the following files are placed in your system or installation prefix (`<prefix>`):

```txt
<prefix>/
├── include/
│   ├── Queue.h                  # Thread-safe queue header
│   └── Socket.h                 # Socket wrapper header
├── lib/                         # (or lib64 on 64-bit distros)
│   ├── libinternalLib.a         # Static library (Linux / Raspberry Pi)
│   ├── libinternalLib.so        # Shared library (Linux / Raspberry Pi)
│   ├── internalLib.lib          # Static / Import library (Windows MSVC)
│   └── cmake/
│       └── internalLib/
│           ├── internalLibConfig.cmake         # Package config for find_package()
│           ├── internalLibConfigVersion.cmake  # Package version file
│           └── internalLibTargets.cmake        # Target definitions
└── bin/                         # (Windows DLL directory)
    └── internalLib.dll          # Shared library DLL (Windows, if enabled)
```

---

## Integration in Modern CMake Projects


After installing `cpp-utils` (`internalLib`), you can easily consume it in any CMake project:

### `CMakeLists.txt`

```cmake
cmake_minimum_required(VERSION 3.16)
project(MyApplication CXX)

set(CMAKE_CXX_STANDARD 17)

# Find the installed internalLib package
find_package(internalLib REQUIRED)

add_executable(my_app main.cpp)

# Link internalLib target (automatically sets includes & libraries like Threads / Winsock)
target_link_libraries(my_app PRIVATE internalLib::internalLib)
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
