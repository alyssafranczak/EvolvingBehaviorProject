#include "RandomGen.h"

#include <sstream>
#include "Save/SaveEvolvingBehavior.h"
#include "EvolvingBehavior.h"
#include "LifeCycleActor.h"

#include <random>

bool URandomGen::WithProbability( double probability )
{
	if ( probability > 1.0 )
	{
		return true;
	}
	
	return UniformRealInRange( 0, 1 ) < probability;
}

int32_t URandomGen::UniformIntInRange( int32_t lower, int32_t upper )
{
	std::uniform_int_distribution<int32_t> distribution( lower, upper );

	return distribution( generator );
}

double URandomGen::UniformRealInRange( double lower, double upper )
{
	std::uniform_real_distribution<double> distribution( lower, upper );
	
	return distribution( generator );
}

double URandomGen::GaussianReal(double mean, double stdDev)
{
	std::normal_distribution<double> distribution(mean, stdDev);

	return distribution(generator);
}

void URandomGen::RegisterLifeCycle()
{
	ALifeCycleActor* lc = ALifeCycleActor::GetLifeCycleActor(GetWorld());
	lc->OnSave().AddUObject(this, &URandomGen::Save);
	lc->OnLoad().AddUObject(this, &URandomGen::Load);
}

void URandomGen::Save(USaveEvolvingBehavior* sg)
{
	sg->SaveObject(this);
}

void URandomGen::Load(USaveEvolvingBehavior* sg)
{
	sg->LoadObject(this);
}

void URandomGen::DataLoaded_Implementation()
{
	std::istringstream stream(TCHAR_TO_UTF8(*state));
	stream >> generator;
}

void URandomGen::DataSaved_Implementation()
{
	std::ostringstream stream;
	stream << generator;
	state = FString(stream.str().c_str());
}