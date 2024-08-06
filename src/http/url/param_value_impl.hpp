/**
 * @file
 * @brief Реализация интерфейса работы со значением URL-параметра.
 */
#ifndef TASP_HTTP_URL_PARAM_VALUE_IMPL_HPP_
#define TASP_HTTP_URL_PARAM_VALUE_IMPL_HPP_

#include <string>
#include <string_view>
#include <vector>

#include <tasp/http/url/param_value.hpp>

namespace tasp::http::url
{

/**
 * @brief Реализация интерфейса для работы со значением URL-параметра.
 */
class ParamValueImpl final
{
public:
    /**
     * @brief Конструктор с параметром.
     *
     * @param whole_value Полное значение URL-параметра в виде 'имя:подзначения'
     * (или 'имя', если параметр простой, без подзначений).
     */
    explicit ParamValueImpl(std::string_view whole_value) noexcept;

    /**
     * @brief Деструктор.
     */
    ~ParamValueImpl() noexcept;

    /**
     * @brief Запрос полного значения URL-параметра в виде 'имя:подзначения'
     * (или 'имя', если параметр простой, без подзначений).
     *
     * @return Полное значение URL-параметра.
     */
    [[nodiscard]] const std::string &Whole() const noexcept;

    /**
     * @brief Запрос имени значения URL-параметра.
     *
     * @return Имя URL-параметра.
     */
    [[nodiscard]] const std::string &Name() const noexcept;

    /**
     * @brief Запрос подзначений URL-параметра.
     *
     * @return Подзначения URL-параметра (массив строк).
     */
    [[nodiscard]] const std::vector<std::string> &Subvalues() const noexcept;

    /**
     * @brief Запрос типа URL-параметра.
     *
     * @return Тип URL-параметра.
     */
    [[nodiscard]] const ParamValue::Type &GetType() const noexcept;

    /**
     * @brief Преобразование URL-значения в SQL-условие (для конкатенации с
     * 'WHERE').
     *
     * @return SQL-условие.
     */
    [[nodiscard]] std::string ToSQLCondition() const noexcept;

    ParamValueImpl(const ParamValueImpl &) = delete;
    ParamValueImpl(ParamValueImpl &&) = delete;
    ParamValueImpl &operator=(const ParamValueImpl &) = delete;
    ParamValueImpl &operator=(ParamValueImpl &&) = delete;

private:
    /**
     * @brief Полное значение URL-параметра в виде 'имя:подзначения' (или 'имя',
     * если параметр простой, без подзначений).
     */
    std::string whole_value_;

    /**
     * @brief Имя URL-параметра.
     */
    std::string name_;

    /**
     * @brief Подзначения URL-параметра.
     */
    std::vector<std::string> subvalues_;

    /**
     * @brief Тип значения URL-параметра.
     */
    ParamValue::Type subvalues_type_{ParamValue::Type::Single};
};

}  // namespace tasp::http::url

#endif  // TASP_HTTP_URL_PARAM_VALUE_IMPL_HPP_
