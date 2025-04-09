#include "BTTemplateCollection.h"

#include "EvolvingBehavior.h"
#include "NodeMapper.h"
#include "BehaviorTreeUtils.h"
#include "BehaviorTree/BehaviorTree.h"

void UBTTemplateCollection::VerifyTemplates()
{
    for ( UBehaviorTree* behaviorTree : initialPopulationTemplates )
    {
        if ( behaviorTree == nullptr ) {
            UE_LOG(LogEvolvingBehaviorError, Error, TEXT("You have invalid or null behavior trees selected in the Template Collection within the editor. Please remove them or replace them with valid ones." ));
            initialPopulationTemplates.Remove(behaviorTree);
        }
    }
    
    for ( UBehaviorTree* behaviorTree : extraTemplates )
    {
        if ( behaviorTree == nullptr ) {
            UE_LOG(LogEvolvingBehaviorError, Error, TEXT("You have invalid or null behavior trees selected in the Template Collection within the editor. Please remove them or replace them with valid ones." ));
            extraTemplates.Remove(behaviorTree);
        }
    }
}

void UBTTemplateCollection::MapTemplates( UNodeMapper* nodeMapper )
{
    MapTemplates( initialPopulationTemplates, nodeMapper );

    MapTemplates( extraTemplates, nodeMapper );
}

void UBTTemplateCollection::MapTemplates( TArray<UBehaviorTree*>& templates, UNodeMapper* nodeMapper )
{
    for( int i = 0; i < templates.Num(); ++i )
	{
	    UBehaviorTree* behaviorTree = templates[i];
        
        UBehaviorTreeUtils::MapBehaviorTree( nodeMapper, behaviorTree );
	}
}

UBehaviorTree* UBTTemplateCollection::GetInitialTemplateByID(FString behaviorTreePath)
{
	for (int i = 0; i < initialPopulationTemplates.Num(); ++i)
	{
		UBehaviorTree* behaviorTree = initialPopulationTemplates[i];
		if (behaviorTree->GetPathName() == behaviorTreePath)
		{
			return behaviorTree;
		}
	}

	return nullptr;
}
