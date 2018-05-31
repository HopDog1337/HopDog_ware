#include "sdk.h"
#include "hooks.h"
#include "Menu.h"
#include "global.h"
#include "Hitmarker.h"
#include "ESP.h"
#include "ImGUI\imconfig.h"
#include "ImGUI\imgui.h"
#include "ImGUI\imgui_internal.h"
#include "ImGUI\stb_rect_pack.h"
#include "ImGUI\stb_textedit.h"
#include "ImGUI\stb_truetype.h"
#include "ImGUI\DX9\imgui_impl_dx9.h"
#include "Items.h"
#include "Config.h"
#include "GameUtils.h"

typedef void(*CL_FullUpdate_t) (void);
CL_FullUpdate_t CL_FullUpdate = nullptr;

void DrawRectRainbow(int x, int y, int width, int height, float flSpeed, float &flRainbow)
{
	ImDrawList* windowDrawList = ImGui::GetWindowDrawList();

	Color colColor(255, 255, 255, 255);

	flRainbow += flSpeed;
	if (flRainbow > 1.f) flRainbow = 0.f;

	for (int i = 0; i < width; i++)
	{
		float hue = (1.f / (float)width) * i;
		hue -= flRainbow;
		if (hue < 0.f) hue += 1.f;

		Color colRainbow = colColor.FromHSB(hue, 1.f, 1.f);
		windowDrawList->AddRectFilled(ImVec2(x + i, y), ImVec2(width, height), colRainbow.GetU32());
	}
}

void KnifeApplyCallbk()
{
	static auto CL_FullUpdate = reinterpret_cast<CL_FullUpdate_t>(FindPattern("engine.dll", reinterpret_cast<PBYTE>("\xA1\x00\x00\x00\x00\xB9\x00\x00\x00\x00\x56\xFF\x50\x14\x8B\x34\x85"), "x????x????xxxxxxx"));
	CL_FullUpdate();
}

PresentFn oPresent;

ImFont* Impact;
ImFont* Default;
ImFont* Tabs;
ImFont* Tabs2;

tReset oResetScene;

void GUI_Init(IDirect3DDevice9* pDevice) // Setup for imgui
{
	ImGuiIO& io = ImGui::GetIO();
	ImGui_ImplDX9_Init(G::Window, pDevice);

	ImGuiStyle& style = ImGui::GetStyle();

	Impact = io.Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\Impact.ttf", 32.0f);
	Default = io.Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\micross.ttf", 14.0f);
	Tabs = io.Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\Skeet-tabs.ttf", 60.0f);
	Tabs2 = io.Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\SkeetFont.ttf", 60.0f);

	ImGuiStyle& styled = ImGui::GetStyle();

	style.Alpha = .0f;
	style.WindowPadding = ImVec2(14, 8);
	style.WindowMinSize = ImVec2(32, 32);
	style.WindowRounding = 0.5f; //4.0
	style.ChildWindowRounding = 0.0f;
	style.FramePadding = ImVec2(1, 0);
	style.FrameRounding = 0.0f; //2.0
	style.ItemSpacing = ImVec2(0, 0);
	style.ItemInnerSpacing = ImVec2(4, 4);
	style.TouchExtraPadding = ImVec2(0, 0);
	style.IndentSpacing = 21.0f;
	style.ColumnsMinSpacing = 3.0f;
	style.ScrollbarSize = 8.0f;
	style.ScrollbarRounding = 1.0f; //16.0
	style.GrabMinSize = 0.1f;
	style.GrabRounding = 16.0f; //16.0
	style.ButtonTextAlign = ImVec2(0.f, -3.f);
	style.DisplayWindowPadding = ImVec2(22, 22);
	style.DisplaySafeAreaPadding = ImVec2(4, 4);
	style.AntiAliasedLines = true;
	style.AntiAliasedShapes = true;
	style.CurveTessellationTol = 1.25f;

	G::Init = true;
}

