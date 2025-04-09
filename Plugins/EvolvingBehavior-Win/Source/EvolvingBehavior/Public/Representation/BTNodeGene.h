
#pragma once

#include "CoreMinimal.h"

#include "BTNodeGene.generated.h"

class UServiceInjector;

/**
 * @file BTNodeGene.h
 */

/**
 * @class UBTNodeGene
 * @author npc
 * @date 12/01/17
 * @brief The abstract base class for all genes that represent behavior tree nodes.
 */
UCLASS( abstract )
class EVOLVINGBEHAVIOR_API UBTNodeGene : public UObject
{
    GENERATED_BODY()

protected:
    UPROPERTY()
    UServiceInjector* ServiceInjector;

public:
    virtual FString ToString() PURE_VIRTUAL( UBTNodeGene::ToString, return ""; );

    virtual UBTNodeGene* Duplicate( UObject* outer ) PURE_VIRTUAL( UBTNodeGene::Duplicate, return nullptr; );

    virtual void SetServiceInjector(UServiceInjector* serviceInjector )
    {
        ServiceInjector = serviceInjector;
    }
};
