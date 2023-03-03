// Copyright shenkns Data System Developed With Unreal Engine. All Rights Reserved 2022.

using UnrealBuildTool;

public class DataSystemEditor : ModuleRules
{
	public DataSystemEditor(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

		PrivateIncludePaths.AddRange(
			new string[]
			{
				"DataSystemEditor/Public/"
			}
		);
		
		PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"Core"
			}
		);

		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				"CoreUObject",
				"Engine",
				"UnrealEd",
				"DataSystem",
				"LogSystem",
				"AssetRegistry"
			}
		);
		
		PrivateIncludePathModuleNames.AddRange(
			new string[]
			{
				"DataSystem",
				"LogSystem"
			}
		);
	}
}