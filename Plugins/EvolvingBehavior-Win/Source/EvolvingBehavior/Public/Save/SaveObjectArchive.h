#pragma once

#include "Serialization/ObjectAndNameAsStringProxyArchive.h"
#include "SavedObjectData.h"
#include "Saveable.h"
#include "UObject/Interface.h"

// Based on code by Rune de Groot on saving Actor data
// http://runedegroot.com/saving-and-loading-actor-data-in-unreal-engine-4/
struct EVOLVINGBEHAVIOR_API FSaveObjectArchive : public FObjectAndNameAsStringProxyArchive
{
private:
    UPROPERTY()
    UObject* OuterObject;

public:
    FSaveObjectArchive(UObject* outer, FArchive& innerArchive);

    FORCEINLINE virtual FArchive& operator<<(UObject*& Obj) override
    {
        if (IsLoading())
        {
            FSavedObjectData data;
            InnerArchive << data;

            if (nullptr == Obj)
            {   
                UClass* SpawnClass = FindObject<UClass>(ANY_PACKAGE, *data.ObjectClass);  
                if ( nullptr == SpawnClass)
                {
                    SpawnClass = StaticLoadClass(UObject::StaticClass(), nullptr, *data.ObjectClass, nullptr, LOAD_None, nullptr);
                }
                Obj = NewObject<UObject>(OuterObject, SpawnClass, data.ObjectName);
            }

            FMemoryReader memoryReader(data.ObjectData, InnerArchive.IsPersistent());
            FSaveObjectArchive archive(Obj, memoryReader);
            Obj->Serialize(archive);

            if (Obj->GetClass()->ImplementsInterface(USaveable::StaticClass()))
            {
                ISaveable::Execute_DataLoaded(Obj);
            }
        }
        else
        {
            FSavedObjectData data;
            data.ObjectName = FName(*Obj->GetName());
            data.ObjectClass = Obj->GetClass()->GetPathName();
            
            if (Obj->GetClass()->ImplementsInterface(USaveable::StaticClass()))
            {
                ISaveable::Execute_DataSaved(Obj);
            }
            
            FMemoryWriter memoryWriter(data.ObjectData, InnerArchive.IsPersistent());
            FSaveObjectArchive archive(Obj, memoryWriter);
            Obj->Serialize(archive);
            
            InnerArchive << data;
        }

        return *this;        
    }
};