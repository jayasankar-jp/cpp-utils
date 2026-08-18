#include <iostream>
#include <thread>
#include <vector>
#include <cassert>
#include <chrono>
#include <string>
#include <atomic>
#include <Queue.h>


void test_basic_ops()
{
    std::cout << "[TEST] Running test_basic_ops..." << std::endl;
    Queue<int> q;

    assert(q.getCount() == 0);

    q.insert(10);
    q.insert(20);
    q.insert(30);

    assert(q.getCount() == 3);

    int topVal = 0;
    assert(q.getTop(topVal) == true);
    assert(topVal == 10);

    int val = 0;
    assert(q.getElement(val, 0) == true);
    assert(val == 10);

    assert(q.getElement(val, 0) == true);
    assert(val == 20);

    assert(q.getElement(val, 0) == true);
    assert(val == 30);

    assert(q.getCount() == 0);
    std::cout << "[PASS] test_basic_ops completed." << std::endl;
}

void test_non_blocking_and_timeout()
{
    std::cout << "[TEST] Running test_non_blocking_and_timeout..." << std::endl;
    Queue<std::string> q;

    std::string data;
    // Non-blocking pop on empty queue should return false immediately
    assert(q.getElement(data, 1) == false);

    // Timed pop on empty queue with 50ms wait should timeout and return false
    auto start = std::chrono::high_resolution_clock::now();
    assert(q.getElement(data, 50) == false);
    auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::high_resolution_clock::now() - start
    ).count();
    assert(elapsed >= 40); // should wait around 50ms

    q.insert(std::string("Hello World"));
    assert(q.getElement(data, 1) == true);
    assert(data == "Hello World");

    std::cout << "[PASS] test_non_blocking_and_timeout completed." << std::endl;
}

void test_multithreaded_producer_consumer()
{
    std::cout << "[TEST] Running test_multithreaded_producer_consumer..." << std::endl;
    Queue<int> q;

    const int NUM_PRODUCERS = 4;
    const int NUM_CONSUMERS = 4;
    const int ITEMS_PER_PRODUCER = 1000;

    std::vector<std::thread> producers;
    std::vector<std::thread> consumers;
    std::atomic<int> total_consumed{0};

    // Spawn producers
    for (int p = 0; p < NUM_PRODUCERS; ++p)
    {
        producers.emplace_back([&q, p]() {
            for (int i = 0; i < ITEMS_PER_PRODUCER; ++i)
            {
                q.insert(p * ITEMS_PER_PRODUCER + i);
            }
        });
    }

    // Spawn consumers
    for (int c = 0; c < NUM_CONSUMERS; ++c)
    {
        consumers.emplace_back([&q, &total_consumed]() {
            int val = 0;
            while (q.getElement(val, 0))
            {
                total_consumed.fetch_add(1, std::memory_order_relaxed);
            }
        });
    }

    // Join producers
    for (auto &t : producers)
        t.join();

    // Close queue to unblock waiting consumers
    q.close();

    // Join consumers
    for (auto &t : consumers)
        t.join();

    assert(total_consumed == NUM_PRODUCERS * ITEMS_PER_PRODUCER);
    assert(q.getCount() == 0);

    std::cout << "[PASS] test_multithreaded_producer_consumer completed." << std::endl;
}

int main()
{
    std::cout << "========================================" << std::endl;
    std::cout << "         QUEUE UNIT & LOAD TESTS        " << std::endl;
    std::cout << "========================================" << std::endl;

    test_basic_ops();
    test_non_blocking_and_timeout();
    test_multithreaded_producer_consumer();

    std::cout << "========================================" << std::endl;
    std::cout << "       ALL QUEUE TESTS PASSED!          " << std::endl;
    std::cout << "========================================" << std::endl;
    return 0;
}
