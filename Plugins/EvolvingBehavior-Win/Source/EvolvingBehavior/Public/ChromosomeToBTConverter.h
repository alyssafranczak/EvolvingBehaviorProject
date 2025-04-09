
#pragma once

#include "CoreMinimal.h"

#include "BehaviorTree/BTCompositeNode.h"
#include "Representation/BTChildContainerGene.h"
#include "Service.h"
#include "ChromosomeToBTConverter.generated.h"

class UBehaviorTree;
class UBTChromosome;

/**
 * @file ChromosomeToBTConverter.h
 */

/**
 * @class UChromosomeToBTConverter
 * @author npc
 * @date 12/01/17
 * @brief Converts a UBTChromosome into to an Unreal Behavior Tree.
 * 
 * At the moment, this conversion does not support:
 * - UDecoratorOps
 * - Composite Decorators
 * - Subtree Task Nodes
 * 
 * We hope to support these in future versions.
 */
UCLASS()
class EVOLVINGBEHAVIOR_API UChromosomeToBTConverter : public UService
{
    GENERATED_BODY()

protected:
    void ConvertRootDecorators( UBehaviorTree* behaviorTree, UBTChromosome* chromosome ) const;

    void ConvertServices( UBTTaskNodeGene* taskGene,
                         UBTTaskNode* task,
                         UBTCompositeNode* compositeParent,
                         uint16 childIdx,
                         uint16* ExecutionIndex,
                         uint8 treeDepth ) const;
    
    void ConvertServices( UBTCompositeNodeGene* compositeGene,
                         UBTCompositeNode* composite,
                         uint16* ExecutionIndex,
                         uint8 treeDepth ) const;
    
    void ConvertDecorators( UBTChildContainerGene* childContainerGene,
        FBTCompositeChild* childContainer,
        UBTCompositeNode* compositeParent,
        uint16 childIdx,
        uint16* ExecutionIndex,
        uint8 treeDepth ) const;

    UBTCompositeNode* Convert( UBTCompositeNode* compositeParent,
        UBTCompositeNodeGene* compositeGene,
        uint16* ExecutionIndex,
        uint8 treeDepth ) const;
    
    UBTTaskNode* Convert(UBTTaskNodeGene *taskGene, UBTCompositeNode *compositeParent, uint16 childIdx, uint16 *ExecutionIndex, uint8 treeDepth) const;
		
	FBTCompositeChild& InitEmptyCompositeChild( UBTCompositeNode* composite ) const;

public:

    UBehaviorTree* Convert( UObject* outer, UBehaviorTree* initialBT, UBTChromosome* chromosome ) const;
};
