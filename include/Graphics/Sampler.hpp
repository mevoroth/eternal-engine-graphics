#ifndef _SAMPLER_HPP_
#define _SAMPLER_HPP_

namespace Eternal
{
	namespace Graphics
	{
		enum AddressMode
		{
			ADDRESS_MODE_WRAP	= 1,
			ADDRESS_MODE_MIRROR	= 2,
			ADDRESS_MODE_CLAMP	= 3,
			ADDRESS_MODE_COUNT
		};

		class Sampler
		{
		public:
			Sampler() {}
			Sampler(_In_ bool MINLinear, _In_ bool MAGLinear, _In_ bool MIPLinear, _In_ const AddressMode& U, _In_ const AddressMode& V, _In_ const AddressMode& W/*, _In_ uint32_t Register*/);

			virtual ~Sampler() {}

		protected:
			bool GetMINLinear() const { return _MINLinear; }
			bool GetMAGLinear() const { return _MAGLinear; }
			bool GetMIPLinear() const { return _MIPLinear; }
			const AddressMode& GetU() const { return _U; }
			const AddressMode& GetV() const { return _V; }
			const AddressMode& GetW() const { return _W; }
			//uint32_t GetRegister() const { return _Register; }

		private:
			//uint32_t	_Register	= 0xFFFFFFFF;
			AddressMode _U			= ADDRESS_MODE_WRAP;
			AddressMode _V			= ADDRESS_MODE_WRAP;
			AddressMode _W			= ADDRESS_MODE_WRAP;
			bool		_MINLinear	= true;
			bool		_MAGLinear	= true;
			bool		_MIPLinear	= false;
		};
	}
}

#endif
