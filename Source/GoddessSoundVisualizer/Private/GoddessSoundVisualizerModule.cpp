// Author AndreaCatania

#include "GoddessSoundVisualizerPrivatePCH.h"
#include "GoddessSoundVisualizerModule.h"

IMPLEMENT_MODULE(FGoddessSoundVisualizerModule, GoddessSoundVisualizer)

DEFINE_LOG_CATEGORY(GoddessSoundVisualizerLog);

void FGoddessSoundVisualizerModule::StartupModule() {
	LoggerLog(TEXT("GoddessSoundVisualizer Module is Loaded"))
}

void FGoddessSoundVisualizerModule::ShutdownModule() {

}