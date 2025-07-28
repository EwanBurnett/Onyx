#ifndef __LOGGER_H
#define __LOGGER_H
/*
* Thread-Safe Console Logging Utility
* ------------------------------------------------
* @author Ewan Burnett(EwanBurnettSK@Outlook.com)
* @date 2025/07/28
*/

// Use the equivalent Function Signature macro on Microsoft compilers. 
#ifdef _MSC_VER
#define __PRETTY_FUNCTION__ __FUNCSIG__
#endif

#include <cstdarg> 
#include <mutex> 



namespace Onyx {
    namespace Utility {

        enum class ELogColour : uint8_t {
            BLACK = 0,
            BLUE,
            GREEN,
            CYAN,
            RED,
            MAGENTA,
            BROWN,
            LIGHTGRAY,
            DARKGRAY,
            LIGHTBLUE,
            LIGHTGREEN,
            LIGHTCYAN,
            LIGHTRED,
            LIGHTMAGENTA,
            YELLOW,
            WHITE,
        };

        /**
         * @brief Mask Controlling the Severty of the Logger's output.
         */
        enum ELogSeverityFlags : uint32_t {
            NONE = 0u,
            GENERAL = (1 << 0),     //Enables Print, Message and Fatal Error statements.
            DEBUG = (1 << 1),       //Enables Debug statements.
            STATUS = (1 << 2),      //Enables Success and Failure status statements. 
            WARNINGS = (1 << 3),    //Enables Warning statements.  
            ERRORS = (1 << 4),      //Enables Error statements. 
            VALIDATION = (1 << 5),  //Enables API Validation statements. 
            ALL = (1 << 6) - 1,     //Enables All severity stages. 
            ELogSeverityFlags_MAX
        };

        inline ELogSeverityFlags operator | (ELogSeverityFlags lhs, ELogSeverityFlags rhs) {
            return (ELogSeverityFlags)((int)lhs | (int)rhs);
        }

        /**
         * @brief Represents the Severity of a logging message.
         */
        enum class ELogSeverity {
            PRINT = 0,
            DEBUG,
            MESSAGE,
            WARNING,
            ERROR,
            FATAL,
            VALIDATION,
            ELogSeverity_MAX
        };

        class Log {
        public:
            Log(const Log&) = delete;
            void operator = (const Log&) = delete;

            static Log& GetInstance();

            void SetSeverityFlags(const ELogSeverityFlags severityFlags);
            ELogSeverityFlags GetSeverityFlags();

            static void Print(const char* fmt, ...);
            static void Debug(const char* fmt, ...);
            static void Message(const char* fmt, ...);
            static void Success(const char* fmt, ...);
            static void Failure(const char* fmt, ...);
            static void Warning(const char* fmt, ...);
            static void Error(const char* file, const uint64_t line, const char* funcsig, const char* fmt, ...);
            static void Fatal(const char* file, const uint64_t line, const char* funcsig, const char* fmt, ...);
            static void Validation(const char* prefix, const char* fmt, ...);

        private:
            Log();

            static void GetTimestamp(char* timestampBuffer, size_t bufferSize);

            void _Output(ELogColour colour, FILE* stream, const char* fmt, ...);
            void _Output(ELogColour colour, FILE* stream, const char* fmt, va_list args);

            void _AOutput(ELogSeverity severity, const char* fmt, ...);
            void _AOutput(ELogSeverity severity, const char* fmt, va_list args); 
        private:
            std::mutex m_OutputLock;    //Mutex lock for outputting. Enables concurrent printing without a race condition. 
            ELogSeverityFlags m_SeverityFlags;    //Current logger severity level flags
        };
    }
}
#endif//__LOGGER_H