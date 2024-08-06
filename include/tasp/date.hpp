/**
 * @file
 * @brief Интерфейс для работы с датой.
 */
#ifndef TASP_DATE_HPP_
#define TASP_DATE_HPP_

#include <jsoncpp/json/json.h>

#include <chrono>
#include <memory>

/**
 * @brief Момент времени (сокращение std::chrono::system_clock::time_point)
 */
using Timepoint = std::chrono::system_clock::time_point;

namespace tasp
{

class DateImpl;

/**
 * @brief Интерфейс для работы с датой.
 *
 * Класс скрывает от пользователя реализацию с помощью идиомы PIMPL
 * (Pointer to Implementation – указатель на реализацию).
 */
class [[gnu::visibility("default")]] Date
{
public:
    /**
     * @brief Конструктор.
     *
     * @param time_point Момент времени
     */
    explicit Date(const Timepoint &time_point =
                      std::chrono::system_clock::now()) noexcept;

    /**
     * @brief Деструктор.
     */
    ~Date() noexcept;

    /**
     * @brief Копирующий конструктор.
     *
     * @param date Дата
     */
    Date(const Date &date);

    /**
     * @brief Конструктор перемещения.
     *
     * @param date Дата
     */
    Date(Date &&date) noexcept;

    /**
     * @brief Копирующее присваивание.
     *
     * @param date Дата
     *
     * @return Ссылка на изменённый объект даты
     */
    Date &operator=(const Date &date);

    /**
     * @brief Присваивание перемещения.
     *
     * @param date Дата
     *
     * @return Ссылка на изменённый объект даты
     */
    Date &operator=(Date &&date) noexcept;

    /**
     * @brief Обновление значений даты на новый момент времени.
     *
     * @param time_point Момент времени
     */
    void Update(const Timepoint &time_point =
                    std::chrono::system_clock::now()) noexcept;

    /**
     * @brief Преобразование даты в строковый формат.
     *
     * @return Дата в строковом формате
     */
    [[nodiscard]] std::string ToString() const noexcept;

    /**
     * @brief Преобразование даты в формат Json::Value.
     *
     * @return Дата в формате Json::Value
     */
    [[nodiscard]] Json::Value ToJSON() const noexcept;

private:
    /**
     * @brief Указатель на реализацию.
     */
    std::unique_ptr<DateImpl> impl_;
};

}  // namespace tasp

#endif  // TASP_DATE_HPP_