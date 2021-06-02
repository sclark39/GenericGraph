#pragma once

#include "CoreMinimal.h"
#include "EdGraph/EdGraphNode.h"
#include "GenericGraphNode.h"
#include "EdNode_GenericGraphNode.generated.h"

class UEdNode_GenericGraphEdge;
class UEdGraph_GenericGraph;
class SEdNode_GenericGraphNode;


UCLASS(MinimalAPI)
class UEdNode_GenericGraphNode : public UEdGraphNode
{
	GENERATED_BODY()

public:
	UEdNode_GenericGraphNode();
	virtual ~UEdNode_GenericGraphNode();

	UPROPERTY(VisibleAnywhere, Instanced, Category = "GenericGraph")
	UGenericGraphNode* GenericGraphNode;

	void SetGenericGraphNode(UGenericGraphNode* InNode);
	UEdGraph_GenericGraph* GetGenericGraphEdGraph();

	SEdNode_GenericGraphNode* SEdNode;

	virtual FString GetDocumentationExcerptName() const override;
	virtual FText GetTooltipText() const override;

	virtual void AllocateDefaultPins() override;
	virtual FText GetNodeTitle(ENodeTitleType::Type TitleType) const override;
	virtual void PrepareForCopying() override;
	virtual void AutowireNewNode(UEdGraphPin* FromPin) override;

	virtual FLinearColor GetBackgroundColor() const;
	virtual const FSlateBrush* GetNodeIcon() const;

	virtual UEdGraphPin* GetInputPin() const;
	virtual UEdGraphPin* GetOutputPin() const;

	virtual bool CanDuplicateNode() const override;
	virtual bool CanUserDeleteNode() const override;

	virtual bool Modify(bool bAlwaysMarkDirty = true) override;

#if WITH_EDITOR
	virtual void PostEditUndo() override;
#endif

};
