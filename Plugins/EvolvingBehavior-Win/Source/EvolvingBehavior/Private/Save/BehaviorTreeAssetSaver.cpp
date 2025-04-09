#include "Save/BehaviorTreeAssetSaver.h"

#include "BehaviorTree/BehaviorTree.h"
#include "EvolvingBehavior.h"
#include "Misc/PackageName.h"

bool UBehaviorTreeAssetSaver::SaveTree(UBehaviorTree* tree, 
                                        const FString& treeName, 
                                        const FString& packagePath)
{
    // The package name controls where the Behavior Tree is saved. You can either split each tree into a separate file
    // or save multiple trees in a single package by using the same package name.
    //
    // The Package Name should be something like "/Game/EvolvingBehavior/<TrialNum>/BT-Fitness_<fitness>-ID_<id>"
    // in order to create a single Package per Behavior Tree, separated by Trial.
    //
    const FString packageName = FString::Printf(TEXT("%s/%s"), *packagePath, *treeName);
	UPackage *treePackage = CreatePackage(*packageName);
    
    // In some cases, it appears we need to force the MetaData for this package to be created, so it doesn't
    // try to load it while serializing later.
    UMetaData* metaData = treePackage->GetMetaData();
    if ( nullptr == metaData )
    {
        UE_LOG(LogEvolvingBehaviorError, Error, TEXT("Could not retrieve or create metadata for new BT package: %s"), *packageName);
        return false;
    }
    
    // We can generate a filename using this function based on the package name.
    FString fileName = FPackageName::LongPackageNameToFilename(packageName, FPackageName::GetAssetPackageExtension());

    // We need to make a unique copy of the Behavior Tree with the correct name and put it in the Package as the parent.
    FName treeObjectName = MakeUniqueObjectName(treePackage, UBehaviorTree::StaticClass(), FName(*treeName));
    UBehaviorTree* treeCopy = DuplicateObject(tree, treePackage, treeObjectName);

    // Now we can save the tree into the package with this SavePackage call.
    bool success = UPackage::SavePackage(treePackage, treeCopy, RF_Public | RF_Standalone, *fileName, GLog);
    if (!success)
    {
        UE_LOG(LogEvolvingBehaviorError, Error, TEXT("Error: could not save package %s at: %s"), *packageName, *fileName);
    }

    return success;
}
