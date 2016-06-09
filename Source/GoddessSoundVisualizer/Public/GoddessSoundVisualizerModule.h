// Author AndreaCatania

#pragma once

#include "ModuleManager.h"

class FGoddessSoundVisualizerModule : public IModuleInterface {

public:
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
};