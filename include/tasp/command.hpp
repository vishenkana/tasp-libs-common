/**
 * @file
 * @brief Интерфейс для запуска команд в отдельном процессе.
 */
#ifndef TASP_COMMAND_HPP_
#define TASP_COMMAND_HPP_

#include <memory>
#include <string_view>

namespace tasp
{

class CommandImpl;
class CommandResult;

/**
 * @brief Интерфейс для запуска команд в отдельном процессе.
 *
 * Класс скрывает от пользователя реализацию с помощью идиомы PIMPL
 * (Pointer to Implementation – указатель на реализацию).
 */
class [[gnu::visibility("default")]] Command final
{
public:
    /**
     * @brief Конструктор.
     *
     * @param command Полный путь к команде
     * @param args Аргументы команды
     */
    explicit Command(std::string_view command,
                     std::string_view args = {}) noexcept;

    /**
     * @brief Деструктор.
     */
    ~Command() noexcept;

    /**
     * @brief Смена аргументов команды.
     *
     * @param args Новые аргументы команды
     */
    void ChangeArgs(std::string_view args) noexcept;

    /**
     * @brief Запуск команды на исполнение.
     *
     * Дополните аргументы добавляются в конец к основным
     *
     * @param extra_args Дополнительные аргументы запуска
     *
     * @return Результат выполнения команды
     */
    std::unique_ptr<CommandResult> Exec(
        std::string_view extra_args = {}) noexcept;

    Command(const Command &) = delete;
    Command(Command &&) = delete;
    Command &operator=(const Command &) = delete;
    Command &operator=(Command &&) = delete;

private:
    /**
     * @brief Указатель на реализацию.
     */
    std::unique_ptr<CommandImpl> impl_;
};

/**
 * @brief Интерфейс с результатом выполнения команды.
 */
class [[gnu::visibility("default")]] CommandResult
{
public:
    /**
     * @brief Конструктор.
     */
    CommandResult() noexcept = default;

    /**
     * @brief Деструктор.
     */
    virtual ~CommandResult() noexcept = default;

    /**
     * @brief Запрос кода возврата.
     *
     * @return Код
     */
    [[nodiscard]] virtual int Code() const noexcept = 0;

    /**
     * @brief Запрос данных выведенных в stdout.
     *
     * @return Данные
     */
    [[nodiscard]] virtual const std::string &StdOut() const noexcept = 0;

    /**
     * @brief Запрос данных выведенных в stderr.
     *
     * @return Данные
     */
    [[nodiscard]] virtual const std::string &StdErr() const noexcept = 0;

    CommandResult(const CommandResult &) = delete;
    CommandResult(CommandResult &&) = delete;
    CommandResult &operator=(const CommandResult &) = delete;
    CommandResult &operator=(CommandResult &&) = delete;

private:
};

}  // namespace tasp

#endif  // TASP_COMMAND_HPP_
