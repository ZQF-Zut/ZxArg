#pragma once
#include <vector>
#include <memory>
#include <string_view>


namespace ZQF::Zut::ZxArg::Plat
{
    struct CMDLineData
    {
    private:
        std::pair<std::size_t, std::unique_ptr<char[]>> m_RawStr;
        std::vector<std::string_view> m_vcCMD;

    public:
        CMDLineData() {};
        ~CMDLineData() {};
        auto operator =(CMDLineData&) -> CMDLineData & = default;
        auto operator =(CMDLineData&&) noexcept -> CMDLineData & = default;

    public:
        auto LoadRawStr() -> void;
        auto ParseCMD() -> void;
        auto ParseCMD(const int argc, const char** const argv) -> void;

    public:
        auto GetRawStrView() const -> std::string_view;
        auto GetVec() const -> const std::vector<std::string_view>&;
    };

} // namespace ZQF::Zut::ZxArg::Plat
