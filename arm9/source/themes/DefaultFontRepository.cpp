#include "common.h"
#include "RuntimeFonts.h"
#include "DefaultFontRepository.h"

const nft2_header_t* DefaultFontRepository::GetFont(FontType fontType) const
{
    return RuntimeFonts::GetFont(fontType);
}
