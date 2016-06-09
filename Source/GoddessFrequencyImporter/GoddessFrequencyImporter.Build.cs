// Author AndreaCatania

using UnrealBuildTool;

public class GoddessFrequencyImporter : ModuleRules
{
	public GoddessFrequencyImporter(TargetInfo Target)
	{
		
		PublicIncludePaths.AddRange(new string[] {"GoddessFrequencyImporter/Public"});
				
		
		PrivateIncludePaths.AddRange(new string[] {"GoddessFrequencyImporter/Private"});
			
		
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
