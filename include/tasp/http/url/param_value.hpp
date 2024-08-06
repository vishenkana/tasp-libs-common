/**
 * @file
 * @brief Интерфейсы для работы со значением URL-параметра.
 */
#ifndef TASP_HTTP_URL_PARAM_VALUE_HPP_
#define TASP_HTTP_URL_PARAM_VALUE_HPP_

#include <memory>
#include <string>
#include <string_view>
#include <vector>

namespace tasp::http::url
{

class ParamValue;
class ParamValueImpl;

/**
 * @brief Массив значений URL-параметра, переданных в URL.
 */
using ParamValueVector = std::vector<std::shared_ptr<ParamValue>>;

/**
 * @brief Интерфейс для работы со значением URL-параметра.
 */
class [[gnu::visibility("default")]] ParamValue final
{
public:
    /**
     * @brief Возможные типы значения URL-параметра.
     *
     * Тип значения URL-параметра определяется структурой его подзначений.
     *
     * Значение URL-параметра - конструкция вида 'имя:подзначения' (или 'имя',
     * если параметр простой, без подзначений). Подзначения объединены между
     * собой через разделитель, определяющий их структуру ("," - множество или
     * массив, "~" - диапазон).
     *
     * Формат значения:
     *     - Single - параметр=имя
     *     - Set - параметр=имя:подзначение_0,подзначение_1,подзначение_2,...
     *     - Array - параметр=имя:подзначение_0,подзначение_1,подзначение_2,...
     *     - Range - параметр=имя:подзначение_0~подзначение_1
     */
    enum class Type : std::uint8_t
    {
        Single = 0, /*!< Подзначения отсутсвуют */
        Set = 1, /*!< Подзначения объединены в множество (порядок не важен) */
        Array = 2, /*!< Подзначения объединены в массив (порядок важен)*/
        Range = 3 /*!< Подзначения представляют собой диапазон 'от~до' */
    };

    /**
     * @brief Конструктор.
     *
     * @param whole_value Полное значение URL-параметра в виде 'имя:подзначения'
     * (или 'имя', если параметр простой, без подзначений).
     */
    explicit ParamValue(std::string_view whole_value) noexcept;

    /**
     * @brief Деструктор.
     */
    ~ParamValue() noexcept;

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

    ParamValue(const ParamValue &) = delete;
    ParamValue(ParamValue &&) = delete;
    ParamValue &operator=(const ParamValue &) = delete;
    ParamValue &operator=(ParamValue &&) = delete;

private:
    /**
     * @brief Указатель на реализацию.
     */
    std::unique_ptr<ParamValueImpl> impl_;
};

}  // namespace tasp::http::url

#endif  // TASP_HTTP_URL_PARAM_VALUE_HPP_
