#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_MoveTo.h"

#include "BTTaskBlueprintable_MoveTo.generated.h"

/**
 * @file BTTaskBlueprintable_MoveTo.h
 */

 /**
  * @class BTTaskBlueprintable_MoveTo
  * @author npcdev
  * @date 10/29/21
  * @file BTTaskBlueprintable_MoveTo.h
  * @brief Extends BTTask_MoveTo to make it blueprintable, so you can subclass it in blueprint and set default values for its properties.
  */
UCLASS(ClassGroup = (Custom), BlueprintType, Blueprintable, DefaultToInstanced)
class EVOLVINGBEHAVIOR_API UBTTaskBlueprintable_MoveTo : public UBTTask_MoveTo
{
    GENERATED_BODY()
};