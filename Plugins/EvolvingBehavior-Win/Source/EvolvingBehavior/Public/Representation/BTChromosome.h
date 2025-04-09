#pragma once

#include "BTCompositeNodeGene.h"
#include "BTChromosome.generated.h"

class UBTDecoratorGene;
class UBTServiceGene;

/**
 * @file BTChromosome.h
 */

/**
 * @class UBTChromosome
 * @author npc
 * @date 12/01/17
 * @brief A chromosome stores the information about a Behavior Tree in its entirety.
 *
 * Each Chromosome represents a single Behavior Tree in gene form.
 *
 * The Behavior Tree consists of a Root Node and a set of Root Decorators.
 *
 * Under the Root Node Gene, you will find the rest of the tree.
 *
 * The tree sturcture mirrors the structure of Unreal Engine Behavior Trees, and is as follows:
 *
 * There are five node types:
 *     - BTCompositeNodeGene (This mirrors BTCompositeNode)
 *     - BTChildContainerGene (This mirrors BTCompositeChild)
 *     - BTTaskNodeGene (This mirros BTTaskNode)
 *     - BTDecoratorGene (This mirrors BTDecorator)
 *     - BTServiceGene (This mirrors BTService)
 *
 * This current class is a BTChromosome, which does not mirror a Unreal Engine node. These objects represent
 * a container for the root node, and have:
 *     0 to many BTDecoratorGene, which are conditionals to be applied to the root node
 *     1 BTCompositeNodeGene, which is the root of the tree
 *
 * The root node is a BTCompositeNodeGene, these nodes have:
 *     0 to many BTChildContainerGene, which act as containers for child noded
 *     0 to many BTServiceGene, which run in the background during the execution of the composite node
 *
 * Each child of a BTCompositeNodeGene is a BTChildContainerGene, which acts as a container for either a
 * BTTaskNodeGene or a BTCompositeNodeGene, these nodes have:
 *     0 to many BTDecoratorGene, which are conditions to be applied to their node
 *     1 of:
 *         'taskChild' of type BTTaskNodeGene
 *         'compositeChild' of type BTCompositeNodeGene
 *
 * A BTTaskNodeGene is an action for the artificial agent to execute, it can be anything from a move instruction, to a
 * wait or play sound instruction. These are the leaf nodes of the behavior tree, and they have:
 *     0 to many BTServiceGene, which run in the background during the execution of the task node
 *
 * BTServiceGene nodes can sit on all BTCompositeNodeGene nodes or BTTaskNodeGene nodes, there can
 * be any number.
 *
 * BTDecoratorGene nodes can sit on all BTChildContainerGene nodes, there can be any number.
 *
 *
 * An example sketch of this version of behavior tree is as follows:
 *
 *
 * 
 * 
 * 
 *                          -                                             -
 *                          |    BTChromosome (Container for root node)   |
 *                          |                                             |
 *                          |            (BTDecoratorGene 1)              |
 *                          |            (BTDecoratorGene 2)              |
 *                          |                                             |
 *                          |                                             |
 *                          |        BTCompositeNodeGene (root)           |
 *                          |                                             |
 *                          |            (BTServiceGene 1)                |
 *                          -                                             -
 *                               ||                                ||   
 *                               ||                                ||
 *                               ||                                ||    
 *        -                          -                         -                          -          
 *        |  BTChildContainerGene 1  |                         |  BTChildContainerGene 2  |
 *        |                          |                         |                          |  
 *        |    (BTDecoratorGene 1)   |                         |    BTCompositeNodeGene   |                      
 *        |                          |                         -                          -
 *        |                          |                                     ||
 *        |      BTTaskNodeGene      |                                     ||
 *        |                          |                                     ||                                                      
 *        |    (BTServiceGene 1)     |                                     ||
 *        |    (BTServiceGene 2)     |                                     ||             
 *        -                          -                        -                          -
 *                                                            |  BTChildContainerGene 1  |
 *                                                            |                          |
 *                                                            |    (BTDecoratorGene 1)   |      
 *                                                            |    (BTDecoratorGene 2)   |      
 *                                                            |    (BTDecoratorGene 3)   |      
 *                                                            |                          |     
 *                                                            |      BTTaskNodeGene      |      
 *                                                            -                          -
 *
 *
 */
UCLASS( DefaultToInstanced )
class EVOLVINGBEHAVIOR_API UBTChromosome : public UObject
{
    GENERATED_BODY()

private:
    UPROPERTY( SaveGame, Instanced ) 
    UBTCompositeNodeGene* rootNode;

    UPROPERTY( SaveGame, Instanced )
    TArray<UBTDecoratorGene*> decorators;

    UPROPERTY( SaveGame )
    FString initialBTAsset;

public:
    void RegisterInitialBTAsset(FString btAssetID) { initialBTAsset = btAssetID; }

    FString GetInitialBTAsset() { return initialBTAsset; };

    const UBTCompositeNodeGene* GetRoot() const { return rootNode; }

    UBTCompositeNodeGene* GetRoot() { return rootNode; }

    void SetRoot( UBTCompositeNodeGene* node );
    
    void AddDecorator( UBTDecoratorGene* decorator ) { decorators.Add( decorator ); }
    
    int32 RemoveDecorator( UBTDecoratorGene* decorator );
    
    int32 InsertDecorator( UBTDecoratorGene* decorator, int32 idx ) { return decorators.Insert( decorator, idx ); }

    int GetNumDecorators() const { return decorators.Num(); }

    UBTDecoratorGene* GetDecorator( int idx ) const { return decorators[idx]; }
	
	UBTChromosome* Duplicate( UObject* outer );

    FString ToString();

    void SetServiceInjector(UServiceInjector* serviceInjector);

	void ToLog();
};
