#pragma once

#include <borderline/adapter/cadwork/cadwork_adapter_export.h>

#include <QString>

#include "ports/IConfigurationPort.hh"

namespace borderline::adapter {

class CADWORK_ADAPTER_EXPORT ConfigurationAdapter final : public edgeprofile::ports::IConfigurationPort {
public:
    explicit ConfigurationAdapter(QString configDirectoryPath, QString filename = "EdgeOffsetConfig.ini");

    [[nodiscard]] edgeprofile::EdgeOffsetConfig loadEdgeOffsetConfig() const override;

    [[nodiscard]] bool saveEdgeOffsetConfig(const edgeprofile::EdgeOffsetConfig& config) const override;

private:
    QString mConfigDirectoryPath;
    QString mFileName;

    [[nodiscard]] QString getConfigFilePath() const;

    [[nodiscard]] static edgeprofile::EdgeOffsetConfig createDefaultConfig();

    void ensureConfigDirectoryExists() const;
};

}  // namespace borderline::adapter
