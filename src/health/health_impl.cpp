#include "health_impl.hpp"

namespace tasp
{
/*------------------------------------------------------------------------------
    HealthReportImpl
------------------------------------------------------------------------------*/
HealthReportImpl::HealthReportImpl(std::string_view name,
                                   const HealthReport::Status &status,
                                   std::string_view message) noexcept
: name_(name)
, status_(status)
, message_(message)
{
}

//------------------------------------------------------------------------------
HealthReportImpl::~HealthReportImpl() noexcept = default;

//------------------------------------------------------------------------------
HealthReportImpl::HealthReportImpl(const HealthReportImpl &) noexcept = default;

//------------------------------------------------------------------------------
HealthReportImpl &HealthReportImpl::operator=(
    const HealthReportImpl &) noexcept = default;

//------------------------------------------------------------------------------
const HealthReport::Status &HealthReportImpl::GetStatus() const noexcept
{
    return status_;
}

//------------------------------------------------------------------------------
Json::Value HealthReportImpl::ToJSON() const noexcept
{
    Json::Value health_report;
    health_report["name"] = name_;
    health_report["status"] = HealthReport::StatusToString(status_);
    health_report["date"] = date_.ToJSON();
    if (!message_.empty())
    {
        health_report["message"] = message_;
    }
    return health_report;
}

}  // namespace tasp
