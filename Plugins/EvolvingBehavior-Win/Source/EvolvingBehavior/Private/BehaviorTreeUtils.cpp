#include "BehaviorTreeUtils.h"

#include "EvolvingBehavior.h"

#include "BehaviorTree/BTCompositeNode.h"
#include "BehaviorTree/BTDecorator.h"
#include "BehaviorTree/BTNode.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BehaviorTree/BTService.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"

#include "BTChildContainerGene.h"
#include "BTCompositeNodeGene.h"
#include "BTDecoratorGene.h"
#include "BTServiceGene.h"
#include "BTTaskNodeGene.h"

#include "VisualLogger/VisualLogger.h"

#include "NodeMapper.h"

#include <string>

void UBehaviorTreeUtils::PrintBehaviorTree( UBehaviorTree* behaviorTree, AActor* owner )
{
    if( nullptr != owner )
	{
	    UE_VLOG( owner, LogBehaviorTree, Display, TEXT( "Behavior Tree:" ) );
	}
    else
	{
	    UE_LOG( LogBehaviorTree, Display, TEXT( "Behavior Tree:" ) );
	}

    for( int d = 0; d < behaviorTree->RootDecorators.Num(); ++d )
	{
	    UBTDecorator* decorator = behaviorTree->RootDecorators[d];
	    if( nullptr == owner )
		{
		    UE_LOG( LogEvolvingBehavior, Display, TEXT( "Root Decorator: %s" ), *decorator->GetNodeName() );
		}
	    else
		{
		    UE_VLOG( owner, LogBehaviorTree, Display, TEXT( "Root Decorator: %s" ), *decorator->GetNodeName() );
		}
	}

    FBTCompositeChild rootNode;
    rootNode.ChildComposite = behaviorTree->RootNode;

    TArray<FBTCompositeChild> stack;
    stack.Push( rootNode );
    FBTCompositeChild child;

    while( stack.Num() > 0 )
	{
	    child = stack.Last();

	    for( int d = 0; d < child.Decorators.Num(); ++d )
		{
		    UBTDecorator* decorator = child.Decorators[d];
		    if( nullptr == owner )
			{
			    UE_LOG( LogEvolvingBehavior, Display, TEXT( "%sDecorator: %s" ),
			        *FString(std::string( (int)(decorator->GetTreeDepth()+1) * 2, '-' ).c_str()),
			        *decorator->GetNodeName() );
			}
		    else
			{
			    UE_VLOG( owner, LogBehaviorTree, Display, TEXT( "%sDecorator: %s" ),
			        *FString(std::string( (int)(decorator->GetTreeDepth()+1) * 2, '-' ).c_str()),
			        *decorator->GetNodeName() );
			}
		}
	    stack.Pop();
	    if( child.ChildComposite )
		{
		    if( nullptr == owner )
			{
			    UE_LOG( LogEvolvingBehavior, Display, TEXT( "%sComposite: %s" ),
			        *FString(std::string( (int)child.ChildComposite->GetTreeDepth() * 2, '-' ).c_str()),
			        *child.ChildComposite->GetNodeName() );
			}
		    else
			{
			    UE_VLOG( owner, LogBehaviorTree, Display, TEXT( "%sComposite: %s" ),
			        *FString(std::string( (int)child.ChildComposite->GetTreeDepth() * 2, '-' ).c_str()),
			        *child.ChildComposite->GetNodeName() );
			}

		    TArray<FBTCompositeChild>& children = child.ChildComposite->Children;
		    for( int i = children.Num() - 1; i >= 0; --i )
			{
			    stack.Push( children[i] );
			}
            TArray<UBTService*>& services = child.ChildComposite->Services;
            for( int i = services.Num() - 1; i >= 0; --i )
            {
                UBTService* service = services[i];
                if( nullptr == owner )
                {
                    UE_LOG( LogEvolvingBehavior, Display, TEXT( "%Service: %s" ),
                           *FString(std::string( (int)(service->GetTreeDepth()+1) * 2, '-' ).c_str()),
                           *service->GetNodeName() );
                }
                else
                {
                    UE_VLOG( owner, LogBehaviorTree, Display, TEXT( "%Service: %s" ),
                            *FString(std::string( (int)(service->GetTreeDepth()+1) * 2, '-' ).c_str()),
                            *service->GetNodeName() );
                }
            }
		}
	    else if( child.ChildTask )
		{
		    if( nullptr == owner )
			{
			    UE_LOG( LogEvolvingBehavior, Display, TEXT( "%sTask: %s" ),
			        *FString(std::string( (int)child.ChildTask->GetTreeDepth() * 2, '-' ).c_str()),
			        *child.ChildTask->GetNodeName() );
			}
		    else
			{
			    UE_VLOG( owner, LogBehaviorTree, Display, TEXT( "%sTask: %s" ),
			        *FString(std::string( (int)child.ChildTask->GetTreeDepth() * 2, '-' ).c_str()),
			        *child.ChildTask->GetNodeName() );
			}
            TArray<UBTService*>& services = child.ChildComposite->Services;
            for( int i = services.Num() - 1; i >= 0; --i )
            {
                UBTService* service = services[i];
                if( nullptr == owner )
                {
                    UE_LOG( LogEvolvingBehavior, Display, TEXT( "%Service: %s" ),
                           *FString(std::string( (int)(service->GetTreeDepth()+1) * 2, '-' ).c_str()),
                           *service->GetNodeName() );
                }
                else
                {
                    UE_VLOG( owner, LogBehaviorTree, Display, TEXT( "%Service: %s" ),
                            *FString(std::string( (int)(service->GetTreeDepth()+1) * 2, '-' ).c_str()),
                            *service->GetNodeName() );
                }
            }
		}
	}
}

