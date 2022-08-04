#ifndef __BASE_LOGGER__H__
#define __BASE_LOGGER__H__
#include <iostream>
#include <sstream>

#include "base/mutex.h"

enum LogLevel
{
    LogLevel_Dubug = 1,
	LogLevel_Trace,
	LogLevel_Warn,
	LogLevel_Error,
	LogLevel_Fatal,
};

#define LOG_DEBUG
    LogTmp(log_event::ptr(new log_event(LogLevel::LogLevel_Trace, __FILE__, __LINE__, __func__))).stream()
#define LOG_TRACE
    LogTmp(log_event::ptr(new log_event(LogLevel::LogLevel_Trace, __FILE__, __LINE__, __func__))).stream()
#define LOG_ERROR
    LogTmp(log_event::ptr(new log_event(LogLevel::LogLevel_Trace, __FILE__, __LINE__, __func__))).stream()
#define LOG_FATAL
    LogTmp(log_event::ptr(new log_event(LogLevel::LogLevel_Trace, __FILE__, __LINE__, __func__))).stream()

class log_event
{
public:
    typedef std::shared_ptr<log_event> ptr;

    log_event(LogLevel level, const char* filename, int line, const char* func_name);

    ~log_event();

    std::stringstream steam();

    void log();
private:
    LogLevel log_level_;
    const char* file_name_;
    int line_;
    const char* func_name_;

    std::stringstream ss_:
};

class logtmp
{
public:
    explicit log_tmp(log_event::ptr event);

    ~log_tmp();

    std::stringstream stream();

private:
    log_event::ptr event_;
};

class async_logger
{
public:
    std::shared_ptr<async_logger> ptr;
};

class logger
{
public:
    std::shared_ptr<logger> ptr;

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
#endif//__BASE_LOGGER__H__