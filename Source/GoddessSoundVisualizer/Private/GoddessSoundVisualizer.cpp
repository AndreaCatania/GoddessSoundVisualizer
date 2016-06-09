// Author AndreaCatania

#include "GoddessSoundVisualizerPrivatePCH.h"
#include "GoddessSoundVisualizer.h"
#include "AudioDevice.h"
#include "Components/AudioComponent.h"
#include "SoundFrequencySpectrum.h"
#include "ActiveSound.h"
#include "Sound/SoundBase.h"

AGoddessSoundVisualizer::AGoddessSoundVisualizer()
	: ActiveSound(nullptr)
	, PlaybackTimePaused(-1)
	, FinishedArtificially(false)
{
	PrimaryActorTick.bCanEverTick = true;

	MainAudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("MainAudioComponent"));
	MainAudioComponent->OnAudioFinished.AddDynamic(this, &AGoddessSoundVisualizer::OnAudioFinished);

	RootComponent = MainAudioComponent;
}

void AGoddessSoundVisualizer::BeginPlay()
{
	Super::BeginPlay();
	PlaybackTimePaused = -1;
}

void AGoddessSoundVisualizer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (nullptr != ActiveSound && ActiveSound->IsAudioComponentValid()) {
		OnUpdateFrequency(SoundFrequencySpectrum->FindFrequencyBands(ActiveSound->RequestedStartTime + ActiveSound->PlaybackTime));
	}
}

void AGoddessSoundVisualizer::OnUpdateFrequency(FFrequencyBands FrequencyBands) {
	this->BP_OnUpdateFrequency(FrequencyBands.SubBass, FrequencyBands.Bass, FrequencyBands.LowMidrange, FrequencyBands.Midrange, FrequencyBands.UpperMidrange, FrequencyBands.Presence, FrequencyBands.Brilliance);
}

void AGoddessSoundVisualizer::OnAudioFinished() {
	if (!FinishedArtificially) {
		ActiveSound = nullptr;
		MainAudioComponent->Stop();
		OnUpdateFrequency(FFrequencyBands());
		BP_OnAudioFinished();
	}
}

void AGoddessSoundVisualizer::Play(USoundBase* Sound, USoundFrequencySpectrum* InSoundFrequencySpectrum) {
	FAudioDevice* AudioDevice = MainAudioComponent->GetAudioDevice();
	if (nullptr == AudioDevice) {
		LoggerWarning(TEXT("The AudioDevice is a nullptr"));
	} else {

		// Reset parameters
		FinishedArtificially = false;
		PlaybackTimePaused = -1;
		// Set sound
		SoundFrequencySpectrum = InSoundFrequencySpectrum;
		MainAudioComponent->SetSound(Sound);
		// Play Sound
		MainAudioComponent->Play(.0f);

		ActiveSound = AudioDevice->FindActiveSound(MainAudioComponent);
		if (nullptr == ActiveSound) {
			LoggerWarning(TEXT("Play function - ActiveSound is nullptr probably no sound is on"));
			// Block Tick execution
			ActiveSound = nullptr;
		} else
		if(nullptr == SoundFrequencySpectrum){
			LoggerWarning(TEXT("Play function - SoundFrequencySpectrum is nullptr"));
			// Block Tick execution
			ActiveSound = nullptr;
		}
	}
}

void AGoddessSoundVisualizer::Stop() {
	FinishedArtificially = true;
	PlaybackTimePaused = -1;
	ActiveSound = nullptr;
	MainAudioComponent->Stop();
	OnUpdateFrequency(FFrequencyBands());
}

void AGoddessSoundVisualizer::Pause() {
	if (nullptr != ActiveSound && ActiveSound->IsAudioComponentValid()) {
		FinishedArtificially = true;
		PlaybackTimePaused = ActiveSound->RequestedStartTime + ActiveSound->PlaybackTime;
		ActiveSound = nullptr;
		MainAudioComponent->Stop();
	} else {
		LoggerWarning(TEXT("Pause function - No sound to pause"));
	}
}

void AGoddessSoundVisualizer::Resume() {
	if (0 < PlaybackTimePaused) {
		FAudioDevice* AudioDevice = MainAudioComponent->GetAudioDevice();
		if (nullptr == AudioDevice) {
			LoggerWarning(TEXT("The AudioDevice is a nullptr"));
		} else {

			// Resume Sound
			MainAudioComponent->Play(PlaybackTimePaused);
			// Reset
			FinishedArtificially = false;
			PlaybackTimePaused = -1;

			ActiveSound = AudioDevice->FindActiveSound(MainAudioComponent);
			if (nullptr == ActiveSound) {
				LoggerWarning(TEXT("Resume function - ActiveSound is nullptr probably no sound is on"));
				// Block Tick execution
				ActiveSound = nullptr;
			} else
			if (nullptr == SoundFrequencySpectrum) {
				LoggerWarning(TEXT("Resume function - SoundFrequencySpectrum is nullptr"));
				// Block Tick execution
				ActiveSound = nullptr;
			}
		}
	} else {
		LoggerWarning(TEXT("Resume function - no sound to resume"));
	}
}

float AGoddessSoundVisualizer::BP_Normalize(const float& InFrequency, const float& MaxFrequency) {
	return USoundFrequencySpectrum::Normalize(InFrequency, MaxFrequency);
}
