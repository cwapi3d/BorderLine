//
// Created by MichaelBrunner on 09/12/2025.
//

#pragma once
#include <cstdint>
#include <cstring>

namespace borderline::edgeprofile
{

enum class EEdgePosition
{
  BOTTOM,
  LEFT,
  TOP,
  RIGHT,
};
// -------------------------------------------------- //
enum class EEdgeExistence
{
  EXISTS,
  NOTEXISTS,
};
// -------------------------------------------------- //
enum class EEdgeVerticeID
{
  PT508,
  PT512,
  PT516,
  PT520
};
// -------------------------------------------------- //
enum class EEdgeType : std::uint8_t
{
  COUNTERFLOW = 0,
  CONTINOUS = 1,
  MITER = 2,
  NONE = 3,
};
// -------------------------------------------------- //
enum class ESortingCriteria
{
  NESTING_PL_NUMBER,
  PL_NUMBER,
  NONE,
};
// -------------------------------------------------- //
enum class EEdgeSymbolSize : int
{
  SMALL = 0,
  MEDIUM = 1,
  LARGE = 2,
}; // EEdgeSymbol;

// -------------------------------------------------- //
enum class CW_Attributes : std::uint8_t
{
  GROUP,
  SUBGROUP
};

// -------------------------------------------------- //

inline auto toChar(const CW_Attributes& aAttribute) -> const char*
{
  switch (aAttribute)
  {
    case CW_Attributes::GROUP:
      return "Baugruppe";
    case CW_Attributes::SUBGROUP:
      return "Bauuntergruppe";
    default:
      return "Unknown";
  }
}

// -------------------------------------------------- //

inline auto toSortingCriteria(const int aIndex) -> ESortingCriteria
{
  switch (aIndex)
  {
    case 0:
      return ESortingCriteria::NESTING_PL_NUMBER;
    case 1:
      return ESortingCriteria::PL_NUMBER;
    default:
      return ESortingCriteria::NONE;
  }
}
// -------------------------------------------------- //

inline auto toEdgeSymbolSize(const char* aName) -> EEdgeSymbolSize
{
  if (strcmp(aName, "Small") == 0)
  {
    return EEdgeSymbolSize::SMALL;
  }
  if (strcmp(aName, "Medium") == 0)
  {
    return EEdgeSymbolSize::MEDIUM;
  }
  if (strcmp(aName, "Large") == 0)
  {
    return EEdgeSymbolSize::LARGE;
  }
  return EEdgeSymbolSize::SMALL;
}

// -------------------------------------------------- //
inline auto toChar(const EEdgeSymbolSize& aEdgeSymbolSize) -> const char*
{
  switch (aEdgeSymbolSize)
  {
    case EEdgeSymbolSize::SMALL:
      return "Small";
      break;
    case EEdgeSymbolSize::MEDIUM:
      return "Medium";
      break;
    case EEdgeSymbolSize::LARGE:
      return "Large";
      break;
    default:
      return "Small";
      break;
  }
}

} // namespace borderline::edgeprofile
