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
			"EnhancedInput",
			"UMG"
		});
		PublicIncludePaths.AddRange(new string[]
		{
			"C:\\YuChan\\C++\\GameEngineUtils\\src"
		});
		
		PublicDefinitions.Add("_HAS_CXX20=1");
		PublicDefinitions.Add("_SILENCE_CLANG_COROUTINE_MESSAGE");
	}
}
