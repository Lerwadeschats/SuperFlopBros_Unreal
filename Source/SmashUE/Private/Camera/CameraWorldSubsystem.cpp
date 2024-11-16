// Fill out your copyright notice in the Description page of Project Settings.


#include "Camera/CameraWorldSubsystem.h"

#include "Camera/CameraFollowTarget.h"
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

void UCameraWorldSubsystem::AddFollowTarget(UObject* FollowTarget)
{
	FollowTargets.Add(FollowTarget);
}

void UCameraWorldSubsystem::RemoveFollowTarget(UObject* FollowTarget)
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
	for(UObject* FollowTarget : FollowTargets)
	{
		ICameraFollowTarget* IFollowTarget = Cast<ICameraFollowTarget>(FollowTarget);
		if(IFollowTarget != nullptr)
		{
			if(IFollowTarget->IsFollowable())
			{
				AverageX += IFollowTarget->GetFollowPosition().X;
				//AverageY += FollowTarget->GetActorLocation().Y;
				AverageZ += IFollowTarget->GetFollowPosition().Z;
			}
			
		}
		
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
