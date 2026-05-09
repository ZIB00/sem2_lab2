#pragma once

template <class T>
class IGroup {
public:
    virtual ~IGroup() = default;
    
    virtual T Add(T a, T b) const = 0;
    virtual T Zero() const = 0;
    virtual T Inverse(T a) const = 0;
};

template <class T>
class IRing : public IGroup<T> {
public:
    virtual T Multiply(T a, T b) const = 0;
    virtual T One() const = 0;
};