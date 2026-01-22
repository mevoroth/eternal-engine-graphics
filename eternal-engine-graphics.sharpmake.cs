using Sharpmake;

[module: Sharpmake.Include(@"..\eternal-engine\eternal-engine-project.sharpmake.cs")]

namespace EternalEngine
{
	public class EternalEngineGraphicsProjectUtils
	{
		public static readonly string[] SpecialFilterPrefixes = new string[] {
			@"GraphicsPrivate\include",
			@"GraphicsPrivate\src",
		};

		public static void ConfigureAll(Project.Configuration InConfiguration, ITarget InTarget)
		{
			// Include paths
			InConfiguration.IncludePaths.Add(new string[] {
				@"$(SolutionDir)eternal-engine-graphics\GraphicsPrivate\include",
				@"$(SolutionDir)eternal-engine-components\include",
				@"$(SolutionDir)eternal-engine-extern\DirectX-Headers\include\directx",
				@"$(SolutionDir)eternal-engine-extern\dxc\include",
			});

			// Defines
			InConfiguration.Defines.AddRange(new string[] {
				"ETERNAL_DEBUG_VERBOSE=(ETERNAL_DEBUG &amp;&amp; 1)",
			});

			InConfiguration.Defines.Add(new string[] {
				"ETERNAL_ENABLE_VULKAN=" + (ExtensionMethods.IsPC(InTarget.GetFragment<Platform>()) || EternalEngineBaseProjectUtils.IsAndroid(InTarget) ? "1" : "0"),
			});

			if (ExtensionMethods.IsPC(InTarget.GetFragment<Platform>()))
			{
				InConfiguration.IncludePaths.Add(new string[] {
					@"$(SolutionDir)packages\WinPixEventRuntime." + EternalEngineSettings.WinPixEventRuntimeVersion + @"\Include",
				});

				InConfiguration.Defines.Add(new string[] {
					"VK_USE_PLATFORM_WIN32_KHR=(ETERNAL_ENABLE_VULKAN &amp;&amp; 1)",
				});

				InConfiguration.ReferencesByNuGetPackage.Add("WinPixEventRuntime", EternalEngineSettings.WinPixEventRuntimeVersion);
			}

			if (EternalEngineBaseProjectUtils.IsAndroid(InTarget))
			{
				InConfiguration.Defines.Add(new string[] {
				"VK_USE_PLATFORM_ANDROID_KHR=(ETERNAL_ENABLE_VULKAN &amp;&amp; 1)",
				});

			}
		}
	}

	[Sharpmake.Generate]
	public class EternalEngineGraphicsProject : EternalEngineBaseProject
	{
		public EternalEngineGraphicsProject()
			: base(
				typeof(Target),
				"graphics",
				new EternalEngineProjectSettings(
					EternalEngineProjectSettingsFlags.EEPSF_IncludeSettingsHeader
				)
			)
		{
		}

		public override void ConfigureAll(Configuration InConfiguration, ITarget InTarget)
		{
			base.ConfigureAll(InConfiguration, InTarget);
			EternalEngineGraphicsProjectUtils.ConfigureAll(InConfiguration, InTarget);
		}

		public override bool ResolveFilterPath(string InRelativePath, out string OutFilterPath)
		{
			if (EternalEngineBaseProjectUtils.InternalResolveFilterPath(InRelativePath, EternalEngineGraphicsProjectUtils.SpecialFilterPrefixes, out OutFilterPath))
				return true;

			return base.ResolveFilterPath(InRelativePath, out OutFilterPath);
		}
	}

	[Sharpmake.Generate]
	public class EternalEngineGraphicsAndroidProject : EternalEngineBaseAndroidProject
	{
		public EternalEngineGraphicsAndroidProject()
			: base(
				typeof(AndroidTarget),
				"graphics",
				new EternalEngineProjectSettings(
					EternalEngineProjectSettingsFlags.EEPSF_IncludeSettingsHeader
				)
			)
		{
		}

		public override void ConfigureAll(Configuration InConfiguration, ITarget InTarget)
		{
			base.ConfigureAll(InConfiguration, InTarget);
			EternalEngineGraphicsProjectUtils.ConfigureAll(InConfiguration, InTarget);
		}

		public override bool ResolveFilterPath(string InRelativePath, out string OutFilterPath)
		{
			if (EternalEngineBaseProjectUtils.InternalResolveFilterPath(InRelativePath, EternalEngineGraphicsProjectUtils.SpecialFilterPrefixes, out OutFilterPath))
				return true;

			return base.ResolveFilterPath(InRelativePath, out OutFilterPath);
		}
	}
}
