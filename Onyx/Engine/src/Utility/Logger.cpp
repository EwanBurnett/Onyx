/*
* @file Logger.cpp
* Thread-Safe Console Logging Utility
* ------------------------------------------------
* @author Ewan Burnett(EwanBurnettSK@Outlook.com)
* @date 2025/07/28
*/
#include "../../include/Onyx/Utility/Logger.h"

#include <cstdio> 
#include <time.h> 

//Controls how large the default buffer for Timestamp formatting is. 
constexpr int kTimestampBufferSize = 0xff;

#ifdef _WIN32

#define WIN32_LEAN_AND_MEAN
#define VC_EXTRALEAN
#define NOGDI
#include <Windows.h> 

#elif __ANDROID__ 

#include <android/log.h>

#define ALOG_UNKNOWN(...) ((void)__android_log_vprint(ANDROID_LOG_UNKNOWN, "Onyx-Engine", __VA_ARGS__))
#define ALOG_PRINT(...) ((void)__android_log_vprint(ANDROID_LOG_DEFAULT, "Onyx-Engine", __VA_ARGS__))
#define ALOG_VERBOSE(...) ((void)__android_log_vprint(ANDROID_LOG_VERBOSE, "Onyx-Engine", __VA_ARGS__))
#define ALOG_DEBUG(...) ((void)__android_log_vprint(ANDROID_LOG_DEBUG, "Onyx-Engine", __VA_ARGS__))
#define ALOG_INFO(...) ((void)__android_log_vprint(ANDROID_LOG_INFO, "Onyx-Engine", __VA_ARGS__))
#define ALOG_WARNING(...) ((void)__android_log_vprint(ANDROID_LOG_WARN, "Onyx-Engine", __VA_ARGS__))
#define ALOG_ERROR(...) ((void)__android_log_vprint(ANDROID_LOG_ERROR, "Onyx-Engine", __VA_ARGS__))
#define ALOG_FATAL(...) ((void)__android_log_vprint(ANDROID_LOG_FATAL, "Onyx-Engine", __VA_ARGS__))

#endif

Onyx::Utility::Log::Log()
{
    m_SeverityFlags = ELogSeverityFlags::ALL;
}

Onyx::Utility::Log& Onyx::Utility::Log::GetInstance()
{
    //As a static variable, this is guaranteed to be initialized only once, and destroyed. 
    static Log instance;
    return instance;
}

void Onyx::Utility::Log::SetSeverityFlags(const ELogSeverityFlags severityFlags)
{
    m_SeverityFlags = severityFlags;
}

Onyx::Utility::ELogSeverityFlags Onyx::Utility::Log::GetSeverityFlags()
{
    return m_SeverityFlags;
}

/**
 * @brief Prints a message to the Console, as-is.
 * @param fmt
 * @param
 */
void Onyx::Utility::Log::Print(const char* fmt, ...)
{
    Log& l = Log::GetInstance();

    {
        //We need to own the mutex for all output calls associated with this message!
        std::lock_guard<std::mutex> lk(l.m_OutputLock);

#if __ANDROID__
        //Forward our message to the Android Logger. 
        va_list args;
        va_start(args, fmt);
        l._AOutput(ELogSeverity::PRINT, fmt, args);
        va_end(args);
#else
        //Manually format the Log Output. 
        char timestamp[kTimestampBufferSize];
        GetTimestamp(timestamp, kTimestampBufferSize);

        l._Output(ELogColour::WHITE, stdout, timestamp);
        l._Output(ELogColour::WHITE, stdout, "");
        va_list args;
        va_start(args, fmt);
        l._Output(ELogColour::WHITE, stdout, fmt, args);
        va_end(args);
#endif
    }

}

/**
 * @brief Prints a message to the Console, as-is.
 * @param colour `ELogColour` for the message.
 * @param fmt
 * @param
 */
