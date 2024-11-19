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
	CameraMain = FindCameraByTag(TEXT("CameraMain"));

	AActor* CameraBoundsActor = FindCameraBoundsActor();
	if(CameraBoundsActor != nullptr)
	{
		InitCameraBounds(CameraBoundsActor);
	}

	InitCameraZoomParameters();
}

void UCameraWorldSubsystem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	TickUpdateCameraZoom(DeltaTime);
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

void UCameraWorldSubsystem::TickUpdateCameraZoom(float DeltaTime)
{
	if(CameraMain == nullptr) return;
	float GreatestDistanceBetweenTargets = CalculateGreatestDistanceBetweenTargets();

	float DistancePercentage = FMath::Clamp(FMath::GetRangePct(CameraZoomDistanceBetweenTargetsMin, CameraZoomDistanceBetweenTargetsMax, GreatestDistanceBetweenTargets), 0, 1);
	//GetRangePct() = InverseLerp()

	//GEngine->AddOnScreenDebugMessage(-1, 0.001f, FColor::White, FString::Printf(TEXT("Min = %f"), CameraZoomDistanceBetweenTargetsMin));
	//GEngine->AddOnScreenDebugMessage(-1, 0.001f, FColor::Red, FString::Printf(TEXT("Current = %f"), GreatestDistanceBetweenTargets));
	//GEngine->AddOnScreenDebugMessage(-1, 0.001f, FColor::Green, FString::Printf(TEXT("Percentage = %f"), DistancePercentage));
	//GEngine->AddOnScreenDebugMessage(-1, 0.001f, FColor::White, FString::Printf(TEXT("Max = %f"), CameraZoomDistanceBetweenTargetsMax));
	float ZoomY = FMath::Lerp(CameraZoomYMin, CameraZoomYMax, DistancePercentage);
	CameraMain->SetWorldLocation(FVector(CameraMain->GetComponentLocation().X, ZoomY, CameraMain->GetComponentLocation().Z));
}

void UCameraWorldSubsystem::TickUpdateCameraPosition(float DeltaTime)
{
	FVector AveragePos = CalculateAveragePositionBetweenTargets();
	
	ClampPositionIntoCameraBounds(AveragePos);
	CameraMain->SetWorldLocation(AveragePos);
}

FVector UCameraWorldSubsystem::CalculateAveragePositionBetweenTargets()
{
	float AverageX = 0;
	float AverageZ = 0;
	for(UObject* FollowTarget : FollowTargets)
	{
		ICameraFollowTarget* IFollowTarget = Cast<ICameraFollowTarget>(FollowTarget);
		if(IFollowTarget != nullptr)
		{
			
			if(IFollowTarget->IsFollowable())
			{
				
				AverageX += IFollowTarget->GetFollowPosition().X;
				AverageZ += IFollowTarget->GetFollowPosition().Z;
			}
		}
	}
	
	AverageX /= FollowTargets.Num();
	AverageZ /= FollowTargets.Num();
	return FVector(AverageX, CameraMain->GetComponentLocation().Y, AverageZ);
}

