#ifndef __MY_LIB_QUEUE__
#define __MY_LIB_QUEUE__
#include <iostream>
#include <list>
#include <mutex>
#include <condition_variable>
template <typename T>
class Queue
{
    std::mutex mu;
    std::condition_variable cv;
    std::list<T> data;
    int size=0;
    bool b_shutdown = false;

public:
    ~Queue(){
        {
            std::lock_guard<std::mutex> lg(mu);
            b_shutdown = true;
        }
        cv.notify_all();
    }
    bool insert(const T &data)
    {
        try
        {
            {
                std::lock_guard<std::mutex> lg(mu);
                this->data.push_back(data);
                size++;
            }
            cv.notify_one();
            return true;
        }
        catch (const std::bad_alloc &e)
        {
           return false;
        }
        return true;
    }
    bool getTop( T &data) {
         std::lock_guard<std::mutex> lg(mu);
         if(this->data.empty() ){
            return false;
         }
         data=this->data.front();
         return true;
    }
    int getCount()  {
       std::lock_guard <std::mutex> lg(mu);
       return size;
    }
    bool getElement(T & data, bool isWait=true) {
        if(isWait){
            std::unique_lock<std::mutex> ul(mu);
            cv.wait(ul,[this]{
                return !this->data.empty() || b_shutdown;
            });
            data=this->data.front();
            this->data.pop_front();
            size--;
            return true;
        }else{
            std::lock_guard<std::mutex> lg(mu);
            if(this->data.empty()){
                return false;
            }
            data=this->data.front();
            this->data.pop_front();
            size--;
            return true;
        }
    }  
    
};

#endif