/**
 * @file
 * @brief Интерфейс работы с отчётом состояния программы.
 */
#ifndef TASP_HEALTH_HPP_
#define TASP_HEALTH_HPP_

#include <memory>
#include <string_view>

#include "jsoncpp/json/json.h"

namespace tasp
{

class HealthReportImpl;

/**
 * @brief Формат возвращаемого отчёта о состоянии программы.
 *
 * Класс скрывает от пользователя реализацию с помощью идиомы PIMPL
 * (Pointer to Implementation – указатель на реализацию).
 */
class [[gnu::visibility("default")]] HealthReport final
{
public:
    /**
     * @brief Статус проверки программы.
     */
    enum class Status : std::uint8_t
    {
        Ok = 0,      /*!< Ок */
        Warning = 1, /*!< Предупреждение */
        Error = 2,   /*!< Критическая ошибка */
        Unknown = 3  /*!< Статус неизвестен */
    };

    /**
     * @brief Перевод значения статуса проверки программы формата перечисления в
     * текст.
     *
     * @param status Статус проверки программы в формате перечисления.
     *
     * @return Текстовое представление статуса проверки программы.
     */
    static std::string StatusToString(const Status &status) noexcept
    {
        switch (status)
        {
            case Status::Ok:
                return "Ok";
            case Status::Warning:
                return "Warning";
            case Status::Error:
                return "Error";
            case Status::Unknown:
                return "Unknown";
        }
        return "None";
    }

    /**
     * @brief Перевод строки в значение статуса проверки программы в формате
     * перечисления.
     *
     * @param string Строка.
     *
     * @return Статус проверки программы в формате перечисления.
     */
    static Status StringToStatus(std::string_view string) noexcept
    {
        if (string == "Ok")
        {
            return Status{Status::Ok};
        }
        if (string == "Warning")
        {
            return Status{Status::Warning};
        }
        if (string == "Error")
        {
            return Status{Status::Error};
        }
        return Status{Status::Unknown};
    }

    /**
     * @brief Конструктор.
     *
     * @param name Название проверки
     * @param status Статус проверки программы
     * @param message Сообщение
     */
    HealthReport(std::string_view name,
                 const Status &status,
                 std::string_view message = {}) noexcept;

    /**
     * @brief Деструктор.
     */
    ~HealthReport() noexcept;

    /**
     * @brief Запрос статуса проверки.
     *
     * @return Статус проверки
     */
    [[nodiscard]] const Status &GetStatus() const noexcept;

    /**
     * @brief Преобразование отчёта в формат Json::Value.
     *
     * @return Отчёт в формате Json::Value
     */
    [[nodiscard]] Json::Value ToJSON() const noexcept;

    /**
     * @brief Копирующий конструктор.
     *
     * @param report Отчёт
     */
    HealthReport(const HealthReport &report);

    /**
     * @brief Конструктор перемещения.
     *
     * @param report Отчёт
     */
    HealthReport(HealthReport &&report) noexcept;

    /**
     * @brief Копирующее присваивание.
     *
     * @param report Отчёт
     *
     * @return Ссылка на изменённый объект отчёта
     */
    HealthReport &operator=(const HealthReport &report);

    /**
     * @brief Присваивание перемещения.
     *
     * @param report Отчёт
     *
     * @return Ссылка на изменённый объект отчёта
     */
    HealthReport &operator=(HealthReport &&report) noexcept;

private:
    /**
     * @brief Указатель на реализацию.
     */
    std::unique_ptr<HealthReportImpl> impl_;
};

}  // namespace tasp

#endif  // TASP_HEALTH_HPP_