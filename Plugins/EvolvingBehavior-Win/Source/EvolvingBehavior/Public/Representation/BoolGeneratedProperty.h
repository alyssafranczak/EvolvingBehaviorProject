#pragma once

#include "CoreMinimal.h"
#include "Representation/GeneratedProperty.h"

#include "BoolGeneratedProperty.generated.h"

class UGeneratedProperty;
class URandomGen;

/**
 * @file BoolGeneratedProperty.h
 */

/**
 * @class BoolGeneratedProperty
 * @author jim
 * @date 11/13/19
 * @brief A class representing a boolean property of a generated node. 
 */
UCLASS( Blueprintable, BlueprintType, EditInlineNew, DefaultToInstanced )
class EVOLVINGBEHAVIOR_API UBoolGeneratedProperty : public UGeneratedProperty
{
    GENERATED_BODY()
    
private:
    UPROPERTY( SaveGame )
    bool val;
    
public:
    virtual void InitMutate( URandomGen* rand ) override;
    
    virtual void Mutate( URandomGen* rand, double stdDevFractionOfRange ) override;

    void SetVal(bool newVal) { val = newVal; }
    
    bool GetVal() { return val; }

    virtual bool SetOn(UObject* object) override;

    virtual FString ToString() override { return FString::Printf(TEXT("%s [%s]"), *UGeneratedProperty::ToString(), GetVal() ? TEXT("True") : TEXT("False")); };
};
