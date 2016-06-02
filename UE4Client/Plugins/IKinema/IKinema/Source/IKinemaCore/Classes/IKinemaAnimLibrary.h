#pragma once
#include "Animation/AnimNodeBase.h"
#include "IKinemaRig.h"
#include "IKinemaAnimLibrary.generated.h"
UENUM()
enum EAxisDirection
{
	Positixe_X,
	Positixe_Y,
	Positixe_Z,
	Negative_X,
	Negative_Y,
	Negative_Z,
};
UCLASS(MinimalAPI)
class UIKinemaAnimLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_UCLASS_BODY()
		
	
	// Make a rotator from a quaternion
	UFUNCTION(BlueprintPure, Category="IKinema")
	static void MakeRotatorFromQuaternion(const FQuat& InQuat, FRotator& OutRotator);

	//Do SLERP Interpolation. Interpolates from A to B based on Alpha, Out = A when Alpha is Zero, B when Alpha is 1
	UFUNCTION(BlueprintPure, Category="IKinema")
	static void Slerp(const FRotator& A, const FRotator& B, const float& alpha, FRotator& Out);

	// Expose FTransform::GetRelative to Kismet.
	UFUNCTION(BlueprintPure, Category="IKinema")
	static void GetRelativeTransform(const FTransform& A, const FTransform& B, FTransform& ATimesBInverse);

	// Expose FTransform::GetRelative to Kismet.
	UFUNCTION(BlueprintPure, Category="IKinema")
	static void GetRelativeTransformReverse(const FTransform& A, const FTransform& B, FTransform& AInverseTimesB);

	// Return Transform for given BoneIndex.
	UFUNCTION(BlueprintPure, Category="IKinema")
	static void GetBoneFromPose(const UAnimInstance* AnimBlueprint, FString CachedPose, FName BoneName, /*FName NotifierName, */bool ForceUpdate, FTransform& BoneTransform);

	// Return Transform for given BoneIndex.
	UFUNCTION(BlueprintPure, Category="IKinema")
	static void GetBoneFromPoseLink(const UAnimInstance* Vim, FPoseLink CachedPose, FName BoneName, bool ForceUpdate, float deltaX, FTransform& BoneTransform);

	UFUNCTION(BlueprintPure, Category="IKinema")
	static FIKinemaSolverTask SetupTask(const FTransform& Target, float Alpha, EIKinemaTargetType::Type TargetType, bool InComponentSpace, bool EnableTranslation = true, bool EnableOrientation = true, bool DebugDraw = false, float DebugDrawScale = 1.f);

	UFUNCTION(BlueprintPure, Category = "IKinema")
	static FIKinemaCOMTask SetupCOMTask(FVector Target, bool Enable, bool InComponentSpace, bool DebugDraw, float DebugDrawScale);

	UFUNCTION(BlueprintPure, Category = "IKinema")
	static FIKinemaSolverLookAtTask SetupLookAtTask(FVector LookAtTarget, FVector LookAtAxis, FVector PositionTarget, float Alpha, EIKinemaTargetType::Type TargetType, bool InComponentSpace, bool ReduceWeight, bool EnableTranslation, bool EnableOrientation, bool DebugDraw, float DebugDrawScale);

	UFUNCTION(BlueprintPure, Category = "IKinema")
	static FIKinemaSolverLookAtTask SetupLookAtTaskFromTask(const FIKinemaSolverTask& Task);

	UFUNCTION(BlueprintPure, Category="IKinema")
	static bool LineTrace(bool bTrace, const UAnimInstance* owner, AActor* IgnoreActor, const FVector& Start, const FVector& End, ECollisionChannel TraceChannel, bool bTraceComplex, bool bDrawDebug, FHitResult& OutHit);

	UFUNCTION(BlueprintPure, Category="IKinema")
	static void GetComponentToWorldSpaceTransform(const UAnimInstance* Vim, FTransform& BoneTransform);

	UFUNCTION(BlueprintCallable, Category="IKinema")
	static void DumpSolvers(const UAnimInstance* Vim, const FString& Folder);

	UFUNCTION(BlueprintPure, Category="IKinema")
	static FRotator FindRotationBetweenVectors(const FVector& From, const FVector& To);

	UFUNCTION(BlueprintPure, Category="IKinema")
	static FRotator  GenerateLookAtTarget (const FTransform& bone,const FVector& lookAtAxis, const FVector& lookAtDirection);

	UFUNCTION(BlueprintPure, Category="IKinema")
	static void FindActorWorldSpaceTransform(FName ActorName, FName TransName, FTransform& OutTransform, bool& Found);

	UFUNCTION(BlueprintPure, Category="IKinema")
	static void FindSkeletonWorldSpaceTransform(USkeletalMeshComponent* Skeleton, FName TransName, FTransform& OutTransform, bool& Found);

	UFUNCTION(BlueprintPure, Category="IKinema")
	static void FindRotatorFromUpAxisAndRefRot(const FVector& UpAxis, 
									  const FRotator& RefRotator, 
									  EAxisDirection RefSpaceRightDir, 
									  EAxisDirection OutSpaceUpDir, 
									  EAxisDirection OutSpaceRightDir, 
									  EAxisDirection OutSpaceFrontDir, 
									  bool LeftHanded,
									  FRotator& OutRotator);

	UFUNCTION(BlueprintPure, Category="IKinema")
	static void InverseTransform(const FTransform& InTransform, FTransform& OutTransform);

	UFUNCTION(BlueprintPure, Category="IKinema")
	static void MakeTaskProperties(bool PositionDofX,
								   bool PositionDofY,
								   bool PositionDofZ,
								   int32 PositionDepth,
								   const FVector& PositionWeight,
								   const FVector& PositionPrecision,
								   bool RotateDofX,
								   bool RotateDofY,
								   bool RotateDofZ,
								   int32 RotateDepth,
								   const FVector& RotateWeight,
								   const FVector& RotatePrecision,
								   FIKinemaTaskProperties& OutProperties);

	UFUNCTION(BlueprintPure, Category="IKinema")
	static void MakeSegmentProperties(bool DofX,
										bool DofY,
										bool DofZ,
										bool EnforceLimits,
										const FVector& Weight,
										bool EnableLimits,
										float LimitsGain,
										bool EnableRetargeting,
										float RetargetingGain,
										FIKinemaSegmentProperties& OutProperties);
};
