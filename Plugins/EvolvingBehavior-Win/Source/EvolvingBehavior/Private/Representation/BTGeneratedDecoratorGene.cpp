#include "BTGeneratedDecoratorGene.h"

#include "EvolvingBehavior.h"

#include "BehaviorTree/BTDecorator.h"

#include "PropertyDict.h"
#include "ServiceInjector.h"

UBTGeneratedDecoratorGene::UBTGeneratedDecoratorGene() : UBTDecoratorGene()
{

}

void UBTGeneratedDecoratorGene::RandomizeAllProperties()
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

void UBTGeneratedDecoratorGene::MutateRandomProperty(float stdDevFractionOfRange)
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

UBTDecoratorGene* UBTGeneratedDecoratorGene::DuplicateDecorator( UObject* outer )
{
    UBTGeneratedDecoratorGene* other = DuplicateObject<UBTGeneratedDecoratorGene>(this, outer);

    return other;
}

UBTDecorator* UBTGeneratedDecoratorGene::BuildPhenotype( UObject* outer )
{
    check(classType->IsChildOf<UBTDecorator>());
    
    UBTDecorator* result = NewObject<UBTDecorator>(outer, classType);
    result->NodeName = name;
    
    for( TPair<FName, UGeneratedProperty*> pair : propertyDict->GetProperties())
    {
        if ( !pair.Value->SetOn(result) )
        {
            UE_LOG(LogEvolvingBehaviorError, Error, TEXT("Unable to set property on Decorator node:  %s (%s)"), *name, *classType->GetName());
        }
    }
   
    return result;
}

FString UBTGeneratedDecoratorGene::ToString()
{
    FString result = FString::Printf(TEXT("%s (%s)\n"), *name, *classType->GetName());
    
    for(TPair<FName, UGeneratedProperty*> pair : propertyDict->GetProperties())
    {
        result += FString::Printf(TEXT("    %s"), *pair.Value->ToString());
    }

    return result;
}

void UBTGeneratedDecoratorGene::DataLoaded_Implementation()
{
    classType = FindObject<UClass>(ANY_PACKAGE, *className);
    if ( nullptr == classType)
    {
        classType = StaticLoadClass(UBTDecorator::StaticClass(), nullptr, *className, nullptr, LOAD_None, nullptr);
    }

    if ( nullptr == classType)
    {
        UE_LOG(LogEvolvingBehaviorError, Error, TEXT("Class could not be loaded for BTGeneratedDecoratorGene: %s"), *className);
        classType = UBTDecorator::StaticClass();
    }
}

void UBTGeneratedDecoratorGene::DataSaved_Implementation()
{
    className = classType->GetPathName();
}