void color()
{
	int MenuRed, MenuGreen, MenuBlue;

	MenuRed = Clientvariables->Colors.MenuColor[0] * 255;
	MenuGreen = Clientvariables->Colors.MenuColor[1] * 255;
	MenuBlue = Clientvariables->Colors.MenuColor[2] * 255;

	ImColor mainColor = ImColor(MenuRed, MenuGreen, MenuBlue, 255);
	ImColor bodyColor = ImColor(24, 24, 24, 239);
	ImColor fontColor = ImColor(255, 255, 255, 255);

	ImGuiStyle& styled = ImGui::GetStyle();

	ImVec4 mainColorHovered = ImVec4(mainColor.Value.x + 0.1f, mainColor.Value.y + 0.1f, mainColor.Value.z + 0.1f, mainColor.Value.w);
	ImVec4 mainColorActive = ImVec4(mainColor.Value.x + 0.2f, mainColor.Value.y + 0.2f, mainColor.Value.z + 0.2f, mainColor.Value.w);
	ImVec4 menubarColor = ImVec4(bodyColor.Value.x, bodyColor.Value.y, bodyColor.Value.z, bodyColor.Value.w - 0.8f);
	ImVec4 frameBgColor = ImVec4(bodyColor.Value.x, bodyColor.Value.y, bodyColor.Value.z, bodyColor.Value.w + .1f);
	ImVec4 tooltipBgColor = ImVec4(bodyColor.Value.x, bodyColor.Value.y, bodyColor.Value.z, bodyColor.Value.w + .05f);

	styled.Colors[ImGuiCol_Text] = fontColor;
	styled.Colors[ImGuiCol_TextDisabled] = ImVec4(0.24f, 0.23f, 0.29f, 1.00f);
	styled.Colors[ImGuiCol_WindowBg] = bodyColor;
	styled.Colors[ImGuiCol_ChildWindowBg] = ImColor(20, 20, 20, 255);
	styled.Colors[ImGuiCol_PopupBg] = tooltipBgColor;
	styled.Colors[ImGuiCol_Border] = ImColor(1, 1, 1, 255);
	styled.Colors[ImGuiCol_BorderShadow] = ImVec4(0.92f, 0.91f, 0.88f, 0.00f);
	styled.Colors[ImGuiCol_FrameBg] = frameBgColor;
	styled.Colors[ImGuiCol_FrameBgHovered] = mainColorHovered;
	styled.Colors[ImGuiCol_FrameBgActive] = mainColorActive;
	styled.Colors[ImGuiCol_FrameOut] = ImColor(65, 64, 65, 255);
	styled.Colors[ImGuiCol_Frameout2] = ImColor(15, 15, 15, 255);
	styled.Colors[ImGuiCol_TitleBg] = mainColor;
	styled.Colors[ImGuiCol_TitleBgCollapsed] = mainColor;
	styled.Colors[ImGuiCol_TitleBgActive] = mainColor;
	styled.Colors[ImGuiCol_MenuBarBg] = ImVec4(0.24f, 0.24f, 0.24f, 1.00f);
	styled.Colors[ImGuiCol_ScrollbarBg] = ImVec4(frameBgColor.x + .1f, frameBgColor.y + .1f, frameBgColor.z + .1f, frameBgColor.w);
	styled.Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.40f, 0.40f, 0.40f, 1.f);
	styled.Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.40f, 0.40f, 0.40f, 1.f);
	styled.Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.40f, 0.40f, 0.40f, 1.f);
	styled.Colors[ImGuiCol_ComboBg] = frameBgColor;
	styled.Colors[ImGuiCol_CheckMark] = mainColor;
	styled.Colors[ImGuiCol_SliderGrab] = mainColor;
	styled.Colors[ImGuiCol_SliderGrabActive] = mainColorHovered;
	styled.Colors[ImGuiCol_Button] = mainColor;
	styled.Colors[ImGuiCol_Buttontemp1] = ImColor(0, 0, 0, 0);
	styled.Colors[ImGuiCol_Buttontemp2] = ImColor(45, 44, 45, 255);
	styled.Colors[ImGuiCol_ButtonHovered] = mainColorHovered;
	styled.Colors[ImGuiCol_ButtonActive] = mainColorActive;
	styled.Colors[ImGuiCol_Header] = mainColor;
	styled.Colors[ImGuiCol_HeaderHovered] = mainColorHovered;
	styled.Colors[ImGuiCol_HeaderActive] = mainColorActive;

	styled.Colors[ImGuiCol_Column] = ImVec4(.30f, .30f, .30f, 1.f);
	styled.Colors[ImGuiCol_ColumnHovered] = mainColorHovered;
	styled.Colors[ImGuiCol_ColumnActive] = mainColorActive;

	styled.Colors[ImGuiCol_ResizeGrip] = mainColor;
	styled.Colors[ImGuiCol_ResizeGripHovered] = mainColorHovered;
	styled.Colors[ImGuiCol_ResizeGripActive] = mainColorActive;
	styled.Colors[ImGuiCol_CloseButton] = ImVec4(0.f, 0.f, 0.f, 0.f);
	styled.Colors[ImGuiCol_CloseButtonHovered] = ImVec4(0.f, 0.f, 0.f, 0.f);
	styled.Colors[ImGuiCol_CloseButtonActive] = ImVec4(0.f, 0.f, 0.f, 0.f);
	styled.Colors[ImGuiCol_PlotLines] = mainColor;
	styled.Colors[ImGuiCol_PlotLinesHovered] = mainColorHovered;
	styled.Colors[ImGuiCol_PlotHistogram] = mainColor;
	styled.Colors[ImGuiCol_PlotHistogramHovered] = mainColorHovered;
	styled.Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.25f, 1.00f, 0.00f, 0.43f);
	styled.Colors[ImGuiCol_ModalWindowDarkening] = ImVec4(1.00f, 0.98f, 0.95f, 0.73f);
}

void TextColor(bool active)
{
	auto& style = ImGui::GetStyle();

	if (active)
		style.Colors[ImGuiCol_Text] = ImVec4(0.98f, 0.98f, 0.98f, 1.f);
	else
		style.Colors[ImGuiCol_Text] = ImVec4(0.39f, 0.39f, 0.39f, 1.f);
}


