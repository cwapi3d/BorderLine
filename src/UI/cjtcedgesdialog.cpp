#include "cjtcedgesdialog.h"

#include <QColorDialog>
#include <QFile>
#include <QFileDialog>
#include <QLabel>
#include <QMessageBox>
#include <QMetaObject>
#include <QProgressBar>
#include <QSpacerItem>
#include <QTimer>
#include <QVBoxLayout>
#include <utility>

#include "../EdgeProfile/utility/utility_types.hh"
#include "ui_cjtcedgesdialog.h"

CJtcEdgesDialog::CJtcEdgesDialog(
    std::shared_ptr<borderline::ui::viewmodels::EdgeImageViewModel> viewModel, std::string pluginPath,
    std::function<uint32_t(borderline::edgeprofile::Number)> colorCallback,
    std::function<std::tuple<uint8_t, uint8_t, uint8_t>(borderline::edgeprofile::Number)> colorToRgbFunc,
    QWidget* parent)
    : QDialog(parent),
      ui(std::make_unique<Ui::CJtcEdgesDialog>()),
      mViewModel(std::move(viewModel)),
      mPluginPath(std::move(pluginPath)),
      mSettingsFilePath(QString::fromStdString(mPluginPath) + "/settings/BorderLine.ini"),
      mColorCallback(std::move(colorCallback)),
      mColorToRGBFunc(std::move(colorToRgbFunc)) {
    ui->setupUi(this);

    applyModernStyling();

    connect(ui->pbn_ok, &QAbstractButton::clicked, this, &CJtcEdgesDialog::execute);
    connect(ui->pbn_cancel, &QAbstractButton::clicked, this, &CJtcEdgesDialog::onCancelClicked);

    // Connect to ViewModel signals
    connect(mViewModel.get(), &borderline::ui::viewmodels::EdgeImageViewModel::exportFinished, this,
            &CJtcEdgesDialog::onExportFinished);
    connect(mViewModel.get(), &borderline::ui::viewmodels::EdgeImageViewModel::exportProgressChanged, this,
            &CJtcEdgesDialog::onProgressChanged);
    connect(mViewModel.get(), &borderline::ui::viewmodels::EdgeImageViewModel::workflowStatusChanged, this,
            &CJtcEdgesDialog::onWorkflowStatusChanged);
    connect(mViewModel.get(), &borderline::ui::viewmodels::EdgeImageViewModel::exportResultAvailable, this,
            &CJtcEdgesDialog::onExportResultAvailable);

    ui->lab_image->setPixmap(QPixmap(QString::fromStdString(mPluginPath + "\\kantenbild.png")));

    // Setup sorting options
    if (ui->cbx_sorting->count() == 0) {
        ui->cbx_sorting->addItem(tr("Nesting Pl Number"));
        ui->cbx_sorting->addItem(tr("Pl Number"));
        ui->cbx_sorting->addItem(tr("None"));
    }

    // Connect symbol size combo box
    connect(ui->cbx_symbol_size, QOverload<int>::of(&QComboBox::currentIndexChanged), this,
            &CJtcEdgesDialog::onSymbolSizeChanged);

    loadSettings();
}

void CJtcEdgesDialog::applyModernStyling() {
    // Load stylesheet from external file
    const QString styleFilePath = QString::fromStdString(mPluginPath) + "/styles/modern.qss";
    QFile styleFile(styleFilePath);

    if (styleFile.open(QFile::ReadOnly | QFile::Text)) {
        const QString styleSheet = QLatin1String(styleFile.readAll());
        setStyleSheet(styleSheet);
        styleFile.close();
    } else {
        // Fallback: If file not found, use minimal styling
        qWarning() << "Could not load stylesheet from:" << styleFilePath;
        setStyleSheet("QDialog { background-color: #f5f5f5; }");
    }
}

CJtcEdgesDialog::~CJtcEdgesDialog() = default;

void CJtcEdgesDialog::execute() {
    saveSettings();

    mViewModel->setCreateEdges(mSettings.createEdges);
    mViewModel->setCreateSymbol(mSettings.createSymbol);
    mViewModel->setPageLimit(mSettings.pageLimit);

    if (mSettings.exportTo2d) {
        setupExportUI();
    }

    const auto sorting = borderline::edgeprofile::toSortingCriteria(mSettings.sortingCriteria);

    mViewModel->runProcessing(mSettings.exportTo2d, mSettings.iniFileLocation.toStdString(), sorting, mSettings.color);

    if (!mSettings.exportTo2d) {
        accept();
    }
}

