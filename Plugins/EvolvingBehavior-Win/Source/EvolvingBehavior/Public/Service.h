#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Service.generated.h"

class UServiceInjector;

/**
 * @file Service.h
 */

/**
 * @class UService
 * @author jim
 * @date 4/28/20
 * @brief An abstract service class
 */
UCLASS()
class EVOLVINGBEHAVIOR_API UService : public UObject
{
    GENERATED_BODY()
    
protected:
    UPROPERTY()
    UServiceInjector* serviceInjector;
    
public:
    virtual void Init(UServiceInjector* injector);
};
