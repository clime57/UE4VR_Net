//=============================================================================
// Factory for a definition of an IKinema solver.
// The factory creates the solver definition.
// The solver definition creates the solver.
//=============================================================================
#pragma once
#include "IKinemaRigFactory.generated.h"
UCLASS(hidecategories=Object)
class UIKinemaRigFactory : public UFactory, public FReimportHandler
{
	GENERATED_UCLASS_BODY()

	class USkeleton* TargetSkeleton;
	// Begin UFactory Interface
	virtual UObject* FactoryCreateText(UClass* InClass, UObject* InParent, FName InName, EObjectFlags Flags, UObject* Context, const TCHAR* Type, const TCHAR*& Buffer, const TCHAR* BufferEnd, FFeedbackContext* Warn) override;
	
	// End UFactory Interface

	// Begin FReimportHandler interface
	virtual bool CanReimport(UObject* Obj, TArray<FString>& OutFilenames) override;
	virtual void SetReimportPaths(UObject* Obj, const TArray<FString>& NewReimportPaths) override;
	virtual EReimportResult::Type Reimport(UObject* Obj) override;
	bool ConfigureProperties() override;
	virtual UObject* FactoryCreateNew(UClass* Class, UObject* InParent, FName Name, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn) override;
	// End FReimportHandler interface

private:

	// Read from a text buffer.
	bool ReadFromText(class UIKinemaRig* rig, const TCHAR* type, const TCHAR*& buffer, const TCHAR* bufferEnd, FFeedbackContext* warn);
};
