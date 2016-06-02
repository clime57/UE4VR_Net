#pragma once
#include "Animation/AnimNodeBase.h"
#include "IKSolver.h"
#include "IKTask.h"

#include "IKinemaRig.generated.h"


UENUM()
namespace EIKinemaBoneAxisType
{
	
	enum Type
	{
		EIKBAT_X,
		EIKBAT_Y,
		EIKBAT_Z,
	};
}

/** Enum for controlling which reference frame a controller is applied in. */
UENUM()
namespace EIKinemaTargetType
{

	enum Type
	{
		BlendToTarget UMETA(DisplayName = "Blend to target", ToolTip = "Blend away from original animation towards the target"),
		BlendWithTarget UMETA(DisplayName = "Blend animation with target", ToolTip = "Blend towards the target, while preserving subtle animation details"),
		BlendWithOffset UMETA(DisplayName = "Target is offset to animation", ToolTip = "Target is offset added to animation")
	};
}

USTRUCT()
struct FIKinemaEulerLimits
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, Category=Advanced)
	FVector MinDegrees;

	UPROPERTY(EditAnywhere, Category=Advanced)
	FVector MaxDegrees;

	UPROPERTY()
	TEnumAsByte<EIKinemaBoneAxisType::Type> BoneAxis;

	// Set to default values..
	void InitializeValues();
};

USTRUCT()
struct IKINEMACORE_API FIKinemaBoneDef
{
	GENERATED_USTRUCT_BODY()

	~FIKinemaBoneDef();

	UPROPERTY(VisibleAnywhere, Category=IKDefinition)
	FName	Name;

	UPROPERTY(VisibleAnywhere, Category=IKDefinition)
	FName	ParentName;

	UPROPERTY(EditAnywhere, Category=IKDefinition)
	bool	DofX;

	UPROPERTY(EditAnywhere, Category=IKDefinition)
	bool	DofY;

	UPROPERTY(EditAnywhere, Category=IKDefinition)
	bool	DofZ;

	// Index of the task that drives this bone or INDEX_NONE (reverse lookup of FIKinemaTaskDef::BoneIndex)
	UPROPERTY()
	int32	TaskIndex;

	// Index of the parent bone or INDEX_NONE (derived from ParentName)
	UPROPERTY()
	int32	ParentIndex;

	UPROPERTY(EditAnywhere, AdvancedDisplay, Category = Advanced)
	FIKinemaEulerLimits Limits;

	UPROPERTY(EditAnywhere, AdvancedDisplay, Category = Advanced)
	bool	EnforceLimits;

	// One weight value for each bone axis.
	UPROPERTY(EditAnywhere, Category = IKDefinition)
	FVector	Weight;

	// The mass of the bone.
	UPROPERTY(EditAnywhere, AdvancedDisplay, Category = Advanced)
	float	Mass;

	UPROPERTY(EditAnywhere, AdvancedDisplay,Category = Advanced)
	bool	ResetToRest;

	UPROPERTY(EditAnywhere, AdvancedDisplay, Category = Advanced)
	bool	EnableLimits;

	UPROPERTY(EditAnywhere, AdvancedDisplay,Category = Advanced)
	float	MaxVelocity;

	UPROPERTY(EditAnywhere, AdvancedDisplay, Category = Advanced)
	float	LimitsGain;

	UPROPERTY(EditAnywhere, Category=IKDefinition)
	bool	Active;

	UPROPERTY(EditAnywhere, Category=IKDefinition)
	bool	EnableRetargeting;

	UPROPERTY(EditAnywhere, Category=IKDefinition)
	float	RetargetingGain;

	UPROPERTY(EditAnywhere, Category = IKDefinition)
	FVector	RetargetingDoFGain;

	UPROPERTY()
	FTransform	RestPose;

	//Constructor
	FIKinemaBoneDef();

	// Set to default values..
	void InitializeValues();

	void PostLoadOrImport();

#if WITH_EDITOR
	void CopyBoneDefPropertiesFrom(FIKinemaBoneDef const* other);

	void CopyBoneDefPropertiesFrom(FIKinemaBoneDef const& other);

	void ResetToDefaults();
#endif

	// Bind to the given skeleton
	bool BindToSkeleton(FIK::ImportBone& importBone, USkeleton& skeleton, int32& ue4BoneIndex, bool upY, USkeletalMeshComponent* component);

	// Configure an IKinema bone segment (inside the solver instance).
	void ConfigureIKSegment(FIK::IKSegment& segment) const;

