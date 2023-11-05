#if ETERNAL_ENABLE_D3D12

#include "d3d12/D3D12Device.hpp"

#include <d3d12.h>
#include <DXGI1_4.h>
#include "d3d12/D3D12Utils.hpp"

#if ETERNAL_USE_NVIDIA_AFTERMATH
#include "NVIDIANsightAftermath/GFSDK_Aftermath.h"
#endif

#if ETERNAL_USE_DEBUG_LAYER
#include "WinPixEventRuntime/pix3.h"
#include <dxgidebug.h>
#define ETERNAL_D3D12_DXGIFLAG_DEBUG	(DXGI_CREATE_FACTORY_DEBUG)
#else
#define ETERNAL_D3D12_DXGIFLAG_DEBUG	(0x0)
#endif

namespace Eternal
{
	namespace Graphics
	{
		using namespace Eternal::Graphics::D3D12;

		namespace D3D12
		{
			static constexpr bool LoadPixDLLOnStartup	= true;
			static constexpr bool UseValidationLayer	= true;
		}

#if ETERNAL_USE_DEBUG_LAYER
		ID3D12Debug3*								D3D12Device::_Debug3									= nullptr;
		IDXGIInfoQueue*								D3D12Device::_DXGIInfoQueue								= nullptr;
		IDXGIDebug*									D3D12Device::_DXGIDebug									= nullptr;
		ID3D12DeviceRemovedExtendedDataSettings*	D3D12Device::_D3D12DeviceRemovedExtendedDataSettings	= nullptr;
		ID3D12DeviceRemovedExtendedDataSettings1*	D3D12Device::_D3D12DeviceRemovedExtendedDataSettings1	= nullptr;
#endif
		bool										D3D12Device::_IsInitialized								= false;
		IDXGIFactory4*								D3D12Device::_DXGIFactory								= nullptr;

