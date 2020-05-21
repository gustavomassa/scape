// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class scapeEditorTarget : TargetRules
{
    public scapeEditorTarget(TargetInfo Target) : base(Target)
    {
        Type = TargetType.Editor;
        DefaultBuildSettings = BuildSettingsVersion.V2;
        //bForcePrecompiledHeaderForGameModules = true;

        ExtraModuleNames.AddRange(new string[] { "scape" });
    }
}
