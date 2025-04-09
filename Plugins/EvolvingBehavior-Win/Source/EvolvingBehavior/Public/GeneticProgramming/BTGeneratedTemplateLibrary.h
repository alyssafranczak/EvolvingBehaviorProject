#pragma once

#include "CoreMinimal.h"
#include "Representation/BTGeneratedTaskGene.h"
#include "Representation/BTGeneratedDecoratorGene.h"
#include "Representation/BTGeneratedServiceGene.h"
#include "Service.h"

#include "BTGeneratedTemplateLibrary.generated.h"

class BTGeneratedTaskGene;
class BTGeneratedDecoratorGene;
class BTGeneratedServiceGene;
class URandomGen;
class UBTNode;

/**
 * @file BTGeneratedTemplates.h
 */

/**
 * @class UBTGeneratedTemplateLibrary
 * @author jim
 * @date 11/13/19
 * @brief Stores node templates for generated nodes to use. The generated node counterpart to the NodeMapper.
 * See Representation->BTGenerated[type]Gene to learn more about generated nodes.
 */
UCLASS( Blueprintable, BlueprintType, EditInlineNew, DefaultToInstanced, CollapseCategories )
class EVOLVINGBEHAVIOR_API UBTGeneratedTemplateLibrary : public UService
{
    GENERATED_BODY()
    
private:
    UPROPERTY( EditAnywhere, Instanced, Category = "EvolvingBehavior")
    TArray<UBTGeneratedTaskGene*> tasks;
    
    UPROPERTY( EditAnywhere, Instanced, Category = "EvolvingBehavior")
    TArray<UBTGeneratedDecoratorGene*> decorators;
    
    UPROPERTY( EditAnywhere, Instanced, Category = "EvolvingBehavior")
    TArray<UBTGeneratedServiceGene*> services;
    
public:
    UBTGeneratedTaskGene* GetRandomGeneratedTask( URandomGen* randomGen, UObject* parentObject );
    
    UBTGeneratedDecoratorGene* GetRandomGeneratedDecorator( URandomGen* randomGen, UObject* parentObject );
    
    UBTGeneratedServiceGene* GetRandomGeneratedService( URandomGen* randomGen, UObject* parentObject );
    
    TArray<UBTGeneratedTaskGene*> GetTasks() {
        return this->tasks;
    }
    
    TArray<UBTGeneratedDecoratorGene*> GetDecorators() {
        return this->decorators;
    }
    
    TArray<UBTGeneratedServiceGene*> GetServices() {
        return this->services;
    }
};
