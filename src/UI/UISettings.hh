#pragma once

#include <QSettings>
#include <QString>
#include <QDir>

namespace borderline::ui
{

struct UISettingsData
{
    bool createEdges{false};
    bool createSymbol{false};
    bool exportTo2d{false};
    uint32_t color{0};
    int pageLimit{1};
    int sortingCriteria{0};
    QString iniFileLocation;
    QString userProfilePath;
};

class UISettings
{
public:
    static UISettingsData load(const QString& settingsFilePath)
    {
        QSettings settings(settingsFilePath, QSettings::IniFormat);
        UISettingsData data;
        data.createEdges = settings.value("createEdges", false).toBool();
        data.createSymbol = settings.value("createSymbol", false).toBool();
        data.exportTo2d = settings.value("exportTo2d", false).toBool();
        data.color = settings.value("color", 0).toUInt();
        data.pageLimit = settings.value("pageLimit", 1).toInt();
        data.sortingCriteria = settings.value("sortingCriteria", 0).toInt();
        data.iniFileLocation = settings.value("iniFileLocation", "").toString();
        data.userProfilePath = settings.value("userProfilePath", "").toString();
        return data;
    }

    static void save(const QString& settingsFilePath, const UISettingsData& data)
    {
        // Ensure settings directory exists
        QFileInfo fileInfo(settingsFilePath);
        QDir dir = fileInfo.absoluteDir();
        if (!dir.exists()) {
            dir.mkpath(".");
        }

        QSettings settings(settingsFilePath, QSettings::IniFormat);
        settings.setValue("createEdges", data.createEdges);
        settings.setValue("createSymbol", data.createSymbol);
        settings.setValue("exportTo2d", data.exportTo2d);
        settings.setValue("color", data.color);
        settings.setValue("pageLimit", data.pageLimit);
        settings.setValue("sortingCriteria", data.sortingCriteria);
        settings.setValue("iniFileLocation", data.iniFileLocation);
        settings.setValue("userProfilePath", data.userProfilePath);
        settings.sync();
    }
};

}
