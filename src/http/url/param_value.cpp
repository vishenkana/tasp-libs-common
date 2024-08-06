#include "tasp/http/url/param_value.hpp"

#include "param_value_impl.hpp"

namespace tasp::http::url
{
/*------------------------------------------------------------------------------
    ParamValue
------------------------------------------------------------------------------*/
ParamValue::ParamValue(std::string_view whole_value) noexcept
: impl_(std::make_unique<ParamValueImpl>(whole_value))
{
}

//------------------------------------------------------------------------------
ParamValue::~ParamValue() noexcept = default;

//------------------------------------------------------------------------------
const std::string &ParamValue::Whole() const noexcept
{
    return impl_->Whole();
}

//------------------------------------------------------------------------------
const std::string &ParamValue::Name() const noexcept
{
    return impl_->Name();
}

//------------------------------------------------------------------------------
const std::vector<std::string> &ParamValue::Subvalues() const noexcept
{
    return impl_->Subvalues();
}

//------------------------------------------------------------------------------
const ParamValue::Type &ParamValue::GetType() const noexcept
{
    return impl_->GetType();
}

//------------------------------------------------------------------------------
std::string ParamValue::ToSQLCondition() const noexcept
{
    return impl_->ToSQLCondition();
}

}  // namespace tasp::http::url
