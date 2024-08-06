#include "console_sink.hpp"

#include <iomanip>
#include <iostream>

using std::cout;
using std::setw;
using std::string;
using std::string_view;

namespace tasp
{
/*------------------------------------------------------------------------------
    ConsoleSink
------------------------------------------------------------------------------*/
ConsoleSink::ConsoleSink(string_view config_path) noexcept
: Sink(config_path)
{
}

//------------------------------------------------------------------------------
ConsoleSink::~ConsoleSink() noexcept = default;

//------------------------------------------------------------------------------
void ConsoleSink::PrintImpl(const LogLine &line) noexcept
{
    const int level_width{16};

    cout << line.Timestamp() << " ";
    cout << setw(level_width) << ToColorLogLevel(line.Level()) << " ";
    cout << line.Message();
    cout << '\n';
}

//------------------------------------------------------------------------------
string ConsoleSink::ToColorLogLevel(const LogLevel &level) noexcept
{
    string color{"37"};
    switch (level.Get())
    {
        case LogLevel::Level::Error:
            color = "31";
            break;
        case LogLevel::Level::Warning:
            color = "33";
            break;
        case LogLevel::Level::Debug:
            color = "34";
            break;
        default:
            break;
    }
    return "\033[" + color + "m" + level.ToString() + "\033[0m";
}

}  // namespace tasp