void buttonColor(bool bActive)
{
	ImGuiStyle& style = ImGui::GetStyle();

	if (bActive)
	{
		style.Colors[ImGuiCol_Button] = style.Colors[ImGuiCol_WindowBg];
		style.Colors[ImGuiCol_ButtonHovered] = style.Colors[ImGuiCol_WindowBg];
		style.Colors[ImGuiCol_ButtonActive] = style.Colors[ImGuiCol_WindowBg];
	}
	else
	{
		style.Colors[ImGuiCol_Button] = ImColor(10, 10, 10);
		style.Colors[ImGuiCol_ButtonHovered] = ImColor(10, 10, 10);
		style.Colors[ImGuiCol_ButtonActive] = ImColor(10, 10, 10);
	}
}

void lineColor(bool bActive)
{
	ImGuiStyle& style = ImGui::GetStyle();

	if (bActive)
	{
		style.Colors[ImGuiCol_Button] = ImColor(120, 120, 120);
		style.Colors[ImGuiCol_ButtonHovered] = ImColor(120, 120, 120);
		style.Colors[ImGuiCol_ButtonActive] = ImColor(120, 120, 120);
	}
	else
	{
		style.Colors[ImGuiCol_Button] = ImColor(10, 10, 10);
		style.Colors[ImGuiCol_ButtonHovered] = ImColor(10, 10, 10);
		style.Colors[ImGuiCol_ButtonActive] = ImColor(10, 10, 10);
	}
}

void lineColor1(bool bActive)
{
	ImGuiStyle& style = ImGui::GetStyle();

	if (bActive)
	{
		style.Colors[ImGuiCol_Button] = ImColor(120, 120, 120);
		style.Colors[ImGuiCol_ButtonHovered] = ImColor(120, 120, 120);
		style.Colors[ImGuiCol_ButtonActive] = ImColor(120, 120, 120);
	}
	else
	{
		style.Colors[ImGuiCol_Button] = style.Colors[ImGuiCol_WindowBg];
		style.Colors[ImGuiCol_ButtonHovered] = style.Colors[ImGuiCol_WindowBg];
		style.Colors[ImGuiCol_ButtonActive] = style.Colors[ImGuiCol_WindowBg];
	}
}


void Border(bool active)
{
	auto& style = ImGui::GetStyle();

	if (active)
		style.Colors[ImGuiCol_Border] = ImColor(80, 80, 80, 255);
	else
		style.Colors[ImGuiCol_Border] = ImColor(0, 0, 0, 0);
}

void BtnNormal()
{
	auto& style = ImGui::GetStyle();
	style.Colors[ImGuiCol_Button] = ImVec4(0.13f, 0.13f, 0.13f, 1.f);
	style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.13f, 0.13f, 0.13f, 1.f);
	style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.13f, 0.13f, 0.13f, 1.f);
	style.Colors[ImGuiCol_Text] = ImVec4(0.78431372549f, 0.78431372549f, 0.78431372549f, 1.f);

}

void BtnActive()
{
	int MenuRed, MenuGreen, MenuBlue;

	MenuRed = Clientvariables->Colors.MenuColor[0] * 255;
	MenuGreen = Clientvariables->Colors.MenuColor[1] * 255;
	MenuBlue = Clientvariables->Colors.MenuColor[2] * 255;

	auto& style = ImGui::GetStyle();
	style.Colors[ImGuiCol_Button] = ImColor(0.13f, 0.13f, 0.13f, 1.f);
	style.Colors[ImGuiCol_ButtonHovered] = ImColor(0.13f, 0.13f, 0.13f, 1.f);
	style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.13f, 0.13f, 0.13f, 1.f);
	style.Colors[ImGuiCol_Text] = ImColor(MenuRed, MenuGreen, MenuBlue, 255);
}

