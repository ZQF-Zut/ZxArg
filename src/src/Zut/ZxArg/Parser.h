#pragma once
#include <format>
#include <vector>
#include <string>
#include <string_view>
#include <unordered_map>
#include <stdexcept>
#include <Zut/ZxArg/Value.h>
#include <Zut/ZxArg/Plat.h>


namespace ZQF::Zut::ZxArg
{
    // https://www.cppstories.com/2021/heterogeneous-access-cpp20/
    struct string_hash
    {
        using is_transparent = void;
        [[nodiscard]] std::size_t operator()(const char* cpKey) const
        {
            return std::hash<std::string_view>{}(cpKey);
        }

        [[nodiscard]] std::size_t operator()(const std::string_view msKey) const
        {
            return std::hash<std::string_view>{}(msKey);
        }

        [[nodiscard]] std::size_t operator()(const std::string& msKey) const
        {
            return std::hash<std::string>{}(msKey);
        }
    };

    class Parser
    {
    private:
        Plat::CMDLineData m_CMDLineData;

    private:
        std::string m_msAbout;
        std::string m_msAuthor;
        std::vector<std::string> m_vcExample;
        std::unordered_map<std::string_view, ZxArg::Value, string_hash, std::equal_to<>> m_mpOption;

    public:
        Parser() {}
        ~Parser() {}
        Parser(const Parser&) = delete;
        Parser(Parser&&) noexcept = delete;
        auto operator=(const Parser&) = delete;
        auto operator=(Parser&&) noexcept = delete;
        auto operator[](int) -> const Value & = delete;
        auto operator[](std::size_t) -> const Value & = delete;
        auto operator[](const std::string_view msOption) const -> const Value&;

    public:
        auto Parse(const bool isShowHelp = true) -> bool;
        auto Parse(const int argc, const char** const argv, const bool isShowHelp = true) -> bool;
        auto ShowHelp() -> void;

    public:
        auto AddOption(const std::string_view msOption, const std::string_view msHelp, const std::string_view msDefaultVal = "") -> Parser&;
        auto AddExample(const std::string_view msExample) -> Parser&;
        auto SetAbout(const std::string_view msAbout) -> Parser&;
        auto SetAuthor(const std::string_view msAuthor) -> Parser&;

    private:
        auto ParseOptions(const bool isShowHelp = true) -> bool;
    };
} // namespace ZQF::Zut::ZxArg
