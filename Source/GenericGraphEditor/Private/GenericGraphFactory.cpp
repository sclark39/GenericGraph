#include "GenericGraphFactory.h"
#include "GenericGraph.h"
#include <SClassPickerDialog.h>
#include <ClassViewerModule.h>
#include <ClassViewerFilter.h>
#include <ModuleManager.h>

#define LOCTEXT_NAMESPACE "GenericGraphFactory"

class FAssetClassParentFilter : public IClassViewerFilter
{
public:

	FAssetClassParentFilter() : DisallowedClassFlags(CLASS_None)
	{}

	/** All children of these classes will be included unless filtered out by another setting. */
	TSet< const UClass* > AllowedChildrenOfClasses;

	/** Disallowed class flags. */
	EClassFlags DisallowedClassFlags;

public:
	virtual bool IsClassAllowed(const FClassViewerInitializationOptions& InInitOptions, const UClass* InClass, TSharedRef<class FClassViewerFilterFuncs> InFilterFuncs) override
	{
		bool bAllowed = !InClass->HasAnyClassFlags(DisallowedClassFlags)
			&& InFilterFuncs->IfInChildOfClassesSet(AllowedChildrenOfClasses, InClass) != EFilterReturn::Failed;

		return bAllowed;
	}


	virtual bool IsUnloadedClassAllowed(const FClassViewerInitializationOptions& InInitOptions, const TSharedRef<const class IUnloadedBlueprintData> InUnloadedClassData, TSharedRef<class FClassViewerFilterFuncs> InFilterFuncs) override
	{
		return !InUnloadedClassData->HasAnyClassFlags(DisallowedClassFlags)
			&& InFilterFuncs->IfInChildOfClassesSet(AllowedChildrenOfClasses, InUnloadedClassData) != EFilterReturn::Failed;
	}

};

UGenericGraphFactory::UGenericGraphFactory()
{
	bCreateNew = true;
	bEditAfterNew = true;
	SupportedClass = UGenericGraph::StaticClass();
}

UGenericGraphFactory::~UGenericGraphFactory()
{

}

UObject* UGenericGraphFactory::FactoryCreateNew(UClass* Class, UObject* InParent, FName Name, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn)
{
	if (GraphClass != nullptr)
	{
		return NewObject<UGenericGraph>(InParent, GraphClass, Name, Flags | RF_Transactional);
	}
	else
	{
		// if we have no data asset class, use the passed-in class instead
		check(Class->IsChildOf(SupportedClass));
		return NewObject<UGenericGraph>(InParent, Class, Name, Flags);
	}
}


bool UGenericGraphFactory::ConfigureProperties()
{
	// nullptr the DataAssetClass so we can check for selection
	GraphClass = nullptr;

	// Load the classviewer module to display a class picker
	FClassViewerModule& ClassViewerModule = FModuleManager::LoadModuleChecked<FClassViewerModule>("ClassViewer");

	// Fill in options
	FClassViewerInitializationOptions Options;
	Options.Mode = EClassViewerMode::ClassPicker;

	TSharedPtr<FAssetClassParentFilter> Filter = MakeShareable(new FAssetClassParentFilter);
	Options.ClassFilter = Filter;

	Filter->DisallowedClassFlags = CLASS_Abstract | CLASS_Deprecated | CLASS_NewerVersionExists | CLASS_HideDropDown;
	Filter->AllowedChildrenOfClasses.Add(SupportedClass);

	const FText TitleText = LOCTEXT("CreateGenericGraphOptions", "Pick Generic Graph Class");
	UClass* ChosenClass = nullptr;
	const bool bPressedOk = SClassPickerDialog::PickClass(TitleText, Options, ChosenClass, SupportedClass);

	if (bPressedOk)
	{
		GraphClass = ChosenClass;
	}

	return bPressedOk;
}

#undef LOCTEXT_NAMESPACE
