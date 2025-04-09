#include "Save/SaveObjectArchive.h"

FSaveObjectArchive::FSaveObjectArchive(UObject* outer, FArchive& innerArchive) : FObjectAndNameAsStringProxyArchive(innerArchive, true)
{
    OuterObject = outer;
    ArIsSaveGame = true;
}