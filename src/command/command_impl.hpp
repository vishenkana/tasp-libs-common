/**
 * @file
 * @brief Реализация интерфейсов для запуска команд в отдельном процессе.
 */
#ifndef TASP_COMMAND_COMMAND_IMPL_HPP_
#define TASP_COMMAND_COMMAND_IMPL_HPP_

#include <string>
#include <string_view>
#include <vector>

#include "tasp/command.hpp"

namespace tasp
{

/**
 * @brief Реализация интерфейса для запуска команд в отдельном процессе.
 */
class CommandImpl final
{
public:
    /**
     * @brief Конструктор.
     *
     * @param command Полный путь к команде
     * @param args Аргументы команды
     */
    explicit CommandImpl(std::string_view command,
                         std::string_view args = {}) noexcept;

    /**
     * @brief Деструктор.
     */
    ~CommandImpl() noexcept;

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

    CommandImpl(const CommandImpl &) = delete;
    CommandImpl(CommandImpl &&) = delete;
    CommandImpl &operator=(const CommandImpl &) = delete;
    CommandImpl &operator=(CommandImpl &&) = delete;

private:
    /**
     * @brief Разбиение аргументов запуска из строки в отдельные элементы.
     *
     * Разбивает параметры разделенные пробелом.
     *
     * @param args Аргументы запуска
     */
    void ParseArgs(std::string_view args = {}) noexcept;

    /**
     * @brief Полный путь к команде.
     */
    std::string command_;

    /**
     * @brief Аргументы запуска.
     */
    std::vector<std::string> args_;

    /**
     * @brief Количество основных аргументов запуска.
     */
    int args_count_{0};
};

/**
 * @brief Реализация интерфейса с результатом выполнения команды.
 */
class CommandResultImpl : public CommandResult
{
public:
    /**
     * @brief Конструктор.
     *
     * @param code Код возврата
     * @param stdout Данные выведенные в stdout
     * @param stderr Данные выведенные в stderr
     */
    explicit CommandResultImpl(int code = -1,
                               std::string_view stdout = {},
                               std::string_view stderr = {}) noexcept;

    /**
     * @brief Деструктор.
     */
    ~CommandResultImpl() noexcept override;

    /**
     * @brief Запрос кода возврата.
     *
     * @return Код
     */
    [[nodiscard]] int Code() const noexcept override;

    /**
     * @brief Запрос данных выведенных в stdout.
     *
     * @return Данные
     */
    [[nodiscard]] const std::string &StdOut() const noexcept override;

    /**
     * @brief Запрос данных выведенных в stderr.
     *
     * @return Данные
     */
    [[nodiscard]] const std::string &StdErr() const noexcept override;

    CommandResultImpl(const CommandResultImpl &) = delete;
    CommandResultImpl(CommandResultImpl &&) = delete;
    CommandResultImpl &operator=(const CommandResultImpl &) = delete;
    CommandResultImpl &operator=(CommandResultImpl &&) = delete;

private:
    /**
     * @brief Код возврата.
     */
    int code_;

    /**
     * @brief Данные выведенные в stdout.
     */
    std::string stdout_;

    /**
     * @brief Данные выведенные в stderr.
     */
    std::string stderr_;
};

/**
 * @brief Класс для работы с однонаправленным каналом межпроцессного
 * взаимодействия.
 */
class Pipe final
{
public:
    /**
     * @brief Конструктор.
     *
     * @param type Тип канала
     */
    explicit Pipe(int type) noexcept;

    /**
     * @brief Деструктор.
     */
    ~Pipe() noexcept = default;

    /**
     * @brief Настройка канала на стороне потомка.
     */
    void Child() noexcept;

    /**
     * @brief Запрос данных выведенные в канал.
     *
     * @return Данные
     */
    std::string Data() noexcept;

    /**
     * @brief Оператор вывода сообщения в канал.
     *
     * @param text Текст сообщения
     *
     * @return Самого себя
     */
    Pipe &operator<<(std::string_view text);

    Pipe(const Pipe &) = delete;
    Pipe(Pipe &&) = delete;
    Pipe &operator=(const Pipe &) = delete;
    Pipe &operator=(Pipe &&) = delete;

private:
    /**
     * @brief Дескрипторы канала.
     */
    std::array<int, 2> pipes_{};

    /**
     * @brief Тип канала.
     */
    int type_;
};

}  // namespace tasp

#endif  // TASP_COMMAND_COMMAND_IMPL_HPP_