		void D3D12Device::Initialize()
		{
			if (_IsInitialized)
				return;

			_IsInitialized = true;

			HRESULT HResult = S_OK;

#if ETERNAL_USE_DEBUG_LAYER
			if (D3D12::LoadPixDLLOnStartup)
				PIXLoadLatestWinPixGpuCapturerLibrary();
#endif

			VerifySuccess(
				CreateDXGIFactory2(ETERNAL_D3D12_DXGIFLAG_DEBUG, __uuidof(IDXGIFactory4), reinterpret_cast<void**>(&_DXGIFactory))
			);

#if ETERNAL_USE_DEBUG_LAYER
			// Enable the D3D12 debug layer
			if (D3D12::UseValidationLayer)
			{
				HResult = D3D12GetDebugInterface(__uuidof(ID3D12Debug3), reinterpret_cast<void**>(&_Debug3));
				if (HResult == S_OK)
				{
					_Debug3->EnableDebugLayer();
					_Debug3->SetEnableGPUBasedValidation(TRUE);
					_Debug3->SetEnableSynchronizedCommandQueueValidation(TRUE);
					_Debug3->SetGPUBasedValidationFlags(D3D12_GPU_BASED_VALIDATION_FLAGS_NONE);
				}
			}

			HMODULE DXGIDebugLib = LoadLibraryEx("dxgidebug.dll", nullptr, LOAD_LIBRARY_SEARCH_SYSTEM32);
			
			if (DXGIDebugLib)
			{
				typedef HRESULT(WINAPI * LPDXGIGETDEBUGINTERFACE)(REFIID, void **);

				LPDXGIGETDEBUGINTERFACE DXGIGetDebugInterface;

				DXGIGetDebugInterface = reinterpret_cast<LPDXGIGETDEBUGINTERFACE>(
					reinterpret_cast<void*>(GetProcAddress(DXGIDebugLib, "DXGIGetDebugInterface")));
				ETERNAL_ASSERT(DXGIGetDebugInterface);

				VerifySuccess(
					DXGIGetDebugInterface(__uuidof(IDXGIDebug), reinterpret_cast<void**>(&_DXGIDebug))
				);

				VerifySuccess(
					HResult = _DXGIDebug->QueryInterface(__uuidof(IDXGIInfoQueue), reinterpret_cast<void**>(&_DXGIInfoQueue))
				);

				if (HResult == S_OK)
				{
					// Break on category
					DXGI_DEBUG_ID DebugID = DXGI_DEBUG_ALL;
					//_DXGIInfoQueue->SetMuteDebugOutput(DebugID, TRUE);

					for (uint32_t CategoryIndex = DXGI_INFO_QUEUE_MESSAGE_CATEGORY_UNKNOWN; CategoryIndex < DXGI_INFO_QUEUE_MESSAGE_CATEGORY_SHADER; ++CategoryIndex)
					{
						HResult = _DXGIInfoQueue->SetBreakOnCategory(DebugID, (DXGI_INFO_QUEUE_MESSAGE_CATEGORY)CategoryIndex, TRUE);
						ETERNAL_ASSERT(HResult == S_OK);
					}

					for (uint32_t SeverityIndex = DXGI_INFO_QUEUE_MESSAGE_SEVERITY_CORRUPTION; SeverityIndex <= DXGI_INFO_QUEUE_MESSAGE_SEVERITY_WARNING; ++SeverityIndex)
					{
						HResult = _DXGIInfoQueue->SetBreakOnSeverity(DebugID, (DXGI_INFO_QUEUE_MESSAGE_SEVERITY)SeverityIndex, TRUE);
						ETERNAL_ASSERT(HResult == S_OK);
					}

					//for (uint32_t MessageId = D3D12_MESSAGE_ID_UNKNOWN; MessageId <= D3D12_MESSAGE_ID_D3D12_MESSAGES_END; ++MessageId)
					//{
					//	hr = _DXGIInfoQueue->SetBreakOnID(DebugID, (DXGI_INFO_QUEUE_MESSAGE_ID)MessageId, TRUE);
					//	ETERNAL_ASSERT(hr == S_OK);
					//}
				}
			}

			if (UseDRED)
			{
				HResult = D3D12GetDebugInterface(__uuidof(ID3D12DeviceRemovedExtendedDataSettings), reinterpret_cast<void**>(&_D3D12DeviceRemovedExtendedDataSettings));
				if (HResult == S_OK)
					HResult = _D3D12DeviceRemovedExtendedDataSettings->QueryInterface(__uuidof(ID3D12DeviceRemovedExtendedDataSettings1), reinterpret_cast<void**>(&_D3D12DeviceRemovedExtendedDataSettings1));

				if (_D3D12DeviceRemovedExtendedDataSettings1)
				{
					_D3D12DeviceRemovedExtendedDataSettings1->SetAutoBreadcrumbsEnablement(D3D12_DRED_ENABLEMENT_FORCED_ON);
					_D3D12DeviceRemovedExtendedDataSettings1->SetBreadcrumbContextEnablement(D3D12_DRED_ENABLEMENT_FORCED_ON);
					_D3D12DeviceRemovedExtendedDataSettings1->SetPageFaultEnablement(D3D12_DRED_ENABLEMENT_FORCED_ON);
					_D3D12DeviceRemovedExtendedDataSettings1->SetWatsonDumpEnablement(D3D12_DRED_ENABLEMENT_FORCED_ON);
				}
			}
#endif
		}

		void D3D12Device::Destroy()
		{
			if (!_IsInitialized)
				return;

#if ETERNAL_USE_DEBUG_LAYER
			VerifySuccess(
				_DXGIDebug->ReportLiveObjects(DXGI_DEBUG_ALL, DXGI_DEBUG_RLO_ALL)
			);

			if (_D3D12DeviceRemovedExtendedDataSettings1)
			{
				_D3D12DeviceRemovedExtendedDataSettings1->Release();
				_D3D12DeviceRemovedExtendedDataSettings1 = nullptr;
			}

			if (_D3D12DeviceRemovedExtendedDataSettings)
			{
				_D3D12DeviceRemovedExtendedDataSettings->Release();
				_D3D12DeviceRemovedExtendedDataSettings = nullptr;
			}

			_DXGIInfoQueue->Release();
			_DXGIInfoQueue = nullptr;

			_DXGIDebug->Release();
			_DXGIDebug = nullptr;

			if (_Debug3)
			{
				_Debug3->Release();
				_Debug3 = nullptr;
			}
#endif

			_DXGIFactory->Release();
			_DXGIFactory = nullptr;
		}

		IDXGIFactory4* D3D12Device::GetDXGIFactory()
		{
			return _DXGIFactory;
		}

