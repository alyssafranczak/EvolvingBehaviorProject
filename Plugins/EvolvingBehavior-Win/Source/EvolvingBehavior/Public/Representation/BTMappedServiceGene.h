
#pragma once

#include "BTServiceGene.h"
#include "ServiceInjector.h"
#include "CoreMinimal.h"
#include "NodeMapID.h"

#include "BTMappedServiceGene.generated.h"

class UNodeMapper;

/**
 * @file BTMappedServiceGene.h
 */

/**
 * @class UBTMappedServiceGene
 * @author jim
 * @date 9/21/19
 * @brief A Service gene that refers to a template node in the NodeMapper. See BTServiceGene.h to read about
 * their function.
 */
UCLASS()
class EVOLVINGBEHAVIOR_API UBTMappedServiceGene : public UBTServiceGene
{
    GENERATED_BODY()

private:

    UPROPERTY( SaveGame )
    FNodeMapID mapID;

public:
    void Init( UServiceInjector* injector, FNodeMapID id );

    virtual FString ToString() override { return mapID.ToString(); }

    virtual UBTService* BuildPhenotype( UObject* outer ) override;

    virtual UBTServiceGene* DuplicateService( UObject* outer ) override;
};
