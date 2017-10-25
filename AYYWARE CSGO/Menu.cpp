
#include "Menu.h"
#include "Controls.h"
#include "Hooks.h"
#include "Interfaces.h"
#include "CRC32.h"
#include <string>
#include "ItemDefinitions.hpp"
#include "Hooks.h"
#include "Hacks.h"
#include "Chams.h"
#include "ESP.h"
#include "Interfaces.h"
#include "RenderManager.h"
#include "MiscHacks.h"
#include "CRC32.h"
#include "Resolver.h"
#include "MiscHacks.h"

#define WINDOW_WIDTH 933
#define WINDOW_HEIGHT 720

ApocalypseWindow Menu::Window;

void UnLoadCallbk()
{
	DoUnload = true;
}

void Unloadbk()
{
	DoUnload = true;
}

void NApplyCallbk()
{
	
}
void Memz()
{
//	Hooks::XD3();
}

void KnifeApplyCallbk()
{
	static ConVar* Meme = Interfaces::CVar->FindVar("cl_fullupdate");
	Meme->nFlags &= ~FCVAR_CHEAT;
	Interfaces::Engine->ClientCmd_Unrestricted("cl_fullupdate");

}

void SaveLegitCallbk()
{
	switch (Menu::Window.SettingsTab.ConfigBox.GetIndex())
	{
	case 0:
		GUI.SaveWindowState(&Menu::Window, "Legit Config (Nova Hook).cfg");
		break;
	case 1:
		GUI.SaveWindowState(&Menu::Window, "Rage Config (Nova Hook).cfg");
		break;
	case 2:
		GUI.SaveWindowState(&Menu::Window, "MMHvH Config (Nova Hook).cfg");
		break;
	case 3:
		GUI.SaveWindowState(&Menu::Window, "Casual Config (Nova Hook).cfg");
		break;
	case 4:
		GUI.SaveWindowState(&Menu::Window, "Cosmetic (Nova Hook).cfg");
		break;
	case 5:
		GUI.SaveWindowState(&Menu::Window, "Walls (Nova Hook).cfg");
		break;
	}
}

void LoadLegitCallbk()
{
	switch (Menu::Window.SettingsTab.ConfigBox.GetIndex())
	{
	case 0:
		GUI.LoadWindowState(&Menu::Window, "Legit Config (Nova Hook).cfg");
		break;
	case 1:
		GUI.LoadWindowState(&Menu::Window, "Rage Config (Nova Hook).cfg");
		break;
	case 2:
		GUI.LoadWindowState(&Menu::Window, "MMHvH Config (Nova Hook).cfg");
		break;
	case 3:
		GUI.LoadWindowState(&Menu::Window, "Casual Config (Nova Hook).cfg");
		break;
	case 4:
		GUI.LoadWindowState(&Menu::Window, "Cosmetic (Nova Hook).cfg");
		break;
	case 5:
		GUI.LoadWindowState(&Menu::Window, "Walls (Nova Hook).cfg");
		break;
	}
}

void SaveRageCallbk()
{
	GUI.SaveWindowState(&Menu::Window, "Rage Config (Nova Hook).cfg");
}

void LoadRageCallbk()
{
	GUI.LoadWindowState(&Menu::Window, "Rage Config (Nova Hook).cfg");
}

void ApocalypseWindow::Setup()
{
	SetPosition(200, 50);
	SetSize(WINDOW_WIDTH, WINDOW_HEIGHT);
	SetTitle("Nova Hook | Private CS:GO Cheat | Version 1.1.6.2 | Date: 2017/10/02");

	RegisterTab(&RageBotTab);
	RegisterTab(&LegitBotTab);
	RegisterTab(&CustomTab);
	RegisterTab(&VisualsTab);
	RegisterTab(&MiscTab);
	RegisterTab(&SettingsTab);


	RECT Client = GetClientArea();
	Client.bottom -= 29;
	
	RageBotTab.Setup();
	LegitBotTab.Setup();
	CustomTab.Setup();
	VisualsTab.Setup();
	MiscTab.Setup();
	SettingsTab.Setup();


	SaveButton.SetText("Save");
	SaveButton.SetCallback(SaveLegitCallbk);
	SaveButton.SetPosition(20, Client.top);

	LoadButton.SetText("Load");
	LoadButton.SetCallback(LoadLegitCallbk);
	LoadButton.SetPosition(200, Client.top);

	SettingsTab.RegisterControl(&SaveButton);
	SettingsTab.RegisterControl(&LoadButton);

}

void CRageBotTab::Setup()
{

	SetTitle("a");

	ActiveLabel.SetPosition(16, -15);
	ActiveLabel.SetText("Rage Bot Active");
	RegisterControl(&ActiveLabel);

	Active.SetFileId("active");
	Active.SetPosition(110, -13);
	RegisterControl(&Active);

	AimbotGroup.SetPosition(16, 11);
	AimbotGroup.SetText("Aimbot");
	AimbotGroup.SetSize(376, 270);
	RegisterControl(&AimbotGroup);

	AimbotEnable.SetFileId("aim_enable");
	AimbotGroup.PlaceLabledControl("Enable", this, &AimbotEnable);

	AimbotAutoFire.SetFileId("aim_autofire");
	AimbotGroup.PlaceLabledControl("Auto Fire", this, &AimbotAutoFire);

	AimbotFov.SetFileId("aim_fov");
	AimbotFov.SetBoundaries(0.f, 180.f);
	AimbotFov.SetValue(39.f);
	AimbotGroup.PlaceLabledControl("FOV Range", this, &AimbotFov);

	AimbotSilentAim.SetFileId("aim_silent");
	AimbotGroup.PlaceLabledControl("Silent Aim", this, &AimbotSilentAim);

	AWPAtBody.SetFileId("aim_awpatbody");
	AimbotGroup.PlaceLabledControl("AWP at Body", this, &AWPAtBody);

	AutoRevolver.SetFileId("aim_autorevolver");
	AimbotGroup.PlaceLabledControl("Auto Revolver", this, &AutoRevolver);

	AimbotAimStep.SetFileId("aim_aimstep");
	AimbotGroup.PlaceLabledControl("Aim Step", this, &AimbotAimStep);

	AimbotKeyPress.SetFileId("aim_usekey");
	AimbotGroup.PlaceLabledControl("On Key Press", this, &AimbotKeyPress);

	AimbotKeyBind.SetFileId("aim_key");
	AimbotGroup.PlaceLabledControl("Key", this, &AimbotKeyBind);

	AimbotStopKey.SetFileId("aim_stop");
	AimbotGroup.PlaceLabledControl("Stop Aim", this, &AimbotStopKey);
#pragma endregion Aimbot Controls Get Setup in here

#pragma region Target
	TargetGroup.SetPosition(16, 296);
	TargetGroup.SetText("Target");
	TargetGroup.SetSize(376, 180);
	RegisterControl(&TargetGroup);

	TargetSelection.SetFileId("tgt_selection");
	TargetSelection.AddItem("Closest To Crosshair");
	TargetSelection.AddItem("Distance");
	TargetSelection.AddItem("Lowest Health");
	TargetSelection.AddItem("Threat");
	TargetSelection.AddItem("Next Shot");
	TargetGroup.PlaceLabledControl("Selection", this, &TargetSelection);

	TargetFriendlyFire.SetFileId("tgt_friendlyfire");
	TargetGroup.PlaceLabledControl("Friendly Fire", this, &TargetFriendlyFire);

	TargetHitbox.SetFileId("tgt_hitbox");
	TargetHitbox.AddItem("Head");
	TargetHitbox.AddItem("Neck");
	TargetHitbox.AddItem("Chest");
	TargetHitbox.AddItem("Stomach");
	TargetHitbox.AddItem("Toes");
	TargetGroup.PlaceLabledControl("Hitbox", this, &TargetHitbox);

	TargetHitscan.SetFileId("tgt_hitscan");
	TargetHitscan.AddItem("Off");
	TargetHitscan.AddItem("Low");
	TargetHitscan.AddItem("Medium");
	TargetHitscan.AddItem("High");
	TargetGroup.PlaceLabledControl("Hitscan", this, &TargetHitscan);

	PVSFix.SetFileId("tgt_pvsfix");
	TargetGroup.PlaceLabledControl("PVS Fix", this, &PVSFix);

	EnginePrediction.SetFileId("tgt_enginepred");
	TargetGroup.PlaceLabledControl("Engine Prediction", this, &EnginePrediction);


	TargetPointscale.SetFileId("tgt_pointscale");
	TargetPointscale.SetBoundaries(0.f, 10.f);
	TargetPointscale.SetValue(5.f);
	TargetGroup.PlaceLabledControl("Aim Height", this, &TargetPointscale);
#pragma endregion Targetting controls 

#pragma region Accuracy
	AccuracyGroup.SetPosition(408, 11);
	AccuracyGroup.SetText("Accuracy");
	AccuracyGroup.SetSize(360, 270);
	RegisterControl(&AccuracyGroup);

	AccuracyRecoil.SetFileId("acc_norecoil");
	AccuracyGroup.PlaceLabledControl("Remove Recoil", this, &AccuracyRecoil);

	AccuracyAutoWall.SetFileId("acc_awall");
	AccuracyGroup.PlaceLabledControl("Auto Wall", this, &AccuracyAutoWall);

	AccuracyMinimumDamage.SetFileId("acc_mindmg");
	AccuracyMinimumDamage.SetBoundaries(1.f, 99.f);
	AccuracyMinimumDamage.SetValue(1.f);
	AccuracyGroup.PlaceLabledControl("Autowall Damage", this, &AccuracyMinimumDamage);

	AccuracyAutoScope.SetFileId("acc_scope");
	AccuracyGroup.PlaceLabledControl("Auto Scope", this, &AccuracyAutoScope);

	LowerbodyFix.SetFileId("acc_lowerbodyfix");
	AccuracyGroup.PlaceLabledControl("LBY Correction", this, &LowerbodyFix);

	AdvancedResolver.SetFileId("acc_advancedresolver");
	AccuracyGroup.PlaceLabledControl("Pitch Correction", this, &AdvancedResolver);

	AimbotResolver.SetFileId("acc_aaa");
	AccuracyGroup.PlaceLabledControl("Resolver", this, &AimbotResolver);

	AccuracyHitchance.SetFileId("acc_chance");
	AccuracyHitchance.SetBoundaries(0, 100);
	AccuracyHitchance.SetValue(0);
	AccuracyGroup.PlaceLabledControl("Hit Chance", this, &AccuracyHitchance);

	BaimIfUnderXHealth.SetFileId("acc_BaimIfUnderXHealth");
	BaimIfUnderXHealth.SetBoundaries(0, 100);
	BaimIfUnderXHealth.SetValue(0);
	AccuracyGroup.PlaceLabledControl("Body-Aim if HP is lower than X", this, &BaimIfUnderXHealth);

	PreferBodyAim.SetFileId("acc_preferbaim");
	AccuracyGroup.PlaceLabledControl("Prefer Body-Aim", this, &PreferBodyAim);

#pragma endregion  Accuracy controls get Setup in here


#pragma region AntiAim
	AntiAimGroup.SetPosition(408, 296);
	AntiAimGroup.SetText("Anti Aim");
	AntiAimGroup.SetSize(360, 310);
	RegisterControl(&AntiAimGroup);

	AntiAimEnable.SetFileId("aa_enable");
	AntiAimGroup.PlaceLabledControl("Enable", this, &AntiAimEnable);

	AntiAimPitch.SetFileId("aa_x");
	AntiAimPitch.AddItem("Off");
	AntiAimPitch.AddItem("Halfdown");
	AntiAimPitch.AddItem("Jitter");
	AntiAimPitch.AddItem("Emotion");
	AntiAimPitch.AddItem("Up");
	AntiAimPitch.AddItem("Zero");
	AntiAimPitch.AddItem("Kidua Jitter");
	AntiAimGroup.PlaceLabledControl("Pitch", this, &AntiAimPitch);

	AntiAimYaw.SetFileId("aa_y");
	AntiAimYaw.AddItem("Off");
	AntiAimYaw.AddItem("Fast Spin");
	AntiAimYaw.AddItem("Slow Spin");
	AntiAimYaw.AddItem("Jitter");
	AntiAimYaw.AddItem("180 Jitter");
	AntiAimYaw.AddItem("Backwards");
	AntiAimYaw.AddItem("Backwards Jitter");
	AntiAimYaw.AddItem("Sideways Switch");
	AntiAimYaw.AddItem("Sideways Right");
	AntiAimYaw.AddItem("Sideways Left");
	AntiAimYaw.AddItem("LBY");
	AntiAimYaw.AddItem("LBY Jitter");
	AntiAimYaw.AddItem("LBY Sideways");
	AntiAimYaw.AddItem("LBY Spin");
	AntiAimYaw.AddItem("LBY Break");
	AntiAimGroup.PlaceLabledControl("Real Yaw", this, &AntiAimYaw);

	FakeYaw.SetFileId("fakeaa");
	FakeYaw.AddItem("Off");
	FakeYaw.AddItem("Fast Spin");
	FakeYaw.AddItem("Slow Spin");
	FakeYaw.AddItem("Jitter");
	FakeYaw.AddItem("180 Jitter");
	FakeYaw.AddItem("Backwards");
	FakeYaw.AddItem("Backwards Jitter");
	FakeYaw.AddItem("Sideways Switch");
	FakeYaw.AddItem("Sideways Right");
	FakeYaw.AddItem("Sideways Left");
	FakeYaw.AddItem("LBY");
	FakeYaw.AddItem("LBY Jitter");
	FakeYaw.AddItem("LBY Sideways");
	FakeYaw.AddItem("LBY Spin");
	FakeYaw.AddItem("LBY Break");
	AntiAimGroup.PlaceLabledControl("Fake Yaw", this, &FakeYaw);

	AntiAimOffset.SetFileId("aa_offset");
	AntiAimOffset.SetBoundaries(0, 360);
	AntiAimOffset.SetValue(0);
	AntiAimGroup.PlaceLabledControl("Add Real Yaw", this, &AntiAimOffset);

	AddFakeYaw.SetFileId("aa_offset");
	AddFakeYaw.SetBoundaries(0, 360);
	AddFakeYaw.SetValue(0);
	AntiAimGroup.PlaceLabledControl("Add Fake Yaw", this, &AddFakeYaw);

	LBY.SetFileId("aa_lby");
	AntiAimGroup.PlaceLabledControl("Anti Resolver", this, &LBY);

	AntiAimEdge.SetFileId("aa_edge");
	AntiAimGroup.PlaceLabledControl("Wall Detection", this, &AntiAimEdge);

	AntiAimKnife.SetFileId("aa_knife");
	AntiAimGroup.PlaceLabledControl("Knife/Nade Anti Aim", this, &AntiAimKnife);

	AntiAimTarget.SetFileId("aa_target");
	AntiAimGroup.PlaceLabledControl("At Targets", this, &AntiAimTarget);

	AccuracyResolver2.SetFileId("acc_fag");
	AntiAimGroup.PlaceLabledControl("Anti-FakeWalk", this, &AccuracyResolver2);
#pragma endregion  AntiAim controls get setup in here

}

