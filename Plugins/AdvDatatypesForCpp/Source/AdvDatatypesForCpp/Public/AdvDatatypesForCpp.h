// Intuitive Workflows | created by : Chopstig [Silvergull Studio] | Copyright 2025 | Unreal Engine 5.5
// Version 2025.1.0

#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"

class FAdvDatatypesForCppModule : public IModuleInterface
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
};
