#include "IntGeneratedProperty.h"

#include "EvolvingBehavior.h"

#include "RandomGen.h"

void UIntGeneratedProperty::InitMutate(URandomGen* rand)
{
    this->val = rand->UniformIntInRange( lowerBound, upperBound );
}

void UIntGeneratedProperty::Mutate(URandomGen* rand, double stdDevFractionOfRange)
{
    float realGaussian = rand->GaussianReal(0.0, (upperBound-lowerBound)*stdDevFractionOfRange);
    // Never perturb by exactly 0, since that would be a no-op.
    if (realGaussian > 0 && realGaussian < 1)
    {
        realGaussian = 1.0f;
    }
    else if (realGaussian < 0 && realGaussian > -1)
    {
        realGaussian = -1.0f;
    }
    int32 gaussianOffset = FMath::RoundToInt(realGaussian);
    
    this->val += gaussianOffset;
    this->val = FMath::Clamp(this->val, lowerBound, upperBound);
}

bool UIntGeneratedProperty::SetOn(UObject* object)
{
    UClass* classType = object->GetClass();
    FProperty* property = classType->FindPropertyByName(GetName());
    if (property == nullptr)
    {
        UE_LOG( LogEvolvingBehaviorError, Error, TEXT( "Property not available to set: %s" ), *ToString());
        return false;
    }
    
    FIntProperty* intProperty = CastField<FIntProperty>(property);
    if ( nullptr == intProperty )
    {
        UE_LOG( LogEvolvingBehaviorError, Error, TEXT( "Property not an int, could not set: %s" ), *ToString());
        return false;
    }

    intProperty->SetPropertyValue(property->ContainerPtrToValuePtr<int32>(object), GetVal());
    return true;
}