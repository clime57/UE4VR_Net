// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "BlueprintGraphDefinitions.h"
#include "Animation/AnimSingleNodeInstance.h"
#include "AnimNode_IKinemaSolver.h"
#include "Animation/AnimSingleNodeInstanceProxy.h"
#include "AnimIKinemaPreviewInstance.generated.h"


/** Proxy override for this UAnimInstance-derived class */
USTRUCT()
struct FAnimIKinemaPreviewInstanceProxy : public FAnimSingleNodeInstanceProxy
{
	GENERATED_BODY()

public:
	FAnimIKinemaPreviewInstanceProxy()
	{
		bCanProcessAdditiveAnimations = true;
	}

	FAnimIKinemaPreviewInstanceProxy(UAnimInstance* InAnimInstance)
		: FAnimSingleNodeInstanceProxy(InAnimInstance)
	{
		bCanProcessAdditiveAnimations = true;
	}

	virtual void Initialize(UAnimInstance* InAnimInstance) override;
	virtual void Update(float DeltaSeconds) override;
	virtual bool Evaluate(FPoseContext& Output) override;

	FAnimNode_IKinemaSolver& GetIKinemaSolver()
	{
		return IKinemaSolver;
	}

	TArray<FIKinemaSolverTask>& GetIKinemaTasks()
	{
		return Tasks;
	}

	TArray<FIKinemaSolverLookAtTask> & GetIKinemaLookAtTasks()
	{
		return LookAtTasks;
	}


	/**
	* Return the task instance to edit, based on the TaskIndex passed in
	* @param   TaskIndex   The index of the task to edit
	* @return  the task instance
	*/
	FIKinemaSolverTask* GetTask(const int& TaskIndex);

	/**
	* Return the Look-At task instance to edit, based on the TaskIndex passed in
	* @param   TaskIndex   The index of the task to edit
	* @return  the Look-At task instance
	*/
	FIKinemaSolverLookAtTask* GetLookAtTask(const int& TaskIndex);

	/**
	* Set the IKinema rig asset in the IKinemaSolver instance
	* @param  InRig  The input rig from the shared data instance
	*/
	void StartTest(UIKinemaRig* InRig);
	
	/**
	* Stop testing the IKinemaRig
	*/
	void EndTest();
private:
	

private:
	/** IKinema Solver for this Rig */
	FAnimNode_IKinemaSolver IKinemaSolver;

	TArray<FIKinemaSolverTask> Tasks;

	TArray<FIKinemaSolverLookAtTask> LookAtTasks;

	/*
	* Used to determine if controller has to be applied or not
	* Used to disable controller during editing
	*/
	bool bEnableSolver;

	/*
	* Map the rig task index to the look at task index
	*/
	TArray<int16> LookAtTasksMap;
};

/**
 * This Instance only contains one AnimationAsset, and produce poses
 * Used by Preview in AnimGraph, Playing single animation in Kismet2 and etc
 */

UCLASS(transient, NotBlueprintable, noteditinlinenew)
class  UAnimIKinemaPreviewInstance : public UAnimSingleNodeInstance
{
	GENERATED_UCLASS_BODY()

	// Disable compiler-generated deprecation warnings by implementing our own destructor
	PRAGMA_DISABLE_DEPRECATION_WARNINGS
	~UAnimIKinemaPreviewInstance() {}
	PRAGMA_ENABLE_DEPRECATION_WARNINGS

	//~ Begin UObject Interface
	virtual void Serialize(FArchive& Ar) override;
	//~ End UObject Interface

	// Begin UAnimInstance interface
	virtual void NativeInitializeAnimation() override;
	virtual FAnimInstanceProxy* CreateAnimInstanceProxy() override;
	// End UAnimInstance interface

	UAnimSequence* GetAnimSequence();

	// Begin UAnimSingleNodeInstance interface
	virtual void SetAnimationAsset(UAnimationAsset* NewAsset, bool bIsLooping = true, float InPlayRate = 1.f) override;
	// End UAnimSingleNodeInstance interface

	/**
	 * Return the task instance to edit, based on the TaskIndex passed in
	 * @param   TaskIndex   The index of the task to edit
	 * @return  the task instance
	*/
	FIKinemaSolverTask* GetTask(const int& TaskIndex);

	/**
	* Return the Look-At task instance to edit, based on the TaskIndex passed in
	* @param   TaskIndex   The index of the task to edit
	* @return  the Look-At task instance
	*/
	FIKinemaSolverLookAtTask* GetLookAtTask(const int& TaskIndex);

	/**
	 * Set the IKinema rig asset in the IKinemaSolver instance
	 * @param  InRig  The input rig from the shared data instance
	 */
	void StartTest(UIKinemaRig* InRig);


	
	/**
	 * Stop testing the IKinemaRig
	 */
	void EndTest();

};



