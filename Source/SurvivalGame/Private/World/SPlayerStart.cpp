// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.


#include "World/SPlayerStart.h"





ASPlayerStart::ASPlayerStart(const class FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer)
{
	bPlayerOnly = true;
	bAlreadySpawned = false;
}
