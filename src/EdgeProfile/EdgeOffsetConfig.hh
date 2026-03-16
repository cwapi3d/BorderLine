//
// Created by MichaelBrunner on 10/12/2025.
//

#pragma once
#include <borderline/edge_profile/edge_profile_export.h>

namespace borderline::edgeprofile
{
/**
 * @brief Configuration class for edge offset distances
 * 
 * Defines two types of offsets:
 * 1. General offset: Moves entire edge inward into the panel (default 20mm)
 * 2. Edge type offset: Adjusts individual points based on COUNTERFLOW/CONTINOUS/MITER type
 */
class EDGE_PROFILE_EXPORT EdgeOffsetConfig
{
public:
    /**
     * @brief Constructs an EdgeOffsetConfig with specified offset distances
     * @param generalOffset General inward offset for all edges (default: 20.0mm)
     * @param counterflowDistance Additional offset for COUNTERFLOW edge type (default: 20.0mm)
     * @param continousDistance Additional offset for CONTINOUS edge type (default: 18.0mm)
     * @param miterDistance Additional offset for MITER edge type (default: 20.0mm)
     */
    explicit EdgeOffsetConfig(const double generalOffset = 20.0,
                             const double counterflowDistance = 20.0,
                             const double continousDistance = 18.0,
                             const double miterDistance = 20.0)
        : mGeneralOffset(generalOffset),
          mCounterflowDistance(counterflowDistance),
          mContinousDistance(continousDistance),
          mMiterDistance(miterDistance)
    {
    }

    [[nodiscard]] double getGeneralOffset() const { return mGeneralOffset; }
    [[nodiscard]] double getCounterflowDistance() const { return mCounterflowDistance; }
    [[nodiscard]] double getContinousDistance() const { return mContinousDistance; }
    [[nodiscard]] double getMiterDistance() const { return mMiterDistance; }

    void setGeneralOffset(const double offset) { mGeneralOffset = offset; }
    void setCounterflowDistance(const double distance) { mCounterflowDistance = distance; }
    void setContinousDistance(const double distance) { mContinousDistance = distance; }
    void setMiterDistance(const double distance) { mMiterDistance = distance; }

private:
    double mGeneralOffset;
    double mCounterflowDistance;
    double mContinousDistance;
    double mMiterDistance;
};
}
