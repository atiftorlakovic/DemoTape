#pragma once
#include <cstddef> // for size_t

namespace rive {

// Enum for buffer types
enum class RenderBufferType {
    Vertex,
    Index
};

// Enum for buffer flags
enum class RenderBufferFlags {
    Static,
    Dynamic
};

class RenderBuffer {
public:
    RenderBuffer(RenderBufferType type, RenderBufferFlags flags, size_t sizeInBytes) {}
    virtual ~RenderBuffer() = default;
    
    // Virtual methods that may need to be overridden
    virtual void* onMap() { return nullptr; }
    virtual void onUnmap() {}
};

// Template for rcp (reference counted pointer) which appears in your code
#ifndef RIVE_RCP_DEFINED
#define RIVE_RCP_DEFINED

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

#endif // RIVE_RCP_DEFINED

} // namespace rive 