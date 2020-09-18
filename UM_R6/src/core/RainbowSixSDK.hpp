#pragma once
#include "../managers/MemoryManager.hpp"
#include "R6Offsets.hpp"


class RainbowSixSDK: public virtual MemoryManager {
protected:
	uintptr_t rainbowSix_exe = 0;
public:

	enum class GAMTE_STATE : uint8_t
	{
		LOBBY = 0,
		PREP_PHASE = 2,
		ACTION_PHASE = 3,
	};

	using MemoryManager::MemoryManager;

	bool UpdateBases() {
		rainbowSix_exe = std::get<0>(GetModule(L"RainbowSix.exe"));

		if (!rainbowSix_exe) return false;

		return true;
	}

	GAMTE_STATE GetGameState() {
		uintptr_t round_manager = Read<uintptr_t>(rainbowSix_exe + BaseOffsets::ROUND_MANAGER);
		if (!round_manager) throw "Get Game state error";
		return Read<GAMTE_STATE>(round_manager + 0x2E8);
	}

	int GetHealth() {
		return Read<int>({ rainbowSix_exe + BaseOffsets::GAME_MANAGER, 0xC8, 0, 0x28, 0xA0, 0xB8, 0x8, 0x168 });
	}
};
