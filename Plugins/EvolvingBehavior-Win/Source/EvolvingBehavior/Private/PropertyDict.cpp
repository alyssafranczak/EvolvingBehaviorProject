#include "PropertyDict.h"

#include "BoolGeneratedProperty.h"
#include "IntGeneratedProperty.h"
#include "FloatGeneratedProperty.h"

bool UPropertyDict::TryGetProperty(FName propertyName, UBoolGeneratedProperty*& propertyResult)
{
    UGeneratedProperty* baseResult = nullptr;
    if (!TryGetProperty(propertyName, baseResult))
    {
        propertyResult = nullptr;
        return false;
    }

    propertyResult = Cast<UBoolGeneratedProperty>(baseResult);

    return propertyResult != nullptr;
}

bool UPropertyDict::TryGetProperty(FName propertyName, UIntGeneratedProperty*& propertyResult)
{
    UGeneratedProperty* baseResult = nullptr;
    if (!TryGetProperty(propertyName, baseResult))
    {
        propertyResult = nullptr;
        return false;
    }

    propertyResult = Cast<UIntGeneratedProperty>(baseResult);

    return propertyResult != nullptr;
}

bool UPropertyDict::TryGetProperty(FName propertyName, UFloatGeneratedProperty*& propertyResult)
{
    UGeneratedProperty* baseResult = nullptr;
    if (!TryGetProperty(propertyName, baseResult))
    {
        propertyResult = nullptr;
        return false;
    }

    propertyResult = Cast<UFloatGeneratedProperty>(baseResult);

    return propertyResult != nullptr;
}

void UPropertyDict::SetProperty(UGeneratedProperty* value)
{
    properties.Add(value->GetName(), value);
}

void UPropertyDict::SetBool(FName propertyName, bool value)
{
    FName uniqueName = MakeUniqueObjectName(this, UBoolGeneratedProperty::StaticClass(), propertyName);
    UBoolGeneratedProperty* newProperty = NewObject<UBoolGeneratedProperty>(this, uniqueName);
    newProperty->SetName(propertyName);
    newProperty->SetVal(value);
    properties.Add(propertyName, newProperty);
}

void UPropertyDict::SetFloat(FName propertyName, float value)
{
    FName uniqueName = MakeUniqueObjectName(this, UBoolGeneratedProperty::StaticClass(), propertyName);
    UFloatGeneratedProperty* newProperty = NewObject<UFloatGeneratedProperty>(this, uniqueName);
    newProperty->SetName(propertyName);
    newProperty->SetVal(value);
    properties.Add(propertyName, newProperty);
}

void UPropertyDict::SetInt(FName propertyName, uint32 value)
{
    FName uniqueName = MakeUniqueObjectName(this, UBoolGeneratedProperty::StaticClass(), propertyName);
    UIntGeneratedProperty* newProperty = NewObject<UIntGeneratedProperty>(this, uniqueName);
    newProperty->SetName(propertyName);
    newProperty->SetVal(value);
    properties.Add(propertyName, newProperty);
};

#if WITH_EDITOR
void UPropertyDict::PostEditChangeProperty( FPropertyChangedEvent& changeEvent)
{
    if (!changeEvent.Property || 
        changeEvent.GetPropertyName() != GET_MEMBER_NAME_CHECKED(UPropertyDict, properties) ||
        changeEvent.ChangeType != EPropertyChangeType::ValueSet)
    {
        return;
    }

    UGeneratedProperty* propertyToAdd = nullptr;
    for( auto property : properties)
    {
        if ( property.Key.IsNone() )
        {
            // If we have no key, but we named the property internally, we need to delete and re-add with the new key.
            if ( property.Value != nullptr && !property.Value->GetName().IsNone() )
            {
                propertyToAdd = property.Value;
            }
        }
        else if ( property.Value != nullptr && property.Value->GetName() != property.Key )
        {
            // If we have a key name, but the property itself doesn't match the key, we need to update the property name to match.
            // TODO: It would be better to have some way of telling which value the user edited, and just updating the opposite one.
            property.Value->SetName(property.Key);
        }
    }

    if ( propertyToAdd != nullptr)                
    {
        properties.Add(propertyToAdd->GetName(), propertyToAdd);
        properties.Remove(FName());
    }
}
#endif