//
// Created by MichaelBrunner on 09/12/2025.
//

#include "CabinetElementAttributes.hh"

#include "IAttributeControlPort.hh"

namespace borderline::edgeprofile
{
CabinetElementAttributes::CabinetElementAttributes(
    const std::shared_ptr<ports::IAttributeControlPort> &aAttributeControlPort,
    const ElementID &aElementId,
    const std::optional<Number> &aPreviousProductionNumber)
    : mAttributeControlPort(aAttributeControlPort),
      mElementID(aElementId),
      mPreviousProductionNumber(aPreviousProductionNumber)
{
}

Number CabinetElementAttributes::getProductionNumber() const
{
    return mAttributeControlPort->getProductionNumber(mElementID);
}

Number CabinetElementAttributes::getAssemblyNumber() const
{
    return mAttributeControlPort->getAssemblyNumber(mElementID);
}

void CabinetElementAttributes::setTemporaryProductionNumber(Number number)
{
    mPreviousProductionNumber = getProductionNumber();
    mAttributeControlPort->setProductionNumber(mElementID, number);
}

void CabinetElementAttributes::resetTemporaryProductionNumber()
{
    if (mPreviousProductionNumber) {
        mAttributeControlPort->setProductionNumber(mElementID, mPreviousProductionNumber.value());
    }
    mPreviousProductionNumber = std::nullopt;
}
} // edgeprofile
// borderline
