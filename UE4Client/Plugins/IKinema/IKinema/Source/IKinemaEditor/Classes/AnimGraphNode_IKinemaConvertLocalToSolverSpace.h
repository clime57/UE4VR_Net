#pragma once

#include "AnimGraphNode_IKinemaConvertLocalToSolverSpace.generated.h"

UCLASS(MinimalAPI)
class UAnimGraphNode_IKinemaConvertLocalToSolverSpace : public UAnimGraphNode_Base
{
	GENERATED_UCLASS_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Settings)
	FAnimNode_IKinemaConvertLocalToSolverSpace Node;

#if WITH_EDITOR
	// UEdGraphNode interface
	virtual FText GetNodeTitle(ENodeTitleType::Type TitleType) const override;
	virtual FLinearColor GetNodeTitleColor() const override;
	virtual FText GetTooltipText() const override;
	// End of UEdGraphNode interface

	// UK2Node interface
	virtual void GetMenuActions(FBlueprintActionDatabaseRegistrar& ActionRegistrar) const override;
	virtual void GetMenuEntries(FGraphContextMenuBuilder& ContextMenuBuilder) const override;
	virtual FString GetNodeCategory() const override;
	virtual bool IsActionFilteredOut(class FBlueprintActionFilter const& Filter) override;
	// End of UK2Node interface

	virtual void CustomizePinData(UEdGraphPin* Pin, FName SourcePropertyName, int32 ArrayIndex) const override;

	// UAnimGraphNode_Base interface
	virtual void ValidateAnimNodeDuringCompilation(class USkeleton* ForSkeleton, class FCompilerResultsLog& MessageLog) override;
	virtual void PreloadRequiredAssets() override;
	// End of UAnimGraphNode_Base interface
#endif
};
