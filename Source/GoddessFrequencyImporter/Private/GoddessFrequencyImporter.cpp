// Author AndreaCatania

#include "GoddessFrequencyImporterPrivatePCH.h"
#include "GoddessFrequencyImporter.h"

IMPLEMENT_MODULE(FGoddessFrequencyImporterModule, GoddessFrequencyImporter)

DEFINE_LOG_CATEGORY(GoddessFrequencyImporterLog);

void FGoddessFrequencyImporterModule::StartupModule()
{
	LoggerLog(TEXT("GoddessFrequencyImporterModule is Loaded"))
}

void FGoddessFrequencyImporterModule::ShutdownModule()
{
	
}
	
