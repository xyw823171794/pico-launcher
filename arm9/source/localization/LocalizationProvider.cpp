#include "common.h"
#include <cstring>
#include "LocalizationProvider.h"

namespace
{
    struct LocalizedTextEntry
    {
        const char* key;
        const char16_t* english;
        const char16_t* chinese;
    };

    static constexpr LocalizedTextEntry sLocalizedTextEntries[] =
    {
        {"ui.display_settings.title", u"Display Settings", u"显示设置"},
        {"ui.display_settings.layout", u"Layout", u"布局"},
        {"ui.display_settings.sorting", u"Sorting", u"排序"},
        {"ui.display_settings.filters", u"Filters", u"筛选"},

        {"ui.game_details.cheats", u"Cheats", u"金手指"},
        {"ui.game_details.favorite", u"Favorite", u"收藏"},

        {"ui.cheats.title", u"Cheats", u"金手指"},
        {"ui.cheats.empty", u"No cheats found.", u"未找到金手指。"},

        {"ui.app_bar.back", u"Back", u"返回"},
        {"ui.app_bar.display_settings", u"Display Settings", u"显示设置"}
    };

    const char16_t* ResolveByLanguage(const LocalizationProvider::Language language,
        const LocalizedTextEntry& entry)
    {
        return language == LocalizationProvider::Language::Chinese
            ? entry.chinese
            : entry.english;
    }
}

LocalizationProvider::LocalizationProvider(const char* language)
{
    if (language != nullptr && strcmp(language, "chinese") == 0)
    {
        _language = Language::Chinese;
    }
    else
    {
        _language = Language::English;
    }
}

const char16_t* LocalizationProvider::Get(const char* key) const
{
    for (const auto& entry : sLocalizedTextEntries)
    {
        if (strcmp(entry.key, key) == 0)
        {
            return ResolveByLanguage(_language, entry);
        }
    }

    return u"";
}
