#include "BTTaskNodeGene.h"
#include "BTServiceGene.h"

#include "EvolvingBehavior.h"


int32 UBTTaskNodeGene::RemoveService( UBTServiceGene* service )
{
    int32 serviceIdx;
    if( !services.Find( service, serviceIdx ) )
    {
        return -1;
    }
    services.RemoveAt( serviceIdx );
    return serviceIdx;
}


void UBTTaskNodeGene::SetServiceInjector(UServiceInjector* serviceInjector )
{
    Super::SetServiceInjector(serviceInjector);
    
    for ( auto service : services )
    {
        service->SetServiceInjector(serviceInjector);
    }
}