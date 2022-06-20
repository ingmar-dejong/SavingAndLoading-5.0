// Copyright Narrative Tools 2022. 

#include "DialogueGraphNode_NPC.h"
#include "DialogueGraph.h"
#include "EdGraph/EdGraphSchema.h"
#include "DialogueEditorTypes.h"
#include "DialogueGraphNode.h"
#include "DialogueSM.h"
#include "Dialogue.h"
#include "DialogueEditorSettings.h"

void UDialogueGraphNode_NPC::AllocateDefaultPins()
{
	CreatePin(EGPD_Input, UDialogueEditorTypes::PinCategory_SingleNode, TEXT(""));
	CreatePin(EGPD_Output, UDialogueEditorTypes::PinCategory_SingleNode, TEXT(""));
}

FLinearColor UDialogueGraphNode_NPC::GetGraphNodeColor() const
{
	if (UDialogueNode_NPC* NPCNode = Cast<UDialogueNode_NPC>(DialogueNode))
	{
		if (UDialogue* OwningDialogue = NPCNode->OwningDialogue)
		{
			FSpeakerInfo SpeakerInfo = OwningDialogue->GetSpeaker(NPCNode->SpeakerID);

			return SpeakerInfo.NodeColor;
		}
	}

	return GetDefault<UDialogueEditorSettings>()->NPCNodeColor;
}
