#include "Experiment/CrossoverProbabilityParamMod.h"
#include "EvolutionControlActor.h"
#include "BTEvolutionManagerComponent.h"
#include "GeneticProgramming/MutatingReproducer.h"
#include "GeneticProgramming/CrossoverMutator.h"
#include "GeneticProgramming/DepthBiasedCrossoverMutator.h"

void UCrossoverProbabilityParamMod::Apply()
{
	TArray<AEvolutionControlActor*> evoControlActors;
	AEvolutionControlActor::GetEvolutionControlActorsByTag(GetWorld(), PopulationTag, evoControlActors);
	for (AEvolutionControlActor* evoControlActor : evoControlActors)
	{
		UBTEvolutionManagerComponent* evoManager = evoControlActor->GetEvolutionManager();

		UReproducer* reproducer = evoManager->GetReproducer();

		UMutatingReproducer* mutatingReproducer = Cast<UMutatingReproducer>(reproducer);

		if (!mutatingReproducer)
		{
			continue;
		}

		TArray<FMutatorInfo>& mutators = mutatingReproducer->GetMutators();

		for (FMutatorInfo& mutatorInfo : mutators)
		{
			if (!mutatorInfo.mutator->IsA(UCrossoverMutator::StaticClass()) &&
				!mutatorInfo.mutator->IsA(UDepthBiasedCrossoverMutator::StaticClass()))
			{
				// This is not a crossover mutator, ignore it.
				continue;
			}

			mutatorInfo.probability = GetCurrentValue();
		}
	}
}