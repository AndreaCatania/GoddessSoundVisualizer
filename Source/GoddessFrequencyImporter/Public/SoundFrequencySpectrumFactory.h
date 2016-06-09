// Author AndreaCatania

#pragma once

#include "Factories/Factory.h"
#include "SoundFrequencySpectrumFactory.generated.h"

UCLASS()
class GODDESSFREQUENCYIMPORTER_API USoundFrequencySpectrumFactory : public UFactory
{
	GENERATED_BODY()

public:
	USoundFrequencySpectrumFactory(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	
	virtual UObject* FactoryCreateBinary(UClass* Class, UObject* InParent, FName Name, EObjectFlags Flags, UObject* Context, const TCHAR* Type, const uint8*& Buffer, const uint8* BufferEnd, FFeedbackContext* Warn) override;

private:
	void CalculateFrequencySpectrum(uint8* PCMDataBuffer, const uint32 PCMBufferSize, const FWaveModInfo& WaveInfo, float InStartTime, float InDuration, TArray<float> & OutFrequencies);
	void GetAverageFrequencyValueInRange(const uint32 SampleRate, TArray<float> InFrequencies, int32 InStartFrequence, int32 InEndFrequence, float& OutAverageFrequency);
	float GetFFTInValue(const int16 InSampleValue, const int16 InSampleIndex, const int16 InSampleCount);
};
