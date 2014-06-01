#pragma once

#include <memory>


template<typename T>
class Component {
public:
    template<typename... Args>
    Component(Args&... args);

    T* get();
    const T* get() const;

    operator T&();

    T* operator->();

    T& operator*();

    Component<T>& operator=(T* newValue);


protected:
    std::unique_ptr<T> m_value;
};


#include "component.inl"
