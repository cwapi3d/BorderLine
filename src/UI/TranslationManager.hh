#pragma once

#include <borderline/user_interface/user_interface_export.h>
#include <QTranslator>
#include <QString>
#include <memory>

namespace borderline::ui {

/**
 * @brief Manages application translations.
 * 
 * This class handles loading and switching between different language translations.
 * It follows the Qt translation system using .qm files.
 */
class USER_INTERFACE_EXPORT TranslationManager final {
public:
    enum class Language {
        English,
        German,
        French,
        Italian
    };

    explicit TranslationManager(QString  translationsPath);
    ~TranslationManager();

    TranslationManager(const TranslationManager&) = delete;
    TranslationManager(TranslationManager&&) = delete;
    TranslationManager& operator=(const TranslationManager&) = delete;
    TranslationManager& operator=(TranslationManager&&) = delete;

    [[nodiscard]] bool loadLanguage(Language language);
    [[nodiscard]] Language getCurrentLanguage() const;
    [[nodiscard]] static QString languageToString(Language language);
    [[nodiscard]] static QString languageToLocale(Language language);

private:
    void unloadCurrentTranslation() const;
    [[nodiscard]] QString getTranslationFileName(Language language) const;

    QString mTranslationsPath;
    std::unique_ptr<QTranslator> mTranslator;
    Language mCurrentLanguage{Language::English};
};

}  // namespace borderline::ui
