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

static char16_t DecodeUtf8Char(const char*& text)
{
    u8 c0 = *text++;
    if (c0 == 0)
        return 0;
    if ((c0 & 0x80) == 0)
        return c0;

    u8 c1 = *text++;
    if ((c1 & 0xC0) != 0x80)
        return '?';

    if ((c0 & 0xE0) == 0xC0)
        return ((c0 & 0x1F) << 6) | (c1 & 0x3F);

    u8 c2 = *text++;
    if ((c2 & 0xC0) != 0x80)
        return '?';

    if ((c0 & 0xF0) == 0xE0)
        return ((c0 & 0x0F) << 12) | ((c1 & 0x3F) << 6) | (c2 & 0x3F);

    // 4-byte UTF-8 codepoints are outside BMP and unsupported by our char16 renderer.
    text++;
    return '?';
}

MaterialFileIcon::MaterialFileIcon(const TCHAR* name, const MaterialColorScheme* materialColorScheme,
    const IFontRepository* fontRepository)
    : _materialColorScheme(materialColorScheme), _fontRepository(fontRepository)
{
    const char* utf8Name = name;
    int i;
    for (i = 0; i < 3; i++)
    {
        char16_t c = DecodeUtf8Char(utf8Name);
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
