#pragma once

#include <functional>
#include <optional>
#include <string>
#include <unordered_map>
#include <vector>

#include "EdgeProfileLogic.hh"
#include "types/types.hh"
#include "utility/utility_types.hh"

namespace borderline::edgeprofile::ports {

/**
 * @brief Application service port for edge profile workflow orchestration.
 *
 * This port defines the high-level workflow for generating edge profiles
 * and optionally exporting them. It orchestrates multiple domain services
 * while keeping the UI layer clean of business logic.
 */
class IEdgeProfileWorkflowService {
public:
    virtual ~IEdgeProfileWorkflowService() = default;

    /**
     * @brief Callback invoked during export progress.
     * @param current Current batch index (1-based)
     * @param total Total number of batches
     */
    using ProgressCallback = std::function<void(int current, int total)>;

    /**
     * @brief Export result information.
     */
    struct ExportResult {
        int startClipboard = 1;
        int endClipboard = 1;
        int totalElements = 0;
        int totalBatches = 0;
    };

    /**
     * @brief Callback invoked when workflow completes.
     * @param success Whether the workflow completed successfully
     * @param message Optional status message
     * @param exportResult Optional export result with clipboard range info
     */
    using CompletionCallback
        = std::function<void(bool success, const std::string& message, std::optional<ExportResult> exportResult)>;

    /**
     * @brief Configuration for the workflow execution.
     */
    struct WorkflowConfig {
        ProfileConfig profileConfig;
        bool shouldExport = false;
        std::string exportSettingsPath;
        ESortingCriteria sortingCriteria = ESortingCriteria::NONE;
        int pageLimit = 6;
        Number colorNumber{0};
    };

    /**
     * @brief Executes the complete edge profile workflow.
     *
     * @param config Workflow configuration
     * @param progressCallback Optional callback for progress updates
     * @param completionCallback Callback invoked on completion
     * @return true if workflow started successfully, false otherwise
     */
    [[nodiscard]] virtual bool executeWorkflow(const WorkflowConfig& config, ProgressCallback progressCallback,
                                               CompletionCallback completionCallback) = 0;

    /**
     * @brief Cancels the currently running workflow.
     */
    virtual void cancelWorkflow() = 0;

    /**
     * @brief Checks if a workflow is currently running.
     */
    [[nodiscard]] virtual bool isWorkflowRunning() const = 0;
};

}  // namespace borderline::edgeprofile::ports
