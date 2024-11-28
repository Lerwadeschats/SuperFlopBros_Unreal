// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/States/SmashCharacterStateJump.h"

#include "Characters/SmashCharacter.h"
#include "Characters/SmashCharacterStateID.h"
#include "Characters/SmashCharacterStateMachine.h"
#include "GameFramework/CharacterMovementComponent.h"


ESmashCharacterStateID USmashCharacterStateJump::GetStateID()
{
	return ESmashCharacterStateID::Jump;
}

void USmashCharacterStateJump::StateEnter(ESmashCharacterStateID PreviousStateID)
{
	Super::StateEnter(PreviousStateID);
	if(Character->GetVelocity().Z == 0)
	{
		Character->PlayAnimMontage(Anim);
	}
	
	
	OnJump();
}

void USmashCharacterStateJump::StateExit(ESmashCharacterStateID NextStateID)
{
	Super::StateExit(NextStateID);
	
	Character->StopJumping();
}

void USmashCharacterStateJump::StateTick(float DeltaTime)
{
	Super::StateTick(DeltaTime);
	if(Character->GetVelocity().Z < 0)
	{
		StateMachine->ChangeState(ESmashCharacterStateID::Fall);
	}
	if(FMath::Abs(Character->GetInputMoveX()) > InputMoveXThreshold)
	{
		Character->SetOrientX(Character->GetInputMoveX());
		Character->GetCharacterMovement()->AirControl = JumpAirControl;
		Character->Move(JumpWalkSpeed);
	}
	
}

void USmashCharacterStateJump::OnJump()
{
	float t = JumpDuration/2;
	float h = JumpMaxHeight;
	Character->GetCharacterMovement()->JumpZVelocity = 2 * h / t;
	Character->GetCharacterMovement()->GravityScale = (-2 * h / (t * t)) / -981;
	
	Character->Jump();
}



