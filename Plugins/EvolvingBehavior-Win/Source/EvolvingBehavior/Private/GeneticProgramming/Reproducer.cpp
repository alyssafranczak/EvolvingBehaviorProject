#include "Reproducer.h"

#include "EvolvingBehavior.h"

#include "BTChromosome.h"

TArray<UBTChromosome*> UReproducer::Reproduce( TArray<UBTChromosome*> parents )
{
    TArray<UBTChromosome*> children;

    if( parents.Num() < 1 )
	{
	    UE_LOG( LogEvolvingBehaviorError, Error, TEXT( "No parents provided - cannot produce offspring." ) );
	    return children;
	}

    for( int i = 0; i < parents.Num(); ++i )
	{
	    FName uniqueName = MakeUniqueObjectName( NULL, parents[i]->StaticClass(), FName( TEXT( "Child" ) ) );
	    UBTChromosome* child = parents[i]->Duplicate( this );

	    children.Add( child );
	}

    return children;
}

void UReproducer::Init( UServiceInjector* serviceInjector )
{
    ServiceInjector = serviceInjector;
}