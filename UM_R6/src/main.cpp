#include <Windows.h>
#include <iostream>
#include "managers/DriverCommunication.hpp"
#include "ProcessHelper.hpp"
#include "core/RainbowSix.hpp"
#include "misc/IMGUI/ImguiHandler.hpp"


int main() {
#ifndef _DEBUG
	if (!DriverCommunication::Init("win32u", "NtDxgkGetTrackedWorkloadStatistics")) return 1;
	if (!DriverCommunication::Test()) return 1;
	auto capcom_count_mmunloadeddrivers = DriverCommunication::RemoveFromMMUnloadedDrivers(L"Capcom.sys");
	auto capcom_count_piddbtable = DriverCommunication::RemoveFromPiDDBTable(L"Capcom.sys");

	std::cout << "Capcom.sys" << std::endl;
	std::cout << "MmUnloadedDrivers found count: " << capcom_count_mmunloadeddrivers << std::endl;
	std::cout << "PiDDBTable found count: " << capcom_count_piddbtable << std::endl;

#endif // !_DEBUG

	std::cout << "UM running" << std::endl;

	RainbowSix r6(L"", L"R6Game");
	ImGuiHandler imguih(L"RiverBand");

	while (!r6.TryToAttach() || std::get<0>(r6.GetModule(L"RainbowSix.exe")) == 0) Sleep(500);

	r6.UpdateBases();	

	while (![&]() -> BOOL {
		try { r6.GetGameState(); return true; } catch (const char*) { return false; }
	}()) Sleep(600); // Wait till game init game vars

	std::cout << "R6 Base: 0x" << std::hex << std::get<0>(r6.GetModule(L"RainbowSix.exe")) << std::endl;
	
	imguih.Init();
	imguih.StyleRayTeak();

	imguih.callBack = [&](MSG) -> BOOL {
		if (!r6.Loop()) return false;

		if (ImGui::Begin("Config", 0, ImGuiWindowFlags_MenuBar)) {
			if (ImGui::CollapsingHeader("CavEsp")) {
				ImGui::Checkbox("Enable", &r6.cavEsp);
			}

			if (ImGui::CollapsingHeader("Misc")) {
				ImGui::Checkbox("NoFlash", &r6.noFlash);
				ImGui::Checkbox("NoClip", &r6.noClip);
				ImGui::SliderInt("MoveSpeed", &r6.moveSpeed, 0, 600);		
				ImGui::SliderInt("Bullets Per Shoot", &r6.bulletsPerShoot, 0, 500);
				ImGui::SliderFloat("Bullet Spread", &r6.bulletSpread, -100, 100, "%.3f", 1);
				ImGui::Checkbox("CanChangeRecoil", &r6.canChangeRecoil);
				if (r6.canChangeRecoil) {
					ImGui::SliderFloat("Recoil Horozontal", &r6.recoilHorizontal, -5, 5, "%.3f", 1);
					ImGui::SliderFloat("Recoil Vertical", &r6.recoilVertical, -5, 5, "%.3f", 1);
				}
			}

			if (ImGui::CollapsingHeader("Glow")) {
				if (ImGui::TreeNode("Options")) {
					ImGui::SliderFloat("Distance", &r6.glowOptions.distance, 0, 200);
					ImGui::SliderFloat("Alpha", &r6.glowOptions.alpha, 0, 200);
					ImGui::SliderFloat("StartDistance", &r6.glowOptions.startDistance, 0, 200);
					ImGui::SliderFloat("Opacity", &r6.glowOptions.opacity, 0, 200);
					ImGui::TreePop();
				}

				float colors[4]{ r6.colors.red, r6.colors.blue, r6.colors.green, r6.colors.alpha };
				if (ImGui::ColorPicker4("Glow Color", colors)) {
					r6.colors.red = colors[0];
					r6.colors.blue = colors[1];
					r6.colors.green = colors[2];
					r6.colors.alpha = colors[3];
				}
				ImGui::SliderFloat("Intencity", &r6.colorIntencity, -1000, 1000);

			}
			

			ImGui::End();
		}
	};

	imguih.Loop();
}