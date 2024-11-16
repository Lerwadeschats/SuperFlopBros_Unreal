// Fill out your copyright notice in the Description page of Project Settings.


#include "Camera/CameraWorldSubsystem.h"
#include "Kismet/GameplayStatics.h"

void UCameraWorldSubsystem::PostInitialize()
{
	Super::PostInitialize();
}

void UCameraWorldSubsystem::OnWorldBeginPlay(UWorld& InWorld)
{
	Super::OnWorldBeginPlay(InWorld);
	CameraMain = FindCameraByTag(TEXT("CameraMain"));
}

void UCameraWorldSubsystem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	TickUpdateCameraPosition(DeltaTime);
}

void UCameraWorldSubsystem::AddFollowTarget(AActor* FollowTarget)
{
	FollowTargets.Add(FollowTarget);
}

void UCameraWorldSubsystem::RemoveFollowTarget(AActor* FollowTarget)
{
	FollowTargets.Remove(FollowTarget);
}

void UCameraWorldSubsystem::TickUpdateCameraPosition(float DeltaTime)
{
	CameraMain->SetWorldLocation(CalculateAveragePositionBetweenTargets());
}

FVector UCameraWorldSubsystem::CalculateAveragePositionBetweenTargets()
{
	float AverageX = CameraMain->GetComponentLocation().X;
	//float AverageY = 0;
	float AverageZ = CameraMain->GetComponentLocation().Z;
	for(AActor* FollowTarget : FollowTargets)
	{
		AverageX += FollowTarget->GetActorLocation().X;
		//AverageY += FollowTarget->GetActorLocation().Y;
		AverageZ += FollowTarget->GetActorLocation().Z;
	}
	AverageX /= FollowTargets.Num();
//	AverageY /= FollowTargets.Num();
	AverageZ /= FollowTargets.Num();
	return FVector(AverageX, CameraMain->GetComponentLocation().Y, AverageZ);
}

UCameraComponent* UCameraWorldSubsystem::FindCameraByTag(const FName& Tag) const
{
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsWithTag(GetWorld(), Tag, FoundActors);
	for(AActor* Actor : FoundActors)
	{
		UCameraComponent* CameraComponent = Actor->GetComponentByClass<UCameraComponent>();
		if(CameraComponent == nullptr) continue;
		else return CameraComponent;
	}
	return nullptr;
}
