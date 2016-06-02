// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.

#pragma once
#include "IKinemaRig.h"
#include "AnimIKinemaPreviewInstance.h"
#include "IKinemaRigToolEdSkeletalMeshComponent.generated.h"


UCLASS()
class UIKinemaRigToolEdSkeletalMeshComponent : public USkeletalMeshComponent
{
	GENERATED_UCLASS_BODY()

	/** Data and methods shared across multiple classes */
	class FIKinemaRigToolSharedData* SharedData;

	// Draw colors

	FColor BoneUnselectedColor;
	FColor BoneSelectedColor;
	FColor HierarchyDrawColor;
	FColor AnimSkelDrawColor;
	
	UPROPERTY(transient)
	class UAnimIKinemaPreviewInstance* PreviewInstance;


	/** Mesh-space matrices showing state of just animation (ie before IKinema) - useful for debugging! */
	TArray<FTransform> AnimationSpaceBases;


	/** UPrimitiveComponent interface */
	virtual FPrimitiveSceneProxy* CreateSceneProxy() override;
	
	/** Renders non-hitproxy elements for the viewport, this function is called in the Game Thread */
	virtual void Render(const FSceneView* View, class FPrimitiveDrawInterface* PDI);
	
	/** Renders hitproxy elements for the viewport, this function is called in the Game Thread */
	virtual void RenderHitTest(const FSceneView* View,class FPrimitiveDrawInterface* PDI);

	/** Handles most of the rendering logic for this component */
	void RenderAssetTools(const FSceneView* View, class FPrimitiveDrawInterface* PDI, bool bHitTest);

	/** Draw the mesh skeleton using lines. bAnimSkel means draw the animation skeleton */
	void DrawHierarchy(FPrimitiveDrawInterface* PDI, bool bAnimSkel);

	/** Draws a constraint */
	void DrawConstraint(int32 ConstraintIndex, const FSceneView* View, FPrimitiveDrawInterface* PDI);

	/** Draws the CoM constraint */
	void DrawCoMConstraint(const FSceneView* View, FPrimitiveDrawInterface* PDI);

	/** Returns the physics asset for this PhATEd component - note: This hides the implementation in the USkinnedMeshComponent base class */
	class UIKinemaRig* GetIKinemaRig() const;


	virtual void InitAnim(bool bForceReinit);
};
