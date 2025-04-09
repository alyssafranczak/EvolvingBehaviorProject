#include "Save/SaveEvolvingBehavior.h"
#include "EvolvingBehavior.h"
#include "Save/SavedObjectData.h"
#include "Serialization/MemoryWriter.h"
#include "Save/SaveObjectArchive.h"
#include "Serialization/MemoryReader.h"
#include "Save/Saveable.h"

#include "EvolvingBehavior.h"

USaveEvolvingBehavior::USaveEvolvingBehavior()
{
	SaveSlotName = TEXT("TestSaveSlot");
	UserIndex = 0;
}

void USaveEvolvingBehavior::SaveObject(UObject* object) 
{
	FSavedObjectData data;
	data.ObjectName = FName(*object->GetName());
	data.ObjectClass = object->GetClass()->GetPathName();
	
	// If we already saved an object with the same name, show a warning.
	if (CheckForDuplicateObjectNames(data.ObjectName))
	{
		UE_LOG(LogEvolvingBehaviorError, Warning, TEXT("Duplicate object name being saved: %s"), *(data.ObjectName.ToString()))
	}

	if (object->GetClass()->ImplementsInterface(USaveable::StaticClass()))
	{
		ISaveable::Execute_DataSaved(object);
	}

	FMemoryWriter memoryWriter(data.ObjectData, true);
	FSaveObjectArchive archive(object, memoryWriter);
	object->Serialize(archive);

	archive.FlushCache();
	archive.Close();

	ObjectData.Add(data);
}

bool USaveEvolvingBehavior::CheckForDuplicateObjectNames(FName name)
{
	for ( int objectIdx = 0; objectIdx < ObjectData.Num(); ++objectIdx )
	{
		if (ObjectData[objectIdx].ObjectName == name)
		{
			return true;
		}
	}

	return false;
}

FSavedObjectData& USaveEvolvingBehavior::LoadObjectData(FName name)
{
	for( int objectIdx = 0; objectIdx < ObjectData.Num(); ++objectIdx)
	{
		if (ObjectData[objectIdx].ObjectName != name)
		{
			continue;
		}

		return ObjectData[objectIdx];
	}

	return FSavedObjectData::Empty;
}

bool USaveEvolvingBehavior::LoadObject(UObject* existingObject)
{
    FSavedObjectData& loadedData = this->LoadObjectData(FName(*existingObject->GetName()));

	if (loadedData.ObjectName == FSavedObjectData::Empty.ObjectName)
	{
		UE_LOG(LogEvolvingBehaviorError, Warning, TEXT("Failed to load object: %s"), *existingObject->GetName());
		return false;
	}

    FMemoryReader reader(loadedData.ObjectData, true);
    FSaveObjectArchive loadArchive(existingObject, reader);
	loadArchive.Seek(0);
    existingObject->Serialize(loadArchive);

	loadArchive.FlushCache();
	loadArchive.Close();
	if (existingObject->GetClass()->ImplementsInterface(USaveable::StaticClass()))
	{
		ISaveable::Execute_DataLoaded(existingObject);
	}

	return true;
}