// Copyright Narrative Tools 2022. 


#include "AssetTypeActions_DialogueAsset.h"
#include "DialogueGraphEditor.h"
#include "NarrativeDialogueEditorModule.h"
#include "DialogueBlueprint.h"

FAssetTypeActions_DialogueAsset::FAssetTypeActions_DialogueAsset(uint32 InAssetCategory) : Category(InAssetCategory)
{

}

UClass* FAssetTypeActions_DialogueAsset::GetSupportedClass() const
{
	return UDialogueBlueprint::StaticClass();
}

void FAssetTypeActions_DialogueAsset::OpenAssetEditor(const TArray<UObject*>& InObjects, TSharedPtr<class IToolkitHost> EditWithinLevelEditor /*= TSharedPtr<IToolkitHost>()*/)
{
	for (auto Object : InObjects)
	{
		if (UDialogueBlueprint* DialogueBlueprint = Cast<UDialogueBlueprint>(Object))
		{
			bool bFoundExisting = false;

			FDialogueGraphEditor* ExistingInstance = nullptr;

			if (UAssetEditorSubsystem* AESubsystem = GEditor->GetEditorSubsystem<UAssetEditorSubsystem>())
			{
				ExistingInstance = static_cast<FDialogueGraphEditor*>(AESubsystem->FindEditorForAsset(DialogueBlueprint, false));
			}

			if (ExistingInstance != nullptr && ExistingInstance->GetDialogueAsset() == nullptr)
			{
				ExistingInstance->InitDialogueEditor(EToolkitMode::Standalone, EditWithinLevelEditor, DialogueBlueprint);
				bFoundExisting = true;
			}

			if (!bFoundExisting)
			{
				FNarrativeDialogueEditorModule& DialogueEditorModule = FModuleManager::GetModuleChecked<FNarrativeDialogueEditorModule>("NarrativeDialogueEditor");
				TSharedRef<IDialogueEditor> NewEditor = DialogueEditorModule.CreateDialogueEditor(EToolkitMode::Standalone, EditWithinLevelEditor, DialogueBlueprint);
			}
		}

	}
}

uint32 FAssetTypeActions_DialogueAsset::GetCategories()
{
	return Category;
}
