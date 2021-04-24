#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <queue>
#include <chrono>
#include <atomic>

int main()
{
  std::queue<int> production;
  
  std::mutex mtx;
  std::condition_variable cv;

  bool ready = false;  
  bool done = false;   

  std::thread producer(
    [&] () -> void {
      for (int i = 1; i < 10; ++i)
      {
        
        std::this_thread ::sleep_for(std::chrono::milliseconds(10));
        std::cout << "producing " << i << std::endl;

        std::unique_lock<std::mutex> lock(mtx);
        production.push(i);

        
        ready = true;
        cv.notify_one();
      }
      
      done = true;
    }
  );

  std::thread consumer(
    [&] () -> void {
      std::unique_lock<std::mutex> lock(mtx);
      
      while(!done || !production.empty())
      {
        
        while(!ready)
        {
          cv.wait(lock);
        }
        //????????
        while(!production.empty())
        {
          
          std::cout << "consuming " << production.front() << std::endl;
          production.pop();
        }

        
        ready = false;
      }
    }
  );

  producer.join();
  consumer.join();

  return 0;
}


/////////////////////////////
#include <queue>
#include <mutex>
#include <condition_variable>
template<typename T>
class threadsafe_queue
{
private:
    std::mutex mut;
    std::queue<T> data_queue;
    std::condition_variable data_cond;
public:
    void push(T new_value)
    {
        std::lock_guard<std::mutex> lk(mut);
        data_queue.push(new_value);
        data_cond.notify_one();
    }
    void wait_and_pop(T& value)
    {
        std::unique_lock<std::mutex> lk(mut);
        data_cond.wait(lk,[this]{return !data_queue.empty();});
        value=data_queue.front();
        data_queue.pop();
} 
};

threadsafe_queue<data_chunk> data_queue;

void data_preparation_thread()
{
    while(more_data_to_prepare())
    {
        data_chunk const data=prepare_data();
        data_queue.push(data);
   }
}

void data_processing_thread()
{
    while(true)
    {
        data_chunk data;
        data_queue.wait_and_pop(data);
        process(data);
        if(is_last_chunk(data))
        {
            break;
         }
    }
}