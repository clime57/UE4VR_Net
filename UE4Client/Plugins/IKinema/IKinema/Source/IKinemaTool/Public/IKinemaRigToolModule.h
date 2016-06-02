// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.

#ifndef __IKinemaRigToolModule_h__
#define __IKinemaRigToolModule_h__

#include "UnrealEd.h"
#include "ModuleInterface.h"
#include "Toolkits/AssetEditorToolkit.h"
#include "Toolkits/IToolkit.h"
#include "IIKinemaRigTool.h"
#include "IKinemaRig.h"

extern const FName IKinemaRigToolAppIdentifier;

DECLARE_LOG_CATEGORY_EXTERN(LogIKinemaRigTool, Log, All);


/*-----------------------------------------------------------------------------
   IIKinemaRigToolModule
-----------------------------------------------------------------------------*/

class IIKinemaRigToolModule : public IModuleInterface,
	public IHasMenuExtensibility, public IHasToolBarExtensibility
{
public:

	virtual bool LicenseCheckPoint() { return true; };

	virtual bool isLicenseValid() { return true; };
	virtual bool DontAsk() { return false; };
	/** Creates a new IKinema RigTool instance */
	virtual TSharedRef<IIKinemaRigTool> CreateIKinemaRigTool(const EToolkitMode::Type Mode, const TSharedPtr< IToolkitHost >& InitToolkitHost, UIKinemaRig* IKinemaRig) = 0;

};

#endif // __IKinemaRigToolModule_h__
