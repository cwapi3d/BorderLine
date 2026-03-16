#include "EdgeImageViewModel.hh"

namespace borderline::ui::viewmodels {

EdgeImageViewModel::EdgeImageViewModel(
    std::shared_ptr<edgeprofile::ports::IEdgeProfileWorkflowService> workflowService,
    const edgeprofile::ProfileConfig& config,
    QObject* parent)
    : QObject(parent),
      mWorkflowService(std::move(workflowService)),
      mConfig(config) {}

EdgeImageViewModel::~EdgeImageViewModel() {
  if (mWorkflowService) {
    mWorkflowService->cancelWorkflow();
  }
}

void EdgeImageViewModel::runProcessing(const bool doExport, const std::string& iniPath,
                                       const edgeprofile::ESortingCriteria sorting, const uint32_t colorNumber) {
    if (mWorkflowService->isWorkflowRunning()) {
    return;
  }

  // Build workflow configuration from UI state
  edgeprofile::ports::IEdgeProfileWorkflowService::WorkflowConfig config{
      .profileConfig = mConfig,
      .shouldExport = doExport,
      .exportSettingsPath = iniPath,
      .sortingCriteria = sorting,
      .pageLimit = mPageLimit,
      .colorNumber = edgeprofile::Number{colorNumber}};

  // Execute workflow with callbacks
  auto progressCallback = [this](const int current, const int total) {
    onWorkflowProgress(current, total);
  };

  auto completionCallback = [this](const bool success, const std::string& message,
                                   const std::optional<edgeprofile::ports::IEdgeProfileWorkflowService::ExportResult>& exportResult) {
    onWorkflowCompleted(success, message, exportResult);
  };

 std::ignore = mWorkflowService->executeWorkflow(config, progressCallback, completionCallback);
}

void EdgeImageViewModel::onWorkflowProgress(const int current, const int total) {
  emit exportProgressChanged(current, total);
}

void EdgeImageViewModel::onWorkflowCompleted(const bool success, const std::string& message,
                                             std::optional<edgeprofile::ports::IEdgeProfileWorkflowService::ExportResult> exportResult) {
  QString statusMessage = QString::fromStdString(message);
  
  if (exportResult.has_value()) {
    // Format additional information for export
    statusMessage += QString("\n\nExport Details:");
    statusMessage += QString("\n• Clipboard Range: %1 - %2")
                         .arg(exportResult->startClipboard)
                         .arg(exportResult->endClipboard);
    statusMessage += QString("\n• Total Elements: %1")
                         .arg(exportResult->totalElements);
    statusMessage += QString("\n• Total Batches: %1")
                         .arg(exportResult->totalBatches);
    
    // Store for clipboard copy functionality
    mLastExportResult = *exportResult;
    emit exportResultAvailable(exportResult->startClipboard, exportResult->endClipboard, 
                              exportResult->totalElements, exportResult->totalBatches);
  }
  
  emit workflowStatusChanged(success, statusMessage);
  emit exportFinished();
}

void EdgeImageViewModel::cancelExport() const {
  mWorkflowService->cancelWorkflow();
}

bool EdgeImageViewModel::isExportRunning() const {
  return mWorkflowService->isWorkflowRunning();
}

void EdgeImageViewModel::setSymbolSize(const edgeprofile::EEdgeSymbolSize size) { mConfig.mSymbolSize = size; }

edgeprofile::EEdgeSymbolSize EdgeImageViewModel::getSymbolSize() const { return mConfig.mSymbolSize; }

void EdgeImageViewModel::setCreateEdges(const bool create) { mConfig.mCreateEdges = create; }

bool EdgeImageViewModel::getCreateEdges() const { return mConfig.mCreateEdges; }

void EdgeImageViewModel::setCreateSymbol(const bool create) { mConfig.mCreateSymbol = create; }

bool EdgeImageViewModel::getCreateSymbol() const { return mConfig.mCreateSymbol; }

void EdgeImageViewModel::setPageLimit(const int limit) { mPageLimit = limit; }

int EdgeImageViewModel::getPageLimit() const { return mPageLimit; }
}  // namespace borderline::ui::viewmodels
