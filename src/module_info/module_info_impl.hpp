/**
 * @file
 * @brief Реализация интерфейсов для обработки информации о модулях (хранение и
 * выдача по запросу).
 */

#ifndef TASP_MODULE_INFO_MODULE_INFO_IMPL_HPP_
#define TASP_MODULE_INFO_MODULE_INFO_IMPL_HPP_

#include <string>
#include <string_view>
#include <vector>

#include "tasp/module_info.hpp"

namespace tasp
{

/**
 * @brief Реализация интерфейса работы с информацией о модуле.
 *
 * В информацию входят: название модуля, версия, описание.
 *
 * Данный интерфейс предполагает наличие функций получения названия, версии и
 * описания модуля (GetName, GetVersion, GetDescription соответственно).
 */
class ModuleInfoImpl final : public ModuleInfo
{
public:
    /**
     * @brief Конструктор с параметрами.
     *
     * Если параметры не переданы, то значения инициализируются по умолчанию.
     *
     * @param name Входящее название модуля.
     *
     * @param version Входящее значение версии модуля.
     *
     * @param desc Входящее описание модуля.
     */
    explicit ModuleInfoImpl(std::string_view name = {},
                            std::string_view version = {},
                            std::string_view desc = {}) noexcept;

    /**
     * @brief Деструктор.
     */
    ~ModuleInfoImpl() noexcept override;

    /**
     * @brief Получить название модуля.
     *
     * @return Строка с названием модуля.
     */
    [[nodiscard]] const std::string &GetName() const noexcept override;

    /**
     * @brief Получить версию модуля.
     *
     * @return Строка с версией модуля.
     */
    [[nodiscard]] const std::string &GetVersion() const noexcept override;

    /**
     * @brief Получить описание модуля.
     *
     * @return Строка с описанием модуля.
     */
    [[nodiscard]] const std::string &GetDescription() const noexcept override;

    ModuleInfoImpl(const ModuleInfoImpl &) = delete;
    ModuleInfoImpl(ModuleInfoImpl &&) = delete;
    ModuleInfoImpl &operator=(const ModuleInfoImpl &) = delete;
    ModuleInfoImpl &operator=(ModuleInfoImpl &&) = delete;

private:
    /**
     * @brief Название модуля.
     */
    std::string name_;

    /**
     * @brief Версия модуля.
     */
    std::string version_;

    /**
     * @brief Описание модуля.
     */
    std::string desc_;
};

/**
 * @brief Реализация интерфейса обработчика информации о модулях.
 *
 * Данный интерфейс предполагает наличие функций добавления информации о модуле
 * (AddInfo) и выдачи конкретной информации по названию модуля (GetInfoByName).
 */
class ModuleInfoCollectorImpl final
{
public:
    /**
     * @brief Конструктор.
     */
    ModuleInfoCollectorImpl() noexcept;

    /**
     * @brief Деструктор.
     */
    ~ModuleInfoCollectorImpl() noexcept;

    /**
     * @brief Добавить в обработчик информацию о конкретном модуле.
     *
     * @param name Входящее название модуля.
     *
     * @param version Входящее значение версии модуля.
     *
     * @param desc Входящее описание модуля.
     */
    void AddInfo(std::string_view name,
                 std::string_view version,
                 std::string_view desc) noexcept;

    /**
     * @brief Выдача конкретной информации по названию модуля.
     *
     * @param name Название искомого модуля.
     *
     * @return Указатель на объект с информацией о модуле.
     */
    [[nodiscard]] std::shared_ptr<ModuleInfo> GetInfoByName(
        std::string_view name) const noexcept;

    // Выключается проверка стиля наименований для этого участка, т.к. это
    // методы для использования в стандартной библиотеке c++.
    // NOLINTBEGIN(readability-identifier-naming)
    /**
     * @brief Метод begin() для работы с range-based for.
     *
     * @return Константный итератор на начало контейнера.
     */
    [[nodiscard]] std::vector<std::shared_ptr<ModuleInfo>>::const_iterator
    begin() const noexcept;

    /**
     * @brief Метод end() для работы с range-based for.
     *
     * @return Константный итератор на конец контейнера.
     */
    [[nodiscard]] std::vector<std::shared_ptr<ModuleInfo>>::const_iterator end()
        const noexcept;
    // NOLINTEND(readability-identifier-naming)

    ModuleInfoCollectorImpl(const ModuleInfoCollectorImpl &) = delete;
    ModuleInfoCollectorImpl(ModuleInfoCollectorImpl &&) = delete;
    ModuleInfoCollectorImpl &operator=(const ModuleInfoCollectorImpl &) =
        delete;
    ModuleInfoCollectorImpl &operator=(ModuleInfoCollectorImpl &&) = delete;

private:
    /**
     * @brief Контейнер для хранения информации по всем модулям.
     */
    std::vector<std::shared_ptr<ModuleInfo>> info_vec_;
};

}  // namespace tasp

#endif  // TASP_MODULE_INFO_MODULE_INFO_IMPL_HPP_
