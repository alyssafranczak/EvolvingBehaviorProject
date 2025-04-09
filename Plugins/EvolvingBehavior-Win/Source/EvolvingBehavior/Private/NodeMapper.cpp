#include "NodeMapper.h"

#include "EvolvingBehavior.h"

#include "BehaviorTree/BTCompositeNode.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BehaviorTree/BTDecorator.h"
#include "BehaviorTree/BTService.h"

#include "NodeMapID.h"

void UNodeMapper::AddMapping( UBTNode* node, FNodeMapID nodeID )
{
    if( nodes.Contains( nodeID ) )
	{
	    return;
	}
	
    nodes.Add( nodeID, node );
}

void UNodeMapper::Register( const UBTCompositeNode* node )
{
    FNodeMapID nodeID = GetMapID( node );
    if( nodes.Contains( nodeID ) )
	{
	    return;
	}
	
    FName uniqueName = MakeUniqueObjectName( this, node->StaticClass(), FName( *nodeID.ToString() ) );
    UBTCompositeNode* copy = Cast<UBTCompositeNode>( DuplicateObject( node, this, uniqueName ) );
    Reset( copy );
	
	composites.Add( nodeID );
	
	AddMapping( copy, nodeID );
}

void UNodeMapper::Register( const UBTTaskNode* node )
{
    FNodeMapID nodeID = GetMapID( node );
    if( nodes.Contains( nodeID ) )
	{
	    return;
	}
	
    FName uniqueName = MakeUniqueObjectName( this, node->StaticClass(), FName( *nodeID.ToString() ) );
    UBTTaskNode* copy = Cast<UBTTaskNode>( DuplicateObject( node, this, uniqueName ) );
    Reset( copy );
	
	tasks.Add( nodeID );
	
	AddMapping( copy, nodeID );
}

void UNodeMapper::Register( const UBTService* node )
{
    FNodeMapID nodeID = GetMapID( node );
    if( nodes.Contains( nodeID ) )
    {
        return;
    }
    
    FName uniqueName = MakeUniqueObjectName( this, node->StaticClass(), FName( *nodeID.ToString() ) );
    UBTService* copy = Cast<UBTService>( DuplicateObject( node, this, uniqueName ) );
    Reset( copy );
    
    services.Add( nodeID );
    
    AddMapping( copy, nodeID );
}

void UNodeMapper::Register( const UBTDecorator* node )
{
    FNodeMapID nodeID = GetMapID( node );
    if( nodes.Contains( nodeID ) )
	{
	    return;
	}
	
    FName uniqueName = MakeUniqueObjectName( this, node->StaticClass(), FName( *nodeID.ToString() ) );
    UBTDecorator* copy = Cast<UBTDecorator>( DuplicateObject( node, this, uniqueName ) );
    Reset( copy );
	
	decorators.Add( nodeID );
	
	AddMapping( copy, nodeID );
}

void UNodeMapper::Reset( UBTNode* node )
{
    UBTCompositeNode* composite = Cast<UBTCompositeNode>( node );
    if( nullptr != composite )
	{
	    composite->Children.Empty();
	}
}

FNodeMapID UNodeMapper::GetMapID( const UBTNode* node ) const
{
    FString nodeName = node->GetNodeName();

    FNodeMapID mapID( nodeName );
    return mapID;
}

bool UNodeMapper::TryGetMapping( const UBTNode* node, FNodeMapID& result ) const
{
    FNodeMapID nodeName = GetMapID( node );

    if( !nodes.Contains( nodeName ) )
	{
	    return false;
	}

    result = nodeName;
    return true;
}

bool UNodeMapper::TryGetTemplate( FNodeMapID mapping, UBTNode*& result ) const
{
    if( !nodes.Contains( mapping ) )
	{
	    return false;
	}
    check(nodes[mapping] != nullptr);
    check(result == nullptr);
    result = nodes[mapping];
    return true;
}

FNodeMapID UNodeMapper::GetOrAddMapping( const UBTCompositeNode* node )
{
    FNodeMapID nodeName = GetMapID( node );

    if( !nodes.Contains( nodeName ) )
	{
	    Register( node );
	}

    return nodeName;
}

FNodeMapID UNodeMapper::GetOrAddMapping( const UBTTaskNode* node )
{
    FNodeMapID nodeName = GetMapID( node );

    if( !nodes.Contains( nodeName ) )
	{
	    Register( node );
	}

    return nodeName;
}

FNodeMapID UNodeMapper::GetOrAddMapping( const UBTService* node )
{
    FNodeMapID nodeName = GetMapID( node );
    
    if( !nodes.Contains( nodeName ) )
    {
        Register( node );
    }
    
    return nodeName;
}

FNodeMapID UNodeMapper::GetOrAddMapping( const UBTDecorator* node )
{
    FNodeMapID nodeName = GetMapID( node );

    if( !nodes.Contains( nodeName ) )
	{
	    Register( node );
	}

    return nodeName;
}

FNodeMapID UNodeMapper::GetRandomCompositeID( URandomGen* randomGen )
{
    if (composites.Num() == 0)
    {
        return FNodeMapID::INVALID_ID;
    }

    int32 randIdx = randomGen->UniformIntInRange( 0, composites.Num() - 1 );

    return composites[randIdx];
}

FNodeMapID UNodeMapper::GetRandomTaskID( URandomGen* randomGen )
{
    if (tasks.Num() == 0)
    {
        return FNodeMapID::INVALID_ID;
    }

    int32 randIdx = randomGen->UniformIntInRange( 0, tasks.Num() - 1 );

    return tasks[randIdx];
}

FNodeMapID UNodeMapper::GetRandomServiceID( URandomGen* randomGen )
{
    if (services.Num() == 0)
    {
        return FNodeMapID::INVALID_ID;
    }
    
    int32 randIdx = randomGen->UniformIntInRange( 0, services.Num() - 1 );
    
    return services[randIdx];
}

FNodeMapID UNodeMapper::GetRandomDecoratorID( URandomGen* randomGen )
{
    if (decorators.Num() == 0)
    {
        return FNodeMapID::INVALID_ID;
    }

    int32 randIdx = randomGen->UniformIntInRange( 0, decorators.Num() - 1 );

    return decorators[randIdx];
}
