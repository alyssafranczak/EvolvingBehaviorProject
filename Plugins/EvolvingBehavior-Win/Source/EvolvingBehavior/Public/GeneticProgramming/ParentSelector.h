
#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"

#include "ParentSelector.generated.h"

class URandomGen;

USTRUCT()
struct FParentFitnessInfo
{
    GENERATED_BODY()
    UPROPERTY( EditAnywhere, Category = "EvolvingBehavior")
    int32 id;

    UPROPERTY( EditAnywhere, Category = "EvolvingBehavior")
    double fitness;
};

/**
 * @file ParentSelector.h
 */

/**
 * @class UParentSelector
 * @author isrivastava
 * @date 12/12/17
 * @brief An abstract base class for selecting parents.
 * 
 * There is currently one subclass, which is the TournamentSelector.
 */
UCLASS( abstract, Blueprintable, EditInlineNew, BlueprintType, DefaultToInstanced, CollapseCategories )
class EVOLVINGBEHAVIOR_API UParentSelector : public UObject
{

    GENERATED_BODY()
public:
    virtual TArray<FParentFitnessInfo> SelectParents( TArray<FParentFitnessInfo> parentInfo, URandomGen* randomGen )
        PURE_VIRTUAL( UParentSelector::SelectParents(), TArray<FParentFitnessInfo> temp; return temp; );
};
