#include "BTMappedDecoratorGene.h"

#include "EvolvingBehavior.h"

#include "BehaviorTree/BTDecorator.h"
#include "NodeMapID.h"

#include "NodeMapper.h"

void UBTMappedDecoratorGene::Init( UServiceInjector* injector, FNodeMapID id )
{
    ServiceInjector = injector;
    mapID = id;
}

UBTDecorator* UBTMappedDecoratorGene::BuildPhenotype( UObject* outer )
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
    UBTDecorator* resultDecorator = Cast<UBTDecorator>( DuplicateObject( result, outer, uniqueName ) );

    if( nullptr == resultDecorator )
	{
	    UE_LOG( LogEvolvingBehaviorError, Error, TEXT( "Error: could not copy/cast node: %s. Result was: %s" ),
	        *mapID.ToString(), *result->GetNodeName() );
	    return nullptr;
	}

    return resultDecorator;
}

UBTDecoratorGene* UBTMappedDecoratorGene::DuplicateDecorator( UObject* outer )
{
    UBTMappedDecoratorGene* other = DuplicateObject<UBTMappedDecoratorGene>( this, outer );

    return other;
}
