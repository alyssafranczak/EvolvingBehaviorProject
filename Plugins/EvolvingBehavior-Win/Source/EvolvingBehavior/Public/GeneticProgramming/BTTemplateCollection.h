#pragma once

#include "BTTemplateCollection.generated.h"

class UBehaviorTree;
class UNodeMapper;

/**
 * @file BTTemplateCollection.h
 */

/**
 * @class UBTTemplateCollection
 * @author npc
 * @date 12/04/17
 * @brief A collection of template Behavior Trees for initial population and additional node randomization.
 * 
 * You can add Behavior Trees to the properties of this object in the Editor, and it will provide the 
 * random initial templates for the starting population.
 * 
 * The "extra templates" will be turned into extra random nodes to be available in the NodeMapper for mutation.
 */
UCLASS(Blueprintable, BlueprintType, EditInlineNew, DefaultToInstanced, CollapseCategories)
class EVOLVINGBEHAVIOR_API UBTTemplateCollection : public UObject
{
    GENERATED_BODY()

private:
    UPROPERTY( EditAnywhere, Category = "EvolvingBehavior")
    TArray<UBehaviorTree*> initialPopulationTemplates;

    UPROPERTY( EditAnywhere, Category = "EvolvingBehavior")
    TArray<UBehaviorTree*> extraTemplates;

protected:
    void MapTemplates( TArray<UBehaviorTree*>& templates, UNodeMapper* nodeMapper );

public:
    void VerifyTemplates();
    
    void MapTemplates( UNodeMapper* nodeMapper );

    int GetNumInitialTemplates() { return initialPopulationTemplates.Num(); }

    UBehaviorTree* GetInitialTemplate( int idx ) { return initialPopulationTemplates[idx]; }

    UBehaviorTree* GetInitialTemplateByID( FString behaviorTreePath );
};
