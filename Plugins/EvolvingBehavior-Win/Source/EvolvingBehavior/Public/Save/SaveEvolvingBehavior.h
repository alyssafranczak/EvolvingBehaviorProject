#pragma once

#include "GameFramework/SaveGame.h"
#include "SavedObjectData.h"
#include "SaveEvolvingBehavior.generated.h"

/**
 *
 */
UCLASS()
class EVOLVINGBEHAVIOR_API USaveEvolvingBehavior : public USaveGame
{
	GENERATED_BODY()

	public:

	UPROPERTY(VisibleAnywhere, Category = Basic)
	uint32 TrialNumber;

	UPROPERTY(VisibleAnywhere, Category = Basic)
	FString ExperimentID;

	UPROPERTY(VisibleAnywhere, Category = Basic)
	FString SaveSlotName;

	UPROPERTY(VisibleAnywhere, Category = Basic)
	uint32 UserIndex;

	UPROPERTY()
	TArray<FSavedObjectData> ObjectData;

	USaveEvolvingBehavior();

	void SaveObject(UObject* object);

	bool LoadObject(UObject* existingObject);
private:

	bool CheckForDuplicateObjectNames(FName name);

	FSavedObjectData& LoadObjectData(FName name);

};