#pragma once
#include "../RainbowSixSDK.hpp"

class CavEsp : public virtual RainbowSixSDK {
public:
	bool cavEsp = true;
protected:
	using RainbowSixSDK::RainbowSixSDK;
	bool CavEspUpdate() {
		uintptr_t GameManager = Read<uintptr_t>(rainbowSix_exe + BaseOffsets::GAME_MANAGER); //game man 

		if (!GameManager) return false;

		uintptr_t EntityList = Read<uintptr_t>(GameManager + 0xC8); //e_list 

		if (!EntityList) return false;

		int EntityCount = Read<DWORD>(GameManager + 0xD0) & 0x3fffffff; //e_count 

		if (!EntityCount) return false;

		for (int i = 0; i < EntityCount; i++)
		{
			uintptr_t EntityObject = Read<uintptr_t>(EntityList + i * 0x8ull);

			if (!EntityObject) return false;

			uintptr_t PtrChain1 = Read<uintptr_t>(EntityObject + 0x28); if (!PtrChain1) continue;
			uintptr_t PtrChain2 = Read<uintptr_t>(PtrChain1 + 0xD8); if (!PtrChain2) continue;

			for (auto Component = 0x80; Component < 0xf0; Component += sizeof(uintptr_t))
			{
				uintptr_t PtrChain3 = Read<uintptr_t>(PtrChain2 + Component);

				if (!PtrChain3) continue;

				try {
					if (Read<uintptr_t>(PtrChain3) != (rainbowSix_exe + BaseOffsets::VTMARKER)) //vtable 
						continue;
				} catch (const char*) { continue; }

				Write<BYTE>(PtrChain3 + 0x552, cavEsp);
				Write<BYTE>(PtrChain3 + 0x554, cavEsp);
			}
		}
		return true;
	}
};
