// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class SnapOutOfIt : ModuleRules
{
	public SnapOutOfIt(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "SnapOutOfIt", "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput", "AIModule" });
	}
}
