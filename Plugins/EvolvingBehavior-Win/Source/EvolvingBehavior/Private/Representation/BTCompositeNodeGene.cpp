#include "BTCompositeNodeGene.h"

#include "EvolvingBehavior.h"

#include "BTChildContainerGene.h"
#include "BTServiceGene.h"

void UBTCompositeNodeGene::AddChild( UBTChildContainerGene* child )
{ 
    check(child->GetOuter() == this);
    children.Add( child );
}

int32 UBTCompositeNodeGene::InsertChild( UBTChildContainerGene* child, int32 idx )
{ 
    check(child->GetOuter() == this);
    return children.Insert( child, idx ); 
}

int32 UBTCompositeNodeGene::RemoveService( UBTServiceGene* service )
{
    int32 serviceIdx;
    if( !services.Find( service, serviceIdx ) )
    {
        return -1;
    }
    services.RemoveAt( serviceIdx );
    return serviceIdx;
}

void UBTCompositeNodeGene::SetServiceInjector(UServiceInjector* serviceInjector)
{
    Super::SetServiceInjector(serviceInjector);

    for (auto child : children)
    {
        child->SetServiceInjector(serviceInjector);
    }
    
    for ( auto service : services )
    {
        service->SetServiceInjector(serviceInjector);
    }
}