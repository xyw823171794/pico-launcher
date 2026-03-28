#include "common.h"
#include <memory>
#include "NotoSansJP-Regular-10_nft2.h"
#include "NotoSansJP-Medium-7_5_nft2.h"
#include "NotoSansJP-Medium-10_nft2.h"
#include "NotoSansJP-Medium-11_nft2.h"
#include "fat/File.h"
#include "gui/font/nitroFont2.h"
#include "RuntimeFonts.h"

namespace
{
    constexpr const char* REGULAR_10_EXTERNAL_PATH = "/_pico/fonts/NotoSansSC-Regular-10.nft2";
    constexpr const char* MEDIUM_7_5_EXTERNAL_PATH = "/_pico/fonts/NotoSansSC-Medium-7_5.nft2";
    constexpr const char* MEDIUM_10_EXTERNAL_PATH = "/_pico/fonts/NotoSansSC-Medium-10.nft2";
    constexpr const char* MEDIUM_11_EXTERNAL_PATH = "/_pico/fonts/NotoSansSC-Medium-11.nft2";

    struct RuntimeFontData
    {
        const nft2_header_t* font = nullptr;
        std::unique_ptr<u8[]> ownedBuffer;
    };

    RuntimeFontData sFonts[4];

    bool LoadExternalFont(RuntimeFontData& runtimeFontData, const char* filePath)
    {
        File file;
        if (file.Open(filePath, FA_READ) != FR_OK)
            return false;

        auto fileSize = file.GetSize();
        if (fileSize == 0 || fileSize > 4 * 1024 * 1024)
        {
            LOG_WARN("Skipping invalid font file '%s' (size=%lu)\n", filePath, (u32)fileSize);
            return false;
        }

        auto fontBuffer = std::make_unique_for_overwrite<u8[]>(fileSize);
        if (!file.ReadExact(fontBuffer.get(), fileSize))
        {
            LOG_WARN("Failed to read font file '%s'\n", filePath);
            return false;
        }

        auto font = reinterpret_cast<nft2_header_t*>(fontBuffer.get());
        if (!nft2_unpack(font))
        {
            LOG_WARN("Invalid nft2 font file '%s'\n", filePath);
            return false;
        }

        runtimeFontData.font = font;
        runtimeFontData.ownedBuffer = std::move(fontBuffer);
        LOG_INFO("Loaded external font '%s'\n", filePath);
        return true;
    }
}

void RuntimeFonts::Init()
{
    auto regular10 = reinterpret_cast<nft2_header_t*>(NotoSansJP_Regular_10_nft2);
    auto medium7_5 = reinterpret_cast<nft2_header_t*>(NotoSansJP_Medium_7_5_nft2);
    auto medium10 = reinterpret_cast<nft2_header_t*>(NotoSansJP_Medium_10_nft2);
    auto medium11 = reinterpret_cast<nft2_header_t*>(NotoSansJP_Medium_11_nft2);

    nft2_unpack(regular10);
    nft2_unpack(medium7_5);
    nft2_unpack(medium10);
    nft2_unpack(medium11);

    sFonts[(int)FontType::Regular10].font = regular10;
    sFonts[(int)FontType::Medium7_5].font = medium7_5;
    sFonts[(int)FontType::Medium10].font = medium10;
    sFonts[(int)FontType::Medium11].font = medium11;

    LoadExternalFont(sFonts[(int)FontType::Regular10], REGULAR_10_EXTERNAL_PATH);
    LoadExternalFont(sFonts[(int)FontType::Medium7_5], MEDIUM_7_5_EXTERNAL_PATH);
    LoadExternalFont(sFonts[(int)FontType::Medium10], MEDIUM_10_EXTERNAL_PATH);
    LoadExternalFont(sFonts[(int)FontType::Medium11], MEDIUM_11_EXTERNAL_PATH);
}

const nft2_header_t* RuntimeFonts::GetFont(FontType fontType)
{
    return sFonts[(int)fontType].font;
}
