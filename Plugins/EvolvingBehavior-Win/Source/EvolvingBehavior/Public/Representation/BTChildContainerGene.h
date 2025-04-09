#pragma once

#include "BTCompositeNodeGene.h"
#include "BTTaskNodeGene.h"
#include "BTChildContainerGene.generated.h"

class UBTDecoratorGene;
class UBTCompositeNodeGene;

/**
 * @file BTChildContainerGene.h
 */

/**
 * @class UBTChildContainerGene
 * @author npc
 * @date 12/01/17
 * @brief A child wrapper object that contains either a Task or Composite gene, and its decorators.
 *
 * This wrapper object is stored inside Composite nodes, to wrap the children of that node.
 *
 * We need this wrapper because each child might be a Composite node gene or a Task node gene.
 *
 * You can determine whether this contains a Task node or a Composite node by calling the IsALeaf() function.
 * If it returns true, the child is a Task node (a leaf). Otherwise, it is a composite node.
 *
 * Additionally, you can retrieve the decorators on this child.
 */
UCLASS( DefaultToInstanced )
class EVOLVINGBEHAVIOR_API UBTChildContainerGene : public UObject
{
    GENERATED_BODY()

private:
    UPROPERTY( SaveGame, Instanced )
    UBTCompositeNodeGene* compositeChild;

    UPROPERTY( SaveGame, Instanced )
    UBTTaskNodeGene* taskChild;

    UPROPERTY( SaveGame, Instanced )
    TArray<UBTDecoratorGene*> decorators;

public:
    bool IsALeaf() { return taskChild != nullptr; }

    const UBTTaskNodeGene* GetTaskChild() const { return taskChild; }

    UBTTaskNodeGene* GetTaskChild() { return taskChild; }

    const UBTCompositeNodeGene* GetCompositeChild() const { return compositeChild; }

    UBTCompositeNodeGene* GetCompositeChild() { return compositeChild; }

    bool SetChild( UBTTaskNodeGene* gene );

    bool SetChild( UBTCompositeNodeGene* gene );

    void ReplaceChild( UBTTaskNodeGene* gene );

    void ReplaceChild( UBTCompositeNodeGene* gene );

    int32 RemoveDecorator( UBTDecoratorGene* decorator );

    int32 InsertDecorator( UBTDecoratorGene* decorator, int32 idx ) { return decorators.Insert( decorator, idx ); }
    
    int32 AddDecorator( UBTDecoratorGene* decorator ) { return decorators.Add( decorator ); }
    
    int GetNumDecorators() const { return decorators.Num(); }
    
    UBTDecoratorGene* GetDecorator( int idx ) const { return decorators[idx]; }
	
	UBTChildContainerGene* Duplicate( UObject* other );

    void SetServiceInjector( UServiceInjector* serviceInjector);
};
