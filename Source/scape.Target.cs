// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class scapeTarget : TargetRules
{
	public scapeTarget( TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;
		DefaultBuildSettings = BuildSettingsVersion.V2;
		bForcePrecompiledHeaderForGameModules = true;

		ExtraModuleNames.AddRange( new string[] { "scape" } );
	}
}
