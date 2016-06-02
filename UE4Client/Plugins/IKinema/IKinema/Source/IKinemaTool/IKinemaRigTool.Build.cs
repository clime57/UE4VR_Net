// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class IKinemaRigTool : ModuleRules
{
    public IKinemaRigTool(TargetInfo Target)
    {
        PublicIncludePaths.Add("Editor/UnrealEd/Public");


        PrivateDependencyModuleNames.AddRange(
            new string[] {
                "Projects",
                "Core",
                "CoreUObject",
                "Engine",
                "InputCore",
                "RenderCore",
                "Slate",
                "SlateCore",
                "EditorStyle",
                "PropertyEditor",
                "LevelEditor",
                "UnrealEd",
                "Kismet",
                "Persona",
                "ClassViewer",
                "IKinemaCore",
                "AnimGraph",
                "AnimGraphRuntime"
			}
		);
		PublicAdditionalLibraries.Add("ws2_32.lib");
        PublicAdditionalLibraries.Add("Winhttp.lib");
		
        DynamicallyLoadedModuleNames.AddRange(
            new string[] {
                "MainFrame",
                "MeshUtilities",
			}
		);
		
	}
}