void CJtcEdgesDialog::setupExportUI() {
    ui->pbn_ok->setEnabled(false);
    ui->pbn_cancel->setText(tr("Cancel Export"));

    if (mProgressDialog) {
        delete mProgressDialog;
        mProgressDialog = nullptr;
    }

    mProgressDialog = new QProgressDialog(tr("Preparing export..."), tr("Cancel"), 0, 100, this);
    mProgressDialog->setWindowModality(Qt::WindowModal);
    mProgressDialog->setMinimumDuration(0);
    mProgressDialog->setValue(0);
    mProgressDialog->setAutoClose(false);
    mProgressDialog->setAutoReset(false);
    mProgressDialog->setMinimumSize(400, 120);
    mProgressDialog->setWindowTitle(tr("Export Progress"));

    connect(mProgressDialog, &QProgressDialog::canceled, this, &CJtcEdgesDialog::onCancelClicked);
}

void CJtcEdgesDialog::onCancelClicked() const {
    if (mViewModel->isExportRunning()) {
        mViewModel->cancelExport();
        if (mProgressDialog) {
            mProgressDialog->setLabelText(tr("Cancelling..."));
        }
    }
}

void CJtcEdgesDialog::onProgressChanged(const int current, const int total) const {
    if (mProgressDialog) {
        mProgressDialog->setMaximum(total);
        mProgressDialog->setValue(current);
        mProgressDialog->setLabelText(tr("Exporting batch %1 of %2...").arg(current).arg(total));
    }
}

void CJtcEdgesDialog::onWorkflowStatusChanged(const bool success, const QString& message) {
    if (success) {
        // Show success message in status bar or as brief notification
        if (mProgressDialog) {
            mProgressDialog->setLabelText(message);
        }
    } else {
        // ReSharper disable once CppDFAConstantConditions
        QMessageBox::information(this, success ? tr("Success") : tr("Information"), message);
    }
}

void CJtcEdgesDialog::onExportResultAvailable(const int startClipboard, const int endClipboard, const int totalElements,
                                              const int totalBatches) {
    mLastExportStartClip = startClipboard;
    mLastExportEndClip = endClipboard;

    // Show result dialog with copy button
    QMessageBox resultBox(this);
    resultBox.setWindowTitle(tr("Export Complete"));
    resultBox.setIcon(QMessageBox::Information);

    const QString detailText = tr("Export completed successfully!\n\n"
                                  "Clipboard Range: %1 - %2\n"
                                  "Total Elements: %3\n"
                                  "Total Batches: %4\n\n"
                                  "Click 'Copy Range' to copy the clipboard range to clipboard.")
                                   .arg(startClipboard)
                                   .arg(endClipboard)
                                   .arg(totalElements)
                                   .arg(totalBatches);

    resultBox.setText(detailText);

    auto* copyButton = resultBox.addButton(tr("Copy Range"), QMessageBox::ActionRole);
    resultBox.addButton(QMessageBox::Ok);

    resultBox.exec();

    if (resultBox.clickedButton() == copyButton) {
        copyClipboardRangeToClipboard();
    }
}

void CJtcEdgesDialog::copyClipboardRangeToClipboard() const {
    const QString rangeText = QString("%1 - %2").arg(mLastExportStartClip).arg(mLastExportEndClip);

    QClipboard* clipboard = QApplication::clipboard();
    clipboard->setText(rangeText);

    QMessageBox::information(const_cast<CJtcEdgesDialog*>(this), tr("Copied"),
                             tr("Clipboard range copied: %1").arg(rangeText));
}

void CJtcEdgesDialog::onExportFinished() {
    ui->pbn_ok->setEnabled(true);
    ui->pbn_cancel->setText(tr("Close"));

    if (mProgressDialog) {
        mProgressDialog->setValue(mProgressDialog->maximum());
        mProgressDialog->close();
        mProgressDialog->deleteLater();
        mProgressDialog = nullptr;
    }

    // Status message will be shown via onWorkflowStatusChanged
    if (!mViewModel->isExportRunning()) {
        // Workflow completed, close dialog after brief delay for user to see completion
        QTimer::singleShot(500, this, [this]() { accept(); });
    }
}

