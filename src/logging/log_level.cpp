#include "log_level.hpp"

#include <algorithm>

using std::equal;
using std::find_if;
using std::string;
using std::string_view;

namespace tasp
{
/*------------------------------------------------------------------------------
    LogLevel
------------------------------------------------------------------------------*/
LogLevel::LogLevel(Level level) noexcept
{
    Set(level);
}

//------------------------------------------------------------------------------
LogLevel::LogLevel(string_view level) noexcept
{
    Set(level);
}

//------------------------------------------------------------------------------
LogLevel::LogLevel(const LogLevel &level) = default;

//------------------------------------------------------------------------------
void LogLevel::Set(Level level) noexcept
{
    value_ = level;
}

//------------------------------------------------------------------------------
void LogLevel::Set(string_view level) noexcept
{
    auto case_equal = [&level](const decltype(level_string_)::value_type &value)
    {
        return equal(level.begin(),
                     level.end(),
                     value.second.begin(),
                     value.second.end(),
                     [](char first, char second)
                     {
                         return tolower(first) == tolower(second);
                     });
    };

    auto iter = find_if(level_string_.begin(), level_string_.end(), case_equal);
    if (iter != level_string_.end())
    {
        value_ = iter->first;
    }
}

//------------------------------------------------------------------------------
LogLevel::Level LogLevel::Get() const noexcept
{
    return value_;
}

//------------------------------------------------------------------------------
string LogLevel::ToString() const noexcept
{
    auto iter = level_string_.find(value_);
    if (iter == level_string_.end())
    {
        return "None";
    }

    return iter->second;
}

//------------------------------------------------------------------------------
bool LogLevel::operator==(const LogLevel &rhs) const noexcept
{
    return value_ == rhs.value_;
}

//------------------------------------------------------------------------------
bool LogLevel::operator!=(const LogLevel &rhs) const noexcept
{
    return value_ != rhs.value_;
}

//------------------------------------------------------------------------------
bool LogLevel::operator<(const LogLevel &rhs) const noexcept
{
    return value_ < rhs.value_;
}

//------------------------------------------------------------------------------
bool LogLevel::operator<=(const LogLevel &rhs) const noexcept
{
    return value_ <= rhs.value_;
}

//------------------------------------------------------------------------------
bool LogLevel::operator>(const LogLevel &rhs) const noexcept
{
    return value_ > rhs.value_;
}

//------------------------------------------------------------------------------
bool LogLevel::operator>=(const LogLevel &rhs) const noexcept
{
    return value_ >= rhs.value_;
}

//------------------------------------------------------------------------------
bool LogLevel::operator==(LogLevel::Level rhs) const noexcept
{
    return value_ == rhs;
}

//------------------------------------------------------------------------------
bool LogLevel::operator!=(LogLevel::Level rhs) const noexcept
{
    return value_ != rhs;
}

//------------------------------------------------------------------------------
bool LogLevel::operator<(LogLevel::Level rhs) const noexcept
{
    return value_ < rhs;
}

//------------------------------------------------------------------------------
bool LogLevel::operator<=(LogLevel::Level rhs) const noexcept
{
    return value_ <= rhs;
}

//------------------------------------------------------------------------------
bool LogLevel::operator>(LogLevel::Level rhs) const noexcept
{
    return value_ > rhs;
}

//------------------------------------------------------------------------------
bool LogLevel::operator>=(LogLevel::Level rhs) const noexcept
{
    return value_ >= rhs;
}

namespace
{
//------------------------------------------------------------------------------
/**
 * @brief Инициализация уровней лога с их текстовыми значениями.
 *
 * @return Список уровней лога
 */
inline LogLevel::LevelList LevelInitialization() noexcept
{
    LogLevel::LevelList list = {
        {LogLevel::Level::Debug, "Debug"},
        {LogLevel::Level::Info, "Info"},
        {LogLevel::Level::Warning, "Warning"},
        {LogLevel::Level::Error, "Error"},
        {LogLevel::Level::None, "None"},
    };
    return list;
}
}  // namespace

//------------------------------------------------------------------------------
const LogLevel::LevelList LogLevel::level_string_{LevelInitialization()};

}  // namespace tasp
