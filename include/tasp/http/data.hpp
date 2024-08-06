/**
 * @file
 * @brief Интерфейсы для работы с данными HTTP.
 */
#ifndef TASP_HTTP_DATA_HPP_
#define TASP_HTTP_DATA_HPP_

#include <jsoncpp/json/json.h>

#include <memory>

namespace tasp::http
{

class DataImpl;

/**
 * @brief Интерфейс для работы с данными HTTP.
 */
class [[gnu::visibility("default")]] Data final
{
public:
    /**
     * @brief Конструктор.
     */
    Data() noexcept;

    /**
     * @brief Деструктор.
     */
    virtual ~Data() noexcept;

    /**
     * @brief Установка данных.
     *
     * @param data Данные
     */
    template<typename Type> void Set(const Type &data) noexcept;

    /**
     * @brief Запрос данных в запрашиваемом представлении.
     *
     * @return Данные
     */
    template<typename Type> [[nodiscard]] Type Get() const noexcept;

    /**
     * @brief Запись данных из внутреннего буфера в переданный.
     *
     * @param data Буфер для записи
     * @param length Разер данных для записи
     *
     * @return Количество прочитанных символов
     */
    [[nodiscard]] size_t Read(char *data, size_t length) noexcept;

    /**
     * @brief Чтение данных из переданного буфера во внутренний.
     *
     * @param data Буфер для чтения
     * @param length Разер данных для чтения
     */
    void Append(char *data, size_t length) noexcept;

    /**
     * @brief Количество данных в буфере.
     *
     * @return Размер
     */
    [[nodiscard]] size_t Length() const noexcept;

    /**
     * @brief Запрос типа данных.
     *
     * @return Тип
     */
    [[nodiscard]] const std::string &GetType() const noexcept;

    /**
     * @brief Установка типа данных.
     *
     * @param type Тип
     */
    void SetType(std::string_view type) noexcept;

    Data(const Data &) = delete;
    Data(Data &&) = delete;
    Data &operator=(const Data &) = delete;
    Data &operator=(Data &&) = delete;

private:
    /**
     * @brief Указатель на реализацию.
     */
    std::unique_ptr<DataImpl> impl_;
};

}  // namespace tasp::http

#endif  // TASP_HTTP_DATA_HPP_
