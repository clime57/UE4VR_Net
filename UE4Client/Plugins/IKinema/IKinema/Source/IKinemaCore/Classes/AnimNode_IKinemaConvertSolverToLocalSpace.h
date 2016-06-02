#pragma once
#include "Animation/AnimNodeBase.h"
#include "AnimNode_IKinemaConvertSolverToLocalSpace.generated.h"

USTRUCT()
struct IKINEMACORE_API FAnimNode_IKinemaConvertSolverToLocalSpace : public FAnimNode_Base
{
	GENERATED_USTRUCT_BODY()

	// The input pose is segmented into two:
	// - The FK input pose that serves as a bias for the solver.
	// - The task targets appended at the end.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Links)
	FPoseLink InPose;

	// This is set by the anim graph node, so don't allow further edits.
	UPROPERTY()
	class UIKinemaRig* IKinemaRig;
/*
	UPROPERTY(transient)
	int32 LastFrameUpdated;

	UPROPERTY(transient)
	int32 LastFrameEvaluated;
*/
protected:
	// Keep the binding here.
	FIKinemaRigToSkeletonBinding mSkeletonBinding;

	// Ensures the specified object is preloaded.  ReferencedObject can be NULL.
	void PreloadObject(UObject* ReferencedObject)
	{
		if ((ReferencedObject != NULL) && ReferencedObject->HasAnyFlags(RF_NeedLoad))
		{
			ReferencedObject->GetLinker()->Preload(ReferencedObject);
		}
	}
public:	

	FAnimNode_IKinemaConvertSolverToLocalSpace();
	~FAnimNode_IKinemaConvertSolverToLocalSpace();

	// Set the rig on this.
	virtual void SetAsset(class UIKinemaRig* rig);

	// FAnimNode_Base interface
	virtual void Initialize(const FAnimationInitializeContext& Context) override;
	virtual void Update(const FAnimationUpdateContext& Context) override;
	virtual void Evaluate(FPoseContext& Output) override;
	virtual void CacheBones(const FAnimationCacheBonesContext & Context) override;
	// End of FAnimNode_Base interface
};
