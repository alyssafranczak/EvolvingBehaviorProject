
#include "BTChromosomeUtils.h"

#include "EvolvingBehavior.h"

#include "BTChromosome.h"
#include "RandomGen.h"
#include "BTDecoratorGene.h"
#include "BTServiceGene.h"
#include "BTChildContainerGene.h"
#include <queue>
#include <deque>
#include "Core/Public/Math/UnrealMathUtility.h"
#include <utility>

bool BTChromosomeUtils::FindRandomNodeOfType( UBTChromosome* chromosome,
    URandomGen* randomGen,
    UClass* nodeClass,
    UBTCompositeNodeGene*& outParent,
    uint16& outChildIdx,
    UBTNodeGene*& resultNode )
{
    ensure( chromosome != nullptr );
    ensure( randomGen != nullptr );
    ensure( nodeClass != nullptr );

    bool foundChild = false;

    if( nullptr == chromosome->GetRoot() )
	{
	    UE_LOG( LogEvolvingBehaviorError, Error, TEXT( "Error: null root!" ) );
	    return false;
	}

    int numValidNodes = BTChromosomeUtils::CountNumNodesOfType( chromosome, nodeClass );

    bool rootIsDesiredType = chromosome->GetRoot()->GetClass()->IsChildOf( nodeClass );

    int32_t randomNodeIdx = randomGen->UniformIntInRange( 1, numValidNodes );

    if( randomNodeIdx == 1 && rootIsDesiredType )
	{
	    // Return false to indicate that we randomly selected the root node, which has no parent nor child index.
	    return false;
	}

    int numNodes = randomNodeIdx;
    DepthFirstSearch( chromosome, [&]( UBTNodeGene* node, UBTCompositeNodeGene* parent, uint16 childIdx ) {
        if( nullptr == node )
        {
            UE_LOG( LogEvolvingBehaviorError, Error, TEXT( "Error: null node in parent: %i" ), childIdx );
            return true;
        }
        if( node->GetClass()->IsChildOf( nodeClass ) )
        {
            numNodes--;
            if( numNodes <= 0 )
            {
                outParent = parent;
                outChildIdx = childIdx;
                resultNode = node;
                return false;
            }
        }
        return true;
    } );

    return foundChild;
}

int BTChromosomeUtils::CountNumNodesOfType( UBTChromosome* chromosome, UClass* nodeClass )
{
    int numNodes = 0;
    DepthFirstSearch( chromosome, [&]( UBTNodeGene* node, UBTCompositeNodeGene* compositeParent, uint16 childIdx ) {
        if( node->GetClass()->IsChildOf( nodeClass ) )
        {
            numNodes++;
        }
        return true;
    } );

    return numNodes;
}

void BTChromosomeUtils::DepthFirstSearch( UBTChromosome* chromosome,
    std::function<bool( UBTNodeGene*, UBTCompositeNodeGene*, uint16 )> nodeFunc )
{
    UBTCompositeNodeGene* rootNode = chromosome->GetRoot();
    if( nullptr == rootNode )
	{
	    return;
	}

    for( int i = 0; i < chromosome->GetNumDecorators(); ++i )
	{
	    if( !nodeFunc( chromosome->GetDecorator( i ), nullptr, 0 ) )
		{
		    return;
		}
	}

    TArray<UBTNodeGene*> seenNodes;
    DepthFirstSearch( rootNode, nullptr, 0, nodeFunc, seenNodes );
}

bool BTChromosomeUtils::DepthFirstSearch( UBTCompositeNodeGene* composite,
    UBTCompositeNodeGene* parent,
    uint16 compositeIdx,
    std::function<bool( UBTNodeGene*, UBTCompositeNodeGene*, uint16 )> nodeFunc,
    TArray<UBTNodeGene*> seenNodes )
{
    if (seenNodes.Contains(composite))
    {
        UE_LOG(LogEvolvingBehaviorError, Error, TEXT("Loop in depth first search. Node we have seen before: %s"), *composite->GetName());
        return false;
    }
    seenNodes.Add(composite);

    if( !nodeFunc( composite, parent, compositeIdx ) )
	{
	    return false;
	}

    for( int childIdx = 0; childIdx < composite->GetNumChildren(); ++childIdx )
	{
	    UBTChildContainerGene* child = composite->GetChild( childIdx );
        check(child->GetOuter() == composite);
        check(child != nullptr);
	    for( int i = 0; i < child->GetNumDecorators(); ++i )
		{
		    if( !nodeFunc( child->GetDecorator( i ), composite, childIdx ) )
			{
			    return false;
			}
		}
	    if( child->IsALeaf() )
		{
            UBTTaskNodeGene* childNode = child->GetTaskChild();
            check(childNode->GetOuter() == child);
            for( int i = 0; i < childNode->GetNumServices(); ++i )
            {
                if( !nodeFunc( childNode->GetService( i ), composite, childIdx ) )
                {
                    return false;
                }
            }
		    if( !nodeFunc( childNode, composite, childIdx ) )
			{
			    return false;
			}
		}
	    else
		{
            UBTCompositeNodeGene* childNode = child->GetCompositeChild();
            check(childNode->GetOuter() == child);
            for( int i = 0; i < childNode->GetNumServices(); ++i )
            {
                if( !nodeFunc( childNode->GetService( i ), composite, childIdx ) )
                {
                    return false;
                }
            }
		    if( !DepthFirstSearch( childNode, composite, childIdx, nodeFunc, seenNodes ) )
			{
			    return false;
			}
		}
	}
    return true;
}

