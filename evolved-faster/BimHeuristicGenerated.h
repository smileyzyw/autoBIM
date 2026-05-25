#pragma once

namespace vvenc {

inline BimSearchParams makeGeneratedBimSearchParams()
{
  return BimSearchParams{
    3.700000,
    46.000000,
    11.500000,
    26.500000,
    57.000000,
    73.500000,
    6.500000,
    -0.550000,
    0.470000,
    0.730000
  };
}

inline int computeGeneratedBimQpOffset( const BimSearchParams& params, const BimQpOffsetFeatures& features )
{
  const int avgErrD1 = features.avgErrD1;
  const int avgErrD2 = features.avgErrD2;
  const double weight = features.weight;
  const double factor = features.factor;
  const double meanRmsAcrossPic = features.meanRmsAcrossPic;
  const double ctuRms = features.ctuRms;
  const double safeWeight = std::max( weight, 1e-6 );
  const double safeWeightSqrt = std::sqrt( safeWeight );
  const double safeScaledCtuRms = std::max( ctuRms * std::max( factor, 1e-6 ), 1e-6 );
  const double safeMeanRmsAcrossPic = std::max( meanRmsAcrossPic, 1e-6 );
  const double safeLog2 = std::log( 2.0 );

  const double d1 = static_cast<double>(avgErrD1);
  const double d2 = static_cast<double>(avgErrD2);
  const double errMax = std::max(d1, d2);
  const double errMin = std::max(0.01, std::min(d1, d2));
  const double errGap = std::abs(d2 - d1);
  const double errMean = 0.5 * (d1 + d2);
  const double gapRatio = errGap / (errMean + 0.01);
  const double activityRatio = safeScaledCtuRms / (safeMeanRmsAcrossPic + 0.01);
  const double activityClamped = std::min(1.0, activityRatio);
  const double directionPolarity = (d1 > d2) ? 1.0 : -1.0;
  const double conflictFactor = std::min(1.0, gapRatio * (1.0 + 0.5 * activityClamped));
  const double errProduct = d1 * d2 / (0.5 * (d1 + d2) + 0.01);
  const double baseTerm = errMax * (0.55 + 0.45 * (1.0 - conflictFactor)) + errGap * params.diffBoost * conflictFactor + activityClamped * 1.5 * conflictFactor;
  const double neutralPenalty = (errMean > 50.0 && errGap < 5.0) ? (errMean - 50.0) * 0.15 : 0.0;
  const double weightedErr = (baseTerm - neutralPenalty) * weight + (1.0 - weight) * params.center;
  const double rmsScale = params.rmsScaleSmall + (params.rmsScaleLarge - params.rmsScaleSmall) * (0.45 + 0.55 * conflictFactor);

  int qpOffset = 0;

  if (weightedErr > params.threshHighStrong && errGap > 10.0 && activityClamped > 0.45 && errMean > 28.0)
  {
      qpOffset = 3;
  }
  else if (weightedErr > params.threshHighWeak && errGap > 7.0 && errMean > 24.0)
  {
      qpOffset = 2;
  }
  else if (weightedErr > params.threshHighWeak && activityClamped < 0.5)
  {
      qpOffset = 1;
  }
  else if (weightedErr > params.threshHighWeak)
  {
      qpOffset = -1;
  }
  else if (weightedErr < params.threshLowStrong && errMean < 13.0 && errGap < 3.0 && activityClamped < 0.3)
  {
      qpOffset = -3;
  }
  else if (weightedErr < params.threshLowWeak && errGap < 3.5 && activityClamped < 0.4)
  {
      qpOffset = -2;
  }
  else if (weightedErr < params.threshLowWeak)
  {
      qpOffset = -1;
  }

  if (meanRmsAcrossPic < ctuRms * factor && (weightedErr > params.threshLowWeak || conflictFactor > 0.4))
  {
      const double rmsRatio = std::max(rmsScale * safeScaledCtuRms, safeMeanRmsAcrossPic) / safeScaledCtuRms;
      qpOffset += static_cast<int>(params.rmsGain * std::log(rmsRatio) / safeWeightSqrt / safeLog2 + params.rmsBias);
  }

  return qpOffset;
}

} // namespace vvenc
