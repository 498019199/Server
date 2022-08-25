#ifndef __BASE_LOGGER__H__
#define __BASE_LOGGER__H__
#include <sstream>
#include <vector>
#include <queue>
#include <memory>

#include "base/lock.h"
#include "net/event_loop.h"

enum LogLevel
{
    LogLevel_Dubug = 1,
    LogLevel_Trace,
    LogLevel_Warn,
    LogLevel_Error,
    LogLevel_Fatal,
};

class log_event
{
public:
    typedef std::shared_ptr<log_event> ptr;

    log_event(LogLevel level, const char* filename, int line, const char* func_name)
            :log_level_(level), file_name_(filename), line_(line), func_name_(func_name)
    {
    }

    ~log_event()
    {
    }

    std::stringstream& stream();

    void log();
private:
    timeval timeval_;
    LogLevel log_level_;
    const char* file_name_;
    int line_;
    const char* func_name_;

    std::stringstream ss_;
};

class log_tmp
{
public:
    explicit log_tmp(log_event::ptr event)
            :event_(event)
    {}

    ~log_tmp()
    {
        event_->log();
    }

    std::stringstream& stream()
    {
        return event_->stream();
    }
private:
    log_event::ptr event_;
};

class async_logger
{
public:
    typedef std::shared_ptr<async_logger> ptr;

    async_logger(const char* file_name, const char* file_path, int max_size);

    ~async_logger();

    void push(std::vector<std::string>& buf);

    void flush();

    static void* excute(void*);

    void stop();
public:
    std::queue<std::vector<std::string>> m_tasks;
    pthread_t thread_;
private:
    const char* file_name_;
    const char* file_path_;
    int max_size_;

    int no_ = 0;
    bool need_reopen_ = false;
    std::string data_;
    FILE* file_handle_ = nullptr;
    bool is_stop_ = false;

    mutex_lock mutex_;
    pthread_cond_t condition_;
};

class logger
{
public:
    typedef std::shared_ptr<logger> ptr;

    logger();

    ~logger();

    void init(const char* file_name, const char* file_path, int max_size, int sync_inteval);

    void log();

    void push_log(const std::string& msg);

    void loop();

    void flush();

    async_logger::ptr get_async_logger() {return async_log_;}
public:
    std::vector<std::string> buffs_;
private:
    bool is_init_ = false;
    mutex_lock mutex_;
    async_logger::ptr async_log_;
};

#define LOG_STREAM(log_level) \
    log_tmp(log_event::ptr(new log_event(log_level, __FILE__, __LINE__, __func__))).stream()

#define LOG_DEBUG \
    LOG_STREAM(LogLevel::LogLevel_Dubug)
#define LOG_TRACE \
    LOG_STREAM(LogLevel::LogLevel_Trace)
#define LOG_WARN \
    LOG_STREAM(LogLevel::LogLevel_Warn)
#define LOG_ERROR \
    LOG_STREAM(LogLevel::LogLevel_Error)
#define LOG_FATAL \
    LOG_STREAM(LogLevel::LogLevel_Error)

// Build the error message string.  This is separate from the "Impl"
// function template because it is not performance critical and so can
// be out of line, while the "Impl" code should be inline.  Caller
// takes ownership of the returned string.
template<class t1, class t2>
std::string* MakeCheckOpString(const t1& v1, const t2& v2, const char* names) {
    std::ostringstream ss;
    ss << names << " (" << v1 << " vs " << v2 << "). ";
    std::string* msg = new std::string(ss.str());
    return msg;
}

#define BAIDU_CHECK_OP(name, op, val1, val2)                            \
    if (std::string* _result =                                          \
               MakeCheckOpString((val1), (val2),                        \
                                #val1 " " #op " " #val2))               \
                                LOG_DEBUG << _result;


#define CHECK_EQ(val1, val2) BAIDU_CHECK_OP(EQ, ==, val1, val2)
#define CHECK_NE(val1, val2) BAIDU_CHECK_OP(NE, !=, val1, val2)
#define CHECK_LE(val1, val2) BAIDU_CHECK_OP(LE, <=, val1, val2)
#define CHECK_LT(val1, val2) BAIDU_CHECK_OP(LT, < , val1, val2)
#define CHECK_GE(val1, val2) BAIDU_CHECK_OP(GE, >=, val1, val2)
#define CHECK_GT(val1, val2) BAIDU_CHECK_OP(GT, > , val1, val2)

#endif//__BASE_LOGGER__H__