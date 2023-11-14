// Copyright shenkns Data System Developed With Unreal Engine. All Rights Reserved 2022.

using UnrealBuildTool;

public class DataSystem : ModuleRules
{
	public DataSystem(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;
		CppStandard = CppStandardVersion.Cpp17;

		PrivateIncludePaths.AddRange(
			new string[]
			{
				"DataSystem/Public/"
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
				"ManagersSystem",
				"LogSystem"
			}
		);

		PrivateIncludePathModuleNames.AddRange(
			new string[]
			{
				"ManagersSystem",
				"LogSystem"
			}
		);
	}
}
