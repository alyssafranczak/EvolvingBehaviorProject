#include "BlackboardKeyGeneratedProperty.h"

#include "EvolvingBehavior.h"
#include "BehaviorTree/BehaviorTreeTypes.h"
#include "Engine/AssetManager.h"
#include "RandomGen.h"
#include "BehaviorTree/BlackboardData.h"

void UBlackboardKeyGeneratedProperty::InitMutate(URandomGen* rand)
{
    if (!blackboard.IsValid()) 
    {
        UE_LOG(LogEvolvingBehavior, Warning, TEXT("No blackboard loaded to resolve blackboard key property: %s"), *ToString());
        blackboard.LoadSynchronous();
    }

    UBlackboardData* blackboardData = blackboard.Get();
    
    TArray<FName> options;
    for ( const FBlackboardEntry& keyEntry : blackboardData->GetKeys())
    {
        if (keyEntry.KeyType->GetClass()->IsChildOf(keyType))
        {
            options.Add(keyEntry.EntryName);
        }
    }

    int32 selectedIdx = rand->UniformIntInRange(0, options.Num()-1);

    keyName = options[selectedIdx];
}

void UBlackboardKeyGeneratedProperty::Mutate(URandomGen* rand, double stdDevFractionOfRange)
{
    InitMutate(rand);
}

bool UBlackboardKeyGeneratedProperty::SetOn(UObject* object)
{
    if (!blackboard.IsValid())
    {
        UE_LOG(LogEvolvingBehavior, Warning, TEXT("No blackboard loaded to resolve blackboard key property: %s - trying to synchronous load it."), *ToString());
        blackboard.LoadSynchronous();
    }

    UClass* classType = object->GetClass();
    FProperty* property = classType->FindPropertyByName(GetName());
    if (property == nullptr)
    {
        UE_LOG( LogEvolvingBehaviorError, Error, TEXT( "Property not available to set: %s" ), *ToString());
        return false;
    }
    
    FStructProperty* blackboardProperty = CastField<FStructProperty>(property);
    if ( nullptr == blackboardProperty )
    {
        UE_LOG( LogEvolvingBehaviorError, Error, TEXT( "Property not a struct property, could not set: %s" ), *ToString());
        return false;
    }

    FBlackboardKeySelector* selector = property->ContainerPtrToValuePtr<FBlackboardKeySelector>(object);

    if (!selector)
    {
        UE_LOG( LogEvolvingBehaviorError, Error, TEXT( "Property not a Blackboard Key, could not set: %s" ), *ToString());
        return false;
    }
    
    selector->SelectedKeyName = keyName;
    selector->SelectedKeyType = keyType;
    selector->ResolveSelectedKey(*(blackboard.Get()));
    return true;
}

void UBlackboardKeyGeneratedProperty::DataLoaded_Implementation()
{
    // First, load our key type from the classpath.
    keyType = StaticLoadClass(UBlackboardKeyType::StaticClass(), nullptr, *savedKeyTypePath);

    // Try to grab the blackboard if it's already loaded.
    blackboard = TSoftObjectPtr<UBlackboardData>(FSoftObjectPath(blackboardPath));
    if (blackboard.IsValid())
    {
        // We already have the blackboard loaded.
        return;
    }
    
    // Otherwise, try to async load the blackboard.
    UAssetManager* assetManager = UAssetManager::GetIfValid();
    if (!assetManager)
    {
        UE_LOG( LogEvolvingBehaviorError, Error, TEXT( "No asset manager available?" ));
        return;
    }

    TArray<FSoftObjectPath> paths;
    paths.Add(FSoftObjectPath(blackboardPath));

    assetManager->LoadAssetList(paths, 
                                FStreamableDelegate::CreateUObject(this, &UBlackboardKeyGeneratedProperty::OnBlackboardLoaded));
}

void UBlackboardKeyGeneratedProperty::OnBlackboardLoaded()
{
    // Just in case we didn't have the soft pointer to our blackboard before, get it now that it's loaded.
    blackboard = TSoftObjectPtr<UBlackboardData>(FSoftObjectPath(blackboardPath));
}

void UBlackboardKeyGeneratedProperty::DataSaved_Implementation()
{
    // Save the path to our blackboard.
    blackboardPath = blackboard.ToSoftObjectPath().GetAssetPathName();

    // Save the classpath to our key type class.
    savedKeyTypePath = keyType->GetPathName();
}