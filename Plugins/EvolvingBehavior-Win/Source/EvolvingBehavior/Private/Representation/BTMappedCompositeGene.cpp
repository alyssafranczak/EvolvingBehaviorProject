#include "BTMappedCompositeGene.h"

#include "EvolvingBehavior.h"

#include "BTChildContainerGene.h"
#include "BTMappedServiceGene.h"
#include "BTServiceGene.h"

#include "BehaviorTree/BTCompositeNode.h"

void UBTMappedCompositeGene::Init( UServiceInjector* injector, FNodeMapID id )
{
    ServiceInjector = injector;
    mapID = id;
}

UBTCompositeNode* UBTMappedCompositeGene::BuildPhenotype( UObject* outer )
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
	    UE_LOG(
	        LogEvolvingBehaviorError, Error, TEXT( "Error: got nullptr template node: %s" ), *mapID.ToString() );
	    return nullptr;
	}

    FName uniqueName = MakeUniqueObjectName( outer, result->StaticClass(), FName( *mapID.ToString() ) );
    UBTCompositeNode* resultComposite = Cast<UBTCompositeNode>( DuplicateObject( result, outer, uniqueName ) );

    if( nullptr == resultComposite )
	{
	    UE_LOG( LogEvolvingBehaviorError, Error, TEXT( "Error: could not copy/cast node: %s. Result was: %s" ),
	        *mapID.ToString(), *result->GetNodeName() );
	    return nullptr;
	}

    return resultComposite;
}

UBTCompositeNodeGene* UBTMappedCompositeGene::DuplicateComposite( UObject* outer )
{
    UBTMappedCompositeGene* other = DuplicateObject<UBTMappedCompositeGene>(this, outer);
    
    return other;
}
