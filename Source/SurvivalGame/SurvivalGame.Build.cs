// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class SurvivalGame : ModuleRules
{
	public SurvivalGame(ReadOnlyTargetRules Target) : base(Target)
    {
		PrivatePCHHeaderFile = "SurvivalGame.h";
		
        /* AIModule is not assigned by default, must be added when dealing with AI in your project */
        PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "AIModule", "EvolvingBehavior",
            /* Temporarily added GameplayTasks to workaround 4.12 compilation bug. */
            "GameplayTasks" , "NavigationSystem"});

        //PublicIncludePaths.AddRange(new string[] { "" });

		PrivateDependencyModuleNames.AddRange(new string[] { "EvolvingBehavior" });

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });
		
		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");
		// if ((Target.Platform == UnrealTargetPlatform.Win32) || (Target.Platform == UnrealTargetPlatform.Win64))
		// {
		//		if (UEBuildConfiguration.bCompileSteamOSS == true)
		//		{
		//			DynamicallyLoadedModuleNames.Add("OnlineSubsystemSteam");
		//		}
		// }
	}
}
