#include "Curve_Helper.h"

ostream & marketdata::curve::operator>>(ostream & os, const InterpolationType & interpolation_type)
{
  switch (interpolation_type)
  {
  case NONE_INTERPOLATION:
  {
    os << "NONE_INTERPOLATION";
    break;
  }
  case FLAT_INTERPOLATION:
  {
    os << "FLAT_INTERPOLATION";
    break;
  }
  case LINEAR_INTERPOLATION:
  {
    os << "LINEAR_INTERPOLATION";
    break;
  }
  case SPLINE_INTERPOLATION:
  {
    os << "SPLINE_INTERPOLATION";
    break;
  }
  }

  return os;
}

ostream & marketdata::curve::operator>>(ostream & os, const ExtrapolationType & extrapolation_type)
{
  switch (extrapolation_type)
  {
  case NONE_EXTRAPOLATION:
  {
    os << "NONE_EXTRAPOLATION";
    break;
  }
  case FLAT_EXTRAPOLATION:
  {
    os << "FLAT_EXTRAPOLATION";
    break;
  }
  }

  return os;
}

ostream& marketdata::curve::operator>>(ostream & os, const CurveType & curve_type)
{
  switch (curve_type)
  {
  case DISCOUNTING:
  {
    os << "DISCOUNTING";
    break;
  }
  case FORECASTING_MONTLY:
  {
    os << "FORECASTING_MONTLY";
    break;
  }
  case FORECASTING_QUARTERLY:
  {
    os << "FORECASTING_QUARTERLY";
    break;
  }
  case FORECASTING_SEMIANNUALY:
  {
    os << "FORECASTING_SEMIANNUALY";
    break;
  }
  case FORECASTING_YEARLY:
  {
    os << "FORECASTING_YEARLY";
    break;
  }
  }
  return os;
}