void UBehaviorTreeUtils::MapBehaviorTree( UNodeMapper* mapper, UBehaviorTree* behaviorTree )
{
    for( int d = 0; d < behaviorTree->RootDecorators.Num(); ++d )
	{
	    UBTDecorator* decorator = behaviorTree->RootDecorators[d];
	    mapper->Register( decorator );
	}

    FBTCompositeChild rootNode;
    rootNode.ChildComposite = behaviorTree->RootNode;

    TArray<FBTCompositeChild> stack;
    stack.Push( rootNode );
    FBTCompositeChild child;

    while( stack.Num() > 0 )
	{
	    child = stack.Last();
	    stack.Pop();

	    for( int d = 0; d < child.Decorators.Num(); ++d )
		{
		    UBTDecorator* decorator = child.Decorators[d];
		    mapper->Register( decorator );
		}

	    if( child.ChildComposite )
		{
		    mapper->Register( child.ChildComposite );
		    TArray<FBTCompositeChild>& children = child.ChildComposite->Children;
		    for( int i = 0; i < children.Num(); ++i )
			{
			    stack.Push( children[i] );
			}
            TArray<UBTService*>& services = child.ChildComposite->Services;
            for( int i = services.Num() - 1; i >= 0; --i )
            {
                mapper->Register( services[i] );
            }
		}
	    else if( child.ChildTask )
		{
		    mapper->Register( child.ChildTask );
            TArray<UBTService*>& services = child.ChildTask->Services;
            for( int i = services.Num() - 1; i >= 0; --i )
            {
                mapper->Register( services[i] );
            }
		}
	}
}

FString UBehaviorTreeUtils::CompositeToString( int indentLevel, UBTCompositeNodeGene* node )
{
    FString result = ( TEXT( "%s\n" ), std::string( indentLevel, ' ' ).c_str() );
    result += node->ToString();
    ++indentLevel;

    for( int i = 0; i < node->GetNumChildren(); ++i )
	{
	    UBTChildContainerGene* child = node->GetChild( i );

	    result += DecoratorsToString( indentLevel, child );

	    if( child->IsALeaf() )
		{
            result += ServicesToString( indentLevel, child->GetTaskChild() );
		    result += ( TEXT( "%s\n" ), std::string( indentLevel, ' ' ).c_str() );
		    result += child->GetTaskChild()->ToString();
		}
	    else
		{
            result += ServicesToString( indentLevel, child->GetCompositeChild() );
		    result += UBehaviorTreeUtils::CompositeToString( indentLevel, child->GetCompositeChild() );
		}
	}

    return result;
}

void UBehaviorTreeUtils::CompositeToLog(int indentLevel, UBTCompositeNodeGene* node)
{
	UE_LOG(LogEvolvingBehavior, Display, TEXT("%s%s"), *FString(std::string(indentLevel, ' ').c_str()), *node->ToString());
	++indentLevel;

	for (int i = 0; i < node->GetNumChildren(); ++i)
	{
		UBTChildContainerGene* child = node->GetChild(i);

		DecoratorsToLog(indentLevel, child);

		if (child->IsALeaf())
		{
            ServicesToLog(indentLevel, child->GetTaskChild());
			UE_LOG(LogEvolvingBehavior, Display, TEXT("%s%s"), *FString(std::string(indentLevel, ' ').c_str()), *child->GetTaskChild()->ToString());
		}
		else
		{
            ServicesToLog(indentLevel, child->GetCompositeChild());
			UBehaviorTreeUtils::CompositeToLog(indentLevel, child->GetCompositeChild());
		}
	}
}

FString UBehaviorTreeUtils::DecoratorsToString( int indentLevel, const TArray<UBTDecoratorGene*>& decorators )
{
    FString result;
    for( int i = 0; i < decorators.Num(); ++i )
	{
	    result += ( TEXT( "%s\n" ), std::string( indentLevel, ' ' ).c_str() );
	    result += decorators[i]->ToString();
	}
    return result;
}

