#include "BTMappedServiceGene.h"

#include "EvolvingBehavior.h"

#include "BehaviorTree/BTService.h"
#include "NodeMapID.h"

#include "NodeMapper.h"

void UBTMappedServiceGene::Init( UServiceInjector* injector, FNodeMapID id )
{
    ServiceInjector = injector;
    mapID = id;
}

UBTService* UBTMappedServiceGene::BuildPhenotype( UObject* outer )
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
    UBTService* resultService = Cast<UBTService>( DuplicateObject( result, outer, uniqueName ) );

    if( nullptr == resultService )
	{
	    UE_LOG( LogEvolvingBehaviorError, Error, TEXT( "Error: could not copy/cast node: %s. Result was: %s" ),
	        *mapID.ToString(), *result->GetNodeName() );
	    return nullptr;
	}

    return resultService;
}

UBTServiceGene* UBTMappedServiceGene::DuplicateService( UObject* outer )
{
    UBTMappedServiceGene* other = DuplicateObject<UBTMappedServiceGene>( this, outer );

    return other;
}
