#pragma once
#include "Animation/AnimNodeBase.h"
#include "AnimNode_SetBone.generated.h"

USTRUCT()
struct IKINEMACORE_API FAnimNode_SetBone : public FAnimNode_Base
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Links)
	FPoseLink InPose;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Links, meta=(AlwaysAsPin))
	FName BoneName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Links, meta=(AlwaysAsPin))
	FTransform BoneTransform;

	UPROPERTY()
	int32 BoneIndex;
public:	

	FAnimNode_SetBone();
	~FAnimNode_SetBone();

	// FAnimNode_Base interface
	virtual void Initialize(const FAnimationInitializeContext& Context) override;
	virtual void Update(const FAnimationUpdateContext& Context) override;
	virtual void Evaluate(FPoseContext& Output) override;
	virtual void CacheBones(const FAnimationCacheBonesContext & Context) override;
	// End of FAnimNode_Base interface
};
