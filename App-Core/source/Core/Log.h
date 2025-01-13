#pragma once
#include "Core/Base.h"

#define LOG_TRACE_ENABLED 1
#define LOG_INFO_ENABLED 1
#define LOG_WARN_ENABLED 1

#if RELEASE == 1
#undef LOG_TRACE_ENABLED
#define LOG_TRACE_ENABLED 0
#endif

#if DIST == 1
#undef LOG_INFO_ENABLED
#undef LOG_WARN_ENABLED
#undef LOG_TRACE_ENABLED

#define LOG_INFO_ENABLED 0
#define LOG_WARN_ENABLED 0
#define LOG_TRACE_ENABLED 0
#endif

typedef enum
{
    LOG_LEVEL_FATAL = 0,
    LOG_LEVEL_ERROR,
    LOG_LEVEL_WARN,
    LOG_LEVEL_INFO,
    LOG_LEVEL_TRACE,
} LogLevel;

namespace Core
{
    b8 InitLogging();
    void ShutdownLogging();

    void _LogOutput(LogLevel level, const char* message, ...);
    void _ReportAssertion(const char* expression, const char* message, const char* file, u32 line);
}

#define debugBreak() __builtin_trap()
#define ASSERT(expr)                                                                                                   \
    {                                                                                                                  \
        if (expr)                                                                                                      \
        {                                                                                                              \
        }                                                                                                              \
        else                                                                                                           \
        {                                                                                                              \
            Core::_ReportAssertion(#expr, "", __FILE__, __LINE__);                                                     \
            debugBreak();                                                                                              \
        }                                                                                                              \
    }

#define ASSERT_MSG(expr, message)                                                                                      \
    {                                                                                                                  \
        if (expr)                                                                                                      \
        {                                                                                                              \
        }                                                                                                              \
        else                                                                                                           \
        {                                                                                                              \
            Core::_ReportAssertion(#expr, message, __FILE__, __LINE__);                                                \
            debugBreak();                                                                                              \
        }                                                                                                              \
    }

#define FATAL(message, ...) Core::_LogOutput(LOG_LEVEL_FATAL, message, ##__VA_ARGS__);
#define ERROR(message, ...) Core::_LogOutput(LOG_LEVEL_ERROR, message, ##__VA_ARGS__);

#if LOG_WARN_ENABLED == 1
#define WARN(message, ...) Core::_LogOutput(LOG_LEVEL_WARN, message, ##__VA_ARGS__);
#else
#define WARN(message, ...)
#endif

#if LOG_INFO_ENABLED == 1
#define INFO(message, ...) Core::_LogOutput(LOG_LEVEL_INFO, message, ##__VA_ARGS__);
#else
#define INFO(message, ...)
#endif

#if LOG_TRACE_ENABLED == 1
#define TRACE(message, ...) Core::_LogOutput(LOG_LEVEL_TRACE, message, ##__VA_ARGS__);
#else
#define TRACE(message, ...)
#endif
