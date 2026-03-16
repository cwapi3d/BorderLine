#pragma once

#include <borderline/user_interface/user_interface_export.h>

#include <QObject>
#include <memory>

#include "EdgeProfileLogic.hh"
#include "IEdgeProfileWorkflowService.hh"
#include "utility/utility_types.hh"

class QTimer;

namespace borderline::ui::viewmodels {

class USER_INTERFACE_EXPORT EdgeImageViewModel : public QObject {
    Q_OBJECT
public:
    explicit EdgeImageViewModel(std::shared_ptr<edgeprofile::ports::IEdgeProfileWorkflowService> workflowService,
                                const edgeprofile::ProfileConfig& config, QObject* parent = nullptr);

    ~EdgeImageViewModel() override;

    // Main entry point for the workflow
    void runProcessing(bool doExport, const std::string& iniPath, edgeprofile::ESortingCriteria sorting,
                       uint32_t colorNumber);

    Q_INVOKABLE void cancelExport() const;
    [[nodiscard]] bool isExportRunning() const;

    Q_INVOKABLE void setSymbolSize(edgeprofile::EEdgeSymbolSize size);
    [[nodiscard]] edgeprofile::EEdgeSymbolSize getSymbolSize() const;

    Q_INVOKABLE void setCreateEdges(bool create);
    [[nodiscard]] bool getCreateEdges() const;

    Q_INVOKABLE void setCreateSymbol(bool create);
    [[nodiscard]] bool getCreateSymbol() const;

    Q_INVOKABLE void setPageLimit(int limit);
    [[nodiscard]] int getPageLimit() const;

signals:
    void exportFinished();
    void exportProgressChanged(int current, int total);
    void workflowStatusChanged(bool success, const QString& message);
    void exportResultAvailable(int startClipboard, int endClipboard, int totalElements, int totalBatches);

private slots:
    void onWorkflowProgress(int current, int total);
    void onWorkflowCompleted(bool success, const std::string& message,
                             std::optional<edgeprofile::ports::IEdgeProfileWorkflowService::ExportResult> exportResult);

private:
    std::shared_ptr<edgeprofile::ports::IEdgeProfileWorkflowService> mWorkflowService;
    edgeprofile::ProfileConfig mConfig;
    int mPageLimit = 6;
    edgeprofile::ports::IEdgeProfileWorkflowService::ExportResult mLastExportResult;
};
}  // namespace borderline::ui::viewmodels
