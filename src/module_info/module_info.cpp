#include "tasp/module_info.hpp"

#include "module_info_impl.hpp"

using std::make_unique;
using std::shared_ptr;
using std::string_view;
using std::vector;

namespace tasp
{
/*------------------------------------------------------------------------------
    ModuleInfo
------------------------------------------------------------------------------*/
ModuleInfo::ModuleInfo() noexcept = default;

//------------------------------------------------------------------------------
ModuleInfo::~ModuleInfo() noexcept = default;

/*------------------------------------------------------------------------------
    ModuleInfoCollector
------------------------------------------------------------------------------*/
ModuleInfoCollector::ModuleInfoCollector() noexcept
: impl_(make_unique<ModuleInfoCollectorImpl>())
{
}

//------------------------------------------------------------------------------
ModuleInfoCollector::~ModuleInfoCollector() noexcept = default;

//------------------------------------------------------------------------------
ModuleInfoCollector &ModuleInfoCollector::Instance() noexcept
{
    static ModuleInfoCollector instance;
    return instance;
}

//------------------------------------------------------------------------------
void ModuleInfoCollector::AddInfo(string_view name,
                                  string_view version,
                                  string_view desc) noexcept
{
    impl_->AddInfo(name, version, desc);
}

//------------------------------------------------------------------------------
[[nodiscard]] shared_ptr<ModuleInfo> ModuleInfoCollector::GetInfoByName(
    string_view name) const noexcept
{
    return impl_->GetInfoByName(name);
}

// Выключается проверка стиля наименований для этого участка, т.к. это
// методы для использования в стандартной библиотеке c++.
// NOLINTBEGIN(readability-identifier-naming)
//------------------------------------------------------------------------------
[[nodiscard]] vector<shared_ptr<ModuleInfo>>::const_iterator
ModuleInfoCollector::begin() const noexcept
{
    return impl_->begin();
}

//------------------------------------------------------------------------------
[[nodiscard]] vector<shared_ptr<ModuleInfo>>::const_iterator
ModuleInfoCollector::end() const noexcept
{
    return impl_->end();
}
// NOLINTEND(readability-identifier-naming)

}  // namespace tasp
