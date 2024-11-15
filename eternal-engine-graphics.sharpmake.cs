﻿using Sharpmake;

[module: Sharpmake.Include(@"..\eternal-engine\eternal-engine-project.sharpmake.cs")]

namespace EternalEngine
{
	[Sharpmake.Generate]
	public class EternalEngineGraphicsProject : EternalEngineBaseProject
	{
		static readonly string[] SpecialFilterPrefixes = new string[] {
			@"GraphicsPrivate\include",
			@"GraphicsPrivate\src",
		};

		public EternalEngineGraphicsProject()
			: base(
				"graphics",
				new EternalEngineProjectSettings(
					EternalEngineProjectSettingsFlags.EEPSF_IncludeSettingsHeader
				)
			)
		{
		}

		public override void ConfigureAll(Configuration InConfiguration, Target InTarget)
		{
			base.ConfigureAll(InConfiguration, InTarget);

			// Include paths
			InConfiguration.IncludePaths.Add(new string[] {
				@"$(SolutionDir)eternal-engine-graphics\GraphicsPrivate\include",
				@"$(SolutionDir)eternal-engine-components\include",
				@"$(SolutionDir)eternal-engine-extern\DirectX-Headers\include\directx",
				@"$(SolutionDir)eternal-engine-extern\dxc\include",
				EternalEngineSettings.VulkanPath + @"\Include",
			});

			// Defines
			InConfiguration.Defines.AddRange(new string[] {
				"ETERNAL_DEBUG_VERBOSE=(ETERNAL_DEBUG &amp;&amp; 1)",
			});

			if (ExtensionMethods.IsPC(InTarget.Platform))
			{
				InConfiguration.IncludePaths.Add(new string[] {
					@"$(SolutionDir)packages\WinPixEventRuntime." + EternalEngineSettings.WinPixEventRuntimeVersion + @"\Include",
				});

				InConfiguration.Defines.Add(new string[] {
					"ETERNAL_ENABLE_VULKAN=1",
					"VK_USE_PLATFORM_WIN32_KHR=(ETERNAL_ENABLE_VULKAN &amp;&amp; 1)",
				});

				InConfiguration.ReferencesByNuGetPackage.Add("WinPixEventRuntime", EternalEngineSettings.WinPixEventRuntimeVersion);
			}
		}

		public override bool ResolveFilterPath(string InRelativePath, out string OutFilterPath)
		{
			if (InternalResolveFilterPath(InRelativePath, SpecialFilterPrefixes, out OutFilterPath))
				return true;

			return base.ResolveFilterPath(InRelativePath, out OutFilterPath);
		}
	}
}
