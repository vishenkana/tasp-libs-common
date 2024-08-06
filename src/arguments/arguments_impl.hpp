/**
 * @file
 * @brief Реализация интерфейсов для работы с аргументами командной строки.
 */
#ifndef TASP_ARGUMENTS_ARGUMENTS_IMPL_HPP_
#define TASP_ARGUMENTS_ARGUMENTS_IMPL_HPP_

#include <string>
#include <string_view>
#include <vector>

namespace tasp
{
/**
 * @brief Реализация интерфейса для работы с аргументами командной строки.
 */
class ProgramArgumentsImpl final
{
public:
    /**
     * @brief Конструктор.
     *
     * @param argc Количество аргументов
     * @param argv Аргументы
     */
    explicit ProgramArgumentsImpl(int argc, const char **argv) noexcept;

    /**
     * @brief Деструктор.
     */
    ~ProgramArgumentsImpl() noexcept;

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

    ProgramArgumentsImpl(const ProgramArgumentsImpl &) = delete;
    ProgramArgumentsImpl(ProgramArgumentsImpl &&) = delete;
    ProgramArgumentsImpl &operator=(const ProgramArgumentsImpl &) = delete;
    ProgramArgumentsImpl &operator=(ProgramArgumentsImpl &&) = delete;

private:
    /**
     * @brief Аргументы после разбора.
     */
    std::vector<std::string> arguments_;
};

}  // namespace tasp

#endif  // TASP_ARGUMENTS_ARGUMENTS_IMPL_HPP_
