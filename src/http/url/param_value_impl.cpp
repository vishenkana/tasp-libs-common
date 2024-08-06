#include "param_value_impl.hpp"

#include <regex>

using std::regex;
using std::string;
using std::string_view;

namespace tasp::http::url
{
/*------------------------------------------------------------------------------
    ParamValueImpl
------------------------------------------------------------------------------*/
ParamValueImpl::ParamValueImpl(string_view whole_value) noexcept
: whole_value_{whole_value}
, name_(whole_value)
{
    auto colon_position = whole_value.find(':');
    if (colon_position == string::npos)  // if whole_value doesn't have a ':'
    {
        return;
    }

    name_ = whole_value.substr(0, colon_position);
    string post_colon;
    post_colon = whole_value.substr(colon_position + 1);
    auto tilda_position = post_colon.find('~');
    if (tilda_position != string::npos)  // if post_colon has '~' (tilda)
    {
        subvalues_ = {post_colon.substr(0, tilda_position),
                      post_colon.substr(tilda_position + 1)};
        subvalues_type_ = ParamValue::Type::Range;
    }
    else  // splitting by ',' (comma)
    {
        const regex has_comma_regex(",");
        const std::sregex_token_iterator start_token_it(
            post_colon.begin(), post_colon.end(), has_comma_regex, -1);
        const std::sregex_token_iterator end_token_it;
        for (auto it = start_token_it; it != end_token_it; it++)
        {
            subvalues_.push_back(*it);
        }
        subvalues_type_ = ParamValue::Type::Set;
    }
}

//------------------------------------------------------------------------------
ParamValueImpl::~ParamValueImpl() noexcept = default;

//------------------------------------------------------------------------------
const string &ParamValueImpl::Whole() const noexcept
{
    return whole_value_;
}

//------------------------------------------------------------------------------
const string &ParamValueImpl::Name() const noexcept
{
    return name_;
}

//------------------------------------------------------------------------------
const std::vector<string> &ParamValueImpl::Subvalues() const noexcept
{
    return subvalues_;
}

//------------------------------------------------------------------------------
const ParamValue::Type &ParamValueImpl::GetType() const noexcept
{
    return subvalues_type_;
}

//------------------------------------------------------------------------------
string ParamValueImpl::ToSQLCondition() const noexcept
{
    string sql_condition;

    if (subvalues_.empty())
    {
        return sql_condition;
    }

    if (subvalues_type_ == ParamValue::Type::Range)
    {
        // range
        if (!subvalues_.front().empty())
        {
            sql_condition += name_ + " >= '" + subvalues_.front() + "'";
            if (!subvalues_.back().empty())
            {
                sql_condition += " AND ";
            }
        }
        if (!subvalues_.back().empty())
        {
            sql_condition += name_ + " <= '" + subvalues_.back() + "'";
        }
    }
    else
    {
        // set or array
        sql_condition += name_ + " IN (";
        for (auto &&value : subvalues_)
        {
            sql_condition += "'" + value + "',";
        }
        sql_condition.replace(sql_condition.size() - 1, 1, ")");
    }

    return sql_condition;
}

}  // namespace tasp::http::url
