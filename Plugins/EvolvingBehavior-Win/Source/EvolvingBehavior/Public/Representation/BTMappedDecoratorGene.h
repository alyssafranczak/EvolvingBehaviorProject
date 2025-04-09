
#pragma once

#include "BTDecoratorGene.h"
#include "ServiceInjector.h"
#include "CoreMinimal.h"
#include "NodeMapID.h"

#include "BTMappedDecoratorGene.generated.h"

class UNodeMapper;

/**
 * @file BTMappedDecoratorGene.h
 */

/**
 * @class UBTMappedDecoratorGene
 * @author npc
 * @date 12/01/17
 * @brief A Decorator gene that refers to a template node in the NodeMapper. See BTDecoratorGene.h to read
 * about their function.
 */
UCLASS()
class EVOLVINGBEHAVIOR_API UBTMappedDecoratorGene : public UBTDecoratorGene
{
    GENERATED_BODY()

private:

    UPROPERTY( SaveGame )
    FNodeMapID mapID;

public:
    void Init( UServiceInjector* injector, FNodeMapID id );

    virtual FString ToString() override { return mapID.ToString(); }

    virtual UBTDecorator* BuildPhenotype( UObject* outer ) override;

    virtual UBTDecoratorGene* DuplicateDecorator( UObject* outer ) override;
};