void Onyx::Utility::Log::Print(ELogColour colour, const char* fmt, ...)
{
    Log& l = Log::GetInstance();

    {
        //We need to own the mutex for all output calls associated with this message!
        std::lock_guard<std::mutex> lk(l.m_OutputLock);

#if __ANDROID__
        //Forward our message to the Android Logger. 
        va_list args;
        va_start(args, fmt);
        l._AOutput(ELogSeverity::PRINT, fmt, args);
        va_end(args);
#else
        //Manually format the Log Output. 
        char timestamp[kTimestampBufferSize];
        GetTimestamp(timestamp, kTimestampBufferSize);

        l._Output(ELogColour::WHITE, stdout, timestamp);
        l._Output(colour, stdout, "");
        va_list args;
        va_start(args, fmt);
        l._Output(colour, stdout, fmt, args);
        va_end(args);
#endif
    }

}

/**
 * @brief Prints a Debug message to the console.
 * @param fmt
 * @param
 */
void Onyx::Utility::Log::Debug(const char* fmt, ...)
{
    Log& l = Log::GetInstance();

    //Only output if Debugging severity is enabled. 
    if ((int)l.GetSeverityFlags() & (int)ELogSeverityFlags::DEBUG) {
        //We need to own the mutex for all output calls associated with this message!
        std::lock_guard<std::mutex> lk(l.m_OutputLock);

#if __ANDROID__
        //Forward our message to the Android Logger. 
        va_list args;
        va_start(args, fmt);
        l._AOutput(ELogSeverity::DEBUG, fmt, args);
        va_end(args);
#else
        //Manually format the Log Output. 
        char timestamp[kTimestampBufferSize];
        GetTimestamp(timestamp, kTimestampBufferSize);

        l._Output(ELogColour::WHITE, stdout, timestamp);
        l._Output(ELogColour::GREEN, stdout, "[Debug]\t");
        va_list args;
        va_start(args, fmt);
        l._Output(ELogColour::GREEN, stdout, fmt, args);
        va_end(args);
#endif
    }

}

/**
 * @brief Prints an Info message to the console.
 * @param fmt
 * @param
 */
void Onyx::Utility::Log::Message(const char* fmt, ...)
{
    Log& l = Log::GetInstance();

    //Only output if Status severity is enabled. 
    if ((int)l.GetSeverityFlags() & (int)ELogSeverityFlags::STATUS) {
        //We need to own the mutex for all output calls associated with this message!
        std::lock_guard<std::mutex> lk(l.m_OutputLock);

#if __ANDROID__
        //Forward our message to the Android Logger. 
        va_list args;
        va_start(args, fmt);
        l._AOutput(ELogSeverity::MESSAGE, fmt, args);
        va_end(args);
#else
        //Manually format the Log Output. 
        char timestamp[kTimestampBufferSize];
        GetTimestamp(timestamp, kTimestampBufferSize);

        l._Output(ELogColour::WHITE, stdout, timestamp);
        l._Output(ELogColour::LIGHTCYAN, stdout, "[Info]\t");
        va_list args;
        va_start(args, fmt);
        l._Output(ELogColour::LIGHTCYAN, stdout, fmt, args);
        va_end(args);
#endif
    }

}

/**
 * @brief Prints a Warning message to the console.
 * @param fmt
 * @param
 */
void Onyx::Utility::Log::Warning(const char* fmt, ...)
{
    Log& l = Log::GetInstance();

    //Only output if Status severity is enabled. 
    if ((int)l.GetSeverityFlags() & (int)ELogSeverityFlags::WARNINGS) {
        //We need to own the mutex for all output calls associated with this message!
        std::lock_guard<std::mutex> lk(l.m_OutputLock);

#if __ANDROID__
        //Forward our message to the Android Logger. 
        va_list args;
        va_start(args, fmt);
        l._AOutput(ELogSeverity::MESSAGE, fmt, args);
        va_end(args);
#else
        //Manually format the Log Output. 
        char timestamp[kTimestampBufferSize];
        GetTimestamp(timestamp, kTimestampBufferSize);

        l._Output(ELogColour::WHITE, stdout, timestamp);
        l._Output(ELogColour::YELLOW, stdout, "[Warning]\t");
        va_list args;
        va_start(args, fmt);
        l._Output(ELogColour::YELLOW, stdout, fmt, args);
        va_end(args);
#endif
    }

}

