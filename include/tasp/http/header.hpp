/**
 * @file
 * @brief Интерфейсы для работы с заголовком HTTP.
 */
#ifndef TASP_HTTP_HEADER_HPP_
#define TASP_HTTP_HEADER_HPP_

#include <string_view>

namespace tasp::http
{

/**
 * @brief Интерфейс для работы с заголовком HTTP.
 */
class Header
{
public:
    /**
     * @brief Конструктор.
     */
    Header() noexcept = default;

    /**
     * @brief Деструктор.
     */
    virtual ~Header() noexcept = default;

    /**
     * @brief Тип заголовка.
     */
    enum class Type : std::uint8_t
    {
        Input = 0,  /*!< Входящий заголовок */
        Output = 1, /*!< Исходящий заголовок */
    };

    /**
     * @brief Запрос параметра заголовка.
     *
     * @param name Название параметра
     *
     * @return Значение
     */
    [[nodiscard]] virtual const std::string &Get(
        std::string_view name) const noexcept = 0;

    /**
     * @brief Установка нового значения параметра заголовка.
     *
     * @param name Название параметра
     * @param value Новое значение
     */
    virtual void Set(std::string_view name,
                     std::string_view value) noexcept = 0;

    Header(const Header &) = delete;
    Header(Header &&) = delete;
    Header &operator=(const Header &) = delete;
    Header &operator=(Header &&) = delete;

private:
};

}  // namespace tasp::http

#endif  // TASP_HTTP_HEADER_HPP_
