// Author AndreaCatania

#include "GoddessFrequencyImporterPrivatePCH.h"
#include "UnrealEd.h"
#include "SoundFrequencySpectrumFactory.h"
#include "SoundFrequencySpectrum.h"

// KISS Headers
#include "ThirdParty/Kiss_FFT/kiss_fft129/kiss_fft.h"
#include "ThirdParty/Kiss_FFT/kiss_fft129/tools/kiss_fftnd.h"

// Approximate the sample to 25 Milliseconds (40 frame per second)
#define SAMPLEAPPROXIMATION .025f

USoundFrequencySpectrumFactory::USoundFrequencySpectrumFactory(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	// Indicate that this factory should be considered by "Import" UI.
	bEditorImport = true;
	// Register that this factory is interested in .wavfs files.
	new(Formats) FString(TEXT("wavfs;Renamed wav file to allow importing of frequency spectrum"));
	// Set to false so it's use "FactoryCreateBinary" function to generate asset
	bCreateNew = false;
	bEditAfterNew = false;
	SupportedClass = USoundFrequencySpectrum::StaticClass();
}

UObject* USoundFrequencySpectrumFactory::FactoryCreateBinary
(
	UClass*			Class
	, UObject*			InParent
	, FName				Name
	, EObjectFlags		Flags
	, UObject*			Context
	, const TCHAR*		FileType
	, const uint8*&		Buffer
	, const uint8*		BufferEnd
	, FFeedbackContext*	Warn
)
{

	// Copy the data from the buffer to an array
	const uint32 BufferSize = BufferEnd - Buffer;
	uint8* RawWaveData = new uint8[BufferSize];
	FMemory::Memcpy(RawWaveData, Buffer, BufferSize);

	// Read Wave info and make sure we have valid wave data
	FWaveModInfo WaveInfo;
	FString ErrorMessage;

	if (WaveInfo.ReadWaveInfo(RawWaveData, BufferSize, &ErrorMessage)) {

		if (16 != *WaveInfo.pBitsPerSample) {
			WaveInfo.ReportImportFailure();
			Warn->Logf(ELogVerbosity::Error, TEXT("Currently only the 16 bit wav file is supported - '%s'"), *Name.ToString());
			delete[] RawWaveData;
			return nullptr;
		}

		if (1 != *WaveInfo.pChannels && 2 != *WaveInfo.pChannels) {
			WaveInfo.ReportImportFailure();
			Warn->Logf(ELogVerbosity::Error, TEXT("Currently only mono or stereo wav file are supported - '%s'"), *Name.ToString());
			delete[] RawWaveData;
			return nullptr;
		}

	} else {
		Warn->Logf(ELogVerbosity::Error, TEXT("Unable tu read the file '%s' - Please ensure that the file is a correct wav file with extension file.wavfs - \"%s\""), *Name.ToString(), *ErrorMessage);
		delete[] RawWaveData;
		return nullptr;
	}

	// Calculate Sound Duration
	float SoundDuration;
	int32 DurationDiv = *WaveInfo.pChannels * *WaveInfo.pBitsPerSample * *WaveInfo.pSamplesPerSec;
	if (DurationDiv) {
		SoundDuration = *WaveInfo.pWaveDataSize * 8.0f / DurationDiv;
	} else {
		SoundDuration = .0f;
	}

	// PCM data is 2 byte for 16bit audio for this reason is doubled
	const uint32 PCMBufferSize = 2*(SoundDuration * (*WaveInfo.pSamplesPerSec) * (*WaveInfo.pChannels));
	
	// Read PCM from WAV
	uint8* PCMBuffer = new uint8[PCMBufferSize];
	FMemory::Memcpy(PCMBuffer, Buffer+44, PCMBufferSize);

	USoundFrequencySpectrum* SoundFrequencySpectrum = NewObject<USoundFrequencySpectrum>(InParent, Class, Name, Flags);

	FFrequencyBands BiggestFrequencyBands;

	// Calculates the Frequencies for the entire Sound
	TArray<float> FrequencySupp;
	for (float MillisecCounter = 0; SoundDuration > MillisecCounter; MillisecCounter += SAMPLEAPPROXIMATION) {
		FFrequencyBands FrequencyBands;

		// Calculate the frequencies for the time between MillisecCounter and MillisecCounter+SAMPLEAPPROXIMATION
		CalculateFrequencySpectrum(PCMBuffer, PCMBufferSize, WaveInfo, MillisecCounter, SAMPLEAPPROXIMATION, FrequencySupp);

		// Sub-bass
		GetAverageFrequencyValueInRange(*WaveInfo.pSamplesPerSec, FrequencySupp, 20, 60, FrequencyBands.SubBass);

		// Bass
		GetAverageFrequencyValueInRange(*WaveInfo.pSamplesPerSec, FrequencySupp, 60, 250, FrequencyBands.Bass);

		// Low Midrange
		GetAverageFrequencyValueInRange(*WaveInfo.pSamplesPerSec, FrequencySupp, 250, 500, FrequencyBands.LowMidrange);

		// Midrange
		GetAverageFrequencyValueInRange(*WaveInfo.pSamplesPerSec, FrequencySupp, 500, 2000, FrequencyBands.Midrange);

		// Upper Midrange
		GetAverageFrequencyValueInRange(*WaveInfo.pSamplesPerSec, FrequencySupp, 2000, 4000, FrequencyBands.UpperMidrange);

		// Presence
		GetAverageFrequencyValueInRange(*WaveInfo.pSamplesPerSec, FrequencySupp, 4000, 6000, FrequencyBands.Presence);

		// Brilliance
		GetAverageFrequencyValueInRange(*WaveInfo.pSamplesPerSec, FrequencySupp, 6000, 20000, FrequencyBands.Brilliance);

		// Check if the current SubBass frequency is the biggest
		if (BiggestFrequencyBands.SubBass < FrequencyBands.SubBass) {
			BiggestFrequencyBands.SubBass = FrequencyBands.SubBass;
		}
		// Check if the current Bass frequency is the biggest
		if (BiggestFrequencyBands.Bass < FrequencyBands.Bass) {
			BiggestFrequencyBands.Bass = FrequencyBands.Bass;
		}
		// Check if the current LowMidrange frequency is the biggest
		if (BiggestFrequencyBands.LowMidrange < FrequencyBands.LowMidrange) {
			BiggestFrequencyBands.LowMidrange = FrequencyBands.LowMidrange;
		}
		// Check if the current Midrange frequency is the biggest
		if (BiggestFrequencyBands.Midrange < FrequencyBands.Midrange) {
			BiggestFrequencyBands.Midrange = FrequencyBands.Midrange;
		}
		// Check if the current UpperMidrange frequency is the biggest
		if (BiggestFrequencyBands.UpperMidrange < FrequencyBands.UpperMidrange) {
			BiggestFrequencyBands.UpperMidrange = FrequencyBands.UpperMidrange;
		}
		// Check if the current Presence frequency is the biggest
		if (BiggestFrequencyBands.Presence < FrequencyBands.Presence) {
			BiggestFrequencyBands.Presence = FrequencyBands.Presence;
		}
		// Check if the current Brilliance frequency is the biggest
		if (BiggestFrequencyBands.Brilliance < FrequencyBands.Brilliance) {
			BiggestFrequencyBands.Brilliance = FrequencyBands.Brilliance;
		}

		SoundFrequencySpectrum->SoundFrequencies.Add(FrequencyBands);
	}

	SoundFrequencySpectrum->BiggestFrequencyBands = BiggestFrequencyBands;

	delete[] PCMBuffer;
	delete[] RawWaveData;
	return SoundFrequencySpectrum;
}