/**
 * @brief Prints an Error message to the console.
 * @param file provide `__FILE__`.
 * @param line provide `__LINE__`.
 * @param funcsig provide `__PRETTY_FUNCTION__`.
 * @param fmt
 * @param
 */
void Onyx::Utility::Log::Error(const char* file, const uint64_t line, const char* funcsig, const char* fmt, ...)
{
    Log& l = Log::GetInstance(); //Track the log.

    if ((int)l.GetSeverityFlags() & (int)ELogSeverityFlags::ERRORS) {
        std::lock_guard<std::mutex> lk(l.m_OutputLock);

#if __ANDROID__
        //Forward our message to the Android Logger. 
        va_list args;
        va_start(args, fmt);
        l._AOutput(ELogSeverity::ERROR, fmt, args);
        l._AOutput(ELogSeverity::ERROR, "File: %s\nLine : %d\nFunction : %s\n", file, line, funcsig);
        va_end(args);
#else
        char timestamp[kTimestampBufferSize];
        GetTimestamp(timestamp, kTimestampBufferSize);

        l._Output(ELogColour::WHITE, stderr, timestamp);

        l._Output(ELogColour::LIGHTRED, stderr, "[Error]\t");
        va_list args;
        va_start(args, fmt);
        l._Output(ELogColour::LIGHTRED, stderr, fmt, args);
        va_end(args);
        l._Output(ELogColour::LIGHTRED, stderr, "\nFile: %s\nLine : %d\nFunction : %s\n", file, line, funcsig);
#endif
    }
}

/**
 * @brief Prints a Fatal Error message to the console.
 * @param file provide `__FILE__`.
 * @param line provide `__LINE__`.
 * @param funcsig provide `__PRETTY_FUNCTION__`.
 * @param fmt
 * @param
 */
void Onyx::Utility::Log::Fatal(const char* file, const uint64_t line, const char* funcsig, const char* fmt, ...)
{
    Log& l = Log::GetInstance(); //Track the log.

    if ((int)l.GetSeverityFlags() & (int)ELogSeverityFlags::ERRORS) {
        std::lock_guard<std::mutex> lk(l.m_OutputLock);

#if __ANDROID__
        //Forward our message to the Android Logger. 
        va_list args;
        va_start(args, fmt);
        l._AOutput(ELogSeverity::FATAL, fmt, args);
        l._AOutput(ELogSeverity::FATAL, "File: %s\nLine : %d\nFunction : %s\n", file, line, funcsig);
        va_end(args);
#else
        char timestamp[kTimestampBufferSize];
        GetTimestamp(timestamp, kTimestampBufferSize);

        l._Output(ELogColour::WHITE, stderr, timestamp);

        l._Output(ELogColour::RED, stderr, "[FATAL]\t");
        va_list args;
        va_start(args, fmt);
        l._Output(ELogColour::RED, stderr, fmt, args);
        va_end(args);
        l._Output(ELogColour::RED, stderr, "\nFile: %s\nLine : %d\nFunction : %s\n", file, line, funcsig);
#endif
    }
}

/**
 * @brief Prints a Validation message to the console.
 * @param prefix Category prefix for the Validation message.
 * @param fmt
 * @param
 */
void Onyx::Utility::Log::Validation(const char* prefix, const char* fmt, ...)
{
    Log& l = Log::GetInstance();

    if ((int)l.GetSeverityFlags() & (int)ELogSeverityFlags::VALIDATION) {
        std::lock_guard<std::mutex> lk(l.m_OutputLock);
#if __ANDROID__
        l._AOutput(ELogSeverity::VALIDATION, "[%s]\t", prefix);
        va_list args;
        va_start(args, fmt);
        l._AOutput(ELogSeverity::VALIDATION, fmt, args);
        va_end(args);
#else
        char timestamp[kTimestampBufferSize];
        GetTimestamp(timestamp, kTimestampBufferSize);

        l._Output(ELogColour::WHITE, stdout, timestamp);

        l._Output(ELogColour::MAGENTA, stdout, "[%s]\t", prefix);

        va_list args;
        va_start(args, fmt);
        l._Output(ELogColour::MAGENTA, stdout, fmt, args);
        va_end(args);
#endif
    }
}

