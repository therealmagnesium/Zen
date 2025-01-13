#include "Core/Log.h"
#include "Core/Base.h"

#include <stdio.h>
#include <stdarg.h>
#include <string.h>

namespace Core
{
    b8 InitLogging()
    {
        // TODO: Create a file to log to

        return true;
    }

    void ShutdownLogging()
    {
        // TODO: Clean up anything from InitLogging()
    }

    void _LogOutput(LogLevel level, const char* message, ...)
    {
        b8 isError = level < 2;
        const char* levelStrings[5] = {
            "[Fatal] ", "[Error] ", "[Warn] ", "[Info] ", "[Trace] ",
        };

        char outMessage[32000];
        memset(outMessage, 0, sizeof(outMessage));

        va_list argPointer;
        va_start(argPointer, message);
        vsnprintf(outMessage, 32000, message, argPointer);
        va_end(argPointer);

        char outMessage2[32000];
        sprintf(outMessage2, "%s%s", levelStrings[level], outMessage);

        printf("%s\n", outMessage2);
    }

}