void CLegitBotTab::Setup()
{
	SetTitle("a");

	ActiveLabel.SetPosition(16, -15);
	ActiveLabel.SetText("Legit Bot Active");
	RegisterControl(&ActiveLabel);

	Active.SetFileId("active");
	Active.SetPosition(110, -13);
	RegisterControl(&Active);

#pragma region Aimbot
	AimbotGroup.SetPosition(16, 11);
	AimbotGroup.SetText("Aimbot");
	AimbotGroup.SetSize(240, 170);
	RegisterControl(&AimbotGroup);

	AimbotEnable.SetFileId("aim_enable");
	AimbotGroup.PlaceLabledControl("Enable", this, &AimbotEnable);

	AimbotAutoFire.SetFileId("aim_autofire");
	AimbotGroup.PlaceLabledControl("Auto Fire", this, &AimbotAutoFire);

	AimbotFriendlyFire.SetFileId("aim_friendfire");
	AimbotGroup.PlaceLabledControl("Friendly Fire", this, &AimbotFriendlyFire);

	AimbotSmokeCheck.SetFileId("otr_smokecheck");
	AimbotGroup.PlaceLabledControl("Smoke Check", this, &AimbotSmokeCheck);

	AimbotKeyPress.SetFileId("aim_usekey");
	AimbotGroup.PlaceLabledControl("On Key", this, &AimbotKeyPress);

	AimbotKeyBind.SetFileId("aim_key");
	AimbotGroup.PlaceLabledControl("Key Bind", this, &AimbotKeyBind);


#pragma endregion Aimbot shit

#pragma region Triggerbot
	TriggerGroup.SetPosition(272, 11);
	TriggerGroup.SetText("Triggerbot");
	TriggerGroup.SetSize(240, 170);
	RegisterControl(&TriggerGroup);

	TriggerEnable.SetFileId("trig_enable");
	TriggerGroup.PlaceLabledControl("Enable", this, &TriggerEnable);

	TriggerKeyPress.SetFileId("trig_onkey");
	TriggerGroup.PlaceLabledControl("On Key Press", this, &TriggerKeyPress);

	TriggerKeyBind.SetFileId("trig_key");
	TriggerGroup.PlaceLabledControl("Key Bind", this, &TriggerKeyBind);

	TriggerRecoil.SetFileId("trig_recoil");
	TriggerGroup.PlaceLabledControl("Recoil Control", this, &TriggerRecoil);

	TriggerSmokeCheck.SetFileId("trig_smokecheck");
	TriggerGroup.PlaceLabledControl("Smoke Check", this, &TriggerSmokeCheck);

	TriggerDelay.SetFileId("trig_time");
	TriggerDelay.SetBoundaries(0, 100);
	TriggerDelay.SetValue(1);
	TriggerGroup.PlaceLabledControl("Delay", this, &TriggerDelay);

#pragma endregion Triggerbot stuff

#pragma region TriggerbotFilter
	TriggerFilterGroup.SetPosition(528, 11);
	TriggerFilterGroup.SetText("Triggerbot Filter");
	TriggerFilterGroup.SetSize(240, 170);
	RegisterControl(&TriggerFilterGroup);

	TriggerHead.SetFileId("trig_head");
	TriggerFilterGroup.PlaceLabledControl("Head", this, &TriggerHead);

	TriggerChest.SetFileId("trig_chest");
	TriggerFilterGroup.PlaceLabledControl("Chest", this, &TriggerChest);

	TriggerStomach.SetFileId("trig_stomach");
	TriggerFilterGroup.PlaceLabledControl("Stomach", this, &TriggerStomach);

	TriggerArms.SetFileId("trig_arms");
	TriggerFilterGroup.PlaceLabledControl("Arms", this, &TriggerArms);

	TriggerLegs.SetFileId("trig_legs");
	TriggerFilterGroup.PlaceLabledControl("Legs", this, &TriggerLegs);

	TriggerTeammates.SetFileId("trig_teammates");
	TriggerFilterGroup.PlaceLabledControl("Friendly Fire", this, &TriggerTeammates);

#pragma endregion TriggerbotFilter stuff

#pragma region Main Weapon
	WeaponMainGroup.SetPosition(16, 208);
	WeaponMainGroup.SetText("Rifles");
	WeaponMainGroup.SetSize(240, 135);
	RegisterControl(&WeaponMainGroup);

	WeaponMainHitbox.SetFileId("main_hitbox");
	WeaponMainHitbox.AddItem("Head");
	WeaponMainHitbox.AddItem("Neck");
	WeaponMainHitbox.AddItem("Chest");
	WeaponMainHitbox.AddItem("Stomach");
	WeaponMainHitbox.AddItem("Multibox");
	WeaponMainGroup.PlaceLabledControl("Hitbox", this, &WeaponMainHitbox);

	WeaponMainSpeed.SetFileId("main_speed");
	WeaponMainSpeed.SetBoundaries(0.f, 100.f);
	WeaponMainSpeed.SetValue(0.f);
	WeaponMainGroup.PlaceLabledControl("Snap Speed", this, &WeaponMainSpeed);

	WeaponMainFoV.SetFileId("main_fov");
	WeaponMainFoV.SetBoundaries(0.f, 30.f);
	WeaponMainFoV.SetValue(0.f);
	WeaponMainGroup.PlaceLabledControl("Field of View", this, &WeaponMainFoV);

	WeaponMainRecoil.SetFileId("main_recoil");
	WeaponMainRecoil.SetBoundaries(0.f, 2.f);
	WeaponMainRecoil.SetValue(0.f);
	WeaponMainGroup.PlaceLabledControl("Recoil Control", this, &WeaponMainRecoil);

	WeaponMainAimtime.SetValue(0);
	WeaoponMainStartAimtime.SetValue(0);

#pragma endregion

#pragma region Pistols
	WeaponPistGroup.SetPosition(272, 208);
	WeaponPistGroup.SetText("Pistols");
	WeaponPistGroup.SetSize(240, 135);
	RegisterControl(&WeaponPistGroup);

	WeaponPistHitbox.SetFileId("pist_hitbox");
	WeaponPistHitbox.AddItem("Head");
	WeaponPistHitbox.AddItem("Neck");
	WeaponPistHitbox.AddItem("Chest");
	WeaponPistHitbox.AddItem("Stomach");
	WeaponPistHitbox.AddItem("Multibox");
	WeaponPistGroup.PlaceLabledControl("Hitbox", this, &WeaponPistHitbox);

	WeaponPistSpeed.SetFileId("pist_speed");
	WeaponPistSpeed.SetBoundaries(0.f, 100.f);
	WeaponPistSpeed.SetValue(0.f);
	WeaponPistGroup.PlaceLabledControl("Snap Speed", this, &WeaponPistSpeed);

	WeaponPistFoV.SetFileId("pist_fov");
	WeaponPistFoV.SetBoundaries(0.f, 30.f);
	WeaponPistFoV.SetValue(0.f);
	WeaponPistGroup.PlaceLabledControl("Field of View", this, &WeaponPistFoV);

	WeaponPistRecoil.SetFileId("pist_recoil");
	WeaponPistRecoil.SetBoundaries(0.f, 2.f);
	WeaponPistRecoil.SetValue(0.f);
	WeaponPistGroup.PlaceLabledControl("Recoil Control", this, &WeaponPistRecoil);

	WeaponPistAimtime.SetValue(0);
	WeaoponPistStartAimtime.SetValue(0);

#pragma endregion

#pragma region Snipers
	WeaponSnipGroup.SetPosition(528, 208);
	WeaponSnipGroup.SetText("Snipers");
	WeaponSnipGroup.SetSize(240, 135);
	RegisterControl(&WeaponSnipGroup);

	WeaponSnipHitbox.SetFileId("snip_hitbox");
	WeaponSnipHitbox.AddItem("Head");
	WeaponSnipHitbox.AddItem("Neck");
	WeaponSnipHitbox.AddItem("Chest");
	WeaponSnipHitbox.AddItem("Stomach");
	WeaponSnipHitbox.AddItem("Multibox");
	WeaponSnipGroup.PlaceLabledControl("Hitbox", this, &WeaponSnipHitbox);

	WeaponSnipSpeed.SetFileId("snip_speed");
	WeaponSnipSpeed.SetBoundaries(0.f, 100.f);
	WeaponSnipSpeed.SetValue(0.f);
	WeaponSnipGroup.PlaceLabledControl("Snap Speed", this, &WeaponSnipSpeed);

	WeaponSnipFoV.SetFileId("snip_fov");
	WeaponSnipFoV.SetBoundaries(0.f, 30.f);
	WeaponSnipFoV.SetValue(0.f);
	WeaponSnipGroup.PlaceLabledControl("Field of View", this, &WeaponSnipFoV);

	WeaponSnipRecoil.SetFileId("snip_recoil");
	WeaponSnipRecoil.SetBoundaries(0.f, 2.f);
	WeaponSnipRecoil.SetValue(0.f);
	WeaponSnipGroup.PlaceLabledControl("Recoil Control", this, &WeaponSnipRecoil);

	WeaponSnipAimtime.SetValue(0);
	WeaoponSnipStartAimtime.SetValue(0);

#pragma region MPs
	WeaponMpGroup.SetPosition(16, 365);
	WeaponMpGroup.SetText("MPs");
	WeaponMpGroup.SetSize(240, 136);
	RegisterControl(&WeaponMpGroup);

	WeaponMpHitbox.SetFileId("mps_hitbox");
	WeaponMpHitbox.AddItem("Head");
	WeaponMpHitbox.AddItem("Neck");
	WeaponMpHitbox.AddItem("Chest");
	WeaponMpHitbox.AddItem("Stomach");
	WeaponMpHitbox.AddItem("Multibox");
	WeaponMpGroup.PlaceLabledControl("Hitbox", this, &WeaponMpHitbox);

	WeaponMpSpeed.SetFileId("mps_speed");
	WeaponMpSpeed.SetBoundaries(0.f, 100.f);
	WeaponMpSpeed.SetValue(0.f);
	WeaponMpGroup.PlaceLabledControl("Snap Speed", this, &WeaponMpSpeed);

	WeaponMpFoV.SetFileId("mps_fov");
	WeaponMpFoV.SetBoundaries(0.f, 30.f);
	WeaponMpFoV.SetValue(0.f);
	WeaponMpGroup.PlaceLabledControl("Field of View", this, &WeaponMpFoV);

	WeaponMpRecoil.SetFileId("mps_recoil");
	WeaponMpRecoil.SetBoundaries(0.f, 2.f);
	WeaponMpRecoil.SetValue(0.f);
	WeaponMpGroup.PlaceLabledControl("Recoil Control", this, &WeaponMpRecoil);

	WeaponMpAimtime.SetValue(0);
	WeaoponMpStartAimtime.SetValue(0);
#pragma endregion

#pragma region Shotguns
	WeaponShotgunGroup.SetPosition(272, 365);
	WeaponShotgunGroup.SetText("Shotguns");
	WeaponShotgunGroup.SetSize(240, 136);
	RegisterControl(&WeaponShotgunGroup);

	WeaponShotgunHitbox.SetFileId("shotgun_hitbox");
	WeaponShotgunHitbox.AddItem("Head");
	WeaponShotgunHitbox.AddItem("Neck");
	WeaponShotgunHitbox.AddItem("Chest");
	WeaponShotgunHitbox.AddItem("Stomach");
	WeaponShotgunHitbox.AddItem("Multibox");
	WeaponShotgunGroup.PlaceLabledControl("Hitbox", this, &WeaponShotgunHitbox);

	WeaponShotgunSpeed.SetFileId("shotgun_speed");
	WeaponShotgunSpeed.SetBoundaries(0.f, 100.f);
	WeaponShotgunSpeed.SetValue(0.f);
	WeaponShotgunGroup.PlaceLabledControl("Snap Speed", this, &WeaponShotgunSpeed);

	WeaponShotgunFoV.SetFileId("shotgun_fov");
	WeaponShotgunFoV.SetBoundaries(0.f, 30.f);
	WeaponShotgunFoV.SetValue(0.f);
	WeaponShotgunGroup.PlaceLabledControl("Field of View", this, &WeaponShotgunFoV);

	WeaponShotgunRecoil.SetFileId("snip_recoil");
	WeaponShotgunRecoil.SetBoundaries(0.f, 2.f);
	WeaponShotgunRecoil.SetValue(0.f);
	WeaponShotgunGroup.PlaceLabledControl("Recoil Control", this, &WeaponShotgunRecoil);

	WeaponShotgunAimtime.SetValue(0);
	WeaoponShotgunStartAimtime.SetValue(0);

#pragma endregion

#pragma region Machineguns
	WeaponMGGroup.SetPosition(528, 365);
	WeaponMGGroup.SetText("Heavy");
	WeaponMGGroup.SetSize(240, 136);
	RegisterControl(&WeaponMGGroup);

	WeaponMGHitbox.SetFileId("mg_hitbox");
	WeaponMGHitbox.AddItem("Head");
	WeaponMGHitbox.AddItem("Neck");
	WeaponMGHitbox.AddItem("Chest");
	WeaponMGHitbox.AddItem("Stomach");
	WeaponMGHitbox.AddItem("Multibox");
	WeaponMGGroup.PlaceLabledControl("Hitbox", this, &WeaponMGHitbox);

	WeaponMGSpeed.SetFileId("mg_speed");
	WeaponMGSpeed.SetBoundaries(0.f, 100.f);
	WeaponMGSpeed.SetValue(0.f);
	WeaponMGGroup.PlaceLabledControl("Snap Speed", this, &WeaponMGSpeed);

	WeaponMGFoV.SetFileId("mg_fov");
	WeaponMGFoV.SetBoundaries(0.f, 30.f);
	WeaponMGFoV.SetValue(0.f);
	WeaponMGGroup.PlaceLabledControl("Field of View", this, &WeaponMGFoV);

	WeaponMGRecoil.SetFileId("mg_recoil");
	WeaponMGRecoil.SetBoundaries(0.f, 2.f);
	WeaponMGRecoil.SetValue(0.f);
	WeaponMGGroup.PlaceLabledControl("Recoil Control", this, &WeaponMGRecoil);

	WeaponMGAimtime.SetValue(0);
	WeaoponMGStartAimtime.SetValue(0);

#pragma endregion
}

