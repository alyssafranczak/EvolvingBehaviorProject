#include "BTGeneratedTaskGene.h"

#include "EvolvingBehavior.h"

#include "BoolGeneratedProperty.h"
#include "FloatGeneratedProperty.h"

#include "BehaviorTree/BTTaskNode.h"

#include "UObject/UObjectBase.h"

#include "PropertyDict.h"
#include "ServiceInjector.h"

UBTGeneratedTaskGene::UBTGeneratedTaskGene() : UBTTaskNodeGene()
{

}

void UBTGeneratedTaskGene::RandomizeAllProperties()
{
    if (nullptr == rand)
    {
        rand = ServiceInjector->GetService<URandomGen>();
    }

    for( TPair<FName, UGeneratedProperty*> pair : propertyDict->GetProperties() )
    {
        pair.Value->InitMutate(rand);
    }
}

void UBTGeneratedTaskGene::MutateRandomProperty(float stdDevFractionOfRange)
{
    if (nullptr == rand)
    {
        rand = ServiceInjector->GetService<URandomGen>();
    }
    
    if (nullptr == propertyDict)
    {
        UE_LOG(LogEvolvingBehaviorError, Error, TEXT("No property dictionary in gene!"));
        return;
    }

    TMap<FName, UGeneratedProperty*>& properties = propertyDict->GetProperties();

    if (properties.Num() == 0 )
    {
        // Empty dict - nothing to mutate.
        return;
    }

    TPair<FName, UGeneratedProperty*>& propertyToChange = rand->RandomMapEntry(properties);
    propertyToChange.Value->Mutate(rand, stdDevFractionOfRange);
}

UBTTaskNodeGene* UBTGeneratedTaskGene::DuplicateTask( UObject* outer )
{
    UBTGeneratedTaskGene* other = DuplicateObject<UBTGeneratedTaskGene>(this, outer);
    
    return other;
}

UBTTaskNode* UBTGeneratedTaskGene::BuildPhenotype( UObject* outer )
{
    check(classType->IsChildOf<UBTTaskNode>());
    
    UBTTaskNode* result = NewObject<UBTTaskNode>(outer, classType);
    result->NodeName = name;
    
    for( TPair<FName, UGeneratedProperty*> pair : propertyDict->GetProperties())
    {
        if ( !pair.Value->SetOn(result) )
        {
            UE_LOG(LogEvolvingBehaviorError, Error, TEXT("Unable to set property on Task node:  %s (%s)"), *name, *classType->GetName());
        }
    }
    
    return result;
}

FString UBTGeneratedTaskGene::ToString()
{
    FString result = FString::Printf(TEXT("%s (%s)\n"), *name, *classType->GetName());

    for(TPair<FName, UGeneratedProperty*> pair : propertyDict->GetProperties())
    {
        result += FString::Printf(TEXT("    %s"), *pair.Value->ToString());
    }
    
    return result;
}

void UBTGeneratedTaskGene::DataLoaded_Implementation()
{
    classType = FindObject<UClass>(ANY_PACKAGE, *className);
    if ( nullptr == classType)
    {
        classType = StaticLoadClass(UBTTaskNode::StaticClass(), nullptr, *className, nullptr, LOAD_None, nullptr);
    }

    if ( nullptr == classType)
    {
        UE_LOG(LogEvolvingBehaviorError, Error, TEXT("Class could not be loaded for BTGeneratedTaskGene: %s"), *className);
        classType = UBTTaskNode::StaticClass();
    }
}

void UBTGeneratedTaskGene::DataSaved_Implementation()
{
    className = classType->GetPathName();
}