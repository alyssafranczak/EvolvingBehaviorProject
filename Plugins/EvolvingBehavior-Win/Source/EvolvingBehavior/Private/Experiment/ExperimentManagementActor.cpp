#include "Experiment/ExperimentManagementActor.h"
#include "Save/SaveEvolvingBehavior.h"
#include "LifeCycleActor.h"

AExperimentManagementActor::AExperimentManagementActor() : Super()
{
}

void AExperimentManagementActor::PreInitializeComponents()
{
	Super::PreInitializeComponents();

	ALifeCycleActor* lifeCycle = ALifeCycleActor::GetLifeCycleActor(GetWorld());
	if (!lifeCycle)
	{
		UE_LOG(LogEvolvingBehavior, Warning, TEXT("No LifeCycle actor found in map, experiment manager cannot manage experiment. Add a LifeCycle actor."));
		return;
	}

	lifeCycle->OnSave().AddUObject(this, &AExperimentManagementActor::Save);
	lifeCycle->OnLoad().AddUObject(this, &AExperimentManagementActor::Load);
	lifeCycle->OnExperimentComplete().AddUObject(this, &AExperimentManagementActor::HandleExperimentComplete);
}

void AExperimentManagementActor::PostInitializeComponents()
{
	Super::PostInitializeComponents();
}

void AExperimentManagementActor::Save(USaveEvolvingBehavior * sg)
{
	sg->SaveObject(this);
}

void AExperimentManagementActor::Load(USaveEvolvingBehavior * sg)
{
	sg->LoadObject(this);
}

void AExperimentManagementActor::DataLoaded_Implementation()
{
}

void AExperimentManagementActor::DataSaved_Implementation()
{
}

void AExperimentManagementActor::HandleExperimentComplete_Implementation()
{
}