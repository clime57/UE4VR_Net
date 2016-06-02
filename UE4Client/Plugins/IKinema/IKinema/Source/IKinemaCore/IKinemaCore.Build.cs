using UnrealBuildTool;
using System;
using System.IO;
public class IKinemaCore : ModuleRules
{
	public IKinemaCore(TargetInfo Target)
	{
        //Currently only Win64 is supported
       // if ((Target.Platform == UnrealTargetPlatform.Win64))
        {
            PublicIncludePaths.AddRange(new string[]{
			"IKinemaCore/Classes",
			"IKinemaCore/Public",
            });
            PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "AnimGraphRuntime" });


        }
	}
}