	// Transfer an IKinema pose to a UE4 pose, for this bone.
	void IKinemaToUE4Pose(const FIK::IKSegment& segment, FTransform& ue4Transform, bool applyTranslation) const;

private:

	// Cache ANSI names from the wide versions.
	void CacheANSINames();

	// Cache the ANSI version of the name and parent name.
	static const int32 MAX_NAME_LENGTH = 512;
	ANSICHAR mNameANSI[MAX_NAME_LENGTH];
	ANSICHAR mParentNameANSI[MAX_NAME_LENGTH];
};

USTRUCT()
struct IKINEMACORE_API FIKinemaTaskDef
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(VisibleAnywhere, Category=IKDefinition)
	FName	Name;

	UPROPERTY(EditAnywhere, Category=IKDefinition)
	bool	HasPositionTask;

	UPROPERTY(EditAnywhere, Category = IKDefinition)
	bool	HasRotationTask;

	UPROPERTY()
	bool	RootAsScalePivot;

	// Index of the FIKinemaBoneDef that is being driven by this task
	UPROPERTY()
	int32	BoneIndex;

	// Index of the parent task or INDEX_NONE (derived by walking up the bone hierarchy to find the first parent that has a corresponding task)
	UPROPERTY()
	int32	ParentIndex;

	//position

	UPROPERTY(EditAnywhere, Category=PositionTask)
	bool PositionAsPoleObject;

	UPROPERTY(EditAnywhere, Category = PositionTask)
	bool PositionDofX;

	UPROPERTY(EditAnywhere, Category = PositionTask)
	bool PositionDofY;

	UPROPERTY(EditAnywhere, Category = PositionTask)
	bool PositionDofZ;

	UPROPERTY(EditAnywhere, Category = PositionTask)
	uint32 PositionDepth;

	UPROPERTY(EditAnywhere, Category = PositionTask)
	FVector PositionWeight;

	UPROPERTY(EditAnywhere, AdvancedDisplay, Category = PositionTask)
	FVector PositionPrecision;

	UPROPERTY(EditAnywhere, AdvancedDisplay, Category = PositionTask)
	uint32 PositionPriority;

	UPROPERTY()
	FVector ScaleRetargeting;

	UPROPERTY()
	FVector OffsetRetargeting;

	//orientation

	UPROPERTY(EditAnywhere, Category = OrientationTask)
	bool RotateDofX;

	UPROPERTY(EditAnywhere, Category = OrientationTask)
	bool RotateDofY;

	UPROPERTY(EditAnywhere, Category = OrientationTask)
	bool RotateDofZ;

	UPROPERTY(EditAnywhere, Category = OrientationTask)
	uint32 RotateDepth;

	UPROPERTY(EditAnywhere, Category = OrientationTask)
	FVector RotateWeight;

	UPROPERTY(EditAnywhere, AdvancedDisplay, Category = OrientationTask)
	FVector RotatePrecision;

	UPROPERTY(EditAnywhere, AdvancedDisplay, Category = OrientationTask)
	uint32 RotatePriority;

	UPROPERTY(EditAnywhere, Category = OrientationTask)
	bool LookAt;

	UPROPERTY()
	bool IsChest;

	UPROPERTY(EditAnywhere, Category = OrientationTask, meta = (DisplayName = "Look-At Axis"))
	FVector TipOffset;
	// Set to default values..
	void InitializeValues();

#if WITH_EDITOR
	void CopyTaskDefPropertiesFrom(FIKinemaTaskDef const* other);
	
	void CopyTaskDefPropertiesFrom(FIKinemaTaskDef const& other);

	void ResetToDefaults();
#endif

	void PostLoadOrImport();

	// Configure a position task using our properties.
	void ConfigurePositionTask(FIK::IKPositionTask& positionTask) const;

	// Configure an orientation task using our properties.
	void ConfigureOrientationTask(FIK::IKOrientationTask& orientationTask) const;
};

USTRUCT()
struct FIKinemaForceDef
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(VisibleAnywhere, Category=IKDefinition)
	FName		Name;

	UPROPERTY(EditAnywhere, Category=IKDefinition)
	uint32		BoneIndex;

	UPROPERTY(EditAnywhere, Category=IKDefinition)
	bool		Support;

	UPROPERTY(EditAnywhere, Category=IKDefinition)
	float		SupportRatio;

	UPROPERTY(EditAnywhere, Category=IKDefinition)
	FVector		Force;

	UPROPERTY(EditAnywhere, Category=IKDefinition)
	FVector		WorldForce;

	// Set to default values..
	void InitializeValues();
};

