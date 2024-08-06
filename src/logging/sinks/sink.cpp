#include "sink.hpp"

#include "console_sink.hpp"
#include "file_sink.hpp"
#include "syslog_sink.hpp"
#include "tasp/config.hpp"

using std::make_unique;
using std::string;
using std::string_view;
using std::unique_ptr;

namespace tasp
{
/*------------------------------------------------------------------------------
    Sink
------------------------------------------------------------------------------*/
Sink::Sink(string_view config_path) noexcept
: config_path_(config_path)
{
    auto &conf{ConfigGlobal::Instance()};

    level_.Set(conf.Get<string>(config_path_ + ".level", "Info"));
}

//------------------------------------------------------------------------------
Sink::~Sink() noexcept = default;

//------------------------------------------------------------------------------
void Sink::Print(const LogLine &line) noexcept
{
    if (level_ <= line.Level())
    {
        PrintImpl(line);
    }
}

//------------------------------------------------------------------------------
const LogLevel &Sink::Level() const noexcept
{
    return level_;
}

//------------------------------------------------------------------------------
const string &Sink::ConfigPath() const noexcept
{
    return config_path_;
}

/*------------------------------------------------------------------------------
    SinkFactory
------------------------------------------------------------------------------*/
SinkFactory::SinkFactory() noexcept
{
    types_["syslog"] = [](string_view config_path)
    {
        return make_unique<SyslogSink>(config_path);
    };

    types_["console"] = [](string_view config_path)
    {
        return make_unique<ConsoleSink>(config_path);
    };

    types_["file"] = [](string_view config_path)
    {
        return make_unique<FileSink>(config_path);
    };
}

//------------------------------------------------------------------------------
SinkFactory::~SinkFactory() noexcept = default;

//------------------------------------------------------------------------------
unique_ptr<Sink> SinkFactory::Create(string_view type,
                                     string_view config_path) const noexcept
{
    auto element = types_.find(type.data());
    if (element != types_.end())
    {
        return element->second(config_path);
    }

    return nullptr;
}

}  // namespace tasp
