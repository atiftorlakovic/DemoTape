#pragma once

#include "artboard.hpp"
#include <memory>
#include <vector>
#include <cstdint>

namespace rive {

enum class ImportResult {
    success,
    malformed,
    unsupported_version,
    invalid_format
};

class Span {
public:
    Span(const uint8_t* data, size_t length) : data(data), length(length) {}
    const uint8_t* data;
    size_t length;
};

class File {
public:
    File() = default;
    virtual ~File() = default;
    
    static std::unique_ptr<File> import(const Span& span, void* factory, ImportResult* result) {
        if (result) *result = ImportResult::success;
        return std::make_unique<File>();
    }
    
    std::unique_ptr<Artboard> artboardDefault() {
        return std::make_unique<Artboard>();
    }
    
    std::unique_ptr<Artboard> artboard(const std::string& name) {
        return std::make_unique<Artboard>();
    }
};

} // namespace rive 