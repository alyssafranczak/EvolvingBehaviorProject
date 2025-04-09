#include "BTGeneratedServiceGene.h"

#include "EvolvingBehavior.h"

#include "BehaviorTree/BTService.h"

#include "PropertyDict.h"
#include "ServiceInjector.h"

UBTGeneratedServiceGene::UBTGeneratedServiceGene() : UBTServiceGene()
{

}

void UBTGeneratedServiceGene::RandomizeAllProperties()
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

void UBTGeneratedServiceGene::MutateRandomProperty(float stdDevFractionOfRange)
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

UBTService* UBTGeneratedServiceGene::BuildPhenotype( UObject* outer )
{
    check(classType->IsChildOf<UBTService>());
    
    UBTService* result = NewObject<UBTService>(outer, classType);
    result->NodeName = name;
    
    for( TPair<FName, UGeneratedProperty*> pair : propertyDict->GetProperties())
    {
        if ( !pair.Value->SetOn(result) )
        {
            UE_LOG(LogEvolvingBehaviorError, Error, TEXT("Unable to set property on Service node:  %s (%s)"), *name, *classType->GetName());
        }
    }
    
   return result;
}

UBTServiceGene* UBTGeneratedServiceGene::DuplicateService( UObject* outer )
{
    UBTGeneratedServiceGene* other = DuplicateObject<UBTGeneratedServiceGene>(this, outer);
    
    return other;
}

FString UBTGeneratedServiceGene::ToString()
{
    FString result = FString::Printf(TEXT("%s (%s)\n"), *name, *classType->GetName());

    for(TPair<FName, UGeneratedProperty*> pair : propertyDict->GetProperties())
    {
        result += FString::Printf(TEXT("    %s"), *pair.Value->ToString());
    }

    return result;
}

void UBTGeneratedServiceGene::DataLoaded_Implementation()
{
    classType = FindObject<UClass>(ANY_PACKAGE, *className);
    if ( nullptr == classType)
    {
        classType = StaticLoadClass(UBTService::StaticClass(), nullptr, *className, nullptr, LOAD_None, nullptr);
    }

    if ( nullptr == classType)
    {
        UE_LOG(LogEvolvingBehaviorError, Error, TEXT("Class could not be loaded for BTGeneratedServiceGene: %s"), *className);
        classType = UBTService::StaticClass();
    }
}

void UBTGeneratedServiceGene::DataSaved_Implementation()
{
    className = classType->GetPathName();
}