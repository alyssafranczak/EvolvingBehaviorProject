#pragma once

#include "BehaviorTree/BTNode.h"
#include "RandomGen.h"
#include "NodeMapID.h"
#include "Service.h"

#include "NodeMapper.generated.h"

class UBTService;
class UBTServiceGene;

/**
 * @file NodeMapper.h
 */

/**
 * @class UNodeMapper
 * @author npc
 * @date 12/01/17
 * @brief Keeps a mapping of IDs to template BehaviorTree nodes.
 *
 * The NodeMapper stores a map of FNodeMapID to UBTNode. Each Node should be thought of as the template
 * for creating new nodes with the same variables and type.
 *
 * When you map a node, its ID must be unique. Currently, IDs are the node name. Therefore, you should always
 * name your nodes uniquely.
 *
 * When you retrieve nodes from the NodeMapper, it returns the template directly. You should then copy the
 * template, not modify it.
 */
UCLASS()
class EVOLVINGBEHAVIOR_API UNodeMapper : public UService
{
    GENERATED_BODY()

private:
    UPROPERTY()
    TMap<FNodeMapID, UBTNode*> nodes;
	
	UPROPERTY()
	TArray<FNodeMapID> composites;
	
	UPROPERTY()
	TArray<FNodeMapID> tasks;
    
    UPROPERTY()
    TArray<FNodeMapID> services;
	
	UPROPERTY()
	TArray<FNodeMapID> decorators;

    FNodeMapID GetMapID( const UBTNode* node ) const;

protected:
    void Reset( UBTNode* node );

    /**
     * @brief Add the given node as a Template, creating a new mapping.
     * @param node The node to add.
     *
     * The node ID must be unique. At the moment, the ID is just the Node Name.
     */
    void AddMapping( UBTNode* node, FNodeMapID nodeID );
public:
    void Register( const UBTTaskNode* node );
	
	void Register( const UBTCompositeNode* node );
    
    void Register( const UBTService* node );
	
	void Register( const UBTDecorator* node );

    /**
     * @brief Tries to retrieve a mapped ID for the give node.
     * @param node The template node to retrieve.
     * @param result The mapped ID, if found.
     * @return True if the mapping was found, false otherwise.
     */
    bool TryGetMapping( const UBTNode* node, FNodeMapID& result ) const;

    /**
     * @brief Tries to retrieve the template node for a given Map ID.
     * @param mapping The Mapped ID to retrieve.
     * @param result The resulting template node, if found.
     * @return True if the node was found, false otherwise
     *
     * You should not modify the resulting template node - you should copy it.
     */
    bool TryGetTemplate( FNodeMapID mapping, UBTNode*& result ) const;

    /**
     * @brief Retrieve a mapped ID for the given node, adding it if it did not exist.
     * @param node The node to map.
     * @return The ID for this node (new or unchanged).
     */
    FNodeMapID GetOrAddMapping( const UBTCompositeNode* node );
	
    /**
     * @brief Retrieve a mapped ID for the given node, adding it if it did not exist.
     * @param node The node to map.
     * @return The ID for this node (new or unchanged).
     */
    FNodeMapID GetOrAddMapping( const UBTTaskNode* node );
    
    /**
     * @brief Retrieve a mapped ID for the given node, adding it if it did not exist.
     * @param node The node to map.
     * @return The ID for this node (new or unchanged).
     */
    FNodeMapID GetOrAddMapping( const UBTService* node );
	
    /**
     * @brief Retrieve a mapped ID for the given node, adding it if it did not exist.
     * @param node The node to map.
     * @return The ID for this node (new or unchanged).
     */
    FNodeMapID GetOrAddMapping( const UBTDecorator* node );
	
	FNodeMapID GetRandomCompositeID( URandomGen* randomGen );
	
	FNodeMapID GetRandomTaskID( URandomGen* randomGen );
    
    FNodeMapID GetRandomServiceID( URandomGen* randomGen );
	
	FNodeMapID GetRandomDecoratorID( URandomGen* randomGen );
};
