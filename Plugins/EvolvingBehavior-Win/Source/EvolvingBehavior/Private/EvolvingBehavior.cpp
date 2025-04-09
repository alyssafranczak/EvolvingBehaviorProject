#include "EvolvingBehavior.h"
#include "Modules/ModuleManager.h"
#include "Interfaces/IPluginManager.h"

#define LOCTEXT_NAMESPACE "FEvolvingBehaviorModule"

DEFINE_LOG_CATEGORY(LogEvolvingBehavior);
DEFINE_LOG_CATEGORY(LogEvolvingBehaviorError);
DEFINE_LOG_CATEGORY(LogEvolvingBehaviorWarning);

void FEvolvingBehaviorModule::StartupModule()
{
	
}

void FEvolvingBehaviorModule::ShutdownModule()
{
	
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FEvolvingBehaviorModule, EvolvingBehavior)