bool BTChromosomeUtils::CheckForPointerCollisions( UBTChromosome* chromosome, UBTChromosome* other )
{
    UBTCompositeNodeGene* root = chromosome->GetRoot();
    UBTCompositeNodeGene* otherRoot = other->GetRoot();

    if( chromosome->ToString() != other->ToString() )
	{
	    UE_LOG( LogEvolvingBehaviorError, Error, TEXT( "ERROR: DIFFERENT ToStrings: %s, %s" ),
	        *chromosome->ToString(), *other->ToString() );
	}

    if( root == otherRoot )
	{
	    UE_LOG( LogEvolvingBehaviorError, Error, TEXT( "ERROR: SAME ROOT: %s, %s" ), *root->ToString(),
	        *otherRoot->ToString() );
	    return true;
	}

    if( root->GetNumChildren() != otherRoot->GetNumChildren() )
	{
	    UE_LOG( LogEvolvingBehaviorError, Error, TEXT( "ERROR: DIFFERENT CHILDREN COUNT: %s, %s" ),
	        *chromosome->ToString(), *other->ToString() );
	    return true;
	}

    for( int childIdx = 0; childIdx < root->GetNumChildren(); ++childIdx )
	{
	    UBTChildContainerGene* child = root->GetChild( childIdx );
        check(child->GetOuter() == root);
	    UBTChildContainerGene* otherChild = otherRoot->GetChild( childIdx );
        check(otherChild->GetOuter() == root);
	    if( child->IsALeaf() )
		{
		    if( !otherChild->IsALeaf() )
			{
			    UE_LOG( LogEvolvingBehaviorError, Error, TEXT( "ERROR: DIFFERENT LEAF TYPE IN ROOT" ) );
			    return true;
			}

		    if( child->GetTaskChild() == otherChild->GetTaskChild() )
			{
			    UE_LOG( LogEvolvingBehaviorError, Error, TEXT( "ERROR: SAME TASK: %s, %s" ),
			        *child->GetTaskChild()->ToString(), *otherChild->GetTaskChild()->ToString() );
			    return true;
			}
		}
	    else
		{
		    if( otherChild->IsALeaf() )
			{
			    UE_LOG( LogEvolvingBehaviorError, Error, TEXT( "ERROR: DIFFERENT LEAF TYPE IN ROOT" ) );
			    return true;
			}

		    ensure( child->GetCompositeChild() != nullptr );
		    ensure( otherChild->GetCompositeChild() != nullptr );

		    if( CheckForPointerCollisions( child->GetCompositeChild(), otherChild->GetCompositeChild() ) )
			{
			    UE_LOG( LogEvolvingBehaviorError, Error, TEXT( "ERROR: SAME INSIDE ROOT" ) );
			    return true;
			}
		}
	}

    return false;
}

