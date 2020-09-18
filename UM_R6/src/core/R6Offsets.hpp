#pragma once
#include <iostream>

namespace BaseOffsets {
    constexpr std::ptrdiff_t GAME_MANAGER = 0x53BEFF0;
    constexpr std::ptrdiff_t STATUS_MANAGER = 0x60ACD40;
    constexpr std::ptrdiff_t FOV_MANAGER = 0x53BEFF8;
    constexpr std::ptrdiff_t ROUND_MANAGER = 0x53AE8B0;
    constexpr std::ptrdiff_t GLOW_MANAGER = 0x60A2628;
    constexpr std::ptrdiff_t PROFILE_MANAGER = 0x53AC118;
    constexpr std::ptrdiff_t VTMARKER = 0x3ACD590;
    constexpr std::ptrdiff_t NETWORK_MANAGER = 0x53AE880;
    constexpr std::ptrdiff_t SPOOF_MANAGER = 0x53AC148;
    constexpr std::ptrdiff_t CAMERA_MANAGER = 0x53BEE00;
    constexpr std::ptrdiff_t SETTINGS_MANAGER = 0x60A0F48;
    constexpr std::ptrdiff_t SKIN_MANAGER = 0x60A2FC0;
    constexpr std::ptrdiff_t GAMERENDERER = 0x60A2688;
    constexpr std::ptrdiff_t INPUT_MANAGER = 0x53AC038;
    constexpr std::ptrdiff_t FREEZE_MANAGER = 0x53A8DD0;
    constexpr std::ptrdiff_t GAMEMODE_MANAGER = 0x60A2838;
}

namespace Offsets {
    constexpr std::ptrdiff_t OFFSET_ENTITY_LIST = 0xC8;
    constexpr std::ptrdiff_t OFFSET_ENTITY_COUNT = 0xD0;
}
