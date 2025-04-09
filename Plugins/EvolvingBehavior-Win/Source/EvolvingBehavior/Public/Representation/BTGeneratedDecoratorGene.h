#pragma once

#include "BTDecoratorGene.h"
#include "BoolGeneratedProperty.h"
#include "FloatGeneratedProperty.h"
#include "IntGeneratedProperty.h"
#include "RandomGen.h"
#include "Save/Saveable.h"
#include "Templates/SubclassOf.h"

#include "BTGeneratedDecoratorGene.generated.h"

class UBTDecorator;
class URandomGen;

class UPropertyDict;

/**
 * @file BTGeneratedDecoratorGene.h
 */

/**
 * @class UBTGeneratedDecoratorGene
 * @author jim
 * @date 11/13/19
 * @brief A Decorator gene which can have its internal structure mutated.
 * 
 * These nodes must be created in the editor under the GeneratedTemplateLibrary. When creating a generated node,
 * the property names you define must match the actual names of the properties for the Unreal Engine class you
 * choose. Any property names you add can now be mutated based on type (int, float, bool), and ones that you do
 * not define can not be mutated, and will remain at their default.
 */
UCLASS( Blueprintable, BlueprintType, EditInlineNew, DefaultToInstanced )
class EVOLVINGBEHAVIOR_API UBTGeneratedDecoratorGene : public UBTDecoratorGene, public ISaveable
{
    GENERATED_BODY()

private:
    UPROPERTY( SaveGame, EditAnywhere, Category = "EvolvingBehavior")
    FString name;
    
    UPROPERTY( EditAnywhere, Category = "EvolvingBehavior")
    TSubclassOf<UBTDecorator> classType;
    
    UPROPERTY( SaveGame )
    FString className;
    
    UPROPERTY( SaveGame, EditAnywhere, Instanced, Category = "EvolvingBehavior")
    UPropertyDict* propertyDict;
   
protected:
    UPROPERTY()
    URandomGen* rand;

public:
    UBTGeneratedDecoratorGene();
    
    virtual UBTDecorator* BuildPhenotype( UObject* outer ) override;
    
    void RandomizeAllProperties();

    void MutateRandomProperty(float stdDevFractionOfRange);

    FString GetName() {
        return name;
    }
   
    virtual UBTDecoratorGene* DuplicateDecorator( UObject* outer ) override;

    virtual FString ToString() override;
    
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Saveable Actor")
    void DataLoaded();
    virtual void DataLoaded_Implementation() override;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Saveable Actor")
    void DataSaved();
    virtual void DataSaved_Implementation() override;
};