void ragebot() {
	auto& style = ImGui::GetStyle();

	style.Colors[ImGuiCol_ChildWindowBg] = ImColor(18, 18, 18, 180);
	ImGui::Columns(2, "yes", false);


	ImGui::BeginChild("Aimbot", ImVec2(272, 289), true);
	{
		ImGui::Checkbox("Enable", &Clientvariables->Ragebot.EnableAimbot);

		ImGui::Text("Selection");
		ImGui::Combo("##selection", &Clientvariables->Ragebot.AimbotSelection, SelectionMode, ARRAYSIZE(SelectionMode));

		ImGui::Checkbox("Friendly fire", &Clientvariables->Ragebot.FriendlyFire);
		ImGui::Checkbox("Automatic fire", &Clientvariables->Ragebot.AutomaticFire);
		ImGui::Checkbox("Silent aim", &Clientvariables->Ragebot.SilentAimbot);

		ImGui::Text("Minimum hit chance");
		ImGui::SliderInt("##hitchance", &Clientvariables->Ragebot.Minhitchance, 0, 100, "%.0f%%");

		ImGui::Text("Minimum damage");
		ImGui::SliderInt("##mindamage", &Clientvariables->Ragebot.Mindamage, 1, 101, "%.0f%%");

		ImGui::Checkbox("Automatic scope", &Clientvariables->Ragebot.AutomaticScope);
		ImGui::Checkbox("Automatic penetration", &Clientvariables->Ragebot.Autowall);
		ImGui::Checkbox("Hitscan penetration", &Clientvariables->Ragebot.AutowallHitscan);

		ImGui::Checkbox("Fakewalk", &Clientvariables->Misc.FakewalkEnable);
		if (Clientvariables->Misc.FakewalkEnable) {
			ImGui::Hotkey("##walkkey", &Clientvariables->Misc.FakewalkKey, ImVec2(150, 20));
			ImGui::SliderInt("fakewalkspeed", &Clientvariables->Misc.fakewalk_speed, 0, 14);
		}
	}
	ImGui::EndChild();

	ImGui::NextColumn();
	ImGui::BeginChild("##antiaimbot", ImVec2(272, 289), true);
	{
		ImGui::Checkbox("Enable", &Clientvariables->Antiaim.AntiaimEnable);
		ImGui::Text("Pitch");
		ImGui::Combo("##pitch", &Clientvariables->Antiaim.Pitch, AntiaimbotPitch, ARRAYSIZE(AntiaimbotPitch));

		ImGui::Text("Anti-aimbot");
		ImGui::Combo("##freestanding", &Clientvariables->Antiaim.Freestanding, Freestanding, ARRAYSIZE(Freestanding));

		ImGui::Spacing();
		if (Clientvariables->Antiaim.Freestanding == 1)
		{
			ImGui::Text("Yaw");
			ImGui::Combo("##yaw", &Clientvariables->Antiaim.Yaw, AntiaimbotYaw, ARRAYSIZE(AntiaimbotYaw));
			ImGui::SliderInt("##yawadd", &Clientvariables->Antiaim.YawAdd, -180, 180);

			ImGui::Text("Move");
			ImGui::Combo("##yawrunning", &Clientvariables->Antiaim.YawRunning, AntiaimbotYawRun, ARRAYSIZE(AntiaimbotYawRun));
			ImGui::SliderInt("##yawrunadd", &Clientvariables->Antiaim.YawRunningAdd, -180, 180);

			ImGui::Text("Fake");
			ImGui::Combo("##fakeyaw", &Clientvariables->Antiaim.FakeYaw, AntiaimbotYawFake, ARRAYSIZE(AntiaimbotYawFake));
			ImGui::SliderInt("##yawfakeadd", &Clientvariables->Antiaim.FakeYawAdd, -180, 180);



			ImGui::Text("LBY");
			ImGui::SliderInt("##freestand", &Clientvariables->Antiaim.FreestandingDelta, -180, 180);

			ImGui::Text("At Target");
			ImGui::Combo("##dynamic", &Clientvariables->Antiaim.AtPlayer, AtTarget, ARRAYSIZE(AtTarget));
		}
		if (Clientvariables->Antiaim.Freestanding == 2)
		{
			ImGui::Text("Right key");
			ImGui::Hotkey("##rightkey", &Clientvariables->Misc.Rightside, ImVec2(150, 20));

			ImGui::Text("Left key");
			ImGui::Hotkey("##leftkey", &Clientvariables->Misc.Leftside, ImVec2(150, 20));

			ImGui::Text("Back key");
			ImGui::Hotkey("##backkey", &Clientvariables->Misc.Backside, ImVec2(150, 20));

			ImGui::Text("Fake");
			ImGui::Combo("##fakeyaw", &Clientvariables->Antiaim.FakeYaw, AntiaimbotYawFake, ARRAYSIZE(AntiaimbotYawFake));
			ImGui::SliderInt("##yawfakeadd", &Clientvariables->Antiaim.FakeYawAdd, -180, 180);


			ImGui::Text("LBY");
			ImGui::SliderInt("##freestand", &Clientvariables->Antiaim.FreestandingDelta, -180, 180);
		}
		if (Clientvariables->Antiaim.Freestanding == 3)
		{
			ImGui::Text("Right key");
			ImGui::Hotkey("##rightkey", &Clientvariables->Misc.Rightside, ImVec2(150, 20));

			ImGui::Text("Left key");
			ImGui::Hotkey("##leftkey", &Clientvariables->Misc.Leftside, ImVec2(150, 20));

			ImGui::Text("Back key");
			ImGui::Hotkey("##backkey", &Clientvariables->Misc.Backside, ImVec2(150, 20));

			ImGui::Text("Fake");
			ImGui::Combo("##fakeyaw", &Clientvariables->Antiaim.FakeYaw, AntiaimbotYawFake, ARRAYSIZE(AntiaimbotYawFake));
			ImGui::SliderInt("##yawfakeadd", &Clientvariables->Antiaim.FakeYawAdd, -180, 180);

			ImGui::Text("LBY");
			ImGui::SliderInt("##freestand", &Clientvariables->Antiaim.FreestandingDelta, -180, 180);
		}

	}
	ImGui::EndChild();

	ImGui::NextColumn();
	ImGui::BeginChild("##fakelag", ImVec2(272, 170), true);
	{
		ImGui::Checkbox("Enable", &Clientvariables->Misc.FakelagEnable);
		ImGui::Combo("##mode", &Clientvariables->Misc.FakelagMode, FakelagMode, ARRAYSIZE(FakelagMode));

		ImGui::Text("Ticks");
		ImGui::SliderInt("##choke", &Clientvariables->Misc.FakelagAmount, 1, 14);

		ImGui::Checkbox("Fakelag on ground", &Clientvariables->Misc.FakelagOnground);
	}
	ImGui::EndChild();


	ImGui::NextColumn();

	ImGui::BeginChild("##other", ImVec2(272, 170), true);
	{

		TextColor(true);
		ImGui::Checkbox("Remove spread", &Clientvariables->Ragebot.NoSpread);
		ImGui::Checkbox("Remove recoil", &Clientvariables->Ragebot.NoRecoil);
		ImGui::Checkbox("Anti-aim correction", &Clientvariables->Ragebot.AutomaticResolver);
		ImGui::Checkbox("Engine prediction", &Clientvariables->Ragebot.EnginePred);
		ImGui::Checkbox("Delay shot", &Clientvariables->Ragebot.PositionAdjustment);
		if (Clientvariables->Ragebot.PositionAdjustment)
		{
			ImGui::SliderInt("##delay", &Clientvariables->Ragebot.EnginePred1, 0, 10, "%.fms%");
		}
		ImGui::Checkbox("Multipoint", &Clientvariables->Ragebot.enablescale);
		ImGui::Combo("##hitbox", &Clientvariables->Ragebot.Hitbox, HitboxMode, ARRAYSIZE(HitboxMode));
		ImGui::Text("Head scale");
		ImGui::SliderFloat("##headscale", &Clientvariables->Ragebot.Headscale, 0.f, 1.f, "%.2f%%");

		ImGui::Text("Body scale");
		ImGui::SliderFloat("##bodyscale", &Clientvariables->Ragebot.Bodyscale, 0.f, 1.f, "%.2f%%");
	}

	TextColor(true);
	ImGui::Text("Hitscan");
	ImGui::Combo("##multibox", &Clientvariables->Ragebot.Hitscan, HitscanMode, ARRAYSIZE(HitscanMode));
	if (Clientvariables->Ragebot.Hitscan == 4)
	{
		ImGui::Text("Bones to hitscan");
		ImGui::Separator();
		ImGui::Selectable("Head", &Clientvariables->Ragebot.HeadH);
		ImGui::Selectable("Neck", &Clientvariables->Ragebot.NeckH);
		ImGui::Selectable("Body", &Clientvariables->Ragebot.BodyH);
		ImGui::Selectable("Arms", &Clientvariables->Ragebot.ArmsH);
		ImGui::Selectable("Legs", &Clientvariables->Ragebot.LegsH);
		ImGui::Selectable("Foot", &Clientvariables->Ragebot.FootsH);
	}
	ImGui::EndChild();
}

