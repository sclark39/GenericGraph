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

bool UGenericGraphNode::CanCreateConnection(UGenericGraphNode* Other, FText& ErrorMessage)
{	
	return true;
}

bool UGenericGraphNode::CanCreateConnectionTo(UGenericGraphNode* Other, int32 NumberOfChildrenNodes, FText& ErrorMessage)
{
	if (ChildrenLimitType == ENodeLimit::Forbidden || (ChildrenLimitType == ENodeLimit::Limited && ChildrenLimit <= 0))
	{
		ErrorMessage = FText::FromString("Node can not have children");
		return false;
	}

	if (ChildrenLimitType == ENodeLimit::Limited && NumberOfChildrenNodes >= ChildrenLimit)
	{
		ErrorMessage = FText::FromString("Children limit exceeded");
		return false;
	}

	return CanCreateConnection(Other, ErrorMessage);
}

bool UGenericGraphNode::CanCreateConnectionFrom(UGenericGraphNode* Other, int32 NumberOfParentNodes, FText& ErrorMessage)
{
	if (ParentLimitType == ENodeLimit::Forbidden || (ParentLimitType == ENodeLimit::Limited && ParentLimit <= 0))
	{
		ErrorMessage = FText::FromString("Node can not have parents");
		return false;
	}

	if (ParentLimitType == ENodeLimit::Limited && NumberOfParentNodes >= ParentLimit)
	{
		ErrorMessage = FText::FromString("Parent limit exceeded");
		return false;
	}

	return true;
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
