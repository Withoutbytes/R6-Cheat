#pragma once
#include <Windows.h>
#include <iostream>
#include "../ProcessHelper.hpp"
#include "../managers/MemoryManager.hpp"
#include "RainbowSixSDK.hpp"
#include "R6Cheats.hpp"
#include "Cheats/GlowHack.hpp"


class RainbowSix : public virtual MemoryManager, public virtual RainbowSixSDK, public virtual R6Cheats {
public:
	using R6Cheats::R6Cheats;
	
	bool Loop() {
		if (!IsWindowValid()) return false;
		auto gameState = GetGameState();
		if (gameState != GAMTE_STATE::ACTION_PHASE && gameState != GAMTE_STATE::PREP_PHASE) return true;
		CavEspUpdate();
		UpdateGlowHack();

		try
		{
			auto health = GetHealth();

			if (health > 0) UpdateMisc();
		} catch (const char*) { return true; }

		return true;
	}
};