void visuals() {
	auto& style = ImGui::GetStyle();

	style.Colors[ImGuiCol_ChildWindowBg] = ImColor(18, 18, 18, 180);
	ImGui::Columns(2, "yes", false);

	ImGui::Text("Effects");
	ImGui::BeginChild("##effects", ImVec2(272, 459), true);  // 272, 289 + 170
	{
		TextColor(true);
		ImGui::Checkbox("Remove smoke grenades", &Clientvariables->Visuals.Nosmoke);
		ImGui::Checkbox("Remove fog", &Clientvariables->Visuals.RemoveParticles);
		ImGui::Checkbox("Remove visual recoil", &Clientvariables->Visuals.Novisrevoil);
		ImGui::Checkbox("Remove scope", &Clientvariables->Visuals.Noscope);
		ImGui::Checkbox("Nightmode", &Clientvariables->Visuals.nightmode);

		ImGui::Text("Skybox");
		ImGui::Combo("##Skybox", &Clientvariables->Visuals.Skybox, Skyboxmode, ARRAYSIZE(Skyboxmode));

		ImGui::Text("Thirdperson");
		ImGui::Hotkey("##thirdperson", &Clientvariables->Misc.TPKey, ImVec2(164, 20));
		ImGui::Combo("##angles", &Clientvariables->Misc.TPangles, ThirdpersonAngles, ARRAYSIZE(ThirdpersonAngles));
	}
	ImGui::EndChild();

	ImGui::NextColumn();

	ImGui::Text("Player ESP");
	ImGui::BeginChild("##player", ImVec2(272, 459), true);
	{

		TextColor(true);
		ImGui::Checkbox("Enable", &Clientvariables->Visuals.EspEnable);

		// box
		ImGui::Checkbox("Bounding box", &Clientvariables->Visuals.BoundingBox);
		ImGui::SameLine();
		ImGui::MyColorEdit3("##box", Clientvariables->Colors.BoundingBox, 1 << 7);

		// health bar
		ImGui::Checkbox("Health bar", &Clientvariables->Visuals.Health);

		// name
		ImGui::Checkbox("Name", &Clientvariables->Visuals.Name);

		// weapons
		ImGui::Checkbox("Weapons", &Clientvariables->Visuals.Weapon);
		ImGui::SameLine();
		ImGui::MyColorEdit3("##colorweapons", Clientvariables->Colors.WeaponsCAT, 1 << 7);

		//  ammo
		ImGui::Checkbox("Ammo", &Clientvariables->Visuals.Ammo);

		// resolver flags
		ImGui::Checkbox("Resolver", &Clientvariables->Visuals.Flags);

		// show armor
		ImGui::Checkbox("Armor", &Clientvariables->Visuals.Armor);
		ImGui::SameLine();
		ImGui::MyColorEdit3("##armorcolor", Clientvariables->Colors.ArmorColor, 1 << 7);

		//snaplines
		ImGui::Checkbox("Line of sight", &Clientvariables->Visuals.SnapLines);
		ImGui::SameLine();
		ImGui::MyColorEdit3("##snapline", Clientvariables->Colors.Snaplines, 1 << 7);

		// skeletons
		ImGui::Checkbox("Skeleton", &Clientvariables->Visuals.Bones);
		ImGui::SameLine();
		ImGui::MyColorEdit3("##bones", Clientvariables->Colors.Skeletons, 1 << 7);

		// Glow
		ImGui::Checkbox("Glow", &Clientvariables->Visuals.Glow);
		if (Clientvariables->Visuals.Glow) {
			ImGui::SliderFloat("##glowforce", &Clientvariables->Visuals.Opacity, 0.f, 1.f, "%.2f%%");
			ImGui::SameLine();
			ImGui::MyColorEdit3("##glow", Clientvariables->Colors.Glow, 1 << 7);
		}

		//  hitmarker
		ImGui::Text("Hitmarker");
		ImGui::Combo("##hitmarkerwav", &Clientvariables->Visuals.Hitmarker, Hitmarkerwavs, ARRAYSIZE(Hitmarkerwavs));
		ImGui::Checkbox("hitmarker", &Clientvariables->Visuals.aimware_hitmarker); // call it lelelelelelelelellelelelellelelelelelellelele
		ImGui::Combo("##aimwarecolor", &Clientvariables->Visuals.aimware_hitmarker_color, aimware_color, ARRAYSIZE(aimware_color));
		ImGui::SliderInt("##hitmarkersize", &Clientvariables->Visuals.hitmarker_size, 1, 20);


		//  player shit
		ImGui::Checkbox("Enable", &Clientvariables->Visuals.ChamsEnable);
		ImGui::Checkbox("Player", &Clientvariables->Visuals.ChamsPlayer);
		ImGui::SameLine();
		ImGui::MyColorEdit3("##chams", Clientvariables->Colors.PlayerChams, 1 << 7);
		ImGui::Checkbox("Player (behind wall)", &Clientvariables->Visuals.ChamsPlayerWall);
		ImGui::SameLine();
		ImGui::MyColorEdit3("##chamswall", Clientvariables->Colors.PlayerChamsWall, 1 << 7);

		// dropped weapons
		ImGui::Text("Dropped weapons");
		ImGui::Combo("##droppedweapon", &Clientvariables->Visuals.DroppedWeapons, DroppedWeapons, ARRAYSIZE(DroppedWeapons));
		ImGui::SameLine();
		ImGui::MyColorEdit3("##weaponcolor", Clientvariables->Colors.DroppedWeapon, 1 << 7);

		// grenades
		ImGui::Checkbox("Grenades", &Clientvariables->Visuals.ThrownNades);

		// bomb
		ImGui::Checkbox("Bomb", &Clientvariables->Visuals.Bomb);

		// grenade trajectory
		ImGui::Checkbox("Grenade trajectory", &Clientvariables->Visuals.GrenadePrediction);
		ImGui::SameLine();
		ImGui::MyColorEdit3("##nadetrajectory", Clientvariables->Colors.GrenadePrediction, 1 << 7);

		// bullet beams
		ImGui::Checkbox("Bullet tracers", &Clientvariables->Visuals.BulletTracers);
		ImGui::SameLine();
		ImGui::MyColorEdit3("##tracers", Clientvariables->Colors.Bulletracer, 1 << 7);

		// bullet impacts
		ImGui::Checkbox("Bullet impacts", &Clientvariables->Visuals.Bullet_impacts);
		ImGui::SameLine();
		ImGui::MyColorEdit3("##impacts", Clientvariables->Colors.bullet_impacts, 1 << 7);

		// spread crosshair
		ImGui::Checkbox("Spread crosshair", &Clientvariables->Visuals.SpreadCrosshair);
		ImGui::SameLine();
		ImGui::MyColorEdit3("##spreadcolor", Clientvariables->Colors.SpreadCrosshair, 1 << 7);

		//_______-_________ test_______-_________//
		ImGui::Checkbox("POVArrows", &Clientvariables->Visuals.OutOfPOVArrows);

		// Anti-aim arrows
		ImGui::Checkbox("anti-aim arrows", &Clientvariables->Visuals.rifk_arrows);

		// Damage indicator
		ImGui::Checkbox("Damage indicator", &Clientvariables->Visuals.DamageIndicators);
		ImGui::SameLine();
		ImGui::MyColorEdit3("##damageindicator", Clientvariables->Colors.DamageIndicator, 1 << 7);

		// asus probs
		ImGui::Checkbox("Asus props", &Clientvariables->Visuals.TransparensWallsEnable);

		// enemy only visuals
		ImGui::Checkbox("Enemy only", &Clientvariables->Visuals.EnemyOnly);

		// local esp
		ImGui::Checkbox("Self ESP", &Clientvariables->Visuals.LocalPlayer);

		// self chams
		ImGui::Checkbox("Self Chams", &Clientvariables->Visuals.LocalChams);
		ImGui::SameLine();
		ImGui::MyColorEdit3("##localchams", Clientvariables->Colors.LocalChams, 1 << 7);

		// self glow
		ImGui::Checkbox("Self Glow", &Clientvariables->Visuals.LocalGlow);
		ImGui::SameLine();
		ImGui::MyColorEdit3("##localglow", Clientvariables->Colors.LocalGlow, 1 << 7);


	}

	ImGui::EndChild();

}

