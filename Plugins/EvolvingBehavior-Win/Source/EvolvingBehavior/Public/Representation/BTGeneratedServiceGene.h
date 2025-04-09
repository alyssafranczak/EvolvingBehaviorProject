#pragma once

#include "BTServiceGene.h"
#include "BoolGeneratedProperty.h"
#include "FloatGeneratedProperty.h"
#include "IntGeneratedProperty.h"
#include "RandomGen.h"
#include "Save/Saveable.h"
#include "Templates/SubclassOf.h"

#include "BTGeneratedServiceGene.generated.h"

class UBTTaskNode;
class URandomGen;
class UPropertyDict;

/**
 * @file BTGeneratedServieGene.h
 */

/**
 * @class UBTGeneratedServiceGene
 * @author jim
 * @date 11/13/19
 * @brief A Service gene which can have its internal structure mutated.
 * 
 * These nodes must be created in the editor under the GeneratedTemplateLibrary. When creating a generated node,
 * the property names you define must match the actual names of the properties for the Unreal Engine class you
 * choose. Any property names you add can now be mutated based on type (int, float, bool), and ones that you do
 * not define can not be mutated, and will remain at their default.
 */
UCLASS( Blueprintable, BlueprintType, EditInlineNew, DefaultToInstanced )
class EVOLVINGBEHAVIOR_API UBTGeneratedServiceGene : public UBTServiceGene, public ISaveable
{
    GENERATED_BODY()

private:
    UPROPERTY( SaveGame, EditAnywhere, Category = "EvolvingBehavior")
    FString name;
    
    UPROPERTY( EditAnywhere, Category = "EvolvingBehavior")
    TSubclassOf<UBTService> classType;

    UPROPERTY( SaveGame )
    FString className;
    
    UPROPERTY( SaveGame, EditAnywhere, Instanced, Category = "EvolvingBehavior")
    UPropertyDict* propertyDict;

protected:
    UPROPERTY()
    URandomGen* rand;

public:

    UBTGeneratedServiceGene();

    virtual UBTService* BuildPhenotype( UObject* outer ) override;
    
    void RandomizeAllProperties();

    void MutateRandomProperty(float stdDevFractionOfRange);
    
    FString GetName() {
        return name;
    }
    
    virtual FString ToString() override;
    
    virtual UBTServiceGene* DuplicateService( UObject* other ) override;
    
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Saveable Actor")
    void DataLoaded();
    virtual void DataLoaded_Implementation() override;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Saveable Actor")
    void DataSaved();
    virtual void DataSaved_Implementation() override;
};
