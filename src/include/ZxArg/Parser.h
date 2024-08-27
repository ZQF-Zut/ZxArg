#pragma once
#include <format>
#include <vector>
#include <string>
#include <string_view>
#include <unordered_map>
#include <stdexcept>

#include <ZxArg/Value.h>
#include <ZxArg/Platform.h>


namespace ZQF::ZxArg
{
    // https://www.cppstories.com/2021/heterogeneous-access-cpp20/
    struct string_hash
    {
        using is_transparent = void;
        [[nodiscard]] size_t operator()(const char* cpKey) const
        {
            return std::hash<std::string_view>{}(cpKey);
        }

        [[nodiscard]] size_t operator()(const std::string_view msKey) const
        {
            return std::hash<std::string_view>{}(msKey);
        }

        [[nodiscard]] size_t operator()(const std::string& msKey) const
        {
            return std::hash<std::string>{}(msKey);
        }
    };

    class Parser
    {
    private:
        std::vector<std::string> m_vcArgData;
        std::vector<std::string_view> m_vcExample;
        std::unordered_map<std::string_view, ZxArg::Value, string_hash, std::equal_to<>> m_mpCmd;

    public:
        Parser()
        {

        }

    private:
        bool ParseData(bool isShowHelp = true)
        {
            if (m_vcArgData.size() < 1) { throw std::runtime_error("ZxArg::Parser::Parse(): arg empty!"); }

            if (m_vcArgData.size() == 1)
            {
                if (isShowHelp) { this->ShowHelp(); }
                return false;
            }

            if ((m_vcArgData.size() % 2) == 0) { throw std::runtime_error("ZxArg::Parser::Parse(): arg count error!"); }

            for (size_t ite_arg = 1; ite_arg < m_vcArgData.size(); ite_arg += 2)
            {
                const std::string_view option = m_vcArgData[ite_arg + 0];
                const std::string_view value = m_vcArgData[ite_arg + 1];

                if ((value.size()) && (value.front() == '\"') && (value.back() == '\"'))
                {
                    m_mpCmd[option].SetValue(value.substr(1, value.size() - 2));
                }
                else
                {
                    m_mpCmd[option].SetValue(value);
                }
            }

            return true;
        }

    public:
        bool Parse(bool isShowHelp = true)
        {
            m_vcArgData = ZxArg::GetCmdLine();
            return this->ParseData(isShowHelp);
        }

        bool Parse(int argc, char** argv, bool isShowHelp = true)
        {
            for (size_t idx = 0; idx < static_cast<size_t>(argc); idx++) { m_vcArgData.emplace_back(argv[idx]); }
            return this->ParseData(isShowHelp);
        }

        void ShowHelp()
        {
            std::string help_log;

            help_log.append("Command:\n");
            for (auto& cmd : m_mpCmd)
            {
                help_log.append(1,'\t');
                help_log.append(cmd.first);
                help_log.append("\t -> ");
                help_log.append(cmd.second.GetHelp());
                help_log.append(1, '\n');
            }

            // get exe pure name
            std::string_view exe_name{ m_vcArgData.front() };
            if (const auto pos{ exe_name.rfind('/') }; pos != std::string_view::npos)
            {
                exe_name = exe_name.substr(pos + 1);
                if (exe_name.ends_with('"'));
                {
                    exe_name = exe_name.substr(0, exe_name.size() - 1);
                }
            }

            help_log.append("Example:\n");
            for (auto& exp : m_vcExample)
            {
                help_log.append(1, '\t');
                help_log.append(exe_name);
                help_log.append(1, ' ');
                help_log.append(exp);
                help_log.append(1, '\n');
            }

            std::printf(help_log.c_str());
        }

        void AddCmd(const std::string_view msOption, const std::string_view msHelp, const std::string_view msDefaultVal = "")
        {
            auto& val_ref = m_mpCmd[msOption];
            val_ref.SetHelp(msHelp);
            if (!msDefaultVal.empty()) { val_ref.SetValue(msDefaultVal); }
        }

        void AddExample(const std::string_view msExample)
        {
            m_vcExample.emplace_back(msExample);
        }

        const Value& operator[](int) = delete;
        const Value& operator[](size_t) = delete;

        const Value& operator[](const std::string_view msOption)
        {
            if (auto ite_map = m_mpCmd.find(msOption); ite_map != m_mpCmd.end())
            {
                if (ite_map->second.GetValue().empty()) { throw std::runtime_error(std::format("ZxArg::Parser::operator[](): arg {} is empty!", msOption)); }
                return ite_map->second;
            }

            throw std::runtime_error(std::format("ZxArg::Parser::operator[](): arg {} is not find!", msOption));
        }
    };
}
