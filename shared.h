/*
 *  Copyright (c) 2022-2023 shangu. All Right Reserved
 
 *  This is free software; you can redistribute it and/or modify it under the
 *  terms of the MIT license. A copy of the license can be found in the file
 *  "LICENSE" at the root of this distribution.
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
            std::lock_guard<Lock> guard(lock_);
            fn(data_);
        }
     
        T& unsafe_ref()
        {
            return data_;
        }
     
        const T& unsafe_ref() const
        {
            return data_;
        }
    private:
        Lock lock_;
        T data_;
    };
}


