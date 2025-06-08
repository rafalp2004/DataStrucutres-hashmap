#pragma once
#include <functional>
#include <random>
#include <chrono>

class ModuloHash {
private:
    int capacity;
public:
    ModuloHash(int cap = 101) : capacity(cap) {}

    template<typename K>
    int operator()(const K& key) const {
        std::hash<K> hash_fn;
        return static_cast<int>(hash_fn(key) % capacity);
    }
};

class MultiplicativeHash {
private:
    static constexpr double A = 0.6180339887;  
    int capacity;

public:
    explicit MultiplicativeHash(int cap = 101) : capacity(cap) {}

    template<typename K>
    int operator()(const K& key) const {
        std::hash<K> hash_fn;
        double hash_val = static_cast<double>(hash_fn(key));

        double intpart;
        double fractional = std::modf(hash_val * A, &intpart);  

        return static_cast<int>(capacity * fractional);
    }
};

class UniversalHash {
private:
    int a, b, p, capacity;
public:
    UniversalHash(int cap = 101)
        : capacity(cap), p(10000019) {
        static std::mt19937 gen(static_cast<unsigned>(
            std::chrono::steady_clock::now().time_since_epoch().count()));
        std::uniform_int_distribution<int> dist(1, p - 1);
        a = dist(gen);
        b = dist(gen);
    }

    template<typename K>
    int operator()(const K& key) const {
        std::hash<K> hash_fn;
        return static_cast<int>(((int64_t(a) * hash_fn(key) + b) % p) % capacity);
    }
};
