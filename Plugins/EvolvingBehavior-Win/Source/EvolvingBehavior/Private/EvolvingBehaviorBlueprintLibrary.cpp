#include "EvolvingBehaviorBlueprintLibrary.h"

#include "EvolutionControlActor.h"
#include "PopulationManager.h"
#include "BTChromosomeUtils.h"

int32 UEvolvingBehaviorBlueprintLibrary::GetSizeOfGenome(AEvolutionControlActor* evolutionControlActor, int32 populationMemberID)
{
    if (nullptr == evolutionControlActor)
    {
        UE_LOG(LogEvolvingBehaviorError, Error, TEXT("EvolutionControlActor is null when trying to get size of genome."));
        return 0;
    }

    UPopulationManager* populationManager = evolutionControlActor->GetPopulationManager();
    if (nullptr == populationManager)
    {
        UE_LOG(LogEvolvingBehaviorError, Error, TEXT("No population manager found on EvolutionControlActor?"));
        return 0;
    }

    UBTChromosome* chromosome = populationManager->GetCurrTrial()->Get(populationMemberID);
    if (nullptr == chromosome)
    {
        UE_LOG(LogEvolvingBehavior, Warning, TEXT("No member found with ID %i in population when trying to find chromosome size"), populationMemberID);
        return 0;
    }

    int32 numNodes = BTChromosomeUtils::CountNumNodesOfType( chromosome, UBTNodeGene::StaticClass() );

    return numNodes;
}