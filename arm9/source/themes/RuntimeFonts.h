#pragma once
#include "themes/FontType.h"

struct nft2_header_t;

class RuntimeFonts
{
public:
    static void Init();
    static const nft2_header_t* GetFont(FontType fontType);
};
