#pragma once
#include "../RainbowSixSDK.hpp"

#pragma optimize( "", off )

#pragma optimize( "", on )

class MiscHack : public virtual RainbowSixSDK {
public:
	bool noFlash = false;
	bool noClip = false;
	bool canChangeRecoil = false;
	int moveSpeed = 105;
	int bulletsPerShoot = 1;
	float bulletSpread = 0.75;
	float recoilHorizontal = 1;
	float recoilVertical = 1;

protected:
	using RainbowSixSDK::RainbowSixSDK;
	bool UpdateMisc() {

		try {
			Write<bool>({ rainbowSix_exe + BaseOffsets::GAME_MANAGER, 0xC8, 0, 0x30, 0x30, 0x28, 0x40 }, !noFlash);
			Write<float>({ rainbowSix_exe + BaseOffsets::NETWORK_MANAGER, 0xF8, 8, 0x7F0 }, noClip ? -1.f : 0.0003051850945f);
			Write<int>({ rainbowSix_exe + BaseOffsets::GAME_MANAGER, 0xC8, 0, 0x30, 0x30, 0x38, 0x58 }, moveSpeed);
			Write<int>({ rainbowSix_exe + BaseOffsets::GAME_MANAGER, 0x2340, 0x138, 0, 0x40 }, bulletsPerShoot);
			Write<float>({ rainbowSix_exe + BaseOffsets::GAME_MANAGER, 0xC8, 0, 0x90, 0xC8, 0x278, 0x58 }, bulletSpread);
			if (canChangeRecoil) {
				Write<bool>({ rainbowSix_exe + BaseOffsets::GAME_MANAGER, 0xC8, 0, 0x90, 0xC8, 0x278, 0x168 }, !canChangeRecoil);

				Write<float>({ rainbowSix_exe + BaseOffsets::GAME_MANAGER, 0xC8, 0, 0x90, 0xC8, 0x278, 0x14C }, recoilHorizontal);
				Write<float>({ rainbowSix_exe + BaseOffsets::GAME_MANAGER, 0xC8, 0, 0x90, 0xC8, 0x278, 0x15C }, recoilVertical);
			} else Write<bool>({ rainbowSix_exe + BaseOffsets::GAME_MANAGER, 0xC8, 0, 0x90, 0xC8, 0x278, 0x168 }, !canChangeRecoil);
		} catch (const char*) { return false; };

		return true;
	}
};

