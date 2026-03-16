#include "UtilityControl.hh"

#include <cwapi3d/ICwAPI3DControllerFactory.h>

namespace borderline::adapter::cadwork {

UtilityControl::UtilityControl(CwAPI3D::Interfaces::ICwAPI3DControllerFactory* factory)
    : mFactory(factory) {
}

uint32_t UtilityControl::getUserColor(const edgeprofile::Number colorNumber) const {
    return mFactory->getUtilityController()->getUserColor(colorNumber.get());
}

}  // namespace borderline::adapter::cadwork
