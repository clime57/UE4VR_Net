// Copyright 2008-2015 IKinema, ltd. All Rights Reserved.

//=============================================================================
// IKinemaRigExporter
//=============================================================================

#pragma once
#include "IKinemaRigExporter.generated.h"

UCLASS()
class UIKinemaRigExporter : public UExporter
{
	GENERATED_UCLASS_BODY()


	// Begin UExporter Interface
	virtual bool ExportText(const FExportObjectInnerContext* Context, UObject* Object, const TCHAR* Type, FOutputDevice& Ar, FFeedbackContext* Warn, uint32 PortFlags = 0) override;
	// End UExporter Interface
};
