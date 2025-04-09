#pragma once

#include "CoreMinimal.h"
#include "SavedObjectData.generated.h"

// Based on code by Rune de Groot on saving Actor data
// http://runedegroot.com/saving-and-loading-actor-data-in-unreal-engine-4/
USTRUCT()
struct EVOLVINGBEHAVIOR_API FSavedObjectData 
{
	GENERATED_BODY()

	static FSavedObjectData Empty;

	UPROPERTY()
	FString ObjectClass;
	
	UPROPERTY()
	FName ObjectName;
	
	UPROPERTY()
	TArray<uint8> ObjectData;

	friend FArchive& operator<<(FArchive& ar, FSavedObjectData& saveObjectData) 
	{
		ar << saveObjectData.ObjectClass;
		ar << saveObjectData.ObjectName;
		ar << saveObjectData.ObjectData;
		return ar;
	}

};