#include "BTChildContainerGene.h"

#include "EvolvingBehavior.h"

#include "BTCompositeNodeGene.h"
#include "BTTaskNodeGene.h"
#include "BTDecoratorGene.h"

bool UBTChildContainerGene::SetChild( UBTTaskNodeGene* gene )
{
    if( nullptr != taskChild || nullptr != compositeChild )
	{
	    UE_LOG( LogEvolvingBehaviorError, Error, TEXT( "Error: composite child gene already has node set." ) );
	    return false;
	}

    taskChild = gene;
    return true;
}

bool UBTChildContainerGene::SetChild( UBTCompositeNodeGene* gene )
{
    if( nullptr != taskChild || nullptr != compositeChild )
	{
	    UE_LOG( LogEvolvingBehaviorError, Error, TEXT( "Error: composite child gene already has node set." ) );
	    return false;
	}

    compositeChild = gene;
    return true;
}

void UBTChildContainerGene::ReplaceChild( UBTCompositeNodeGene* gene )
{
    taskChild = nullptr;
    compositeChild = gene;
}

void UBTChildContainerGene::ReplaceChild( UBTTaskNodeGene* gene )
{
    taskChild = gene;
    compositeChild = nullptr;
}

int32 UBTChildContainerGene::RemoveDecorator( UBTDecoratorGene* decorator )
{
    int32 decoratorIdx;
    if( !decorators.Find( decorator, decoratorIdx ) )
	{
	    return -1;
	}
    decorators.RemoveAt( decoratorIdx );
    return decoratorIdx;
}

UBTChildContainerGene* UBTChildContainerGene::Duplicate( UObject* outer )
{
    UBTChildContainerGene* other = NewObject<UBTChildContainerGene>( outer, UBTChildContainerGene::StaticClass() );

    if( IsALeaf() )
	{
	    UBTTaskNodeGene* copy = GetTaskChild()->DuplicateTask( other );
	    ensure( nullptr != copy );
	    other->SetChild( copy );
	}
    else
	{
	    UBTCompositeNodeGene* copy = GetCompositeChild()->DuplicateComposite( other );
	    ensure( nullptr != copy );
	    other->SetChild( copy );
	}

    for( int i = 0; i < decorators.Num(); ++i )
	{
	    UBTDecoratorGene* copy = decorators[i]->DuplicateDecorator( other );
	    ensure( nullptr != copy );
	    other->AddDecorator( copy );
	}

    return other;
}

void UBTChildContainerGene::SetServiceInjector( UServiceInjector* serviceInjector )
{
	for ( auto decorator : decorators )
	{
		decorator->SetServiceInjector(serviceInjector);
	}
	
	if (IsALeaf() )
	{
		taskChild->SetServiceInjector(serviceInjector);
	}
	else
	{
		compositeChild->SetServiceInjector(serviceInjector);
	}
}
