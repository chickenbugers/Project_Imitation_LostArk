// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class LostarkImitation : ModuleRules
{
	public LostarkImitation(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicIncludePaths.AddRange(new string[] {
		});

        PublicDependencyModuleNames.AddRange(new string[] {
			"Core",
			"CoreUObject",
			"Engine",
			"InputCore",
			"EnhancedInput",
			"AIModule",
			"NavigationSystem",
			"StateTreeModule",
			"GameplayStateTreeModule",
			"Niagara",
			"UMG",
			"Slate"
		});

		PrivateDependencyModuleNames.AddRange(new string[] { });

		PublicIncludePaths.AddRange(new string[] {
			"LostarkImitation",
			"LostarkImitation/Variant_Strategy",
			"LostarkImitation/Variant_Strategy/UI",
			"LostarkImitation/Variant_TwinStick",
			"LostarkImitation/Variant_TwinStick/AI",
			"LostarkImitation/Variant_TwinStick/Gameplay",
			"LostarkImitation/Variant_TwinStick/UI"
		});

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
