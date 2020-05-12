#ifndef CPP_COMMON_H
#define CPP_COMMON_H


#include <chrono>
#include <utility>

typedef std::chrono::high_resolution_clock::time_point TimeVar;
#define duration(a) std::chrono::duration_cast<std::chrono::nanoseconds>(a).count()
#define timeNow() std::chrono::high_resolution_clock::now()

template<typename F, typename... Args>
double funcTime(F func, Args&&... args){
    TimeVar t1=timeNow();
    func(std::forward<Args>(args)...);
    return duration(timeNow()-t1);
}

template<class F, class... Args>
auto FuncTime(F&& func, Args&&... args)->typename std::result_of<F(Args...)>::type{
    TimeVar t1=timeNow();
    auto ret = func(std::forward<Args>(args)...);
    std::tuple<>(duration(timeNow()-t1),ret);
}



#endif