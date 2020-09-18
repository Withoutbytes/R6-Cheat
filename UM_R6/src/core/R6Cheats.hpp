#pragma once
#include "Cheats/CavEsp.hpp"
#include "Cheats/GlowHack.hpp"
#include "Cheats/MiscHacks.hpp"


class R6Cheats: public CavEsp, public MiscHack, public GlowHack {
	using CavEsp::CavEsp;
	using MiscHack::MiscHack;
	using GlowHack::GlowHack;
};