/**
 * @brief Formats a Timestamp in the format [%H:%M:%S] in the buffer provided.
 * @param timestampBuffer pointer to a buffer of bufferSize or greater.
 * @param bufferSize the size of the buffer.
 */
void Onyx::Utility::Log::GetTimestamp(char* timestampBuffer, size_t bufferSize)
{
    time_t now = time(NULL);
    auto _tm = localtime(&now);    //NOTE: This isn't thread safe in the standard!

    strftime(timestampBuffer, bufferSize, "[%H:%M:%S] ", _tm);
}

/**
 * @brief Outputs a message to the console.
 * @param colour
 * @param stream
 * @param fmt
 * @param
 */
void Onyx::Utility::Log::_Output(ELogColour colour, FILE* stream, const char* fmt, ...)
{
    //Construct a va_list, and forward to the handler. 
    va_list args;
    va_start(args, fmt);
    _Output(colour, stream, fmt, args);
    va_end(args);
}

/**
 * @brief Outputs a message to the console.
 * @param colour
 * @param stream
 * @param fmt
 * @param args
 */
void Onyx::Utility::Log::_Output(ELogColour colour, FILE* stream, const char* fmt, va_list args)
{
    //Change the output colour 
#ifdef _WIN32
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, (WORD)colour);
#else //Assume the program is running on linux, or a similar system. 
    switch (colour) {
    case ELogColour::RED:
        fprintf(stream, "\033[31m");
        break;
    case ELogColour::GREEN:
        fprintf(stream, "\033[32m");
        break;
    case ELogColour::YELLOW:
    case ELogColour::BROWN:
        fprintf(stream, "\033[33m");
        break;
    case ELogColour::BLUE:
        fprintf(stream, "\033[34m");
        break;
    case ELogColour::CYAN:
        fprintf(stream, "\033[36m");
        break;
    case ELogColour::WHITE:
        fprintf(stream, "\033[37m");
        break;
    default:
        fprintf(stream, "\033[35m");
        break;
    }
#endif

    vfprintf(stream, fmt, args);
    //Reset the output colour
#ifdef _WIN32
    SetConsoleTextAttribute(hConsole, (WORD)ELogColour::WHITE);
#else
    fprintf(stream, "\033[0m");
#endif
}

/**
 * @brief Outputs a message to the Android Console.
 * @param severity `ELogSeverity` of the message to output.
 * @param fmt
 * @param
 */
void Onyx::Utility::Log::_AOutput(ELogSeverity severity, const char* fmt, ...) {
    //Construct a va_list, and forward to the handler. 
    va_list args;
    va_start(args, fmt);
    _AOutput(severity, fmt, args);
    va_end(args);
}

/**
 * @brief Outputs a message to the Android Console.
 * @param severity `ELogSeverity` of the message to output.
 * @param fmt
 * @param args
 */
void Onyx::Utility::Log::_AOutput(ELogSeverity severity, const char* fmt, va_list args) {
#if __ANDROID__
    switch (severity) {
    case ELogSeverity::PRINT:
        ALOG_PRINT(fmt, args);
        break;
    case ELogSeverity::DEBUG:
        ALOG_DEBUG(fmt, args);
        break;
    case ELogSeverity::MESSAGE:
        ALOG_INFO(fmt, args);
        break;
    case ELogSeverity::VALIDATION:
        ALOG_VERBOSE(fmt, args);
        break;
    case ELogSeverity::WARNING:
        ALOG_WARNING(fmt, args);
        break;
    case ELogSeverity::ERROR:
        ALOG_ERROR(fmt, args);
        break;
    case ELogSeverity::FATAL:
        ALOG_FATAL(fmt, args);
        break;
    default:
        ALOG_UNKNOWN(fmt, args);
    }
#endif
}



