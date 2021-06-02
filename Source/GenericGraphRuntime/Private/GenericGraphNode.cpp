#include "GenericGraphNode.h"
#include "GenericGraph.h"

#define LOCTEXT_NAMESPACE "GenericGraphNode"

UGenericGraphNode::UGenericGraphNode()
{
#if WITH_EDITORONLY_DATA
	CompatibleGraphType = UGenericGraph::StaticClass();

	BackgroundColor = FLinearColor(0.01f,0.01,0.01f);
#endif
}

UGenericGraphNode::~UGenericGraphNode()
{
}

UGenericGraphEdge* UGenericGraphNode::GetEdge(const UGenericGraphNode* ChildNode) const
{
	return Edges.Contains(ChildNode) ? Edges.FindChecked(ChildNode) : nullptr;
}

FText UGenericGraphNode::GetDescription_Implementation() const
{
	return LOCTEXT("NodeDesc", "Generic Graph Node");
}

#if WITH_EDITOR
TSubclassOf<UGenericGraphEdge> UGenericGraphNode::GetEdgeType() const
{
	if (Graph->bEdgeEnabled)
		return Graph->EdgeType;
	return nullptr;
}

bool UGenericGraphNode::IsNameEditable() const
{
	return true;
}

FLinearColor UGenericGraphNode::GetBackgroundColor() const
{
	return BackgroundColor;
}

FText UGenericGraphNode::GetNodeTitle() const
{
	return NodeTitle.IsEmpty() ? GetDescription() : NodeTitle;
}

void UGenericGraphNode::SetNodeTitle(const FText& NewTitle)
{
	NodeTitle = NewTitle;
}

FText UGenericGraphNode::GetNodeTooltip() const
{
	return NodeTooltip.IsEmpty() ? GetClass()->GetToolTipText() : NodeTooltip;
}

FText UGenericGraphNode::GetContextMenuName() const
{
	return ContextMenuName;
}

FText UGenericGraphNode::GetContextMenuCategory() const
{
	return ContextMenuCategory;
}

FText UGenericGraphNode::GetContextMenuDescription() const
{
	return ContextMenuDescription;
}

bool UGenericGraphNode::CanCreateConnection(UGenericGraphNode* Other, FText& ErrorMessage)
{	
	return true;
}

bool UGenericGraphNode::CanCreateConnectionTo(UGenericGraphNode* Other, int32 NumberOfChildrenNodes, FText& ErrorMessage)
{
	EGenericGraphNodeLimit LimitType;
	int LimitCount;
	GetChildrenLimit(LimitType, LimitCount);

	if (LimitType == EGenericGraphNodeLimit::Forbidden || (LimitType == EGenericGraphNodeLimit::Limited && ChildrenLimit <= 0))
	{
		ErrorMessage = FText::FromString("Node can not have children");
		return false;
	}

	if (LimitType == EGenericGraphNodeLimit::Limited && NumberOfChildrenNodes >= LimitCount)
	{
		ErrorMessage = FText::FromString("Children limit exceeded");
		return false;
	}

	return CanCreateConnection(Other, ErrorMessage);
}

bool UGenericGraphNode::CanCreateConnectionFrom(UGenericGraphNode* Other, int32 NumberOfParentNodes, FText& ErrorMessage)
{
	EGenericGraphNodeLimit LimitType;
	int LimitCount;
	GetParentLimit(LimitType, LimitCount);

	if (LimitType == EGenericGraphNodeLimit::Forbidden || (LimitType == EGenericGraphNodeLimit::Limited && LimitCount <= 0))
	{
		ErrorMessage = FText::FromString("Node can not have parents");
		return false;
	}

	if (LimitType == EGenericGraphNodeLimit::Limited && NumberOfParentNodes >= LimitCount)
	{
		ErrorMessage = FText::FromString("Parent limit exceeded");
		return false;
	}

	return true;
}

const FSlateBrush* UGenericGraphNode::GetNodeIcon() const
{
	return FEditorStyle::GetBrush(TEXT("BTEditor.Graph.BTNode.Icon"));
}

void UGenericGraphNode::GetChildrenLimit(EGenericGraphNodeLimit &LimitType, int32 &LimitCount) const
{
	LimitType = ChildrenLimitType;
	LimitCount = ChildrenLimit;
}

void UGenericGraphNode::GetParentLimit(EGenericGraphNodeLimit &LimitType, int32 &LimitCount) const
{
	LimitType = ParentLimitType;
	LimitCount = ParentLimit;
}


#endif

bool UGenericGraphNode::IsLeafNode() const
{
	return ChildrenNodes.Num() == 0;
}

UGenericGraph* UGenericGraphNode::GetGraph() const
{
	return Graph;
}

#undef LOCTEXT_NAMESPACE
