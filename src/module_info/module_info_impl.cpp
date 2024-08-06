#include "module_info_impl.hpp"

#include <algorithm>

using std::make_shared;
using std::shared_ptr;
using std::string;
using std::string_view;
using std::vector;

namespace tasp
{
/*------------------------------------------------------------------------------
    ModuleInfoImpl
------------------------------------------------------------------------------*/

//------------------------------------------------------------------------------
ModuleInfoImpl::ModuleInfoImpl(string_view name,
                               string_view version,
                               string_view desc) noexcept
: name_(name)
, version_(version)
, desc_(desc)
{
}

//------------------------------------------------------------------------------
ModuleInfoImpl::~ModuleInfoImpl() noexcept = default;

//------------------------------------------------------------------------------
[[nodiscard]] const string &ModuleInfoImpl::GetName() const noexcept
{
    return name_;
}

//------------------------------------------------------------------------------
[[nodiscard]] const string &ModuleInfoImpl::GetVersion() const noexcept
{
    return version_;
}

//------------------------------------------------------------------------------
[[nodiscard]] const string &ModuleInfoImpl::GetDescription() const noexcept
{
    return desc_;
}

/*------------------------------------------------------------------------------
    ModuleInfoCollectorImpl
------------------------------------------------------------------------------*/
ModuleInfoCollectorImpl::ModuleInfoCollectorImpl() noexcept = default;

//------------------------------------------------------------------------------
ModuleInfoCollectorImpl::~ModuleInfoCollectorImpl() noexcept = default;

//------------------------------------------------------------------------------
void ModuleInfoCollectorImpl::AddInfo(string_view name,
                                      string_view version,
                                      string_view desc) noexcept
{
    if (GetInfoByName(name)->GetName().empty())
    {
        info_vec_.push_back(make_shared<ModuleInfoImpl>(name, version, desc));
    }
}

//------------------------------------------------------------------------------
[[nodiscard]] shared_ptr<ModuleInfo> ModuleInfoCollectorImpl::GetInfoByName(
    string_view name) const noexcept
{
    auto info_it = std::find_if(std::begin(info_vec_),
                                std::end(info_vec_),
                                [name](const shared_ptr<ModuleInfo> &info)
                                {
                                    return info->GetName() == name;
                                });
    if (info_it != std::end(info_vec_))
    {
        return *info_it;
    }
    // возвращаем указатель на пустой объект (без информации), если искомый
    // модуль не был найден
    return make_shared<ModuleInfoImpl>();
}

// Выключается проверка стиля наименований для этого участка, т.к. это
// методы для использования в стандартной библиотеке c++.
// NOLINTBEGIN(readability-identifier-naming)
//------------------------------------------------------------------------------
[[nodiscard]] vector<shared_ptr<ModuleInfo>>::const_iterator
ModuleInfoCollectorImpl::begin() const noexcept
{
    return info_vec_.cbegin();
}

//------------------------------------------------------------------------------
[[nodiscard]] vector<shared_ptr<ModuleInfo>>::const_iterator
ModuleInfoCollectorImpl::end() const noexcept
{
    return info_vec_.cend();
}
// NOLINTEND(readability-identifier-naming)

}  // namespace tasp
