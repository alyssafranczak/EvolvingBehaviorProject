#include "Experiment/MultiRunNoOpModifier.h"

#if WITH_EDITOR
void UMultiRunNoOpModifier::PostEditChangeProperty(FPropertyChangedEvent& propertyChangedEvent)
{
	ApplicationValues.Empty();
	for (int i = 0; i < NumRuns; ++i)
	{
		ApplicationValues.Add(i);
	}
}
#endif