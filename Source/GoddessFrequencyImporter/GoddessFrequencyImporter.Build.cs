/// GoddessSoundVisualizer plugin for UnrealEngine
/// Author AndreaCatania

namespace UnrealBuildTool.Rules
{
    public class GoddessFrequencyImporter : ModuleRules{
        public GoddessFrequencyImporter(ReadOnlyTargetRules Target) : base(Target){
            PublicIncludePaths.AddRange(new string[] { "GoddessFrequencyImporter/Public" });
            
            PrivateIncludePaths.AddRange(new string[] { "GoddessFrequencyImporter/Private" });
            
            PublicDependencyModuleNames.AddRange(
                new string[]
                {
                  "Core"
                , "CoreUObject"
                , "Engine"
                , "Kiss_FFT"
                , "GoddessSoundVisualizer"
                , "UnrealEd"
                });
        }
    }
}
