
#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "PropertyOwner.h"

#include "PropertyDict.generated.h"

/**
 * 
 */
UCLASS( Blueprintable, BlueprintType, EditInlineNew, DefaultToInstanced, CollapseCategories )
class EVOLVINGBEHAVIOR_API UPropertyDict : public UObject, public IPropertyOwner
{
	GENERATED_BODY()

private:
	UPROPERTY( EditAnywhere, Instanced, SaveGame, Category = "EvolvingBehavior")
	TMap<FName, UGeneratedProperty*> properties;

public:	
	virtual bool TryGetProperty(FName propertyName, UGeneratedProperty*& propertyResult)
	{
		if ( !properties.Contains(propertyName))
		{
			propertyResult = nullptr;
			return false;
		}

		propertyResult = properties[propertyName];
		return true;
	};

	virtual bool TryGetProperty(FName propertyName, UBoolGeneratedProperty*& propertyResult) override;

	virtual bool TryGetProperty(FName propertyName, UIntGeneratedProperty*& propertyResult) override;

	virtual bool TryGetProperty(FName propertyName, UFloatGeneratedProperty*& propertyResult) override;

	virtual void SetProperty(UGeneratedProperty* value) override;
	
	virtual void SetBool(FName propertyName, bool value) override;

	virtual void SetFloat(FName propertyName, float value) override;

	virtual void SetInt(FName propertyName, uint32 value) override;

	virtual TMap<FName, UGeneratedProperty*>& GetProperties() override { return properties; }

#if WITH_EDITOR
	virtual void PostEditChangeProperty( FPropertyChangedEvent& changeEvent ) override;
#endif
};
