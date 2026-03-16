#include "ConfigurationAdapter.hh"

#include <QDir>
#include <QFile>
#include <QSettings>

namespace borderline::adapter {

ConfigurationAdapter::ConfigurationAdapter(QString configDirectoryPath, QString filename)
    : mConfigDirectoryPath(std::move(configDirectoryPath)),
      mFileName(std::move(filename)) {
    ensureConfigDirectoryExists();
}

edgeprofile::EdgeOffsetConfig ConfigurationAdapter::loadEdgeOffsetConfig() const {
    const QString filePath = getConfigFilePath();

    // If file doesn't exist, return default configuration
    if (!QFile::exists(filePath)) {
        return createDefaultConfig();
    }

    QSettings settings(filePath, QSettings::IniFormat);

    edgeprofile::EdgeOffsetConfig config;

    // Read offset distances from INI file
    // Group: [EdgeOffsets]
    settings.beginGroup("EdgeOffsets");

    const double counterflowDistance = settings.value("CounterflowDistance", 20.0).toDouble();
    const double continousDistance = settings.value("ContinousDistance", 18.0).toDouble();
    const double miterDistance = settings.value("MiterDistance", 20.0).toDouble();

    settings.endGroup();

    // Apply loaded values
    config.setCounterflowDistance(counterflowDistance);
    config.setContinousDistance(continousDistance);
    config.setMiterDistance(miterDistance);

    return config;
}

bool ConfigurationAdapter::saveEdgeOffsetConfig(const edgeprofile::EdgeOffsetConfig& config) const {
    ensureConfigDirectoryExists();

    const QString filePath = getConfigFilePath();
    QSettings settings(filePath, QSettings::IniFormat);

    // Write offset distances to INI file
    settings.beginGroup("EdgeOffsets");

    settings.setValue("CounterflowDistance", config.getCounterflowDistance());
    settings.setValue("ContinousDistance", config.getContinousDistance());
    settings.setValue("MiterDistance", config.getMiterDistance());

    settings.endGroup();

    // Ensure data is written to disk
    settings.sync();

    return settings.status() == QSettings::NoError;
}

QString ConfigurationAdapter::getConfigFilePath() const {
    return mConfigDirectoryPath + "/" + mFileName;
}

edgeprofile::EdgeOffsetConfig ConfigurationAdapter::createDefaultConfig() {
    edgeprofile::EdgeOffsetConfig config;
    config.setCounterflowDistance(20.0);  // COUNTERFLOW edge offset (mm)
    config.setContinousDistance(18.0);    // CONTINOUS edge offset (mm)
    config.setMiterDistance(20.0);        // MITER edge offset (mm)
    return config;
}

void ConfigurationAdapter::ensureConfigDirectoryExists() const {
    if (const QDir dir; !dir.exists(mConfigDirectoryPath)) {
        std::ignore = dir.mkpath(mConfigDirectoryPath);
    }
}

}  // namespace borderline::adapter