bool BTChromosomeUtils::CheckForPointerCollisions( UBTCompositeNodeGene* composite, UBTCompositeNodeGene* other )
{
    ensure( nullptr != composite );
    ensure( nullptr != other );

    if( composite == other )
	{
	    UE_LOG( LogEvolvingBehaviorError, Error, TEXT( "ERROR: SAME COMPOSITE: %s, %s" ), *composite->ToString(),
	        *other->ToString() );
	    return true;
	}

    if( composite->GetNumChildren() != other->GetNumChildren() )
	{
	    UE_LOG( LogEvolvingBehaviorError, Error, TEXT( "ERROR: DIFFERENT COMPOSITE CHILDREN COUNT: %s, %s" ),
	        *composite->ToString(), *other->ToString() );
	    return true;
	}
    
    if( composite->GetNumServices() != other->GetNumServices() )
    {
        UE_LOG( LogEvolvingBehaviorError, Error, TEXT( "ERROR: DIFFERENT SERVICE COUNT: %s, %s" ),
               *composite->ToString(), *other->ToString() );
        return true;
    }
    
    for( int i = 0; i < composite->GetNumServices(); ++i )
    {
        if( composite->GetService( i ) == other->GetService( i ) )
        {
            UE_LOG( LogEvolvingBehaviorError, Error, TEXT( "ERROR: SAME SERVICE: %s, %s" ),
                   *composite->GetService( i )->ToString(), *other->GetService( i )->ToString() );
            return true;
        }
    }

    for( int childIdx = 0; childIdx < composite->GetNumChildren(); ++childIdx )
	{
	    UBTChildContainerGene* child = composite->GetChild( childIdx );
	    UBTChildContainerGene* otherChild = other->GetChild( childIdx );
	    if( child == otherChild )
		{
		    UE_LOG( LogEvolvingBehaviorError, Error, TEXT( "ERROR: SAME CompositeGeneChild: %s, %s, %d" ),
		        *composite->ToString(), *other->ToString(), childIdx );
		    return true;
		}

	    if( child->GetNumDecorators() != otherChild->GetNumDecorators() )
		{
		    UE_LOG( LogEvolvingBehaviorError, Error, TEXT( "ERROR: DIFFERENT DECORATOR COUNT: %s, %s" ),
		        *composite->ToString(), *other->ToString() );
		    return true;
		}

	    for( int i = 0; i < child->GetNumDecorators(); ++i )
		{
		    if( child->GetDecorator( i ) == otherChild->GetDecorator( i ) )
			{
			    UE_LOG( LogEvolvingBehaviorError, Error, TEXT( "ERROR: SAME DECORATOR: %s, %s" ),
			        *child->GetDecorator( i )->ToString(), *otherChild->GetDecorator( i )->ToString() );
			    return true;
			}
		}

	    if( child->IsALeaf() )
		{
		    if( !otherChild->IsALeaf() )
			{
			    UE_LOG( LogEvolvingBehaviorError, Error, TEXT( "ERROR: DIFFERENT LEAF TYPE %s, %s" ),
			        *composite->ToString(), *other->ToString() );
			    return true;
			}

		    if( child->GetTaskChild() == otherChild->GetTaskChild() )
			{
			    UE_LOG( LogEvolvingBehaviorError, Error, TEXT( "ERROR: SAME TASK: %s, %s" ),
			        *child->GetTaskChild()->ToString(), *otherChild->GetTaskChild()->ToString() );
			    return true;
			}
		}
	    else
		{
		    if( otherChild->IsALeaf() )
			{
			    UE_LOG( LogEvolvingBehaviorError, Error, TEXT( "ERROR: DIFFERENT LEAF TYPE %s, %s" ),
			        *composite->ToString(), *other->ToString() );
			    return true;
			}

		    ensure( child->GetCompositeChild() != nullptr );
		    ensure( otherChild->GetCompositeChild() != nullptr );

		    if( CheckForPointerCollisions( child->GetCompositeChild(), otherChild->GetCompositeChild() ) )
			{
			    UE_LOG( LogEvolvingBehaviorError, Error, TEXT( "ERROR: SAME INSIDE: %s, %s" ),
			        *composite->ToString(), *other->ToString() );
			    return true;
			}
		}
	}
    return false;
}

bool BTChromosomeUtils::ContainsACycle( UBTChromosome* chromosome )
{
    TSet<UBTNodeGene*> seenNodes;
    bool containsCycle = false;
    BTChromosomeUtils::DepthFirstSearch(
        chromosome, [&]( UBTNodeGene* node, UBTCompositeNodeGene* parent, uint16 compositeIndex ) {
	    if( seenNodes.Contains( node ) )
	        {
		    UE_LOG( LogEvolvingBehaviorError, Error, TEXT( "ERROR: Node cycle! %s, parent %s" ),
		        *node->ToString(), *parent->ToString() );
		    containsCycle = true;
		    return false;
	        }

	    seenNodes.Add( node );
	    return true;
        } );

    return containsCycle;
}

// returns the lengths of the deepest path in the tree
int BTChromosomeUtils::DepthOfTree( UBTChromosome* chromosome )
{
    // gets the root of the chromosom
    UBTCompositeNodeGene* root = chromosome->GetRoot();
    
    // returns the depth of the tree starting at the root
    return DepthOfTreeHelp( chromosome, root, 1 );
}

