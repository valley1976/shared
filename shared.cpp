// shared.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>

#include "shared.h"
#include <atomic>

int main()
{
    valley::Shared<int> s_int(0);

    auto thd1 = std::thread(
        [&]()
        {
            auto i = 1000000;
            while (i--) {
                s_int.safe_do(
                    [](int& v)
                    { 
                        ++v;

                        if (v % 10000 == 0)
                            std::cout << v << std::endl;
                    }
                );
            }
        });

    auto thd2 = std::thread(
        [&]()
        {
            auto i = 1000000;
            while (i--) {
                s_int.safe_do(
                    [](int& v)
                    { 
                        --v;

                        if (v % 10000 == 0)
                            std::cout << v << std::endl;
                    }
                );
            }
        });

    thd1.join();
    thd2.join();

    s_int.safe_do(
        [](int v)
        {
            std::cout << "result: " << v << std::endl;
        });

    std::cout << "bye!\n";
}
