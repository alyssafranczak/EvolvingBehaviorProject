#pragma once

#include "CoreMinimal.h"
#include "Representation/GeneratedProperty.h"

#include "IntGeneratedProperty.generated.h"

class UGeneratedProperty;

class URandomGen;

/**
 * @file IntGeneratedProperty.h
 */

/**
 * @class IntGeneratedProperty
 * @author jim
 * @date 11/13/19
 * @brief A class representing an integer property of a generated node.
 */
UCLASS( Blueprintable, BlueprintType, EditInlineNew, DefaultToInstanced )
class EVOLVINGBEHAVIOR_API UIntGeneratedProperty : public UGeneratedProperty
{
    GENERATED_BODY()
    
private:
    UPROPERTY( SaveGame )
    int32 val;
    
    UPROPERTY( SaveGame, EditAnywhere, Category = "EvolvingBehavior")
    int32 lowerBound;
    
    UPROPERTY( SaveGame, EditAnywhere, Category = "EvolvingBehavior")
    int32 upperBound;
    
public:
    virtual void InitMutate( URandomGen* rand ) override;
    
    virtual void Mutate( URandomGen* rand, double stdDevFractionOfRange = 0.25 ) override;

    void SetVal(int32 newVal) { val = newVal; }

    int32 GetVal() { return val; }
    
    virtual bool SetOn(UObject* object) override;
    
    virtual FString ToString() override { return FString::Printf(TEXT("%s [%i]"), *UGeneratedProperty::ToString(), GetVal()); };
};
