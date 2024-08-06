/**
 * @file
 * @brief Интерфейсы для работы с UUID (universally unique identifier
 * «универсальный уникальный идентификатор») версии 4.
 */
#ifndef TASP_UUID_HPP_
#define TASP_UUID_HPP_

#include <memory>
#include <string_view>

namespace tasp
{

class UUIDImpl;

/**
 * @brief Интерфейс для работы с UUID (universally unique identifier
 * «универсальный уникальный идентификатор») версии 4.
 *
 * Класс скрывает от пользователя реализацию с помощью идиомы PIMPL
 * (Pointer to Implementation – указатель на реализацию).
 */
class [[gnu::visibility("default")]] UUID final
{
public:
    /**
     * @brief Конструктор генерации UUID.
     *
     * Генерирует идентификатор.
     */
    UUID() noexcept;

    /**
     * @brief Конструктор c идентификатором переданным в параметре.
     *
     * @param uuid Идентификатор
     */
    explicit UUID(std::string_view uuid) noexcept;

    /**
     * @brief Деструктор.
     */
    ~UUID() noexcept;

    /**
     * @brief Запрос идентификатора в текстовом представлении.
     *
     * @return Идентификатор
     */
    [[nodiscard]] const std::string &Get() const noexcept;

    UUID(const UUID &) = delete;
    UUID(UUID &&) = delete;
    UUID &operator=(const UUID &) = delete;
    UUID &operator=(UUID &&) = delete;

private:
    /**
     * @brief Указатель на реализацию.
     */
    std::unique_ptr<UUIDImpl> impl_;
};

}  // namespace tasp

#endif  // TASP_UUID_HPP_
