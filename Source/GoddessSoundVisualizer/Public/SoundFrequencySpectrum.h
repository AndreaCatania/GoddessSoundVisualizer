// Author AndreaCatania

#pragma once

#include "Object.h"
#include "SoundFrequencySpectrum.generated.h"

USTRUCT(BlueprintType)
struct FFrequencyBands {

	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FrequencyBands)
	float SubBass; // 20hz - 60hz
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FrequencyBands)
	float Bass; // 60hz - 250Khz
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FrequencyBands)
	float LowMidrange; // 250hz - 500hz
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FrequencyBands)
	float Midrange; // 500hz - 2Khz
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FrequencyBands)
	float UpperMidrange; // 2Khz - 4Khz
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FrequencyBands)
	float Presence; // 4Khz - 6Khz
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FrequencyBands)
	float Brilliance; // 6Khz - 20Khz

	FFrequencyBands() {
		SubBass = 0;
		Bass = 0;
		LowMidrange = 0;
		Midrange = 0;
		UpperMidrange = 0;
		Presence = 0;
		Brilliance = 0;
	}
};

UCLASS()
class GODDESSSOUNDVISUALIZER_API USoundFrequencySpectrum : public UObject
{
	GENERATED_BODY()

public:

	USoundFrequencySpectrum(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	/** The list of all frequencies for each 0.1sec of sound. Each row contains the average of frequencies from N to N+0.1 where N is the PlaybackTime in seconds for each band. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Property)
	TArray<FFrequencyBands> SoundFrequencies;

	/** An utility variable used to normalize the frequency. Holds the biggest values of frequency for each of 7 bands */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Property)
	FFrequencyBands BiggestFrequencyBands;

	/** Return the Frequency bands for the PlaybackTime submitted, If the frequency bands are not found returns 0. */
	UFUNCTION()
	FFrequencyBands FindFrequencyBands(float PlaybackTime);

	/** Returning a number from 0 to 1 according to the "InFrequency" and the "MaxFrequency" of Sound. */
	static float Normalize(const float& InFrequency, const float& MaxFrequency);

};
