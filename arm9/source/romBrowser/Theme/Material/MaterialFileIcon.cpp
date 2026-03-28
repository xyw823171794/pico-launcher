#include "common.h"
#include "gui/GraphicsContext.h"
#include "gui/PaletteManager.h"
#include "gui/font/nitroFont2.h"
#include "core/math/RgbMixer.h"
#include "gui/OamBuilder.h"
#include "largeFolderIcon.h"
#include "gui/palette/GradientPalette.h"
#include "themes/IFontRepository.h"
#include "MaterialFileIcon.h"

namespace
{
    static const char* DecodeUtf8Character(const char* src, char16_t& dst)
    {
        u8 c0 = (u8)*src++;
        if (c0 == 0)
        {
            dst = 0;
            return src - 1;
        }

        if ((c0 & 0x80) == 0)
        {
            dst = c0;
            return src;
        }

        if ((c0 & 0xE0) == 0xC0)
        {
            u8 c1 = (u8)*src++;
            if ((c1 & 0xC0) != 0x80)
            {
                dst = '?';
                return src - 1;
            }

            dst = ((c0 & 0x1F) << 6) | (c1 & 0x3F);
            return src;
        }

        if ((c0 & 0xF0) == 0xE0)
        {
            u8 c1 = (u8)*src++;
            u8 c2 = (u8)*src++;
            if ((c1 & 0xC0) != 0x80 || (c2 & 0xC0) != 0x80)
            {
                dst = '?';
                return src - 1;
            }

            dst = ((c0 & 0x0F) << 12) | ((c1 & 0x3F) << 6) | (c2 & 0x3F);
            return src;
        }

        // 4-byte UTF-8 sequences are outside BMP and unsupported in this renderer.
        if ((c0 & 0xF8) == 0xF0)
        {
            for (int i = 0; i < 3 && ((*src & 0xC0) == 0x80); i++)
            {
                src++;
            }
        }

        dst = '?';
        return src;
    }
}

MaterialFileIcon::MaterialFileIcon(const TCHAR* name, const MaterialColorScheme* materialColorScheme,
    const IFontRepository* fontRepository)
    : _materialColorScheme(materialColorScheme), _fontRepository(fontRepository)
{
    int i;
    const char* p = name;
    for (i = 0; i < 3; i++)
    {
        char16_t c;
        p = DecodeUtf8Character(p, c);
        if (c == 0)
            break;

        _displayName[i] = c;
    }
    _displayName[i] = 0;
}

void MaterialFileIcon::UploadGraphics(vu16* vram)
{
    dma_ntrCopy32(3, GetIconTiles(), vram, 32 * 32 / 2);

    auto font = _fontRepository->GetFont(FontType::Medium11);
    u8 tileBuffer[32 * 16 / 2];
    memset(tileBuffer, 0, sizeof(tileBuffer));
    u32 textWidth, textHeight;
    nft2_measureString(font, _displayName, textWidth, textHeight);
    nft2_string_render_params_t renderParams;
    renderParams.x = ((int)32 - (int)textWidth) / 2;
    renderParams.y = 0;
    renderParams.width = 32;
    renderParams.height = 16;
    renderParams.a5i3 = false;
    nft2_renderString(font, _displayName, tileBuffer, 32, &renderParams);
    memcpy((u8*)vram + largeFolderIconTilesLen, tileBuffer, sizeof(tileBuffer));
}

void MaterialFileIcon::Draw(GraphicsContext& graphicsContext, const Rgb<8, 8, 8>& backgroundColor)
{
    auto iconColor = GetIconColor();
    auto nameColor = GetTextColor();

    auto oams = graphicsContext.GetOamManager().AllocOams(2);

    u32 iconPaletteRow = graphicsContext.GetPaletteManager().AllocRow(
        GradientPalette(backgroundColor, iconColor), _position.y, _position.y + 32);
    OamBuilder::OamWithSize<32, 32>(_position.x, _position.y, _vramOffset >> 7)
        .WithPalette16(iconPaletteRow)
        .WithPriority(graphicsContext.GetPriority())
        .Build(oams[1]);

    u32 namePaletteRow = graphicsContext.GetPaletteManager().AllocRow(
        GradientPalette(iconColor, nameColor), _position.y, _position.y + 32);
    OamBuilder::OamWithSize<32, 16>(_position.x, _position.y + GetTextYOffset(), (_vramOffset + largeFolderIconTilesLen) >> 7)
        .WithPalette16(namePaletteRow)
        .WithPriority(graphicsContext.GetPriority())
        .Build(oams[0]);
}