void UBehaviorTreeUtils::DecoratorsToLog(int indentLevel, const TArray<UBTDecoratorGene*>& decorators)
{
	for (int i = 0; i < decorators.Num(); ++i)
	{
		UE_LOG(LogEvolvingBehavior, Display, TEXT("%s%s"), *FString(std::string(indentLevel, ' ').c_str()), *decorators[i]->ToString())
	}
}

FString UBehaviorTreeUtils::DecoratorsToString( int indentLevel, const UBTChildContainerGene* compositeChild )
{
    FString result;
    for( int i = 0; i < compositeChild->GetNumDecorators(); ++i )
	{
	    result += ( TEXT( "%s\n" ), std::string( indentLevel, ' ' ).c_str() );
	    result += compositeChild->GetDecorator( i )->ToString();
	}
    return result;
}

void UBehaviorTreeUtils::DecoratorsToLog(int indentLevel, const UBTChildContainerGene* compositeChild)
{
	for (int i = 0; i < compositeChild->GetNumDecorators(); ++i)
	{
		UE_LOG(LogEvolvingBehavior, Display, TEXT("%s%s"), *FString(std::string(indentLevel, ' ').c_str()), *compositeChild->GetDecorator(i)->ToString())
	}
}

FString UBehaviorTreeUtils::ServicesToString( int indentLevel, const TArray<UBTServiceGene*>& services )
{
    FString result;
    for( int i = 0; i < services.Num(); ++i )
    {
        result += ( TEXT( "%s\n" ), std::string( indentLevel, ' ' ).c_str() );
        result += services[i]->ToString();
    }
    return result;
}

void UBehaviorTreeUtils::ServicesToLog(int indentLevel, const TArray<UBTServiceGene*>& services)
{
    for (int i = 0; i < services.Num(); ++i)
    {
        UE_LOG(LogEvolvingBehavior, Display, TEXT("%s%s"), *FString(std::string(indentLevel, ' ').c_str()), *services[i]->ToString())
    }
}

FString UBehaviorTreeUtils::ServicesToString( int indentLevel, const UBTCompositeNodeGene* node )
{
    FString result;
    for( int i = 0; i < node->GetNumServices(); ++i )
    {
        result += ( TEXT( "%s\n" ), std::string( indentLevel, ' ' ).c_str() );
        result += node->GetService( i )->ToString();
    }
    return result;
}

void UBehaviorTreeUtils::ServicesToLog(int indentLevel, const UBTCompositeNodeGene* node)
{
    for (int i = 0; i < node->GetNumServices(); ++i)
    {
        UE_LOG(LogEvolvingBehavior, Display, TEXT("%s%s"), *FString(std::string(indentLevel, ' ').c_str()), *node->GetService(i)->ToString())
    }
}

FString UBehaviorTreeUtils::ServicesToString( int indentLevel, const UBTTaskNodeGene* node )
{
    FString result;
    for( int i = 0; i < node->GetNumServices(); ++i )
    {
        result += ( TEXT( "%s\n" ), std::string( indentLevel, ' ' ).c_str() );
        result += node->GetService( i )->ToString();
    }
    return result;
}

void UBehaviorTreeUtils::ServicesToLog(int indentLevel, const UBTTaskNodeGene* node)
{
    for (int i = 0; i < node->GetNumServices(); ++i)
    {
        UE_LOG(LogEvolvingBehavior, Display, TEXT("%s%s"), *FString(std::string(indentLevel, ' ').c_str()), *node->GetService(i)->ToString())
    }
}

bool UBehaviorTreeUtils::AssertEquals( UBehaviorTree* left, UBehaviorTree* right )
{
    if( !AssertEquals( left->RootNode, right->RootNode ) )
	{
	    UE_LOG( LogEvolvingBehaviorError, Error, TEXT( "Root nodes do not match!" ) );
	    return false;
	}

    if( !AssertEquals( left->RootDecorators, right->RootDecorators ) )
	{
	    UE_LOG( LogEvolvingBehaviorError, Error, TEXT( "Root decorators do not match!" ) );
	    return false;
	}

    return true;
}

