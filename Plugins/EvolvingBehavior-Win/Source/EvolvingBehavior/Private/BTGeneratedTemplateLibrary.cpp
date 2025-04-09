#include "BTGeneratedTemplateLibrary.h"

#include "EvolvingBehavior.h"

#include "BehaviorTree/BTCompositeNode.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BehaviorTree/BTDecorator.h"
#include "BehaviorTree/BTService.h"

#include "RandomGen.h"

UBTGeneratedTaskGene* UBTGeneratedTemplateLibrary::GetRandomGeneratedTask( URandomGen* randomGen, UObject* parentObject ) {
    if (tasks.Num() == 0)
    {
        UE_LOG( LogEvolvingBehavior, Display, TEXT( "There are no Generated Tasks in the Generated node library, but you tried to add one." ) );
        return nullptr;
    }
    
    int32 randIdx = randomGen->UniformIntInRange( 0, tasks.Num() - 1 );
    
    UBTGeneratedTaskGene* taskTemplate = tasks[randIdx];
    
    FName uniqueName = MakeUniqueObjectName( parentObject, taskTemplate->StaticClass(), FName( *taskTemplate->GetName() ) );
    UBTGeneratedTaskGene* copy = Cast<UBTGeneratedTaskGene>( DuplicateObject( taskTemplate, parentObject, uniqueName ) );
    copy->SetServiceInjector(serviceInjector);
    
    return copy;
}

UBTGeneratedServiceGene* UBTGeneratedTemplateLibrary::GetRandomGeneratedService( URandomGen* randomGen, UObject* parentObject ) {
    if (services.Num() == 0)
    {
        UE_LOG( LogEvolvingBehavior, Display, TEXT( "There are no Generated Services in the Generated node library, but you tried to add one." ) );
        return nullptr;
    }
    
    int32 randIdx = randomGen->UniformIntInRange( 0, services.Num() - 1 );
    
    UBTGeneratedServiceGene* serviceTemplate = services[randIdx];
    
    FName uniqueName = MakeUniqueObjectName( parentObject, serviceTemplate->StaticClass(), FName( *serviceTemplate->GetName() ) );
    UBTGeneratedServiceGene* copy = Cast<UBTGeneratedServiceGene>( DuplicateObject( serviceTemplate, parentObject, uniqueName ) );
    copy->SetServiceInjector(serviceInjector);
    
    return copy;
}

UBTGeneratedDecoratorGene* UBTGeneratedTemplateLibrary::GetRandomGeneratedDecorator( URandomGen* randomGen, UObject* parentObject ) {
    if (decorators.Num() == 0)
    {
        UE_LOG( LogEvolvingBehavior, Display, TEXT( "There are no Generated Decorators in the Generated node library, but you tried to add one." ) );
        return nullptr;
    }
    
    int32 randIdx = randomGen->UniformIntInRange( 0, decorators.Num() - 1 );
    
    UBTGeneratedDecoratorGene* decoratorTemplate = decorators[randIdx];
    
    FName uniqueName = MakeUniqueObjectName( parentObject, decoratorTemplate->StaticClass(), FName( *decoratorTemplate->GetName() ) );
    UBTGeneratedDecoratorGene* copy = Cast<UBTGeneratedDecoratorGene>( DuplicateObject( decoratorTemplate, parentObject, uniqueName ) );
    copy->SetServiceInjector(serviceInjector);

    return copy;
}