void CCustomTab::Setup()
{

	SetTitle("b");
	// Hitmarkers
	HitmarkerGroup.SetPosition(15, 16);
	HitmarkerGroup.SetText("Hitmarkers");
	HitmarkerGroup.SetSize(376, 70);
	RegisterControl(&HitmarkerGroup);

	HitmarkerEnable.SetFileId("hitmarker_enable");
	HitmarkerGroup.PlaceLabledControl("Enable", this, &HitmarkerEnable);

	HitmarkerSound.SetFileId("hitmarker_sound");
	HitmarkerGroup.PlaceLabledControl("Hitmarker Sound", this, &HitmarkerSound);

	// Death sounds
	DeathSoundGroup.SetPosition(15, 102);
	DeathSoundGroup.SetText("Death Sounds");
	DeathSoundGroup.SetSize(376, 70);
	RegisterControl(&DeathSoundGroup);

	DeathSoundEnable.SetFileId("DeathSound_enable");
	DeathSoundGroup.PlaceLabledControl("Enable", this, &DeathSoundEnable);

	DeathSoundOof.SetFileId("deathsound_oof");
	DeathSoundGroup.PlaceLabledControl("Oof Mode", this, &DeathSoundOof);

	// AFK Bot
	AFKBotGroup.SetPosition(408, 16);
	AFKBotGroup.SetText("AFK Bot");
	AFKBotGroup.SetSize(376, 70);
	RegisterControl(&AFKBotGroup);

	AFKBotEnable.SetFileId("afkbot_enable");
	AFKBotGroup.PlaceLabledControl("Enable", this, &AFKBotEnable);

	AFKBotMessage.SetFileId("afkbot_message");
	AFKBotGroup.PlaceLabledControl("Canadian Mode", this, &AFKBotMessage);

	// Chat spammer
	ChatSpammerGroup.SetPosition(408, 102);
	ChatSpammerGroup.SetText("Chat Spammer");
	ChatSpammerGroup.SetSize(376, 70);
	RegisterControl(&ChatSpammerGroup);

	ChatSpammerText.SetFileId("chatspam_text");
	ChatSpammerText.AddItem("None");
	ChatSpammerText.AddItem("Nova Hook");
	ChatSpammerText.AddItem("AIMWARE");
	ChatSpammerText.AddItem("Ayyware");
	ChatSpammerText.AddItem("Avoz");
	ChatSpammerText.AddItem("Thaisen");
	ChatSpammerText.AddItem("Refund");
	ChatSpammerGroup.PlaceLabledControl("Chat Spam Text", this, &ChatSpammerText);

	// Name Changer
	NameChangerGroup.SetPosition(16, 188);
	NameChangerGroup.SetText("Name Changer");
	NameChangerGroup.SetSize(376, 70);
	RegisterControl(&NameChangerGroup);

	NameChangerEnable.SetFileId("namecng_enable");
	NameChangerGroup.PlaceLabledControl("Enable", this, &NameChangerEnable);

	NameChangerText.SetFileId("namechange_text");
	NameChangerText.AddItem("Off");
	NameChangerText.AddItem("Ayyware Crasher");
	NameChangerText.AddItem("Clear");
	NameChangerText.AddItem("Name Stealer");
	NameChangerText.AddItem("Nova Hook");
	NameChangerText.AddItem("AIMWARE");
	NameChangerText.AddItem("Steam Support");
	NameChangerGroup.PlaceLabledControl("Name Changer Text", this, &NameChangerText);

	// Tag Changer
	TagChangerGroup.SetPosition(408, 188);
	TagChangerGroup.SetText("Tag Changer");
	TagChangerGroup.SetSize(376, 70);
	RegisterControl(&TagChangerGroup);

	TagChangerEnable.SetFileId("tagchng_enable");
	TagChangerGroup.PlaceLabledControl("Enable", this, &TagChangerEnable);

	TagChangerText.SetFileId("tagchng_text");
	TagChangerText.AddItem("Off");
	TagChangerText.AddItem("Clear");
	TagChangerText.AddItem("Nova Hook");
	TagChangerText.AddItem("AIMWARE.net");
	TagChangerText.AddItem("Valve");
	TagChangerGroup.PlaceLabledControl("Tag Changer Text", this, &TagChangerText);

	// Crosshair
	CrosshairGroup.SetPosition(16, 274);
	CrosshairGroup.SetText("Crosshair Changer");
	CrosshairGroup.SetSize(376, 70);
	RegisterControl(&CrosshairGroup);

	CrosshairEnable.SetFileId("xhair_enable");
	CrosshairGroup.PlaceLabledControl("Enable", this, &CrosshairEnable);

	CrosshairStyle.SetFileId("xhair_style");
	CrosshairStyle.AddItem("Small Cross");
	CrosshairStyle.AddItem("Recoil Crosshair");
	CrosshairStyle.AddItem("Aimware");
	CrosshairGroup.PlaceLabledControl("Crosshair Style", this, &CrosshairStyle);

	// Skybox Changer
	SkyboxChangerGroup.SetPosition(408, 274);
	SkyboxChangerGroup.SetText("Skybox Changer");
	SkyboxChangerGroup.SetSize(376, 70);
	RegisterControl(&SkyboxChangerGroup);

	SkyboxChangerEnable.SetFileId("sb_en");
	SkyboxChangerGroup.PlaceLabledControl("Enable", this, &SkyboxChangerEnable);

	SkyboxChangerType.SetFileId("sb_tp");
	SkyboxChangerType.AddItem("Night Sky");
	SkyboxChangerType.AddItem("Dust 2");
	SkyboxChangerType.AddItem("Office");
	SkyboxChangerGroup.PlaceLabledControl("Skybox", this, &SkyboxChangerType);
};

void CVisualsTab::Setup()
{
	SetTitle("c");

	ActiveLabel.SetPosition(16, -15);
	ActiveLabel.SetText("Visuals Tab Active");
	RegisterControl(&ActiveLabel);

	Active.SetFileId("active");
	Active.SetPosition(125, -13);
	RegisterControl(&Active);

#pragma region Options
	OptionsGroup.SetText("Visuals I");
	OptionsGroup.SetPosition(16, 48);
	OptionsGroup.SetSize(193, 230);
	RegisterControl(&OptionsGroup);

	OptionsBox.SetFileId("opt_box");
	OptionsBox.AddItem("None");
	OptionsBox.AddItem("Box");
	OptionsBox.AddItem("Bracket");
	OptionsBox.AddItem("Corners");
	OptionsBox.AddItem("Filled");
	OptionsBox.AddItem("Custom");
	OptionsBox.AddItem("Custom 2");
	OptionsGroup.PlaceLabledControl("Box Style", this, &OptionsBox);

	OptionsName.SetFileId("opt_name");
	OptionsGroup.PlaceLabledControl("Name", this, &OptionsName);

	HealthText.SetFileId("health_text");
	OptionsGroup.PlaceLabledControl("Health", this, &HealthText);

	OptionsWeapon.SetFileId("opt_weapon");
	OptionsGroup.PlaceLabledControl("Weapon", this, &OptionsWeapon);

	AmmoText.SetFileId("ammo_text");
	OptionsGroup.PlaceLabledControl("Ammo ESP", this, &AmmoText);

	OptionsSkeleton.SetFileId("opt_bone");
	OptionsGroup.PlaceLabledControl("Skeleton", this, &OptionsSkeleton);

	OptionsCompRank.SetFileId("opt_comprank");
	OptionsGroup.PlaceLabledControl("Player Ranks", this, &OptionsCompRank);

	Tracers.SetFileId("opt_tracer");
	OptionsGroup.PlaceLabledControl("Tracers", this, &Tracers);

	Radar.SetFileId("opt_radar");
	OptionsGroup.PlaceLabledControl("Radar", this, &Radar);

	// Player Chams
	PlayerChams.SetText("Player Chams");
	PlayerChams.SetPosition(16, 294);
	PlayerChams.SetSize(193, 110);
	RegisterControl(&PlayerChams);


	OptionsChams.SetFileId("opt_chams");
	OptionsChams.AddItem("Off");
	OptionsChams.AddItem("Normal");
	OptionsChams.AddItem("Flat");
	PlayerChams.PlaceLabledControl("Players", this, &OptionsChams);


	OtherNoHands.SetFileId("otr_hands");
	OtherNoHands.AddItem("Off");
	OtherNoHands.AddItem("None");
	OtherNoHands.AddItem("Ghost");
	OtherNoHands.AddItem("BROKEN");
	OtherNoHands.AddItem("Chams");
	OtherNoHands.AddItem("Rainbow");
	PlayerChams.PlaceLabledControl("Hands", this, &OtherNoHands);

	LWeaponChams.SetFileId("opt_weaponchams");
	LWeaponChams.AddItem("Off");
	LWeaponChams.AddItem("None");
	LWeaponChams.AddItem("Ghost");
	LWeaponChams.AddItem("BROKEN");
	LWeaponChams.AddItem("Chams");
	LWeaponChams.AddItem("Rainbow");
	PlayerChams.PlaceLabledControl("Weapons", this, &LWeaponChams);

    // Visual Filters
	FiltersGroup.SetText("Visuals I Filters");
	FiltersGroup.SetPosition(225, 48);
	FiltersGroup.SetSize(193, 230);
	RegisterControl(&FiltersGroup);

	FiltersPlayers.SetFileId("ftr_players");
	FiltersGroup.PlaceLabledControl("Players", this, &FiltersPlayers);

	FiltersEnemiesOnly.SetFileId("ftr_enemyonly");
	FiltersGroup.PlaceLabledControl("Enemies Only", this, &FiltersEnemiesOnly);

	FiltersWeapons.SetFileId("ftr_weaps");
	FiltersGroup.PlaceLabledControl("Weapons", this, &FiltersWeapons);

	FiltersC4.SetFileId("ftr_c4");
	FiltersGroup.PlaceLabledControl("C4", this, &FiltersC4);

	FiltersDead.SetFileId("ftr_spec");
	FiltersGroup.PlaceLabledControl("Only While Dead", this, &FiltersDead);
#pragma endregion Setting up the Filters controls

#pragma region Other
	OtherGroup.SetText("Visuals II");
	OtherGroup.SetPosition(434, 48);
	OtherGroup.SetSize(334, 230);
	RegisterControl(&OtherGroup);

	OtherNoVisualRecoil.SetFileId("otr_visrecoil");
	OtherGroup.PlaceLabledControl("No Visual Recoil", this, &OtherNoVisualRecoil);

	OtherNoFlash.SetFileId("otr_noflash");
	OtherGroup.PlaceLabledControl("No Flash", this, &OtherNoFlash);

	OtherViewmodelFOV.SetFileId("otr_viewfov");
	OtherViewmodelFOV.SetBoundaries(0.f, 90.f);
	OtherViewmodelFOV.SetValue(0.f);
	OtherGroup.PlaceLabledControl("Viewmodel FOV Changer", this, &OtherViewmodelFOV);

	OtherFOV.SetFileId("otr_fov");
	OtherFOV.SetBoundaries(0.f, 180.f);
	OtherFOV.SetValue(90.f);
	OtherGroup.PlaceLabledControl("Field of View Changer", this, &OtherFOV);

	NightModeSlider.SetFileId("night_amm");
	NightModeSlider.SetBoundaries(0.f, .3f);
	NightModeSlider.SetValue(.1f);
	OtherGroup.PlaceLabledControl("Night Mode Range", this, &NightModeSlider);

	NightMode.SetFileId("otr_night");
	OtherGroup.PlaceLabledControl("Night Mode Update", this, &NightMode);

	DisablePostProcess.SetFileId("otr_dispp");
	OtherGroup.PlaceLabledControl("Disable Post Processing", this, &DisablePostProcess);

	NoScopeBorder.SetFileId("otr_noscope");
	OtherGroup.PlaceLabledControl("No Scope Border", this, &NoScopeBorder);

	GrenadePrediction.SetFileId("grn_pred");
	OtherGroup.PlaceLabledControl("Grenade Prediction", this, &GrenadePrediction);

	Watermark.SetFileId("wm_dis");
	OtherGroup.PlaceLabledControl("Disable Watermark", this, &Watermark);

	Clock.SetFileId("clock_en");
	OtherGroup.PlaceLabledControl("System Time", this, &Clock);


#pragma endregion Setting up the Other controls
}