// helper for DepthOfTree
int BTChromosomeUtils::DepthOfTreeHelp( UBTChromosome* chromosome, UBTCompositeNodeGene* composite, int maxDepth )
{
    // integer representing overall depth of the tree
    int depth = maxDepth;
    
    
    // for each child of the given node, if the child is a leaf, sets the total depth to
    // the max depth so far plus one
    for( int childIdx = 0; childIdx < composite->GetNumChildren(); ++childIdx )
    {
        // gets the composite gene child for this node
        UBTChildContainerGene* child = composite->GetChild( childIdx );
        
        if( child->IsALeaf() )
        {
            if ( maxDepth + 1 > depth )
            {
                depth = maxDepth + 1;
            }
        }
        // if not a leaf, call the helper with the new node and an incremented max depth
        else
        {
            int childDepth = DepthOfTreeHelp( chromosome, child->GetCompositeChild(), maxDepth + 1);
            if ( childDepth > depth )
            {
                depth = childDepth;
            }
        }
    }
    // return the maximum of the depths of all paths
    return depth;
}

void BTChromosomeUtils::BreadthFirstSearch( UBTChromosome* chromosome,
    std::function<bool( UBTNodeGene*, UBTCompositeNodeGene*, uint16, uint16 )> nodeFunc )
{
    std::deque< std::pair<UBTCompositeNodeGene*, uint16> > q;
    UBTCompositeNodeGene* rootNode = chromosome->GetRoot();
    if( nullptr == rootNode )
    {
        return;
    }
    
    for( int i = 0; i < chromosome->GetNumDecorators(); ++i )
    {
        if( !nodeFunc( chromosome->GetDecorator( i ), nullptr, i, 0 ) )
        {
            return;
        }
    }
    
    if( !nodeFunc( rootNode, nullptr, 0, 0 ) )
    {
        return;
    }
    std::pair<UBTCompositeNodeGene*, uint16> rootPair = std::make_pair( rootNode, 0 );
    q.push_back( rootPair );
    BreadthFirstSearchInternal( std::queue< std::pair<UBTCompositeNodeGene*, uint16> >(q), nodeFunc );
}

bool BTChromosomeUtils::BreadthFirstSearchInternal( std::queue< std::pair<UBTCompositeNodeGene*, uint16> > q,
    std::function<bool( UBTNodeGene*, UBTCompositeNodeGene*, uint16, uint16 )> nodeFunc )
{
    while( q.empty() == false ) {
        std::pair<UBTCompositeNodeGene*, uint16> curPair = q.front();
    	q.pop();
        UBTCompositeNodeGene* composite = curPair.first;
        uint16 curLayer = curPair.second;
        for( int childIdx = 0; childIdx < composite->GetNumChildren(); ++childIdx )
        {
            UBTChildContainerGene* child = composite->GetChild( childIdx );
            for( int i = 0; i < child->GetNumDecorators(); ++i )
            {
                if( !nodeFunc( child->GetDecorator( i ), composite, childIdx, curLayer + 1 ) )
                {
                    return false;
                }
            }
            
            if( child->IsALeaf() )
            {
                UBTTaskNodeGene* childNode = child->GetTaskChild();
                for( int i = 0; i < childNode->GetNumServices(); ++i )
                {
                    if( !nodeFunc( childNode->GetService( i ), composite, childIdx, curLayer + 1 ) )
                    {
                        return false;
                    }
                }
                if( !nodeFunc( childNode, composite, childIdx, curLayer + 1 ) )
                {
                    return false;
                }
            }
            else
            {
                UBTCompositeNodeGene* childNode = child->GetCompositeChild();
                for( int i = 0; i < childNode->GetNumServices(); ++i )
                {
                    if( !nodeFunc( childNode->GetService( i ), composite, childIdx, curLayer + 1 ) )
                    {
                        return false;
                    }
                }
                if( !nodeFunc( childNode, composite, childIdx, curLayer + 1 ) )
                {
                    return false;
                }
                
                std::pair<UBTCompositeNodeGene*, uint16> childToProcess = std::make_pair( childNode, curLayer + 1 );
                q.push( childToProcess );
            }
        }
    }
    return true;
}

bool BTChromosomeUtils::PickNodeAtLayer( UBTChromosome* chromosome, int layer, UBTCompositeNodeGene*& outParent, uint16& outChildIdx, UBTNodeGene*& resultNode )
{
    bool foundNode = false;
    int nodesSeen = 1;
    UBTNodeGene* curNode = nullptr;
    BreadthFirstSearch( chromosome, [&]( UBTNodeGene* nodeGene, UBTCompositeNodeGene* parent, uint16 childIdx, uint16 curLayer)
                       {
                           if ( curLayer == layer )
                           {
                               if( FMath::RandRange(0.0f,1.0f) < (1.0f / nodesSeen) ) {
                                   outParent = parent;
                                   outChildIdx = childIdx;
                                   resultNode = nodeGene;
                                   foundNode = true;
                                   nodesSeen++;
                               }
                               return true;
                           }
                           else if ( curLayer < layer )
                           {
                               return true;
                           }
                           else
                           {
                               return false;
                           }
                       } );
    return foundNode;
}
