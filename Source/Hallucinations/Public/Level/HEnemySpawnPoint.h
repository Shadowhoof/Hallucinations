#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "HEnemySpawnPoint.generated.h"

class UArrowComponent;
class AHNonPlayerCharacter;


UCLASS()
class HALLUCINATIONS_API AHEnemySpawnPoint : public AActor
{
	GENERATED_BODY()

public:

	AHEnemySpawnPoint();

	TSubclassOf<AHNonPlayerCharacter> GetRandomEnemyType() const;

	void GetSpawnableEnemyTypes(TArray<TSubclassOf<AHNonPlayerCharacter>>& OutTypes);

	int32 GetRandomEnemyCount() const;

	int32 GetMinEnemyCount() const;

	int32 GetMaxEnemyCount() const;

	bool CanSpawnElites() const;

	float GetSpawnRadius() const;
	
protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Visual")
	UArrowComponent* ArrowComponent;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawn")
	TArray<TSubclassOf<AHNonPlayerCharacter>> EnemyTypes;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawn")
	int32 MinEnemyCount = 4;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawn")
	int32 MaxEnemyCount = 6;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawn")
	bool bCanSpawnElites = true;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawn")
	float SpawnRadius = 300.f;
};