		D3D12Device::D3D12Device(_In_ uint32_t DeviceIndex)
		{
#if ETERNAL_USE_NVIDIA_AFTERMATH
			_NVIDIANsightAftermath.InitializeGpuCrashTracker();
#endif

			ETERNAL_ASSERT(_DXGIFactory);
			HRESULT HResult = _DXGIFactory->EnumAdapters1(DeviceIndex, &_DXGIAdapter);
	
			if (HResult == DXGI_ERROR_NOT_FOUND)
			{
				// No GPU at this index
				ETERNAL_BREAK();
				return;
			}

			VerifySuccess(
				D3D12CreateDevice(_DXGIAdapter, D3D_FEATURE_LEVEL_12_0, __uuidof(ID3D12Device), reinterpret_cast<void**>(&_Device))
			);
			VerifySuccess(
				_Device->QueryInterface(__uuidof(ID3D12Device5), reinterpret_cast<void**>(&_Device5))
			);
			//_Device = _Device5;
			ETERNAL_ASSERT(_Device);

#if ETERNAL_USE_NVIDIA_AFTERMATH
			_NVIDIANsightAftermath.InitializeAftermath(_Device);
#endif

#if ETERNAL_USE_DEBUG_LAYER
			DXGI_ADAPTER_DESC1 DXGIAdapterDesc1;
			VerifySuccess(
				_DXGIAdapter->GetDesc1(&DXGIAdapterDesc1)
			); // Break here for debug info on device

			/*VerifySuccess*/(
				HResult = _Device5->QueryInterface(__uuidof(ID3D12InfoQueue), reinterpret_cast<void**>(&_D3D12InfoQueue))
			);
			if (HResult == S_OK)
			{
				/*VerifySuccess*/(
					HResult = _D3D12InfoQueue->QueryInterface(__uuidof(ID3D12InfoQueue1), reinterpret_cast<void**>(&_D3D12InfoQueue1))
				);

				if (HResult == S_OK)
				{
					//DWORD CallBackCookies = 0x0;
					//VerifySuccess(
					//	_D3D12InfoQueue1->RegisterMessageCallback(
					//		D3D12MessageFunctor,
					//		D3D12_MESSAGE_CALLBACK_FLAG_NONE,
					//		this,
					//		&CallBackCookies
					//	)
					//);
				}
			}

			//VerifySuccess(
			//	_Device->QueryInterface(
			//		__uuidof(ID3D12DeviceRemovedExtendedData),
			//		reinterpret_cast<void**>(&_D3D12DeviceRemovedExtendedData)
			//	)
			//);

			//ID3D12DeviceRemovedExtendedData1* _D3D12DeviceRemovedExtendedData1 = nullptr;
			//VerifySuccess(
			//	_D3D12DeviceRemovedExtendedData->QueryInterface(__uuidof(ID3D12DeviceRemovedExtendedData1), reinterpret_cast<void**>(&_D3D12DeviceRemovedExtendedData1))
			//);

			//D3D12_DRED_AUTO_BREADCRUMBS_OUTPUT1 BreadCrumb1 = {};
			//VerifySuccess(_D3D12DeviceRemovedExtendedData1->GetAutoBreadcrumbsOutput1(&BreadCrumb1));

			//D3D12_DRED_AUTO_BREADCRUMBS_OUTPUT BreadCrumb = {};
			//VerifySuccess(_D3D12DeviceRemovedExtendedData1->GetAutoBreadcrumbsOutput(&BreadCrumb));
#endif

			D3D12_FEATURE_DATA_D3D12_OPTIONS FeatureD3D12Options = {};
			VerifySuccess(
				_Device->CheckFeatureSupport(D3D12_FEATURE_D3D12_OPTIONS, &FeatureD3D12Options, sizeof(D3D12_FEATURE_DATA_D3D12_OPTIONS))
			);

			D3D12_FEATURE_DATA_ARCHITECTURE FeatureArchitecture;
			FeatureArchitecture.NodeIndex = DeviceIndex;
			VerifySuccess(
				_Device->CheckFeatureSupport(D3D12_FEATURE_ARCHITECTURE, &FeatureArchitecture, sizeof(D3D12_FEATURE_DATA_ARCHITECTURE))
			);

			D3D_FEATURE_LEVEL FeatureLevels[] =
			{
				D3D_FEATURE_LEVEL_9_1,
				D3D_FEATURE_LEVEL_9_2,
				D3D_FEATURE_LEVEL_9_3,
				D3D_FEATURE_LEVEL_10_0,
				D3D_FEATURE_LEVEL_10_1,
				D3D_FEATURE_LEVEL_11_0,
				D3D_FEATURE_LEVEL_11_1,
				D3D_FEATURE_LEVEL_12_0,
				D3D_FEATURE_LEVEL_12_1
			};
			D3D12_FEATURE_DATA_FEATURE_LEVELS FeatureFeatureLevels;
			FeatureFeatureLevels.NumFeatureLevels			= ETERNAL_ARRAYSIZE(FeatureLevels);
			FeatureFeatureLevels.pFeatureLevelsRequested	= FeatureLevels;
			VerifySuccess(
				_Device->CheckFeatureSupport(D3D12_FEATURE_FEATURE_LEVELS, &FeatureFeatureLevels, sizeof(D3D12_FEATURE_DATA_FEATURE_LEVELS))
			);

			DXGI_FORMAT Formats[]
			{
				DXGI_FORMAT_UNKNOWN,
				DXGI_FORMAT_R32G32B32A32_TYPELESS,
				DXGI_FORMAT_R32G32B32A32_FLOAT,
				DXGI_FORMAT_R32G32B32A32_UINT,
				DXGI_FORMAT_R32G32B32A32_SINT,
				DXGI_FORMAT_R32G32B32_TYPELESS,
				DXGI_FORMAT_R32G32B32_FLOAT,
				DXGI_FORMAT_R32G32B32_UINT,
				DXGI_FORMAT_R32G32B32_SINT,
				DXGI_FORMAT_R16G16B16A16_TYPELESS,
				DXGI_FORMAT_R16G16B16A16_FLOAT,
				DXGI_FORMAT_R16G16B16A16_UNORM,
				DXGI_FORMAT_R16G16B16A16_UINT,
				DXGI_FORMAT_R16G16B16A16_SNORM,
				DXGI_FORMAT_R16G16B16A16_SINT,
				DXGI_FORMAT_R32G32_TYPELESS,
				DXGI_FORMAT_R32G32_FLOAT,
				DXGI_FORMAT_R32G32_UINT,
				DXGI_FORMAT_R32G32_SINT,
				DXGI_FORMAT_R32G8X24_TYPELESS,
				DXGI_FORMAT_D32_FLOAT_S8X24_UINT,
				DXGI_FORMAT_R32_FLOAT_X8X24_TYPELESS,
				DXGI_FORMAT_X32_TYPELESS_G8X24_UINT,
				DXGI_FORMAT_R10G10B10A2_TYPELESS,
				DXGI_FORMAT_R10G10B10A2_UNORM,
				DXGI_FORMAT_R10G10B10A2_UINT,
				DXGI_FORMAT_R11G11B10_FLOAT,
				DXGI_FORMAT_R8G8B8A8_TYPELESS,
				DXGI_FORMAT_R8G8B8A8_UNORM,
				DXGI_FORMAT_R8G8B8A8_UNORM_SRGB,
				DXGI_FORMAT_R8G8B8A8_UINT,
				DXGI_FORMAT_R8G8B8A8_SNORM,
				DXGI_FORMAT_R8G8B8A8_SINT,
				DXGI_FORMAT_R16G16_TYPELESS,
				DXGI_FORMAT_R16G16_FLOAT,
				DXGI_FORMAT_R16G16_UNORM,
				DXGI_FORMAT_R16G16_UINT,
				DXGI_FORMAT_R16G16_SNORM,
				DXGI_FORMAT_R16G16_SINT,
				DXGI_FORMAT_R32_TYPELESS,
				DXGI_FORMAT_D32_FLOAT,
				DXGI_FORMAT_R32_FLOAT,
				DXGI_FORMAT_R32_UINT,
				DXGI_FORMAT_R32_SINT,
				DXGI_FORMAT_R24G8_TYPELESS,
				DXGI_FORMAT_D24_UNORM_S8_UINT,
				DXGI_FORMAT_R24_UNORM_X8_TYPELESS,
				DXGI_FORMAT_X24_TYPELESS_G8_UINT,
				DXGI_FORMAT_R8G8_TYPELESS,
				DXGI_FORMAT_R8G8_UNORM,
				DXGI_FORMAT_R8G8_UINT,
				DXGI_FORMAT_R8G8_SNORM,
				DXGI_FORMAT_R8G8_SINT,
				DXGI_FORMAT_R16_TYPELESS,
				DXGI_FORMAT_R16_FLOAT,
				DXGI_FORMAT_D16_UNORM,
				DXGI_FORMAT_R16_UNORM,
				DXGI_FORMAT_R16_UINT,
				DXGI_FORMAT_R16_SNORM,
				DXGI_FORMAT_R16_SINT,
				DXGI_FORMAT_R8_TYPELESS,
				DXGI_FORMAT_R8_UNORM,
				DXGI_FORMAT_R8_UINT,
				DXGI_FORMAT_R8_SNORM,
				DXGI_FORMAT_R8_SINT,
				DXGI_FORMAT_A8_UNORM,
				DXGI_FORMAT_R1_UNORM,
				DXGI_FORMAT_R9G9B9E5_SHAREDEXP,
				DXGI_FORMAT_R8G8_B8G8_UNORM,
				DXGI_FORMAT_G8R8_G8B8_UNORM,
				DXGI_FORMAT_BC1_TYPELESS,
				DXGI_FORMAT_BC1_UNORM,
				DXGI_FORMAT_BC1_UNORM_SRGB,
				DXGI_FORMAT_BC2_TYPELESS,
				DXGI_FORMAT_BC2_UNORM,
				DXGI_FORMAT_BC2_UNORM_SRGB,
				DXGI_FORMAT_BC3_TYPELESS,
				DXGI_FORMAT_BC3_UNORM,
				DXGI_FORMAT_BC3_UNORM_SRGB,
				DXGI_FORMAT_BC4_TYPELESS,
				DXGI_FORMAT_BC4_UNORM,
				DXGI_FORMAT_BC4_SNORM,
				DXGI_FORMAT_BC5_TYPELESS,
				DXGI_FORMAT_BC5_UNORM,
				DXGI_FORMAT_BC5_SNORM,
				DXGI_FORMAT_B5G6R5_UNORM,
				DXGI_FORMAT_B5G5R5A1_UNORM,
				DXGI_FORMAT_B8G8R8A8_UNORM,
				DXGI_FORMAT_B8G8R8X8_UNORM,
				DXGI_FORMAT_R10G10B10_XR_BIAS_A2_UNORM,
				DXGI_FORMAT_B8G8R8A8_TYPELESS,
				DXGI_FORMAT_B8G8R8A8_UNORM_SRGB,
				DXGI_FORMAT_B8G8R8X8_TYPELESS,
				DXGI_FORMAT_B8G8R8X8_UNORM_SRGB,
				DXGI_FORMAT_BC6H_TYPELESS,
				DXGI_FORMAT_BC6H_UF16,
				DXGI_FORMAT_BC6H_SF16,
				DXGI_FORMAT_BC7_TYPELESS,
				DXGI_FORMAT_BC7_UNORM,
				DXGI_FORMAT_BC7_UNORM_SRGB,
				DXGI_FORMAT_AYUV,
				DXGI_FORMAT_Y410,
				DXGI_FORMAT_Y416,
				DXGI_FORMAT_NV12,
				DXGI_FORMAT_P010,
				DXGI_FORMAT_P016,
				DXGI_FORMAT_420_OPAQUE,
				DXGI_FORMAT_YUY2,
				DXGI_FORMAT_Y210,
				DXGI_FORMAT_Y216,
				DXGI_FORMAT_NV11,
				DXGI_FORMAT_AI44,
				DXGI_FORMAT_IA44,
				DXGI_FORMAT_P8,
				DXGI_FORMAT_A8P8,
				DXGI_FORMAT_B4G4R4A4_UNORM,
				DXGI_FORMAT_P208,
				DXGI_FORMAT_V208,
				DXGI_FORMAT_V408,
				DXGI_FORMAT_FORCE_UINT
			};

			const uint32_t FormatsCount = ETERNAL_ARRAYSIZE(Formats);

			D3D12_FEATURE_DATA_FORMAT_SUPPORT FeatureFormatSupports[FormatsCount];
			for (uint32_t FormatType = 0; FormatType < FormatsCount; ++FormatType)
			{
				FeatureFormatSupports[FormatType].Format = (DXGI_FORMAT)Formats[FormatType];
				HResult = _Device->CheckFeatureSupport(D3D12_FEATURE_FORMAT_SUPPORT, &FeatureFormatSupports[FormatType], sizeof(D3D12_FEATURE_DATA_FORMAT_SUPPORT));
				ETERNAL_ASSERT(HResult == S_OK || HResult == E_FAIL);
			}

			const uint32_t SampleCountMaxLog2			= 7;
			const uint32_t MultisampleQualityFlagsCount	= 2;

			D3D12_FEATURE_DATA_MULTISAMPLE_QUALITY_LEVELS FeatureMultisampleQualityLevels[FormatsCount * SampleCountMaxLog2 * MultisampleQualityFlagsCount];
			bool FeatureEnabledMultisampleQualityLevels[FormatsCount * SampleCountMaxLog2 * MultisampleQualityFlagsCount];

			for (uint32_t FormatType = 0; FormatType < FormatsCount; ++FormatType)
			{
				for (uint32_t MultisampleQualityFlag = 0; MultisampleQualityFlag < MultisampleQualityFlagsCount; ++MultisampleQualityFlag)
				{
					for (uint32_t SampleCountLog2 = 0; SampleCountLog2 < SampleCountMaxLog2; ++SampleCountLog2)
					{
						uint32_t Key = ((FormatType * MultisampleQualityFlagsCount) + MultisampleQualityFlag) * SampleCountMaxLog2 + SampleCountLog2;
						FeatureMultisampleQualityLevels[Key].Format			= (DXGI_FORMAT)Formats[FormatType];
						FeatureMultisampleQualityLevels[Key].Flags			= (D3D12_MULTISAMPLE_QUALITY_LEVEL_FLAGS)MultisampleQualityFlag;
						FeatureMultisampleQualityLevels[Key].SampleCount	= 1 << SampleCountLog2;
						HResult = _Device->CheckFeatureSupport(D3D12_FEATURE_MULTISAMPLE_QUALITY_LEVELS, &FeatureMultisampleQualityLevels[Key], sizeof(D3D12_FEATURE_DATA_MULTISAMPLE_QUALITY_LEVELS));
						FeatureEnabledMultisampleQualityLevels[Key] = (HResult == S_OK);
						ETERNAL_ASSERT(HResult == S_OK || HResult == E_FAIL);
					}
				}
			}


			D3D12_FEATURE_DATA_FORMAT_INFO FeatureFormatInfos[FormatsCount];
			for (uint32_t FormatType = 0; FormatType < FormatsCount; ++FormatType)
			{
				FeatureFormatInfos[FormatType].Format	= (DXGI_FORMAT)Formats[FormatType];
				if (!(FeatureFormatSupports[FormatType].Support1 == D3D12_FORMAT_SUPPORT1_NONE && FeatureFormatSupports[FormatType].Support2 == D3D12_FORMAT_SUPPORT2_NONE))
				{
					VerifySuccess(
						_Device->CheckFeatureSupport(D3D12_FEATURE_FORMAT_INFO, &FeatureFormatInfos[FormatType], sizeof(D3D12_FEATURE_DATA_FORMAT_INFO))
					);
				}
				else
				{
					FeatureFormatInfos[FormatType].PlaneCount = 0;
				}
			}

			D3D12_FEATURE_DATA_GPU_VIRTUAL_ADDRESS_SUPPORT FeatureGpuVirtualAddressSupport;
			VerifySuccess(
				_Device->CheckFeatureSupport(D3D12_FEATURE_GPU_VIRTUAL_ADDRESS_SUPPORT, &FeatureGpuVirtualAddressSupport, sizeof(D3D12_FEATURE_DATA_GPU_VIRTUAL_ADDRESS_SUPPORT))
			);

			_DeviceMask = 1 << DeviceIndex;

			D3D12_HEAP_PROPERTIES DefaultHeapProperties		= _Device->GetCustomHeapProperties(_DeviceMask, D3D12_HEAP_TYPE_DEFAULT);
			D3D12_HEAP_PROPERTIES UploadHeapProperties		= _Device->GetCustomHeapProperties(_DeviceMask, D3D12_HEAP_TYPE_UPLOAD);
			D3D12_HEAP_PROPERTIES ReadbackHeapProperties	= _Device->GetCustomHeapProperties(_DeviceMask, D3D12_HEAP_TYPE_READBACK);
		}

		D3D12Device::~D3D12Device()
		{
#if ETERNAL_USE_DEBUG_LAYER
			if (_D3D12InfoQueue1)
			{
				_D3D12InfoQueue1->Release();
				_D3D12InfoQueue1 = nullptr;
			}

			if (_D3D12InfoQueue)
			{
				_D3D12InfoQueue->Release();
				_D3D12InfoQueue = nullptr;
			}
#endif

			_Device5->Release();
			_Device5 = nullptr;

			_Device->Release();
			_Device = nullptr;

			_DXGIAdapter->Release();
			_DXGIAdapter = nullptr;
		}

		uint32_t D3D12Device::GetDeviceMask() const
		{
			ETERNAL_ASSERT(_DeviceMask != 0xFFFFFFFF);
			return _DeviceMask;
		}
	}
}

#endif
