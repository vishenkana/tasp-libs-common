/**
 * @file
 * @brief Реализация интерфейсов для работы с данными HTTP.
 */
#ifndef TASP_HTTP_DATA_IMPL_HPP_
#define TASP_HTTP_DATA_IMPL_HPP_

#include <jsoncpp/json/json.h>

#include <sstream>
#include <string>

#include <tasp/logging.hpp>

namespace tasp::http
{

/**
 * @brief Реализация интерфейса для работы с данными HTTP.
 */
class DataImpl final
{
public:
    /**
     * @brief Конструктор.
     */
    DataImpl() noexcept;

    /**
     * @brief Деструктор.
     */
    virtual ~DataImpl() noexcept;

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
    template<typename Type> Type Get() noexcept;

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

    DataImpl(const DataImpl &) = delete;
    DataImpl(DataImpl &&) = delete;
    DataImpl &operator=(const DataImpl &) = delete;
    DataImpl &operator=(DataImpl &&) = delete;

private:
    /**
     * @brief Данные.
     */
    std::stringstream data_;

    /**
     * @brief Тип данных.
     */
    std::string type_{"text/plain"};
};

/*------------------------------------------------------------------------------
    DataImpl
------------------------------------------------------------------------------*/
template<typename Type> void DataImpl::Set(const Type &data) noexcept
{
    if (std::is_same<Type, Json::Value>::value)
    {
        SetType("application/json");
    }

    data_.clear();
    data_ << data;
}

}  // namespace tasp::http

#endif  // TASP_HTTP_DATA_IMPL_HPP_
