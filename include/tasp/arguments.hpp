/**
 * @file
 * @brief Интерфейсы для работы с аргументами командной строки.
 */
#ifndef TASP_ARGUMENTS_HPP_
#define TASP_ARGUMENTS_HPP_

#include <memory>
#include <string_view>

namespace tasp
{

class ProgramArgumentsImpl;

/**
 * @brief Интерфейс для работы с аргументами командной строки.
 *
 * Класс скрывает от пользователя реализацию с помощью идиомы PIMPL
 * (Pointer to Implementation – указатель на реализацию).
 */
class [[gnu::visibility("default")]] ProgramArguments final
{
public:
    /**
     * @brief Конструктор.
     *
     * @param argc Количество аргументов
     * @param argv Аргументы
     */
    explicit ProgramArguments(int argc, const char **argv) noexcept;

    /**
     * @brief Деструктор.
     */
    ~ProgramArguments() noexcept;

    /**
     * @brief Запрос значения аргумента.
     *
     * @param argument Аргумент
     *
     * @return Значение
     */
    [[nodiscard]] const std::string &Get(
        std::string_view argument) const noexcept;

    /**
     * @brief Проверка существования аргумента.
     *
     * @param argument Аргумент
     *
     * @return Результат проверки
     */
    [[nodiscard]] bool Exists(std::string_view argument) const noexcept;

    ProgramArguments(const ProgramArguments &) = delete;
    ProgramArguments(ProgramArguments &&) = delete;
    ProgramArguments &operator=(const ProgramArguments &) = delete;
    ProgramArguments &operator=(ProgramArguments &&) = delete;

private:
    /**
     * @brief Указатель на реализацию.
     */
    std::unique_ptr<ProgramArgumentsImpl> impl_;
};

}  // namespace tasp

#endif  // TASP_ARGUMENTS_HPP_
