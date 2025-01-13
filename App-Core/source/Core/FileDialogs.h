#pragma once
#include <string>

namespace Core
{
    class FileDialogs
    {
    public:
        static std::string OpenFile(const char* filter, const char* defaultPath);
        static std::string SaveFile(const char* filter, const char* defaultPath);
    };
}
