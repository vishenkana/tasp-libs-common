/**
 * @file
 * @brief Реализация интерфейса работы с отчётом состояния программы.
 */
#ifndef TASP_HEALTH_IMPL_HPP_
#define TASP_HEALTH_IMPL_HPP_

#include <tasp/date.hpp>
#include <tasp/health.hpp>

namespace tasp
{

/**
 * @brief Реализация формата возвращаемого отчёта о состоянии программы.
 */
class HealthReportImpl final
{
public:
    /**
     * @brief Конструктор.
     *
     * @param name Название проверки
     * @param status Статус проверки программы
     * @param message Сообщение
     */
    HealthReportImpl(std::string_view name,
                     const HealthReport::Status &status,
                     std::string_view message = {}) noexcept;

    /**
     * @brief Деструктор.
     */
    ~HealthReportImpl() noexcept;

    /**
     * @brief Запрос статуса проверки.
     *
     * @return Статус проверки
     */
    [[nodiscard]] const HealthReport::Status &GetStatus() const noexcept;

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
    HealthReportImpl(const HealthReportImpl &report) noexcept;
    HealthReportImpl(HealthReportImpl &&) = delete;

    /**
     * @brief Копирующее присваивание.
     *
     * @param report Отчёт
     *
     * @return Ссылка на изменённый объект отчёта
     */
    HealthReportImpl &operator=(const HealthReportImpl &report) noexcept;
    HealthReportImpl &operator=(HealthReportImpl &&) = delete;

private:
    /**
     * @brief Название проверки.
     */
    std::string name_;

    /**
     * @brief Статус проверки программы.
     */
    HealthReport::Status status_;

    /**
     * @brief Дата проверки.
     */
    Date date_;

    /**
     * @brief Сообщение (описание) проверки.
     */
    std::string message_;
};

}  // namespace tasp

#endif  // TASP_HEALTH_IMPL_HPP_
