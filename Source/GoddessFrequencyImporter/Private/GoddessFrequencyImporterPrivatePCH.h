// Author AndreaCatania

#pragma once

#include "Engine.h"
#include "GoddessFrequencyImporter.h"

DECLARE_LOG_CATEGORY_EXTERN(GoddessFrequencyImporterLog, Log, All);

#define LoggerLog(TextMsg) UE_LOG(GoddessFrequencyImporterLog, Log, TextMsg)
#define LoggerWarning(TextMsg) UE_LOG(GoddessFrequencyImporterLog, Warning, TextMsg)
#define LoggerError(TextMsg) UE_LOG(GoddessFrequencyImporterLog, Error, TextMsg)