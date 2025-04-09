#pragma once

#include "BehaviorTree/BTCompositeNode.h"
#include "ServiceInjector.h"

#include "BehaviorTreeToGeneConverter.generated.h"

class UNodeMapper;
class UBTChromosome;
class UBTCompositeNode;
class UBTCompositeNodeGene;
class UBehaviorTree;
class UBTTaskNode;
class UBTTaskNodeGene;
class UBTChildContainerGene;
class UBTDecorator;
class UServiceInjector;
class UBTDecoratorGene;
class UBTService;
class UBTServiceGene;

/**
 * @file BehaviorTreeToGeneConverter.h
 */

/**
 * @class UBehaviorTreeToGeneConverter
 * @author npc
 * @date 12/01/17
 * @brief Converts a Behavior Tree to a Chromosome for evolution.
 * 
 * At the moment, this conversion does not support:
 * - UDecoratorOps
 * - Services
 * - Composite Decorators
 * - Subtree Task Nodes
 * 
 * We hope to support these in future versions.
 */
UCLASS()
class EVOLVINGBEHAVIOR_API UBehaviorTreeToGeneConverter : public UObject
{
	GENERATED_BODY()
	
private:
	UPROPERTY()
	UServiceInjector* serviceInjector;
    
    UPROPERTY()
    UNodeMapper* mapper;
	
protected:
	
	UBTCompositeNodeGene* Convert( UObject* outer, const UBTCompositeNode* node ) const;
	
	UBTTaskNodeGene* Convert( UObject* outer, const UBTTaskNode* node ) const;
    
    void ConvertAndAddServices( UBTCompositeNodeGene* compositeGene, const UBTCompositeNode* compositeNode ) const;
    
    void ConvertAndAddServices( UBTTaskNodeGene* taskGene, const UBTTaskNode* taskNode ) const;
	
	void ConvertAndAddDecorators( UBTChildContainerGene* childGene, const FBTCompositeChild childNode ) const;
	
    UBTServiceGene* Convert( UObject* outer, const UBTService* node ) const;
    
	UBTDecoratorGene* Convert( UObject* outer, const UBTDecorator* node ) const;
	
	void AddRootDecorators( UBTChromosome* chromosome, const UBehaviorTree* behaviorTree ) const;
	
public:

    void Init( UServiceInjector* injector );

	UBTChromosome* Convert( UObject* outer, const UBehaviorTree* behaviorTree ) const;
};
