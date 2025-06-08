#pragma once
template <typename K, typename V>
class Entry {
public:
    Entry(const K& k, const V& v) : k_(k), v_(v) {}
    const K& key() const { return k_; }
    const V& value() const { return v_; }
    void setValue(const V& v) { v_ = v; }
private:
    K k_;
    V v_;
};
