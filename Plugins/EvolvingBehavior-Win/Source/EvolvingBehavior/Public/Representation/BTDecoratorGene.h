
#pragma once

#include "CoreMinimal.h"

#include "BTNodeGene.h"

#include "BTDecoratorGene.generated.h"

class UBTDecorator;

/**
 * @file BTDecoratorGene.h
 */

/**
 * @class UBTDecoratorGene
 * @author npc
 * @date 12/01/17
 * @brief The abstract base class representing Decorators nodes.
 * 
 * Decorator nodes sit on top of BTChildContainerGene's, which contain either a CompositeNodeGene or
 * a TaskNodeGene. They can influence the way that those nodes execute in a number of ways, even stopping
 * them from executing at all.
 *
 * There are currently two subclasses:
 *
 * BTMappedDecoratorGene:
 * These are objects which map to templates for creating an Unreal Engine decorator node. Defaults are automatically
 * set, and the only set up required is to add default nodes to the "NodeMapper" in the editor, or make sure the
 * nodes you want are in there. We recommend using only mapped nodes until you are comfortable with the tool.
 *
 * BTGeneratedDecoratorGene:
 * These are objects which map to templates for creating an Unreal Engine decorator node. These nodes are based off the
 * default Unreal nodes, sharing their base class, but must be "created" in the editor. This means going into the
 * GeneratedTemplateLibrary and adding nodes. You must choose a name, class, and properties for each node. If your
 * property names match those of the UClass you chose, the internal variables of the node will be mutated during the
 * evolution process, giving an increased degree of control.
 */
UCLASS( abstract )
class EVOLVINGBEHAVIOR_API UBTDecoratorGene : public UBTNodeGene
{
    GENERATED_BODY()

public:
    virtual UBTDecorator* BuildPhenotype( UObject* outer )
        PURE_VIRTUAL( UBTDecoratorGene::BuildPhenotype, return nullptr; );
    
    virtual UBTNodeGene* Duplicate( UObject* outer ) override { return DuplicateDecorator( outer ); }

    virtual UBTDecoratorGene* DuplicateDecorator( UObject* other )
        PURE_VIRTUAL( UBTDecoratorGene::DuplicateDecorator, return nullptr; );
};
