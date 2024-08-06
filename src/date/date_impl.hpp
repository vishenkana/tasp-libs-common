/**
 * @file
 * @brief Реализация интерфейса для работы с датой.
 */
#ifndef TASP_DATE_IMPL_HPP_
#define TASP_DATE_IMPL_HPP_

#include <tasp/date.hpp>

namespace tasp
{

/**
 * @brief Реализация интерфейса для работы с датой.
 */
class DateImpl final
{
public:
    /**
     * @brief Конструктор.
     *
     * @param time_point Момент времени
     */
    explicit DateImpl(const Timepoint &time_point =
                          std::chrono::system_clock::now()) noexcept;

    /**
     * @brief Деструктор.
     */
    ~DateImpl() noexcept = default;

    /**
     * @brief Копирующий конструктор.
     *
     * @param date Дата
     */
    DateImpl(const DateImpl &date) noexcept;
    DateImpl(DateImpl &&) = delete;

    /**
     * @brief Копирующее присваивание.
     *
     * @param date Дата
     *
     * @return Ссылка на изменённый объект даты
     */
    DateImpl &operator=(const DateImpl &date) noexcept;
    DateImpl &operator=(DateImpl &&) = delete;

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
     * @brief Календарная временная метка.
     */
    std::tm timestamp_{};
};

}  // namespace tasp

#endif  // TASP_DATE_IMPL_HPP_