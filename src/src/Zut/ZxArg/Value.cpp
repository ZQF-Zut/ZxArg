#include "Value.h"
#include <stdexcept>


namespace ZQF::Zut::ZxArg
{
    auto Value::GetValue() const -> std::string_view
    {
        return m_msValue;
    }

    auto Value::GetHelp() const -> std::string_view
    {
        return m_msHelp;
    }

    auto Value::SetValue(const std::string_view msValue) -> void
    {
        m_msValue = msValue;
    }

    auto Value::SetHelp(const std::string_view msHelp) -> void
    {
        m_msHelp = msHelp;
    }

    auto Value::GetBool() const -> bool
    {
        std::string_view value{ this->GetValue() };
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

    auto Value::GetStrView() const -> std::string_view
    {
        return this->GetValue();
    }

} // namespace ZQF::Zut::ZxArg
