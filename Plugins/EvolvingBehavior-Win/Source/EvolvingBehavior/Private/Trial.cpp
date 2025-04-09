#include "Trial.h"

#include "EvolvingBehavior.h"
#include "BTChromosome.h"

UBTChromosome* UTrial::Get(int32 id)
{
	if ( !population.Contains( id ) )
	{
		return nullptr;
	}
	return population[id];
}

void UTrial::Add(int32 id, UBTChromosome* bt)
{
	population.Add(id, bt);
}

TArray<UBTChromosome*> UTrial::GetChromosomes()
{
	TArray<UBTChromosome*> chromosomes;
	for (auto& Elem : population)
	{
		chromosomes.Add(Elem.Value);
	}
	return chromosomes;
}

void UTrial::SetServiceInjector(UServiceInjector* serviceInjector)
{
	for ( auto& member : population )
	{
		member.Value->SetServiceInjector(serviceInjector);
	}
}