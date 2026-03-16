/** @file
 * Copyright (C) 2023 cadwork Holz AG
 *
 *
 * @ingroup       JtcEdges
 * @since         30.0
 * @author        Brunner
 * @date          2023-01-31
 */

#pragma once

#include <borderline/user_interface/user_interface_export.h>

#include <QClipboard>
#include <QComboBox>
#include <QDialog>
#include <QProgressDialog>
#include <memory>

#include "UISettings.hh"
#include "viewmodels/EdgeImageViewModel.hh"

QT_BEGIN_NAMESPACE

namespace Ui {
class CJtcEdgesDialog;
};

QT_END_NAMESPACE

class USER_INTERFACE_EXPORT CJtcEdgesDialog final : public QDialog {
public:
    CJtcEdgesDialog(const CJtcEdgesDialog &aOther) = delete;
    CJtcEdgesDialog(CJtcEdgesDialog &&aOther) noexcept = delete;
    CJtcEdgesDialog &operator=(const CJtcEdgesDialog &aOther) = delete;
    CJtcEdgesDialog &operator=(CJtcEdgesDialog &&aOther) noexcept = delete;

private:
    Q_OBJECT  // NOLINT(*-identifier-length)

public:
    explicit CJtcEdgesDialog(
        std::shared_ptr<borderline::ui::viewmodels::EdgeImageViewModel> viewModel, std::string pluginPath,
        std::function<uint32_t(borderline::edgeprofile::Number)> colorFunc,
        std::function<std::tuple<uint8_t, uint8_t, uint8_t>(borderline::edgeprofile::Number)> colorToRgbFunc,
        QWidget *parent = nullptr);

    ~CJtcEdgesDialog() override;

    void execute();

public slots:
    void onExportFinished();
    void onProgressChanged(int current, int total) const;
    void onWorkflowStatusChanged(bool success, const QString &message);
    void onExportResultAvailable(int startClipboard, int endClipboard, int totalElements, int totalBatches);

private slots:
    void on_pbn_color_clicked();
    void on_pbn_path_clicked();
    void on_tbn_help_clicked();
    void onSymbolSizeChanged(int index) const;
    void onCancelClicked() const;

private:  // NOLINT(*-redundant-access-specifiers)
    void loadSettings();
    void saveSettings();
    void copyClipboardRangeToClipboard() const;
    void updateColorButton(uint32_t color) const;
    void setupExportUI();
    void applyModernStyling();
    QColor convertColorSettingToQColor() const;

    std::unique_ptr<Ui::CJtcEdgesDialog> ui;
    std::shared_ptr<borderline::ui::viewmodels::EdgeImageViewModel> mViewModel;
    borderline::ui::UISettingsData mSettings;
    QProgressDialog *mProgressDialog = nullptr;
    std::string mPluginPath;
    QString mSettingsFilePath;
    int mLastExportStartClip = 0;
    int mLastExportEndClip = 0;
    std::function<uint32_t(borderline::edgeprofile::Number)> mColorCallback;
    std::function<std::tuple<uint8_t, uint8_t, uint8_t>(borderline::edgeprofile::Number)> mColorToRGBFunc;
};
