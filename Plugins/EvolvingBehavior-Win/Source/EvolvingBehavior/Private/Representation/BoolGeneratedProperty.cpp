#include "BoolGeneratedProperty.h"

#include "EvolvingBehavior.h"

#include "RandomGen.h"

void UBoolGeneratedProperty::InitMutate(URandomGen* rand)
{
    this->val = rand->WithProbability(0.5);
}

void UBoolGeneratedProperty::Mutate(URandomGen* rand, double stdDevFractionOfRange = 1.0)
{
    this->val = !this->val;
}

bool UBoolGeneratedProperty::SetOn(UObject* object)
{
    UClass* classType = object->GetClass();
    FProperty* property = classType->FindPropertyByName(GetName());
    if (property == nullptr)
    {
        UE_LOG( LogEvolvingBehaviorError, Error, TEXT( "Property not available to set: %s" ), *ToString());
        return false;
    }
    
    FBoolProperty* boolProperty = CastField<FBoolProperty>(property);
    if ( nullptr == boolProperty )
    {
        UE_LOG( LogEvolvingBehaviorError, Error, TEXT( "Property not a bool, could not set: %s" ), *ToString());
        return false;
    }

    boolProperty->SetPropertyValue(property->ContainerPtrToValuePtr<bool>(object), GetVal());
    return true;
}