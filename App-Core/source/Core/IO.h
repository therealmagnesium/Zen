#pragma once
#include "Core/Base.h"

namespace Core
{
    /*
     * [@brief] A representation of a file on disk.
     * [@field] data - The contents of the file.
     * [@field] length - The total length of the file.
     * [@field] isValid - Describes if the file has been read.
     */
    struct File
    {
        char* data;
        u64 length;
        b8 isValid;
    };

    /*
     * [@brief] Read a file as binary, and return the data.
     * [@field] path - The path to the file on disk to read from.
     * */
    File ReadFile(const char* path);

    /*
     * [@brief] Write a buffer of memory to a file on disk.
     * [@field] buffer - The data that will be written to the file.
     * [@field] size - The size of the buffer in bytes.
     * [@field] path - The path to the file on disk to write to.
     * */
    void WriteFile(void* buffer, u64 size, const char* path);

    /*
     * [@brief] Formats a string of text using C format specifiers.
     * [@field] text - The string to be formatted.
     * [@field] VA_ARGS - The values to replace the format specifiers.
     * */
    const char* TextFormat(const char* text, ...);
}
