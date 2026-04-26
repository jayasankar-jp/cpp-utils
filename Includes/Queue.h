#ifndef __MY_LIB_QUEUE__
#define __MY_LIB_QUEUE__
#include <iostream>
#include <list>
#include <mutex>
#include <condition_variable>
#include <atomic>
template <typename T>
class Queue
{
    std::mutex mu;
    std::condition_variable cv;
    std::list<T> data;

    // std::atomic<bool> b_shutdown{false};
    bool b_shutdown = false;

public:
    void close()
    {
        {
            std::lock_guard<std::mutex> lg(mu);
            b_shutdown = true;
        }
        cv.notify_all();
    }

    ~Queue()
    {
        close();
    }
    bool insert(const T &cl_data)
    {
        try
        {

            {
                std::lock_guard<std::mutex> lg(mu);
                if (b_shutdown)
                    return false;

                data.push_back(std::move(cl_data));
            }
            cv.notify_one();
            return true;
        }
        catch (...)
        {
            return false;
        }
        return true;
    }
    bool getTop(T &cl_data)
    {
        std::lock_guard<std::mutex> lg(mu);
        if (data.empty())
        {
            return false;
        }
        cl_data = data.front();
        return true;
    }
    size_t getCount()
    {
        std::lock_guard<std::mutex> lg(mu);
        return data.size();
    }
    bool getElement(T &cl_data, bool isWait = true)
    {
        if (isWait)
        {
            std::unique_lock<std::mutex> ul(mu);
            cv.wait(ul, [this]
                    { return !data.empty() || b_shutdown; });
            if (data.empty())
                return false;
            cl_data = std::move(data.front());
            data.pop_front();

            return true;
        }
        else
        {
            std::lock_guard<std::mutex> lg(mu);
            if (data.empty())
            {
                return false;
            }
            cl_data = std::move(data.front());
            data.pop_front();
            return true;
        }
    }
    Queue() = default;
    Queue(const Queue &) = delete;
    Queue &operator=(const Queue &) = delete;
};

#endif