void USoundFrequencySpectrumFactory::CalculateFrequencySpectrum(uint8* PCMDataBuffer, const uint32 PCMBufferSize, const FWaveModInfo& WaveInfo, const float InStartTime, const float InDuration, TArray<float>& OutFrequencies)
{
	// Clear the Array before continuing
	OutFrequencies.Empty();

	const int32 NumChannels = *WaveInfo.pChannels;
	const int32 SampleRate = *WaveInfo.pSamplesPerSec;

	// Make sure the Number of Channels is correct
	if (NumChannels > 0 && NumChannels <= 2)
	{

		// The first sample is just the StartTime * SampleRate
		int32 FirstSample = SampleRate * InStartTime;

		// The last sample is the SampleRate times (StartTime plus the Duration)
		int32 LastSample = SampleRate * (InStartTime + InDuration);

		// Get Maximum amount of samples in this Sound
		const int32 SampleCount = WaveInfo.SampleDataSize / (2 * NumChannels);

		// An early check if we can create a Sample window
		FirstSample = FMath::Min(SampleCount, FirstSample);
		LastSample = FMath::Min(SampleCount, LastSample);

		// Actual amount of samples we gonna read
		int32 SamplesToRead = LastSample - FirstSample;

		if (SamplesToRead < 0) {

			LoggerError(TEXT("Number of SamplesToRead is < 0!"))
				return;
		}

		// Shift the window enough so that we get a PowerOfTwo. FFT works better with that
		int32 PoT = 2;

		while (SamplesToRead > PoT) {
			PoT *= 2;
		}

		// Now we have a good PowerOfTwo to work with
		SamplesToRead = PoT;

		// Create two 2-dim Arrays for complex numbers | Buffer and Output
		kiss_fft_cpx* Buffer[2] = { 0 };
		kiss_fft_cpx* Output[2] = { 0 };

		// Create 1-dim Array with one slot for SamplesToRead
		int32 Dims[1] = { SamplesToRead };

		kiss_fftnd_cfg STF = kiss_fftnd_alloc(Dims, 1, 0, nullptr, nullptr);

		int16* SamplePtr = reinterpret_cast<int16*>(PCMDataBuffer);

		// Allocate space in the Buffer and Output Arrays for all the data that FFT returns
		for (int32 ChannelIndex = 0; ChannelIndex < NumChannels; ChannelIndex++)
		{
			Buffer[ChannelIndex] = (kiss_fft_cpx*)KISS_FFT_MALLOC(sizeof(kiss_fft_cpx) * SamplesToRead);
			Output[ChannelIndex] = (kiss_fft_cpx*)KISS_FFT_MALLOC(sizeof(kiss_fft_cpx) * SamplesToRead);
		}

		// Shift our SamplePointer to the Current "FirstSample"
		SamplePtr += FirstSample * NumChannels;

		for (int32 SampleIndex = 0; SampleIndex < SamplesToRead; SampleIndex++)
		{
			for (int32 ChannelIndex = 0; ChannelIndex < NumChannels; ChannelIndex++)
			{
				// Make sure the Point is Valid and we don't go out of bounds
				if (SamplePtr != NULL && (SampleIndex + FirstSample < SampleCount))
				{
					// Use Window function to get a better result for the Data (Hann Window)
					Buffer[ChannelIndex][SampleIndex].r = GetFFTInValue(*SamplePtr, SampleIndex, SamplesToRead);
					Buffer[ChannelIndex][SampleIndex].i = 0.f;
				} else
				{
					// Use Window function to get a better result for the Data (Hann Window)
					Buffer[ChannelIndex][SampleIndex].r = 0.f;
					Buffer[ChannelIndex][SampleIndex].i = 0.f;
				}

				// Take the next Sample
				SamplePtr++;
			}
		}

		// Now that the Buffer is filled, use the FFT
		for (int32 ChannelIndex = 0; ChannelIndex < NumChannels; ChannelIndex++)
		{
			if (Buffer[ChannelIndex])
			{
				kiss_fftnd(STF, Buffer[ChannelIndex], Output[ChannelIndex]);
			}
		}

		OutFrequencies.AddZeroed(SamplesToRead);

		for (int32 SampleIndex = 0; SampleIndex < SamplesToRead; ++SampleIndex)
		{
			float ChannelSum = 0.0f;

			for (int32 ChannelIndex = 0; ChannelIndex < NumChannels; ++ChannelIndex)
			{
				if (Output[ChannelIndex])
				{
					// With this we get the actual Frequency value for the frequencies from 0hz to ~22000hz
					ChannelSum += FMath::Sqrt(FMath::Square(Output[ChannelIndex][SampleIndex].r) + FMath::Square(Output[ChannelIndex][SampleIndex].i));
				}
			}

			OutFrequencies[SampleIndex] = ChannelSum / NumChannels;
		}

		// Make sure to free up the FFT stuff
		KISS_FFT_FREE(STF);

		for (int32 ChannelIndex = 0; ChannelIndex < NumChannels; ++ChannelIndex)
		{
			KISS_FFT_FREE(Buffer[ChannelIndex]);
			KISS_FFT_FREE(Output[ChannelIndex]);
		}

	} else {
		LoggerError(TEXT("Number of Channels is < 0!"));
	}
}

