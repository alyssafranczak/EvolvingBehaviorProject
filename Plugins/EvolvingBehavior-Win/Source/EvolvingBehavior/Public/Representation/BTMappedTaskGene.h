#pragma once

#include "ServiceInjector.h"
#include "NodeMapper.h"
#include "NodeMapID.h"
#include "BTTaskNodeGene.h"

#include "BTMappedTaskGene.generated.h"

class UBTTaskNode;

/**
 * @file BTMappedTaskGene.h
 */

/**
 * @class UBTMappedTaskGene
 * @author npc
 * @date 12/01/17
 * @brief A Task gene that refers to a template node in the NodeMapper. See BTTaskNodeGene.h to learn about their function.
 */
UCLASS()
class EVOLVINGBEHAVIOR_API UBTMappedTaskGene : public UBTTaskNodeGene
{
    GENERATED_BODY()

private:

    UPROPERTY( SaveGame )
    FNodeMapID mapID;

public:
    void Init( UServiceInjector* injector, FNodeMapID id );

    virtual UBTTaskNode* BuildPhenotype( UObject* outer ) override;

    virtual FString ToString() override { return mapID.ToString(); }

    virtual UBTTaskNodeGene* DuplicateTask( UObject* other ) override;
};
