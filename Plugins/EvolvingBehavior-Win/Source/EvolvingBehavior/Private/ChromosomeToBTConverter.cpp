#include "ChromosomeToBTConverter.h"

#include "EvolvingBehavior.h"

#include "BTChromosome.h"
#include "BTCompositeNodeGene.h"
#include "BTServiceGene.h"
#include "BTDecoratorGene.h"

#include "BehaviorTree/BTCompositeNode.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BehaviorTree/BTService.h"
#include "BehaviorTree/BTDecorator.h"
#include "BehaviorTree/BehaviorTree.h"

UBehaviorTree*
UChromosomeToBTConverter::Convert( UObject* outer, UBehaviorTree* initialBT, UBTChromosome* chromosome ) const
{
    UBehaviorTree* resultTree = DuplicateObject<UBehaviorTree>( initialBT, outer );
    // Clear the root node.
    resultTree->RootNode = nullptr;
    //resultTree->BlackboardAsset = initialBT->BlackboardAsset;

#if WITH_EDITORONLY_DATA
    // We can't create a new, accurate BehaviorTree Graph, due to it being an internal type in the BehaviorTreeEditor's module.
    // However, if we delete the existing graph, we will force the Behavior Tree Editor to rebuild it whenever the BT is next loaded.
    // This will automatically update the graph to match the re-designed tree when it is eventually opened in the editor.
    if (resultTree->BTGraph != nullptr)
    {
        resultTree->BTGraph = nullptr;
    }
#endif

    ConvertRootDecorators( resultTree, chromosome );

    uint16 ExecutionIndex = 0;
    UBTCompositeNode* rootNode = Convert( nullptr, chromosome->GetRoot(), &ExecutionIndex, 0 );
    rootNode->Rename( NULL, resultTree );
    if( nullptr == rootNode )
	{
	    //UE_LOG( LogEvolvingBehavior, Error, TEXT( "Failed to convert Behavior Tree root node." ) );
	    return nullptr;
	}

    resultTree->RootNode = rootNode;

    return resultTree;
}

UBTCompositeNode* UChromosomeToBTConverter::Convert( UBTCompositeNode* compositeParent,
    UBTCompositeNodeGene* compositeGene,
    uint16* ExecutionIndex,
    uint8 treeDepth ) const
{
    UBTCompositeNode* composite = compositeGene->BuildPhenotype( compositeParent );
    
    if( nullptr == composite )
	{
	    //UE_LOG( LogEvolvingBehavior, Error, TEXT( "Failed to convert Composite Node: %s" ) );
	    return nullptr;
	}
    
    //UE_LOG( LogEvolvingBehavior, Error, TEXT( "Converted Composite Node: %s" ),
      //     *compositeGene->ToString() );

    composite->InitializeNode( compositeParent, *ExecutionIndex, 0, treeDepth );
    *ExecutionIndex += 1;
    
    ConvertServices(compositeGene, composite, ExecutionIndex, treeDepth);

    for( int childIdx = 0; childIdx < compositeGene->GetNumChildren(); ++childIdx )
	{
	    UBTChildContainerGene* childContainerGene = compositeGene->GetChild( childIdx );
	    FBTCompositeChild& childContainer = InitEmptyCompositeChild( composite );

	    ConvertDecorators( childContainerGene, &childContainer, composite, childIdx, ExecutionIndex, treeDepth );

	    if( childContainerGene->IsALeaf() )
		{
            childContainer.ChildTask = Convert(childContainerGene->GetTaskChild(), composite, childIdx, ExecutionIndex, treeDepth + 1);
		}
	    else
		{
		    UBTCompositeNode* compositeChild
		        = Convert( composite, childContainerGene->GetCompositeChild(), ExecutionIndex, treeDepth + 1 );

		    if( nullptr == compositeChild )
			{
			    //UE_LOG( LogEvolvingBehavior, Error, TEXT( "Failed to convert Composite Child Node: %s" ),
			      //  *childContainerGene->GetCompositeChild()->ToString() );
			    continue;
			}
            
            //UE_LOG( LogEvolvingBehavior, Error, TEXT( "Converted Composite Child Node: %s" ),
              //     *childContainerGene->GetCompositeChild()->ToString() );

		    compositeChild->InitializeComposite( ( *ExecutionIndex ) - 1 );

		    childContainer.ChildComposite = compositeChild;
		}
	}

    return composite;
}

UBTTaskNode* UChromosomeToBTConverter::Convert(UBTTaskNodeGene *taskGene, UBTCompositeNode *compositeParent, uint16 childIdx, uint16 *ExecutionIndex, uint8 treeDepth) const
{
    UBTTaskNode* taskChild = taskGene->BuildPhenotype( compositeParent );
    
    if( nullptr == taskChild )
    {
        //UE_LOG( LogEvolvingBehavior, Error, TEXT( "Failed to convert Task Node: %s" ),
          //     *taskGene->ToString() );
        return nullptr;
    }
    
    //UE_LOG( LogEvolvingBehavior, Error, TEXT( "Converted Task: %s" ),
      //     *taskGene->ToString() );
    
    taskChild->InitializeNode( compositeParent, *ExecutionIndex, 0, treeDepth + 1 );
    *ExecutionIndex += 1;
    
    ConvertServices(taskGene, taskChild, compositeParent, childIdx, ExecutionIndex, treeDepth);
    
    return taskChild;
}

