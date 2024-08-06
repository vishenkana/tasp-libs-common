/**
 * @file
 * @brief Интерфейсы для работы с указателями на ресурс (URI).
 */
#ifndef TASP_HTTP_URI_HPP_
#define TASP_HTTP_URI_HPP_

#include <memory>
#include <string>
#include <string_view>
#include <vector>

#include <tasp/http/url/param_value.hpp>

namespace tasp::http
{

/**
 * @brief Интерфейс для работы с указателями на ресурс (URI).
 */
class Uri
{
public:
    /**
     * @brief Конструктор.
     */
    Uri() noexcept = default;

    /**
     * @brief Деструктор.
     */
    virtual ~Uri() noexcept = default;

    /**
     * @brief Запрос полного идентификатора ресурса.
     *
     * @return Путь
     */
    [[nodiscard]] virtual const std::string &Url() const noexcept = 0;

    /**
     * @brief Запрос URL-пути.
     *
     * @return Путь
     */
    [[nodiscard]] virtual const std::string &Path() const noexcept = 0;

    /**
     * @brief Смена пути запроса.
     *
     * @param path Путь запроса
     */
    virtual void ChangePath(std::string_view path) noexcept = 0;

    /**
     * @brief Запрос значений параметра.
     *
     * @param name Название параметра
     *
     * @return Массив значений, переданных в URL
     */
    [[nodiscard]] virtual url::ParamValueVector ParamValues(
        std::string_view name) const noexcept = 0;

    /**
     * @brief Проверка соответствия URL-пути регулярному выражению и
     * формирование подгрупп пути.
     *
     * @param expr Регулярное выражение
     *
     * @return Результат сравнения
     */
    [[nodiscard]] virtual bool Match(std::string_view expr) noexcept = 0;

    /**
     * @brief Запрос значения подгруппы пути. Если подгруппа отсутствует,
     * возвращается пустое значение.
     *
     * @param number Номер подгруппы
     *
     * @return Значение подгруппы
     */
    [[nodiscard]] virtual const std::string &SubMatch(
        size_t number) const noexcept = 0;

    /**
     * @brief Преобразование параметров запроса в SQL-условие (для конкатенации
     * с 'WHERE').
     *
     * @return SQL-условие.
     */
    [[nodiscard]] virtual std::string ToSQLCondition() const noexcept = 0;

    Uri(const Uri &) = delete;
    Uri(Uri &&) = delete;
    Uri &operator=(const Uri &) = delete;
    Uri &operator=(Uri &&) = delete;

private:
};

}  // namespace tasp::http

#endif  // TASP_HTTP_URI_HPP_