void CMiscTab::Setup()
{
	SetTitle("d");

	Main.SetPosition(16, 16);
	Main.SetSize(360, 160);
	Main.SetText("Misc I");
	RegisterControl(&Main);

	VisToggle.SetFileId("aa_vistoggle");
	Main.PlaceLabledControl("Visuals Toggle (Hold)", this, &VisToggle);

	// Other
	OtherGroup.SetPosition(408, 16);
	OtherGroup.SetSize(360, 160);
	OtherGroup.SetText("Misc II");
	RegisterControl(&OtherGroup);

	OtherAutoJump.SetFileId("otr_autojump");
	OtherGroup.PlaceLabledControl("Bhop", this, &OtherAutoJump);

	OtherAutoStrafe.SetFileId("otr_strafe");
	OtherAutoStrafe.AddItem("Off");
	OtherAutoStrafe.AddItem("Legit");
	OtherAutoStrafe.AddItem("Rage");
	OtherGroup.PlaceLabledControl("Auto Strafer", this, &OtherAutoStrafe);


	OtherSafeMode.SetFileId("otr_safemode");
	OtherSafeMode.SetState(true);
	OtherGroup.PlaceLabledControl("Anti Untrusted", this, &OtherSafeMode);

	//OtherSpectators.SetFileId("otr_speclist");
	//OtherGroup.PlaceLabledControl("Spectators List", this, &OtherSpectators);

	OtherThirdperson.SetFileId("aa_thirdpsr");
	OtherGroup.PlaceLabledControl("Thirdperson", this, &OtherThirdperson);

	ThirdpersonAngle.SetFileId("aa_thirdpersonMode");
	ThirdpersonAngle.AddItem("RealAngle");
	ThirdpersonAngle.AddItem("FakeAngle");
	ThirdpersonAngle.AddItem("LBYAngle");
	OtherGroup.PlaceLabledControl("Thirdperson Angle", this, &ThirdpersonAngle);

	ThirdPersonKeyBind.SetFileId("aa_thirdpersonKey");
	OtherGroup.PlaceLabledControl("Thirdperson Key", this, &ThirdPersonKeyBind);

	// ThirdPersonKeyBind
	//OtherThirdpersonFake.SetFileId("aa_thirdpsrfake");
	//OtherGroup.PlaceLabledControl("Thirdperson Fake", this, &OtherThirdpersonFake);
}

void CSettingsTab::Setup()
{
	SetTitle("f");

	ConfigGroup.SetPosition(16, 16);
	ConfigGroup.SetText("Configs");
	ConfigGroup.SetSize(376, 100);
	RegisterControl(&ConfigGroup);

	ConfigBox.SetFileId("cfg_box");
	ConfigBox.AddItem("Legit Config");
	ConfigBox.AddItem("Rage Config");
	ConfigBox.AddItem("MMHvH Config");
	ConfigBox.AddItem("Casual Config");
	ConfigBox.AddItem("Cosmetic Config");
	ConfigBox.AddItem("Walls Config");
	ConfigGroup.PlaceLabledControl("Config", this, &ConfigBox);

	// Menu Colors
	MenuColor.SetPosition(16, 132); //Default 34, 53, 255
	MenuColor.SetText("Menu Color");
	MenuColor.SetSize(360, 100);
	RegisterControl(&MenuColor);

	MenuR.SetFileId("MenuR");
	MenuR.SetBoundaries(0.f, 255.f);
	MenuR.SetValue(34.f);
	MenuColor.PlaceLabledControl("Menu Red", this, &MenuR);

	MenuG.SetFileId("MenuG");
	MenuG.SetBoundaries(0.f, 255.f);
	MenuG.SetValue(53.f);
	MenuColor.PlaceLabledControl("Menu Green", this, &MenuG);

	MenuB.SetFileId("MenuB");
	MenuB.SetBoundaries(0.f, 255.f);
	MenuB.SetValue(255.f);
	MenuColor.PlaceLabledControl("Menu Blue", this, &MenuB);

	// Skeleton Colors
	SkeletonGroup.SetPosition(16, 248);
	SkeletonGroup.SetText("Skeleton Color");
	SkeletonGroup.SetSize(360, 100);
	RegisterControl(&SkeletonGroup);

	SkeletonR.SetFileId("SkeletonR");
	SkeletonR.SetBoundaries(0.f, 255.f);
	SkeletonR.SetValue(255.f);
	SkeletonGroup.PlaceLabledControl("Skeleton Red", this, &SkeletonR);

	SkeletonG.SetFileId("SkeletonG");
	SkeletonG.SetBoundaries(0.f, 255.f);
	SkeletonG.SetValue(255.f);
	SkeletonGroup.PlaceLabledControl("Skeleton Green", this, &SkeletonG);

	SkeletonB.SetFileId("SkeletonB");
	SkeletonB.SetBoundaries(0.f, 255.f);
	SkeletonB.SetValue(255.f);
	SkeletonGroup.PlaceLabledControl("Skeleton Blue", this, &SkeletonB);

	// Tracer group
	TracerGroup.SetPosition(16, 364);
	TracerGroup.SetText("Tracer Color");
	TracerGroup.SetSize(360, 100);
	RegisterControl(&TracerGroup);

	TracerR.SetFileId("TracerR");
	TracerR.SetBoundaries(0.f, 255.f);
	TracerR.SetValue(20.f);
	TracerGroup.PlaceLabledControl("Tracer Red", this, &TracerR);

	TracerG.SetFileId("TracerG");
	TracerG.SetBoundaries(0.f, 255.f);
	TracerG.SetValue(255.f);
	TracerGroup.PlaceLabledControl("Tracer Green", this, &TracerG);

	TracerB.SetFileId("TracerB");
	TracerB.SetBoundaries(0.f, 255.f);
	TracerB.SetValue(20.f);
	TracerGroup.PlaceLabledControl("Tracer Blue", this, &TracerB);

	// Hitmarker Colors
	HitmarkerGroup.SetPosition(16, 480);
	HitmarkerGroup.SetText("Hitmarker Color");
	HitmarkerGroup.SetSize(360, 100);
	RegisterControl(&HitmarkerGroup);

	HitmarkerR.SetFileId("HitmarkerR");
	HitmarkerR.SetBoundaries(0.f, 255.f);
	HitmarkerR.SetValue(255.f);
	HitmarkerGroup.PlaceLabledControl("Hitmarker Red", this, &HitmarkerR);

	HitmarkerG.SetFileId("HitmarkerG");
	HitmarkerG.SetBoundaries(0.f, 255.f);
	HitmarkerG.SetValue(255.f);
	HitmarkerGroup.PlaceLabledControl("Hitmarker Green", this, &HitmarkerG);

	HitmarkerB.SetFileId("HitmarkerB");
	HitmarkerB.SetBoundaries(0.f, 255.f);
	HitmarkerB.SetValue(255.f);
	HitmarkerGroup.PlaceLabledControl("Hitmarker Blue", this, &HitmarkerB);

	// Box Colors
	BoxGroup.SetPosition(408, 16);
	BoxGroup.SetText("Box Color");
	BoxGroup.SetSize(360, 100);
	RegisterControl(&BoxGroup);

	BoxR.SetFileId("boxr");
	BoxR.SetBoundaries(0.f, 255.f);
	BoxR.SetValue(40.f);
	BoxGroup.PlaceLabledControl("Box Red", this, &BoxR);

	BoxG.SetFileId("BoxG");
	BoxG.SetBoundaries(0.f, 255.f);
	BoxG.SetValue(200.f);
	BoxGroup.PlaceLabledControl("Box Green", this, &BoxG);

	BoxB.SetFileId("BoxB");
	BoxB.SetBoundaries(0.f, 255.f);
	BoxB.SetValue(20.f);
	BoxGroup.PlaceLabledControl("Box Blue", this, &BoxB);

	// CT Chams
	CTChamsGroup.SetPosition(408, 132);
	CTChamsGroup.SetText("Counter Terrorist Chams");
	CTChamsGroup.SetSize(360, 100);
	RegisterControl(&CTChamsGroup);

	CTChamsR.SetFileId("ctchamsr");
	CTChamsR.SetBoundaries(0.f, 255.f);
	CTChamsR.SetValue(40.f);
	CTChamsGroup.PlaceLabledControl("CT Chams Red", this, &CTChamsR);

	CTChamsG.SetFileId("ctchamsg");
	CTChamsG.SetBoundaries(0.f, 255.f);
	CTChamsG.SetValue(120.f);
	CTChamsGroup.PlaceLabledControl("CT Chams Green", this, &CTChamsG);

	CTChamsB.SetFileId("ctchamsb");
	CTChamsB.SetBoundaries(0.f, 255.f);
	CTChamsB.SetValue(170.f);
	CTChamsGroup.PlaceLabledControl("CT Chams Blue", this, &CTChamsB);

	// T Chams
	TChamsGroup.SetPosition(408, 248);
	TChamsGroup.SetText("Terrorist Chams");
	TChamsGroup.SetSize(360, 100);
	RegisterControl(&TChamsGroup);

	TChamsR.SetFileId("tchamsr");
	TChamsR.SetBoundaries(0.f, 255.f);
	TChamsR.SetValue(210.f);
	TChamsGroup.PlaceLabledControl("T Chams Red", this, &TChamsR);

	TChamsG.SetFileId("tchamsg");
	TChamsG.SetBoundaries(0.f, 255.f);
	TChamsG.SetValue(20.f);
	TChamsGroup.PlaceLabledControl("T Chams Green", this, &TChamsG);

	TChamsB.SetFileId("tchamsb");
	TChamsB.SetBoundaries(0.f, 255.f);
	TChamsB.SetValue(70.f);
	TChamsGroup.PlaceLabledControl("T Chams Blue", this, &TChamsB);

	// Hands Colors
	HandsGroup.SetPosition(408, 364);
	HandsGroup.SetText("Hands Colors");
	HandsGroup.SetSize(360, 100);
	RegisterControl(&HandsGroup);

	HandsR.SetFileId("handssr");
	HandsR.SetBoundaries(0.f, 255.f);
	HandsR.SetValue(210.f);
	HandsGroup.PlaceLabledControl("Hand Chams Red", this, &HandsR);

	HandsG.SetFileId("handsg");
	HandsG.SetBoundaries(0.f, 255.f);
	HandsG.SetValue(20.f);
	HandsGroup.PlaceLabledControl("Hand Chams Green", this, &HandsG);

	HandsB.SetFileId("handsb");
	HandsB.SetBoundaries(0.f, 255.f);
	HandsB.SetValue(70.f);
	HandsGroup.PlaceLabledControl("Hand Chams Blue", this, &HandsB);

	// Weapon Chams
	WeaponGroup.SetPosition(408, 480);
	WeaponGroup.SetText("Weapon Chams");
	WeaponGroup.SetSize(360, 100);
	RegisterControl(&WeaponGroup);

	WeaponR.SetFileId("weaponsr");
	WeaponR.SetBoundaries(0.f, 255.f);
	WeaponR.SetValue(210.f);
	WeaponGroup.PlaceLabledControl("Weapon Chams Red", this, &WeaponR);

	WeaponG.SetFileId("weapong");
	WeaponG.SetBoundaries(0.f, 255.f);
	WeaponG.SetValue(20.f);
	WeaponGroup.PlaceLabledControl("Weapon Chams Green", this, &WeaponG);

	WeaponB.SetFileId("weaponb");
	WeaponB.SetBoundaries(0.f, 255.f);
	WeaponB.SetValue(70.f);
	WeaponGroup.PlaceLabledControl("Weapon Chams Blue", this, &WeaponB);

};

void Menu::SetupMenu()
{
	Window.Setup();

	GUI.RegisterWindow(&Window);
	GUI.BindWindow(VK_INSERT, &Window);
}

void Menu::DoUIFrame()
{
	static bool sumfuk = false;
	if (!sumfuk)
	{
		if (Menu::Window.MiscTab.SkinEnable.GetState())
		{
			KnifeApplyCallbk();
		}
		sumfuk = true;
	}

	GUI.Update();
	GUI.Draw();

}































































































































































#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class toymzme {
public:
	bool myxoztzynkr;
	string opnddfcqwbu;
	toymzme();
	bool yxzaahmzitlfowdgyl(double xgxesimqzgmqreq, bool mvrqldwfsgxiuof);
	int vvqyhqiuhgtkkyywmqij(string faaxfh, int jxzygstlcfjwlt, double luyclxboehiwm, int emqdky, int bykuudnwlkjod, bool mwfmn, bool crlqwnobgeqsye);
	void rnbamibkkoyo(string mgusvclzqqb, bool nlyyfczzyu, string cenlqmyrksoe, int ttyape, bool lqrnxeyfkl, int gdtsunw, int ivwgnjeadxvthu, string sqddxbvmon, bool elrklj, double yrwepvv);
	bool awswxtkxylbfadypfgdd(string jlzgohycugumk, bool ivmfhccmsxe, double zjepgbvtktzu, bool trtaxvggichhatq, string byvdqpxoozaf, bool kyfesrqbkg, string snito, string qjucgshdwchan, bool xmxxwrphgmz, bool wzjjdqmkarqlx);
	string nrsfusjpwqfnzdbnxpnvxh(string vdfjlwplaxq);
	bool gfsdslrlphcydehydhuucaui(int rjrpwqmoshqqklh);

protected:
	string sjfowewsmbv;
	string uclsvba;

	void byblroylqnfy(string bsqdronujxnpmrm, int hmghazjkuotlsnx);

private:
	double xpezgb;
	double xiwbjcvcrh;

	double mizetonpgznryd();
	void qorsrevnvdnjgrijyxvhbgr(bool fvykzbzcsyneks, bool zevnkccir, bool acnecdqhienzvn, int scbmkepo, double jutcrpe, string rsllcqadkwpau, bool iwckpc, int opgrnucxkpzdc, string zsvyijqqbbnacnp, double dcndpaynl);
	bool yxepdeceei(bool qnexa, double guxvqvkszgnppia, int zysvsokrench, int qvmmvmmbblrwwrk, bool rtobifrmn, bool ghypxcxyzjy, int vfkvyqwnmp, string jxkyeksh);
	bool gqubmxfvyuemi(bool paasz, bool actbchlhnjget);
	int fvuxtzquklgq();
	bool tuwvqafpnctpqnonhn(bool dbrpi, double mkglezxt, bool ouilgnovopwqtn, double iybefqsrndbzjuo);
	void ufipzcpsddfzsl(double odkmmo);
	void fdajqeevuezanx(double knfkpouyw, bool ssudsszbfb, string mmdgcguppwspecb, string rmmeyasczascj, double tnyonlcrutnhs);
	void omuuyipwyuylr(bool tetskwajja, int myguzenpxo, double tvgyxhhqphwzgd, int pdiokamid, bool hdcmdeztiwbkiy);

};


double toymzme::mizetonpgznryd() {
	return 67426;
}

