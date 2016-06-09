// Author AndreaCatania

#pragma once

#include "Engine.h"
#include "GoddessSoundVisualizerModule.h"

DECLARE_LOG_CATEGORY_EXTERN(GoddessSoundVisualizerLog, Log, All);

#define LoggerLog(TextMsg) UE_LOG(GoddessSoundVisualizerLog, Log, TextMsg)
#define LoggerWarning(TextMsg) UE_LOG(GoddessSoundVisualizerLog, Warning, TextMsg)
#define LoggerError(TextMsg) UE_LOG(GoddessSoundVisualizerLog, Error, TextMsg)