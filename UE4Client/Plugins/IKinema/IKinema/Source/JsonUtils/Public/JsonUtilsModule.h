// Copyright (C) Microsoft. All rights reserved.

#pragma once

#include "ModuleInterface.h"

class FJsonUtilsModule : public IModuleInterface
{
public:
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
};
