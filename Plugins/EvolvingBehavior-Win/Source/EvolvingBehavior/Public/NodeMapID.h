#pragma once

#include "NodeMapID.generated.h"

/**
 * @file NodeMapID.h
 */

/**
 * @class FNodeMapID
 * @author npc
 * @date 12/01/17
 * @brief The ID for a Behavior Tree node in the NodeMapper.
 */
USTRUCT( Atomic )
struct FNodeMapID
{
    GENERATED_BODY()

public:
    static const FNodeMapID INVALID_ID;

private:
	UPROPERTY(SaveGame)
    FString stringID;

public:
    FNodeMapID() = default;

    FNodeMapID( const FString& string ) { stringID = string; }

    FNodeMapID( const FNodeMapID& other ) = default;

    FString ToString() { return stringID; }

    friend bool operator==( const FNodeMapID& first, const FNodeMapID& second )
    {
	    return first.stringID == second.stringID;
    }

    friend uint32 GetTypeHash( const FNodeMapID& other ) { return GetTypeHash( other.stringID ); }
};