void misc() {

	auto& style = ImGui::GetStyle();
	style.Colors[ImGuiCol_ChildWindowBg] = ImColor(18, 18, 18, 180);
	ImGui::Columns(2, "yes", false);
	ImGui::Text("Miscellaneous");
	ImGui::BeginChild("##misc", ImVec2(250, 220), true);
	{
		ImGui::Separator();
		ImGui::Checkbox("Clan-tag", &Clientvariables->Misc.clantag);
		ImGui::Checkbox("Anti-untrusted", &Clientvariables->Misc.AntiUT);
		ImGui::Checkbox("Enable buybot", &Clientvariables->Misc.buybot);
		if (Clientvariables->Misc.buybot) {
			ImGui::Text("Buy bot");
			ImGui::Combo("##buybot", &Clientvariables->Misc.buybot2, BuyBot, ARRAYSIZE(BuyBot));
			ImGui::Checkbox("Helmet and grenades", &Clientvariables->Misc.buybot3);
		}
		ImGui::Checkbox("Bunnyhop", &Clientvariables->Misc.AutoJump);
		ImGui::Checkbox("Air strafe", &Clientvariables->Misc.AutoStrafe);
		if (Clientvariables->Misc.AutoStrafe) {
			ImGui::Text("Circle-Strafe");
			ImGui::Hotkey("##speedkey", &Clientvariables->Misc.PrespeedKey, ImVec2(150, 20));
			ImGui::SliderInt("##retrack", &Clientvariables->Misc.Retrack, 1, 8);
		}
		ImGui::Separator();
	}
	ImGui::EndChild();


	ImGui::NextColumn();
	ImGui::Text("Settings");
	ImGui::BeginChild("##settings", ImVec2(246, 300), true);
	{
		ImGui::Separator();
		ImGui::NewLine();
		ImGui::SameLine(40);
		ImGui::Text("Menu color");
		ImGui::SameLine(216);
		ImGui::MyColorEdit3("##menucolor", Clientvariables->Colors.MenuColor, 1 << 7);

		ImGui::NewLine();
		ImGui::SameLine(40);
		ImGui::Text("Menu key");
		ImGui::NewLine();
		ImGui::SameLine(35);
		ImGui::Hotkey("##menukey", &Clientvariables->Misc.MenuKey, ImVec2(150, 20));

		ImGui::NewLine();
		style.Colors[ImGuiCol_Button] = ImColor(26, 28, 33, 180);
		ImGui::SameLine(40);
		ImGui::Combo("##cfg", &Clientvariables->Misc.ConfigSelection, Configs, ARRAYSIZE(Configs));

		ImGui::NewLine();
		ImGui::SameLine(40);
		style.ButtonTextAlign = ImVec2(0.5f, 0.5f);
		if (ImGui::Button("Save", ImVec2(156, 27)))
			ConSys->SaveConfig();

		ImGui::NewLine();
		ImGui::SameLine(40);
		style.ButtonTextAlign = ImVec2(0.5f, 0.5f);
		if (ImGui::Button("Load", ImVec2(156, 27)))
			ConSys->LoadConfig();

		ImGui::NewLine();
		ImGui::SameLine(40);
		style.ButtonTextAlign = ImVec2(0.5f, 0.5f);
		if (ImGui::Button("Reset", ImVec2(156, 27)))
			ConSys->Reset();

	}
	ImGui::EndChild();

	ImGui::TextColored(ImVec4(0.f, 1.f, 0.f, 1.f), "its a big hake");
	ImGui::TextColored(ImVec4(0.f, 1.f, 0.f, 1.f), "[pasted version by hopdog]");
}

