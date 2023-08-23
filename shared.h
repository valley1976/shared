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

#include <valley/utils/scope_exit.h>

namespace valley::sync {

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

        void operator=(const T& other)
        {
            std::lock_guard<Lock> guard(lock_);
            data_ = other;
        }

        void operator=(T&& other)
        {
            std::lock_guard<Lock> guard(lock_);
            data_ = std::move(other);
        }

        void copy_to(T& other) const
        {
            std::lock_guard<Lock> guard(lock_);
            other = data_;
        }

        void move_to(T& other)
        {
            std::lock_guard<Lock> guard(lock_);
            other = std::move(data_);
        }

        template<typename Fn>
        auto safe_do(Fn&& fn)
        {
            std::lock_guard<Lock> guard(lock_);
            return fn(data_);
        }

        template<typename Fn>
        auto safe_do(Fn&& fn) const
        {
            std::lock_guard<Lock> guard(lock_);
            return fn(data_);
        }

        template<typename Fn>
        auto try_safe_do(Fn&& fn)
        {
            if (lock_.try_lock()) {
                auto guard = utils::make_scope_exit(
                    [this]() {
                        lock_.unlock();
                    }
                );

                return fn(data_);
            }
        }

        template<typename Fn>
        auto try_safe_do(Fn&& fn) const
        {
            if (lock_.try_lock()) {
                auto guard = utils::make_scope_exit(
                    [this]() {
                        lock_.unlock();
                    }
                );

                return fn(data_);
            }
        }

    private:
        mutable Lock lock_;
        T data_;
    };
}
