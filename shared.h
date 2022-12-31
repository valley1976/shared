/*
 *  Copyright (c) 2022-2023 shangu. All Right Reserved
 *
 *  author:  shangu
 *  Date:    2022-12-31
 *
 *  Description: 
 *
*/

#pragma once

#include <mutex>

namespace valley
{
    template<typename T, typename Lock = std::mutex>
    class Shared
    {
    public:
        template<typename ... Args>
        Shared(Args&& ... args) : lock_(),
            data_(std::forward<Args>(args)...)
        {}

        Shared(const Shared&) = delete;
        Shared& operator=(const Shared&) = delete;

        template<typename Fn>
        void safe_do(Fn&& fn)
        {
            lock_.lock();
            fn(data_);
            lock_.unlock();
        }

    private:
        Lock lock_;
        T data_;
    };
}


