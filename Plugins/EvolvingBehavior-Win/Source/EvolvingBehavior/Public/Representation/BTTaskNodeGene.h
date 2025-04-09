#pragma once

#include "BTNodeGene.h"
#include "BTTaskNodeGene.generated.h"

class UBTTaskNode;
class UBTTaskNodeGene;
class UBTServiceGene;

/**
 * @file BTTaskNodeGene.h
 */

/**
 * @class UBTTaskNodeGene
 * @author npc
 * @date 12/01/17
 * @brief The abstract base class for genes representing Task nodes, which are leaves of the behavior tree.
 * 
 * Task nodes perform an action, like moving a character or changing it's color.
 *
 * There are currently two subclasses:
 *
 * BTMappedTaskGene:
 * These are objects which map to templates for creating an Unreal Engine task node. Defaults are automatically
 * set, and the only set up required is to add default nodes to the "NodeMapper" in the editor, or make sure the
 * nodes you want are in there. We recommend using only mapped nodes until you are comfortable with the tool.
 *
 * BTGeneratedTaskGene:
 * These are objects which map to templates for creating an Unreal Engine task node. These nodes are based off the
 * default Unreal nodes, sharing their base class, but must be "created" in the editor. This means going into the
 * GeneratedTemplateLibrary and adding nodes. You must choose a name, class, and properties for each node. If your
 * property names match those of the UClass you chose, the internal variables of the node will be mutated during the
 * evolution process, giving an increased degree of control.
 */
UCLASS( abstract )
class EVOLVINGBEHAVIOR_API UBTTaskNodeGene : public UBTNodeGene
{
    GENERATED_BODY()
    
protected:
    UPROPERTY( SaveGame, Instanced )
    TArray<UBTServiceGene*> services;

public:
    int32 RemoveService( UBTServiceGene* service );
    
    int32 InsertService( UBTServiceGene* service, int32 idx ) { return services.Insert( service, idx ); }
    
    int32 AddService( UBTServiceGene* service ) { return services.Add( service ); }
    
    int GetNumServices() const { return services.Num(); }
    
    UBTServiceGene* GetService( int idx ) const { return services[idx]; }
    
    virtual UBTTaskNode* BuildPhenotype( UObject* outer ) PURE_VIRTUAL( UBTTaskNode::BuildPhenotype, return nullptr; );

    virtual UBTNodeGene* Duplicate( UObject* outer ) override { return DuplicateTask( outer ); }
    
    virtual UBTTaskNodeGene* DuplicateTask( UObject* other )
        PURE_VIRTUAL( UBTTaskNode::DuplicateTask, return nullptr; );
    
    virtual void SetServiceInjector(UServiceInjector* serviceInjector ) override;
};
