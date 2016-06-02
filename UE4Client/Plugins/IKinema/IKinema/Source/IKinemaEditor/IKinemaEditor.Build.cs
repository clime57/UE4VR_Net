using UnrealBuildTool;

public class IKinemaEditor : ModuleRules
{
	public IKinemaEditor(TargetInfo Target)
	{
		//PublicIncludePaths.AddRange(new string[] {
		//	"Plugins/IKinemaSource/Source/IKinemaEditor/Public",
		//	"Plugins/IKinemaSource/Source/IKinemaEditor/Classes",
		//	"Editor/UnrealEd/Public",
		//	BuildConfiguration.BaseIntermediatePath + "Include/IKinemaEditor",
		//	BuildConfiguration.BaseIntermediatePath + "Include/IKinemaCore"
		//});
		PrivateIncludePaths.AddRange(new string[] {
			"IKinemaEditor/Public",
			"IKinemaEditor/Private",
			"IKinemaCore/Classes"
		});

		
        PublicDependencyModuleNames.AddRange(new string[] {"Projects", "Core", "CoreUObject", "Engine", "UnrealEd", "IKinemaCore", "JsonUtils", "BlueprintGraph", "AnimGraph", "Slate", 
				"SlateCore" ,
				"InputCore",
				"EditorStyle" });
        PrivateDependencyModuleNames.AddRange(new string[] { "UnrealEd", "Slate", "MessageLog", "JsonUtils", "ContentBrowser", "ClassViewer", "IKinemaRigTool" });
		PrivateIncludePathModuleNames.AddRange(new string[] { "AssetTools", "PropertyEditor", "JsonUtils" });
		DynamicallyLoadedModuleNames.AddRange(new string[] { "AssetTools", "PropertyEditor" });

	}
}