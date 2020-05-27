#ifndef THREAD_POOL_H
#define THREAD_POOL_H

#include <vector>
#include <queue>
#include <memory>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <future>
#include <functional>
#include <stdexcept>



class ThreadPool {
public:
    ThreadPool(size_t,string);
    std::size_t TaskSize();
    void ClearTask();
    std::vector<thread::id>GetThreadIds();
    template<class F, class... Args>
    auto enqueue(F&& f, Args&&... args) 
        -> std::future<typename std::result_of<F(Args...)>::type>;
    ~ThreadPool();
private:
    // need to keep track of threads so we can join them
    void threadfun(string poolname,int32_t index);
    std::vector< std::thread > workers;
    std::vector<thread::id> _threadidsVec;
    // the task queue
    std::queue< std::function<void()> > tasks;
    
    // synchronization
    std::mutex queue_mutex;
    std::condition_variable condition;
    bool stop;
};
 
// the constructor just launches some amount of workers
inline ThreadPool::ThreadPool(size_t threads,string poolname)
    :   stop(false)
{
    // for(size_t i = 0;i<threads;++i)
    //     workers.emplace_back(
    //         [this,poolname,i]
    //         {
    //             auto threadname = poolname +"_"+to_string(i);
    //             pthread_setname_np(pthread_self(), threadname.c_str());
    //             for(;;)
    //             {
    //                 std::function<void()> task;

    //                 {
    //                     std::unique_lock<std::mutex> lock(this->queue_mutex);
    //                     this->condition.wait(lock,
    //                         [this]{ return this->stop || !this->tasks.empty(); });
    //                     if(this->stop && this->tasks.empty())
    //                         return;
    //                     task = std::move(this->tasks.front());
    //                     this->tasks.pop();
    //                 }

    //                 task();
    //             }
    //         }
    //     );


    for(auto i = 0; i< threads;i++){
        std::thread f(&ThreadPool::threadfun,this,poolname,i);
        this->_threadidsVec.push_back(f.get_id());
        this->workers.push_back(std::move(f));
    }
}


inline void ThreadPool::threadfun(string poolname,int32_t index){
    auto threadname = poolname +"_"+to_string(index);
    pthread_setname_np(pthread_self(), threadname.c_str());
    for(;;)
    {
        std::function<void()> task;
        {
            std::unique_lock<std::mutex> lock(this->queue_mutex);
            this->condition.wait(lock,
                [this]{ return this->stop || !this->tasks.empty(); });
            if(this->stop && this->tasks.empty())
                return;
            task = std::move(this->tasks.front());
            this->tasks.pop();
        }
        task();
    }    
    return;
}

inline std::size_t ThreadPool::TaskSize(){
    std::unique_lock<std::mutex> lock(queue_mutex);
    return this->tasks.size();
}

inline void ThreadPool::ClearTask(){
    std::unique_lock<std::mutex> lock(queue_mutex);
    while (0 != this->tasks.size()){
        this->tasks.pop();
    }
    
}

inline std::vector<thread::id> ThreadPool::GetThreadIds(){
    return this->_threadidsVec;
}
// add new work item to the pool
template<class F, class... Args>
auto ThreadPool::enqueue(F&& f, Args&&... args) 
    -> std::future<typename std::result_of<F(Args...)>::type>
{
    using return_type = typename std::result_of<F(Args...)>::type;

    auto task = std::make_shared< std::packaged_task<return_type()> >(
            std::bind(std::forward<F>(f), std::forward<Args>(args)...)
        );
        
    std::future<return_type> res = task->get_future();
    {
        std::unique_lock<std::mutex> lock(queue_mutex);

        // don't allow enqueueing after stopping the pool
        if(stop)
            throw std::runtime_error("enqueue on stopped ThreadPool");

        tasks.emplace([task](){ (*task)(); });
    }
    condition.notify_one();
    return res;
}

// the destructor joins all threads
inline ThreadPool::~ThreadPool()
{
    {
        std::unique_lock<std::mutex> lock(queue_mutex);
        stop = true;
    }
    condition.notify_all();
    for(std::thread &worker: workers)
        worker.join();
}

#endif
