#include "GenericGraphAssetEditor/EdNode_GenericGraphNode.h"
#include "GenericGraphAssetEditor/EdGraph_GenericGraph.h"
#include "Kismet2/Kismet2NameValidators.h"
#include "Kismet2/BlueprintEditorUtils.h"
#include "GenericGraph.h"

#define LOCTEXT_NAMESPACE "EdNode_GenericGraph"

UEdNode_GenericGraphNode::UEdNode_GenericGraphNode()
{
	bCanRenameNode = true;
}

UEdNode_GenericGraphNode::~UEdNode_GenericGraphNode()
{

}

FString UEdNode_GenericGraphNode::GetDocumentationExcerptName() const
{
	UClass* MyClass = GetClass();
	if (GenericGraphNode)
		MyClass = GenericGraphNode->GetClass();

	// Default the node to searching for an excerpt named for the C++ node class name, including the U prefix.
	// This is done so that the excerpt name in the doc file can be found by find-in-files when searching for the full class name.
	return FString::Printf(TEXT("%s%s"), MyClass->GetPrefixCPP(), *MyClass->GetName());
}

FText UEdNode_GenericGraphNode::GetTooltipText() const
{
	if (GenericGraphNode)
		return GenericGraphNode->GetNodeTooltip();

	UClass* MyClass = GetClass();
	return MyClass->GetToolTipText();
}


void UEdNode_GenericGraphNode::AllocateDefaultPins()
{
	CreatePin(EGPD_Input, "MultipleNodes", FName(), TEXT("In"));
	CreatePin(EGPD_Output, "MultipleNodes", FName(), TEXT("Out"));
}

UEdGraph_GenericGraph* UEdNode_GenericGraphNode::GetGenericGraphEdGraph()
{
	return Cast<UEdGraph_GenericGraph>(GetGraph());
}

FText UEdNode_GenericGraphNode::GetNodeTitle(ENodeTitleType::Type TitleType) const
{
	if (GenericGraphNode == nullptr)
	{
		return Super::GetNodeTitle(TitleType);
	}
	else
	{
		return GenericGraphNode->GetNodeTitle();
	}
}

void UEdNode_GenericGraphNode::PrepareForCopying()
{
	GenericGraphNode->Rename(nullptr, this, REN_DontCreateRedirectors | REN_DoNotDirty);
}

void UEdNode_GenericGraphNode::AutowireNewNode(UEdGraphPin* FromPin)
{
	Super::AutowireNewNode(FromPin);

	if (FromPin != nullptr)
	{
		if (GetSchema()->TryCreateConnection(FromPin, GetInputPin()))
		{
			FromPin->GetOwningNode()->NodeConnectionListChanged();
		}
	}
}

void UEdNode_GenericGraphNode::SetGenericGraphNode(UGenericGraphNode* InNode)
{
	GenericGraphNode = InNode;
}

FLinearColor UEdNode_GenericGraphNode::GetBackgroundColor() const
{
	return GenericGraphNode? GenericGraphNode->GetBackgroundColor() : FLinearColor::Black;
}

const FSlateBrush *UEdNode_GenericGraphNode::GetNodeIcon() const
{
	return GenericGraphNode? GenericGraphNode->GetNodeIcon() : FEditorStyle::GetBrush(TEXT("BTEditor.Graph.BTNode.Icon"));
}

UEdGraphPin* UEdNode_GenericGraphNode::GetInputPin() const
{
	return Pins[0];
}

UEdGraphPin* UEdNode_GenericGraphNode::GetOutputPin() const
{
	return Pins[1];
}

bool UEdNode_GenericGraphNode::CanDuplicateNode() const
{
	bool bIsUniqueRoot = GenericGraphNode &&
		GenericGraphNode->Graph->NodeType != GenericGraphNode->Graph->RootNodeType &&
		GenericGraphNode->GetClass() == GenericGraphNode->Graph->RootNodeType;
	return !bIsUniqueRoot;
}

bool UEdNode_GenericGraphNode::CanUserDeleteNode() const
{
	bool bIsUniqueRoot = GenericGraphNode &&
		GenericGraphNode->Graph->NodeType != GenericGraphNode->Graph->RootNodeType &&
		GenericGraphNode->GetClass() == GenericGraphNode->Graph->RootNodeType;
	return !bIsUniqueRoot;
}


void UEdNode_GenericGraphNode::PostEditUndo()
{
	UEdGraphNode::PostEditUndo();
}

#undef LOCTEXT_NAMESPACE