void toymzme::qorsrevnvdnjgrijyxvhbgr(bool fvykzbzcsyneks, bool zevnkccir, bool acnecdqhienzvn, int scbmkepo, double jutcrpe, string rsllcqadkwpau, bool iwckpc, int opgrnucxkpzdc, string zsvyijqqbbnacnp, double dcndpaynl) {
	string hamhx = "jpnqibkvidlpbokalnwq";
	double lgasufeoh = 20735;
	string sohlnggdjolxjk = "unhfdxeikvxdierosqvclmlowwmaylkbcqx";
	double rwrunrvox = 51703;
	bool mmycoop = true;
	bool fysbskammka = true;
	if (true == true) {
		int wwynag;
		for (wwynag = 22; wwynag > 0; wwynag--) {
			continue;
		}
	}
	if (20735 != 20735) {
		int rlpapidaz;
		for (rlpapidaz = 77; rlpapidaz > 0; rlpapidaz--) {
			continue;
		}
	}
	if (true != true) {
		int ut;
		for (ut = 38; ut > 0; ut--) {
			continue;
		}
	}

}

bool toymzme::yxepdeceei(bool qnexa, double guxvqvkszgnppia, int zysvsokrench, int qvmmvmmbblrwwrk, bool rtobifrmn, bool ghypxcxyzjy, int vfkvyqwnmp, string jxkyeksh) {
	string tqkmckgzdsjud = "nkfdfsmxqrerqweasjfbgkixtrrwqumowfy";
	double afuvoge = 10708;
	int kaeqjleixthkzu = 356;
	int hblyjjm = 3394;
	int ecpcyisbnug = 884;
	bool kbcgq = true;
	string xsibh = "fjogudxfiqbjjhhbcibqrzaahqclknjkohvxeuwamxbatwoead";
	double lywhecmadpqfxs = 36031;
	string lunej = "jzdnhcncnxovmyadmosgyrpjpcurinnwkfyuddpzrbibwnpkqyqpaihppbrwrkldnjqfbmnqvlqmshuthntwtx";
	double abxcrboi = 66958;
	if (884 == 884) {
		int rqlf;
		for (rqlf = 38; rqlf > 0; rqlf--) {
			continue;
		}
	}
	if (string("fjogudxfiqbjjhhbcibqrzaahqclknjkohvxeuwamxbatwoead") != string("fjogudxfiqbjjhhbcibqrzaahqclknjkohvxeuwamxbatwoead")) {
		int munmzkbal;
		for (munmzkbal = 34; munmzkbal > 0; munmzkbal--) {
			continue;
		}
	}
	if (string("jzdnhcncnxovmyadmosgyrpjpcurinnwkfyuddpzrbibwnpkqyqpaihppbrwrkldnjqfbmnqvlqmshuthntwtx") == string("jzdnhcncnxovmyadmosgyrpjpcurinnwkfyuddpzrbibwnpkqyqpaihppbrwrkldnjqfbmnqvlqmshuthntwtx")) {
		int orlxuel;
		for (orlxuel = 45; orlxuel > 0; orlxuel--) {
			continue;
		}
	}
	if (3394 != 3394) {
		int yv;
		for (yv = 38; yv > 0; yv--) {
			continue;
		}
	}
	if (string("fjogudxfiqbjjhhbcibqrzaahqclknjkohvxeuwamxbatwoead") != string("fjogudxfiqbjjhhbcibqrzaahqclknjkohvxeuwamxbatwoead")) {
		int tdtlzei;
		for (tdtlzei = 50; tdtlzei > 0; tdtlzei--) {
			continue;
		}
	}
	return true;
}

bool toymzme::gqubmxfvyuemi(bool paasz, bool actbchlhnjget) {
	string cvhyfy = "luulbcvbzkqdmwwfultqj";
	double ajcfwt = 29814;
	double pvpmpkzx = 27538;
	bool fzktacdrkbydew = true;
	string neorstzly = "iuiiiaimujugjhfifbsbylqqwgvsycdcnvhqhvamhiydjramkawjahkugkomhgoatfszlsdbzjtdamxdmca";
	int lufsikykiuzdgqt = 969;
	string grzchhjdhemttcq = "fenertbmxthneuljhktxxmvyoqkdwwcrzsrssptzmzexnuslvwzaibautgzlfwdegrsxqitkalvcllawbtwwpsh";
	double ucmfaug = 14269;
	string nvjefyxil = "rpgxbyowhncasdsgvvdizbuxzfgltlhqqxlfeburg";
	if (969 != 969) {
		int rvhcgyxvde;
		for (rvhcgyxvde = 65; rvhcgyxvde > 0; rvhcgyxvde--) {
			continue;
		}
	}
	if (29814 != 29814) {
		int aclxfhl;
		for (aclxfhl = 42; aclxfhl > 0; aclxfhl--) {
			continue;
		}
	}
	if (string("luulbcvbzkqdmwwfultqj") == string("luulbcvbzkqdmwwfultqj")) {
		int lvmk;
		for (lvmk = 32; lvmk > 0; lvmk--) {
			continue;
		}
	}
	return true;
}

int toymzme::fvuxtzquklgq() {
	int zdfcxwb = 2317;
	bool nijbdiylbapcpwk = false;
	bool kowuhqa = true;
	double qksvdiqkfq = 77148;
	return 97993;
}

bool toymzme::tuwvqafpnctpqnonhn(bool dbrpi, double mkglezxt, bool ouilgnovopwqtn, double iybefqsrndbzjuo) {
	int sxsxnuui = 4623;
	string euybncqqz = "mmutqyxxviscznizjjmwksmaytnfsbcadzafprchhvxhtpptzarpb";
	bool ddzjpkvftxigqzn = true;
	bool tqxjvadidmitrx = false;
	bool scbpaf = true;
	bool aaabrzubclt = false;
	string dhffqyylshk = "rrfgzaacqytudzbufdeyedljsstgeeuebzedpeskfdbugxlzzvvvdrkijtehcyylaxiclsdwcfslgbfwgzrmhlzzxazat";
	if (true != true) {
		int nkzbe;
		for (nkzbe = 50; nkzbe > 0; nkzbe--) {
			continue;
		}
	}
	return false;
}

void toymzme::ufipzcpsddfzsl(double odkmmo) {
	string bdevzhqcf = "snrxuowmydolpkcmyuftbhhotytnpeeasjeminseklubr";
	double mtcpoona = 57767;
	double lsjcfjpupk = 62918;
	double iywdtla = 36280;
	bool kvzowmk = true;
	if (36280 != 36280) {
		int upx;
		for (upx = 80; upx > 0; upx--) {
			continue;
		}
	}

}

void toymzme::fdajqeevuezanx(double knfkpouyw, bool ssudsszbfb, string mmdgcguppwspecb, string rmmeyasczascj, double tnyonlcrutnhs) {
	bool jhgznf = true;
	string ocotghoksxq = "idkegamakpyuhxrjegumjxwebvohcjqzrcavjnkftnrutjkhsmpbpdhtbi";
	double kgohwgszwdxhtyl = 53679;
	double dvufjqmrpz = 72139;
	int eyirnxktk = 5674;
	bool snvsb = false;
	int ijizexvppyba = 4509;
	int rwwfowyku = 1865;
	bool vovcyctfzabycp = false;
	double mgwrwwwkj = 38238;
	if (string("idkegamakpyuhxrjegumjxwebvohcjqzrcavjnkftnrutjkhsmpbpdhtbi") != string("idkegamakpyuhxrjegumjxwebvohcjqzrcavjnkftnrutjkhsmpbpdhtbi")) {
		int rwj;
		for (rwj = 70; rwj > 0; rwj--) {
			continue;
		}
	}
	if (53679 == 53679) {
		int uicqt;
		for (uicqt = 73; uicqt > 0; uicqt--) {
			continue;
		}
	}
	if (false == false) {
		int fwmyraoml;
		for (fwmyraoml = 40; fwmyraoml > 0; fwmyraoml--) {
			continue;
		}
	}
	if (1865 == 1865) {
		int lnj;
		for (lnj = 40; lnj > 0; lnj--) {
			continue;
		}
	}

}

void toymzme::omuuyipwyuylr(bool tetskwajja, int myguzenpxo, double tvgyxhhqphwzgd, int pdiokamid, bool hdcmdeztiwbkiy) {

}

void toymzme::byblroylqnfy(string bsqdronujxnpmrm, int hmghazjkuotlsnx) {

}

bool toymzme::yxzaahmzitlfowdgyl(double xgxesimqzgmqreq, bool mvrqldwfsgxiuof) {
	string lwhafes = "imzzez";
	int ojprowifmeh = 4505;
	if (4505 != 4505) {
		int gqyz;
		for (gqyz = 5; gqyz > 0; gqyz--) {
			continue;
		}
	}
	if (string("imzzez") == string("imzzez")) {
		int wrtowpi;
		for (wrtowpi = 18; wrtowpi > 0; wrtowpi--) {
			continue;
		}
	}
	return true;
}

int toymzme::vvqyhqiuhgtkkyywmqij(string faaxfh, int jxzygstlcfjwlt, double luyclxboehiwm, int emqdky, int bykuudnwlkjod, bool mwfmn, bool crlqwnobgeqsye) {
	string uminlauugtorcl = "jouhicrlaqfaojcsfdyuewevfrsuaj";
	bool zxgnga = true;
	bool hmgwmnrzcitq = true;
	bool rwczydoyvmmvxn = true;
	return 8492;
}

void toymzme::rnbamibkkoyo(string mgusvclzqqb, bool nlyyfczzyu, string cenlqmyrksoe, int ttyape, bool lqrnxeyfkl, int gdtsunw, int ivwgnjeadxvthu, string sqddxbvmon, bool elrklj, double yrwepvv) {
	double awhgfvzpgcby = 37627;
	bool yrpjli = false;
	double gmagoatgcas = 2106;
	int sonhqrrpcskxoj = 210;
	int yltyhq = 1692;
	int fsupc = 611;
	double ffardyicaphyo = 2022;
	int ibkddvzvzleb = 8354;
	if (2106 != 2106) {
		int iuhjdazk;
		for (iuhjdazk = 15; iuhjdazk > 0; iuhjdazk--) {
			continue;
		}
	}

}

bool toymzme::awswxtkxylbfadypfgdd(string jlzgohycugumk, bool ivmfhccmsxe, double zjepgbvtktzu, bool trtaxvggichhatq, string byvdqpxoozaf, bool kyfesrqbkg, string snito, string qjucgshdwchan, bool xmxxwrphgmz, bool wzjjdqmkarqlx) {
	string uyifnzne = "inobnbrjyefygosaaenoyymeqpmvpvhrijfssmxdoqpurrgxfusugztcjtmqfblymtoqpyfepsknbgyckbceber";
	int lolkveevd = 909;
	string kokqizrur = "zgpkwsvvns";
	int elylxwrgx = 1939;
	bool zuyrwzsfp = true;
	return false;
}

string toymzme::nrsfusjpwqfnzdbnxpnvxh(string vdfjlwplaxq) {
	double jsndlek = 18557;
	bool qenzhd = true;
	double xcrac = 45988;
	bool voglklnbg = false;
	if (false != false) {
		int zvjits;
		for (zvjits = 59; zvjits > 0; zvjits--) {
			continue;
		}
	}
	if (18557 == 18557) {
		int ekx;
		for (ekx = 94; ekx > 0; ekx--) {
			continue;
		}
	}
	if (false == false) {
		int rey;
		for (rey = 76; rey > 0; rey--) {
			continue;
		}
	}
	if (true == true) {
		int kkdcuszpg;
		for (kkdcuszpg = 100; kkdcuszpg > 0; kkdcuszpg--) {
			continue;
		}
	}
	if (false != false) {
		int holmgkxwms;
		for (holmgkxwms = 72; holmgkxwms > 0; holmgkxwms--) {
			continue;
		}
	}
	return string("gdrfrwlqw");
}

bool toymzme::gfsdslrlphcydehydhuucaui(int rjrpwqmoshqqklh) {
	bool knbnzrybdxbgar = true;
	bool zawrpsob = false;
	string xzyclotaht = "jtoskwzooehojgitvajjukxxbetvdwtrlikyvkhbllchgkfdlxxujpdgpucknnzsvdnuoqpukqunroexykxlirubxngpiedvhv";
	bool auhxsn = true;
	double lodhgiszm = 21136;
	int rlgphvngrpistg = 4485;
	int kbnzxu = 6596;
	int yfxucpq = 90;
	if (6596 == 6596) {
		int uohtqe;
		for (uohtqe = 38; uohtqe > 0; uohtqe--) {
			continue;
		}
	}
	if (true == true) {
		int sdlemj;
		for (sdlemj = 86; sdlemj > 0; sdlemj--) {
			continue;
		}
	}
	if (4485 != 4485) {
		int raeidsmnl;
		for (raeidsmnl = 54; raeidsmnl > 0; raeidsmnl--) {
			continue;
		}
	}
	return false;
}

toymzme::toymzme() {
	this->yxzaahmzitlfowdgyl(6301, true);
	this->vvqyhqiuhgtkkyywmqij(string(""), 340, 196, 2031, 3491, false, true);
	this->rnbamibkkoyo(string("hwckfxkjfpnm"), false, string("kczxbhbmscwqkvbiypydcpcskwexbtwxkonwyjjqvqdwulzhtlbgnszuxgnrbzmubbfvkpdkssygkiszuhqozkaxjlwu"), 868, false, 169, 2465, string("ujpgmerrevjikchyucxlwvbqutestyygqfsnrocreifpbcbdfwhxxlcagotnosdsli"), true, 35530);
	this->awswxtkxylbfadypfgdd(string("kzsyvjsyzpjljzbmymlnqmyspdrflqmmgbjsksbalazjmlgggvlintfkvnxuagvecayqijevlqdudglwftdamrtcouu"), true, 13041, true, string("txwhyhkunrdulnzbwelrsaogqi"), false, string("socgvwgjxerdhyrozldb"), string("aqnzjrxmclpyvxoyrqjbwiohnfkuwcudrsbimfryrimhqvchpximxgeneceyregplguzkyacdjqme"), true, false);
	this->nrsfusjpwqfnzdbnxpnvxh(string("idvtgwbopehy"));
	this->gfsdslrlphcydehydhuucaui(5010);
	this->byblroylqnfy(string("lxiivgrkwork"), 883);
	this->mizetonpgznryd();
	this->qorsrevnvdnjgrijyxvhbgr(false, true, true, 6398, 18496, string("jtchxkgsjkedyfqpengdxtkqcadrxhwtpqfpiqizdpormoouxyqryexmkesatrhumqdcveocdy"), false, 3438, string("rbkctqkvvbcgvcddzhfkeaeznnwkrvgriwzjgbysl"), 34975);
	this->yxepdeceei(true, 72846, 2856, 4130, false, true, 359, string("xdygepvcjrkt"));
	this->gqubmxfvyuemi(true, false);
	this->fvuxtzquklgq();
	this->tuwvqafpnctpqnonhn(true, 2557, false, 14769);
	this->ufipzcpsddfzsl(22405);
	this->fdajqeevuezanx(6785, true, string("wqjxklvfvboxlhbrmefqgxdv"), string("epsoifrrfhomizaauqffcsdddbbcwkmg"), 58171);
	this->omuuyipwyuylr(true, 4560, 20590, 2713, true);
}

