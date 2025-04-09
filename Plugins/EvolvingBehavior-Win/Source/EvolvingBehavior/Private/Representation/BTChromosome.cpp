#include "BTChromosome.h"

#include "EvolvingBehavior.h"

#include "BTCompositeNodeGene.h"
#include "BehaviorTreeUtils.h"
#include "BTDecoratorGene.h"

void UBTChromosome::SetRoot( UBTCompositeNodeGene* node ) { rootNode = node; }

FString UBTChromosome::ToString()
{
    FString result = "Chromosome:";

    result += UBehaviorTreeUtils::DecoratorsToString( 0, decorators );

    if( nullptr == rootNode )
	{
	    return result;
	}

    result += UBehaviorTreeUtils::CompositeToString( 0, rootNode );

    return result;
}

void UBTChromosome::ToLog()
{
    UBehaviorTreeUtils::DecoratorsToLog( 0, decorators );

    if( nullptr == rootNode )
	{
	    return;
	}

    UBehaviorTreeUtils::CompositeToLog( 0, rootNode );
    return;
}

int32 UBTChromosome::RemoveDecorator( UBTDecoratorGene* decorator )
{
    int32 decoratorIdx;
    if( !decorators.Find( decorator, decoratorIdx ) )
	{
	    return -1;
	}
    decorators.RemoveAt( decoratorIdx );
    return decoratorIdx;
}

UBTChromosome* UBTChromosome::Duplicate( UObject* outer )
{
    UBTChromosome* newChromosome = NewObject<UBTChromosome>( UBTChromosome::StaticClass() );
	newChromosome->RegisterInitialBTAsset(initialBTAsset);

    if( nullptr != rootNode )
	{
	    UBTCompositeNodeGene* copy = rootNode->DuplicateComposite( newChromosome );
	    ensure( nullptr != copy );
	    newChromosome->SetRoot( copy );
	}

    for( int i = 0; i < decorators.Num(); ++i )
	{
	    UBTDecoratorGene* copy = decorators[i]->DuplicateDecorator( newChromosome );
	    ensure( nullptr != copy );
	    newChromosome->AddDecorator( copy );
	}

    return newChromosome;
}

void UBTChromosome::SetServiceInjector( UServiceInjector* serviceInjector )
{
	for( auto node : decorators )
	{
		node->SetServiceInjector(serviceInjector);
	}

	rootNode->SetServiceInjector(serviceInjector);
}