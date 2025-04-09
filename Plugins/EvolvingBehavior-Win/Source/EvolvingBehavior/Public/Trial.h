
#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Trial.generated.h"

class UBTChromosome;
class UServiceInjector;

/**
 * @file Trial.h
 */

/**
 * @class UTrial
 * @author agrundwerg
 * @date 12/12/17
 * @brief Tracks the population for a single generation/trial.
 */
UCLASS()
class EVOLVINGBEHAVIOR_API UTrial : public UObject
{
	GENERATED_BODY()
	
private:
	UPROPERTY(SaveGame)
	TMap<int32, UBTChromosome*> population;
	
	int32 numSpawned;

public:

	void Add(int32 id, UBTChromosome* bt);
	UBTChromosome* Get(int32 id);
	TArray<UBTChromosome*> GetChromosomes();
	void GetAllIDs( TArray<int32>& ids ) { population.GetKeys( ids ); }
	
	int32 GetNumMembers() { return population.Num(); }
	
	int32 GetNumSpawned() { return numSpawned; }
	
	int32 GetNumAvailable() { return GetNumMembers() - GetNumSpawned(); }
	
	void OnSpawned( int32 id ) { numSpawned++; }

	void OnReleased(int32 id) { numSpawned--; }

	void SetServiceInjector(UServiceInjector* serviceInjector);
};
