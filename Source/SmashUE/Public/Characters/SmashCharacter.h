

#pragma once

#include "CoreMinimal.h"
#include "InputActionValue.h"
#include "GameFramework/Character.h"
#include "SmashCharacter.generated.h"

class USmashCharacterInputData;
class UInputMappingContext;
class USmashCharacterStateMachine;


UCLASS()
class SMASHUE_API ASmashCharacter : public ACharacter
{
	GENERATED_BODY()
	
#pragma region Unreal Default
	
public:
	// Sets default values for this character's properties
	ASmashCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

#pragma endregion

#pragma region Orient

public:
	float GetOrientX() const;

	void SetOrientX(float NewOrientX);

protected:
	UPROPERTY(BlueprintReadOnly)
	float OrientX = 1.f;

	void RotateMeshUsingOrientX() const;

#pragma endregion

#pragma region State Machine

public:
	void CreateStateMachine();

	void InitStateMachine();

	void TickStateMachine(float DeltaTime) const;

	void UpdateAnimation(UAnimMontage* Anim);

	void Move(float Speed);
	

protected:
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<USmashCharacterStateMachine> StateMachine;
	
	
	
#pragma endregion

#pragma region Input Data / Mapping Context

public:
	UPROPERTY()
	TObjectPtr<UInputMappingContext> InputMappingContext;

	UPROPERTY()
	TObjectPtr<USmashCharacterInputData> InputData;

	

protected:
	void SetupMappingContextIntoController() const;


#pragma endregion

#pragma region Input Move X

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FInputMoveXEvent, float, InputMoveX);
	
public:
	float GetInputMoveX() const;
	float GetInputFall() const;

	UPROPERTY()
	FInputMoveXEvent InputMoveXFastEvent;
protected:
	UPROPERTY()
	float InputMoveX = 0.f;

	UPROPERTY()
	float InputFall = 0.f;

private:
	void OnInputMoveX(const FInputActionValue& InputActionValue);
	void OnInputMoveXFast(const FInputActionValue& InputActionValue);
	void OnInputFallFast(const FInputActionValue& InputActionValue);
	void BindInputMoveXAxisAndActions(UEnhancedInputComponent* EnhancedInputComponent);
	
#pragma endregion

#pragma region Jump / Fall
	
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FInputJumpXEvent, float, InputJump);
	
private:
	void OnInputJump(const FInputActionValue& InputActionValue);
	
#pragma endregion
};