QColor CJtcEdgesDialog::convertColorSettingToQColor() const {
    const auto rgb = mColorToRGBFunc(borderline::edgeprofile::Number(mSettings.color));
    const auto QColorRgb = QColor(std::get<0>(rgb), std::get<1>(rgb), std::get<2>(rgb));
    return QColorRgb;
}
void CJtcEdgesDialog::loadSettings() {
    mSettings = borderline::ui::UISettings::load(mSettingsFilePath);

    ui->cbb_edge->setChecked(mSettings.createEdges);
    ui->cbb_symbol->setChecked(mSettings.createSymbol);
    ui->cbb_export->setChecked(mSettings.exportTo2d);
    ui->spb_count_labels->setValue(mSettings.pageLimit);
    ui->cbx_sorting->setCurrentIndex(mSettings.sortingCriteria);
    ui->lab_export->setText(mSettings.iniFileLocation.isEmpty() ? tr("No INI file loaded") : mSettings.iniFileLocation);

    // Set symbol size
    switch (mViewModel->getSymbolSize()) {
        case borderline::edgeprofile::EEdgeSymbolSize::SMALL:
            ui->cbx_symbol_size->setCurrentIndex(0);
            break;
        case borderline::edgeprofile::EEdgeSymbolSize::MEDIUM:
            ui->cbx_symbol_size->setCurrentIndex(1);
            break;
        case borderline::edgeprofile::EEdgeSymbolSize::LARGE:
            ui->cbx_symbol_size->setCurrentIndex(2);
            break;
    }

    mViewModel->setCreateEdges(mSettings.createEdges);
    mViewModel->setCreateSymbol(mSettings.createSymbol);

    const QColor QColorRgb = convertColorSettingToQColor();
    updateColorButton(QColorRgb.rgb());
}

void CJtcEdgesDialog::saveSettings() {
    mSettings.createEdges = ui->cbb_edge->isChecked();
    mSettings.createSymbol = ui->cbb_symbol->isChecked();
    mSettings.exportTo2d = ui->cbb_export->isChecked();
    mSettings.pageLimit = ui->spb_count_labels->value();
    mSettings.sortingCriteria = ui->cbx_sorting->currentIndex();
    mSettings.iniFileLocation = ui->lab_export->text();

    borderline::ui::UISettings::save(mSettingsFilePath, mSettings);
}

void CJtcEdgesDialog::on_pbn_color_clicked() {
    mSettings.color = mColorCallback(borderline::edgeprofile::Number(mSettings.color));

    const QColor QColorRgb = convertColorSettingToQColor();
    updateColorButton(QColorRgb.rgb());
}

void CJtcEdgesDialog::on_pbn_path_clicked() {
    if (const QString fileName
        = QFileDialog::getOpenFileName(this, tr("Select Ini File"), mSettings.iniFileLocation, tr("Ini Files (*.ini)"));
        !fileName.isEmpty()) {
        ui->lab_export->setText(fileName);
        mSettings.iniFileLocation = fileName;
    }
}

void CJtcEdgesDialog::on_tbn_help_clicked() {
    QMessageBox::information(
        this, tr("Help"),
        tr("<h3>Etiketten Schreinerei</h3>"
           "<p><b>Generation Options:</b> Choose what to generate - edge profiles and/or triangle symbols.</p>"
           "<p><b>Symbol Settings:</b> Configure triangle symbol appearance and line colors.</p>"
           "<p><b>Export Settings:</b> Enable 2D export and configure sorting/pagination.</p>"
           "<p>Click <b>Run</b> to generate based on your configuration.</p>"));
}

void CJtcEdgesDialog::updateColorButton(const uint32_t color) const {
    const QColor qColor(color);
    const QString qss = QString("QPushButton#pbn_color { background-color: %1; border: 2px solid #bdc3c7; color: %2; }")
                            .arg(qColor.name())
                            .arg(qColor.lightness() > 128 ? "#2c3e50" : "#ffffff");
    ui->pbn_color->setStyleSheet(qss);
    ui->pbn_color->setText(qColor.name());
}

void CJtcEdgesDialog::onSymbolSizeChanged(const int index) const {
    using namespace borderline::edgeprofile;
    auto size = EEdgeSymbolSize::MEDIUM;
    switch (index) {
        case 0:
            size = EEdgeSymbolSize::SMALL;
            break;
        case 1:
            size = EEdgeSymbolSize::MEDIUM;
            break;
        case 2:
            size = EEdgeSymbolSize::LARGE;
            break;
    }
    mViewModel->setSymbolSize(size);
}
