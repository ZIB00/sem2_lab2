template<class T>
Option<T>::Option()
{
    this->value = nullptr;
}

template<class T>
Option<T>::Option(T val)
{
    this->value = new T(val);
}

template<class T>
Option<T>::Option(const Option<T>& other)
{
    if (other.value != nullptr) {
        this->value = new T(*(other.value));
    } else {
        this->value = nullptr;
    }
}

template<class T>
Option<T>& Option<T>::operator=(const Option<T>& other)
{
    if (this == &other) return *this;

    delete this->value;

    if (other.value != nullptr) {
        this->value = new T(*(other.value));
    } else {
        this->value = nullptr;
    }

    return *this;
}

template<class T>
Option<T>::~Option()
{
    if (this->value != nullptr) {
        delete this->value;
    }
}

template<class T>
bool Option<T>::HasValue()
{
    return this->value != nullptr;
}

template<class T>
T Option<T>::GetValue()
{
    if (this->value == nullptr) { 
        throw LogicError("Option::GetValue - Logic Error: Access denied. Attempted to retrieve a value from an empty Option instance (None). "
                         "Current state: [hasValue = false, value = nullptr]. ");
    }
    
    return *(this->value);
}

template<class T>
T Option<T>::ValueOr(T defaultValue)
{
    if (this->value != nullptr) {
        return *(this->value);
    }
    return defaultValue;
}

template<class T>
template<class R>
Option<R> Option<T>::Map(std::function<R(T)> function)
{
    if (this->value == nullptr) {
        return Option<R>();
    }
    return Option<R>(function(*(this->value))); 
}

template<class T>
template<class R>
Option<R> Option<T>::FlatMap(std::function<Option<R>(T)> function)
{
    if (this->value == nullptr) {
        return Option<R>();
    }
    return function(*(this->value));
}