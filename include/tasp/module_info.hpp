/**
 * @file
 * @brief Интерфейсы для обработки информации о модулях (хранение и выдача по
 * запросу).
 */

#ifndef TASP_MODULE_INFO_HPP_
#define TASP_MODULE_INFO_HPP_

#include <memory>
#include <string_view>
#include <vector>

namespace tasp
{

/**
 * @brief Интерфейс работы с информацией о модуле.
 *
 * В информацию входят: название модуля, версия, описание.
 *
 * Данный интерфейс предполагает наличие функций получения названия, версии и
 * описания модуля (GetName, GetVersion, GetDescription соответственно).
 */
class [[gnu::visibility("default")]] ModuleInfo
{
public:
    /**
     * @brief Конструктор.
     */
    ModuleInfo() noexcept;

    /**
     * @brief Деструктор.
     */
    virtual ~ModuleInfo() noexcept;

    /**
     * @brief Получить название модуля.
     *
     * @return Строка с названием модуля.
     */
    [[nodiscard]] virtual const std::string &GetName() const noexcept = 0;

    /**
     * @brief Получить версию модуля.
     *
     * @return Строка с версией модуля.
     */
    [[nodiscard]] virtual const std::string &GetVersion() const noexcept = 0;

    /**
     * @brief Получить описание модуля.
     *
     * @return Строка с описанием модуля.
     */
    [[nodiscard]] virtual const std::string &GetDescription()
        const noexcept = 0;

    ModuleInfo(const ModuleInfo &) = delete;
    ModuleInfo(ModuleInfo &&) = delete;
    ModuleInfo &operator=(const ModuleInfo &) = delete;
    ModuleInfo &operator=(ModuleInfo &&) = delete;
};

class ModuleInfoCollectorImpl;

/**
 * @brief Интерфейс обработчика информации о модулях.
 *
 * Класс реализован с использованием паттерна Singleton (цель - создать единый
 * объект-обработчик). Также применена идиома PIMPL (Pointer to Implementation –
 * указатель на реализацию), при помощи которой класс скрывает от пользователя
 * свою реализацию.
 *
 * Данный интерфейс предполагает наличие функций добавления информации о модуле
 * (AddInfo) и выдачи конкретной информации по названию модуля (GetInfoByName).
 */
class [[gnu::visibility("default")]] ModuleInfoCollector final
{
public:
    /**
     * @brief Получить (или создать) единственный экземпляр обработчика.
     *
     * @return Ссылка на единственный экземпляр обработчика.
     */
    static ModuleInfoCollector &Instance() noexcept;

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

    ModuleInfoCollector(const ModuleInfoCollector &) = delete;
    ModuleInfoCollector(ModuleInfoCollector &&) = delete;
    ModuleInfoCollector &operator=(const ModuleInfoCollector &) = delete;
    ModuleInfoCollector &operator=(ModuleInfoCollector &&) = delete;

private:
    /**
     * @brief Указатель на реализацию.
     */
    std::unique_ptr<ModuleInfoCollectorImpl> impl_;

    /**
     * @brief Конструктор.
     */
    ModuleInfoCollector() noexcept;

    /**
     * @brief Деструктор.
     */
    ~ModuleInfoCollector() noexcept;
};

}  // namespace tasp

#endif  // TASP_MODULE_INFO_HPP_
