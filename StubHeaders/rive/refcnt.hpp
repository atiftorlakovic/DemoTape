#pragma once

// Only define rcp if it's not already defined in render_buffer.hpp
#ifndef RIVE_RCP_DEFINED
#define RIVE_RCP_DEFINED

namespace rive {

// If this is already defined in render_buffer.hpp, the compiler will ignore this duplicate
template <typename T>
class rcp {
public:
    rcp() : ptr(nullptr) {}
    rcp(T* p) : ptr(p) {}
    ~rcp() { delete ptr; }
    
    T* operator->() const { return ptr; }
    T& operator*() const { return *ptr; }
    
private:
    T* ptr;
};

} // namespace rive 

#endif // RIVE_RCP_DEFINED 