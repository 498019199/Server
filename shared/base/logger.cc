#include "logger.h"

#include <time.h>
#include <sys/time.h>

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


std::stringstream& log_event::stream()
{
    gettimeofday(&timeval_, nullptr);
    struct tm time;
    localtime_r(&(timeval_.tv_sec), &time);
    const char* format = "%Y-%m-%d %H:%M:%S";
    char buf[128];
    strftime(buf, sizeof(buf), format, &time);
    ss_ << "[" << buf << "." << timeval_.tv_usec << "]\t";
    std::string s_level = level_to_string(log_level_);
    ss_ << "[" << s_level << "]\t";

    ss_ << "[" << file_name_ << ":" << line_ << "]\t";

    return ss_;
}

void log_event::log()
{
    ss_ << "/n";
    g_logger->push_log(ss_.str());
}


async_logger::async_logger(const char* file_name, const char* file_path, int max_size)
    :file_name_(file_name), file_path_(file_path),max_size_(max_size)
{
    pthread_create(&thread_, nullptr, &async_logger::excute, this);
}

async_logger::~async_logger()
{

}

void async_logger::push(std::vector<std::string>& buf)
{
    mutex lock;
    m_tasks.push(buf);
}

void async_logger::flush()
{

}

void* async_logger::excute(void* arg)
{
    async_logger* ptr = reinterpret_cast<async_logger*>(arg);
    pthread_cond_init(&ptr->condition_, nullptr);

    while (1)
    {
        mutex lock;
        while (ptr->m_tasks.empty())
        {
            //pthread_cond_wait(&(ptr->condition_), lock);
        }

        std::vector<std::string> tmp;
        tmp.swap(ptr->m_tasks.front());
        ptr->m_tasks.pop();
        bool is_stop = ptr->is_stop_;
        lock.unlock();


        timeval now;
        gettimeofday(&now, nullptr);
        struct tm now_time;
        localtime_r(&(now.tv_sec), &now_time);
        const char *format = "%Y%m%d";
        char date[32];
        strftime(date, sizeof(date), format, &now_time);
        if (ptr->data_ != std::string(date))
        {
            // cross day
            // reset m_no m_date
            //ptr->m_no = 0;
            ptr->data_ = std::string(date);
            ptr->need_reopen_ = true;
        }

        if (!ptr->file_handle_)
        {
            ptr->need_reopen_ = true;
        }

        std::stringstream ss;
        ss << ptr->file_handle_ << ptr->file_name_ << "_" << ptr->data_ << "_" << LogTypeToString(ptr->m_log_type) << "_" << ptr->m_no << ".log";
        std::string full_file_name = ss.str();
    }
}


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