#pragma once

#include "BTTaskNodeGene.h"
#include "BoolGeneratedProperty.h"
#include "FloatGeneratedProperty.h"
#include "IntGeneratedProperty.h"
#include "RandomGen.h"
#include "Save/Saveable.h"
#include "Templates/SubclassOf.h"

#include "BTGeneratedTaskGene.generated.h"

class UBTTaskNode;
class UBoolGeneratedProperty;
class UFloatGeneratedProperty;
class UIntGeneratedProperty;
class URandomGen;
class UPropertyDict;

/**
 * @file BTGeneratedTaskGene.h
 */

/**
 * @class UBTGeneratedTaskGene
 * @author jim
 * @date 11/13/19
 * @brief A Task gene which can have its internal structure mutated.
 *
 * These nodes must be created in the editor under the GeneratedTemplateLibrary. When creating a generated node,
 * the property names you define must match the actual names of the properties for the Unreal Engine class you
 * choose. Any property names you add can now be mutated based on type (int, float, bool), and ones that you do
 * not define can not be mutated, and will remain at their default.
 */
UCLASS( Blueprintable, BlueprintType, EditInlineNew, DefaultToInstanced )
class EVOLVINGBEHAVIOR_API UBTGeneratedTaskGene : public UBTTaskNodeGene, public ISaveable
{
    GENERATED_BODY()

private:
    UPROPERTY( SaveGame, EditAnywhere, Category = "EvolvingBehavior")
    FString name;
    
    UPROPERTY( EditAnywhere, Category = "EvolvingBehavior")
    TSubclassOf<UBTTaskNode> classType;

    UPROPERTY( SaveGame )
    FString className;

    UPROPERTY( SaveGame, EditAnywhere, Instanced, Category = "EvolvingBehavior")
    UPropertyDict* propertyDict;

protected:
    UPROPERTY()
    URandomGen* rand;
public:
    UBTGeneratedTaskGene();

    virtual UBTTaskNode* BuildPhenotype( UObject* outer ) override;
    
    // initializes node by setting all properties to random values
    void RandomizeAllProperties();
    
    void MutateRandomProperty(float stdDevFractionOfRange);
    
    FString GetName() {
        return name;
    }
    
    virtual UBTTaskNodeGene* DuplicateTask( UObject* other ) override;

    virtual FString ToString() override;
    
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Saveable Actor")
    void DataLoaded();
    virtual void DataLoaded_Implementation() override;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Saveable Actor")
    void DataSaved();
    virtual void DataSaved_Implementation() override;
};
