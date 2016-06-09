using UnrealBuildTool;
using System.IO;

public class GoddessSoundVisualizer : ModuleRules
{
    public GoddessSoundVisualizer(TargetInfo Target)
    {
        PrivateIncludePaths.AddRange(new string[] { "GoddessSoundVisualizer/Private" });
        PublicIncludePaths.AddRange(new string[] { "GoddessSoundVisualizer/Public" });

        PublicDependencyModuleNames.AddRange(new string[] { "Engine", "Core", "InputCore", "CoreUObject" });
    }
}