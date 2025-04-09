#include "ServiceInjector.h"

#include "EvolvingBehavior.h"

#include "Service.h"

bool operator == (const FServiceWrapper& left, const FServiceWrapper& right) {
    UClass* leftClass = left.classPtr;
    UClass* rightClass = right.classPtr;
    if (leftClass == nullptr || rightClass == nullptr) {
        return leftClass == nullptr && rightClass == nullptr;
    }
    return rightClass->IsChildOf(leftClass) || leftClass->IsChildOf(rightClass);
}

uint32 GetTypeHash(const FServiceWrapper& key) {
    return GetTypeHash(key.classPtr);
}

void UServiceInjector::RegisterService(UService* service) {
    UClass* serviceType = service->GetClass();
    UClass* serviceBase = serviceType->GetSuperClass();
    while( serviceBase != UService::StaticClass())
    {
        serviceType = serviceBase;
        serviceBase = serviceType->GetSuperClass();
    }
    
    FServiceWrapper wrapper(serviceType);
    
    services.Add(wrapper,service);
}

UService* UServiceInjector::GetService(UClass* srvClass) {
    FServiceWrapper wrapper(srvClass);
    if(services.Contains(wrapper)) {
        return services[wrapper];
    }
    return nullptr;
}
