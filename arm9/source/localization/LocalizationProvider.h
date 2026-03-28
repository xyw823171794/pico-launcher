#pragma once

class LocalizationProvider
{
public:
    enum class Language
    {
        English,
        Chinese
    };

    explicit LocalizationProvider(const char* language);

    const char16_t* Get(const char* key) const;

private:
    Language _language = Language::English;
};
