#pragma once

#include "CoreMinimal.h"

#include "GeneratedProperty.generated.h"

class URandomGen;

/**
 * @file GeneratedProperty.h
 */

/**
 * @class GeneratedProperty
 * @author jim
 * @date 11/13/19
 * @brief An abstract class representing any type of property of a generated node.
 */
UCLASS( Blueprintable, Abstract, BlueprintType, EditInlineNew, DefaultToInstanced )
class EVOLVINGBEHAVIOR_API UGeneratedProperty : public UObject
{
    GENERATED_BODY()
    
private:
    UPROPERTY( SaveGame, EditAnywhere, Category = "EvolvingBehavior")
    FName name;
    
public:
    //called when first initialized. Sets property to random value
    virtual void InitMutate( URandomGen* rand ) { return; };
    
    //called later on to slighlty modify a property
    virtual void Mutate( URandomGen* rand, double stdDevFractionOfRange = 0.25 ) { return; };

    void SetName(FString newName)
    {
        name = FName(*newName);
    }
    
    void SetName(FName newName)
    {
        name = newName;
    }

    FName GetName() {
        return name;
    }

    virtual bool SetOn(UObject* object) { return false; };

    virtual FString ToString() { return name.ToString(); };
};