float UCameraWorldSubsystem::CalculateGreatestDistanceBetweenTargets()
{
	float GreatestDistance = 0;
	for(int i = 0; i < FollowTargets.Num(); i++)
	{
		AActor* FirstTargetActor = Cast<AActor>(FollowTargets[i]);
		for(int j = 0; j < FollowTargets.Num(); j++)
		{
			
			AActor* SecondTargetActor = Cast<AActor>(FollowTargets[j]);
			float DistanceBetweenTargets = FVector::Dist(Cast<AActor>(FollowTargets[i])->GetActorLocation(), Cast<AActor>(FollowTargets[j])->GetActorLocation());
			if(DistanceBetweenTargets > GreatestDistance)
			{
				GreatestDistance = DistanceBetweenTargets;
			}
		}
	}
	return GreatestDistance;
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

AActor* UCameraWorldSubsystem::FindCameraBoundsActor()
{
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsWithTag(GetWorld(), "CameraBounds", FoundActors);
	if(FoundActors.Num() == 0) return nullptr;
	else
	{
		//GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, FString::Printf(TEXT("Found")));
		return FoundActors[0];
	}
	
}

void UCameraWorldSubsystem::InitCameraBounds(AActor* CameraBoundsActor)
{
	FVector BoundsCenter;
	FVector BoundsExtents;
	CameraBoundsActor->GetActorBounds(false, BoundsCenter, BoundsExtents);
	
	CameraBoundsYProjectionCenter = BoundsCenter.Y;
	CameraBoundsMin = FVector2D(BoundsCenter.X - BoundsExtents.X, BoundsCenter.Z - BoundsExtents.Z);
	CameraBoundsMax = FVector2D(BoundsCenter.X + BoundsExtents.X, BoundsCenter.Z + BoundsExtents.Z);
}

void UCameraWorldSubsystem::ClampPositionIntoCameraBounds(FVector& Position)
{
	FVector2D ViewportBoundsMin, ViewportBoundsMax;
	GetViewportBounds(ViewportBoundsMin, ViewportBoundsMax);

	FVector WorldBoundsMin = CalculateWorldPositionFromViewportPosition(ViewportBoundsMin);
	FVector WorldBoundsMax = CalculateWorldPositionFromViewportPosition(ViewportBoundsMax);

	float HeightWorldBounds = FMath::Abs(WorldBoundsMax.Z - WorldBoundsMin.Z);
	float WidthWorldBounds = FMath::Abs(WorldBoundsMax.X - WorldBoundsMin.X);

	float BoundsMinX = CameraBoundsMin.X + WidthWorldBounds / 2;
	float BoundsMaxX = CameraBoundsMax.X - WidthWorldBounds / 2;

	if(Position.X < BoundsMinX) Position.X = BoundsMinX;
	else if(Position.X > BoundsMaxX) Position.X = BoundsMaxX;
	
	float BoundsMinZ = CameraBoundsMin.Y + HeightWorldBounds / 2;
	float BoundsMaxZ = CameraBoundsMax.Y - HeightWorldBounds / 2;

	if(Position.Z > BoundsMaxZ) Position.Z = BoundsMaxZ;
	else if(Position.Z < BoundsMinZ) Position.Z = BoundsMinZ;

	//Debug
	//GEngine->AddOnScreenDebugMessage(-1, 0.001f, FColor::White, FString::Printf(TEXT("Ymin = %f"), BoundsMinZ));
	//GEngine->AddOnScreenDebugMessage(-1, 0.001f, FColor::White, FString::Printf(TEXT("PosX = %f"), Position.Z));
	//GEngine->AddOnScreenDebugMessage(-1, 0.001f, FColor::White, FString::Printf(TEXT("Ymax = %f"), BoundsMaxZ));
}

void UCameraWorldSubsystem::GetViewportBounds(FVector2D& OutViewportBoundsMin, FVector2D& OutViewportBoundsMax)
{
	UGameViewportClient* ViewportClient = GetWorld()->GetGameViewport();
	if(ViewportClient == nullptr) return;

	FViewport* Viewport = ViewportClient->Viewport;
	if(Viewport == nullptr) return;

	FIntRect ViewRect(
		Viewport->GetInitialPositionXY(),
		Viewport->GetInitialPositionXY() + Viewport->GetSizeXY()
	);
	FIntRect ViewportRect = Viewport->CalculateViewExtents(CameraMain->AspectRatio, ViewRect);

	OutViewportBoundsMin.X = ViewportRect.Min.X;
	OutViewportBoundsMin.Y = ViewportRect.Min.Y;

	OutViewportBoundsMax.X = ViewportRect.Max.X;
	OutViewportBoundsMax.Y = ViewportRect.Max.Y;
}

FVector UCameraWorldSubsystem::CalculateWorldPositionFromViewportPosition(const FVector2D& ViewportPosition)
{
	if(CameraMain == nullptr) return FVector::Zero();

	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if(PlayerController == nullptr) return FVector::Zero();

	float VDistanceToCenter = CameraMain->GetOwner()->GetActorLocation().Y - CameraBoundsYProjectionCenter;

	FVector CameraWorldProjectDir;
	FVector WorldPosition;
	UGameplayStatics::DeprojectScreenToWorld(
		PlayerController,
		ViewportPosition,
		WorldPosition,
		CameraWorldProjectDir
	);

	WorldPosition += CameraWorldProjectDir * VDistanceToCenter;

	return WorldPosition;
}

void UCameraWorldSubsystem::InitCameraZoomParameters()
{
	TArray<AActor*> CamerasDistMin;
	UGameplayStatics::GetAllActorsWithTag(GetWorld(), "CameraDistanceMin", CamerasDistMin);
	if(CamerasDistMin[0] == nullptr) return;
	CameraZoomYMin = CamerasDistMin[0]->GetActorLocation().Y;
	
	
	TArray<AActor*> CamerasDistMax;
	UGameplayStatics::GetAllActorsWithTag(GetWorld(), "CameraDistanceMax", CamerasDistMax);
	if(CamerasDistMax[0] == nullptr) return;
	CameraZoomYMax = CamerasDistMax[0]->GetActorLocation().Y;
	
	
}


