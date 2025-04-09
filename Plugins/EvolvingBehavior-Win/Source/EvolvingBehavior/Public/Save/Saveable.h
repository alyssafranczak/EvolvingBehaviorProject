#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Saveable.generated.h"

// Based on code by Rune de Groot's article on saving actors 
// http://runedegroot.com/saving-and-loading-actor-data-in-unreal-engine-4/
UINTERFACE(MinimalAPI)
class USaveable : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class EVOLVINGBEHAVIOR_API ISaveable
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Saveable Actor")
		void DataLoaded();
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Saveable Actor")
		void DataSaved();
};
