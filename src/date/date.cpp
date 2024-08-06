#include <tasp/date.hpp>

#include "date_impl.hpp"

namespace tasp
{

/*------------------------------------------------------------------------------
    Date
------------------------------------------------------------------------------*/
Date::Date(const Timepoint &time_point) noexcept
: impl_(std::make_unique<DateImpl>(time_point))
{
}

//------------------------------------------------------------------------------
Date::~Date() noexcept = default;

//------------------------------------------------------------------------------
Date::Date(const Date &date)
: impl_(std::make_unique<DateImpl>(*date.impl_))
{
}

//------------------------------------------------------------------------------
Date::Date(Date &&) noexcept = default;

//------------------------------------------------------------------------------
Date &Date::operator=(const Date &date)
{
    if (&date != this)
    {
        impl_ = std::make_unique<DateImpl>(*date.impl_);
    }
    return *this;
}

//------------------------------------------------------------------------------
Date &Date::operator=(Date &&) noexcept = default;

//------------------------------------------------------------------------------
void Date::Update(const Timepoint &time_point) noexcept
{
    impl_->Update(time_point);
}

//------------------------------------------------------------------------------
std::string Date::ToString() const noexcept
{
    return impl_->ToString();
}

//------------------------------------------------------------------------------
Json::Value Date::ToJSON() const noexcept
{
    return impl_->ToJSON();
}

}  // namespace tasp
