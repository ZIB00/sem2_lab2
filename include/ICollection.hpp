#pragma once

template<class T>
class ICollection
{
public:
    virtual ~ICollection() = default;
    
    virtual T Get(size_t index) = 0;
    virtual size_t GetLength() = 0;
};