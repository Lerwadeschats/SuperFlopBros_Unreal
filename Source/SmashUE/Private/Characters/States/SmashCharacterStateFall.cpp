// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/States/SmashCharacterStateFall.h"

#include "Characters/SmashCharacter.h"
#include "Characters/SmashCharacterStateID.h"
#include "Characters/SmashCharacterStateMachine.h"
#include "GameFramework/CharacterMovementComponent.h"


ESmashCharacterStateID USmashCharacterStateFall::GetStateID()
{
	return ESmashCharacterStateID::Fall;
}

void USmashCharacterStateFall::StateEnter(ESmashCharacterStateID PreviousStateID)
{
	Super::StateEnter(PreviousStateID);
	Character->PlayAnimMontage(Anim);

	if(FMath::Abs(Character->GetInputFall()) > 0.1f)
	{
		GEngine->AddOnScreenDebugMessage(-1, 0.1f, FColor::Red, FString::Printf(TEXT("Input fall value: %f"), Character->GetInputFall()));

		Character->GetCharacterMovement()->GravityScale = FallFastGravityScale;
	}
	else
	{
		Character->GetCharacterMovement()->GravityScale = FallGravityScale;
	}
	
}

void USmashCharacterStateFall::StateExit(ESmashCharacterStateID NextStateID)
{
	Super::StateExit(NextStateID);
	Character->GetCharacterMovement()->GravityScale = 1;
	
}

void USmashCharacterStateFall::StateTick(float DeltaTime)
{
	Super::StateTick(DeltaTime);
	
	if(Character->GetVelocity().Z == 0)
	{
		StateMachine->ChangeState(ESmashCharacterStateID::Idle);
	}
	else if(FMath::Abs(Character->GetInputMoveX()) > InputMoveXThreshold)
	{
		Character->SetOrientX(Character->GetInputMoveX());
		Character->GetCharacterMovement()->AirControl = FallAirControl;
		Character->Move(FallHorizontalMoveSpeed);
	}
	
	
	
}