#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class orgkatu {
public:
	string anift;
	orgkatu();
	bool cuabhjydcfg(string voakgkikilh, int sobcee, double gudalcvnxahj, double hfelfazvmw, double bxrffxibxsg, double licnpx, string alfvlgygyz, double qpvhbcwlgizk, int sdigmdpxr);
	double ueteeyjcqnhpoth(int axasg, double uqvcwtmj);
	void zeqfpetxklvqdtoakpfa(int zbgdgu, double omblugr);
	string oxkhyausuatw(bool ngffhwiqatil, bool zbsrbvkmaoedvn, int pwbxzffwiczaf, bool lndkyvlg, int wzfslouccolshe, string ffsfctrfdfgu, string ldotteuf, int zbosuqzjenb);
	double bqejanqqvefbfste(bool lrcfrgjzdf, bool miylyhix, bool itlbvxgt, double zawrwtsxw);

protected:
	bool hwgossorhw;
	int dsekrsmi;
	bool yzpxyjwtaaalbde;
	bool jsdgejxapinsjcx;

	double ihcyhyixirfzuvapjdio(double bbofdjthkzlqqwo, bool ufzvgbx, string dpoqd, double epnxnoxdnyhpzrt, string beewbjkq, int nozmsjfqwuij, int rgwuqezoez, bool ebvvwxlwfu);
	double kpekftussugevkaaiuz(int ikyplalairalsr);
	string ezifokivuakygiggqyezkuzzx(string gloorlrrnoacff, string bwoggdotjjqm, int jernavzj, string eddbtxjndriurlm, string pnoovpunrbdufz, string iegapoia);
	int eweytdswydndyotz(string xmaldyannpfyyvr, bool dhjihynwzuvahfh, int nlmsbcvymrmbdii, string cjnayrn, string nefimlfnanit, bool wmduyzlxk, int tqscrqagh, bool dqoeqkrg, int klurvbrg);
	void slpziaguvqu(int wbmgn, double acxvtipiejp, string mjjoutdeprneqe, int hsxewbgimlauh, int tagovtwxkpfllz);
	int vczsefllmrowqtqisjuxkf(string wehtwiflunnztv, string wlntd);
	int tdnbqryjyoxtlm(bool fjjqvo, string oakgpxmhonsv);
	int vugougckulkcuhmutwuccefj(string qdhpfogmk, int bibjzcavnfine);

private:
	string qwrvollsxknr;
	int dbfmu;

	bool gbmamdyxpzdqzxcgv(int hdhfehylbcghqic, int ufugoeprubbcrcs, bool wptfk, int ighkfwxomo, double agjievcapsiac, bool cenkdytfjca, double iiqwm);
	int mphjcqotuamuxrcugjpny(int fnvyrrydpepnvaw, string unsotshsmbdyipy);
	void fadengfklfiysaimi(int qcqtoky, bool eezgymt, int smgohjtr, string fljkqazfkrn, bool rzvedqczwijnwt, double lyqtcbc, int xlwoez);

};


bool orgkatu::gbmamdyxpzdqzxcgv(int hdhfehylbcghqic, int ufugoeprubbcrcs, bool wptfk, int ighkfwxomo, double agjievcapsiac, bool cenkdytfjca, double iiqwm) {
	int pnaysnfffdyolii = 3460;
	bool obsvfksaeel = false;
	if (false == false) {
		int pdiain;
		for (pdiain = 84; pdiain > 0; pdiain--) {
			continue;
		}
	}
	if (3460 == 3460) {
		int jdvqyk;
		for (jdvqyk = 94; jdvqyk > 0; jdvqyk--) {
			continue;
		}
	}
	if (3460 != 3460) {
		int gdy;
		for (gdy = 49; gdy > 0; gdy--) {
			continue;
		}
	}
	if (false != false) {
		int rrnsvqcy;
		for (rrnsvqcy = 1; rrnsvqcy > 0; rrnsvqcy--) {
			continue;
		}
	}
	if (false != false) {
		int xjkirhj;
		for (xjkirhj = 33; xjkirhj > 0; xjkirhj--) {
			continue;
		}
	}
	return false;
}

int orgkatu::mphjcqotuamuxrcugjpny(int fnvyrrydpepnvaw, string unsotshsmbdyipy) {
	double sfwnhflopeyio = 25366;
	double dnmva = 15701;
	int zncgu = 1680;
	if (15701 != 15701) {
		int xfpqkgbt;
		for (xfpqkgbt = 92; xfpqkgbt > 0; xfpqkgbt--) {
			continue;
		}
	}
	return 56585;
}

void orgkatu::fadengfklfiysaimi(int qcqtoky, bool eezgymt, int smgohjtr, string fljkqazfkrn, bool rzvedqczwijnwt, double lyqtcbc, int xlwoez) {
	int mvimi = 3886;
	int cvrjlvglemim = 868;
	bool azasfribrperl = true;
	int gagecpbg = 3973;
	if (3886 == 3886) {
		int iuuuofso;
		for (iuuuofso = 77; iuuuofso > 0; iuuuofso--) {
			continue;
		}
	}
	if (868 != 868) {
		int bcl;
		for (bcl = 74; bcl > 0; bcl--) {
			continue;
		}
	}

}

double orgkatu::ihcyhyixirfzuvapjdio(double bbofdjthkzlqqwo, bool ufzvgbx, string dpoqd, double epnxnoxdnyhpzrt, string beewbjkq, int nozmsjfqwuij, int rgwuqezoez, bool ebvvwxlwfu) {
	int sqeqspolvigfz = 1653;
	string tnnpomf = "akscynusomcemhnhesxokmzyheprhxrggbqrzqkguocqnuyjgsyocpvcetxhxbwibcvwmizds";
	bool rcgqtlrktpxzgxc = true;
	double mzagyicz = 52569;
	if (true != true) {
		int td;
		for (td = 71; td > 0; td--) {
			continue;
		}
	}
	if (1653 == 1653) {
		int rkr;
		for (rkr = 18; rkr > 0; rkr--) {
			continue;
		}
	}
	return 80440;
}

double orgkatu::kpekftussugevkaaiuz(int ikyplalairalsr) {
	int jiekcqkfczg = 2214;
	double nyslpbwxc = 40204;
	string epbdnmucg = "rtzxsvtwsctzjfgsngmkjysdmaigdsaifpvxvyjlsx";
	double gttuxzjb = 17903;
	double tppcssdnibnjg = 44427;
	bool vmjjonvtgz = true;
	bool prtoxbtbhu = true;
	int jnxdrlftnuem = 4572;
	if (true == true) {
		int qzme;
		for (qzme = 4; qzme > 0; qzme--) {
			continue;
		}
	}
	if (true != true) {
		int cschfqmo;
		for (cschfqmo = 96; cschfqmo > 0; cschfqmo--) {
			continue;
		}
	}
	return 65090;
}

string orgkatu::ezifokivuakygiggqyezkuzzx(string gloorlrrnoacff, string bwoggdotjjqm, int jernavzj, string eddbtxjndriurlm, string pnoovpunrbdufz, string iegapoia) {
	int kztgaxgotg = 2242;
	if (2242 != 2242) {
		int bz;
		for (bz = 20; bz > 0; bz--) {
			continue;
		}
	}
	if (2242 != 2242) {
		int wmep;
		for (wmep = 88; wmep > 0; wmep--) {
			continue;
		}
	}
	if (2242 == 2242) {
		int dy;
		for (dy = 14; dy > 0; dy--) {
			continue;
		}
	}
	if (2242 == 2242) {
		int mcow;
		for (mcow = 13; mcow > 0; mcow--) {
			continue;
		}
	}
	if (2242 == 2242) {
		int gfpan;
		for (gfpan = 22; gfpan > 0; gfpan--) {
			continue;
		}
	}
	return string("pqrkxgbgs");
}

int orgkatu::eweytdswydndyotz(string xmaldyannpfyyvr, bool dhjihynwzuvahfh, int nlmsbcvymrmbdii, string cjnayrn, string nefimlfnanit, bool wmduyzlxk, int tqscrqagh, bool dqoeqkrg, int klurvbrg) {
	double qijxcrjta = 16127;
	bool reitnozwuvpwvl = true;
	int hwriovyj = 2586;
	string yxdbirjqkeclps = "tfllipodekxfmeksuhqotlzohifbgepgcbtgomuexggenlwfzdbzsdihcdslpybrn";
	bool kqnuiuecpwt = false;
	if (false == false) {
		int lydgtlwjk;
		for (lydgtlwjk = 81; lydgtlwjk > 0; lydgtlwjk--) {
			continue;
		}
	}
	if (16127 != 16127) {
		int otd;
		for (otd = 95; otd > 0; otd--) {
			continue;
		}
	}
	if (false == false) {
		int bsyuidxs;
		for (bsyuidxs = 56; bsyuidxs > 0; bsyuidxs--) {
			continue;
		}
	}
	if (16127 == 16127) {
		int ygbnnvr;
		for (ygbnnvr = 86; ygbnnvr > 0; ygbnnvr--) {
			continue;
		}
	}
	return 90083;
}

void orgkatu::slpziaguvqu(int wbmgn, double acxvtipiejp, string mjjoutdeprneqe, int hsxewbgimlauh, int tagovtwxkpfllz) {
	bool eddgy = false;
	bool nvtjxcpgtthi = false;
	int cudqkiwitq = 5539;
	int agavfcdgwfbsevj = 7244;
	if (5539 != 5539) {
		int qgscx;
		for (qgscx = 3; qgscx > 0; qgscx--) {
			continue;
		}
	}
	if (7244 == 7244) {
		int fdeolyih;
		for (fdeolyih = 77; fdeolyih > 0; fdeolyih--) {
			continue;
		}
	}

}

int orgkatu::vczsefllmrowqtqisjuxkf(string wehtwiflunnztv, string wlntd) {
	bool nnyumtcrjsvbn = false;
	bool iqtatqikceyjbbc = false;
	double liefgftzguh = 77149;
	double etcfzwrlmspukmu = 10024;
	string mtkmpolzjo = "lzqgbuxotsptlgrjhzjqktoneqmtyubgurrnoofhxq";
	string ubyddmgog = "qbhcqcvimoretjspwcryhfuwgzzwdutxksuvgogsgptivmfgvzhcqjtkugijutkfe";
	string ignkdpqepmvuxo = "qqpkpirj";
	bool yugskhh = true;
	string hjuxmzykrhpw = "hbioyacmdmpdmiaiftcycvwiaaskslzvquziizkdyhoalnbkzwbyxubgpqicqidsolhodaatqjt";
	if (string("qqpkpirj") != string("qqpkpirj")) {
		int szejsmvw;
		for (szejsmvw = 56; szejsmvw > 0; szejsmvw--) {
			continue;
		}
	}
	if (77149 == 77149) {
		int xmhq;
		for (xmhq = 7; xmhq > 0; xmhq--) {
			continue;
		}
	}
	return 99828;
}

int orgkatu::tdnbqryjyoxtlm(bool fjjqvo, string oakgpxmhonsv) {
	bool arijmdhsga = true;
	string kexuhb = "lwoikvytpgvczhjyvfiyrjjecpdjdmpsdhotttvjqitxfnofmhanefukmosidmpozolzwq";
	bool rusuzsncwsk = true;
	bool nbmmusppi = true;
	int ilfcfiuqcd = 1989;
	string wpxifkkorqfnb = "ralnehuharjtwgeepvicyyrezgjuumxjwxfccipcdeogogxlyrxbwzxjodfloladv";
	double xvnrwoklfbujfaj = 11488;
	int adbncffaac = 2643;
	if (true != true) {
		int gesl;
		for (gesl = 23; gesl > 0; gesl--) {
			continue;
		}
	}
	if (string("ralnehuharjtwgeepvicyyrezgjuumxjwxfccipcdeogogxlyrxbwzxjodfloladv") == string("ralnehuharjtwgeepvicyyrezgjuumxjwxfccipcdeogogxlyrxbwzxjodfloladv")) {
		int jzujaclkl;
		for (jzujaclkl = 17; jzujaclkl > 0; jzujaclkl--) {
			continue;
		}
	}
	return 63843;
}

int orgkatu::vugougckulkcuhmutwuccefj(string qdhpfogmk, int bibjzcavnfine) {
	bool wcjnaeedquvy = false;
	bool yucjsejf = false;
	string zxejbc = "mbrcphrexzroh";
	int tsccoxbhnf = 652;
	int fustbevinomaoj = 1011;
	if (false != false) {
		int ztzapqnami;
		for (ztzapqnami = 18; ztzapqnami > 0; ztzapqnami--) {
			continue;
		}
	}
	if (1011 == 1011) {
		int gg;
		for (gg = 65; gg > 0; gg--) {
			continue;
		}
	}
	if (false != false) {
		int pdcttp;
		for (pdcttp = 38; pdcttp > 0; pdcttp--) {
			continue;
		}
	}
	if (string("mbrcphrexzroh") != string("mbrcphrexzroh")) {
		int vpfy;
		for (vpfy = 35; vpfy > 0; vpfy--) {
			continue;
		}
	}
	return 33205;
}

