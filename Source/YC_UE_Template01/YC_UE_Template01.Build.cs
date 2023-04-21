using UnrealBuildTool;

public class YC_UE_Template01 : ModuleRules
{
	public YC_UE_Template01(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
		CppStandard = CppStandardVersion.Latest;
		
		PublicDependencyModuleNames.AddRange(new string[]
		{
			"Core", 
			"CoreUObject", 
			"Engine", 
			"InputCore", 
			"HeadMountedDisplay", 
			"EnhancedInput"
		});
		PublicIncludePaths.AddRange(new string[]
		{
			"C:\\YuChan\\C++\\GameEngineUtils\\src"
		});
	}
}
