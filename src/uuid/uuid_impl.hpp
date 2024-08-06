/**
 * @file
 * @brief Реализация интерфейсов для работы с UUID (universally unique
 * identifier «универсальный уникальный идентификатор») версии 4.
 */
#ifndef TASP_UUID_UUID_IMPL_HPP_
#define TASP_UUID_UUID_IMPL_HPP_

#include <bitset>
#include <string>
#include <string_view>

namespace tasp
{

/**
 * @brief Реализация интерфейса для работы с UUID (universally unique identifier
 * «универсальный уникальный идентификатор») версии 4.
 */
class UUIDImpl final
{
public:
    /**
     * @brief Конструктор генерации UUID.
     *
     * Генерирует идентификатор.
     */
    UUIDImpl() noexcept;

    /**
     * @brief Конструктор c идентификатором переданным в параметре.
     *
     * @param uuid Идентификатор
     */
    explicit UUIDImpl(std::string_view uuid) noexcept;

    /**
     * @brief Деструктор.
     */
    ~UUIDImpl() noexcept;

    /**
     * @brief Запрос идентификатора в текстовом представлении.
     *
     * @return Идентификатор
     */
    [[nodiscard]] const std::string &Get() const noexcept;

    UUIDImpl(const UUIDImpl &) = delete;
    UUIDImpl(UUIDImpl &&) = delete;
    UUIDImpl &operator=(const UUIDImpl &) = delete;
    UUIDImpl &operator=(UUIDImpl &&) = delete;

private:
    /**
     * @brief Генерация идентификатора.
     *
     * @return Идентификатор
     */
    static std::string Generate() noexcept;

    /**
     * @brief Идентификатор в текстовом представлении.
     */
    std::string value_;
};

}  // namespace tasp

#endif  // TASP_UUID_UUID_IMPL_HPP_