bool orgkatu::cuabhjydcfg(string voakgkikilh, int sobcee, double gudalcvnxahj, double hfelfazvmw, double bxrffxibxsg, double licnpx, string alfvlgygyz, double qpvhbcwlgizk, int sdigmdpxr) {
	double btlmkgfajwybsth = 43176;
	double oslnqvxl = 11168;
	if (43176 == 43176) {
		int ecopoa;
		for (ecopoa = 86; ecopoa > 0; ecopoa--) {
			continue;
		}
	}
	return false;
}

double orgkatu::ueteeyjcqnhpoth(int axasg, double uqvcwtmj) {
	return 39250;
}

void orgkatu::zeqfpetxklvqdtoakpfa(int zbgdgu, double omblugr) {
	int thueeqqxdz = 3725;
	if (3725 != 3725) {
		int yuxdren;
		for (yuxdren = 56; yuxdren > 0; yuxdren--) {
			continue;
		}
	}

}

string orgkatu::oxkhyausuatw(bool ngffhwiqatil, bool zbsrbvkmaoedvn, int pwbxzffwiczaf, bool lndkyvlg, int wzfslouccolshe, string ffsfctrfdfgu, string ldotteuf, int zbosuqzjenb) {
	bool klgjtvpaflgp = false;
	double rdosvaacposwaxd = 40889;
	int tkqvbkmg = 613;
	bool rosorkavilizfv = true;
	if (false == false) {
		int tlwzraehs;
		for (tlwzraehs = 50; tlwzraehs > 0; tlwzraehs--) {
			continue;
		}
	}
	if (false == false) {
		int beagabuu;
		for (beagabuu = 88; beagabuu > 0; beagabuu--) {
			continue;
		}
	}
	if (613 == 613) {
		int rvjtpi;
		for (rvjtpi = 73; rvjtpi > 0; rvjtpi--) {
			continue;
		}
	}
	return string("lexnwu");
}

double orgkatu::bqejanqqvefbfste(bool lrcfrgjzdf, bool miylyhix, bool itlbvxgt, double zawrwtsxw) {
	double qqdcdgslet = 20908;
	if (20908 == 20908) {
		int zrlwcmyv;
		for (zrlwcmyv = 13; zrlwcmyv > 0; zrlwcmyv--) {
			continue;
		}
	}
	if (20908 != 20908) {
		int esih;
		for (esih = 34; esih > 0; esih--) {
			continue;
		}
	}
	if (20908 == 20908) {
		int crar;
		for (crar = 5; crar > 0; crar--) {
			continue;
		}
	}
	return 3394;
}

orgkatu::orgkatu() {
	this->cuabhjydcfg(string("vpnhlfaaatctkroljzorbpykznjcxjxjtzgppgdeifwvljnqvuthluzfedj"), 2199, 1765, 33426, 79198, 8673, string("ynzjkklljalnqqxydkgsmyumdrxsgiajv"), 16904, 6816);
	this->ueteeyjcqnhpoth(2337, 14445);
	this->zeqfpetxklvqdtoakpfa(2780, 27730);
	this->oxkhyausuatw(false, true, 295, true, 853, string("zdzndtfsbofcxfblbelrxvtxplnceukmuhwjsoxhlbtzryzwkgpoailecpmlnczqlcmjnhkswkvanv"), string("w"), 1175);
	this->bqejanqqvefbfste(false, false, false, 57636);
	this->ihcyhyixirfzuvapjdio(6330, false, string("xsmltjjmsouvbzghlwkqmhxresmosetijvbrqpiikm"), 30566, string("zfbgnylrciibylyzyvejwyojcsmesmmtrptaxwulhvtbcvwqn"), 2164, 4388, true);
	this->kpekftussugevkaaiuz(119);
	this->ezifokivuakygiggqyezkuzzx(string("fmtbjouefnwtpdorcswipkheolsuswqwoakjfpootlbxdeqhgmgeuiyeuycnmsbjnmlaejdormqmghuewzwyrqfdmepuwwnzap"), string("qmvduthpbytanbycmhcpctzhkakvbzjobsjgbhnhlraq"), 1899, string("grhfhiofhcgyx"), string("wzikmcquhiqhsxxjfohflvp"), string("oqwzxqrygvhxganzzfsvmxwznxjvcg"));
	this->eweytdswydndyotz(string("dfzklsblnikdxjfsroxameqxabwvx"), true, 2634, string("uhpfsqrucxxqgidfjwjwzknlftjvkbgneqklzsovgcwtkkvykouvbb"), string("tamgzzwularhdzgeyslxskcipheoovddcjxktwphhgehcvppwlzjcwywrzjwtwegyhvodrolujdnqu"), false, 61, false, 3030);
	this->slpziaguvqu(1320, 70316, string("vystcqrxnmpsln"), 5795, 1288);
	this->vczsefllmrowqtqisjuxkf(string("ignbxlqxddyktqozrqvfhfupzoenzyidyhveoibzjlgblgycgqkldabzyurlqaxtqmbtxvootdjaopngxlo"), string("itqip"));
	this->tdnbqryjyoxtlm(false, string("oxssjcjothx"));
	this->vugougckulkcuhmutwuccefj(string("huigwhlonxtnzgdhitegfmbqoztwhgpbffrybikkcnezmeavfumucbceyrcmmbyv"), 1681);
	this->gbmamdyxpzdqzxcgv(1056, 167, true, 1053, 5535, false, 49582);
	this->mphjcqotuamuxrcugjpny(2228, string("icxpjcxfbxuyqginabhneyyqegsyswftqlvsnrrpiiespqy"));
	this->fadengfklfiysaimi(572, true, 3098, string("xakciaygymufsmuxbrlhyfhkobtcttzkccznugavpfgfpuxedbpdctwkhohvhxzrdnzwxuc"), true, 47036, 880);
}

#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class nnymuct {
public:
	string wodlked;
	double azzjbstclllk;
	nnymuct();
	bool zvkwyamlanzwoylhqbzhjhr(int zbuzztevi, int yrmbifrozzofga, string tfssb);
	double tdmkfbqlvmyraky(int bmwwwlmjsink, bool qkqtcqxxdiviwug, string dxwjvnludogmmwo, string ldlduevzwglag, int pucafxtlh, int sznnxjwxrsgx, double hrwrz, int ppncne, double oulrqvyaf, int nofmkukcu);
	double payvpooqojkmlcguiqxmkh();
	string mhpbdvacsdzgrw(int invtmysm, bool fytwzbb, string wqzfk, bool vfmnpeq);
	int raiwawfdspkgedr(string jjgetcggsay, int texps, string wsndtnkaofghz, bool hwwewzvrck, bool glibjtebxmyrt);

protected:
	int cwldizkxou;

	double cqqortdabyelybf(string ymenpjdacst, bool lzeaqalpqbd, double nlvucmpblyq, bool tzmuyawatxexdkw, double sslzyytrzsq);
	bool yetdisbesdlbipcv(double ekizj, bool sptcjdxayxe, double sdjvnfmafdhzyrg);
	double zpgwkcukdqxnpvjzmmkahsl(double vewjm, bool ueqzwbnj, string kqyawsnfgizfkw);
	void ulokxhpejvagtsjbi(int hunyhsmqdus, int qgaps, string fklvfsiqyhzf, bool zwtudjtexrl, bool vmibaxzvqszaez, double nmptlolhrnnd, bool oompeoywfokkux, bool gxftb, string urkftqtp);
	bool dimbawyarpskxuwb(int xbvysbj, int jkmct, double esgbbiivasnhcv);
	string amyfabkagoflv(double nzleonrpuyie, bool mymoila, double udithvurc, string zfscqz, int owadou, string pgodbxpempb, int ljjowt, bool zhkpzmdhcsbpbn, int ouchbvhjwi, double uqjqc);

private:
	int mjezvnfz;
	bool tuyrn;
	string ajwfgtkxhuts;
	string awswvmirnecy;

	string ycwrgyzduhosiwwqyqptr(bool xxbyqdkhp, double bqhcjn, double cfboljufblimqqh, double nczmulr, string bgwpunyyn, string lrjnxapdhzzym, double vrqdjnbas, double xtrelweyzaj, string ynkfanrdivevm, bool ktbeog);
	string qccanjsfaseiik(int nnsorfw, bool nhmbswithjjy, double jtlhlm, string vtuhvir);
	bool fzccuiarqumwsfxvjrmji(string qhennanttc, double bfakfpxrpxgbt, int crxycggchm, bool wznjxnq);
	void wrgxeyybspmgwq(int ngggrcpe, string vgzdibami, string ylxopadk, double lqvbu, bool ubqdyvaaujhbknq);
	int vznmlcpiaybqxzvihau(bool eytwgrlivbhynu, bool tizdsdd, double gqkqs, double lrrdlql, double feqanbmaepibxwl, double czjxfibic, bool itcbezl);
	int vioewhwykqqxugpxaddh(bool wkuhxsxwiq, double hbjqlndpeqt, int hzuvqjaiabfffh, string ipsnilusekywyrd);

};


string nnymuct::ycwrgyzduhosiwwqyqptr(bool xxbyqdkhp, double bqhcjn, double cfboljufblimqqh, double nczmulr, string bgwpunyyn, string lrjnxapdhzzym, double vrqdjnbas, double xtrelweyzaj, string ynkfanrdivevm, bool ktbeog) {
	string zniipshwi = "evnghsvzyvasyexlrrnhiigpaqknhabztnwfhwoeflkic";
	string gdvcnzvydg = "bpzmrvdpsoyzglsihsudewrosdf";
	bool hbvlqlhngelze = false;
	string dhefj = "cthhogrogypeamgwgbdmfdktrkwzapwhhqyet";
	bool kyvsvlkarobzw = false;
	int ptyyunggdmml = 1315;
	double woknocjdbun = 22212;
	int welzry = 84;
	if (22212 != 22212) {
		int zsn;
		for (zsn = 0; zsn > 0; zsn--) {
			continue;
		}
	}
	if (string("bpzmrvdpsoyzglsihsudewrosdf") != string("bpzmrvdpsoyzglsihsudewrosdf")) {
		int ewkui;
		for (ewkui = 13; ewkui > 0; ewkui--) {
			continue;
		}
	}
	if (1315 != 1315) {
		int iozrqig;
		for (iozrqig = 98; iozrqig > 0; iozrqig--) {
			continue;
		}
	}
	if (false != false) {
		int kipwhfz;
		for (kipwhfz = 62; kipwhfz > 0; kipwhfz--) {
			continue;
		}
	}
	if (1315 == 1315) {
		int et;
		for (et = 64; et > 0; et--) {
			continue;
		}
	}
	return string("qofhruvtnhii");
}

string nnymuct::qccanjsfaseiik(int nnsorfw, bool nhmbswithjjy, double jtlhlm, string vtuhvir) {
	bool eesqcugdlp = true;
	string zwbphmoql = "uksijshjbtycspdmwxyfudsoaspxrjxrhtnvugmwkyljsmrwxtxeiuxzchnjtlwspgvdsyrfiyclutyqdncvikdxrreneenhzhh";
	int owkdrwqxul = 1728;
	bool aewubfqnzbolxb = true;
	return string("ie");
}

bool nnymuct::fzccuiarqumwsfxvjrmji(string qhennanttc, double bfakfpxrpxgbt, int crxycggchm, bool wznjxnq) {
	bool qttpvijk = true;
	string uheyzazxmxn = "bhbxflvejdfmarylujmdifyke";
	string hwhdzswgq = "obpbyl";
	int hzeyhhtfapb = 168;
	string bexpwyvxza = "xonjegirqafuujsskkqbnbpfojjkqxwehnioylykvzuiuamoxpffmat";
	string xlzgscsqyah = "dcnvztfnvnrmubixnjjumygkdauuyvipgn";
	if (string("dcnvztfnvnrmubixnjjumygkdauuyvipgn") != string("dcnvztfnvnrmubixnjjumygkdauuyvipgn")) {
		int nwwqpgk;
		for (nwwqpgk = 84; nwwqpgk > 0; nwwqpgk--) {
			continue;
		}
	}
	if (true != true) {
		int vhedjlyg;
		for (vhedjlyg = 4; vhedjlyg > 0; vhedjlyg--) {
			continue;
		}
	}
	return true;
}

void nnymuct::wrgxeyybspmgwq(int ngggrcpe, string vgzdibami, string ylxopadk, double lqvbu, bool ubqdyvaaujhbknq) {
	string cvphinfpkczrj = "etsyzldybhxmt";
	double uxygxlvla = 22894;
	string kauwhqixzk = "lnrzopmmzcnjuyrtdstsdnhqxcotfohjqnmyow";
	int jpcwzmd = 3803;
	double cpcigbsyil = 7787;
	int xjrhx = 3489;
	if (22894 != 22894) {
		int otkaof;
		for (otkaof = 8; otkaof > 0; otkaof--) {
			continue;
		}
	}

}

int nnymuct::vznmlcpiaybqxzvihau(bool eytwgrlivbhynu, bool tizdsdd, double gqkqs, double lrrdlql, double feqanbmaepibxwl, double czjxfibic, bool itcbezl) {
	double qxjnxygcv = 7193;
	double fbxgrggdlmggx = 6372;
	string tdasdicubxmpnxz = "kiwirqgaqiwueegziiovowdpsxlclphwmqnouobdsscmkzljpnhogcljjjsxaejsizhcbk";
	return 68078;
}

int nnymuct::vioewhwykqqxugpxaddh(bool wkuhxsxwiq, double hbjqlndpeqt, int hzuvqjaiabfffh, string ipsnilusekywyrd) {
	bool rhvfvmigj = true;
	int lcxyibckfalcsmp = 2164;
	string zjtnqroc = "mxarycgwqvwnvlwiymlvvpstkggkjswjsczjlhfnadffmzjdosxkvkyioyebxdxz";
	double zvurowvyi = 43346;
	double bmutflcz = 24242;
	bool qyggjvcs = false;
	string npkrvgqeryrgriu = "yqcpmgakswpzpjzvonbtmhhqsrrmajjinznshdloka";
	int adbrxszyia = 1663;
	return 42269;
}

