#pragma once

#include "../EdgeOffsetConfig.hh"

namespace borderline::edgeprofile::ports {

/**
 * @brief Port for loading configuration from external sources
 *
 * This interface defines the contract for configuration loading.
 * Implementations handle the actual I/O operations (file system, database, etc.)
 * following the Dependency Inversion Principle.
 */
class IConfigurationPort {
public:
    virtual ~IConfigurationPort() = default;

    /**
     * @brief Load edge offset configuration
     * @return EdgeOffsetConfig with loaded values, or default values if source unavailable
     */
    [[nodiscard]] virtual EdgeOffsetConfig loadEdgeOffsetConfig() const = 0;

    /**
     * @brief Save edge offset configuration
     * @param config Configuration to persist
     * @return true if saved successfully, false otherwise
     */
    virtual bool saveEdgeOffsetConfig(const EdgeOffsetConfig& config) const = 0;
};

}  // namespace borderline::edgeprofile::ports