USTRUCT()
struct IKINEMACORE_API FIKinemaCOMDef
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, Category = IKDefinition, DisplayName = "DoFX")
	bool DegreeOfFreedomX;

	UPROPERTY(EditAnywhere, Category = IKDefinition, DisplayName = "DoFY")
    bool DegreeOfFreedomY;

	UPROPERTY(EditAnywhere, Category = IKDefinition, DisplayName = "DoFZ")
	bool DegreeOfFreedomZ;


	UPROPERTY(EditAnywhere, Category=IKDefinition)
	FVector Weight;

	UPROPERTY(EditAnywhere, Category=IKDefinition)
	FVector Precision;

	// Set to default values..
	void InitializeValues();

	// Configure the task using our properties.
	void ConfigureTask(FIK::IKBalanceTask& task) const;

#if WITH_EDITOR
	void CopyTaskDefPropertiesFrom(FIKinemaCOMDef const* other);

	void CopyTaskDefPropertiesFrom(FIKinemaCOMDef const& other);

	void ResetToDefaults();
#endif

};

USTRUCT()
struct FIKinemaSolverDef
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, Category=IKDefinition)
	int32 MaxIterations;

	UPROPERTY(VisibleAnywhere, AdvancedDisplay, Category = IKDefinition)
	FName UpAxis;

	UPROPERTY()
	bool ContinuouslySolving;

	UPROPERTY()
	float PoseTolerance;

	UPROPERTY(EditAnywhere, AdvancedDisplay, Category = Advanced)
	float SolutionTolerance;

	UPROPERTY(EditAnywhere, AdvancedDisplay, Category = Advanced)
	float RootTranslationWeight;

	UPROPERTY(EditAnywhere, AdvancedDisplay, Category = Advanced)
	FVector RootTargetTranslation;

	UPROPERTY(EditAnywhere, Category = IKDefinition)
	bool TranslateRoot;

	UPROPERTY(EditAnywhere, AdvancedDisplay, Category = Advanced)
	bool RootTranslationDofX;

	UPROPERTY(EditAnywhere, AdvancedDisplay, Category = Advanced)
	bool RootTranslationDofY;

	UPROPERTY(EditAnywhere, AdvancedDisplay, Category = Advanced)
	bool RootTranslationDofZ;

	UPROPERTY()
	bool Retargeting;

	UPROPERTY()
	FVector RetargetingTaskScale;

	UPROPERTY()
	FVector RetargetingTaskOffset;

	UPROPERTY()
	bool EnableRootTargetTranslation;

	UPROPERTY(EditAnywhere, AdvancedDisplay, Category = Advanced)
	bool EnableMoments;

	UPROPERTY(EditAnywhere, AdvancedDisplay, Category = Advanced)
	float MomentsWeight;

	UPROPERTY(EditAnywhere, AdvancedDisplay, Category = Advanced)
	float MomentsPriority;

	UPROPERTY(EditAnywhere, AdvancedDisplay, Category = Advanced)
	float FigureMass;

	UPROPERTY(EditAnywhere, AdvancedDisplay, Category = Advanced)
	bool UseDefaultZMP;

	UPROPERTY(EditAnywhere, AdvancedDisplay, Category = Advanced)
	FVector ZeroMomentPoint;

	UPROPERTY(EditAnywhere, AdvancedDisplay, Category = Advanced)
	float TasksPrecision;

	UPROPERTY()
	bool RescaleTasks;

	UPROPERTY(EditAnywhere, AdvancedDisplay, Category = Advanced)
	float LimitsGain;

	UPROPERTY(EditAnywhere, Category=IKDefinition)
	float RetargetingGain;

	UPROPERTY()
	float SecondaryTaskWeight;

	UPROPERTY()
	float SecondaryTaskPrecision;

	UPROPERTY(EditAnywhere, Category=IKDefinition, meta = (ClampMin = "0", UIMin = "0", ClampMax = "1", UIMax = "1"))
	float PCoefficient; 

	UPROPERTY(EditAnywhere, Category = IKDefinition)
	bool AutoTune;

	UPROPERTY(EditAnywhere, AdvancedDisplay, Category = Advanced)
	bool ActiveCOM;

	UPROPERTY(EditAnywhere, AdvancedDisplay, Category = Advanced)
	struct FIKinemaCOMDef COM;

	//UPROPERTY(EditFixedSize, EditAnywhere, Category=IKDefinition)
	UPROPERTY( EditAnywhere, Category = IKDefinition)
	TArray<struct FIKinemaBoneDef> Bones;

	UPROPERTY(EditFixedSize, EditAnywhere, Category=IKDefinition)
	TArray<struct FIKinemaTaskDef> Tasks;

	UPROPERTY(EditFixedSize, EditAnywhere, Category=IKDefinition)
	TArray<struct FIKinemaForceDef> Forces;

	// Reset to default values.
	void InitializeValues();

	// Configure the given solver using our properties.
	bool ConfigureSolver(class FIKinemaSolverInstance& solverInstance) const;

	bool ApplySettings(class FIKinemaSolverInstance& solverInstance) const;

	// Called from the outer rig class.
	void PostLoadOrImport();

	void ClearDefinition();

	// Find the index of the task with the given name in our list.
	// Returns INDEX_NONE if not found.
	IKINEMACORE_API int32 FindTaskIndex(const FName& taskName) const;

	// Find the index of the bone with the given name in our list.
	// Returns INDEX_NONE if not found.
	IKINEMACORE_API int32 FindBoneIndex(const FName& boneName) const;

	// Gets the index of the task driving the bone referenced by boneIndex.
	// Returns INDEX_NONE if boneIndex is invalid or if bone is not being driven.
	IKINEMACORE_API int32 GetTaskIndexFromBone(int32 boneIndex) const;

	// Gets the index of the bone being driven by the task referenced by taskIndex.
	// Returns INDEX_NONE if taskIndex is invalid.
	IKINEMACORE_API int32 GetBoneIndexFromTask(int32 taskIndex) const;
};




