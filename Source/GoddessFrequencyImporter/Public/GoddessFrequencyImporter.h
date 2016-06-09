// Author AndreaCatania

#pragma once

#include "ModuleManager.h"

class FGoddessFrequencyImporterModule : public IModuleInterface
{
public:

	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
};