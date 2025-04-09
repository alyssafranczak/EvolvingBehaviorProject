
#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"

#include "PropertyOwner.generated.h"

class UGeneratedProperty;
class UBoolGeneratedProperty;
class UFloatGeneratedProperty;
class UIntGeneratedProperty;

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UPropertyOwner : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class EVOLVINGBEHAVIOR_API IPropertyOwner
{
	GENERATED_BODY()

private:
	static TMap<FName, UGeneratedProperty*> EmptyPropertyMap;

public:
	
	virtual bool TryGetProperty(FName propertyName, UGeneratedProperty*& propertyResult)
	{
		propertyResult = nullptr;
		return false;
	};

	virtual bool TryGetProperty(FName propertyName, UBoolGeneratedProperty*& propertyResult)
	{
		propertyResult = nullptr;
		return false;
	};
	
	virtual bool TryGetProperty(FName propertyName, UIntGeneratedProperty*& propertyResult)
	{
		propertyResult = nullptr;
		return false;
	};
	
	virtual bool TryGetProperty(FName propertyName, UFloatGeneratedProperty*& propertyResult)
	{
		propertyResult = nullptr;
		return false;
	};
	
	virtual void SetProperty(UGeneratedProperty* value)
	{
		
	};
	
	virtual void SetBool(FName propertyName, bool value)
	{
		
	};

	virtual void SetFloat(FName propertyName, float value)
	{
		
	};
	
	virtual void SetInt(FName propertyName, uint32 value)
	{
		
	};

	virtual TMap<FName, UGeneratedProperty*>& GetProperties()
	{
		return IPropertyOwner::EmptyPropertyMap;
	};

};