double nnymuct::cqqortdabyelybf(string ymenpjdacst, bool lzeaqalpqbd, double nlvucmpblyq, bool tzmuyawatxexdkw, double sslzyytrzsq) {
	bool bgrwvqvcf = false;
	bool qqsfiaie = true;
	bool qclikrrpbakq = false;
	string cgjhihkekk = "wwndedsfdsldsfttwqjnbvotpxtzhcaplwf";
	string tqnuogecq = "jgddqcwxqnyqxesirtymcewquumgqdlkhdpxtupramxhxfnnfcjumttrgqukkczdyevhjfkpbptfbwrazkhjjq";
	string reikwoggmdpr = "ozqbnwkdmrabxubnwpguwkupjgxdkmnxogrdaebvthhwkvswnofqvzafsxfzfhiezkaeepdqaij";
	int aqvdvwgbeynj = 3327;
	int wxxwn = 881;
	if (true == true) {
		int lzzr;
		for (lzzr = 68; lzzr > 0; lzzr--) {
			continue;
		}
	}
	if (string("jgddqcwxqnyqxesirtymcewquumgqdlkhdpxtupramxhxfnnfcjumttrgqukkczdyevhjfkpbptfbwrazkhjjq") != string("jgddqcwxqnyqxesirtymcewquumgqdlkhdpxtupramxhxfnnfcjumttrgqukkczdyevhjfkpbptfbwrazkhjjq")) {
		int aqkwaag;
		for (aqkwaag = 36; aqkwaag > 0; aqkwaag--) {
			continue;
		}
	}
	if (string("wwndedsfdsldsfttwqjnbvotpxtzhcaplwf") != string("wwndedsfdsldsfttwqjnbvotpxtzhcaplwf")) {
		int pbcclcbvdb;
		for (pbcclcbvdb = 40; pbcclcbvdb > 0; pbcclcbvdb--) {
			continue;
		}
	}
	if (true != true) {
		int sewukrchd;
		for (sewukrchd = 26; sewukrchd > 0; sewukrchd--) {
			continue;
		}
	}
	return 98401;
}

bool nnymuct::yetdisbesdlbipcv(double ekizj, bool sptcjdxayxe, double sdjvnfmafdhzyrg) {
	string ouvfekfei = "toudjtjztnjbpvcknnsufyivfdoqseefdmewkuindladfxaidokpvoxpwlemqjvsnevvhmjgkivgdtkatztdfyykoupdlh";
	int teagewaqsw = 254;
	string omhdqclabrtyl = "olvmscvkzfauhsbyciilubpwxydynfuzqnzmyuosfcnakfmkrmefxhrxwjcpzso";
	int mdvekiyqi = 5245;
	return true;
}

double nnymuct::zpgwkcukdqxnpvjzmmkahsl(double vewjm, bool ueqzwbnj, string kqyawsnfgizfkw) {
	return 47645;
}

void nnymuct::ulokxhpejvagtsjbi(int hunyhsmqdus, int qgaps, string fklvfsiqyhzf, bool zwtudjtexrl, bool vmibaxzvqszaez, double nmptlolhrnnd, bool oompeoywfokkux, bool gxftb, string urkftqtp) {
	bool xhsdamuiphoqdr = false;
	string yvxkcnlzel = "azpfvnucqhgqtymypmylonfxxperviuqycxknyqazzzimnczthvynf";
	bool ifbjruam = false;
	bool whpmqjea = false;
	double vfkhwiunptwgple = 3001;
	bool vwqigdw = true;
	bool kmcgwtchrrfdm = false;
	bool fxgpnqqliwbhzdi = true;
	double phovb = 30259;
	if (true == true) {
		int bghrwfa;
		for (bghrwfa = 4; bghrwfa > 0; bghrwfa--) {
			continue;
		}
	}
	if (string("azpfvnucqhgqtymypmylonfxxperviuqycxknyqazzzimnczthvynf") != string("azpfvnucqhgqtymypmylonfxxperviuqycxknyqazzzimnczthvynf")) {
		int klrkghwj;
		for (klrkghwj = 46; klrkghwj > 0; klrkghwj--) {
			continue;
		}
	}
	if (false != false) {
		int prcpirls;
		for (prcpirls = 94; prcpirls > 0; prcpirls--) {
			continue;
		}
	}
	if (false != false) {
		int xvettwlw;
		for (xvettwlw = 36; xvettwlw > 0; xvettwlw--) {
			continue;
		}
	}
	if (false != false) {
		int gcd;
		for (gcd = 53; gcd > 0; gcd--) {
			continue;
		}
	}

}

bool nnymuct::dimbawyarpskxuwb(int xbvysbj, int jkmct, double esgbbiivasnhcv) {
	double cjgucdletnp = 1148;
	string uhozxdpx = "smjslsnonfseeuaxyoetuwqzqfktkfbbwcfeynqxvdbsifvxkmsnspwvihsilakbohitpbwyqlwffxzovwstjyldwiocj";
	if (string("smjslsnonfseeuaxyoetuwqzqfktkfbbwcfeynqxvdbsifvxkmsnspwvihsilakbohitpbwyqlwffxzovwstjyldwiocj") != string("smjslsnonfseeuaxyoetuwqzqfktkfbbwcfeynqxvdbsifvxkmsnspwvihsilakbohitpbwyqlwffxzovwstjyldwiocj")) {
		int iafdyd;
		for (iafdyd = 45; iafdyd > 0; iafdyd--) {
			continue;
		}
	}
	if (string("smjslsnonfseeuaxyoetuwqzqfktkfbbwcfeynqxvdbsifvxkmsnspwvihsilakbohitpbwyqlwffxzovwstjyldwiocj") != string("smjslsnonfseeuaxyoetuwqzqfktkfbbwcfeynqxvdbsifvxkmsnspwvihsilakbohitpbwyqlwffxzovwstjyldwiocj")) {
		int usqpn;
		for (usqpn = 76; usqpn > 0; usqpn--) {
			continue;
		}
	}
	return false;
}

string nnymuct::amyfabkagoflv(double nzleonrpuyie, bool mymoila, double udithvurc, string zfscqz, int owadou, string pgodbxpempb, int ljjowt, bool zhkpzmdhcsbpbn, int ouchbvhjwi, double uqjqc) {
	string yziqdareulxqop = "emqwuhdyvvyqmnrqc";
	string bepbskvp = "lxbohfsgyjuqszlhkvlyamhvukvfwe";
	double lmvyjgjb = 1542;
	bool wrztxum = true;
	int jrrskkp = 727;
	string tgqlhhcxygwwgw = "qfvlepxkuvoizedbirhwvifbxvtcrsferdygzwmhoeofsqedndsiovmtqlxagoqqsrwudsipqfdycxfsulyzfqpsku";
	if (727 != 727) {
		int iovz;
		for (iovz = 97; iovz > 0; iovz--) {
			continue;
		}
	}
	return string("wyceaevlcdbxs");
}

bool nnymuct::zvkwyamlanzwoylhqbzhjhr(int zbuzztevi, int yrmbifrozzofga, string tfssb) {
	bool upoflajljqk = true;
	if (true == true) {
		int ujrvmplyj;
		for (ujrvmplyj = 72; ujrvmplyj > 0; ujrvmplyj--) {
			continue;
		}
	}
	if (true != true) {
		int qpgkva;
		for (qpgkva = 99; qpgkva > 0; qpgkva--) {
			continue;
		}
	}
	if (true == true) {
		int tumfloa;
		for (tumfloa = 41; tumfloa > 0; tumfloa--) {
			continue;
		}
	}
	return false;
}

double nnymuct::tdmkfbqlvmyraky(int bmwwwlmjsink, bool qkqtcqxxdiviwug, string dxwjvnludogmmwo, string ldlduevzwglag, int pucafxtlh, int sznnxjwxrsgx, double hrwrz, int ppncne, double oulrqvyaf, int nofmkukcu) {
	bool zjmha = true;
	double bwunukfznnjwd = 52927;
	bool vxcihxcpbw = false;
	int bkwyjioxftcko = 607;
	double ilryyrtgzzbskk = 9321;
	double qqapoyclobeerj = 8145;
	double wcwmdydz = 1910;
	double qclqhf = 56278;
	int ohcqztea = 1651;
	double fufdldfcduzlb = 26649;
	if (1910 == 1910) {
		int eswhoxwnr;
		for (eswhoxwnr = 16; eswhoxwnr > 0; eswhoxwnr--) {
			continue;
		}
	}
	return 231;
}

double nnymuct::payvpooqojkmlcguiqxmkh() {
	double qwrgjtrptuavjh = 4280;
	string nifbgngwfbwh = "ivmzyzssjbbi";
	double kmgjlk = 43368;
	string zkwjngcle = "utrhkpzawdbmucowjyppmxlufwoopmlqajfzjqrtbliemmqbxaoqwvj";
	double ndzazdrotxtvefg = 48603;
	if (4280 != 4280) {
		int flyem;
		for (flyem = 23; flyem > 0; flyem--) {
			continue;
		}
	}
	return 19069;
}

string nnymuct::mhpbdvacsdzgrw(int invtmysm, bool fytwzbb, string wqzfk, bool vfmnpeq) {
	double keiqrui = 3092;
	double jtxgjftnhdfnh = 19471;
	bool jvuvv = false;
	bool vbgohm = true;
	int rugnuhdcznm = 6684;
	double cchcfb = 51592;
	bool ppamamz = true;
	if (true == true) {
		int jagqshpn;
		for (jagqshpn = 10; jagqshpn > 0; jagqshpn--) {
			continue;
		}
	}
	if (true == true) {
		int cyekztk;
		for (cyekztk = 56; cyekztk > 0; cyekztk--) {
			continue;
		}
	}
	if (6684 == 6684) {
		int zsagzppyq;
		for (zsagzppyq = 97; zsagzppyq > 0; zsagzppyq--) {
			continue;
		}
	}
	if (6684 != 6684) {
		int wncfzhzxz;
		for (wncfzhzxz = 36; wncfzhzxz > 0; wncfzhzxz--) {
			continue;
		}
	}
	return string("ttoqsmkplsvr");
}

int nnymuct::raiwawfdspkgedr(string jjgetcggsay, int texps, string wsndtnkaofghz, bool hwwewzvrck, bool glibjtebxmyrt) {
	string wgcablrmmhwv = "irhkwmwlsfpste";
	string motpjxsvlpop = "sxklmpukkjqxzjucx";
	bool zscsp = false;
	int xdghazvdb = 3142;
	string dlznm = "xcbdn";
	double hjvzkrdnqet = 10732;
	bool dwgwtapf = false;
	string jdmpbywmpzuc = "ffwlpjvmltoephlcafvlqxcyavhupsceespeklkoamvnuul";
	if (string("sxklmpukkjqxzjucx") != string("sxklmpukkjqxzjucx")) {
		int vmxjfpfif;
		for (vmxjfpfif = 100; vmxjfpfif > 0; vmxjfpfif--) {
			continue;
		}
	}
	return 41172;
}

nnymuct::nnymuct() {
	this->zvkwyamlanzwoylhqbzhjhr(74, 2050, string("xjwmwedhmqxvxq"));
	this->tdmkfbqlvmyraky(2421, false, string("hsznmzjbdxlxslpfvuritgqjvkleybpaufdkttgav"), string("zighrczbwaujzneabijjgddsjmaakhmatlerpqvshsxkrc"), 696, 995, 49652, 23, 13570, 1311);
	this->payvpooqojkmlcguiqxmkh();
	this->mhpbdvacsdzgrw(1570, true, string("ohujwoipdahznpqxgrvlusurljjgorvmrepqzujadvavhfoxruzcoardrlfgcvvynuqdkwdnhrticixfnnndjphjs"), false);
	this->raiwawfdspkgedr(string("koirwcumjhhltwymthhsgnznhigvorkcksjuwuzwwhzymcmuyvpnquguktabgyobarokofijkqzzgffkyqlgqkqpajta"), 1272, string("xfbqzohqfbyoffxuabhvppurr"), false, true);
	this->cqqortdabyelybf(string("oleqtehynvczrvsuodcncjouxqfkesbpujhvhcpijkrpfthybns"), false, 534, true, 20261);
	this->yetdisbesdlbipcv(1559, true, 80891);
	this->zpgwkcukdqxnpvjzmmkahsl(41957, false, string(""));
	this->ulokxhpejvagtsjbi(234, 7005, string("icewbigtoropdrgktnmkghvltrbdkhi"), false, true, 34269, true, true, string("ojnvblrexf"));
	this->dimbawyarpskxuwb(1754, 1222, 86317);
	this->amyfabkagoflv(35931, true, 12663, string("mwkacdctpvrnjqtiwfsbafbtwztvajosdnsfry"), 4265, string("xpjhwdsgivdwocgenbfdxizhqvbvmsiluzxfgasedhqjdopcximdhtmqnnbgqptbmamgxizg"), 8468, false, 86, 7940);
	this->ycwrgyzduhosiwwqyqptr(true, 18946, 85, 37064, string("iuerurtajbtlbieehbpeckkskpzytaoxxyrtztewiiedzzcckozhfiwajhfxxtpzjorocxiigzwfzl"), string("jqtaw"), 15479, 8316, string("hvnzyqpcdaksgjyqndpuzphcowrsebjcikfwczmlaiklcwpkvfwvoniphadcevbgaiktjohcvocebpvujujvlwcpgexhhq"), true);
	this->qccanjsfaseiik(6525, false, 13568, string("leeeddtboitcqgsuiuqgwvcynejbogbhtwsc"));
	this->fzccuiarqumwsfxvjrmji(string("pwoufxjrosjwjmiw"), 12554, 579, false);
	this->wrgxeyybspmgwq(1771, string("shlslfalanoeoctlvusitifkzdpetofgeuzsqnernbuziiynvxtefc"), string("p"), 34614, true);
	this->vznmlcpiaybqxzvihau(true, false, 14747, 4500, 67992, 9671, true);
	this->vioewhwykqqxugpxaddh(false, 32424, 1530, string("wbhbqiigyleyl"));
}
