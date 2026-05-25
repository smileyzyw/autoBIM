#pragma once

#include <algorithm>
#include <cmath>

namespace vvenc {

struct BimSearchParams
{
  double diffBoost;
  double center;
  double threshLowStrong;
  double threshLowWeak;
  double threshHighWeak;
  double threshHighStrong;
  double rmsGain;
  double rmsBias;
  double rmsScaleSmall;
  double rmsScaleLarge;
};

struct BimQpOffsetFeatures
{
  int avgErrD1;
  int avgErrD2;
  double weight;
  double factor;
  double meanRmsAcrossPic;
  double ctuRms;
};

BimSearchParams makeGeneratedBimSearchParams();
int computeGeneratedBimQpOffset( const BimSearchParams& params, const BimQpOffsetFeatures& features );

inline const BimSearchParams& getBimSearchParams()
{
  static const BimSearchParams params = makeGeneratedBimSearchParams();
  return params;
}

inline int getGeneratedBimQpOffset( const BimQpOffsetFeatures& features )
{
  return computeGeneratedBimQpOffset( getBimSearchParams(), features );
}

} // namespace vvenc

#include "BimHeuristicGenerated.h"
