// Fill out your copyright notice in the Description page of Project Settings.


#include "LocalMultiplayerSubsystem.h"

#include "EnhancedInputSubsystems.h"

void ULocalMultiplayerSubsystem::CreateAndInitPlayers(ELocalMultiplayerInputMappingType MappingType)
{
	const ULocalMultiplayerSettings* MultiplayerSettings = GetDefault<ULocalMultiplayerSettings>();
	for(int i = 0; i < MultiplayerSettings->KeyboardProfilesData.Num(); i++)
	{
		AssignKeyboardMapping(AssignNewPlayerToKeyboardProfile(i), i, MappingType);
	}
}

int ULocalMultiplayerSubsystem::GetAssignedPlayerIndexFromKeyboardProfileIndex(int KeyboardProfileIndex)
{
	int* PlayerIndex = PlayerIndexFromKeyboardProfileIndex.Find(KeyboardProfileIndex);
	if(PlayerIndex == nullptr) return -1;
	return *PlayerIndex;
}

int ULocalMultiplayerSubsystem::AssignNewPlayerToKeyboardProfile(int KeyboardProfileIndex)
{
	int TempLastPlayerIndex = LastAssignedPlayerIndex;
	if(TempLastPlayerIndex >= GetGameInstance()->GetLocalPlayers().Num()) return -1;
	PlayerIndexFromKeyboardProfileIndex[KeyboardProfileIndex] = TempLastPlayerIndex;
	LastAssignedPlayerIndex++;
	return TempLastPlayerIndex;
}

void ULocalMultiplayerSubsystem::AssignKeyboardMapping(int PlayerIndex, int KeyboardProfileIndex,
	ELocalMultiplayerInputMappingType MappingType) const
{
	const ULocalMultiplayerSettings* MultiplayerSettings = GetDefault<ULocalMultiplayerSettings>();
	if(MultiplayerSettings == nullptr) return;
	//=> Get player controller with an index => GetGameInstance()->GetLocalPlayers()[PlayerIndex]->PlayerController

	UInputMappingContext* PlayerIMC = MultiplayerSettings->KeyboardProfilesData[KeyboardProfileIndex].GetIMCFromType(MappingType);
	if(PlayerIMC == nullptr) return;
	GetGameInstance()->GetLocalPlayers()[PlayerIndex]->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>()->AddMappingContext(PlayerIMC, 1);
}

int ULocalMultiplayerSubsystem::GetAssignedPlayerIndexFromGamepadDeviceID(int DeviceID)
{
	int* PlayerIndex = PlayerIndexFromGamepadProfileIndex.Find(DeviceID);
	if(PlayerIndex == nullptr) return -1;
	return *PlayerIndex;
}

int ULocalMultiplayerSubsystem::AssignNewPlayerToGamepadDeviceID(int DeviceID)
{
	int TempLastPlayerIndex = LastAssignedPlayerIndex;
	if(TempLastPlayerIndex >= GetGameInstance()->GetLocalPlayers().Num()) return -1;
	PlayerIndexFromGamepadProfileIndex[DeviceID] = TempLastPlayerIndex;
	LastAssignedPlayerIndex++;
	return TempLastPlayerIndex;
}

void ULocalMultiplayerSubsystem::AssignGamepadDeviceID(int PlayerIndex,
	ELocalMultiplayerInputMappingType MappingType) const
{
	const ULocalMultiplayerSettings* MultiplayerSettings = GetDefault<ULocalMultiplayerSettings>();
	if(MultiplayerSettings == nullptr) return;
	
	
	UInputMappingContext* IMC = MultiplayerSettings->GamepadProfileData.GetIMCFromType(MappingType);
	if(IMC == nullptr)return;
	GetGameInstance()->GetLocalPlayers()[PlayerIndex]->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>()->AddMappingContext(IMC, 1);
}
