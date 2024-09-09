#pragma once
#include <cstdlib>
#include <string>
#include <stdexcept>


namespace ZQF::Zut::ZxArg
{
    class Value
    {
    private:
        std::string_view m_msValue;
        std::string_view m_msHelp;

    public:
        Value()
        {

        }

        Value(const std::string_view msValue, const std::string_view msHelp)
            : m_msValue(msValue), m_msHelp(msHelp)
        {

        }

    public:
        auto GetValue() const -> const std::string_view
        {
            return m_msValue;
        }

        auto GetHelp() const -> const std::string_view
        {
            return m_msHelp;
        }

        auto SetValue(std::string_view msValue) -> void
        {
            m_msValue = msValue;
        }

        auto SetHelp(std::string_view msHelp) -> void
        {
            m_msHelp = msHelp;
        }

    public:
        template<class T>
        auto Get() const -> const T
        {
            if constexpr (std::is_same_v<T, bool>)
            {
                std::string_view value = this->GetValue();
                if (value == "true" || value == "yes")
                {
                    return true;
                }
                else if (value == "false" || value == "no")
                {
                    return false;
                }

                throw std::runtime_error("ZxArg::Value::Get<bool>(): not bool type");
            }
            else if constexpr (std::is_integral_v<T>)
            {
                char* end_ptr{};
                const auto c_str = this->GetValue().data();
                const auto radix = this->GetValue().starts_with("0x") ? 16 : 10;
                return static_cast<T>(std::is_unsigned_v<T> ? std::strtoull(c_str, &end_ptr, radix) : std::strtoll(c_str, &end_ptr, radix));
            }
            else if constexpr (std::is_floating_point_v<T>)
            {
                return static_cast<T>(std::stod(this->GetValue().data(), nullptr));
            }
            else if constexpr (std::is_same_v<T, std::string_view>)
            {
                return this->GetValue();
            }
            else if constexpr (std::is_same_v<T, std::string>)
            {
                return std::string{ this->GetValue() };
            }
            else
            {
                static_assert(false, "ZxArg::Value::Get<>(): error type!");
            }
        }
    };

} // namespace ZQF::Zut::ZxArg
