// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class Online_RPG : ModuleRules
{
	public Online_RPG(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput","Networking",  "Sockets" , "UMG"});
	}
}
