#pragma once

#include "CoreMinimal.h"
#include "Representation/GeneratedProperty.h"

#include "FloatGeneratedProperty.generated.h"

class UGeneratedProperty;
class URandomGen;

/**
 * @file FloatGeneratedProperty.h
 */

/**
 * @class FloatGeneratedProperty
 * @author jim
 * @date 11/13/19
 * @brief A class representing a float property of a generated node.
 */
UCLASS( Blueprintable, BlueprintType, EditInlineNew, DefaultToInstanced )
class EVOLVINGBEHAVIOR_API UFloatGeneratedProperty : public UGeneratedProperty
{
    GENERATED_BODY()
    
private:
    UPROPERTY( SaveGame )
    float val;
    
    UPROPERTY( SaveGame, EditAnywhere, Category = "EvolvingBehavior")
    float lowerBound;
    
    UPROPERTY( SaveGame, EditAnywhere, Category = "EvolvingBehavior")
    float upperBound;
    
public:
    virtual void InitMutate( URandomGen* rand ) override;
    
    virtual void Mutate( URandomGen* rand, double stdDevFractionOfRange = 0.25) override;

    void SetVal(float newVal) { val = newVal; }
    
    float GetVal() { return val; }
    
    virtual bool SetOn(UObject* object) override;
    
    virtual FString ToString() override { return FString::Printf(TEXT("%s [%f]"), *UGeneratedProperty::ToString(), GetVal()); };
};
