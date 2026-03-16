#include "TranslationManager.hh"

#include <QApplication>
#include <QDir>
#include <utility>

namespace borderline::ui {

TranslationManager::TranslationManager(QString translationsPath)
    : mTranslationsPath(std::move(translationsPath))
    , mTranslator(std::make_unique<QTranslator>()) {
  // Ensure translations directory exists
  if (const QDir dir(mTranslationsPath); !dir.exists()) {
    std::ignore = dir.mkpath(".");
  }
}

TranslationManager::~TranslationManager() { unloadCurrentTranslation(); }

bool TranslationManager::loadLanguage(const Language language) {
  if (language == mCurrentLanguage && QCoreApplication::installTranslator(mTranslator.get())) {
    return true;
  }

  unloadCurrentTranslation();

  if (const QString translationFile = getTranslationFileName(language);
    !mTranslator->load(translationFile)) {
    // If translation file doesn't exist or can't be loaded, use source language (English)
    mCurrentLanguage = Language::English;
    return language == Language::English;
  }

  if (QCoreApplication::installTranslator(mTranslator.get())) {
    mCurrentLanguage = language;
    return true;
  }

  return false;
}

TranslationManager::Language TranslationManager::getCurrentLanguage() const { return mCurrentLanguage; }

QString TranslationManager::languageToString(const Language language) {
  switch (language) {
    case Language::English:
      return "English";
    case Language::German:
      return "Deutsch";
    case Language::French:
      return "Français";
    case Language::Italian:
      return "Italiano";
  }
  return "English";
}

QString TranslationManager::languageToLocale(const Language language) {
  switch (language) {
    case Language::English:
      return "en";
    case Language::German:
      return "de";
    case Language::French:
      return "fr";
    case Language::Italian:
      return "it";
  }
  return "en";
}

void TranslationManager::unloadCurrentTranslation() const {
  if (mTranslator) {
    QCoreApplication::removeTranslator(mTranslator.get());
  }
}

QString TranslationManager::getTranslationFileName(const Language language) const {
  const QString locale = languageToLocale(language);
  return QDir(mTranslationsPath).filePath(QString("borderline_%1.qm").arg(locale));
}

}  // namespace borderline::ui
