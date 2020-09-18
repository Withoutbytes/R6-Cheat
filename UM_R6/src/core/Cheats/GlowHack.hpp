#pragma once
#include "../RainbowSixSDK.hpp"

#pragma optimize( "", off )
struct GlowStructColors
{
	float red, blue, green, alpha;

	GlowStructColors operator *(float val) {
		return { red * val, blue * val, green * val, alpha * val };
	}
};

struct GlowStructOptions
{
	float distance, startDistance, alpha, opacity;
};

#pragma optimize( "", on )

class GlowHack : public virtual RainbowSixSDK {
public:
	bool glowHack = true;
	GlowStructColors colors = { 255, 0, 0 };
	float colorIntencity = 1.f;
	GlowStructOptions glowOptions{ 1, 1, 4, 1 };
protected:
	using RainbowSixSDK::RainbowSixSDK;
	bool UpdateGlowHack() {
		uintptr_t glowBase = Read<uintptr_t>(rainbowSix_exe + BaseOffsets::GLOW_MANAGER);
		if (!glowBase) return false;
		uintptr_t glowBase1 = Read<uintptr_t>(glowBase + 0xB8);
		if (!glowBase1) return false;
			
		Write<GlowStructColors>(glowBase1 + 0xD0, colors * colorIntencity);
		Write<GlowStructOptions>(glowBase1 + 0x110, glowOptions);
		return true;
	}
};