void UChromosomeToBTConverter::ConvertServices( UBTCompositeNodeGene* compositeGene,
                                                 UBTCompositeNode* composite,
                                                 uint16* ExecutionIndex,
                                                 uint8 treeDepth ) const
{
    for( int i = 0; i < compositeGene->GetNumServices(); i++ )
    {
        UBTServiceGene* serviceGene = compositeGene->GetService( i );
        UBTService* service = nullptr;
        if(serviceGene != nullptr) {
            service = serviceGene->BuildPhenotype( composite );
        }
        
        if( nullptr == service )
        {
            //UE_LOG( LogEvolvingBehavior, Error, TEXT( "Failed to convert Service: %s" ),
              //     *serviceGene->ToString() );
            continue;
        }
        
        //UE_LOG( LogEvolvingBehavior, Error, TEXT( "Converted Service: %s" ),
          //     *serviceGene->ToString() );
        
        service->InitializeNode( composite, *ExecutionIndex, 0, treeDepth );
        *ExecutionIndex += 1;
        service->InitializeParentLink( BTSpecialChild::OwnedByComposite );
        
        composite->Services.Add( service );
    }
}

void UChromosomeToBTConverter::ConvertServices( UBTTaskNodeGene* taskGene,
                                               UBTTaskNode* task,
                                               UBTCompositeNode* compositeParent,
                                               uint16 childIdx,
                                               uint16* ExecutionIndex,
                                               uint8 treeDepth ) const
{
    for( int i = 0; i < taskGene->GetNumServices(); ++i )
    {
        UBTServiceGene* serviceGene = taskGene->GetService( i );
        
        UBTService* service = nullptr;
        if(serviceGene != nullptr) {
            service = serviceGene->BuildPhenotype( task );
        }
        
        if( nullptr == service )
        {
            //UE_LOG( LogEvolvingBehavior, Error, TEXT( "Failed to convert Service: %s" ),
              //     *serviceGene->ToString() );
            continue;
        }
        
        //UE_LOG( LogEvolvingBehavior, Error, TEXT( "Converted Service: %s" ),
          //     *serviceGene->ToString() );
        
        //Cant do this for task?
        service->InitializeNode( compositeParent, *ExecutionIndex, 0, treeDepth );
        *ExecutionIndex += 1;
        service->InitializeParentLink( childIdx );
        
        task->Services.Add( service );
    }
}

void UChromosomeToBTConverter::ConvertDecorators( UBTChildContainerGene* childContainerGene,
    FBTCompositeChild* childContainer,
    UBTCompositeNode* compositeParent,
    uint16 childIdx,
    uint16* ExecutionIndex,
    uint8 treeDepth ) const
{
    for( int i = 0; i < childContainerGene->GetNumDecorators(); ++i )
	{
	    UBTDecoratorGene* decoratorGene = childContainerGene->GetDecorator( i );

	    UBTDecorator* decorator = decoratorGene->BuildPhenotype( compositeParent );

	    if( nullptr == decorator )
		{
		    //UE_LOG( LogEvolvingBehavior, Error, TEXT( "Failed to convert Decorator: %s" ),
		      //  *decoratorGene->ToString() );
		    continue;
		}
        
        //UE_LOG( LogEvolvingBehavior, Error, TEXT( "Converted Decorator: %s" ),
          //     *decoratorGene->ToString() );

	    decorator->InitializeNode( compositeParent, *ExecutionIndex, 0, treeDepth );
	    *ExecutionIndex += 1;
	    decorator->InitializeParentLink( childIdx );

	    childContainer->Decorators.Add( decorator );
	}
}

void UChromosomeToBTConverter::ConvertRootDecorators( UBehaviorTree* behaviorTree, UBTChromosome* chromosome ) const
{
    for( int i = 0; i < chromosome->GetNumDecorators(); ++i )
	{
	    UBTDecoratorGene* decoratorGene = chromosome->GetDecorator( i );

	    UBTDecorator* decorator = decoratorGene->BuildPhenotype( behaviorTree );

	    if( nullptr == decorator )
		{
		    //UE_LOG( LogEvolvingBehavior, Error, TEXT( "Failed to convert Root Decorator: %s" ),
		      //  *decoratorGene->ToString() );
		    continue;
		}
        
        //UE_LOG( LogEvolvingBehavior, Error, TEXT( "Converted Root Decorator: %s" ),
          //     *decoratorGene->ToString() );

	    decorator->InitializeNode( nullptr, MAX_uint16, 0, 0 );

	    behaviorTree->RootDecorators.Add( decorator );
	}
}

FBTCompositeChild& UChromosomeToBTConverter::InitEmptyCompositeChild( UBTCompositeNode* composite ) const
{
    TArray<UBTService*> Services;
    composite->Services = Services;
    
    int32 childIndex = composite->Children.AddDefaulted();
    FBTCompositeChild& childContainer = composite->Children[childIndex];

    childContainer.ChildTask = nullptr;
    childContainer.ChildComposite = nullptr;
    TArray<UBTDecorator*> Decorators;
    childContainer.Decorators = Decorators;
    TArray<FBTDecoratorLogic> DecoratorOperations;
    childContainer.DecoratorOps = DecoratorOperations;

    return childContainer;
}
