#pragma once

#include <string>
#include <cmath>
#include <type_traits>
#include <random>
#include <chrono>


// Pomocnicza funkcją hashująca
class BasicHasher {
public:
    // Liczby całkowite (int, long, char itp.)
    template<typename T>
    static typename std::enable_if<std::is_integral<T>::value, int>::type
    hash(const T& key) {
        return std::abs(static_cast<int64_t>(key));
    }

    // Liczby zmiennoprzecinkowe (float, double)
    template<typename T>
    static typename std::enable_if<std::is_floating_point<T>::value, int>::type
    hash(const T& key) {
        return std::abs(static_cast<int64_t>(key * 100000)); 
    }

    // Ciągi znaków
    static int hash(const std::string& key) {
        int hash = 0;
        for (char c : key) {
            hash = 31 * hash + static_cast<int>(c);
        }
        return std::abs(hash);
    }

    // Pojedynczy znak
    static int hash(char c) {
        return static_cast<int>(c);
    }

};


// MODULO HASH

template<typename K>
class ModuloHash {
private:
    int capacity;

public:
    ModuloHash(int cap = 101) : capacity(cap) {}

    int operator()(const K& key) const {
        return BasicHasher::hash(key) % capacity;
    }
};


// MULTIPLICATIVE HASH
template<typename K>
class MultiplicativeHash {
private:
    static constexpr double A = 0.6180339887;
    int capacity;

public:
    explicit MultiplicativeHash(int cap = 101) : capacity(cap) {}

    int operator()(const K& key) const {
        double fractional = std::fmod(BasicHasher::hash(key) * A, 1.0);
        return static_cast<int>(capacity * fractional);
    }
};


// UNIVERSAL HASH

template<typename K>
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

    int operator()(const K& key) const {
        return static_cast<int>(((int64_t(a) * BasicHasher::hash(key) + b) % p) % capacity);
    }
};
