#include "d3d12/D3D12Device.hpp"

#include "Macros/Macros.hpp"

#include <d3d12.h>
#include <DXGI1_4.h>

#include "d3d12/D3D12CommandQueue.hpp"
#include "d3d12/D3D12RenderTarget.hpp"
#include "d3d12/D3D12DescriptorHeap.hpp"
#include "Window/Window.hpp"

using namespace Eternal::Graphics;

#ifdef ETERNAL_DEBUG
#define ETERNAL_D3D12_DXGIFLAG	(0x1)
#else
#define ETERNAL_D3D12_DXGIFLAG	(0x0)
#endif

ID3D12Debug* D3D12Device::_Debug = nullptr;
IDXGIFactory4* D3D12Device::_DXGIFactory = nullptr;

void D3D12Device::Initialize()
{
	HRESULT hr = CreateDXGIFactory2(ETERNAL_D3D12_DXGIFLAG, __uuidof(IDXGIFactory4), (void**)&_DXGIFactory);
	ETERNAL_ASSERT(hr == S_OK);

#ifdef ETERNAL_DEBUG
	// Enable the D3D12 debug layer
	hr = D3D12GetDebugInterface(__uuidof(ID3D12Debug), (void**)&_Debug);
	if (hr == S_OK)
	{
		_Debug->EnableDebugLayer();
	}
#endif
}

IDXGIFactory4* D3D12Device::GetDXGIFactory()
{
	return _DXGIFactory;
}

D3D12Device::D3D12Device(_In_ uint32_t DeviceIndex)
{
	ETERNAL_ASSERT(_DXGIFactory);
	HRESULT hr = _DXGIFactory->EnumAdapters1(DeviceIndex, &_DXGIAdapter);
	
	if (hr == DXGI_ERROR_NOT_FOUND)
	{
		// No GPU at this index
		return;
	}

	hr = D3D12CreateDevice(_DXGIAdapter, D3D_FEATURE_LEVEL_12_0, __uuidof(ID3D12Device), (void**)&_Device);
	ETERNAL_ASSERT(hr == S_OK);
	ETERNAL_ASSERT(_Device);

#ifdef ETERNAL_DEBUG
	DXGI_ADAPTER_DESC1 DXGIAdapterDesc1;
	hr = _DXGIAdapter->GetDesc1(&DXGIAdapterDesc1);
	ETERNAL_ASSERT(hr == S_OK); // Break here for debug info on device
#endif

	D3D12_FEATURE_DATA_D3D12_OPTIONS FeatureD3D12Options;
	hr = _Device->CheckFeatureSupport(D3D12_FEATURE_D3D12_OPTIONS, &FeatureD3D12Options, sizeof(D3D12_FEATURE_DATA_D3D12_OPTIONS));
	ETERNAL_ASSERT(hr == S_OK);

	D3D12_FEATURE_DATA_ARCHITECTURE FeatureArchitecture;
	FeatureArchitecture.NodeIndex = DeviceIndex;
	hr = _Device->CheckFeatureSupport(D3D12_FEATURE_ARCHITECTURE, &FeatureArchitecture, sizeof(D3D12_FEATURE_DATA_ARCHITECTURE));
	ETERNAL_ASSERT(hr == S_OK);

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
	hr = _Device->CheckFeatureSupport(D3D12_FEATURE_FEATURE_LEVELS, &FeatureFeatureLevels, sizeof(D3D12_FEATURE_DATA_FEATURE_LEVELS));
	ETERNAL_ASSERT(hr == S_OK);

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
		hr = _Device->CheckFeatureSupport(D3D12_FEATURE_FORMAT_SUPPORT, &FeatureFormatSupports[FormatType], sizeof(D3D12_FEATURE_DATA_FORMAT_SUPPORT));
		ETERNAL_ASSERT(hr == S_OK || hr == E_FAIL);
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
				hr = _Device->CheckFeatureSupport(D3D12_FEATURE_MULTISAMPLE_QUALITY_LEVELS, &FeatureMultisampleQualityLevels[Key], sizeof(D3D12_FEATURE_DATA_MULTISAMPLE_QUALITY_LEVELS));
				FeatureEnabledMultisampleQualityLevels[Key] = (hr == S_OK);
				ETERNAL_ASSERT(hr == S_OK || hr == E_FAIL);
			}
		}
	}


	D3D12_FEATURE_DATA_FORMAT_INFO FeatureFormatInfos[FormatsCount];
	for (uint32_t FormatType = 0; FormatType < FormatsCount; ++FormatType)
	{
		FeatureFormatInfos[FormatType].Format	= (DXGI_FORMAT)Formats[FormatType];
		if (!(FeatureFormatSupports[FormatType].Support1 == D3D12_FORMAT_SUPPORT1_NONE && FeatureFormatSupports[FormatType].Support2 == D3D12_FORMAT_SUPPORT2_NONE))
		{
			hr = _Device->CheckFeatureSupport(D3D12_FEATURE_FORMAT_INFO, &FeatureFormatInfos[FormatType], sizeof(D3D12_FEATURE_DATA_FORMAT_INFO));
			ETERNAL_ASSERT(hr == S_OK);
		}
		else
		{
			FeatureFormatInfos[FormatType].PlaneCount = 0;
		}
	}

	D3D12_FEATURE_DATA_GPU_VIRTUAL_ADDRESS_SUPPORT FeatureGpuVirtualAddressSupport;
	hr = _Device->CheckFeatureSupport(D3D12_FEATURE_GPU_VIRTUAL_ADDRESS_SUPPORT, &FeatureGpuVirtualAddressSupport, sizeof(D3D12_FEATURE_DATA_GPU_VIRTUAL_ADDRESS_SUPPORT));
	ETERNAL_ASSERT(hr == S_OK);

	_DeviceMask = 1 << DeviceIndex;
}

uint32_t D3D12Device::GetDeviceMask() const
{
	ETERNAL_ASSERT(_DeviceMask != 0xFFFFFFFF);
	return _DeviceMask;
}
