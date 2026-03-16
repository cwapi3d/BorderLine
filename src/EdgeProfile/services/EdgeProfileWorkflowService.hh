#pragma once

#include <borderline/edge_profile/edge_profile_export.h>

#include <atomic>
#include <memory>

#include "ports/IAttributeControlPort.hh"
#include "ports/IEdgeProfileWorkflowService.hh"
#include "ports/ISortingStrategyFactory.hh"
#include "services/EdgeImageService.hh"
#include "services/ElementGroupProcessingService.hh"
#include "services/ShopDrawingExportService.hh"

namespace borderline::edgeprofile::services {
class ColorApplicationService;

/**
 * @brief Concrete implementation of edge profile workflow orchestration.
 *
 * This application service coordinates multiple domain services to execute
 * the complete workflow: generate edge images, sort/chunk data, and export.
 * It contains no UI dependencies and can be tested independently.
 */
class EDGE_PROFILE_EXPORT EdgeProfileWorkflowService : public ports::IEdgeProfileWorkflowService {
public:
    explicit EdgeProfileWorkflowService(std::shared_ptr<EdgeImageService> edgeImageService,
                                        std::shared_ptr<ShopDrawingExportService> exportService,
                                        std::shared_ptr<ColorApplicationService> colorApplicationService,
                                        std::shared_ptr<ports::IAttributeControlPort> attributePort,
                                        std::shared_ptr<ports::ISortingStrategyFactory> sortingStrategyFactory,
                                        std::function<std::vector<ElementID>()> elementProvider);

    void aggregateAndApplyColor(const WorkflowConfig& config, const std::vector<ElementID>& symbolLines) const;

    [[nodiscard]] bool executeWorkflow(const WorkflowConfig& config, ProgressCallback progressCallback,
                                       CompletionCallback completionCallback) override;

    void cancelWorkflow() override;

    [[nodiscard]] bool isWorkflowRunning() const override;

private:
    void executeExport(const std::vector<std::vector<ElementID>>& chunkedData, const ProgressCallback& progressCallback,
                       const CompletionCallback& completionCallback);

    void exportBatches(const std::vector<std::vector<ElementID>>& chunkedData,
                       const ProgressCallback& progressCallback) const;

    void exportSingleBatch(const std::vector<std::vector<ElementID>>& chunkedData, int batchIndex) const;

    static void notifyProgress(int current, int total, const ProgressCallback& progressCallback);

    [[nodiscard]] static std::vector<ElementID> flattenChunkedData(
        const std::vector<std::vector<ElementID>>& chunkedData);

    std::shared_ptr<EdgeImageService> mEdgeImageService;
    std::shared_ptr<ShopDrawingExportService> mExportService;
    std::shared_ptr<ColorApplicationService> mColorApplicationService;
    std::shared_ptr<ports::IAttributeControlPort> mAttributePort;
    std::shared_ptr<ports::ISortingStrategyFactory> mSortingStrategyFactory;
    std::function<std::vector<ElementID>()> mElementProvider;

    std::atomic<bool> mCancelToken{false};
    std::atomic<bool> mIsRunning{false};
};

}  // namespace borderline::edgeprofile::services
