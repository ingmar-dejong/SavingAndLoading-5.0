// Copyright Narrative Tools 2022. 

#include "DialogueGraphNode_Player.h"
#include "DialogueGraph.h"
#include "EdGraph/EdGraphSchema.h"
#include "DialogueEditorTypes.h"
#include "DialogueSM.h"
#include "Dialogue.h"
#include "DialogueGraphNode.h"
#include "DialogueEditorSettings.h"

void UDialogueGraphNode_Player::AllocateDefaultPins()
{
	CreatePin(EGPD_Input, UDialogueEditorTypes::PinCategory_SingleNode, TEXT(""));
	CreatePin(EGPD_Output, UDialogueEditorTypes::PinCategory_SingleNode, TEXT(""));
}

FLinearColor UDialogueGraphNode_Player::GetGraphNodeColor() const
{
	return GetDefault<UDialogueEditorSettings>()->PlayerNodeColor;
}

