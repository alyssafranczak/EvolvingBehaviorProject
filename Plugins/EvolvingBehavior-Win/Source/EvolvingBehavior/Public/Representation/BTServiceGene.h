
#pragma once

#include "CoreMinimal.h"

#include "BTNodeGene.h"

#include "BTServiceGene.generated.h"

class UBTService;

/**
 * @file BTDecoratorGene.h
 */

/**
 * @class UBTServiceGene
 * @author npc
 * @date 12/01/17
 * @brief The abstract base class representing Service nodes, which live below other nodes.
 * 
 * Service nodes attach to both task and composite nodes, and execute at a predefined frequency while the branch
 * they are on is bein executed. This can be a variable that you check many times, and a service node can perform
 * an action only after the variable reaches a certain value. Read about other use cases in the Unreal Engine docs.
 *
 * There are currently two subclasses:
 *
 * BTMappedServiceGene:
 * These are objects which map to templates for creating an Unreal Engine service node. Defaults are automatically
 * set, and the only set up required is to add default nodes to the "NodeMapper" in the editor, or make sure the
 * nodes you want are in there. We recommend using only mapped nodes until you are comfortable with the tool.
 *
 * BTGeneratedServiceGene:
 * These are objects which map to templates for creating an Unreal Engine service node. These nodes are based off the
 * default Unreal nodes, sharing their base class, but must be "created" in the editor. This means going into the
 * GeneratedTemplateLibrary and adding nodes. You must choose a name, class, and properties for each node. If your
 * property names match those of the UClass you chose, the internal variables of the node will be mutated during the
 * evolution process, giving an increased degree of control.
 */
UCLASS( abstract )
class EVOLVINGBEHAVIOR_API UBTServiceGene : public UBTNodeGene
{
    GENERATED_BODY()

public:
    virtual UBTService* BuildPhenotype( UObject* outer )
        PURE_VIRTUAL( UBTServiceGene::BuildPhenotype, return nullptr; );
    
    virtual UBTNodeGene* Duplicate( UObject* outer ) override { return DuplicateService( outer ); }

    virtual UBTServiceGene* DuplicateService( UObject* other )
        PURE_VIRTUAL( UBTServiceGene::DuplicateService, return nullptr; );
    
};
