#include "Experiment/SequentialParameterModifier.h"

void USequentialParameterModifier::Restart()
{
	CurrNumApplied = 0;
}

FString USequentialParameterModifier::GetShortString()
{
	return FString::Printf(TEXT("%s-%d"), *GetParameterName(), CurrNumApplied);
}

bool USequentialParameterModifier::Increment()
{
	if (CurrNumApplied >= ApplicationValues.Num()-1)
	{
		return false;
	}

	CurrNumApplied++;
	return true;
}

float USequentialParameterModifier::GetCurrentValue()
{
	return ApplicationValues[CurrNumApplied];
}