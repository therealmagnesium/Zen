#include "Core/IO.h"
#include "Core/Base.h"
#include "Core/Log.h"

#include <stdio.h>
#include <stdlib.h>

#define IO_READ_CHUNK_SIZE 20971520

namespace Core
{
    File ReadFile(const char* path)
    {
        File file;
        file.isValid = false;

        FILE* fileHandle = fopen(path, "rb");
        if (!fileHandle)
        {
            WARN("Failed to read file %s!", path);
            return file;
        }

        char* data = NULL;
        char* temp = NULL;
        u64 used = 0;
        u64 size = 0;
        u64 n = 0;

        while (true)
        {
            if (used + IO_READ_CHUNK_SIZE + 1 > size)
            {
                size = used + IO_READ_CHUNK_SIZE + 1;

                if (size <= used)
                {
                    free(data);
                    WARN("%s is too large to read!", path);
                    return file;
                }

                temp = (char*)realloc(data, size);
                if (!temp)
                {
                    free(data);
                    WARN("Not enough memory to read file %s!", path);
                    return file;
                }

                data = temp;
            }

            n = fread(data + used, 1, IO_READ_CHUNK_SIZE, fileHandle);
            if (n == 0)
                break;

            used += n;
        }

        if (!fileHandle)
        {
            free(data);
            WARN("Failed to read file %s!", path);
            return file;
        }

        temp = (char*)realloc(data, used + 1);
        if (!temp)
        {
            free(data);
            WARN("Not enough memory to read file %s!", path);
            return file;
        }

        data = temp;
        data[used] = 0;

        file.data = data;
        file.length = used;
        file.isValid = true;

        fclose(fileHandle);
        return file;
    }

    void WriteFile(void* buffer, u64 size, const char* path)
    {
        FILE* fileHandle = fopen(path, "wb");
        if (!fileHandle)
        {
            WARN("Failed to write file %s!", path);
            return;
        }

        u64 chunksWritten = fwrite(buffer, size, 1, fileHandle);
        fclose(fileHandle);

        if (chunksWritten != 1)
        {
            WARN("Unexpected %ld chunks written!", chunksWritten);
            return;
        }
    }

    const char* TextFormat(const char* text, ...)
    {
        const u8 maxBuffers = 4;
        const u16 maxBufferLength = 1024;

        // We create an array of buffers so strings don't expire until MAX_TEXTFORMAT_BUFFERS invocations
        static char buffers[maxBuffers][maxBufferLength] = {0};
        static int index = 0;

        char* currentBuffer = buffers[index];
        memset(currentBuffer, 0, maxBufferLength); // Clear buffer before using

        va_list args;
        va_start(args, text);
        int requiredByteCount = vsnprintf(currentBuffer, maxBufferLength, text, args);
        va_end(args);

        // If requiredByteCount is larger than the MAX_TEXT_BUFFER_LENGTH, then overflow occured
        if (requiredByteCount >= maxBufferLength)
        {
            // Inserting "..." at the end of the string to mark as truncated
            char* truncBuffer = buffers[index] + maxBufferLength - 4; // Adding 4 bytes = "...\0"
            sprintf(truncBuffer, "...");
        }

        index += 1; // Move to next buffer for next function call
        if (index >= maxBuffers)
            index = 0;

        return currentBuffer;
    }

}
