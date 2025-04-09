#pragma once

#include "CoreMinimal.h"
#include "Representation/GeneratedProperty.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType.h"

#include "Save/Saveable.h"

#include "BlackboardKeyGeneratedProperty.generated.h"

class UGeneratedProperty;

class URandomGen;

/**
 * @file BlackboardKeyGeneratedProperty.h
 */

/**
 * @class BlackboardKeyGeneratedProperty
 * @author npcdev
 * @date 11/13/19
 * @brief A class representing a blackboard key property of a generated node.
 */
UCLASS( Blueprintable, BlueprintType, EditInlineNew, DefaultToInstanced )
class EVOLVINGBEHAVIOR_API UBlackboardKeyGeneratedProperty : public UGeneratedProperty, public ISaveable
{
    GENERATED_BODY()
    
private:
    UPROPERTY( SaveGame )
    FName keyName;
    
    UPROPERTY( EditAnywhere, Category = "EvolvingBehavior")
    TSubclassOf<UBlackboardKeyType> keyType;

    UPROPERTY( EditAnywhere, Category = "EvolvingBehavior")
    TSoftObjectPtr<UBlackboardData> blackboard;

    UPROPERTY( SaveGame)
    FName blackboardPath;

    UPROPERTY( SaveGame)
    FString savedKeyTypePath;

public:
    virtual void InitMutate( URandomGen* rand ) override;
    
    virtual void Mutate( URandomGen* rand, double stdDevFractionOfRange = 0.25 ) override;

    void SetKeyName(FName newKeyName) { keyName = newKeyName; }

    FName GetKeyName() { return keyName; }

    TSubclassOf<UBlackboardKeyType> GetKeyType() { return keyType; }
    
    virtual bool SetOn(UObject* object) override;
    
    virtual FString ToString() override
    { 
        return FString::Printf(TEXT("%s [%s]"), *UGeneratedProperty::ToString(), *(GetKeyName().ToString()));
    }
    
    void OnBlackboardLoaded();

    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Saveable Actor")
    void DataLoaded();
    virtual void DataLoaded_Implementation() override;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Saveable Actor")
    void DataSaved();
    virtual void DataSaved_Implementation() override;
};
