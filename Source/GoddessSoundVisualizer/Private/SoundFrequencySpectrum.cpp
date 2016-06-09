// Author AndreaCatania

#include "GoddessSoundVisualizerPrivatePCH.h"
#include "SoundFrequencySpectrum.h"

// The sample is approximated to 25 Milliseconds (40 frame per second)
#define SAMPLEAPPROXIMATIONFACTOR 40.0f

USoundFrequencySpectrum::USoundFrequencySpectrum(const FObjectInitializer& ObjectInitializer) {}

FFrequencyBands USoundFrequencySpectrum::FindFrequencyBands(float PlaybackTime) {
	int32 Index = (int32)(PlaybackTime * SAMPLEAPPROXIMATIONFACTOR);
	Index = 0 < Index ? Index - 1 : 0;
	if (SoundFrequencies.Num() > Index) {
		return SoundFrequencies[Index];
	}
	return FFrequencyBands();
}

float USoundFrequencySpectrum::Normalize(const float& InFrequency, const float& MaxFrequency) {
	return InFrequency / MaxFrequency;
}