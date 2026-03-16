#pragma once

#include <variant>
#include <cwapi3d/composite/Component.h>

#include "CabinetAggregate.hh"
#include "CabinetElement.hh"
#include "EdgeLine.hh"
#include "NestingAggregate.hh"

namespace borderline::edgeprofile
{
using CabinetData = std::variant<NestingAggregate, CabinetAggregate, CabinetElement, EdgeLine>;

// using NestingComponent = CwAPI3D::Composite::ComponentBase<NestingAggregate>;
using CabinetComponent = CwAPI3D::Composite::ComponentBase<CabinetData>;
using CabinetCompositeNode = CwAPI3D::Composite::CompositeNode<CabinetData>;
using CabinetLeafNode = CwAPI3D::Composite::LeafNode<CabinetData>;
}
