#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Service.h"
#include "ServiceInjector.generated.h"

class UService;

USTRUCT()
struct FServiceWrapper
{
    // Inspired by code written by Daniel Hilburn
    // https://answers.unrealengine.com/questions/421477/is-it-possible-to-use-a-uclass-as-a-tmap-key.html
    GENERATED_USTRUCT_BODY()
    
    UPROPERTY()
    UClass* classPtr;
    
    FServiceWrapper() : classPtr() {}
    
    FServiceWrapper(UClass* inClass) : classPtr(inClass) {}
};
bool operator == (const FServiceWrapper& left, const FServiceWrapper& right);
uint32 GetTypeHash(const FServiceWrapper& key);

/**
 * @class UServiceInjector
 * @author jim
 * @date 4/28/20
 * @file ServiceInjector.h
 * @brief A service injector which provides relevant collection to different mutators
 */
UCLASS( Blueprintable, BlueprintType, EditInlineNew, DefaultToInstanced )
class EVOLVINGBEHAVIOR_API UServiceInjector : public UObject
{
    GENERATED_BODY()
    
private:
    
public:
    UPROPERTY()
    TMap<FServiceWrapper, UService*> services;
    
    void RegisterService(UService* service);
    
    UService* GetService(UClass* srvClass);

    template<class T>
    T* GetService()
    {
        return Cast<T>(GetService(T::StaticClass()));
    }
};
