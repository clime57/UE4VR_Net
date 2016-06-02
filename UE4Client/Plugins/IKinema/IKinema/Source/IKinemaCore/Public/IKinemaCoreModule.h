#pragma once

#include "ModuleInterface.h"

class FIKinemaCoreModule : public IModuleInterface
{
public:
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
};

FGraphEventRef ExecOnGameThread(TFunction<void()> funcLambda);