USTRUCT()
struct IKINEMACORE_API FIKinemaSolverTask
{
	GENERATED_USTRUCT_BODY()
	
	/**
	
		FTransform representing the task target in solver space, or the offset from the animation target depending on the value of AsOffset.
	*/
	UPROPERTY(BlueprintReadWrite, Category=Links)
	FTransform Target;

	/**
		Alpha factor for the linear interpolation between animation target and Target.
	*/

	UPROPERTY(BlueprintReadWrite, Category = Links)
	float Alpha;

	UPROPERTY(BlueprintReadWrite, Category = Links)
	TEnumAsByte<EIKinemaTargetType::Type> TargetType;

	/**
		The provided target transform is in component space
	*/
	UPROPERTY(BlueprintReadWrite, Category = Links)
	bool InComponentSpace;


	UPROPERTY(BlueprintReadWrite, Category=Links)
	bool EnableTranslation;
	UPROPERTY(BlueprintReadWrite, Category=Links)
	bool EnableOrientation;
	UPROPERTY(BlueprintReadWrite, Category=Links)
	bool DebugDraw;
	UPROPERTY(BlueprintReadWrite, Category=Links)
	float DebugDrawScale;

	FIKinemaSolverTask();
};


USTRUCT()
struct IKINEMACORE_API FIKinemaSolverLookAtTask: public FIKinemaSolverTask
{
	GENERATED_USTRUCT_BODY()

	/**
		FVector representing the task target in component space.
	*/
	UPROPERTY(BlueprintReadWrite, Category = Links)
	FVector LookAtTarget;

	/**
		FVector representing the LookAt Axis, in bone local space. If nothing is specified, the value exported in the rig will be used
	*/
	UPROPERTY(BlueprintReadWrite, Category = Links)
	FVector LookAtAxis;

	/**
		Position target in component space
	*/
	UPROPERTY(BlueprintReadWrite, Category = Links)
	FVector PositionTarget;


	UPROPERTY(BlueprintReadWrite, Category = Links)
	bool ReduceWeight;

	bool isLookAt;

	FIKinemaSolverLookAtTask();
};

USTRUCT()
struct IKINEMACORE_API FIKinemaCOMTask
{

	GENERATED_USTRUCT_BODY()

	UPROPERTY(BlueprintReadWrite, Category = Links)
	FVector Target;

	UPROPERTY(BlueprintReadWrite, Category = Links)
	bool Enable;

	UPROPERTY(BlueprintReadWrite, Category = Links)
	bool InComponentSpace;

	UPROPERTY(BlueprintReadWrite, Category = Links)
	bool DebugDraw;

	UPROPERTY(BlueprintReadWrite, Category = Links)
	float DebugDrawScale;

	FIKinemaCOMTask();
};


