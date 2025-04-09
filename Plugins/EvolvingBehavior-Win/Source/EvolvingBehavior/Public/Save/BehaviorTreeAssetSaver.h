#pragma once

#include "BehaviorTreeAssetSaver.generated.h"

class UBehaviorTree;

/**
 *
 */
UCLASS()
class EVOLVINGBEHAVIOR_API UBehaviorTreeAssetSaver : public UObject
{
	GENERATED_BODY()

public:

    static bool SaveTree(UBehaviorTree* tree,
                            const FString& treeName = TEXT("BehaviorTree"),
                            const FString& packagePath = TEXT("/Game/EvolvingBehavior"));

};
