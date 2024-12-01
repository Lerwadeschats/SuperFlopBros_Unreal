// Fill out your copyright notice in the Description page of Project Settings.


#include "LocalMultiplayerGameViewportClient.h"

#include "LocalMultiplayerSubsystem.h"
#include "GameFramework/PlayerInput.h"

class ULocalMultiplayerSubsystem;

void ULocalMultiplayerGameViewportClient::PostInitProperties()
{
	Super::PostInitProperties();
	MaxSplitscreenPlayers = 8;
}

bool ULocalMultiplayerGameViewportClient::InputKey(const FInputKeyEventArgs& EventArgs)
{
	ULocalMultiplayerSubsystem* MultiplayerSubsystem = GetGameInstance()->GetSubsystem<ULocalMultiplayerSubsystem>();
	const ULocalMultiplayerSettings* MultiplayerSettings = GetDefault<ULocalMultiplayerSettings>();
	int PlayerIndex = -1;
	if(EventArgs.IsGamepad()) 
	{
		PlayerIndex = MultiplayerSubsystem->GetAssignedPlayerIndexFromGamepadDeviceID(EventArgs.InputDevice.GetId());
		if(PlayerIndex == -1)
		{
			PlayerIndex = MultiplayerSubsystem->AssignNewPlayerToGamepadDeviceID(EventArgs.InputDevice.GetId());
			if(PlayerIndex == -1) return Super::InputKey(EventArgs); //Return la version de la fonction du Viewport par défaut d'Unreal
		}
	}
	else
	{
		int KeyboardProfile = MultiplayerSettings->FindKeyboardProfileIndexFromKey(EventArgs.Key, ELocalMultiplayerInputMappingType::InGame);
		if(KeyboardProfile == -1) return Super::InputKey(EventArgs);
		PlayerIndex = MultiplayerSubsystem->GetAssignedPlayerIndexFromKeyboardProfileIndex(KeyboardProfile);
		if(PlayerIndex == -1)
		{
			PlayerIndex = MultiplayerSubsystem->AssignNewPlayerToKeyboardProfile(KeyboardProfile);
			if(PlayerIndex == -1) return Super::InputKey(EventArgs);
		}
	}
	APlayerController* PlayerController =  GetGameInstance()->GetLocalPlayers()[PlayerIndex]->PlayerController;
	if(PlayerController == nullptr) return Super::InputKey(EventArgs);
	
	return PlayerController->InputKey(FInputKeyParams(EventArgs.Key, EventArgs.Event, EventArgs.AmountDepressed, EventArgs.IsGamepad(), EventArgs.InputDevice));
	
}

bool ULocalMultiplayerGameViewportClient::InputAxis(FViewport* InViewport, FInputDeviceId InputDevice, FKey Key,
	float Delta, float DeltaTime, int32 NumSamples, bool bGamepad)
{
	ULocalMultiplayerSubsystem MultiplayerSubsystem = ULocalMultiplayerSubsystem();
	const ULocalMultiplayerSettings* MultiplayerSettings = GetDefault<ULocalMultiplayerSettings>();
	int PlayerIndex = -1;
	if(bGamepad)//IsGamepad = true
	{
		PlayerIndex = MultiplayerSubsystem.GetAssignedPlayerIndexFromGamepadDeviceID(InputDevice.GetId());
		if(PlayerIndex == -1)
		{
			PlayerIndex = MultiplayerSubsystem.AssignNewPlayerToGamepadDeviceID(InputDevice.GetId());
			if(PlayerIndex == -1) return Super::InputAxis(InViewport, InputDevice, Key, Delta, DeltaTime, NumSamples, bGamepad);
		}
	}
	else
	{
		int KeyboardProfile = MultiplayerSettings->FindKeyboardProfileIndexFromKey(Key, ELocalMultiplayerInputMappingType::InGame);
		if(KeyboardProfile == -1) return Super::InputAxis(InViewport, InputDevice, Key, Delta, DeltaTime, NumSamples, bGamepad);
		PlayerIndex = MultiplayerSubsystem.GetAssignedPlayerIndexFromKeyboardProfileIndex(KeyboardProfile);
		if(PlayerIndex == -1)
		{
			PlayerIndex = MultiplayerSubsystem.AssignNewPlayerToKeyboardProfile(KeyboardProfile);
			if(PlayerIndex == -1) return Super::InputAxis(InViewport, InputDevice, Key, Delta, DeltaTime, NumSamples, bGamepad);
		}
	}
	APlayerController* PlayerController =  GetGameInstance()->GetLocalPlayers()[PlayerIndex]->PlayerController;
	if(PlayerController == nullptr) return Super::InputAxis(InViewport, InputDevice, Key, Delta, DeltaTime, NumSamples, bGamepad);
	
	return PlayerController->InputKey(FInputKeyParams(Key, EInputEvent::IE_Axis,Delta , bGamepad, InputDevice));
}
