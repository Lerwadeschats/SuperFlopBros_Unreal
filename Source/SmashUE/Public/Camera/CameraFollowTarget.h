// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "CameraFollowTarget.generated.h"

// This class does not need to be modified.
UINTERFACE()
class UCameraFollowTarget : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class SMASHUE_API ICameraFollowTarget
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual FVector GetFollowPosition() = 0;

	virtual bool IsFollowable() = 0;
};
