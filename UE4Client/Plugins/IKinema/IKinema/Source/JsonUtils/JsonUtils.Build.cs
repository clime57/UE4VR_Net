// Copyright (C) Microsoft. All rights reserved.

using UnrealBuildTool;

public class JsonUtils : ModuleRules
{
	public JsonUtils(TargetInfo Target)
	{
		PublicIncludePaths.AddRange(new string[] {
			"Plugins/IKinema/Source/JsonUtils/Public",
			"Plugins/IKinema/Source/JsonUtils/Classes",
			BuildConfiguration.BaseIntermediatePath + "Include/JsonUtils"
		});
		PrivateIncludePaths.AddRange(new string[] {
			"Plugins/IKinema/Source/JsonUtils/Private",

		});
		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine"});
		PrivateDependencyModuleNames.AddRange(new string[] { "MessageLog","Json", "Core" });
	}
}
