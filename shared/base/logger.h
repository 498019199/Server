#ifndef __BASE_LOGGER__H__
#define __BASE_LOGGER__H__
#include <iostream>
#include <sstream>
#include <vector>
#include <queue>
#include <memory>

#include "base/mutex.h"

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

public:
    std::queue<std::vector<std::string>> m_tasks;
    pthread_t thread_;
    pthread_cond_t condition_;
private:
    const char* file_name_;
    const char* file_path_;
    int max_size_;

    bool need_reopen_ = false;
    std::string data_;
    FILE* file_handle_ = nullptr;
    bool is_stop_ = false;
};

class logger
{
public:
    typedef std::shared_ptr<logger> ptr;

    logger();

    ~logger();

    void init(const char* file_name, const char* file_path, int max_size, int sync_inteval);

    void log();

    void push_log(std::string log_buf);

    void loop();

    void flush();

public:
    std::vector<std::string> buffs_;
private:
    bool is_init = false;
    mutex mutex_;
    async_logger::ptr m_async_log_;
};


#define LOG_DEBUG \
    log_tmp(std::make_shared<log_event>(LogLevel::LogLevel_Dubug, __FILE__, __LINE__, __func__)).stream()
#define LOG_TRACE \
    log_tmp(log_event::ptr(new log_event(LogLevel::LogLevel_Trace, __FILE__, __LINE__, __func__))).stream()
#define LOG_WARN \
    log_tmp(log_event::ptr(new log_event(LogLevel::LogLevel_Warn, __FILE__, __LINE__, __func__))).stream()
#define LOG_ERROR \
    log_tmp(log_event::ptr(new log_event(LogLevel::LogLevel_Error, __FILE__, __LINE__, __func__))).stream()
#define LOG_FATAL \
    log_tmp(log_event::ptr(new log_event(LogLevel::LogLevel_Fatal, __FILE__, __LINE__, __func__))).stream()


#define LOG_SYSERR std::cout
#define LOG_SYSFATAL std::cout
#endif//__BASE_LOGGER__H__