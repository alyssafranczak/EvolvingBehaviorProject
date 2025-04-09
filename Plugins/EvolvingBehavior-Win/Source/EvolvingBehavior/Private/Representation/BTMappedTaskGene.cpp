#include "BTMappedTaskGene.h"

#include "EvolvingBehavior.h"

#include "BehaviorTree/BTTaskNode.h"

void UBTMappedTaskGene::Init( UServiceInjector* injector, FNodeMapID id )
{
    ServiceInjector = injector;
    mapID = id;
}

UBTTaskNode* UBTMappedTaskGene::BuildPhenotype( UObject* outer )
{
    UNodeMapper* mapper = ServiceInjector->GetService<UNodeMapper>();
    UBTNode* result = nullptr;
    ensure( nullptr != mapper );
    if( !mapper->TryGetTemplate( mapID, result ) )
	{
	    UE_LOG(
	        LogEvolvingBehaviorError, Error, TEXT( "Error: Could not get template node: %s" ), *mapID.ToString() );
	    return nullptr;
	}

    if( nullptr == result )
	{
	    UE_LOG( LogEvolvingBehaviorError, Error, TEXT( "Error: got nullptr template node: %s" ), *mapID.ToString() );
	    return nullptr;
	}

    FName uniqueName = MakeUniqueObjectName( outer, result->StaticClass(), FName( *mapID.ToString() ) );
    UBTTaskNode* resultTask = Cast<UBTTaskNode>( DuplicateObject( result, outer, uniqueName ) );

    if( nullptr == resultTask )
	{
	    UE_LOG( LogEvolvingBehaviorError, Error, TEXT( "Error: could not copy/cast node: %s. Result was: %s" ),
	        *mapID.ToString(), *result->GetNodeName() );
	    return nullptr;
	}

    return resultTask;
}

UBTTaskNodeGene* UBTMappedTaskGene::DuplicateTask( UObject* outer )
{
    UBTMappedTaskGene* other = DuplicateObject<UBTMappedTaskGene>(this, outer);
    
    return other;
}
