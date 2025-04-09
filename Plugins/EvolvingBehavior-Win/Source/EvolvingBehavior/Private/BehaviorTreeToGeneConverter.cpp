#include "BehaviorTreeToGeneConverter.h"

#include "EvolvingBehavior.h"

#include "BehaviorTree/BTCompositeNode.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BehaviorTree/BTService.h"
#include "BehaviorTree/BTDecorator.h"
#include "BehaviorTree/BTNode.h"
#include "BehaviorTree/BehaviorTree.h"

#include "BTChromosome.h"
#include "BTChildContainerGene.h"
#include "BTCompositeNodeGene.h"
#include "BTTaskNodeGene.h"
#include "BTMappedCompositeGene.h"
#include "BTMappedDecoratorGene.h"
#include "BTMappedServiceGene.h"
#include "BTMappedTaskGene.h"
#include "NodeMapper.h"

void UBehaviorTreeToGeneConverter::Init( UServiceInjector* injector ) {
    serviceInjector = injector;
    mapper = injector->GetService<UNodeMapper>();
}

UBTChromosome* UBehaviorTreeToGeneConverter::Convert( UObject* outer, const UBehaviorTree* behaviorTree ) const
{
    UBTChromosome* chromosome = NewObject<UBTChromosome>( outer, UBTChromosome::StaticClass() );

    if( nullptr == mapper )
	{
	    //UE_LOG( LogEvolvingBehaviorError, Error, TEXT( "No mapper initialized!" ) );
	    return nullptr;
	}
	
    if( nullptr == behaviorTree->RootNode )
	{
	    return chromosome;
	}

    UBTCompositeNodeGene* rootGene = Convert( chromosome, behaviorTree->RootNode );
    chromosome->SetRoot( rootGene );

    AddRootDecorators( chromosome, behaviorTree );

    return chromosome;
}

UBTCompositeNodeGene* UBehaviorTreeToGeneConverter::Convert( UObject* outer, const UBTCompositeNode* node ) const
{
    UBTMappedCompositeGene* gene = NewObject<UBTMappedCompositeGene>( outer, UBTMappedCompositeGene::StaticClass() );
    
    gene->Init( serviceInjector, mapper->GetOrAddMapping( node ) );

    ConvertAndAddServices( gene, node );

    TArray<FBTCompositeChild> children = node->Children;
    for( int i = 0; i < children.Num(); ++i )
	{
	    FBTCompositeChild child = children[i];

	    UBTChildContainerGene* geneChild
	        = NewObject<UBTChildContainerGene>( gene, UBTChildContainerGene::StaticClass() );

	    ConvertAndAddDecorators( geneChild, child );

	    if( child.ChildComposite )
		{
            //UE_LOG( LogEvolvingBehaviorError, Error, TEXT( "ChildComposite converted to gene" ));
		    UBTCompositeNodeGene* compositeChildNode = Convert( geneChild, child.ChildComposite );
		    geneChild->SetChild( compositeChildNode );
		    gene->AddChild( geneChild );
		}
	    else if( child.ChildTask )
		{
            //UE_LOG( LogEvolvingBehaviorError, Error, TEXT( "ChildTask converted to gene" ));
		    UBTTaskNodeGene* taskChildNode = Convert( geneChild, child.ChildTask );
		    geneChild->SetChild( taskChildNode );
		    gene->AddChild( geneChild );
		}
	}

    return gene;
}

UBTTaskNodeGene* UBehaviorTreeToGeneConverter::Convert( UObject* outer, const UBTTaskNode* node ) const
{
    UBTMappedTaskGene* gene = NewObject<UBTMappedTaskGene>( outer, UBTMappedTaskGene::StaticClass() );

    gene->Init( serviceInjector, mapper->GetOrAddMapping( node ) );

    ConvertAndAddServices( gene, node );
    
    return gene;
}

UBTServiceGene* UBehaviorTreeToGeneConverter::Convert( UObject* outer, const UBTService* node ) const
{
    UBTMappedServiceGene* gene = NewObject<UBTMappedServiceGene>( outer, UBTMappedServiceGene::StaticClass() );
    
    gene->Init( serviceInjector, mapper->GetOrAddMapping( node ) );
    
    return gene;
}

UBTDecoratorGene* UBehaviorTreeToGeneConverter::Convert( UObject* outer, const UBTDecorator* node ) const
{
    UBTMappedDecoratorGene* gene = NewObject<UBTMappedDecoratorGene>( outer, UBTMappedDecoratorGene::StaticClass() );

    gene->Init( serviceInjector, mapper->GetOrAddMapping( node ) );

    return gene;
}

void UBehaviorTreeToGeneConverter::ConvertAndAddServices( UBTCompositeNodeGene* compositeGene,
                                                           const UBTCompositeNode* compositeNode ) const
{
    for( int d = 0; d < compositeNode->Services.Num(); ++d )
    {
        UBTService* service = compositeNode->Services[d];
        //UE_LOG( LogEvolvingBehaviorError, Error, TEXT( "Service converted to gene" ));
        UBTServiceGene* serviceGene = Convert( compositeGene, service );
        compositeGene->AddService( serviceGene );
    }
}

void UBehaviorTreeToGeneConverter::ConvertAndAddServices( UBTTaskNodeGene* taskGene,
                                                         const UBTTaskNode* taskNode ) const
{
    for( int d = 0; d < taskNode->Services.Num(); ++d )
    {
        UBTService* service = taskNode->Services[d];
        //UE_LOG( LogEvolvingBehaviorError, Error, TEXT( "Service converted to gene" ));
        UBTServiceGene* serviceGene = Convert( taskGene, service );
        taskGene->AddService( serviceGene );
    }
}

void UBehaviorTreeToGeneConverter::ConvertAndAddDecorators( UBTChildContainerGene* childGene,
    const FBTCompositeChild childNode ) const
{
    for( int d = 0; d < childNode.Decorators.Num(); ++d )
	{
	    UBTDecorator* decorator = childNode.Decorators[d];
        //UE_LOG( LogEvolvingBehaviorError, Error, TEXT( "Decorator converted to gene" ));
	    UBTDecoratorGene* decoratorGene = Convert( childGene, decorator );
	    childGene->AddDecorator( decoratorGene );
	}
}

void UBehaviorTreeToGeneConverter::AddRootDecorators( UBTChromosome* chromosome,
    const UBehaviorTree* behaviorTree ) const
{
    for( int d = 0; d < behaviorTree->RootDecorators.Num(); ++d )
	{
	    UBTDecorator* decorator = behaviorTree->RootDecorators[d];
        //UE_LOG( LogEvolvingBehaviorError, Error, TEXT( "Decorator converted to gene" ));
	    UBTDecoratorGene* decoratorGene = Convert( chromosome, decorator );
	    chromosome->AddDecorator( decoratorGene );
	}
}
