/// GoddessSoundVisualizer plugin for UnrealEngine
/// Author AndreaCatania

using UnrealBuildTool;
using System.IO;

namespace UnrealBuildTool.Rules
{
    public class GoddessSoundVisualizer : ModuleRules
    {
        public GoddessSoundVisualizer(ReadOnlyTargetRules Target) : base(Target)
        {
            PrivateIncludePaths.AddRange(new string[] { "GoddessSoundVisualizer/Private" });
            PublicIncludePaths.AddRange(new string[] { "GoddessSoundVisualizer/Public" });

            PublicDependencyModuleNames.AddRange(new string[] { "Engine", "Core", "InputCore", "CoreUObject" });
        }
    }
}