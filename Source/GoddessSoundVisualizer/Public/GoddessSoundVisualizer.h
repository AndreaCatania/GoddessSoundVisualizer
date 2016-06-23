// Author AndreaCatania

#pragma once

#include "GameFramework/Actor.h"
#include "GoddessSoundVisualizer.generated.h"

UCLASS()
class GODDESSSOUNDVISUALIZER_API AGoddessSoundVisualizer : public AActor
{
	GENERATED_BODY()
	
public:	

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Sound)
	class UAudioComponent* MainAudioComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Sound)
	class USoundFrequencySpectrum* SoundFrequencySpectrum;
	
	struct FActiveSound* ActiveSound;

	/** Parameter used to handle Pause/Resume */
	float PlaybackTimePaused;
	/** This parameter is used to trak if the soud is Finished artificially by Stop or Pause function */
	bool FinishedArtificially;

	AGoddessSoundVisualizer();

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;

	/** 
	 * This function calls Blueprint implementable event function "BP_OnUpdateFrequency" so if you override this and you need Bluepirnt event you must call it manually.
	 * (This is done for performance reason when you want only c++ running)
	 */
	virtual void OnUpdateFrequency(struct FFrequencyBands FrequencyBands);

	/**
	 * This function calls Blueprint implementable event function "BP_OnAudioFinished" so if you override this and you need Bluepirnt event you must call it manually.
	 * (This is done for performance reason when you want only c++ running)
	 */
	UFUNCTION()
	virtual void OnAudioFinished();

	/** Play sound from the beginning */
	UFUNCTION(BlueprintCallable, Category = Controller)
	void Play(class USoundBase* Sound, class USoundFrequencySpectrum* InSoundFrequencySpectrum);

	/** Stop the sound (cannot be resumed)*/
	UFUNCTION(BlueprintCallable, Category = Controller)
	void Stop();

	/** Pause the sound (can be resumed) */
	UFUNCTION(BlueprintCallable, Category = Controller)
	void Pause();

	/** Resume sound if paused */
	UFUNCTION(BlueprintCallable, Category = Controller)
	void Resume();

	/** Function called when the current frequency is updated */
	UFUNCTION(BlueprintImplementableEvent, Category = Event, DisplayName = OnUpdateFrequency)
	void BP_OnUpdateFrequency(float SubBass, float Bass, float LowMidrange, float Midrange, float UpperMidrange, float Presence, float Brilliance);

	/** Called only when the Audio finished */
	UFUNCTION(BlueprintImplementableEvent, Category = Event, DisplayName = OnAudioFinished)
	void BP_OnAudioFinished();

	/** Returning a number from 0 to 1 according to the "InFrequency" and the "MaxFrequency" of Sound. */
	UFUNCTION(BlueprintCallable, Category = Utility, DisplayName = Normalize)
	float BP_Normalize(const float& InFrequency, const float& MaxFrequency);
};