HRESULT __stdcall Hooks::D3D9_EndScene(IDirect3DDevice9* pDevice)
{
	HRESULT result = d3d9VMT->GetOriginalMethod<EndSceneFn>(42)(pDevice);

	if (!G::Init)
	{
		GUI_Init(pDevice);
	}
	else
	{
		if (g_pEngine->IsInGame() && g_pEngine->IsConnected())
			ImGui::GetIO().MouseDrawCursor = G::Opened;
		else
			ImGui::GetIO().MouseDrawCursor = true;
		ImGui_ImplDX9_NewFrame();

		auto& style = ImGui::GetStyle();

		if (G::Opened && style.Alpha < 1.f)
		{
			G::ShowMenu = true;
			if (style.Alpha > 1.f)
				style.Alpha = 1.f;
			else if (style.Alpha != 1.f)
				style.Alpha += 0.03f;
		}
		else if (style.Alpha > 0.f)
		{
			if (style.Alpha < 0.f)
				style.Alpha = 0.f;
			else if (style.Alpha != 0.f)
				style.Alpha -= 0.03f;
			if (style.Alpha == 0.f)
				G::ShowMenu = false;
		}

		static int tab;

		if (G::ShowMenu)
		{
			ImVec2 mainWindowPos; // wir brauchen die main window pos um nen vernünftigen border zu drawen

			ImGui::PushFont(Default);
			color();
			ImGui::SetNextWindowSize(ImVec2(649, 539));
			ImGui::Begin("Gamephetamine", &G::ShowMenu, ImVec2(712, 699), 0.98f, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_ShowBorders | ImGuiWindowFlags_NoScrollbar);
			{
				mainWindowPos = ImGui::GetWindowPos();

				style.ItemSpacing = ImVec2(0, 0);

				if (tab == 0) BtnActive(); else BtnNormal();
				style.ButtonTextAlign = ImVec2(0.5f, 0.5f);
				if (ImGui::Button("ragebot", ImVec2(161, 41))) tab = 0;
				ImGui::SameLine();

				if (tab == 1) BtnActive(); else BtnNormal();
				style.ButtonTextAlign = ImVec2(0.5f, 0.5f);
				if (ImGui::Button("legitbot", ImVec2(161, 41))) tab = 1;
				ImGui::SameLine();

				if (tab == 2) BtnActive(); else BtnNormal();
				style.ButtonTextAlign = ImVec2(0.5f, 0.5f);
				if (ImGui::Button("visuals", ImVec2(161, 41))) tab = 2;
				ImGui::SameLine();

				if (tab == 3) BtnActive(); else BtnNormal();
				style.ButtonTextAlign = ImVec2(0.5f, 0.5f);
				if (ImGui::Button("misc", ImVec2(161, 41))) tab = 3;

				ImGui::PushFont(Default);
				/*//////////////////////////*/


				ImGui::NextColumn();
				TextColor(true);
				Border(true);
				color();



				style.ItemSpacing = ImVec2(8, 6);
				style.Colors[ImGuiCol_Border] = ImColor(80, 80, 80, 0);
				style.Colors[ImGuiCol_ChildWindowBg] = ImColor(18, 18, 18, 0);
				style.WindowPadding = ImVec2(8, 8);
				ImGui::BeginChild("masterchild", ImVec2(705, 530), true); // Master Child
				{

					style.Colors[ImGuiCol_Border] = ImColor(80, 80, 80, 255);
					if (tab == 0) // Ragebot
					{
						ragebot();
					}
					else if (tab == 1)
					{
						ImGui::TextColored(ImVec4(1.f, 0.f, 0.f, 1.f), "i would assume thats gay");
					}
					else if (tab == 2)
					{
						visuals();
					}
					else if (tab == 3)
					{
						misc();
					}
				} ImGui::EndChild();
			} ImGui::End();

			style.WindowPadding = ImVec2(2, 4);
			style.Colors[ImGuiCol_WindowBg] = ImVec4(0.1f, 0.1f, 0.1f, 0.95f);

			ImGui::SetNextWindowPos(ImVec2(mainWindowPos.x - 5, mainWindowPos.y - 5));
			style.Colors[ImGuiCol_WindowBg] = ImVec4(0.20f, 0.20f, 0.20f, 1.f);
			ImGui::SetNextWindowSize(ImVec2(659, 549));
			ImGui::Begin("##border2", &G::ShowMenu, ImVec2(659, 549), 0.98f, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoInputs | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_ShowBorders);
			{
				mainWindowPos = ImGui::GetWindowPos();
			} ImGui::End();

		}
		ImGui::Render();
	}
	return result;
}

HRESULT __stdcall Hooks::hkdReset(IDirect3DDevice9* pDevice, D3DPRESENT_PARAMETERS* pPresParam)
{
	if (!G::Init)
		return oResetScene(pDevice, pPresParam);

	ImGui_ImplDX9_InvalidateDeviceObjects();
	auto newr = oResetScene(pDevice, pPresParam);
	ImGui_ImplDX9_CreateDeviceObjects();

	return newr;
}
































































































































































































































































































































































































































































