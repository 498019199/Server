#include "logger.h"

extern logger::ptr g_logger;
std::string level_to_string(LogLevel level)
{
    std::string re = "DEBUG";
    switch(level)
    {
    case LogLevel_Dubug:
        re = "DEBUG";
        return re;

    case LogLevel_Trace:
        re = "TRACE";
        return re;

    case LogLevel_Warn:
        re = "WARN";
        return re;

    case LogLevel_Error:
        re = "ERROR";
        return re;

    case LogLevel_Fatal:
        re = "FATAL";
        return re;

    default:
        return re;
    }
}

LogLevel string_to_level(const std::string& str) 
{
    if (str == "DEBUG")
        return LogLevel::LogLevel_Dubug;
    else if (str == "TRACE")
        return LogLevel::LogLevel_Trace;
    else if (str == "WARN")
        return LogLevel::LogLevel_Warn;
    else if (str == "ERROR")
        return LogLevel::LogLevel_Error;
    else if (str == "FATAL")
        return LogLevel::LogLevel_Fatal;
    return LogLevel::LogLevel_Dubug;
}

log_event::log_event(LogLevel level, const char* filename, int line, const char* func_name)
    :log_level_(level), file_name_(filename), line_(line)
{
    
}

log_event::~log_event()
{
    
}

std::stringstream log_event::steam()
{
    
}

void log_event::log()
{
   
}




logtmp::log_tmp(log_event::ptr event)
    :event_(event)
{
    
}

logtmp::~log_tmp()
{
    event_->log();
}

std::stringstream logtmp::stream()
{
    return event_->stream();
}


async_logger::async_logger(const char* file_name, const char* file_path, int max_size){}

async_logger::~async_logger(){}

void async_logger::push(std::vector<std::string>& buf){}

void async_logger::flush(){}

void* async_logger::excute(void*){}




logger::logger()
{}

logger::~logger()
{}

void logger::init(const char* file_name, const char* file_path, int max_size, int sync_inteval)
{}

void logger::log()
{}

void logger::push_log(std::string log_buf)
{}

void logger::loop()
{}

void logger::flush()
{}