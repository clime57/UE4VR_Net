#pragma once

#include "ModuleInterface.h"

DECLARE_LOG_CATEGORY_EXTERN(LogIKinemaEditor, Warning, All);

class FIKinemaEditorModule : public IModuleInterface
{
public:
	FIKinemaEditorModule();
	
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

	bool LicenseCheckPoint();
	bool isLicenseValid();
	virtual TSharedPtr<FExtensibilityManager> GetIKinemaEditorMenuExtensibilityManager() { return IKinemaEditor_MenuExtensibilityManager; }
	virtual TSharedPtr<FExtensibilityManager> GetIKinemaEditorToolBarExtensibilityManager() { return IKinemaEditor_ToolBarExtensibilityManager; }

private:
	// Asset type actions for IKinema assets.  Cached here so that we can unregister it during shutdown.
	TSharedPtr< class FAssetTypeActions_IKinemaRig > mIKinemaRigAssetTypeActions;
	//TSharedPtr< class FAssetTypeActions_IKinemaTaskSkeleton > mIKinemaTaskSkeletonAssetTypeActions;
	//TSharedPtr< class FAssetTypeActions_IKinemaTaskAnim > mIKinemaTaskAnimAssetTypeActions;

	TSharedPtr<FExtensibilityManager> IKinemaEditor_MenuExtensibilityManager;
	TSharedPtr<FExtensibilityManager> IKinemaEditor_ToolBarExtensibilityManager;

	bool isLicensed;

};
