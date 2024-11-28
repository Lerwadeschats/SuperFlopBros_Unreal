// Fill out your copyright notice in the Description page of Project Settings.


#include "LocalMultiplayerSettings.h"

bool FLocalMultiplayerProfileData::ContainsKey(const FKey& Key, ELocalMultiplayerInputMappingType MappingType) const
{
	UInputMappingContext* IMCFound = GetIMCFromType(MappingType);
	if(IMCFound != nullptr)
	{
		for(FEnhancedActionKeyMapping Input : IMCFound->GetMappings())
		{
			if(Input.Key == Key)
			{
				return true;
			}
		}
		return false;
	}
	return false;
}

UInputMappingContext* FLocalMultiplayerProfileData::GetIMCFromType(ELocalMultiplayerInputMappingType MappingType) const
{
	switch(MappingType)
	{
	case ELocalMultiplayerInputMappingType::Menu:
		return IMCMenu;
	case ELocalMultiplayerInputMappingType::InGame:
		return IMCInGame;
	}
	return nullptr;
}

int ULocalMultiplayerSettings::GetNbKeyboardProfiles() const
{
	return KeyboardProfilesData.Num();
}

int ULocalMultiplayerSettings::FindKeyboardProfileIndexFromKey(const FKey& Key,
	ELocalMultiplayerInputMappingType MappingType) const
{
	for (int i = 0;  i < GetNbKeyboardProfiles(); ++i)
	{
		if(KeyboardProfilesData[i].ContainsKey(Key, MappingType))
		{
			return i;
		}
	}
	return -1;
}
