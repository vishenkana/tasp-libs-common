/**
 * @file
 * @brief Интерфейсы для работы с ответом HTTP.
 */
#ifndef TASP_HTTP_RESPONSE_HPP_
#define TASP_HTTP_RESPONSE_HPP_

#include <jsoncpp/json/json.h>

#include <memory>

#include <tasp/http/data.hpp>
#include <tasp/http/header.hpp>

namespace tasp::http
{

/**
 * @brief Интерфейс для работы с ответом HTTP.
 */
class Response
{
public:
    /**
     * @brief Конструктор.
     */
    Response() noexcept = default;

    /**
     * @brief Деструктор.
     */
    virtual ~Response() noexcept = default;

    /**
     * @brief Коды ответа.
     */
    enum class Code
    {
        Ok = 200,        /*!< Успешно */
        Created = 201,   /*!< Создано */
        NoContent = 204, /*!< Нет содержимого */
        MovePerm = 301, /*!< Перемещён на постоянной основе */
        MoveTemp = 302,       /*!< Найдено */
        NotModified = 304,    /*!< Не модифицировано */
        BadRequest = 400,     /*!< Плохой запрос */
        NotFound = 404,       /*!< Не найден */
        BadMethod = 405,      /*!< Метод не разрешён */
        EntityTooLarge = 413, /*!< Превышен лимит, */
        ExpectationFailed = 417, /*!< Ожидание не может быть выполнено */
        UnprocessableEntity =
            422, /*!< Семантически некорректное содержимое запроса */
        Internal = 500, /*!< Внутренняя ошибка сервера */
        NotImplemented = 501, /*!< Не выполнено */
        ServUnavail = 503     /*!< Сервис недоступен */
    };

    /**
     * @brief Установка кода ответа.
     *
     * @param code Код
     */
    virtual void SetCode(Code code) noexcept = 0;

    /**
     * @brief Запрос кода ответа.
     *
     * @return Код
     */
    [[nodiscard]] virtual Code GetCode() const noexcept = 0;

    /**
     * @brief Запрос заголовков ответа.
     *
     * @return Заголовок
     */
    [[nodiscard]] virtual std::shared_ptr<http::Header> Header()
        const noexcept = 0;

    /**
     * @brief Запрос данных запроса в текстовом представлении.
     *
     * @return Данные
     */
    [[nodiscard]] virtual std::shared_ptr<http::Data> Data() const noexcept = 0;

    /**
     * @brief Добавление в ответ сообщения об ошибке.
     *
     * @param code Код ответа
     * @param message Сообщение
     */
    void virtual SetError(Code code, std::string_view message) noexcept = 0;

    Response(const Response &) = delete;
    Response(Response &&) = delete;
    Response &operator=(const Response &) = delete;
    Response &operator=(Response &&) = delete;

private:
};

}  // namespace tasp::http

#endif  // TASP_HTTP_RESPONSE_HPP_
