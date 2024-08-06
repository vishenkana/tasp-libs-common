#include "logging_impl.hpp"

#include "tasp/config.hpp"

using std::make_unique;
using std::scoped_lock;
using std::string;
using std::thread;
using std::unique_lock;
using std::vector;
using std::chrono::seconds;

namespace tasp
{
/*------------------------------------------------------------------------------
    LoggingImpl
------------------------------------------------------------------------------*/
LoggingImpl::LoggingImpl() noexcept
{
    Print(LogLine(LogLevel::Level::Info, "Начало логирования"));
    thread_ = make_unique<thread>(&LoggingImpl::Worker, this);
}

//------------------------------------------------------------------------------
LoggingImpl::~LoggingImpl() noexcept
{
    ChangeStatus(Status::NeedStop);

    thread_->join();
    thread_.reset(nullptr);
}

//------------------------------------------------------------------------------
void LoggingImpl::Print(const LogLine &line) noexcept
{
    const scoped_lock lock{mutex_};
    messages_.push(line);
}

//------------------------------------------------------------------------------
void LoggingImpl::Reload() noexcept
{
    Print(LogLine(LogLevel::Level::Info, "Перезапуск логирования"));
    ChangeStatus(Status::NeedReload);
}

//------------------------------------------------------------------------------
void LoggingImpl::Worker() noexcept
{
    ReloadImpl();

    while (status_ != Status::NeedStop)
    {
        unique_lock condition_lock{condition_mutex_};
        condition_.wait_for(condition_lock,
                            timeout_,
                            [&]()
                            {
                                return status_ != Status::Work;
                            });

        PrintImpl();

        if (status_ == Status::NeedReload)
        {
            ReloadImpl();
        }
    }

    Print(LogLine(LogLevel::Level::Info, "Завершение логирования"));

    PrintImpl();

    status_ = Status::Stop;
}

//------------------------------------------------------------------------------
void LoggingImpl::ChangeStatus(LoggingImpl::Status status) noexcept
{
    if (status_ < Status::NeedStop)
    {
        status_ = status;

        condition_.notify_one();
    }
}

//------------------------------------------------------------------------------
void LoggingImpl::PrintImpl() noexcept
{
    const scoped_lock lock{mutex_};
    while (!messages_.empty())
    {
        const auto &message{messages_.front()};
        for (const auto &sink : sinks_)
        {
            sink->Print(message);
        }
        messages_.pop();
    }
}

//------------------------------------------------------------------------------
void LoggingImpl::ReloadImpl() noexcept
{
    sinks_.clear();

    auto &conf{ConfigGlobal::Instance()};

    const seconds default_timeout{5};
    timeout_ = conf.Get("logging.timeout", default_timeout);

    const string sinks_path{"logging.sinks."};
    auto types{conf.Get<vector<string>>(sinks_path, {"file"})};
    for (const auto &type : types)
    {
        auto enable = conf.Get(sinks_path + type + ".enable", true);
        if (enable)
        {
            auto log{factory_.Create(type, sinks_path + type)};
            if (log != nullptr)
            {
                sinks_.push_back(std::move(log));
            }
        }
    }

    ChangeStatus(Status::Work);
}

/*------------------------------------------------------------------------------
    FormatWithLocationImpl
------------------------------------------------------------------------------*/
FormatWithLocationImpl::FormatWithLocationImpl(
    const char *format,
    const SourceLocation &location) noexcept
: format_(format)
, location_(location)
{
}

//------------------------------------------------------------------------------
FormatWithLocationImpl::~FormatWithLocationImpl() noexcept = default;

//------------------------------------------------------------------------------
string FormatWithLocationImpl::Format() const noexcept
{
    return format_;
}

//------------------------------------------------------------------------------
SourceLocation FormatWithLocationImpl::Location() const noexcept
{
    return location_;
}

}  // namespace tasp
