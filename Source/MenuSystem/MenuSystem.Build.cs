// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class MenuSystem : ModuleRules
{
	public MenuSystem(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput", "OnlineSubsystemSteam", "OnlineSubsystem", "MultiplayerSessions", "UMG", "Slate", "SlateCore"});
        PrivateDependencyModuleNames.AddRange(
        new string[]
        {
                "CoreUObject",
                "Engine",
                "MultiplayerSessions",
                "UMG",
                "Slate",
                "SlateCore",
            // ... add private dependencies that you statically link with here ...	
        }
        );
    }
}
