#pragma once

#include <random>
#include <chrono>
#include <algorithm>
#include <string>

class IdGen
{

private:
    std::mt19937 rng; // 随机数生成器

public:
    IdGen()
    {
        // 初始化随机数生成器
        using std::chrono::high_resolution_clock;
        rng = std::mt19937(static_cast<unsigned int>(high_resolution_clock::now().time_since_epoch().count()));
    }

    std::string randomId(size_t length)
    {
        static const std::string characters("0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz");
        std::string id(length, '0');
        std::uniform_int_distribution<int> uniform(0, int(characters.size() - 1));
        std::generate(id.begin(), id.end(), [&]()
                      { return characters.at(uniform(rng)); });

        return id;
    }
};