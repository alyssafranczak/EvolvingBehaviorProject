#pragma once

#include "NodeMapper.h"

#include "NodeMapID.h"

#include "BTCompositeNodeGene.h"

#include "BTMappedCompositeGene.generated.h"

class UBTCompositeNode;

/**
 * @file BTMappedCompositeGene.h
 */

/**
 * @class UBTMappedCompositeGene
 * @author npc
 * @date 12/01/17
 * @brief A Composite gene that refers to a template node in the NodeMapper. See BTCompositeNodeGene.h to
 * read about their function.
 */
UCLASS()
class EVOLVINGBEHAVIOR_API UBTMappedCompositeGene : public UBTCompositeNodeGene
{
    GENERATED_BODY()

private:

    UPROPERTY( SaveGame )
    FNodeMapID mapID;

public:
    void Init( UServiceInjector* injector, FNodeMapID id );

    virtual UBTCompositeNode* BuildPhenotype( UObject* outer ) override;

    virtual FString ToString() override { return mapID.ToString(); }
	
	virtual UBTCompositeNodeGene* DuplicateComposite( UObject* outer ) override;
};
