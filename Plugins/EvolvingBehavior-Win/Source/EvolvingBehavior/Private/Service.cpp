#include "Service.h"
#include "ServiceInjector.h"
#include "EvolvingBehavior.h"

void UService::Init( UServiceInjector* injector )
{
    serviceInjector = injector;
    serviceInjector->RegisterService(this);
}
