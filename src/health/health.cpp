#include "tasp/health.hpp"

#include "health_impl.hpp"

namespace tasp
{
/*------------------------------------------------------------------------------
    HealthReport
------------------------------------------------------------------------------*/
HealthReport::HealthReport(std::string_view name,
                           const Status &status,
                           std::string_view message) noexcept
: impl_(std::make_unique<HealthReportImpl>(name, status, message))
{
}

//------------------------------------------------------------------------------
HealthReport::~HealthReport() noexcept = default;

//------------------------------------------------------------------------------
HealthReport::HealthReport(const HealthReport &report)
: impl_(std::make_unique<HealthReportImpl>(*report.impl_))
{
}

//-----------------------------------------------------------------------------
HealthReport::HealthReport(HealthReport &&) noexcept = default;

//------------------------------------------------------------------------------
HealthReport &HealthReport::operator=(const HealthReport &report)
{
    if (&report != this)
    {
        impl_ = std::make_unique<HealthReportImpl>(*report.impl_);
    }
    return *this;
}

//------------------------------------------------------------------------------
HealthReport &HealthReport::operator=(HealthReport &&) noexcept = default;

//------------------------------------------------------------------------------
const HealthReport::Status &HealthReport::GetStatus() const noexcept
{
    return impl_->GetStatus();
}

//------------------------------------------------------------------------------
Json::Value HealthReport::ToJSON() const noexcept
{
    return impl_->ToJSON();
}

}  // namespace tasp