USTRUCT()
struct IKINEMACORE_API FIKinemaTaskProperties
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Links)
	bool PositionDofX;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Links)
	bool PositionDofY;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Links)
	bool PositionDofZ;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Links)
	int32 PositionDepth;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Links)
	FVector PositionWeight;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Links)
	FVector PositionPrecision;

	//orientation

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Links)
	bool RotateDofX;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Links)
	bool RotateDofY;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Links)
	bool RotateDofZ;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Links)
	int32 RotateDepth;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Links)
	FVector RotateWeight;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Links)
	FVector RotatePrecision;

	FIKinemaTaskProperties();
};

USTRUCT()
struct IKINEMACORE_API FIKinemaSegmentProperties
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Links)
	bool	DofX;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Links)
	bool	DofY;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Links)
	bool	DofZ;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Links)
	bool	EnforceLimits;

	// One weight value for each bone axis.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Links)
	FVector	Weight;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Links)
	bool	EnableLimits;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Links)
	float	LimitsGain;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Links)
	bool	EnableRetargeting;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Links)
	float	RetargetingGain;

	FIKinemaSegmentProperties();
};

USTRUCT()
struct IKINEMACORE_API FIKinemaCOMTaskProperties
{
	GENERATED_USTRUCT_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Links)
	bool DofX;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Links)
	bool DofY;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Links)
	bool DofZ;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Links)
	FVector Weight;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Links)
	FVector Precision;

	FIKinemaCOMTaskProperties();
};


USTRUCT()
struct IKINEMACORE_API FIKinemaTaskOverride
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, Category=Links)
	FName	TaskName;

	UPROPERTY(EditAnywhere, Category=Links)
	bool	ShowAsPin;
};

USTRUCT()
struct IKINEMACORE_API FIKinemaSegmentOverride
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, Category=Links)
	FName	BoneName;

	UPROPERTY(EditAnywhere, Category=Links)
	bool	ShowAsPin;
};

USTRUCT()
struct IKINEMACORE_API FIKinemaCOMTaskOverride
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, Category = Links)
	FName	Name;

	UPROPERTY(EditAnywhere, Category = Links)
	bool	ShowAsPin;

};


UCLASS(hidecategories=Object, MinimalAPI, BlueprintType)
class UIKinemaRig : public UObject
{
	GENERATED_UCLASS_BODY()

#if WITH_EDITORONLY_DATA
	// File containing a text description of this rig.
	// Not used from in-game runtime.
	UPROPERTY()
	FString ImportPath;
#endif //WITH_EDITORONLY_DATA
	UPROPERTY()
	int32	UpdateVersion;
	// The solver definition.
	UPROPERTY(EditAnywhere, Category = IKDefinition, meta = (ShowOnlyInnerProperties))
	FIKinemaSolverDef SolverDef;

	// Called just before re-importing properties from text.
	IKINEMACORE_API void PreReimport();

	// To do post-processing on loaded properties.
	virtual void PostLoad() override;

	// Called from the above, and from import.
	IKINEMACORE_API void PostLoadOrImport();

	void SetMaxIteration(int Iter);

	// Create an IKinema solver instance using this rig.
	void CreateSolver(class FIKinemaSolverInstance& solverInstance) const;

	void SolveAndUpdatePose(class FIKinemaSolverInstance& solverInstance, TArray<FTransform>& ue4PoseInAndOut);

	void ClearDefinition();

#if WITH_EDITOR
	virtual void PostEditChangeChainProperty( struct FPropertyChangedChainEvent& PropertyChangedEvent );

	IKINEMACORE_API bool CreateRigFromSkeleton();
#endif

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Skeleton)
	USkeleton* Skeleton;

private:
	int MaxIterations;
	// Transfer an IKinema pose to UE4.
	void IKinemaToUE4Pose(class FIKinemaSolverInstance& solverInstance, TArray<FTransform>& ue4Pose) const;
};

// Represents a binding of an IKinemaRig to a specific skeleton.
// Resolves UE4 bone indices and stores them.
class IKINEMACORE_API FIKinemaRigToSkeletonBinding
{
public:

	// Default constructor.
	FIKinemaRigToSkeletonBinding();

	// Bind the given IKinemaRig to the given skeleton and store the result.
	// Since the member properties are not stored on file, this should be called
	// from runtime each time, to set up the members.
	bool BindToSkeleton(UIKinemaRig* ikinemaRig, USkeleton* skeleton, USkeletalMeshComponent* component);

	// Configure the given solver using our cached data.
	void ConfigureSolver(FIK::IKSolver& solver) const;

