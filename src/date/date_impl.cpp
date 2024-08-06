#include "date_impl.hpp"

#include <iomanip>

namespace tasp
{

/*------------------------------------------------------------------------------
    DateImpl
------------------------------------------------------------------------------*/
DateImpl::DateImpl(const Timepoint &time_point) noexcept
{
    Update(time_point);
}

//------------------------------------------------------------------------------
DateImpl::DateImpl(const DateImpl &) noexcept = default;

//------------------------------------------------------------------------------
DateImpl &DateImpl::operator=(const DateImpl &) noexcept = default;

//------------------------------------------------------------------------------
void DateImpl::Update(const Timepoint &time_point) noexcept
{
    const std::time_t time = std::chrono::system_clock::to_time_t(time_point);
    localtime_r(&time, &timestamp_);  // thread-safe, unlike std::localtime
}

//------------------------------------------------------------------------------
std::string DateImpl::ToString() const noexcept
{
    std::stringstream buf{};
    buf << std::put_time(&timestamp_, "%F %T");
    return buf.str();
}

//------------------------------------------------------------------------------
Json::Value DateImpl::ToJSON() const noexcept
{
    Json::Value date;
    date["year"] = timestamp_.tm_year + 1900;
    date["month"] = timestamp_.tm_mon + 1;
    date["day"] = timestamp_.tm_mday;
    date["hours"] = timestamp_.tm_hour;
    date["minutes"] = timestamp_.tm_min;
    date["seconds"] = timestamp_.tm_sec;
    return date;
}

}  // namespace tasp
