

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "InputMappingContext.h"
#include "Characters//SmashCharacterInputData.h"
#include "MatchGameMode.generated.h"




class AArenaPlayerStart;
class ASmashCharacter;


/**
 * 
 */
UCLASS()
class SMASHUE_API AMatchGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;

protected:
	UPROPERTY()
	TArray<ASmashCharacter*> CharactersInsideArena;

	
private:
	USmashCharacterInputData* LoadInputDataFromConfig();

	UInputMappingContext* LoadInputMappingContextFromConfig();
	
	void FindPlayerStartActorsInArena(TArray<AArenaPlayerStart*>& ResultsActors);

	void SpawnCharacters(const TArray<AArenaPlayerStart*>& SpawnPoints);
	
	TSubclassOf<ASmashCharacter> GetSmashCharacterClassFromInputType(EAutoReceiveInput::Type InputType) const;
};
