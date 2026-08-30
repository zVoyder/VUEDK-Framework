// Copyright VUEDK, Inc. All Rights Reserved.

#include "UK2Node_CallEvent.h"
#include "BlueprintActionDatabaseRegistrar.h"
#include "BlueprintNodeSpawner.h"
#include "GameplayTagContainer.h"
#include "K2Node_CallFunction.h"
#include "KismetCompiler.h"
#include "Utility/EventsDispatcherUtility.h"

FText UUK2Node_CallEvent::GetNodeTitle(ENodeTitleType::Type TitleType) const
{
	return FText::FromString(TEXT("Call Event"));
}

FText UUK2Node_CallEvent::GetTooltipText() const
{
	return FText::FromString(TEXT("Calls an event in the Events Dispatcher system."));
}

FText UUK2Node_CallEvent::GetMenuCategory() const
{
	return FText::FromString(TEXT("Events Dispatcher"));
}

void UUK2Node_CallEvent::GetMenuActions(FBlueprintActionDatabaseRegistrar& ActionRegistrar) const
{
	Super::GetMenuActions(ActionRegistrar);
	const UClass* Action = GetClass();
	if (ActionRegistrar.IsOpenForRegistration(Action))
	{
		UBlueprintNodeSpawner* Spawner = UBlueprintNodeSpawner::Create(GetClass());
		ActionRegistrar.AddBlueprintAction(Action, Spawner);
	}
}

void UUK2Node_CallEvent::AllocateDefaultPins()
{
	CreatePin(EGPD_Input, UEdGraphSchema_K2::PC_Exec, UEdGraphSchema_K2::PN_Execute);
	CreatePin(EGPD_Output, UEdGraphSchema_K2::PC_Exec, UEdGraphSchema_K2::PN_Then);
	UEdGraphPin* EventTagPin = CreatePin(EGPD_Input, UEdGraphSchema_K2::PC_Struct, FGameplayTag::StaticStruct(), TEXT("EventTag"));
	EventTagPin->PinToolTip = TEXT("The gameplay tag identifying the event to call.");
	
	UEdGraphPin* PayloadPin = CreatePin(EGPD_Input, UEdGraphSchema_K2::PC_Object, UObject::StaticClass(), TEXT("Payload"));
	PayloadPin->PinToolTip = TEXT("Optional payload object to send with the event.");
	PayloadPin->bAdvancedView = true;
	
	UEdGraphPin* StrictDispatchPin = CreatePin(EGPD_Input, UEdGraphSchema_K2::PC_Boolean, TEXT("bStrictDispatch"));
	StrictDispatchPin->PinToolTip =
		TEXT("When true, only listeners matching the presence or absence of a payload will be called.\n"
			 "When false, all listeners for the event tag are called regardless of payload.");
	StrictDispatchPin->bAdvancedView = true;
	
	AdvancedPinDisplay = ENodeAdvancedPins::Type::Hidden;
}

void UUK2Node_CallEvent::ExpandNode(FKismetCompilerContext& CompilerContext, UEdGraph* SourceGraph)
{
	Super::ExpandNode(CompilerContext, SourceGraph);

	UEdGraphPin* ExecPin = GetExecPin();
	UEdGraphPin* ThenPin = GetThenPin();
	UEdGraphPin* EventTagPin = FindPin(TEXT("EventTag"));
	UEdGraphPin* PayloadPin = FindPin(TEXT("Payload"));
	UEdGraphPin* StrictDispatchPin = FindPin(TEXT("bStrictDispatch"));

	const bool bPayloadProvided = PayloadPin->LinkedTo.Num() > 0;
	UK2Node_CallFunction* CallFuncNode = CompilerContext.SpawnIntermediateNode<UK2Node_CallFunction>(this, SourceGraph);
	CallFuncNode->FunctionReference.SetExternalMember(TEXT("CallEvent"), UEventsDispatcherUtility::StaticClass());
	CallFuncNode->AllocateDefaultPins();

	CompilerContext.MovePinLinksToIntermediate(*ExecPin, *CallFuncNode->GetExecPin());
	CompilerContext.MovePinLinksToIntermediate(*EventTagPin, *CallFuncNode->FindPinChecked(TEXT("EventTag")));

	UEdGraphPin* UsePayloadRuntimePin = CallFuncNode->FindPinChecked(TEXT("bUsePayload"));
	UsePayloadRuntimePin->DefaultValue = bPayloadProvided ? TEXT("true") : TEXT("false");

	CompilerContext.MovePinLinksToIntermediate(*PayloadPin, *CallFuncNode->FindPinChecked(TEXT("Payload")));
	CompilerContext.MovePinLinksToIntermediate(*StrictDispatchPin, *CallFuncNode->FindPinChecked(TEXT("bStrictDispatch")));
	CompilerContext.MovePinLinksToIntermediate(*ThenPin, *CallFuncNode->GetThenPin());
	BreakAllNodeLinks();
}
