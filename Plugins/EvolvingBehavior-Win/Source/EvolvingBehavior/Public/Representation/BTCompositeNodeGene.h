#pragma once

#include "BTNodeGene.h"

#include "BTCompositeNodeGene.generated.h"

class UBTCompositeNode;
class UBTChildContainerGene;
class UBTServiceGene;

/**
 * @file BTCompositeNodeGene.h
 */

/**
 * @class UBTCompositeNodeGene
 * @author npc
 * @date 12/01/17
 * @brief The abstract base class of genes representing Composite nodes, which contain other nodes.
 *
 * The Composite node has one or more children, which are represented as UBTChildContainerGene objects.
 * Each of these objects contains either another composite node, which has additional children, or a task node,
 * which is a leaf and performs an action. The composite node's 'type' defines how those children are executed.
 *
 * There is currently only one subclass:
 *
 * BTMappedTaskGene:
 * These are objects which map to templates for creating an Unreal Engine task node. Defaults are automatically
 * set, and the only set up required is to add default nodes to the "NodeMapper" in the editor, or make sure the
 * nodes you want are in there. We recommend using only mapped nodes until you are comfortable with the tool.
 *
 * NOTE: There is no 'generated' version of a composite node because they do not contain any internal variables which
 * can be mutated, rather they just provide a high level structure for the execution of their children.
 */
UCLASS( abstract )
class EVOLVINGBEHAVIOR_API UBTCompositeNodeGene : public UBTNodeGene
{
    GENERATED_BODY()

protected:
    UPROPERTY( SaveGame, Instanced )
    TArray<UBTChildContainerGene*> children;
    
    UPROPERTY( SaveGame, Instanced )
    TArray<UBTServiceGene*> services;

public:
    void AddChild( UBTChildContainerGene* child );

    int32 RemoveChild( UBTChildContainerGene* child ) { return children.Remove( child ); }

    int32 InsertChild( UBTChildContainerGene* child, int32 idx );

    int GetNumChildren() const { return children.Num(); }

    UBTChildContainerGene* GetChild( int idx ) const { return children[idx]; }
    
    int32 RemoveService( UBTServiceGene* service );
    
    int32 InsertService( UBTServiceGene* service, int32 idx ) { return services.Insert( service, idx ); }
    
    int32 AddService( UBTServiceGene* service ) { return services.Add( service ); }
    
    int GetNumServices() const { return services.Num(); }
    
    UBTServiceGene* GetService( int idx ) const { return services[idx]; }

    virtual UBTCompositeNode* BuildPhenotype( UObject* outer )
        PURE_VIRTUAL( UBTCompositeNodeGene::BuildPhenotype, return nullptr; );
	
    virtual UBTNodeGene* Duplicate( UObject* outer ) override { return DuplicateComposite( outer ); };

    virtual UBTCompositeNodeGene* DuplicateComposite( UObject* outer )
        PURE_VIRTUAL( UBTCompositeNodeGene::DuplicateComposite, return nullptr; );

    virtual void SetServiceInjector(UServiceInjector* serviceInjector) override;
};
