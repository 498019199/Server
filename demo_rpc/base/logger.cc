#include "logger.h"
#include "../../network/include/timer.h"

#include <algorithm>
#include <atomic>
#include <fcntl.h>
#include <signal.h>
#include <time.h>
#include <sys/time.h>
#include <pthread.h>
#include <iostream>

extern logger::ptr g_logger;
void core_dump_handler(int signal_no)
{
    LOG_ERROR << "progress received invalid signal, will exit";
    printf("progress received invalid signal, will exit\n");
    g_logger->flush();
    pthread_join(g_logger->get_async_logger()->thread_, nullptr);

    signal(signal_no, SIG_DFL);
    raise(signal_no);
}

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
        :file_name_(file_name),
        file_path_(file_path),
        max_size_(max_size)
{
    pthread_create(&thread_, nullptr, &async_logger::excute, this);
}

async_logger::~async_logger()
{
}

void async_logger::push(std::vector<std::string>& buf)
{
    mutex_.lock();
    m_tasks.push(buf);
    mutex_.unlock();
    pthread_cond_signal(&condition_);
}

void async_logger::flush()
{
    if (file_handle_)
    {
        fflush(file_handle_);
    }
}

void* async_logger::excute(void* arg)
{
    auto ptr = reinterpret_cast<async_logger*>(arg);
    pthread_cond_init(&ptr->condition_, NULL);
    while (1)
    {
        ptr->mutex_.lock();
        while (ptr->m_tasks.empty())
        {
            pthread_cond_wait(&ptr->condition_, ptr->mutex_.get_mutex());
        }

        std::vector<std::string> tmp;
        tmp.swap(ptr->m_tasks.front());
        ptr->m_tasks.pop();
        bool is_stop = ptr->is_stop_;
        ptr->mutex_.unlock();


        timeval now{};
        gettimeofday(&now, nullptr);
        struct tm now_time{};
        localtime_r(&(now.tv_sec), &now_time);
        const char *format = "%Y%m%d";
        char date[32];
        strftime(date, sizeof(date), format, &now_time);
        if (ptr->data_ != std::string(date))
        {
            // cross day
            // reset no_ m_date
            ptr->no_ = 0;
            ptr->data_ = std::string(date);
            ptr->need_reopen_ = true;
        }

        if (!ptr->file_handle_)
        {
            ptr->need_reopen_ = true;
        }

        std::stringstream ss;
        ss << ptr->file_handle_ << ptr->file_name_ << "_" << ptr->data_ << "_" << ptr->no_ << ".log";
        std::string full_file_name = ss.str();

        if (ptr->need_reopen_)
        {
            if (ptr->file_handle_)
            {
                fclose(ptr->file_handle_);
            }

            ptr->file_handle_ = fopen(full_file_name.c_str(), "a");
            ptr->need_reopen_ = false;
        }

        if (ftell(ptr->file_handle_) > ptr->max_size_)
        {
            fclose(ptr->file_handle_);

            // single log file over max size
            ptr->no_++;
            std::stringstream ss2;
            ss2 << ptr->file_path_ << ptr->file_name_ << "_" << ptr->data_  << "_" << ptr->no_ << ".log";
            full_file_name = ss2.str();

            // printf("open file %s", full_file_name.c_str());
            ptr->file_handle_ = fopen(full_file_name.c_str(), "a");
            ptr->need_reopen_ = false;
        }

        if (!ptr->file_handle_)
        {
            printf("open log file %s error!", full_file_name.c_str());
        }

        for(auto i : tmp)
        {
            fwrite(i.c_str(), 1, i.length(), ptr->file_handle_);
#if defined(MODE_DEBUG)
            std::cout << i.c_str() << std::endl;
#endif//MODE_DEBUG
            // printf("succ write rt %d bytes ,[%s] to file[%s]", rt, i.c_str(), full_file_name.c_str());
        }
        fflush(ptr->file_handle_);
        if (is_stop)
        {
            break;
        }
    }

    if (!ptr->file_handle_)
    {
        fclose(ptr->file_handle_);
    }
    return nullptr;
}

void async_logger::stop()
{
    if (!is_stop_)
    {
        is_stop_ = true;
    }
}

logger::logger()
{}

logger::~logger()
{
    flush();
    pthread_join(async_log_->thread_, nullptr);
}

void logger::init(const char* file_name, const char* file_path, int max_size, int sync_inteval)
{
    if (!is_init_)
    {
        async_log_ = std::make_shared<async_logger>(file_name, file_path, max_size);
        signal(SIGSEGV, core_dump_handler);
        signal(SIGABRT, core_dump_handler);
        signal(SIGTERM, core_dump_handler);
        signal(SIGKILL, core_dump_handler);
        signal(SIGINT, core_dump_handler);
        signal(SIGSTKFLT, core_dump_handler);

        // ignore SIGPIPE
        signal(SIGPIPE, SIG_IGN);
        is_init_ = true;
    }
}

void logger::log()
{}

void logger::push_log(const std::string& msg)
{
    mutex_.lock();
    buffs_.push_back(msg);
    mutex_.unlock();

    loop();
}

void logger::loop()
{
    std::vector<std::string> tmp;
    mutex_.lock();
    tmp.swap(buffs_);
    mutex_.unlock();

    async_log_->push(tmp);
}

void logger::flush()
{
    loop();
    async_log_->stop();
    async_log_->flush();
}