float USoundFrequencySpectrumFactory::GetFFTInValue(const int16 InSampleValue, const int16 InSampleIndex, const int16 InSampleCount)
{
	float FFTValue = InSampleValue;

	// Apply the Hann window
	FFTValue *= 0.5f * (1 - FMath::Cos(2 * PI * InSampleIndex / (InSampleCount - 1)));

	return FFTValue;
}

void USoundFrequencySpectrumFactory::GetAverageFrequencyValueInRange(const uint32 SampleRate, TArray<float> InFrequencies, int32 InStartFrequence, int32 InEndFrequence, float& OutAverageFrequency)
{
	// Init the Return Value
	OutAverageFrequency = 0.0f;

	if (InStartFrequence >= InEndFrequence || InStartFrequence < 0 || InEndFrequence > 22000)
		return;

	int32 FStart = (int32)(InStartFrequence  * InFrequencies.Num() * 2 / SampleRate);
	int32 FEnd = (int32)(InEndFrequence * InFrequencies.Num() * 2 / SampleRate);

	if (FStart < 0 || FEnd >= InFrequencies.Num())
		return;

	int32 NumberOfFrequencies = 0;

	float ValueSum = 0.0f;

	for (int i = FStart; i <= FEnd; i++)
	{
		NumberOfFrequencies++;

		ValueSum += InFrequencies[i];
	}

	OutAverageFrequency = ValueSum / NumberOfFrequencies;
}