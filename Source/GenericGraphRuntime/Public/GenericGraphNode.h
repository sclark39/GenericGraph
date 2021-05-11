#pragma once

#include "CoreMinimal.h"
#include "Templates/SubclassOf.h"
#include "GenericGraphNode.generated.h"

class UGenericGraph;
class UGenericGraphEdge;

UENUM(BlueprintType)
enum class EGenericGraphNodeLimit : uint8
{
	Unlimited,
    Limited,
	Forbidden
};


UCLASS(Blueprintable)
class GENERICGRAPHRUNTIME_API UGenericGraphNode : public UObject
{
	GENERATED_BODY()

public:
	UGenericGraphNode();
	virtual ~UGenericGraphNode();

	UPROPERTY(VisibleDefaultsOnly, Category = "GenericGraphNode")
	UGenericGraph* Graph;

	UPROPERTY(BlueprintReadOnly, Category = "GenericGraphNode")
	TArray<UGenericGraphNode*> ParentNodes;

	UPROPERTY(BlueprintReadOnly, Category = "GenericGraphNode")
	TArray<UGenericGraphNode*> ChildrenNodes;

	UPROPERTY(BlueprintReadOnly, Category = "GenericGraphNode")
	TMap<UGenericGraphNode*, UGenericGraphEdge*> Edges;

	UFUNCTION(BlueprintCallable, Category = "GenericGraphNode")
	virtual UGenericGraphEdge* GetEdge(const UGenericGraphNode* ChildNode) const;

	UFUNCTION(BlueprintCallable, Category = "GenericGraphNode")
	bool IsLeafNode() const;

	UFUNCTION(BlueprintCallable, Category = "GenericGraphNode")
	UGenericGraph* GetGraph() const;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "GenericGraphNode")
	FText GetDescription() const;
	virtual FText GetDescription_Implementation() const;

	//////////////////////////////////////////////////////////////////////////
#if WITH_EDITORONLY_DATA
	UPROPERTY(EditDefaultsOnly, Category = "GenericGraphNode_Editor")
	FText NodeTitle;

	UPROPERTY(VisibleDefaultsOnly, Category = "GenericGraphNode_Editor")
	TSubclassOf<UGenericGraph> CompatibleGraphType;

	UPROPERTY(EditDefaultsOnly, Category = "GenericGraphNode_Editor")
	FLinearColor BackgroundColor;

	UPROPERTY(EditDefaultsOnly, Category = "GenericGraphNode_Editor")
	FText ContextMenuName;

	UPROPERTY(EditDefaultsOnly, Category = "GenericGraphNode_Editor")
	FText ContextMenuCategory;

	UPROPERTY(EditDefaultsOnly, Category = "GenericGraphNode_Editor")
	EGenericGraphNodeLimit ParentLimitType;

	UPROPERTY(EditDefaultsOnly, Category = "GenericGraphNode_Editor" ,meta = (ClampMin = "1",EditCondition = "ParentLimitType == ENodeLimit::Limited", EditConditionHides))
	int32 ParentLimit = 1;

	UPROPERTY(EditDefaultsOnly, Category = "GenericGraphNode_Editor")
	EGenericGraphNodeLimit ChildrenLimitType;

	UPROPERTY(EditDefaultsOnly, Category = "GenericGraphNode_Editor" ,meta = (ClampMin = "1",EditCondition = "ChildrenLimitType == ENodeLimit::Limited", EditConditionHides))
	int32 ChildrenLimit = 1;
	
#endif

#if WITH_EDITOR
	virtual TSubclassOf<UGenericGraphEdge> GetEdgeType() const;

	virtual bool IsNameEditable() const;

	virtual FLinearColor GetBackgroundColor() const;

	virtual FText GetNodeTitle() const;

	virtual void SetNodeTitle(const FText& NewTitle);

	virtual const FSlateBrush* GetNodeIcon() const;

	virtual bool CanCreateConnection(UGenericGraphNode* Other, FText& ErrorMessage);

	virtual bool CanCreateConnectionTo(UGenericGraphNode* Other, int32 NumberOfChildrenNodes, FText& ErrorMessage);
	virtual bool CanCreateConnectionFrom(UGenericGraphNode* Other, int32 NumberOfParentNodes, FText& ErrorMessage);
#endif
};