bool UBehaviorTreeUtils::AssertBaseDataEquals( UBTNode* left, UBTNode* right )
{
    if( left->GetNodeName() != right->GetNodeName() )
	{
	    UE_LOG( LogEvolvingBehaviorError, Error, TEXT( "Node names do not match: %s vs %s" ), *left->GetNodeName(),
	        *right->GetNodeName() );
	    return false;
	}

    if( left->GetExecutionIndex() != right->GetExecutionIndex() )
	{
	    UE_LOG( LogEvolvingBehaviorError, Error, TEXT( "Execution index does not match: %s: %i vs. %i" ),
	        *left->GetNodeName(), left->GetExecutionIndex(), right->GetExecutionIndex() );
	    return false;
	}

    if( left->GetMemoryOffset() != right->GetMemoryOffset() )
	{
	    UE_LOG( LogEvolvingBehaviorError, Error, TEXT( "Memory offset does not match: %s: %i vs. %i" ),
	        *left->GetNodeName(), left->GetMemoryOffset(), right->GetMemoryOffset() );
	    return false;
	}

    if( left->GetTreeDepth() != right->GetTreeDepth() )
	{
	    UE_LOG( LogEvolvingBehaviorError, Error, TEXT( "Tree depth does not match: %s: %i vs. %i" ),
	        *left->GetNodeName(), left->GetTreeDepth(), right->GetTreeDepth() );
	    return false;
	}

    return true;
}

bool UBehaviorTreeUtils::AssertEquals( UBTCompositeNode* left, UBTCompositeNode* right )
{
    if( !AssertBaseDataEquals( left, right ) )
	{
	    UE_LOG( LogEvolvingBehaviorError, Error, TEXT( "Base data not equal: %s and %s" ), *left->GetNodeName(),
	        *right->GetNodeName() );
	    return false;
	}

    if( left->Children.Num() != right->Children.Num() )
	{
	    UE_LOG( LogEvolvingBehaviorError, Error, TEXT( "Number of children does not match: %s: %i vs. %i" ),
	        *left->GetNodeName(), left->Children.Num(), right->Children.Num() );
	    return false;
	}
    
    if( !AssertEquals( left->Services, right->Services ) )
    {
        UE_LOG( LogEvolvingBehaviorError, Error, TEXT( "Services are not equal!" ));
        return false;
    }
    
    int numChildren = left->Children.Num();
    for( int i = 0; i < numChildren; ++i )
	{
	    FBTCompositeChild& leftChild = left->Children[i];
	    FBTCompositeChild& rightChild = right->Children[i];
	    if( !AssertEquals( leftChild, rightChild ) )
		{
		    UE_LOG( LogEvolvingBehaviorError, Error, TEXT( "Left child does not equal right child: %s: %i" ),
		        *left->GetNodeName(), i );
		    return false;
		}
	}

    return true;
}

bool UBehaviorTreeUtils::AssertEquals( FBTCompositeChild& left, FBTCompositeChild& right )
{
    if( !AssertEquals( left.Decorators, right.Decorators ) )
	{
	    UE_LOG( LogEvolvingBehaviorError, Error, TEXT( "Decorators are not equal!" ) );
	    return false;
	}

    if( left.ChildTask != NULL )
	{
	    if( right.ChildTask == NULL )
		{
		    UE_LOG( LogEvolvingBehaviorError, Error,
		        TEXT( "Left child is Task, right is not!: Left child: %s: " ), *left.ChildTask->GetNodeName() );
		    return false;
		}

	    return AssertBaseDataEquals( left.ChildTask, right.ChildTask );
	}
    else if( left.ChildComposite != NULL )
	{
	    if( right.ChildComposite == NULL )
		{
		    UE_LOG( LogEvolvingBehaviorError, Error,
		        TEXT( "Left child is Composite, right is not!: Left child: %s: " ),
		        *left.ChildComposite->GetNodeName() );
		    return false;
		}

	    return AssertEquals( left.ChildComposite, right.ChildComposite );
	}

    return true;
}

bool UBehaviorTreeUtils::AssertEquals( TArray<UBTDecorator*>& left, TArray<UBTDecorator*>& right )
{
    if( left.Num() != right.Num() )
	{
	    UE_LOG( LogEvolvingBehaviorError, Error, TEXT( "Number of decorators does not match: %i vs. %i" ),
	        left.Num(), right.Num() );
	    return false;
	}

    for( int i = 0; i < left.Num(); ++i )
	{
	    if( !AssertBaseDataEquals( left[i], right[i] ) )
		{
		    UE_LOG( LogEvolvingBehaviorError, Error, TEXT( "Decorator at %i does not match!" ), i );
		    return false;
		}
	}

    return true;
}

bool UBehaviorTreeUtils::AssertEquals( TArray<UBTService*>& left, TArray<UBTService*>& right )
{
    if( left.Num() != right.Num() )
    {
        UE_LOG( LogEvolvingBehaviorError, Error, TEXT( "Number of services does not match: %i vs. %i" ),
               left.Num(), right.Num() );
        return false;
    }
    
    for( int i = 0; i < left.Num(); ++i )
    {
        if( !AssertBaseDataEquals( left[i], right[i] ) )
        {
            UE_LOG( LogEvolvingBehaviorError, Error, TEXT( "Service at %i does not match!" ), i );
            return false;
        }
    }
    
    return true;
}

