/**
 * @file
 * @brief Интерфейсы для работы с запросом HTTP.
 */
#ifndef TASP_HTTP_REQUEST_HPP_
#define TASP_HTTP_REQUEST_HPP_

#include <jsoncpp/json/json.h>

#include <memory>

#include <tasp/http/data.hpp>
#include <tasp/http/header.hpp>
#include <tasp/http/uri.hpp>

namespace tasp::http
{

/**
 * @brief Интерфейс для работы с запросом HTTP.
 */
class Request
{
public:
    /**
     * @brief Конструктор.
     */
    Request() noexcept = default;

    /**
     * @brief Деструктор.
     */
    virtual ~Request() noexcept = default;

    /**
     * @brief Список методов запроса.
     */
    enum class Method
    {
        Get = 1 << 0,    /*!< Запрос ресурса */
        Post = 1 << 1,   /*!< Отправка данных ресурсу */
        Head = 1 << 2,   /*!< Запрос ресурса, без тела  */
        Put = 1 << 3,    /*!< Замена данных ресурса */
        Delete = 1 << 4, /*!< Удаление данных ресурса */
        Options = 1 << 5, /*!< Описание параметров соединения */
        Trace = 1 << 6, /*!< Тестовое сообщение к ресурсу */
        Connect = 1 << 7, /*!< Установка туннеля к ресурсу */
        Patch = 1 << 8 /*!< Частичное изменение ресурса */
    };

    /**
     * @brief Перевод значения метода формата перечисления в текст.
     *
     * @param method Метод запроса в формате перечисления
     *
     * @return Текстовое представление метода
     */
    static constexpr const char *MethodToString(Method method) noexcept
    {
        switch (method)
        {
            case Method::Get:
                return "GET";
            case Method::Post:
                return "POST";
            case Method::Head:
                return "HEAD";
            case Method::Put:
                return "PUT";
            case Method::Delete:
                return "DELETE";
            case Method::Options:
                return "OPTIONS";
            case Method::Trace:
                return "TRACE";
            case Method::Connect:
                return "CONNECT";
            case Method::Patch:
                return "PATCH";
        }
    }

    /**
     * @brief Запрос указателя на ресурс.
     *
     * @return Указатель на ресурс
     */
    [[nodiscard]] virtual std::shared_ptr<http::Uri> Uri() const noexcept = 0;

    /**
     * @brief Запрос заголовков запроса.
     *
     * @return Заголовок
     */
    [[nodiscard]] virtual std::shared_ptr<http::Header> Header()
        const noexcept = 0;

    /**
     * @brief Запрос метода запроса.
     *
     * @return Метод
     */
    [[nodiscard]] virtual Method GetMethod() const noexcept = 0;

    /**
     * @brief Запрос данных запроса в текстовом представлении.
     *
     * @return Данные
     */
    [[nodiscard]] virtual std::shared_ptr<http::Data> Data() const noexcept = 0;

    Request(const Request &) = delete;
    Request(Request &&) = delete;
    Request &operator=(const Request &) = delete;
    Request &operator=(Request &&) = delete;

private:
};

}  // namespace tasp::http

#endif  // TASP_HTTP_REQUEST_HPP_
