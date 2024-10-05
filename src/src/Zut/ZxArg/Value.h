#pragma once
#include <cstdlib>
#include <string_view>
#include <cmath>
#include <string>


namespace ZQF::Zut::ZxArg
{
    class Value
    {
    private:
        std::string_view m_msValue;
        std::string_view m_msHelp;

    public:
        Value() {}
        Value(const std::string_view msValue, const std::string_view msHelp) : m_msValue(msValue), m_msHelp(msHelp) {}

    public:
        auto GetValue() const -> std::string_view;
        auto GetHelp() const -> std::string_view;
        auto SetValue(const std::string_view msValue) -> void;
        auto SetHelp(const std::string_view msHelp) -> void;

    public:
        auto GetBool() const -> bool;
        auto GetStrView() const -> std::string_view;
        template <typename T = std::size_t> auto GetNum() const -> T;
        template <typename T = std::double_t> auto GetFloat() const -> T;
    };

    template <typename T>
    inline auto Value::GetNum() const -> T
    {
        char* end_ptr{};
        const auto c_str{ this->GetValue().data() };
        const auto radix{ this->GetValue().starts_with("0x") ? 16 : 10 };
        return static_cast<T>(std::is_unsigned_v<T> ? std::strtoull(c_str, &end_ptr, radix) : std::strtoll(c_str, &end_ptr, radix));
    }

    template <typename T>
    inline auto Value::GetFloat() const -> T
    {
        return static_cast<T>(std::stod(std::string{ this->GetValue() }, nullptr));
    }

} // namespace ZQF::Zut::ZxArg
