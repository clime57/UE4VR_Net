#pragma once

#include "AnimGraphNode_SetBone.generated.h"

UCLASS(MinimalAPI)
class UAnimGraphNode_SetBone: public UAnimGraphNode_Base
{
	GENERATED_UCLASS_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Settings)
	FAnimNode_SetBone Node;

#if WITH_EDITOR
	// UEdGraphNode interface
	virtual FText GetNodeTitle(ENodeTitleType::Type TitleType) const override;
	virtual FLinearColor GetNodeTitleColor() const override;
	virtual FText GetTooltipText() const override;
	// End of UEdGraphNode interface
#endif
};
