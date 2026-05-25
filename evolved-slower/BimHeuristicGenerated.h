#pragma once

namespace vvenc {

inline BimSearchParams makeGeneratedBimSearchParams()
{
  return BimSearchParams{
    1.500000,
    55.000000,
    8.000000,
    20.000000,
    60.000000,
    90.000000,
    5.000000,
    -2.000000,
    0.300000,
    0.700000
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

  const double tmpErrGap = std::abs( static_cast<double>( avgErrD2 - avgErrD1 ) );
  const double tmpErrMean = 0.5 * ( static_cast<double>( avgErrD1 ) + static_cast<double>( avgErrD2 ) );
  const double minErr = std::min( static_cast<double>( avgErrD1 ), static_cast<double>( avgErrD2 ) );
  const double weightedErrBase = 0.70 * std::max( static_cast<double>( avgErrD1 ), static_cast<double>( avgErrD2 ) )
    + 0.20 * tmpErrMean
    + 0.10 * minErr
    + tmpErrGap * params.diffBoost;
  const double weightedErr = weightedErrBase * weight + ( 1.0 - weight ) * params.center;
  const double rmsScale = params.rmsScaleSmall + ( params.rmsScaleLarge - params.rmsScaleSmall ) * weight;

  int qpOffset = 0;

  if( weightedErr > params.threshHighStrong )
  {
    qpOffset = 2;
  }
  else if( weightedErr > params.threshHighWeak )
  {
    qpOffset = 0;
  }
  else if( weightedErr < params.threshLowStrong && tmpErrGap < 3.0 && ( avgErrD1 < 12.0 || avgErrD2 < 12.0 ) )
  {
    qpOffset = -3;
  }
  else if( weightedErr < params.threshLowWeak )
  {
    qpOffset = -2;
  }
  else if( weightedErr > params.threshLowWeak && weightedErr < params.threshHighWeak && tmpErrGap < 5.0 && minErr < 20.0 )
  {
    qpOffset = -1;
  }

  if( meanRmsAcrossPic < ctuRms * factor )
  {
    const double rmsRatio = std::max( rmsScale * safeScaledCtuRms, safeMeanRmsAcrossPic ) / safeScaledCtuRms;
    qpOffset += int( params.rmsGain * std::log( rmsRatio ) / safeWeightSqrt / safeLog2 + params.rmsBias );
  }

  return qpOffset;
}

} // namespace vvenc
