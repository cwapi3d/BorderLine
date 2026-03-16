#include "EdgeProfileWorkflowService.hh"

#include <ranges>

#include "ColorApplicationService.hh"
#include "ProductionNumberGuard.hh"

namespace borderline::edgeprofile::services {

EdgeProfileWorkflowService::EdgeProfileWorkflowService(
    std::shared_ptr<EdgeImageService> edgeImageService, std::shared_ptr<ShopDrawingExportService> exportService,
    std::shared_ptr<ColorApplicationService> colorApplicationService,
    std::shared_ptr<ports::IAttributeControlPort> attributePort,
    std::shared_ptr<ports::ISortingStrategyFactory> sortingStrategyFactory,
    std::function<std::vector<ElementID>()> elementProvider)
    : mEdgeImageService(std::move(edgeImageService)),
      mExportService(std::move(exportService)),
      mColorApplicationService(std::move(colorApplicationService)),
      mAttributePort(std::move(attributePort)),
      mSortingStrategyFactory(std::move(sortingStrategyFactory)),
      mElementProvider(std::move(elementProvider)) {
}

void EdgeProfileWorkflowService::aggregateAndApplyColor(const WorkflowConfig& config,
                                                        const std::vector<ElementID>& symbolLines) const {
    if (!symbolLines.empty()) {
        mColorApplicationService->applyColorToElements(config.colorNumber, symbolLines);
    }
}

bool EdgeProfileWorkflowService::executeWorkflow(const WorkflowConfig& config, const ProgressCallback progressCallback,
                                                 const CompletionCallback completionCallback) {
    if (mIsRunning.load()) {
        if (completionCallback) {
            completionCallback(false, "Workflow already running", std::nullopt);
        }
        return false;
    }

    mIsRunning.store(true);
    mCancelToken.store(false);

    // Step 1: Generate edge images
    const auto elementGroupings = mEdgeImageService->createEdgeImageLinesAndGroupCabinElementsByNestingParent(
        config.profileConfig, std::invoke(mElementProvider));

    if (config.colorNumber.get() > 0) {
        // clang-format off
        const std::vector<ElementID> flatList = elementGroupings.getEdgeLinesByPanelId()
        | std::views::values
        | std::views::join
        | std::ranges::to<std::vector>();
        // clang-format on

        aggregateAndApplyColor(config, flatList);
    }

    if (elementGroupings.getEdgeLinesByPanelId().empty()) {
        mIsRunning.store(false);
        if (completionCallback) {
            completionCallback(true, "No data to process", std::nullopt);
        }
        return true;
    }

    // If no export required, we're done
    if (!config.shouldExport) {
        mIsRunning.store(false);
        if (completionCallback) {
            completionCallback(true, "Edge images generated successfully", std::nullopt);
        }
        return true;
    }

    // Step 2: Create sorting strategy and process data
    const auto strategy = mSortingStrategyFactory->createStrategy(config.sortingCriteria);
    const auto processingService
        = std::make_shared<ElementGroupProcessingService>(mAttributePort, strategy, config.sortingCriteria);

    const auto chunkedData
        = processingService->sortAndChunk(elementGroupings.getElementsByNestingAssociation(), config.pageLimit);

    // Step 3: Load export settings
    mExportService->loadSettings(config.exportSettingsPath);

    // Step 4: Execute export with progress tracking
    executeExport(chunkedData, progressCallback, completionCallback);

    return true;
}

void EdgeProfileWorkflowService::executeExport(const std::vector<std::vector<ElementID>>& chunkedData,
                                               const ProgressCallback& progressCallback,
                                               const CompletionCallback& completionCallback) {
    const auto allElements = flattenChunkedData(chunkedData);

    // RAII guard: automatically saves/restores production numbers
    constexpr uint32_t productionNumberStart = 40'000;
    ProductionNumberGuard numberGuard(mAttributePort, allElements, productionNumberStart);

    exportBatches(chunkedData, progressCallback);

    const bool cancelled = mCancelToken.load();
    mIsRunning.store(false);

    if (completionCallback) {
        if (cancelled) {
            completionCallback(false, "Export cancelled by user", std::nullopt);
        } else {
            ExportResult result{.startClipboard = 1,
                                .endClipboard = static_cast<int>(chunkedData.size()),
                                .totalElements = static_cast<int>(allElements.size()),
                                .totalBatches = static_cast<int>(chunkedData.size())};
            completionCallback(true, "Export completed successfully", result);
        }
    }
}

void EdgeProfileWorkflowService::exportBatches(const std::vector<std::vector<ElementID>>& chunkedData,
                                               const ProgressCallback& progressCallback) const {
    const int totalBatches = static_cast<int>(chunkedData.size());

    for (int i = 0; i < totalBatches && !mCancelToken.load(); ++i) {
        notifyProgress(i + 1, totalBatches, progressCallback);
        exportSingleBatch(chunkedData, i);
    }
}

void EdgeProfileWorkflowService::notifyProgress(const int current, const int total,
                                                const ProgressCallback& progressCallback) {
    if (progressCallback) {
        progressCallback(current, total);
    }
}

void EdgeProfileWorkflowService::exportSingleBatch(const std::vector<std::vector<ElementID>>& chunkedData,
                                                   const int batchIndex) const {
    try {
        mExportService->exportGroup(chunkedData.at(batchIndex), batchIndex + 1);
    } catch (const std::out_of_range& outOfRange) {
        std::ignore = outOfRange;
    }
}

std::vector<ElementID> EdgeProfileWorkflowService::flattenChunkedData(
    const std::vector<std::vector<ElementID>>& chunkedData) {
    std::vector<ElementID> result;

    for (const auto& chunk : chunkedData) {
        result.insert(result.end(), chunk.begin(), chunk.end());
    }

    return result;
}

void EdgeProfileWorkflowService::cancelWorkflow() {
    mCancelToken.store(true);
}

bool EdgeProfileWorkflowService::isWorkflowRunning() const {
    return mIsRunning.load();
}

}  // namespace borderline::edgeprofile::services
