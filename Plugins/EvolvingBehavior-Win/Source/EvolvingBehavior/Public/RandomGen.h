#pragma once

#include <random>

#include "Service.h"
#include "Save/Saveable.h"
#include "Save/SaveEvolvingBehavior.h"

#include "RandomGen.generated.h"

/**
 * @file RandomGen.h
 */

/**
 * @class URandomGen
 * @author npc
 * @date 12/12/17
 * @brief Provides an easy wrapper for random number generation.
 */
UCLASS(CollapseCategories)
class EVOLVINGBEHAVIOR_API URandomGen : public UService, public ISaveable
{
    GENERATED_BODY()

private:

    std::mt19937 generator;
    
    UPROPERTY(EditAnywhere, SaveGame, Category = "EvolvingBehavior")
    uint32 seed;

    UPROPERTY(SaveGame)
    FString state;

public:
    void Seed() { generator.seed( seed ); }

    void Seed( int32 newSeed )
    {
		seed = newSeed;
		Seed();
    }

    bool WithProbability( double probability );

    void RegisterLifeCycle();

    void Save(USaveEvolvingBehavior* sg);
    void Load(USaveEvolvingBehavior* sg);

    int32_t UniformIntInRange( int32_t lower, int32_t upper );

    double UniformRealInRange( double lower, double upper );

    double GaussianReal(double mean, double stdDev);

    template<typename KeyT, typename ValueT>
    TPair<KeyT, ValueT>& RandomMapEntry(TMap<KeyT, ValueT> map)
	{
		int32 keyIdx = UniformIntInRange(0, map.Num() - 1);

		auto iterator = map.CreateIterator();
		for (int i = 0; i < keyIdx; ++i)
		{
			++iterator;
#ifdef DEBUG
			if (!iterator)
			{
				UE_LOG(LogEvolvingBehavior, Error, TEXT("Error: iterator not valid while trying to get random map entry!"));
			}
#endif
		}

		TPair<KeyT, ValueT>& result = *iterator;

		return result;
	}

    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Saveable Actor")
    void DataLoaded();
    virtual void DataLoaded_Implementation() override;

    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Saveable Actor")
    void DataSaved();
    virtual void DataSaved_Implementation() override;

};
