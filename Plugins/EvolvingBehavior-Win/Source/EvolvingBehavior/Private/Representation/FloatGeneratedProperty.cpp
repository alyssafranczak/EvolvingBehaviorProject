#include "FloatGeneratedProperty.h"

#include "EvolvingBehavior.h"

#include "RandomGen.h"

void UFloatGeneratedProperty::InitMutate(URandomGen* rand)
{
    this->val = rand->UniformRealInRange( lowerBound, upperBound );
}

void UFloatGeneratedProperty::Mutate(URandomGen* rand, double stdDevFractionOfRange)
{
    float gaussianOffset = rand->GaussianReal(0.0, (upperBound-lowerBound)*stdDevFractionOfRange);
    
    this->val += gaussianOffset;
    this->val = FMath::Clamp(this->val, lowerBound, upperBound);
}

bool UFloatGeneratedProperty::SetOn(UObject* object)
{
    UClass* classType = object->GetClass();
    FProperty* property = classType->FindPropertyByName(GetName());
    if (property == nullptr)
    {
        UE_LOG( LogEvolvingBehaviorError, Error, TEXT( "Property not available to set: %s" ), *ToString());
        return false;
    }
    
    FFloatProperty* floatProperty = CastField<FFloatProperty>(property);
    if ( nullptr == floatProperty )
    {
        UE_LOG( LogEvolvingBehaviorError, Error, TEXT( "Property not a float, could not set: %s" ), *ToString());
        return false;
    }

    floatProperty->SetPropertyValue(property->ContainerPtrToValuePtr<float>(object), GetVal());
    return true;
}