#include "Experiment/GridSearchExperimentManagementActor.h"
#include "LifeCycleActor.h"

void AGridSearchExperimentManagementActor::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	ApplyModifiers();
	
	ALifeCycleActor* lifeCycle = ALifeCycleActor::GetLifeCycleActor(GetWorld());
	if (!lifeCycle)
	{
		return;
	}
	OriginalExperimentID = lifeCycle->GetExperimentSubID();
	lifeCycle->SetExperimentID(FString::Printf(TEXT("%s%s"), *OriginalExperimentID, *GetModifierString()));
}

void AGridSearchExperimentManagementActor::DataLoaded_Implementation()
{
	ApplyModifiers();
}

void AGridSearchExperimentManagementActor::ApplyModifiers()
{
	for (USequentialParameterModifier* modifier : Modifiers)
	{
		modifier->Apply();
	}
}

void AGridSearchExperimentManagementActor::DataSaved_Implementation()
{
}

bool AGridSearchExperimentManagementActor::IncrementModifiers()
{
	int32 modifierToIncrement = Modifiers.Num() - 1;
	while (!Modifiers[modifierToIncrement]->Increment())
	{
		Modifiers[modifierToIncrement]->Restart();
		modifierToIncrement--;
		if ( modifierToIncrement < 0 )
		{
			return false;
		}
	}

	ApplyModifiers();

	return true;
}

FString AGridSearchExperimentManagementActor::GetModifierString()
{
	FString modifierString = FString(TEXT(""));
	for (int i = 0; i < Modifiers.Num(); ++i)
	{
		modifierString = FString::Printf(TEXT("%s-%s"), *modifierString, *(Modifiers[i]->GetShortString()));
	}

	return modifierString;
}

void AGridSearchExperimentManagementActor::HandleExperimentComplete_Implementation()
{
	ALifeCycleActor* lifeCycle = ALifeCycleActor::GetLifeCycleActor(GetWorld());
	if (!lifeCycle)
	{
		return;
	}

	if (!IncrementModifiers())
	{
		// Let the life cycle end the experiment normally.
		return;
	}

	lifeCycle->SetExperimentID(FString::Printf(TEXT("%s%s"), *OriginalExperimentID, *GetModifierString()));

	lifeCycle->RestartExperiment();
}