	// Access the UE4 bone index given the bone def index.
	int32 GetUE4BoneIndex(int32 boneDefIndex) const;

	//int32 GetRootBoneIndex() const;

	// Access the IKinema rig.
	UIKinemaRig* GetRig() const;

	const USkeleton* GetSkeleton() const;

	const USkeletalMeshComponent* GetSkeletalMeshComponent() const;

	void ClearImportBones();

	bool IsValid();

private:

	// Remember the rig.
	UIKinemaRig* mRig;

	USkeleton* mSkeleton;

	USkeletalMeshComponent* mComponent;

	// Array of bones (IKinema segments) to directly import into the solver.
	TArray<FIK::ImportBone> ImportBones;

	// Cache the UE4 bone indices corresponding to IKinema segments.
	TArray<int32> UE4BoneIndices;
	//int32 RootBoneIndex; 
};

// This represents a task instance on a segment/joint/bone.
struct FIKinemaJointTaskInstance
{
	// Either of these could be null.
	FIK::IKPositionTask* mPositionTask;
	FIK::IKOrientationTask* mOrientationTask;

	// Default constructor.
	FIKinemaJointTaskInstance();

	// Utility methods to configure the tasks.

	void SetPositionWeight(const FVector& weight);
	void SetOrientationWeight(const FVector& weight);
	void SetPositionTarget(const FVector& positionTarget);
	void SetOrientationTarget(const FQuat& orientationTarget);
	void SetPositionEnabled(bool state);
	void SetOrientationEnabled(bool state);
	void ApplyPositionProperties(const FIKinemaTaskProperties& properties);
	void ApplyOrientationProperties(const FIKinemaTaskProperties& properties);
};

// This represents a COM task instance.
struct FIKinemaCOMTaskInstance
{
	// Either of these could be null.
	FIK::IKBalanceTask* mTask;

	// Default constructor.
	FIKinemaCOMTaskInstance();

	// Utility methods to configure the tasks.
	void SetWeight(const FVector& weight);
	void SetTarget(const FVector& positionTarget);
	void SetEnabled(bool state);
	void ApplyProperties(const FIKinemaCOMTaskProperties& properties);
};


// This represents an instance of an IKinema solver
// along with the tasks on it.
class IKINEMACORE_API FIKinemaSolverInstance
{
private:
	unsigned int InstID;
	// Binding to a skeleton.
	const FIKinemaRigToSkeletonBinding* mSkeletonBinding;

	// The array of tasks that were created on joints/segments.
	TArray<struct FIKinemaJointTaskInstance> mJointTasks;

	//The COM task wrapper
	FIKinemaCOMTaskInstance mCoMTask;

	// The IKinema solver.
	FIK::IKSolver* mIKSolver;

	FTransform mSolverToWorldSpace;

public:

	// Default constructor.
	FIKinemaSolverInstance();

	// Destructor.
	~FIKinemaSolverInstance();

	void Dump(const FString& Folder) const;

	// Create using the given skeleton binding.
	void Create(const FIKinemaRigToSkeletonBinding& skeletonBinding);

	// Destroy this solver instance.
	void Destroy();

	bool UpdateAndSolve(
		TArray<FIKinemaSolverTask>& tasks,
		const TArray<FIKinemaTaskProperties>& taskProperties,
		const TArray<FIKinemaTaskOverride>& taskOverride,
		const TArray<FIKinemaSegmentProperties>& segProperties,
		const TArray<FIKinemaSegmentOverride>& segOverride,
		/*CoM Task*/ const FIKinemaCOMTask& comTask,
		/*CoM Task Propertires*/ const FIKinemaCOMTaskProperties& comProperties,
		/*CoM Override*/ const FIKinemaCOMTaskOverride& comOverride,
		TArray<FTransform>& outPose);

	// Access the skeleton binding.
	const FIKinemaRigToSkeletonBinding* GetSkeletonBinding() const;

	const FTransform GetSolverSpaceTransform(const int& boneIndex, const TArray<FTransform>& outPose) const;

private:

	bool UpdateTaskInstances(const TArray<FIKinemaSolverTask>& tasks, const TArray<FIKinemaTaskProperties>& taskProperties, const TArray<FIKinemaTaskOverride>& taskOverride);
	bool UpdateCoM(const FIKinemaCOMTask& comTask, const FIKinemaCOMTaskProperties&  comProperties, const FIKinemaCOMTaskOverride& comOverride);
	friend class UIKinemaRig;
	friend struct FIKinemaSolverDef;
};
