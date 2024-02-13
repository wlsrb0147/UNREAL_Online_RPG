// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class Online_RPG : ModuleRules
{
	public Online_RPG(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput","Networking",  "Sockets" , "UMG", "Http", "Json", "JsonUtilities", "BehaviorTree" ,"GameplayTasks"});
		
		//PublicIncludePaths.Add(Path.Combine(ModuleDirectory, "../../ThirdParty/connector/include/mysql"));
//PublicAdditionalLibraries.Add(Path.Combine(ModuleDirectory, "../../ThirdParty/connector/lib64", "YourLibrary.lib"));
//PublicDelayLoadDLLs.Add("YourLibrary.dll");
//RuntimeDependencies.Add(Path.Combine(ModuleDirectory, "../../ThirdParty/connector/lib64", "mysqlcppconn-9-vs14.dll"));
        
         // MySQL Connector/C++ 헤더 파일 경로
        
	}
}
