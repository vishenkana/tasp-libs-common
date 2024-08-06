#include "syslog_sink.hpp"

#include <syslog.h>

#include "tasp/config.hpp"

using std::string;
using std::string_view;

namespace tasp
{
/*------------------------------------------------------------------------------
    SyslogSink
------------------------------------------------------------------------------*/
SyslogSink::SyslogSink(string_view config_path) noexcept
: Sink(config_path)
{
    auto &conf{ConfigGlobal::Instance()};
    name_ = conf.Get<string>("program.name");

    openlog(name_.c_str(), LOG_PID, LOG_USER);
}

//------------------------------------------------------------------------------
SyslogSink::~SyslogSink() noexcept
{
    closelog();
}

//------------------------------------------------------------------------------
void SyslogSink::PrintImpl(const LogLine &line) noexcept
{
    syslog(ConvertLevelToSyslog(line.Level()), "%s", line.Message().c_str());
}

//------------------------------------------------------------------------------
inline int SyslogSink::ConvertLevelToSyslog(const LogLevel &level) noexcept
{
    auto iter = level_syslog_.find(level.Get());
    if (iter == level_syslog_.end())
    {
        return LOG_INFO;
    }

    return iter->second;
}

namespace
{
//------------------------------------------------------------------------------
/**
 * @brief Инициализация уровней лога с их значениями из Syslog.
 *
 * @return Список уровней лога
 */
inline SyslogSink::LevelList LevelInitialization() noexcept
{
    SyslogSink::LevelList list = {
        {LogLevel::Level::Debug, LOG_DEBUG},
        {LogLevel::Level::Info, LOG_INFO},
        {LogLevel::Level::Warning, LOG_WARNING},
        {LogLevel::Level::Error, LOG_ERR},
        {LogLevel::Level::None, LOG_INFO},
    };
    return list;
}
}  // namespace

//------------------------------------------------------------------------------
const SyslogSink::LevelList SyslogSink::level_syslog_{LevelInitialization()};

}  // namespace tasp
