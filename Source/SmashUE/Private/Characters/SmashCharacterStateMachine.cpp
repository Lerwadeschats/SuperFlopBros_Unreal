// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/SmashCharacterStateMachine.h"

#include "SmashCharacterState.h"
#include "Characters/SmashCharacter.h"
#include "Characters/SmashCharacterStateID.h"

void USmashCharacterStateMachine::Init(ASmashCharacter* InCharacter)
{
	Character = InCharacter;
	FindStates();
	InitStates();
}

ASmashCharacter* USmashCharacterStateMachine::GetCharacter() const
{
	return Character;
}

void USmashCharacterStateMachine::FindStates()
{
	TArray<UActorComponent*> FoundComponents = Character->K2_GetComponentsByClass(USmashCharacterState::StaticClass());
	for (UActorComponent* StateComponent : FoundComponents)
	{
		USmashCharacterState* State = Cast<USmashCharacterState>(StateComponent);
		if(State == nullptr) continue;
		if(State->GetStateID() == ESmashCharacterStateID::None) continue;

		AllStates.Add(State);
	}
}

void USmashCharacterStateMachine::InitStates()
{
	for (USmashCharacterState* State : AllStates)
	{
		State->StateInit(this);
	}
}

