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
    class Parser
    {
    private:
        std::string_view m_msProgramPath;
        std::vector<std::string_view> m_vcExample;
        std::unordered_map<std::string_view, Value> m_mpCmd;
        std::vector<std::string> m_vcArgData;

    public:
        Parser()
        {

        }

    private:
        bool ParseData()
        {
            if (m_vcArgData.size() < 1) { throw std::runtime_error("ZxArg::Parser::Parse(): arg count error!"); }

            m_msProgramPath = m_vcArgData[0];

            if (m_vcArgData.size() == 1)
            {
                this->ShowHelp();
                return false;
            }

            if ((m_vcArgData.size() % 2) == 0) { throw std::runtime_error("ZxArg::Parser::Parse(): error arg count!"); }

            for (size_t ite_arg = 1; ite_arg < m_vcArgData.size(); ite_arg += 2)
            {
                const std::string_view option = m_vcArgData[ite_arg + 0];
                const std::string_view value = m_vcArgData[ite_arg + 1];

                if ((value.size()) && (value.front() == '\"') && (value.back() == '\"'))
                {
                    m_mpCmd[option.data()].SetValue(value.substr(1, value.size() - 2));
                }
                else
                {
                    m_mpCmd[option.data()].SetValue(value);
                }
            }

            return true;
        }

    public:
        bool Parse()
        {
            m_vcArgData = ZxArg::GetCmdLine();
            return this->ParseData();
        }

        bool Parse(int argc, char** argv)
        {
            for (size_t idx = 0; idx < static_cast<size_t>(argc); idx++)
            {
                m_vcArgData.emplace_back(argv[idx]);
            }

            return this->ParseData();
        }

        void ShowHelp()
        {
            std::string help_log;

            help_log.append("Command:\n");
            for (auto& cmd : m_mpCmd)
            {
                help_log.append(1,'\t');
                help_log.append(cmd.first);
                help_log.append(1, '\t');
                help_log.append(cmd.second.GetHelp());
                help_log.append(1, '\n');
            }

            help_log.append("Example:\n");
            for (auto& exp : m_vcExample)
            {
                help_log.append(1, '\t');
                help_log.append(m_msProgramPath);
                help_log.append(1, ' ');
                help_log.append(exp);
                help_log.append(1, '\n');
            }

            std::printf(help_log.c_str());
        }

        void AddCmd(const std::string_view msOption, const std::string_view msHelp, const std::string_view msDefaultVal = "")
        {
            auto& val_ref = m_mpCmd.operator[](msOption.data());
            val_ref.SetHelp(msHelp);
            if (!msDefaultVal.empty())
            {
                val_ref.SetValue(msDefaultVal);
            }
        }

        void AddExample(const std::string_view msExample)
        {
            m_vcExample.emplace_back(msExample);
        }

        const Value& operator[](int) = delete;
        const Value& operator[](size_t) = delete;

        const Value& operator[](const std::string_view msOption)
        {
            if (auto ite_map = m_mpCmd.find(msOption.data()); ite_map != m_mpCmd.end())
            {
                if (ite_map->second.GetValue().empty()) { throw std::runtime_error(std::format("ZxArg::Parser::operator[](): cmd {} is empty!", msOption)); }
                return ite_map->second;
            }
            else
            {
                throw std::runtime_error(std::format("ZxArg::Parser::operator[](): cmd {} is not find!", msOption));
            }
        }
    };
}