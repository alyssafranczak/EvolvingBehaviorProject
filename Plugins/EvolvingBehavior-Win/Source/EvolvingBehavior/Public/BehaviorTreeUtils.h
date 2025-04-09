#pragma once

#include "BehaviorTree/BTNode.h"
#include "BehaviorTree/BTCompositeNode.h"
#include "BehaviorTreeUtils.generated.h"

class UBehaviorTree;
class UBTCompositeNodeGene;
class UBTTaskNodeGene;
class UBTDecoratorGene;
class UBTServiceGene;
class UBTChildContainerGene;
class UBTDecorator;
class UBTService;
class UNodeMapper;

/**
 * @file BehaviorTreeUtils.h
 */

/**
 * @class UBehaviorTreeUtils
 * @author npc
 * @date 12/01/17
 * @brief A class containing utility functions related to Unreal Engine Behavior Trees.
 */
UCLASS()
class EVOLVINGBEHAVIOR_API UBehaviorTreeUtils : public UObject
{
	GENERATED_BODY()
	
protected:
	
	static FString DecoratorsToString( int indentLevel, const UBTChildContainerGene* compositeChild );
	
	static void DecoratorsToLog(int indentLevel, const UBTChildContainerGene* compositeChild);

	static bool AssertBaseDataEquals( UBTNode* left, UBTNode* right );
	
	static bool AssertEquals( UBTCompositeNode* left, UBTCompositeNode* right );
	
	static bool AssertEquals( FBTCompositeChild& left, FBTCompositeChild& right );
	
	static bool AssertEquals( TArray<UBTDecorator*>& left, TArray<UBTDecorator*>& right );
    
    static bool AssertEquals( TArray<UBTService*>& left, TArray<UBTService*>& right );
	
public:
	
	/**
	 * @brief Print a behavior tree to the log
	 * @param behaviorTree The Behavior Tree to print.
	 * @param owner The Actor that owns this tree - may be null.
	 * 
	 * If an Actor is provided as the owner, will use the Visual log.
	 */
	static void PrintBehaviorTree(UBehaviorTree* behaviorTree, AActor* owner = NULL);
	
	/**
	 * @brief Map all nodes in a given behavior tree using a UNodeMapper.
	 * @param mapper The mapper to use.
	 * @param behaviorTree The Behavior Tree to map.
	 */
	static void MapBehaviorTree( UNodeMapper* mapper, UBehaviorTree* behaviorTree);
	
	/**
	 * @brief Converts the given CompositeNodeGene to a string.
	 * @param indentLevel The indent level at which to indent each line.
	 * @param node The CompositeNodeGene to convert.
	 * @return The string.
	 */
	static FString CompositeToString( int indentLevel, UBTCompositeNodeGene* node );

	static void CompositeToLog(int indentLevel, UBTCompositeNodeGene * node);
    
    /**
     * @brief Converts the given CompositeNodeGene's Services to a string.
     * @param indentLevel The indent level at which to indent each line.
     * @param node The CompositeNodeGene who's services to convert.
     * @return The string.
     */
    static FString ServicesToString( int indentLevel, const UBTCompositeNodeGene* node );
    
    static void ServicesToLog(int indentLevel, const UBTCompositeNodeGene* node);
    
    /**
     * @brief Converts the given TaskNodeGene's Services to a string.
     * @param indentLevel The indent level at which to indent each line.
     * @param node The TaskNodeGene who's services to convert.
     * @return The string.
     */
    static FString ServicesToString( int indentLevel, const UBTTaskNodeGene* node );
    
    static void ServicesToLog(int indentLevel, const UBTTaskNodeGene* node);
	
	/**
	 * @brief Converts an array of decorators into a string.
	 * @param indentLevel The indent level at which to indent each line.
	 * @param decorators The array of decorators.
	 * @return The resulting string.
	 */
	static FString DecoratorsToString( int indentLevel, const TArray<UBTDecoratorGene*>& decorators );
	
	static void DecoratorsToLog(int indentLevel, const TArray<UBTDecoratorGene*>& decorators);

    
    static FString ServicesToString( int indentLevel, const TArray<UBTServiceGene*>& services );
    
    static void ServicesToLog(int indentLevel, const TArray<UBTServiceGene*>& services);
    
	/**
	 * @brief Checks that the given behavior trees are exactly alike.
	 * @param left The left behavior tree.
	 * @param right The right behavior tree.
	 * @return True if the trees are the same, false otherwise.
	 */
	static bool AssertEquals( UBehaviorTree* left, UBehaviorTree* right );
};
