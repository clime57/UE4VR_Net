#pragma once
#include "Animation/AnimNodeBase.h"
#include "AnimNode_IKinemaSolver.h"
#include "AnimNode_IKinemaFootPlacement.generated.h"

UENUM()
namespace ELimitAxisOption
{
	enum Type
	{
		X,
		Y,
		Z
	};
}

UENUM()
namespace EAimingOptions
{
	enum Type
	{
		SingleHand UMETA(DisplayName = "Single Hand Wield", ToolTip = "Holding a single weapon (e.g. Pistol)"), 
		TwoHand  UMETA(DisplayName = "Two Hand Wield", ToolTip = "Holding a single weapon with two hands(e.g. Rifle, Bow)"),
		DualWield UMETA(DisplayName = "Dual Wield", ToolTip = "Holding a two weapon (e.g. Pistol)")
	};
}

USTRUCT()
struct IKINEMACORE_API FAnimNode_IKinemaFootPlacement : public FAnimNode_IKinemaSolver
{
	GENERATED_USTRUCT_BODY()

		/** Enum indicating the collision channel to use when casting rays */
		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Collision)
		TEnumAsByte<enum ECollisionChannel> CollisionChannel;

		/** If EffectorTransformSpace is a bone, this is the bone to use. **/
		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = EndEffector)
		TArray<FBoneReference> LimbTransforms;

		/** The name of the bone attached to the weapon handle. **/
		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = EndEffector)
		FBoneReference HandleBone;

		/** The name of the bone attached to the weapon trigger. **/
		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = EndEffector)
		FBoneReference TriggerBone;

		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = EndEffector)
		FBoneReference HipsTransform;

		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Offset , meta = (PinShownByDefault))
		TArray<float> MeshOffset;
		
		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Offset, meta = (PinShownByDefault))
		TArray<float> Alpha;

		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Offset, meta = (PinShownByDefault))
		float MaxHipMultiplier;

		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Offset, meta = (PinShownByDefault))
		float MinHipMultiplier;

		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Collision, meta = (ToolTip = "Scale the length of the line trace. By default, the length is equal to the Capsule Half Height"))
		float LineTraceScale;

		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Offset, meta = (PinShownByDefault, ToolTip = "Upperbody offset in Component Space"))
		FVector UpperBodyOffset;

		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = LookAt, meta = (PinHiddenByDefault))
		FVector lookAtDirection;

		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = LookAt, meta = (PinShownByDefault))
		FVector HandleOffset;

		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = LookAt, meta = (PinShownByDefault))
		bool AimWithWeapon;
		
		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = LookAt, meta = (PinShownByDefault))
		TEnumAsByte<enum EAimingOptions::Type> AimingType;

		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = LookAt, meta = (PinShownByDefault))
		bool LockFreeArm;

		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Offset, meta = (PinShownByDefault))
		bool bReverseFootSetup;

		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Offset, meta = (PinShownByDefault))
		float ReverseFootFactor;

		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Offset)
		float RollLimit;

		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Offset)
		float PitchLimit;

		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Smoothness, meta = (DisplayName = "Position Blend Speed", ToolTip = "Speed of change in task position when smoothing"))
		float PTransitionSpeed;

		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Smoothness, meta = (DisplayName = "Rotation Blend Speed", ToolTip = "Speed of change in task rotation when smoothing"))
		float RTransitionSpeed;
		
		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Smoothness, meta = (DisplayName = "Maximum step change", ToolTip = "If change in hit location greater than this value, do not smooth the task demands"))
		float PTransitionMaxDistance;
	
		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = LookAt, meta = (PinShownByDefault))
		float LookAtAlpha;
		
		/** Flag to show and use the generic solver task inputs along with the foot placement logic */
		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Offset)
		bool ExposeSolverTasks;

		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = LookAt)
		bool InComponentSpace;

		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = LookAt)
		bool ReduceWeight;

		/** Whether or not to use Up axis */
		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = LookAt)
		bool UseLookUpAxis;

		/** Up axis in local space */
		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = LookAt)
		TEnumAsByte<ELimitAxisOption::Type>	LookUpAxis;
		/** Look at limit value in degree - limit the rotation around the look at axis*/
		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = LookAt)
		FRotator lookAtLimit;

		/** Look at Clamp value in degree - if you're look at axis is Z, only X, Y degree of clamp will be used*/
		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = LookAt, meta = (PinHiddenByDefault))
		float LookAtClamp;
	
		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Offset, meta = (PinShownByDefault))
		bool bFloorContact;

		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Debug)
		bool DrawDebug;


public:

	// FAnimNode_Base interface
	virtual void Update(const FAnimationUpdateContext& Context) override;
	virtual void Evaluate(FPoseContext& Output) override;
	// End of FAnimNode_Base interface

protected:
	//Hold task current and previous task targets
	TArray<FTransform> TaskTargets;
	int TriggerHandTaskIndex;
	int HandleHandTaskIndex;
private:
	TArray<FVector> PrevNormal;
	TArray<FVector> PrevTarget;
	float interp;

protected:
	bool DoRayTrace(const USkeletalMeshComponent* mesh, FTransform& transform, const FTransform& SolverToWorld, const int index);
	void ReverseFootSetup(const FCompactPose& pose, USkeletalMeshComponent const* mesh, FA2CSPose& ComponentSpacePose, const FTransform& SolverToWorld);
	void PassTargetsToTasks(FA2CSPose& ComponentSpacePose, FTransform& SolverToWorld, FTransform& ComponentToWorld, FVector& avr);
	FQuat GenerateLookAtTarget(FVector targetLocation, const FTransform& animTarget, const int taskIndex, const FTransform& SolverToWorld);

	// Copied from FAnimNode_SkeletalControlBase interface
	bool IsValidToEvaluate(const USkeleton * Skeleton, const FBoneContainer & RequiredBones);
	void InitializeBoneReferences(const FBoneContainer & RequiredBones);
	// End of FAnimNode_SkeletalControlBase interface

	
public:
	FAnimNode_IKinemaFootPlacement();
	~FAnimNode_IKinemaFootPlacement();

	// Set the rig on this.
	virtual void SetAsset(class UIKinemaRig* rig);
	// FAnimNode_Base interface
	virtual void Initialize(const FAnimationInitializeContext& Context) override;
	virtual void CacheBones(const FAnimationCacheBonesContext & Context) override;
	// End of FAnimNode_Base interface
};
