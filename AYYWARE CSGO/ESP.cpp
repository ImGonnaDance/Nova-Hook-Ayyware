
#include "ESP.h"
#include "Interfaces.h"
#include "RenderManager.h"
#include "nosmoke.h"
#include "string.h"
#include "Backtrack.h"

void CEsp::Init()
{
	BombCarrier = nullptr;
}

// Yeah dude we're defo gunna do some sick moves for the esp yeah
void CEsp::Move(CUserCmd *pCmd,bool &bSendPacket) 
{

}

// Main ESP Drawing loop
void CEsp::Draw()
{

	IClientEntity *pLocal = hackManager.pLocal();

	if (Menu::Window.VisualsTab.FiltersDead.GetState())
	{
		if (pLocal->IsAlive())
		{
			//Nothing :)
		}
		else
		{
			// Loop through all active entitys
			for (int i = 0; i < Interfaces::EntList->GetHighestEntityIndex(); i++)
			{
				// Get the entity
				IClientEntity *pEntity = Interfaces::EntList->GetClientEntity(i);
				player_info_t pinfo;

				// The entity isn't some laggy peice of shit or something
				if (pEntity &&  pEntity != pLocal && !pEntity->IsDormant())
				{
					// Radar
					if (Menu::Window.VisualsTab.Radar.GetState())
					{
						DWORD m_bSpotted = NetVar.GetNetVar(0x839EB159);
						*(char*)((DWORD)(pEntity)+m_bSpotted) = 1;
					}

					// Is it a player?!
					if (Menu::Window.VisualsTab.FiltersPlayers.GetState() && Interfaces::Engine->GetPlayerInfo(i, &pinfo) && pEntity->IsAlive())
					{
						DrawPlayer(pEntity, pinfo);
					}

					// ~ Other ESP's here (items and shit) ~ //
					ClientClass* cClass = (ClientClass*)pEntity->GetClientClass();

					// Dropped weapons
					if (Menu::Window.VisualsTab.FiltersWeapons.GetState() && cClass->m_ClassID != (int)CSGOClassID::CBaseWeaponWorldModel && ((strstr(cClass->m_pNetworkName, "Weapon") || cClass->m_ClassID == (int)CSGOClassID::CDEagle || cClass->m_ClassID == (int)CSGOClassID::CAK47)))
					{
						DrawDrop(pEntity, cClass);
					}

					// If entity is the bomb
					if (Menu::Window.VisualsTab.FiltersC4.GetState())
					{
						if (cClass->m_ClassID == (int)CSGOClassID::CPlantedC4)
							DrawBombPlanted(pEntity, cClass);

						if (cClass->m_ClassID == (int)CSGOClassID::CC4)
							DrawBomb(pEntity, cClass);
					}

					
				}
			}
		}
	}
	else
	{
		Color Color;
		// Loop through all active entitys
		for (int i = 0; i < Interfaces::EntList->GetHighestEntityIndex(); i++)
		{
			// Get the entity
			IClientEntity *pEntity = Interfaces::EntList->GetClientEntity(i);
			player_info_t pinfo;

			// The entity isn't some laggy peice of shit or something
			if (pEntity &&  pEntity != pLocal && !pEntity->IsDormant())
			{
				// Radar
				if (Menu::Window.VisualsTab.OtherRadar.GetState())
				{
					DWORD m_bSpotted = NetVar.GetNetVar(0x839EB159);
					*(char*)((DWORD)(pEntity)+m_bSpotted) = 1;
				}
				if (hackManager.pLocal()->IsAlive()) {
					CEsp::DrawLinesAA(Color);
					//drawBacktrackedSkelet(pEntity);

				}
				//DrawBullets();
				// Is it a player?!
				if (Menu::Window.VisualsTab.FiltersPlayers.GetState() && Interfaces::Engine->GetPlayerInfo(i, &pinfo) && pEntity->IsAlive())
				{
					DrawPlayer(pEntity, pinfo);
				}

				// ~ Other ESP's here (items and shit) ~ //
				ClientClass* cClass = (ClientClass*)pEntity->GetClientClass();

				// Dropped weapons
				if (Menu::Window.VisualsTab.FiltersWeapons.GetState() && cClass->m_ClassID != (int)CSGOClassID::CBaseWeaponWorldModel && ((strstr(cClass->m_pNetworkName, "Weapon") || cClass->m_ClassID == (int)CSGOClassID::CDEagle || cClass->m_ClassID == (int)CSGOClassID::CAK47)))
				{
					DrawDrop(pEntity, cClass);
				}

				// If entity is the bomb
				if (Menu::Window.VisualsTab.FiltersC4.GetState())
				{
					if (cClass->m_ClassID == (int)CSGOClassID::CPlantedC4)
						DrawBombPlanted(pEntity, cClass);

					if (cClass->m_ClassID == (int)CSGOClassID::CC4)
						DrawBomb(pEntity, cClass);
				}

			

				//DrawGrenades(pEntity);

			}
		}
	}

	//Those are good like this, no edit needed :)
	if (Menu::Window.MiscTab.OtherSpectators.GetState())
	{
		SpecList();
	}
	// Anti Flash
	if (Menu::Window.VisualsTab.OtherNoFlash.GetState())
	{
		DWORD m_flFlashMaxAlpha = NetVar.GetNetVar(0xFE79FB98);
		*(float*)((DWORD)pLocal + m_flFlashMaxAlpha) = Menu::Window.VisualsTab.OtherNoFlashVal.GetValue();
	}
}

void CEsp::NoSky()
{
	
}
void CEsp::DoFOV()
{
	float FoV;
	std::vector<int> HitBoxesToScan;
	IClientEntity* pLocal = hackManager.pLocal();
	CBaseCombatWeapon* pWeapon = (CBaseCombatWeapon*)Interfaces::EntList->GetClientEntityFromHandle(pLocal->GetActiveWeaponHandle());

	if (!pWeapon)
		return;

	if (GameUtils::IsPistol(pWeapon))
	{
		FoV = Menu::Window.LegitBotTab.WeaponPistFoV.GetValue();
		
	}
	else if (GameUtils::IsSniper(pWeapon))
	{
		FoV = Menu::Window.LegitBotTab.WeaponSnipFoV.GetValue();
		
	}
	else
	{
		FoV = Menu::Window.LegitBotTab.WeaponMainFoV.GetValue();
	}
	


	int w, h;
	Interfaces::Engine->GetScreenSize(w, h);
	float CenterX = (float)w / 2;
	float CenterY = (float)h / 2;

	
	Offsets::VMT::Surface_DrawOutlinedCircle;//(CenterX, CenterY, FoV, 360, 0, 255, 0, 255); //(int x, int y, int radius, int segments, int r, int g, int b, int a)
}








void CEsp::SpecList()
{

	IClientEntity *pLocal = hackManager.pLocal();

	RECT scrn = Render::GetViewport();
	int AC = 0;


	// Loop through all active entitys
	for (int i = 0; i < Interfaces::EntList->GetHighestEntityIndex(); i++)
	{
		// Get the entity
		IClientEntity *pEntity = Interfaces::EntList->GetClientEntity(i);
		player_info_t pinfo;

		Render::Text(scrn.left, (scrn.bottom / 2 - 16), Color(255, 255, 255, 255), Render::Fonts::SpecFont, " Spectators");


		// The entity isn't some laggy peice of shit or something
		if (pEntity &&  pEntity != pLocal)
		{

			if (Interfaces::Engine->GetPlayerInfo(i, &pinfo) && !pEntity->IsAlive() && !pEntity->IsDormant())
			{

				HANDLE obs = pEntity->GetObserverTargetHandle();

				if (obs)
				{
					IClientEntity *pTarget = Interfaces::EntList->GetClientEntityFromHandle(obs);
					player_info_t pinfo2;
					if (pTarget)
					{
						if (Interfaces::Engine->GetPlayerInfo(pTarget->GetIndex(), &pinfo2))
						{
							if (strlen(pinfo.name) > 16)
							{
								pinfo.name[16] = 0;
								strcat(pinfo.name, "...");
								puts(pinfo.name);
								RECT TextSize = Render::GetTextSize(Render::Fonts::ESP, pinfo.name);
								Render::Text(scrn.left + 10, (scrn.bottom / 2) + (16 * AC), pTarget->GetIndex() == pLocal->GetIndex() ? Color(240, 70, 80, 255) : Color(255, 255, 255, 0), Render::Fonts::SpecFont2, pinfo.name);
							}
							else
							{
								RECT TextSize = Render::GetTextSize(Render::Fonts::ESP, pinfo.name);
								Render::Text(scrn.left + 10, (scrn.bottom / 2) + (16 * AC), pTarget->GetIndex() == pLocal->GetIndex() ? Color(240, 70, 80, 255) : Color(255, 255, 255, 0), Render::Fonts::SpecFont2, pinfo.name);
							}
							
							AC++;
						}
					}
				}
			}
		}
	}

	//Render::Outline(scrn.left + 261, (scrn.bottom / 2) - 1, 262, (16 * AC) + 2, Color(23, 23, 23, 255));
	//Render::Outline(scrn.left + 260, (scrn.bottom / 2), 260, (16 * AC), Color(90, 90, 90, 255));

}



void CEsp::DrawPlayer(IClientEntity* pEntity, player_info_t pinfo)
{
	//Here is our very advanced system yay
	
	ESPBox Box;
	Color Color;
	//terrorist red



	
	// Show own team false? well gtfo teammate lol
	if (Menu::Window.VisualsTab.FiltersEnemiesOnly.GetState() && (pEntity->GetTeamNum() == hackManager.pLocal()->GetTeamNum()))
		return;
	
	if (GetBox(pEntity, Box))
	{
		Color = GetPlayerColor(pEntity);

		DrawGrenades(pEntity);		

		if (Menu::Window.VisualsTab.OptionsBox.GetIndex() == 1)
			DrawBox(Box, Color);

		if (Menu::Window.VisualsTab.OptionsBox.GetIndex() == 2)
			DrawBrackets(Box, Color);

		if (Menu::Window.VisualsTab.OptionsBox.GetIndex() == 3)
			DrawCorners(Box, Color);

		if (Menu::Window.VisualsTab.OptionsBox.GetIndex() == 4)
			DrawFilled(Box, Color);

		if (Menu::Window.VisualsTab.OptionsBox.GetIndex() == 5)
			DrawCustom(Box, Color, pEntity);

		if (Menu::Window.VisualsTab.OptionsBox.GetIndex() == 6)
			DrawTopCustom(Box, Color, pEntity);

		if (Menu::Window.VisualsTab.OptionsName.GetState())
			DrawName(pinfo, Box, pEntity);

		if (Menu::Window.VisualsTab.OptionsArmor.GetState())
			DrawArmor(pEntity, Box);

		//if (Menu::Window.VisualsTab.HealthText.GetState())
		//	DrawHealthText(pinfo, Box, pEntity);

		if (Menu::Window.VisualsTab.HealthText.GetState())
			DrawHealth(pEntity, Box);

		if (Menu::Window.VisualsTab.Tracers.GetState())
			Tracers(Box, Color, pEntity);

		if (Menu::Window.VisualsTab.Active.GetState() && Menu::Window.VisualsTab.OptionsWeapon.GetState())
			DrawInfo(pEntity, Box);

		if (Menu::Window.VisualsTab.OptionsAimSpot.GetState())
			DrawCross(pEntity);

		if (Menu::Window.VisualsTab.OptionsSkeleton.GetState())
			DrawSkeleton(pEntity);

		if (Menu::Window.VisualsTab.AmmoText.GetState())
			Ammo(pEntity, pinfo, Box);

		if (Menu::Window.VisualsTab.GrenadePrediction.GetState())
			DiLight();
	}
}



// Gets the 2D bounding box for the entity
// Returns false on failure nigga don't fail me
bool CEsp::GetBox(IClientEntity* pEntity, CEsp::ESPBox &result)
{
	// Variables
	Vector  vOrigin, min, max, sMin, sMax, sOrigin,
		flb, brt, blb, frt, frb, brb, blt, flt;
	float left, top, right, bottom;

	// Get the locations
	vOrigin = pEntity->GetOrigin();
	min = pEntity->collisionProperty()->GetMins() + vOrigin;
	max = pEntity->collisionProperty()->GetMaxs() + vOrigin;

	// Points of a 3d bounding box
	Vector points[] = { Vector(min.x, min.y, min.z),
		Vector(min.x, max.y, min.z),
		Vector(max.x, max.y, min.z),
		Vector(max.x, min.y, min.z),
		Vector(max.x, max.y, max.z),
		Vector(min.x, max.y, max.z),
		Vector(min.x, min.y, max.z),
		Vector(max.x, min.y, max.z) };

	// Get screen positions
	if (!Render::WorldToScreen(points[3], flb) || !Render::WorldToScreen(points[5], brt)
		|| !Render::WorldToScreen(points[0], blb) || !Render::WorldToScreen(points[4], frt)
		|| !Render::WorldToScreen(points[2], frb) || !Render::WorldToScreen(points[1], brb)
		|| !Render::WorldToScreen(points[6], blt) || !Render::WorldToScreen(points[7], flt))
		return false;

	// Put them in an array (maybe start them off in one later for speed?)
	Vector arr[] = { flb, brt, blb, frt, frb, brb, blt, flt };

	// Init this shit
	left = flb.x;
	top = flb.y;
	right = flb.x;
	bottom = flb.y;

	// Find the bounding corners for our box
	for (int i = 1; i < 8; i++)
	{
		if (left > arr[i].x)
			left = arr[i].x;
		if (bottom < arr[i].y)
			bottom = arr[i].y;
		if (right < arr[i].x)
			right = arr[i].x;
		if (top > arr[i].y)
			top = arr[i].y;
	}

	// Width / height
	result.x = left;
	result.y = top;
	result.w = right - left;
	result.h = bottom - top;

	return true;
}


// Get an entities color depending on team and vis ect
Color CEsp::GetPlayerColor(IClientEntity* pEntity)
{
//#define hitboxes (int)CSGOHitboxID::Head, (int)CSGOHitboxID::Neck, (int)CSGOHitboxID::NeckLower, (int)CSGOHitboxID::Stomach, (int)CSGOHitboxID::Pelvis, (int)CSGOHitboxID::UpperChest,(int)CSGOHitboxID::Chest, (int)CSGOHitboxID::LowerChest,(int)CSGOHitboxID::LeftUpperArm, (int)CSGOHitboxID::RightUpperArm, (int)CSGOHitboxID::LeftThigh, (int)CSGOHitboxID::RightThigh, (int)CSGOHitboxID::LeftHand, (int)CSGOHitboxID::RightHand, (int)CSGOHitboxID::LeftFoot, (int)CSGOHitboxID::RightFoot, (int)CSGOHitboxID::LeftShin, (int)CSGOHitboxID::RightShin, (int)CSGOHitboxID::LeftLowerArm, (int)CSGOHitboxID::RightLowerArm
	int TeamNum = pEntity->GetTeamNum();
	bool IsVis;
	if (GameUtils::IsVisible(hackManager.pLocal(), pEntity, (int)CSGOHitboxID::Chest) || GameUtils::IsVisible(hackManager.pLocal(), pEntity, (int)CSGOHitboxID::Head) || GameUtils::IsVisible(hackManager.pLocal(), pEntity, (int)CSGOHitboxID::Neck) || GameUtils::IsVisible(hackManager.pLocal(), pEntity, (int)CSGOHitboxID::NeckLower) || GameUtils::IsVisible(hackManager.pLocal(), pEntity, (int)CSGOHitboxID::Pelvis) || GameUtils::IsVisible(hackManager.pLocal(), pEntity, (int)CSGOHitboxID::Chest) || GameUtils::IsVisible(hackManager.pLocal(), pEntity, (int)CSGOHitboxID::LeftFoot) || GameUtils::IsVisible(hackManager.pLocal(), pEntity, (int)CSGOHitboxID::LeftHand) || GameUtils::IsVisible(hackManager.pLocal(), pEntity, (int)CSGOHitboxID::LeftLowerArm) || GameUtils::IsVisible(hackManager.pLocal(), pEntity, (int)CSGOHitboxID::LeftShin) || GameUtils::IsVisible(hackManager.pLocal(), pEntity, (int)CSGOHitboxID::LeftThigh) || GameUtils::IsVisible(hackManager.pLocal(), pEntity, (int)CSGOHitboxID::LeftUpperArm) || GameUtils::IsVisible(hackManager.pLocal(), pEntity, (int)CSGOHitboxID::LowerChest) || GameUtils::IsVisible(hackManager.pLocal(), pEntity, (int)CSGOHitboxID::RightFoot) || GameUtils::IsVisible(hackManager.pLocal(), pEntity, (int)CSGOHitboxID::RightHand) || GameUtils::IsVisible(hackManager.pLocal(), pEntity, (int)CSGOHitboxID::RightLowerArm) || GameUtils::IsVisible(hackManager.pLocal(), pEntity, (int)CSGOHitboxID::RightShin) || GameUtils::IsVisible(hackManager.pLocal(), pEntity, (int)CSGOHitboxID::RightThigh) || GameUtils::IsVisible(hackManager.pLocal(), pEntity, (int)CSGOHitboxID::RightUpperArm))
	{
		IsVis = true;
	}
	else
	{
		IsVis = false;
	}
	
	Color color;

	


	return color;
}
static float lastDistance;
static float newDistance;
void CEsp::Ammo(IClientEntity* pEntity, player_info_t pinfo, CEsp::ESPBox size)
{

	//IClientEntity *pLocal = hackManager.pLocal();

	CBaseCombatWeapon* CSWeapon = (CBaseCombatWeapon*)Interfaces::EntList->GetClientEntityFromHandle(pEntity->GetActiveWeaponHandle());

	RECT nameSize = Render::GetTextSize(Render::Fonts::ESP, "");
	Render::Text(size.x + 3 + size.w + 1, size.y + 20, Color(255, 255, 255, 255), Render::Fonts::ESP, "Ammo: ");

	int a = CSWeapon->GetAmmoInClip();
	int radix = 10;
	char buffer[20];
	char *p;

	p = itoa(a, buffer, radix);
	Render::Text(size.x + 3 + size.w + 38, size.y + 20, Color(255, 255, 255, 255), Render::Fonts::ESP, p);

	char ScopeResponse[32];
	int DC;
	if (pEntity->IsScoped())
	{
		sprintf_s(ScopeResponse, "Scoped");
		DC = 0;
	}
	else if (!pEntity->IsScoped())
	{
		sprintf_s(ScopeResponse, "Not Scoped");
		DC = 255;
	}
	Render::Text(size.x + size.w + 3, size.y + 10, Color(255, DC, DC, 255), Render::Fonts::ESP, ScopeResponse);
}

void CEsp::DrawBullets() {

	Ray_t ray;
	Vector bonePosEnemy, bonePosMe;

	if (Globals::Target <= 0) {
		return;
	}

	CTraceFilter filter;
	filter.pSkip = hackManager.pLocal();


	bonePosMe = hackManager.pLocal()->GetBonePos(8);
	bonePosMe += Vector(0, 0, 5);
	bonePosEnemy = Globals::Target->GetBonePos(77);


	ray.Init(bonePosMe, bonePosEnemy);
	trace_t trace;

	Interfaces::Trace->TraceRay(ray, MASK_SHOT_HULL | CONTENTS_HITBOX, &filter, &trace);
	if (trace.fraction != 1.0f) 
	{
		// Hit Enemy
		Vector start, end;
		if (!Render::WorldToScreen(trace.startpos, start) || !Render::WorldToScreen(trace.endpos, end))
			return;
		Render::Line(start.x, start.y, end.x, end.y, Color(0, 255, 0, 255));

	}
	
}

#include "lagcomp.h"

void CEsp::Tracers(CEsp::ESPBox size, Color color, IClientEntity* pEntity)
{
	Vector src3D, src;
	src3D = pEntity->GetOrigin() - Vector(0, 0, 0);

	if (!Render::WorldToScreen(src3D, src))
		return;

	int ScreenWidth, ScreenHeight;
	Interfaces::Engine->GetScreenSize(ScreenWidth, ScreenHeight);

	int x = (int)(ScreenWidth * 0.5f);
	int y = 0;

	y = ScreenHeight;

	Render::Line((int)(src.x), (int)(src.y), x, y, Color(Menu::Window.SettingsTab.SkeletonR.GetValue(), Menu::Window.SettingsTab.SkeletonG.GetValue(), Menu::Window.SettingsTab.SkeletonB.GetValue(), 255));
}



void CEsp::drawBacktrackedSkelet(IClientEntity *base)
{
	IClientEntity *player = base;
	IClientEntity *locPlyr = hackManager.pLocal();
	if (!locPlyr)
		return;

	int idx = player->GetIndex();

	LagRecord_A *m_LagRecords = lagComp->m_LagRecord[idx];
	LagRecord_A recentLR = m_LagRecords[3];
	
	Vector screenSpot4;
	if (Render::WorldToScreen(recentLR.headSpot, screenSpot4))
	{
		if (player->GetTeamNum() != locPlyr->GetTeamNum())
			Render::fillRGBA(screenSpot4.x, screenSpot4.y, 3, 3, Color(255, 0, 235, 255));
	} 
}


void CEsp::DrawLinesAA(Color color) {
	Vector src3D, dst3D, forward, src, dst;
	trace_t tr;
	Ray_t ray;
	CTraceFilter filter;

	filter.pSkip = hackManager.pLocal();

	// LBY
	AngleVectors(QAngle(0, lineLBY, 0), &forward);
	src3D = hackManager.pLocal()->GetOrigin();
	dst3D = src3D + (forward * 30.f); //replace 50 with the length you want the line to have

	ray.Init(src3D, dst3D);

	Interfaces::Trace->TraceRay(ray, 0, &filter, &tr);

	if (!Render::WorldToScreen(src3D, src) || !Render::WorldToScreen(tr.endpos, dst))
		return;

	Render::Line(src.x, src.y, dst.x, dst.y, Color(210, 105, 30, 255));
	// REAL AGNEL
	AngleVectors(QAngle(0, lineRealAngle, 0), &forward);
	dst3D = src3D + (forward * 30.f); //replace 50 with the length you want the line to have

	ray.Init(src3D, dst3D);

	Interfaces::Trace->TraceRay(ray, 0, &filter, &tr);

	if (!Render::WorldToScreen(src3D, src) || !Render::WorldToScreen(tr.endpos, dst))
		return;

	Render::Line(src.x, src.y, dst.x, dst.y, Color(0, 255, 0, 255));

	// Fake AGNEL
	AngleVectors(QAngle(0, lineFakeAngle, 0), &forward);
	dst3D = src3D + (forward * 30.f); //replace 50 with the length you want the line to have

	ray.Init(src3D, dst3D);

	Interfaces::Trace->TraceRay(ray, 0, &filter, &tr);

	if (!Render::WorldToScreen(src3D, src) || !Render::WorldToScreen(tr.endpos, dst))
		return;

	Render::Line(src.x, src.y, dst.x, dst.y, Color(255, 0, 0, 255));
}


// 2D  Esp box
void CEsp::DrawBox(CEsp::ESPBox size, Color color)
{
	int VertLine = (((float)size.w) * (1.0f));
	int BHorzLine = (((float)size.h) * (1.0f));

	Render::Clear(size.x, size.y - 1, VertLine, 1, Color(Menu::Window.SettingsTab.BoxR.GetValue(), Menu::Window.SettingsTab.BoxG.GetValue(), Menu::Window.SettingsTab.BoxB.GetValue(), 255));
	Render::Clear(size.x, size.y + size.h - 1, VertLine, 1, Color(Menu::Window.SettingsTab.BoxR.GetValue(), Menu::Window.SettingsTab.BoxG.GetValue(), Menu::Window.SettingsTab.BoxB.GetValue(), 255));
	Render::Clear(size.x - 1, size.y, 1, BHorzLine, Color(Menu::Window.SettingsTab.BoxR.GetValue(), Menu::Window.SettingsTab.BoxG.GetValue(), Menu::Window.SettingsTab.BoxB.GetValue(), 255));
	Render::Clear(size.x + size.w - 1, size.y, 1, BHorzLine, Color(Menu::Window.SettingsTab.BoxR.GetValue(), Menu::Window.SettingsTab.BoxG.GetValue(), Menu::Window.SettingsTab.BoxB.GetValue(), 255));

	Render::Clear(size.x, size.y, VertLine, 1, color);
	Render::Clear(size.x + size.w - VertLine, size.y, VertLine, 1, color);
	Render::Clear(size.x, size.y + size.h, VertLine, 1, color);
	Render::Clear(size.x + size.w - VertLine, size.y + size.h, VertLine, 1, color);

	Render::Clear(size.x, size.y, 1, BHorzLine, color);
	Render::Clear(size.x, size.y + size.h - BHorzLine, 1, BHorzLine, color);
	Render::Clear(size.x + size.w, size.y, 1, BHorzLine, color);
	Render::Clear(size.x + size.w, size.y + size.h - BHorzLine, 1, BHorzLine, color);
}


void CEsp::DrawBrackets(CEsp::ESPBox size, Color color)
{
	int VertLine = (((float)size.w) * (0.25f));
	int BHorzLine = (((float)size.h) * (1.0f));

	Render::Clear(size.x, size.y - 1, VertLine, 1, Color(Menu::Window.SettingsTab.BoxR.GetValue(), Menu::Window.SettingsTab.BoxG.GetValue(), Menu::Window.SettingsTab.BoxB.GetValue(), 255));
	Render::Clear(size.x + size.w - VertLine, size.y - 1, VertLine, 1, Color(Menu::Window.SettingsTab.BoxR.GetValue(), Menu::Window.SettingsTab.BoxG.GetValue(), Menu::Window.SettingsTab.BoxB.GetValue(), 255));
	Render::Clear(size.x, size.y + size.h - 1, VertLine, 1, Color(Menu::Window.SettingsTab.BoxR.GetValue(), Menu::Window.SettingsTab.BoxG.GetValue(), Menu::Window.SettingsTab.BoxB.GetValue(), 255));
	Render::Clear(size.x + size.w - VertLine, size.y + size.h - 1, VertLine, 1, Color(Menu::Window.SettingsTab.BoxR.GetValue(), Menu::Window.SettingsTab.BoxG.GetValue(), Menu::Window.SettingsTab.BoxB.GetValue(), 255));

	Render::Clear(size.x - 1, size.y, 1, BHorzLine, Color(Menu::Window.SettingsTab.BoxR.GetValue(), Menu::Window.SettingsTab.BoxG.GetValue(), Menu::Window.SettingsTab.BoxB.GetValue(), 255));
	Render::Clear(size.x + size.w - 1, size.y, 1, BHorzLine, Color(Menu::Window.SettingsTab.BoxR.GetValue(), Menu::Window.SettingsTab.BoxG.GetValue(), Menu::Window.SettingsTab.BoxB.GetValue(), 255));

	Render::Clear(size.x, size.y, VertLine, 1, color);
	Render::Clear(size.x + size.w - VertLine, size.y, VertLine, 1, color);
	Render::Clear(size.x, size.y + size.h, VertLine, 1, color);
	Render::Clear(size.x + size.w - VertLine, size.y + size.h, VertLine, 1, color);

	Render::Clear(size.x, size.y, 1, BHorzLine, color);
	Render::Clear(size.x, size.y + size.h - BHorzLine, 1, BHorzLine, color);
	Render::Clear(size.x + size.w, size.y, 1, BHorzLine, color);
	Render::Clear(size.x + size.w, size.y + size.h - BHorzLine, 1, BHorzLine, color);
}

void CEsp::DrawCorners(CEsp::ESPBox size, Color color)
{
	int VertLine = (((float)size.w) * (0.20f));
	int BHorzLine = (((float)size.h) * (0.30f));

	Render::Clear(size.x, size.y - 1, VertLine, 1, Color(Menu::Window.SettingsTab.BoxR.GetValue(), Menu::Window.SettingsTab.BoxG.GetValue(), Menu::Window.SettingsTab.BoxB.GetValue(), 255));
	Render::Clear(size.x + size.w - VertLine, size.y - 1, VertLine, 1, Color(Menu::Window.SettingsTab.BoxR.GetValue(), Menu::Window.SettingsTab.BoxG.GetValue(), Menu::Window.SettingsTab.BoxB.GetValue(), 255));
	Render::Clear(size.x, size.y + size.h - 1, VertLine, 1, Color(Menu::Window.SettingsTab.BoxR.GetValue(), Menu::Window.SettingsTab.BoxG.GetValue(), Menu::Window.SettingsTab.BoxB.GetValue(), 255));
	Render::Clear(size.x + size.w - VertLine, size.y + size.h - 1, VertLine, 1, Color(Menu::Window.SettingsTab.BoxR.GetValue(), Menu::Window.SettingsTab.BoxG.GetValue(), Menu::Window.SettingsTab.BoxB.GetValue(), 255));

	Render::Clear(size.x - 1, size.y, 1, BHorzLine, Color(Menu::Window.SettingsTab.BoxR.GetValue(), Menu::Window.SettingsTab.BoxG.GetValue(), Menu::Window.SettingsTab.BoxB.GetValue(), 255));
	Render::Clear(size.x - 1, size.y + size.h - BHorzLine, 1, BHorzLine, Color(Menu::Window.SettingsTab.BoxR.GetValue(), Menu::Window.SettingsTab.BoxG.GetValue(), Menu::Window.SettingsTab.BoxB.GetValue(), 255));
	Render::Clear(size.x + size.w - 1, size.y, 1, BHorzLine, Color(Menu::Window.SettingsTab.BoxR.GetValue(), Menu::Window.SettingsTab.BoxG.GetValue(), Menu::Window.SettingsTab.BoxB.GetValue(), 255));
	Render::Clear(size.x + size.w - 1, size.y + size.h - BHorzLine, 1, BHorzLine, Color(Menu::Window.SettingsTab.BoxR.GetValue(), Menu::Window.SettingsTab.BoxG.GetValue(), Menu::Window.SettingsTab.BoxB.GetValue(), 255));

	Render::Clear(size.x, size.y, VertLine, 1, color);
	Render::Clear(size.x + size.w - VertLine, size.y, VertLine, 1, color);
	Render::Clear(size.x, size.y + size.h, VertLine, 1, color);
	Render::Clear(size.x + size.w - VertLine, size.y + size.h, VertLine, 1, color);

	Render::Clear(size.x, size.y, 1, BHorzLine, color);
	Render::Clear(size.x, size.y + size.h - BHorzLine, 1, BHorzLine, color);
	Render::Clear(size.x + size.w, size.y, 1, BHorzLine, color);
	Render::Clear(size.x + size.w, size.y + size.h - BHorzLine, 1, BHorzLine, color);
}

void CEsp::DrawFilled(CEsp::ESPBox size, Color color)
{
	int VertLine = (((float)size.w) * (0.20f));
	int BHorzLine = (((float)size.h) * (0.20f));

	Render::Clear(size.x + 1, size.y + 1, size.w - 2, size.h - 2, Color(0, 0, 0, 40));
	Render::Clear(size.x + 1, size.y + 1, size.w - 2, size.h - 2, Color(0, 0, 0, 40));
	Render::Clear(size.x, size.y, VertLine, 1, color);
	Render::Clear(size.x + size.w - VertLine, size.y, VertLine, 1, color);
	Render::Clear(size.x, size.y + size.h, VertLine, 1, color);
	Render::Clear(size.x + size.w - VertLine, size.y + size.h, VertLine, 1, color);
	Render::Clear(size.x + 1, size.y + 1, size.w - 2, size.h - 2, Color(0, 0, 0, 40));
	Render::Clear(size.x, size.y, 1, BHorzLine, color);
	Render::Clear(size.x, size.y + size.h - BHorzLine, 1, BHorzLine, color);
	Render::Clear(size.x + size.w, size.y, 1, BHorzLine, color);
	Render::Clear(size.x + size.w, size.y + size.h - BHorzLine, 1, BHorzLine, color);
	Render::Clear(size.x + 1, size.y + 1, size.w - 2, size.h - 2, Color(0, 0, 0, 40));
}

void CEsp::DrawCustom(CEsp::ESPBox size, Color color, IClientEntity* pEntity)
{

	int HPEnemy = 100;
	HPEnemy = pEntity->GetHealth();
	char nameBuffer[512];
	sprintf_s(nameBuffer, "%d", HPEnemy);

	float h = (size.h);
	float offset = (h / 4.f) + 5;
	float w = h / 64.f;
	float health = pEntity->GetHealth();
	UINT hp = h - (UINT)((h * health) / 100);

	int Red = 255 - (health*2.55);
	int Green = health*2.55;


	int VertLine = (((float)size.w) * (0.20f));
	int HorzLine = (((float)size.h) * (0.20f));

	int BVertLine = (((float)size.w) * (0.25f));
	int BHorzLine = (((float)size.h) * (1.0f));

	// Brackets
	Render::Clear(size.x, size.y - 1, BVertLine, 1, Color(Menu::Window.SettingsTab.BoxR.GetValue(), Menu::Window.SettingsTab.BoxG.GetValue(), Menu::Window.SettingsTab.BoxB.GetValue(), 255));
	Render::Clear(size.x + size.w - BVertLine, size.y - 1, BVertLine, 1, Color(Menu::Window.SettingsTab.BoxR.GetValue(), Menu::Window.SettingsTab.BoxG.GetValue(), Menu::Window.SettingsTab.BoxB.GetValue(), 255));
	Render::Clear(size.x, size.y + size.h - 1, BVertLine, 1, Color(Menu::Window.SettingsTab.BoxR.GetValue(), Menu::Window.SettingsTab.BoxG.GetValue(), Menu::Window.SettingsTab.BoxB.GetValue(), 255));
	Render::Clear(size.x + size.w - BVertLine, size.y + size.h - 1, BVertLine, 1, Color(Menu::Window.SettingsTab.BoxR.GetValue(), Menu::Window.SettingsTab.BoxG.GetValue(), Menu::Window.SettingsTab.BoxB.GetValue(), 255));
	Render::Clear(size.x - 1, size.y, 1, BHorzLine, Color(Menu::Window.SettingsTab.BoxR.GetValue(), Menu::Window.SettingsTab.BoxG.GetValue(), Menu::Window.SettingsTab.BoxB.GetValue(), 255));
	Render::Clear(size.x + size.w - 1, size.y, 1, BHorzLine, Color(Menu::Window.SettingsTab.BoxR.GetValue(), Menu::Window.SettingsTab.BoxG.GetValue(), Menu::Window.SettingsTab.BoxB.GetValue(), 255));
	Render::Clear(size.x, size.y, BVertLine, 1, color);
	Render::Clear(size.x + size.w - BVertLine, size.y, BVertLine, 1, color);
	Render::Clear(size.x, size.y + size.h, BVertLine, 1, color);
	Render::Clear(size.x + size.w - BVertLine, size.y + size.h, BVertLine, 1, color);
	Render::Clear(size.x, size.y, 1, BHorzLine, color);
	Render::Clear(size.x, size.y + size.h - BHorzLine, 1, BHorzLine, color);
	Render::Clear(size.x + size.w, size.y, 1, BHorzLine, color);
	Render::Clear(size.x + size.w, size.y + size.h - BHorzLine, 1, BHorzLine, color);


	// Filled box
	Render::Clear(size.x + 1, size.y + 1, size.w - 2, size.h - 2, Color(0, 0, 0, 40));
	Render::Clear(size.x + 1, size.y + 1, size.w - 2, size.h - 2, Color(0, 0, 0, 40));
	Render::Clear(size.x, size.y, VertLine, 1, color);
	Render::Clear(size.x + size.w - VertLine, size.y, VertLine, 1, color);
	Render::Clear(size.x, size.y + size.h, VertLine, 1, color);
	Render::Clear(size.x + size.w - VertLine, size.y + size.h, VertLine, 1, color);
	Render::Clear(size.x + 1, size.y + 1, size.w - 2, size.h - 2, Color(0, 0, 0, 40));
	Render::Clear(size.x, size.y, 1, BHorzLine, color);
	Render::Clear(size.x, size.y + size.h - BHorzLine, 1, BHorzLine, color);
	Render::Clear(size.x + size.w, size.y, 1, BHorzLine, color);
	Render::Clear(size.x + size.w, size.y + size.h - BHorzLine, 1, BHorzLine, color);
	Render::Clear(size.x + 1, size.y + 1, size.w - 2, size.h - 2, Color(0, 0, 0, 40));
}



void CEsp::DrawTopCustom(CEsp::ESPBox size, Color color, IClientEntity* pEntity)
{

	int HPEnemy = 100;
	HPEnemy = pEntity->GetHealth();
	char nameBuffer[512];
	sprintf_s(nameBuffer, "%d", HPEnemy);

	float h = (size.h);
	float offset = (h / 4.f) + 5;
	float w = h / 64.f;
	float health = pEntity->GetHealth();
	UINT hp = h - (UINT)((h * health) / 100);

	int Red = 255 - (health*2.55);
	int Green = health*2.55;


	int VertLine = (((float)size.w) * (0.20f));
	int HorzLine = (((float)size.h) * (0.20f));

	int BVertLine = (((float)size.w) * (1.0f));
	int BHorzLine = (((float)size.h) * (0.3f));

	// Brackets
	Render::Clear(size.x, size.y - 1, BVertLine, 1, Color(Menu::Window.SettingsTab.BoxR.GetValue(), Menu::Window.SettingsTab.BoxG.GetValue(), Menu::Window.SettingsTab.BoxB.GetValue(), 255));
	Render::Clear(size.x + size.w - BVertLine, size.y - 1, BVertLine, 1, Color(Menu::Window.SettingsTab.BoxR.GetValue(), Menu::Window.SettingsTab.BoxG.GetValue(), Menu::Window.SettingsTab.BoxB.GetValue(), 255));
	Render::Clear(size.x, size.y + size.h - 1, BVertLine, 1, Color(Menu::Window.SettingsTab.BoxR.GetValue(), Menu::Window.SettingsTab.BoxG.GetValue(), Menu::Window.SettingsTab.BoxB.GetValue(), 255));
	Render::Clear(size.x + size.w - BVertLine, size.y + size.h - 1, BVertLine, 1, Color(Menu::Window.SettingsTab.BoxR.GetValue(), Menu::Window.SettingsTab.BoxG.GetValue(), Menu::Window.SettingsTab.BoxB.GetValue(), 255));

	Render::Clear(size.x - 1, size.y, 1, BHorzLine, Color(Menu::Window.SettingsTab.BoxR.GetValue(), Menu::Window.SettingsTab.BoxG.GetValue(), Menu::Window.SettingsTab.BoxB.GetValue(), 255));
	Render::Clear(size.x - 1, size.y + size.h - BHorzLine, 1, BHorzLine, Color(Menu::Window.SettingsTab.BoxR.GetValue(), Menu::Window.SettingsTab.BoxG.GetValue(), Menu::Window.SettingsTab.BoxB.GetValue(), 255));
	Render::Clear(size.x + size.w - 1, size.y, 1, BHorzLine, Color(Menu::Window.SettingsTab.BoxR.GetValue(), Menu::Window.SettingsTab.BoxG.GetValue(), Menu::Window.SettingsTab.BoxB.GetValue(), 255));
	Render::Clear(size.x + size.w - 1, size.y + size.h - BHorzLine, 1, BHorzLine, Color(Menu::Window.SettingsTab.BoxR.GetValue(), Menu::Window.SettingsTab.BoxG.GetValue(), Menu::Window.SettingsTab.BoxB.GetValue(), 255));

	Render::Clear(size.x, size.y, BVertLine, 1, color);
	Render::Clear(size.x + size.w - BVertLine, size.y, BVertLine, 1, color);
	Render::Clear(size.x, size.y + size.h, BVertLine, 1, color);
	Render::Clear(size.x + size.w - BVertLine, size.y + size.h, BVertLine, 1, color);
	Render::Clear(size.x, size.y, 1, BHorzLine, color);
	Render::Clear(size.x, size.y + size.h - BHorzLine, 1, BHorzLine, color);
	Render::Clear(size.x + size.w, size.y, 1, BHorzLine, color);
	Render::Clear(size.x + size.w, size.y + size.h - BHorzLine, 1, BHorzLine, color);


	// Filled box
	Render::Clear(size.x + 1, size.y + 1, size.w - 2, size.h - 2, Color(0, 0, 0, 40));
	Render::Clear(size.x + 1, size.y + 1, size.w - 2, size.h - 2, Color(0, 0, 0, 40));
	Render::Clear(size.x, size.y, VertLine, 1, color);
	Render::Clear(size.x + size.w - VertLine, size.y, VertLine, 1, color);
	Render::Clear(size.x, size.y + size.h, VertLine, 1, color);
	Render::Clear(size.x + size.w - VertLine, size.y + size.h, VertLine, 1, color);
	Render::Clear(size.x + 1, size.y + 1, size.w - 2, size.h - 2, Color(0, 0, 0, 40));
	Render::Clear(size.x, size.y, 1, BHorzLine, color);
	Render::Clear(size.x, size.y + size.h - BHorzLine, 1, BHorzLine, color);
	Render::Clear(size.x + size.w, size.y, 1, BHorzLine, color);
	Render::Clear(size.x + size.w, size.y + size.h - BHorzLine, 1, BHorzLine, color);
	Render::Clear(size.x + 1, size.y + 1, size.w - 2, size.h - 2, Color(0, 0, 0, 40));
}






void DiLight()
{
	IClientEntity *pLocal = hackManager.pLocal();
	IClientEntity *pEntity = hackManager.pLocal();
	CBaseCombatWeapon* pWeapon = (CBaseCombatWeapon*)Interfaces::EntList->GetClientEntityFromHandle(pLocal->GetActiveWeaponHandle());
	const float TIMEALIVE = 5.f;
	const float GRENADE_COEFFICIENT_OF_RESTITUTION = 0.4f;

	float fStep = 0.1f;
	float fGravity = 800.0f / 8.f;

	Vector vPos;
	Vector vStart;
	Vector vThrow;
	Vector vThrow2;
	int iCollisions = 0;

	Vector vViewAngles;
	Interfaces::Engine->GetViewAngles(vViewAngles);

	vThrow[0] = vViewAngles[0];
	vThrow[1] = vViewAngles[1];
	vThrow[2] = vViewAngles[2];

	if (vThrow[0] < 0)
		vThrow[0] = -10 + vThrow[0] * ((90 - 10) / 90.0);
	else
		vThrow[0] = -10 + vThrow[0] * ((90 + 10) / 90.0);

	float fVel = (90 - vThrow[0]) * 4;
	if (fVel > 500)
		fVel = 500;

	AngleVectors(vThrow, &vThrow2);

	Vector vEye = pEntity->GetEyePosition();
	vStart[0] = vEye[0] + vThrow2[0] * 16;
	vStart[1] = vEye[1] + vThrow2[1] * 16;
	vStart[2] = vEye[2] + vThrow2[2] * 16;

	vThrow2[0] = (vThrow2[0] * fVel) + pEntity->GetVelocity()[0];
	vThrow2[1] = (vThrow2[1] * fVel) + pEntity->GetVelocity()[1];
	vThrow2[2] = (vThrow2[2] * fVel) + pEntity->GetVelocity()[2]; // casualhacker for da magic calc help

	for (float fTime = 0.0f; fTime < TIMEALIVE; fTime += fStep)
	{
		vPos = vStart + vThrow2 * fStep;

		Ray_t ray;
		trace_t tr;

		CTraceFilter loc;
		loc.pSkip = pEntity;

		ray.Init(vStart, vPos);
		Interfaces::Trace->TraceRay(ray, MASK_SOLID, &loc, &tr);

		if (tr.DidHit()) // if the trace hits a surface
		{
			//float proj = DotProduct(throwvec, tr.plane.normal);
			vThrow2 = tr.plane.normal * -2.0f *DotProduct(vThrow2, tr.plane.normal) + vThrow2;
			vThrow2 *= GRENADE_COEFFICIENT_OF_RESTITUTION;

			iCollisions++;
			if (iCollisions > 2)
				break;

			vPos = vStart + vThrow2*tr.fraction*fStep;

			fTime += (fStep * (1 - tr.fraction));
		} // end of bounce

		Vector vOutStart, vOutEnd;
		if (GameUtils::IsG(pWeapon))
		{
			if (Render::WorldToScreen(vStart, vOutStart), Render::WorldToScreen(vPos, vOutEnd))
			{
				Render::Line(vOutStart.x, vOutStart.y, vOutEnd.x, vOutEnd.y, Color(0, 150, 255, 255));
			}

			vStart = vPos;
			vThrow2.z -= fGravity*tr.fraction*fStep;
		}
	}
}


// Unicode Conversions
static wchar_t* CharToWideChar(const char* text)
{
	size_t size = strlen(text) + 1;
	wchar_t* wa = new wchar_t[size];
	mbstowcs_s(NULL, wa, size/4, text, size);
	return wa;
}
void CEsp::DrawArmor(IClientEntity* pEntity, CEsp::ESPBox size)
{
	ESPBox ArmorBar = size;
	ArmorBar.y += (ArmorBar.h + 6);
	ArmorBar.h = 4;

	float ArmorValue = pEntity->ArmorValue();
	float ArmorPerc = ArmorValue / 100.f;
	float Width = (size.w * ArmorPerc);
	ArmorBar.w = Width;

	int armor = pEntity->ArmorValue();

	if (armor > 100)
		armor = 100;
	/* I dont want a change yellow is good */
	//int r = 255 - armor * 2.55;
	//int g = armor * 2.55;

	int armorBar = size.h / 100 * armor;
	int armorBarDelta = size.h - armorBar;

	if (!armor < 1)
	{
		Render::Clear(size.x - 10, size.y - 1, 2, size.h + 2, Color(0, 0, 0, 150));
		Render::Clear(size.x - 9, size.y + 1, 1, size.h * 0.01 * armor, Color(0, 150, 255, 255));
	}
}

void CEsp::DrawHealthText(player_info_t pinfo, CEsp::ESPBox size, IClientEntity* pEntity)
{
	int HPEnemy = 100;
	HPEnemy = pEntity->GetHealth();
	char nameBuffer[512];
	sprintf_s(nameBuffer, "%d", HPEnemy);

	float h = (size.h);
	float offset = (h / 4.f) + 5;
	float w = h / 64.f;
	float health = pEntity->GetHealth();
	UINT hp = h - (UINT)((h * health) / 100);

	int Red = 255 - (health*2.55);
	int Green = health*2.55;

	int VertLine = (((float)size.w) * (0.25f));
	int BHorzLine = (((float)size.h) * (1.0f));

	RECT nameSize = Render::GetTextSize(Render::Fonts::ESP, pinfo.name);

	float HealthValue = pEntity->GetHealth();
	float HealthPerc = HealthValue;
	char faszommeretejobbra_cmbe[64];
	sprintf_s(faszommeretejobbra_cmbe, "HP: %0.0f", HealthPerc);
	Render::Text(size.x + size.w + 3, size.y - 10, Color(Red, Green, 0, 255), Render::Fonts::ESP, faszommeretejobbra_cmbe);

}

// Player name
void CEsp::DrawName(player_info_t pinfo, CEsp::ESPBox size, IClientEntity* pEntity)
{

	RECT nameSize = Render::GetTextSize(Render::Fonts::ESP, pinfo.name);
	if (strlen(pinfo.name) > 16)
	{
		pinfo.name[16] = 0;
		strcat(pinfo.name, "...");
		puts(pinfo.name);
		RECT nameSize = Render::GetTextSize(Render::Fonts::ESP, pinfo.name);
		Render::Text(size.x + (size.w / 2) - (nameSize.right / 2), size.y - 12, Color(255, 255, 255, 255), Render::Fonts::MyMain, pinfo.name);
	}
	else
	{
		RECT nameSize = Render::GetTextSize(Render::Fonts::ESP, pinfo.name);
		Render::Text(size.x + (size.w / 2) - (nameSize.right / 2), size.y - 12, Color(255, 255, 255, 255), Render::Fonts::MyMain, pinfo.name);
	}

	if (Menu::Window.VisualsTab.OptionsHealth.GetState())
	{
		static RECT Size = Render::GetTextSize(Render::Fonts::Default, "Hi");
	}
}


void CEsp::DrawGrenades(IClientEntity* pEntity)
{
	auto granade = Interfaces::CVar->FindVar("sv_grenade_trajectory");
	auto granadespoof = new SpoofedConvar(granade);
	granadespoof->SetInt(1);
}


// Draw a health bar. For Tf2 when a bar is bigger than max health a second bar is displayed
void CEsp::DrawHealth(IClientEntity* pEntity, CEsp::ESPBox size)
{
	int HPEnemy = 100;
	HPEnemy = pEntity->GetHealth();
	char nameBuffer[512];
	sprintf_s(nameBuffer, "%d", HPEnemy);


	float h = (size.h);
	float offset = (h / 4.f) + 5;
	float w = h / 64.f;
	float health = pEntity->GetHealth();
	UINT hp = h - (UINT)((h * health) / 100);

	int Red = 255 - (health*2.55);
	int Green = health*2.55;

	Render::DrawOutlinedRect((size.x - 6) - 1, size.y - 1, 3, h + 2, Color(0, 0, 0, 180));

	Render::Line((size.x - 6), size.y + hp, (size.x - 6), size.y + h, Color(Red, Green, 0, 180));



	if (health < 100) {

		Render::Text(size.x - 9, size.y + hp, Color(255, 255, 255, 255), Render::Fonts::ESP, nameBuffer);
	}

}

// Cleans the internal class name up to something human readable and nice
std::string CleanItemName(std::string name)
{
	std::string Name = name;
	// Tidy up the weapon Name
	if (Name[0] == 'C')
		Name.erase(Name.begin());

	// Remove the word Weapon
	auto startOfWeap = Name.find("Weapon");
	if (startOfWeap != std::string::npos)
		Name.erase(Name.begin() + startOfWeap, Name.begin() + startOfWeap + 6);

	return Name;
}

// Anything else: weapons, class state? idk
void CEsp::DrawInfo(IClientEntity* pEntity, CEsp::ESPBox size)
{
	std::vector<std::string> Info;

	// Player Weapon ESP
	IClientEntity* pWeapon = Interfaces::EntList->GetClientEntityFromHandle((HANDLE)pEntity->GetActiveWeaponHandle());
	if (Menu::Window.VisualsTab.OptionsWeapon.GetState() && pWeapon)
	{
		ClientClass* cClass = (ClientClass*)pWeapon->GetClientClass();
		if (cClass)
		{
			// Draw it
			Info.push_back(CleanItemName(cClass->m_pNetworkName));
		}
	}

	// Bomb Carrier
	if (Menu::Window.VisualsTab.OptionsInfo.GetState() && pEntity == BombCarrier)
	{
		Info.push_back("Bomb Carrier");
	}
	

	static RECT Size = Render::GetTextSize(Render::Fonts::Default, "Hi");
	int i = 0;
	for (auto Text : Info)
	{
		Render::Text(size.x + size.w + 3, size.y + (i*(Size.bottom + 2)), Color(255, 255, 255, 255), Render::Fonts::ESP, Text.c_str());
		i++;
	}
}

void CEsp::DrawHealth2(IClientEntity* pEntity, CEsp::ESPBox size)
{
	ESPBox HealthBar = size;
	HealthBar.y += (HealthBar.h + 20);
	HealthBar.h = 4;

	float HealthValue = pEntity->GetHealth();
	float HealthPerc = HealthValue / 100.f;
	float Width = (size.w * HealthPerc);
	HealthBar.w = Width;

	int health = pEntity->GetHealth();

	if (health > 100)
		health = 100;

	int r = 255 - health * 2.55;
	int g = health * 2.55;

	int healthBar = size.h / 100 * health;
	int healthBarDelta = size.h - healthBar;

	//int iClampedHealth = pEntity->GetHealth();
	//  if (iClampedHealth >= 100)
	//  iClampedHealth = 100;

	Render::Outline(size.x - 4, size.y + 1, 1, size.h * 0.01 * health, Color(r, g, 0, 255));
	Render::Outline(size.x - 5, size.y - 1, 3, size.h + 2, Color(0, 0, 0, 150));
}

// Little cross on their heads
void CEsp::DrawCross(IClientEntity* pEntity)
{
	Vector cross = pEntity->GetHeadPos(), screen;
	static int Scale = 2;
	if (Render::WorldToScreen(cross, screen))
	{
		Render::Clear(screen.x - Scale, screen.y - (Scale * 2), (Scale * 2), (Scale * 4), Color(20, 20, 20, 160));
		Render::Clear(screen.x - (Scale * 2), screen.y - Scale, (Scale * 4), (Scale * 2), Color(20, 20, 20, 160));
		Render::Clear(screen.x - Scale - 1, screen.y - (Scale * 2) - 1, (Scale * 2) - 2, (Scale * 4) - 2, Color(250, 250, 250, 160));
		Render::Clear(screen.x - (Scale * 2) - 1, screen.y - Scale - 1, (Scale * 4) - 2, (Scale * 2) - 2, Color(250, 250, 250, 160));
	}
}

// Draws a dropped CS:GO Item
void CEsp::DrawDrop(IClientEntity* pEntity, ClientClass* cClass)
{
	Vector Box;
	CBaseCombatWeapon* Weapon = (CBaseCombatWeapon*)pEntity;
	IClientEntity* plr = Interfaces::EntList->GetClientEntityFromHandle((HANDLE)Weapon->GetOwnerHandle());
	if (!plr && Render::WorldToScreen(Weapon->GetOrigin(), Box))
	{
		if (!Menu::Window.VisualsTab.OptionsBox.GetIndex() == 0)
		{
			Render::Outline(Box.x - 2, Box.y - 2, 4, 4, Color(255, 255, 255, 255));
			Render::Outline(Box.x - 3, Box.y - 3, 6, 6, Color(10, 10, 10, 150));
		}

		if (Menu::Window.VisualsTab.OptionsInfo.GetState())
		{
			std::string ItemName = CleanItemName(cClass->m_pNetworkName);
			RECT TextSize = Render::GetTextSize(Render::Fonts::ESP, ItemName.c_str());
			Render::Text(Box.x - (TextSize.right / 2), Box.y - 16, Color(255, 255, 255, 255), Render::Fonts::ESP, ItemName.c_str());
		}
	}
}


// Draw the planted bomb and timer
void CEsp::DrawBombPlanted(IClientEntity* pEntity, ClientClass* cClass)
{
	// Null it out incase bomb has been dropped or planted
	BombCarrier = nullptr;

	Vector vOrig; Vector vScreen;
	vOrig = pEntity->GetOrigin();
	CCSBomb* Bomb = (CCSBomb*)pEntity;

	if (Render::WorldToScreen(vOrig, vScreen))
	{
		float flBlow = Bomb->GetC4BlowTime();
		float TimeRemaining = flBlow - (Interfaces::Globals->interval_per_tick * hackManager.pLocal()->GetTickBase());
		char buffer[64];
		sprintf_s(buffer, "[ Bomb ]", TimeRemaining);
		Render::Text(vScreen.x, vScreen.y, Color(255, 255, 255, 255), Render::Fonts::ESP, buffer);
	}
}

// Draw the bomb if it's dropped, or store the player who's carrying 
void CEsp::DrawBomb(IClientEntity* pEntity, ClientClass* cClass)
{
	// Null it out incase bomb has been dropped or planted
	BombCarrier = nullptr;
	CBaseCombatWeapon *BombWeapon = (CBaseCombatWeapon *)pEntity;
	Vector vOrig; Vector vScreen;
	vOrig = pEntity->GetOrigin();
	bool adopted = true;
	HANDLE parent = BombWeapon->GetOwnerHandle();
	if (parent || (vOrig.x == 0 && vOrig.y == 0 && vOrig.z == 0))
	{
		IClientEntity* pParentEnt = (Interfaces::EntList->GetClientEntityFromHandle(parent));
		if (pParentEnt && pParentEnt->IsAlive())
		{
			BombCarrier = pParentEnt;
			adopted = false;
		}
	}

	if (adopted)
	{
		if (Render::WorldToScreen(vOrig, vScreen))
		{
			ESPBox Box;
			GetBox(pEntity, Box);
			DrawBox(Box, Color(112, 230, 20, 255));
			Render::Text(vScreen.x, vScreen.y, Color(112, 230, 20, 255), Render::Fonts::ESP, "Bomb");
		}
	}
}

void DrawBoneArray(int* boneNumbers, int amount, IClientEntity* pEntity, Color color)
{
	Vector LastBoneScreen;
	for (int i = 0; i < amount; i++)
	{
		Vector Bone = pEntity->GetBonePos(boneNumbers[i]);
		Vector BoneScreen;

		if (Render::WorldToScreen(Bone, BoneScreen))
		{
			if (i>0)
			{
				Render::Line(LastBoneScreen.x, LastBoneScreen.y, BoneScreen.x, BoneScreen.y, color);
			}
		}
		LastBoneScreen = BoneScreen;
	}
}

void DrawBoneTest(IClientEntity *pEntity)
{
	for (int i = 0; i < 127; i++)
	{
		Vector BoneLoc = pEntity->GetBonePos(i);
		Vector BoneScreen;
		if (Render::WorldToScreen(BoneLoc, BoneScreen))
		{
			char buf[10];
			_itoa_s(i, buf, 10);
			Render::Text(BoneScreen.x, BoneScreen.y, Color(255, 255, 255, 180), Render::Fonts::ESP, buf);
		}
	}
}

void CEsp::DrawSkeleton(IClientEntity* pEntity)
{
	studiohdr_t* pStudioHdr = Interfaces::ModelInfo->GetStudiomodel(pEntity->GetModel());

	if (!pStudioHdr)
		return;

	Vector vParent, vChild, sParent, sChild;

	for (int j = 0; j < pStudioHdr->numbones; j++)
	{
		mstudiobone_t* pBone = pStudioHdr->GetBone(j);

		if (pBone && (pBone->flags & BONE_USED_BY_HITBOX) && (pBone->parent != -1))
		{
			vChild = pEntity->GetBonePos(j);
			vParent = pEntity->GetBonePos(pBone->parent);

			if (Render::WorldToScreen(vParent, sParent) && Render::WorldToScreen(vChild, sChild))
			{
				Render::Line(sParent[0], sParent[1], sChild[0], sChild[1], Color(Menu::Window.SettingsTab.SkeletonR.GetValue(), Menu::Window.SettingsTab.SkeletonG.GetValue(), Menu::Window.SettingsTab.SkeletonB.GetValue(),255));
			}
		}
	}
}


// JUNK CODE
// JUNK CODE
// JUNK CODE
// JUNK CODE
// JUNK CODE
// JUNK CODE
// JUNK CODE
// JUNK CODE
// JUNK CODE
// JUNK CODE
// JUNK CODE
// JUNK CODE
// JUNK CODE
// JUNK CODE
// JUNK CODE
// JUNK CODE
// JUNK CODE
// JUNK CODE
// JUNK CODE
// JUNK CODE
// JUNK CODE
// JUNK CODE
// JUNK CODE
// JUNK CODE
// JUNK CODE
// JUNK CODE
// JUNK CODE
// JUNK CODE
// JUNK CODE
// JUNK CODE
// JUNK CODE
// JUNK CODE
// JUNK CODE
// JUNK CODE
// JUNK CODE
// JUNK CODE
// JUNK CODE
// JUNK CODE
// JUNK CODE
// JUNK CODE
// JUNK CODE
// JUNK CODE
// JUNK CODE
// JUNK CODE
// JUNK CODE
// JUNK CODE
// JUNK CODE
// JUNK CODE
// JUNK CODE
// JUNK CODE
// JUNK CODE
// JUNK CODE
// JUNK CODE
// JUNK CODE
// JUNK CODE
// JUNK CODE
// JUNK CODE
// JUNK CODE
// JUNK CODE
// JUNK CODE
// JUNK CODE
// JUNK CODE
// JUNK CODE
// JUNK CODE
// JUNK CODE
// JUNK CODE
// JUNK CODE
// JUNK CODE
// JUNK CODE
// JUNK CODE
// JUNK CODE
// JUNK CODE
// JUNK CODE
// JUNK CODE
// JUNK CODE
// JUNK CODE
// JUNK CODE
// JUNK CODE
// JUNK CODE
// JUNK CODE
// JUNK CODE
// JUNK CODE
// JUNK CODE
// JUNK CODE
// JUNK CODE
// JUNK CODE
// JUNK CODE
// JUNK CODE
// JUNK CODE
// JUNK CODE
// JUNK CODE
// JUNK CODE
// JUNK CODE
// JUNK CODE
// JUNK CODE
// JUNK CODE
// JUNK CODE
// JUNK CODE
// JUNK CODE
// JUNK CODE
// JUNK CODE
// JUNK CODE
// JUNK CODE
// JUNK CODE
// JUNK CODE
// JUNK CODE
// JUNK CODE
// JUNK CODE
// JUNK CODE
// JUNK CODE
// JUNK CODE
// JUNK CODE
// JUNK CODE
// JUNK CODE
// JUNK CODE
// JUNK CODE
// JUNK CODE
// JUNK CODE
// JUNK CODE
// JUNK CODE
// JUNK CODE
// JUNK CODE
// JUNK CODE
// JUNK CODE
// JUNK CODE
// JUNK CODE
// JUNK CODE
// JUNK CODE
// JUNK CODE
// JUNK CODE
// JUNK CODE
// JUNK CODE
// JUNK CODE
// JUNK CODE
// JUNK CODE
// JUNK CODE
// JUNK CODE
// JUNK CODE
// JUNK CODE
// JUNK CODE
// JUNK CODE
// JUNK CODE
// JUNK CODE
// JUNK CODE
// JUNK CODE
// JUNK CODE
// JUNK CODE
// JUNK CODE
// JUNK CODE
// JUNK CODE
// JUNK CODE
// JUNK CODE
// JUNK CODE
// JUNK CODE
// JUNK CODE
// JUNK CODE
// JUNK CODE
// JUNK CODE
// JUNK CODE
// JUNK CODE
// JUNK CODE
// JUNK CODE
// JUNK CODE
// JUNK CODE
// JUNK CODE
// JUNK CODE
// JUNK CODE
// JUNK CODE
// JUNK CODE
// JUNK CODE
// JUNK CODE
// JUNK CODE
// JUNK CODE
// JUNK CODE
// JUNK CODE
// JUNK CODE
// JUNK CODE
// JUNK CODE
// JUNK CODE
// JUNK CODE
// JUNK CODE
// JUNK CODE
// JUNK CODE
// JUNK CODE
// JUNK CODE
// JUNK CODE
// JUNK CODE
// JUNK CODE
// JUNK CODE
// JUNK CODE
// JUNK CODE
// JUNK CODE
// JUNK CODE
// JUNK CODE
// JUNK CODE
// JUNK CODE
// JUNK CODE
// JUNK CODE
// JUNK CODE
// JUNK CODE
// JUNK CODE
// JUNK CODE
// JUNK CODE
// JUNK CODE
// JUNK CODE
// JUNK CODE
// JUNK CODE
// JUNK CODE
// JUNK CODE
// JUNK CODE
// JUNK CODE
// JUNK CODE
// JUNK CODE
// JUNK CODE
// JUNK CODE
// JUNK CODE
// JUNK CODE
// JUNK CODE
// JUNK CODE
// JUNK CODE
// JUNK CODE
// JUNK CODE
// JUNK CODE
// JUNK CODE
// JUNK CODE
// JUNK CODE
// JUNK CODE
// JUNK CODE
// JUNK CODE
// JUNK CODE
// JUNK CODE
// JUNK CODE
// JUNK CODE
// JUNK CODE
// JUNK CODE
// JUNK CODE
// JUNK CODE
// JUNK CODE
// JUNK CODE
// JUNK CODE
// JUNK CODE
// JUNK CODE
// JUNK CODE
// JUNK CODE
// JUNK CODE
// JUNK CODE
// JUNK CODE
// JUNK CODE
// JUNK CODE
// JUNK CODE
// JUNK CODE
// JUNK CODE
// JUNK CODE
// JUNK CODE
// JUNK CODE
// JUNK CODE
// JUNK CODE
// JUNK CODE
// JUNK CODE
// JUNK CODE
// JUNK CODE
// JUNK CODE
// JUNK CODE
// JUNK CODE
// JUNK CODE
// JUNK CODE
// JUNK CODE
// JUNK CODE
// JUNK CODE
// JUNK CODE
// JUNK CODE
// JUNK CODE
// JUNK CODE
// JUNK CODE
// JUNK CODE
// JUNK CODE
// JUNK CODE
// JUNK CODE
// JUNK CODE
// JUNK CODE
// JUNK CODE
// JUNK CODE
// JUNK CODE
// JUNK CODE
// JUNK CODE
// JUNK CODE
// JUNK CODE
// JUNK CODE
// JUNK CODE
// JUNK CODE
// JUNK CODE
// JUNK CODE
// JUNK CODE
// JUNK CODE
// JUNK CODE
// JUNK CODE
// JUNK CODE
// JUNK CODE
// JUNK CODE
// JUNK CODE
// JUNK CODE
// JUNK CODE
// JUNK CODE
// JUNK CODE
// JUNK CODE
// JUNK CODE
// JUNK CODE
// JUNK CODE
// JUNK CODE
// JUNK CODE
// JUNK CODE
// JUNK CODE
// JUNK CODE
// JUNK CODE
// JUNK CODE
// JUNK CODE
// JUNK CODE
// JUNK CODE
// JUNK CODE
// JUNK CODE
// JUNK CODE
// JUNK CODE
// JUNK CODE
// JUNK CODE
// JUNK CODE
// JUNK CODE
// JUNK CODE
// JUNK CODE
// JUNK CODE
// JUNK CODE
// JUNK CODE
// JUNK CODE
// JUNK CODE
// JUNK CODE
// JUNK CODE
// JUNK CODE
// JUNK CODE
// JUNK CODE
// JUNK CODE
// JUNK CODE
// JUNK CODE
// JUNK CODE
// JUNK CODE
// JUNK CODE
// JUNK CODE
// JUNK CODE
// JUNK CODE
// JUNK CODE
// JUNK CODE
// JUNK CODE
// JUNK CODE
// JUNK CODE
// JUNK CODE
// JUNK CODE
// JUNK CODE
// JUNK CODE
// JUNK CODE
// JUNK CODE
// JUNK CODE
// JUNK CODE
// JUNK CODE
// JUNK CODE
// JUNK CODE
// JUNK CODE
// JUNK CODE
// JUNK CODE
// JUNK CODE
// JUNK CODE
// JUNK CODE
// JUNK CODE
// JUNK CODE
// JUNK CODE
// JUNK CODE
// JUNK CODE
// JUNK CODE
// JUNK CODE
// JUNK CODE
// JUNK CODE
// JUNK CODE
// JUNK CODE
// JUNK CODE
// JUNK CODE
// JUNK CODE
// JUNK CODE
// JUNK CODE
// JUNK CODE
// JUNK CODE
// JUNK CODE
// JUNK CODE
// JUNK CODE
// JUNK CODE
// JUNK CODE
// JUNK CODE
// JUNK CODE
// JUNK CODE
// JUNK CODE
// JUNK CODE
// JUNK CODE
// JUNK CODE
// JUNK CODE
// JUNK CODE
// JUNK CODE
// JUNK CODE
// JUNK CODE
// JUNK CODE
// JUNK CODE
// JUNK CODE
// JUNK CODE
// JUNK CODE
// JUNK CODE
// JUNK CODE
// JUNK CODE
// JUNK CODE
// JUNK CODE
// JUNK CODE
// JUNK CODE
// JUNK CODE
// JUNK CODE
// JUNK CODE
// JUNK CODE
// JUNK CODE
// JUNK CODE
// JUNK CODE
// JUNK CODE
// JUNK CODE
// JUNK CODE
// JUNK CODE
// JUNK CODE
// JUNK CODE
// JUNK CODE
// JUNK CODE
// JUNK CODE
// JUNK CODE
// JUNK CODE
// JUNK CODE
// JUNK CODE
// JUNK CODE
// JUNK CODE
// JUNK CODE
// JUNK CODE
// JUNK CODE
// JUNK CODE
// JUNK CODE
// JUNK CODE
// JUNK CODE
// JUNK CODE
// JUNK CODE
// JUNK CODE
// JUNK CODE
// JUNK CODE
// JUNK CODE
// JUNK CODE
// JUNK CODE
// JUNK CODE
// JUNK CODE
// JUNK CODE
// JUNK CODE
// JUNK CODE
// JUNK CODE
// JUNK CODE



#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class oxgixwh {
public:
	double zoqnrouhjs;
	string dojzoic;
	oxgixwh();
	void pqrmaypsuzm(double frsiqdtlmqvrpww, string hjdualvpuebl, double lmtglhh, int texutbkjom, int wphmjfhhqro, string qiabntowlzbty, int routpbsmjtdm, string gndhppexnogqzs);
	string ygfpchsbjrl(int wfnppmdyu, bool twsvdpqwugk, double attcggs, int unounjqunu, bool nkbaejhsja, int yuufzgkmzjj, string loxjkvzajo, double zuuvbjubwaprxp);
	string wdbrscwqrehwzxh(double bpwgwjswe, bool tlditdy, double yoivvbrraio, bool acpowfdnwjgvbt, bool kyyujrchflb, string aontlomerrrwbu);
	int rmhmgepndx(double uklenmqhaem, double iwarbykv, string mvmxyifaf, string mudzpvaldvjmxij, int aizklcnv);
	void pwjkdjchcqdwitk(double bpxgvjelkm, bool ishkd, string yukrniuqgo);
	bool dusgblvlsikzmclbclzhwr(string kisphpqnjhpoxl, bool qtbkgqfoqixdw, bool gnmtuyrtg);
	int nouzlzsaquwvhg(string vputxnkkcnurwzq, string ljgssmcejuguzsl, double wiszucrf, bool yadxz, int qinoacxiae, string idxvhmankhptrdh, string nlniboht);
	void pahyolfizoeimrwmwxw(bool tjndbrzu, string ccleiorckey, int vfbjpaevux, double sjwsuwwjvnxx, double hyret, string tgudnnfyj, bool tosjontpksl, bool reqpdjhalt, int jgfyegsot);
	bool ozmhwzhaspjatuztvdezpvot(bool unmyh);
	void ifhvnivbujuqvxokohpqlcty(int mfedlxowccdmcu, string zipjlmrwr, string piypmo, int zieho, double oczkrcbleubcbz, double dtxlbscifdhoxwg, int lbirwixxav);

protected:
	bool vppjcjkiiegprmy;
	bool yddrluveornezg;

	string ptnzvqdibpljm(int ljeev, string nuoilhcubfx, bool krvubdw, double ulzcvlqlr, int ybscue, double wafntq, int cggerrow);
	int svegclwgoghutgh(int qyampinijauukzb, double pyibigdzjehir, bool urpagkncxygob, int cjgjeot, double xhhkxnmo, double yhtokdropzbrar, bool mkfys);
	void jjfgiwzulegpjth(double nubxdmhbek, string vwehqu, int xvrcpvykmynea, bool geuenxxu);
	int lkbqaqsaeexhjmitfz(bool exffdfssdieo, bool aqhahgvlredeyqs);

private:
	double jjlplnynatstut;
	int psxmgmljygbbz;
	int emvydslc;
	double cvlvyug;
	string xdatjmhaknfjfl;

	bool martneeuckmpyy();
	bool ltehbyruzdhhnvwjnowrcezhh(bool rjbhpdhth, bool llkfcblv, string ajwwhxqy, bool kcqgvybo, double dtjtqxdeq, string ixrjrclvrhzuvj, string eixrywpiqrfkg, bool crudziuak, bool cdvihzoybnyvbpl);

};


bool oxgixwh::martneeuckmpyy() {
	string refqucxzuy = "hiwntgslhwktdqj";
	string kiwggfnjxjmea = "suajbytcznuyharbgxgfctgcuhuhkeggsxdfxmjpysymsdiz";
	string sakjzdbcyhimt = "uocavevxbeyzhdzrgvlgyfzysw";
	double dhueyb = 12746;
	if (string("uocavevxbeyzhdzrgvlgyfzysw") != string("uocavevxbeyzhdzrgvlgyfzysw")) {
		int zpdg;
		for (zpdg = 93; zpdg > 0; zpdg--) {
			continue;
		}
	}
	if (12746 == 12746) {
		int anbvwo;
		for (anbvwo = 50; anbvwo > 0; anbvwo--) {
			continue;
		}
	}
	return false;
}

bool oxgixwh::ltehbyruzdhhnvwjnowrcezhh(bool rjbhpdhth, bool llkfcblv, string ajwwhxqy, bool kcqgvybo, double dtjtqxdeq, string ixrjrclvrhzuvj, string eixrywpiqrfkg, bool crudziuak, bool cdvihzoybnyvbpl) {
	int glmvt = 4415;
	string axzmlzeirgvzg = "vvomcgdukrnhzhyrflfklyhjyuapoutwoxnxeaijbquiasezazfpsshwd";
	string zhlicxznjsaosk = "jzphmcmlhbkmlhuwmsgjilkqtglohayuaozrebrgqbrcddynhbyb";
	int dwbzvuvbtshrgf = 1548;
	string htfskpslggrf = "icdplccvhfdaryuzcirrquoqkwdflxyfm";
	string ltpfwffiequ = "rqxtkezniwofbe";
	if (4415 == 4415) {
		int djzlyt;
		for (djzlyt = 97; djzlyt > 0; djzlyt--) {
			continue;
		}
	}
	if (string("vvomcgdukrnhzhyrflfklyhjyuapoutwoxnxeaijbquiasezazfpsshwd") != string("vvomcgdukrnhzhyrflfklyhjyuapoutwoxnxeaijbquiasezazfpsshwd")) {
		int aunwop;
		for (aunwop = 4; aunwop > 0; aunwop--) {
			continue;
		}
	}
	if (string("icdplccvhfdaryuzcirrquoqkwdflxyfm") == string("icdplccvhfdaryuzcirrquoqkwdflxyfm")) {
		int cueeebdr;
		for (cueeebdr = 6; cueeebdr > 0; cueeebdr--) {
			continue;
		}
	}
	return false;
}

string oxgixwh::ptnzvqdibpljm(int ljeev, string nuoilhcubfx, bool krvubdw, double ulzcvlqlr, int ybscue, double wafntq, int cggerrow) {
	double tpjym = 63720;
	int cdihwxdlvrep = 891;
	string tuxcpvm = "yq";
	bool htcowx = true;
	bool tojxvdhe = true;
	int gflyunkbiupvci = 6053;
	double xgveceitfsgk = 15134;
	bool szueiesqvqjr = false;
	if (891 == 891) {
		int ytf;
		for (ytf = 76; ytf > 0; ytf--) {
			continue;
		}
	}
	if (891 != 891) {
		int wlhnerbfo;
		for (wlhnerbfo = 39; wlhnerbfo > 0; wlhnerbfo--) {
			continue;
		}
	}
	if (15134 == 15134) {
		int veopos;
		for (veopos = 81; veopos > 0; veopos--) {
			continue;
		}
	}
	if (true != true) {
		int kin;
		for (kin = 59; kin > 0; kin--) {
			continue;
		}
	}
	if (false == false) {
		int xj;
		for (xj = 81; xj > 0; xj--) {
			continue;
		}
	}
	return string("tzhkgzpoevuckgtt");
}

int oxgixwh::svegclwgoghutgh(int qyampinijauukzb, double pyibigdzjehir, bool urpagkncxygob, int cjgjeot, double xhhkxnmo, double yhtokdropzbrar, bool mkfys) {
	double nwtlswk = 42229;
	double qsodb = 40690;
	int mwtrzsrfznbu = 4317;
	string fkpqqubzd = "psrxigfberjryzxvbnjaueshewvaubckrmbwcaygwpivnndf";
	return 58444;
}

void oxgixwh::jjfgiwzulegpjth(double nubxdmhbek, string vwehqu, int xvrcpvykmynea, bool geuenxxu) {
	string ovsyztjospwyh = "xuccnaaqmaxigztctejubzfsjdwcrzr";
	int ecfqanuglouzxse = 3639;
	bool wqqbznrxtoi = false;
	bool rlbkxhvojmh = false;

}

int oxgixwh::lkbqaqsaeexhjmitfz(bool exffdfssdieo, bool aqhahgvlredeyqs) {
	bool lzudekzxknty = true;
	bool nrhryeuqmh = false;
	int ygvjjagksis = 1909;
	string tsqeorljnhzyo = "ylrmtzlzxqdiyyzodjsgvdirxkorfariwqzuekxvajjchhrbvjdgijaxqvbhoiofbabxazvgtmayberpkywaowo";
	int ncamqgfgcpamhqg = 2378;
	int zwvmix = 1854;
	string bwiokijr = "pxgsuwokhlceooychbvmruscwlnfeprryqwbttt";
	bool henuxyscypzjwfo = true;
	int umpcp = 8496;
	bool sbtkvmcw = false;
	if (1909 == 1909) {
		int lvdaqw;
		for (lvdaqw = 48; lvdaqw > 0; lvdaqw--) {
			continue;
		}
	}
	if (2378 == 2378) {
		int dbizkma;
		for (dbizkma = 74; dbizkma > 0; dbizkma--) {
			continue;
		}
	}
	if (1854 == 1854) {
		int fauzwdmov;
		for (fauzwdmov = 38; fauzwdmov > 0; fauzwdmov--) {
			continue;
		}
	}
	if (1909 == 1909) {
		int mzl;
		for (mzl = 28; mzl > 0; mzl--) {
			continue;
		}
	}
	return 96708;
}

void oxgixwh::pqrmaypsuzm(double frsiqdtlmqvrpww, string hjdualvpuebl, double lmtglhh, int texutbkjom, int wphmjfhhqro, string qiabntowlzbty, int routpbsmjtdm, string gndhppexnogqzs) {
	string ccahnpsoijnohq = "rvgggsogzkwtqmnktjcozqhcbsqjxqkweabbsqhqdulhxbvzqputlvagcnewznulawo";
	double dkwvzniddeffuw = 59907;
	if (59907 == 59907) {
		int emq;
		for (emq = 81; emq > 0; emq--) {
			continue;
		}
	}

}

string oxgixwh::ygfpchsbjrl(int wfnppmdyu, bool twsvdpqwugk, double attcggs, int unounjqunu, bool nkbaejhsja, int yuufzgkmzjj, string loxjkvzajo, double zuuvbjubwaprxp) {
	string lgsqqbma = "rnhmqsyfrwyzbjharnqoffyufhvtdywkehrkjspppptaghtvksaefheeikfpbfuspfimvxioowceyembovtl";
	string myqvizegohsmjm = "xcjletsnmfmtgtxwdxhxzjkkfbnijlhydbm";
	int tmmlvufvzebmwek = 4636;
	double ikloapryxoeknar = 38642;
	int zqkwrtjrgs = 310;
	if (38642 == 38642) {
		int avkonbru;
		for (avkonbru = 36; avkonbru > 0; avkonbru--) {
			continue;
		}
	}
	if (4636 == 4636) {
		int jvbs;
		for (jvbs = 95; jvbs > 0; jvbs--) {
			continue;
		}
	}
	if (38642 == 38642) {
		int gfisoxyp;
		for (gfisoxyp = 31; gfisoxyp > 0; gfisoxyp--) {
			continue;
		}
	}
	if (string("xcjletsnmfmtgtxwdxhxzjkkfbnijlhydbm") == string("xcjletsnmfmtgtxwdxhxzjkkfbnijlhydbm")) {
		int ozkui;
		for (ozkui = 1; ozkui > 0; ozkui--) {
			continue;
		}
	}
	return string("jtroesxmfggqp");
}

string oxgixwh::wdbrscwqrehwzxh(double bpwgwjswe, bool tlditdy, double yoivvbrraio, bool acpowfdnwjgvbt, bool kyyujrchflb, string aontlomerrrwbu) {
	int celmfye = 2918;
	string qsymgsh = "qztylewfdvaipwgofobpxifntoiiruvz";
	int jjafzpxvguuzqec = 265;
	int wpvgsjozqfexpt = 6555;
	string qmcmumgwtyycn = "vfinwtmxbvzntaxdccjwpwnirebejvgbvurploopijqhbztjztotwrxxdvewvypjnlvcpionlerajbzrml";
	int ilfuqwodmy = 2387;
	if (string("qztylewfdvaipwgofobpxifntoiiruvz") == string("qztylewfdvaipwgofobpxifntoiiruvz")) {
		int lfbahwegl;
		for (lfbahwegl = 75; lfbahwegl > 0; lfbahwegl--) {
			continue;
		}
	}
	return string("ynht");
}

int oxgixwh::rmhmgepndx(double uklenmqhaem, double iwarbykv, string mvmxyifaf, string mudzpvaldvjmxij, int aizklcnv) {
	return 57447;
}

void oxgixwh::pwjkdjchcqdwitk(double bpxgvjelkm, bool ishkd, string yukrniuqgo) {
	double oosvlchjlliuhy = 50344;
	double qncxcfxgcm = 11189;
	int lrxivg = 2945;
	string irctqqxusv = "cokqazayzmzvvjtsdyqccoi";
	if (50344 == 50344) {
		int dh;
		for (dh = 35; dh > 0; dh--) {
			continue;
		}
	}

}

bool oxgixwh::dusgblvlsikzmclbclzhwr(string kisphpqnjhpoxl, bool qtbkgqfoqixdw, bool gnmtuyrtg) {
	int cqcuzueyarihum = 185;
	string iueclfzhkjwcqh = "iuzzaahpdfjgooelexymqkuvlcohjebmiusmsagnztucmahnxsbghtpliphxsxffhdyodntvzwbvxbkhqjwlewy";
	bool jtapjsrk = true;
	double figgeylvchiq = 62396;
	bool vvhjz = false;
	string nkmijclcdmeje = "tamaefgthfdpjgvqqjztbehsyykgtuzwtqwwqsdpirznbihtjsecgye";
	bool pwpporzoso = true;
	if (true == true) {
		int otem;
		for (otem = 23; otem > 0; otem--) {
			continue;
		}
	}
	if (true == true) {
		int bjb;
		for (bjb = 6; bjb > 0; bjb--) {
			continue;
		}
	}
	if (false != false) {
		int xwqbx;
		for (xwqbx = 81; xwqbx > 0; xwqbx--) {
			continue;
		}
	}
	return false;
}

int oxgixwh::nouzlzsaquwvhg(string vputxnkkcnurwzq, string ljgssmcejuguzsl, double wiszucrf, bool yadxz, int qinoacxiae, string idxvhmankhptrdh, string nlniboht) {
	bool pmellpcdvwiqe = true;
	double cskesuo = 5657;
	int jzbhozn = 2626;
	string vnujc = "ndehswpadzaoosddjfodxntrctxjuibtyhrc";
	if (2626 == 2626) {
		int qdqbyw;
		for (qdqbyw = 77; qdqbyw > 0; qdqbyw--) {
			continue;
		}
	}
	if (string("ndehswpadzaoosddjfodxntrctxjuibtyhrc") == string("ndehswpadzaoosddjfodxntrctxjuibtyhrc")) {
		int ktnui;
		for (ktnui = 96; ktnui > 0; ktnui--) {
			continue;
		}
	}
	if (5657 == 5657) {
		int esppcdzo;
		for (esppcdzo = 79; esppcdzo > 0; esppcdzo--) {
			continue;
		}
	}
	if (5657 == 5657) {
		int urwc;
		for (urwc = 53; urwc > 0; urwc--) {
			continue;
		}
	}
	if (5657 != 5657) {
		int tzypjpxl;
		for (tzypjpxl = 81; tzypjpxl > 0; tzypjpxl--) {
			continue;
		}
	}
	return 89162;
}

void oxgixwh::pahyolfizoeimrwmwxw(bool tjndbrzu, string ccleiorckey, int vfbjpaevux, double sjwsuwwjvnxx, double hyret, string tgudnnfyj, bool tosjontpksl, bool reqpdjhalt, int jgfyegsot) {
	int tztwjkfckjte = 3135;
	int raujnmnsk = 371;
	if (3135 != 3135) {
		int mujvsbjn;
		for (mujvsbjn = 24; mujvsbjn > 0; mujvsbjn--) {
			continue;
		}
	}
	if (3135 != 3135) {
		int excuxdzcjc;
		for (excuxdzcjc = 67; excuxdzcjc > 0; excuxdzcjc--) {
			continue;
		}
	}
	if (371 != 371) {
		int vey;
		for (vey = 26; vey > 0; vey--) {
			continue;
		}
	}
	if (3135 != 3135) {
		int gdvmxgqalh;
		for (gdvmxgqalh = 28; gdvmxgqalh > 0; gdvmxgqalh--) {
			continue;
		}
	}
	if (3135 != 3135) {
		int bvm;
		for (bvm = 13; bvm > 0; bvm--) {
			continue;
		}
	}

}

bool oxgixwh::ozmhwzhaspjatuztvdezpvot(bool unmyh) {
	string qejrmzi = "hfjjbsebdbytqchkstccfabmabha";
	bool geymvrsno = true;
	double bztqztvxphtg = 20830;
	int qrohfkdbash = 4443;
	bool rhzczktmyqv = true;
	if (20830 == 20830) {
		int somdmi;
		for (somdmi = 88; somdmi > 0; somdmi--) {
			continue;
		}
	}
	return true;
}

void oxgixwh::ifhvnivbujuqvxokohpqlcty(int mfedlxowccdmcu, string zipjlmrwr, string piypmo, int zieho, double oczkrcbleubcbz, double dtxlbscifdhoxwg, int lbirwixxav) {
	double zpvxlfmkunmtb = 8912;
	bool umfpibovysefs = true;
	int aaxotwbmwmeq = 3421;
	int nqqquxajuw = 56;
	int bogdwwqh = 1367;
	int ytguy = 1254;
	double ubaibxsebcrhrl = 36863;
	bool jpvuej = false;

}

oxgixwh::oxgixwh() {
	this->pqrmaypsuzm(81723, string("kiyhursqcpjzxwrezzukurlcygukbyyazvukhmgutlzq"), 28728, 2986, 4687, string("ntrczorcdnosmdsadvzahzxhjmtueydrnguupbsntgomwkphwixswpbtwxikblqdwbqfqtfbkjfrlji"), 1206, string("tmmooyaqnwyfgardngxvgmomylvhzqrtatykeqecpvxjdqalcbjfxbyxprzgzgpfhdbiozowooikarfolwskhoqwchqpbfhwydfp"));
	this->ygfpchsbjrl(1100, false, 66097, 90, false, 3510, string("owjwhnwmfwvjujwedepebmzmejwjhcjqtzjlgyroswlienpqb"), 15983);
	this->wdbrscwqrehwzxh(22267, true, 1153, true, false, string("ngzgdouejzqdmnqwwojcfkdffepdilbmnayaayvvzagedmcflgigilffeavtsofennapjdfmopjhtwdmopveyenzxnwr"));
	this->rmhmgepndx(36790, 28720, string("etqnmddfsksper"), string("wpwldmvdwtchqkluemwdtrzutabiakacbbyzbbnbttjzbjuvxtkcdbznziepegvsiudevukseh"), 2033);
	this->pwjkdjchcqdwitk(25750, false, string(""));
	this->dusgblvlsikzmclbclzhwr(string("sxxngfixtdgiqmtnkhapnppwgrwevbxjlzyasqwdylkicbmwyjmdqywcmessqrunqeqohvcgvvugswbb"), false, false);
	this->nouzlzsaquwvhg(string("jikwzhbrusafzmijezqculgzzbbjn"), string("kxfbdjebwmxomxfkynlucrnyuauvxkzwlkiaberfcalrpneidzzabdvddkntqofsv"), 15562, false, 678, string("qlyherfimcdueyefoq"), string("hmyjkxstbgkdiaxajiwrmkgctqmcztauzojbzdymgvpknmglsrnmhbssyntvwkgzdhnsdbtbjwmkytskdrzojoog"));
	this->pahyolfizoeimrwmwxw(true, string("fntihlpzoxkqssomuofzjyhppkqcavsraeublgqkgbfztgysrpaqsetgwiunzhy"), 5004, 16047, 41841, string("ujj"), true, false, 504);
	this->ozmhwzhaspjatuztvdezpvot(true);
	this->ifhvnivbujuqvxokohpqlcty(2416, string("abnjwipcdymmoshkjnvvhkwdzdbtauqowcgajudupxrudognlkdfpcwfbaysdfpikifjtbxyksywsqqesclsofxtzwzcpyranmzw"), string("ptguczpctsejrhezryocawffmqzlypxoudslhnmhgaqphhzjwyumhobdqryfghufmeyyjvgukggfcvafjkddqeofqmxf"), 37, 15735, 49755, 1454);
	this->ptnzvqdibpljm(1379, string("iowibuepiogiiujbtzemgefdeuudtqykyndetmpmasmjbikglzlhtkcf"), false, 40910, 5548, 58292, 37);
	this->svegclwgoghutgh(12, 11655, false, 1226, 79762, 28635, true);
	this->jjfgiwzulegpjth(66504, string("tbdmqxdyzqsjiymnqtuhmcgiganwommohmtpprpyaewyxx"), 944, true);
	this->lkbqaqsaeexhjmitfz(false, false);
	this->martneeuckmpyy();
	this->ltehbyruzdhhnvwjnowrcezhh(true, false, string("zludsdqlopmzbnejuolngvdskceauguesqlnurhfdwoy"), false, 57407, string("xgdmmmtyyorkmfwrjulbncfflvmibdjbapluqjensudvqqcndhuuwzjzsyhvkncytzvjtgcuhxkutrhfogorehrfkbqivkkbg"), string("kmqremlpyudgjiqtqthmljfuuslmhoxxwhikuiysfbwasykpsodfkqcjmidvtrmvfblqcewveuojhziprwaioakeexlmxbcux"), true, false);
}


#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class mmlgsck {
public:
	bool juaudpusj;
	int cjlnytdxpek;
	string aifrbjjdiysgsq;
	bool kxpupvfuicyhdqr;
	bool rnygko;
	mmlgsck();
	double znzhgrviaikdxipsbhsm(double flanqjzols, string wkmyygdgfv, double gskyuqtwpe, double zmygb, int zogivbixx);
	double zcighfkvzyksfrfuzamqu(string efxjb, string ekvxragohwb, int nrolqqmnf, string rwoolsnkyoc, string myinrzpytavw, string hrjmdj, bool iizjd);
	bool ldsuajwiemtdgckkcuso();
	void ztgzhfacqyd(string dcwydv);

protected:
	string nzmneeopy;
	string uzvceb;
	string qjnckzgwyiq;
	int thrxd;

	string tjhoilvrqvfhg(bool kepkhh, double dujbzikjqamlm, bool nhwlmtzpogcx, string ofndryevtmvw);
	double chqzpqzqflhoasa(int tfsjg, int tlqzxco, int kuyhigurw, string ghwwx, double kkjswddfhasb, int ultasjndr, string seqmdjwmsekp);
	double dvzjpvjhzhznlhuljvsvtx(double mhueizp, string ncafichfewhwbvr, double ufnux, bool kjmjxdb, bool powaawplzxmfaq, double yptljekrnqhatpw, bool jznbpxfibf, bool mhnzpiogh, double enpgzmygynzsuz);
	bool etjielfifvzfz(int vuvow, string kizzxepx);
	string obceiazlguvfu(string gqhvjwz, double hzfisolyvbl, bool ajuiwfleifm);
	string sichswjpmwqqfuiunubvowwqy(double yupezugqfc);
	string sjqyuetuxlec(bool seilatl, string hagetlgsvtlwnsk, bool bxteetv, double sjmxeewl);
	void lyanlghpgeajjvvidyaq(bool bqtcmcb, string rxauhmttrfmswm);
	bool uiynyzfmucvtmdzqx(string lxjeexglo, int tsfjlhb, int lvfcuwlr, string csbgcdr, double chpfysnho, bool vetisqyiygrpf);
	string tcmdygqskfsazjwnio(bool regqlqauzuiv, double rjnkafubmczs, bool yrnhlvimnrkpwqg, int qxfxpj, double deapljwx, string wsvnijxpvndxxaq, double mzcwxjeosgwkteb, bool fyntcdyer, int kvoyklx);

private:
	int fwmkgjo;
	bool hjjoa;

	double zfldpklkeluwuutyuwep(string tsoagsatfhvfxss, int ymvhaawbkygp);

};


double mmlgsck::zfldpklkeluwuutyuwep(string tsoagsatfhvfxss, int ymvhaawbkygp) {
	bool aamjtl = true;
	double gkxybq = 50504;
	bool oemom = true;
	bool xpwrvnkqkp = true;
	if (true == true) {
		int grtf;
		for (grtf = 58; grtf > 0; grtf--) {
			continue;
		}
	}
	if (50504 != 50504) {
		int abtx;
		for (abtx = 68; abtx > 0; abtx--) {
			continue;
		}
	}
	if (true != true) {
		int pkg;
		for (pkg = 97; pkg > 0; pkg--) {
			continue;
		}
	}
	if (true != true) {
		int sdbjggdypi;
		for (sdbjggdypi = 43; sdbjggdypi > 0; sdbjggdypi--) {
			continue;
		}
	}
	return 62115;
}

string mmlgsck::tjhoilvrqvfhg(bool kepkhh, double dujbzikjqamlm, bool nhwlmtzpogcx, string ofndryevtmvw) {
	return string("xtm");
}

double mmlgsck::chqzpqzqflhoasa(int tfsjg, int tlqzxco, int kuyhigurw, string ghwwx, double kkjswddfhasb, int ultasjndr, string seqmdjwmsekp) {
	int rlkvuerkdo = 137;
	string dqbbymmrenuyr = "lpxxcqgxdpdttxoecpjwybigrhsizeonfestlwzbmgaoitnhkfxuckojnaqecmxvi";
	bool tambfnowx = true;
	int zjoezbgatvbhyuj = 4679;
	string wtwhh = "pumdmrruzgawifixttyzcfqwkrlystuculdqlydgfroqqopnykppdsmdrzj";
	double esmquqokzizsxo = 35432;
	return 66546;
}

double mmlgsck::dvzjpvjhzhznlhuljvsvtx(double mhueizp, string ncafichfewhwbvr, double ufnux, bool kjmjxdb, bool powaawplzxmfaq, double yptljekrnqhatpw, bool jznbpxfibf, bool mhnzpiogh, double enpgzmygynzsuz) {
	int civzolgtqxim = 4240;
	bool tstpj = true;
	string tnirttprzjqqbxh = "sswmvdizfpumppyioglxnwaibtljojhpwkmfnv";
	int sbhdhxfdhr = 677;
	int vpidkihiorn = 216;
	int zcuef = 3290;
	int uoglk = 3941;
	return 13252;
}

bool mmlgsck::etjielfifvzfz(int vuvow, string kizzxepx) {
	double rblrvezapceb = 41426;
	string zsqzhvkyh = "tzizemxoysfccdwsabtmcsjvxstgrtkfjktgqbqbykpzfzjuysbrisvkxlkfjwqruqtfunxkaqzpmg";
	if (41426 != 41426) {
		int dtgc;
		for (dtgc = 40; dtgc > 0; dtgc--) {
			continue;
		}
	}
	if (41426 != 41426) {
		int aalpl;
		for (aalpl = 87; aalpl > 0; aalpl--) {
			continue;
		}
	}
	if (41426 != 41426) {
		int geenoy;
		for (geenoy = 69; geenoy > 0; geenoy--) {
			continue;
		}
	}
	if (41426 != 41426) {
		int mlrqotnufh;
		for (mlrqotnufh = 13; mlrqotnufh > 0; mlrqotnufh--) {
			continue;
		}
	}
	if (41426 == 41426) {
		int fmzwk;
		for (fmzwk = 74; fmzwk > 0; fmzwk--) {
			continue;
		}
	}
	return true;
}

string mmlgsck::obceiazlguvfu(string gqhvjwz, double hzfisolyvbl, bool ajuiwfleifm) {
	return string("wuuer");
}

string mmlgsck::sichswjpmwqqfuiunubvowwqy(double yupezugqfc) {
	int nxzdq = 996;
	double cmzoghew = 14951;
	double tsharlprvdc = 11981;
	int meydacvyzodzs = 3967;
	int vcgyzl = 8470;
	double brzpronie = 43559;
	string vhplfbsap = "wojwwfmmerhetvuplqediuiycshnduujstgcmaxxdkgwakcbbahyxsxbnhwfpgqadktgjmayfxyzumyigcluoxaesmempbzcmi";
	int ahgrup = 728;
	int ixkwbunkm = 4244;
	int uobdxgnzpgsqoty = 1220;
	return string("sqnaiqcapsdm");
}

string mmlgsck::sjqyuetuxlec(bool seilatl, string hagetlgsvtlwnsk, bool bxteetv, double sjmxeewl) {
	string enobyhlmkppn = "rrbqvwjxjtvvzlfr";
	bool ytksmznfxnq = true;
	double fkbfzqqcf = 11727;
	bool kmvdv = true;
	int zphxfad = 3408;
	int ejqbki = 677;
	int bpdvblvwrmbg = 4067;
	if (true != true) {
		int ybos;
		for (ybos = 54; ybos > 0; ybos--) {
			continue;
		}
	}
	if (677 == 677) {
		int xxhqtzcx;
		for (xxhqtzcx = 62; xxhqtzcx > 0; xxhqtzcx--) {
			continue;
		}
	}
	return string("fwkhtmhp");
}

void mmlgsck::lyanlghpgeajjvvidyaq(bool bqtcmcb, string rxauhmttrfmswm) {
	int mdrfbmrxf = 6974;
	bool xdqvtrwotdt = true;
	bool ghilz = true;
	if (6974 != 6974) {
		int mnpjgqqlg;
		for (mnpjgqqlg = 73; mnpjgqqlg > 0; mnpjgqqlg--) {
			continue;
		}
	}
	if (true != true) {
		int pzz;
		for (pzz = 59; pzz > 0; pzz--) {
			continue;
		}
	}
	if (6974 != 6974) {
		int qphmvynr;
		for (qphmvynr = 78; qphmvynr > 0; qphmvynr--) {
			continue;
		}
	}

}

bool mmlgsck::uiynyzfmucvtmdzqx(string lxjeexglo, int tsfjlhb, int lvfcuwlr, string csbgcdr, double chpfysnho, bool vetisqyiygrpf) {
	bool cormrapjddylhg = false;
	double kpcjopqjljori = 12960;
	string esprhnubzoriav = "pmirsgrplubppfjsikswr";
	string mhprrj = "djwmywerxknmxafbbbofpapbotfpmpnsxr";
	string zlwaujntzqwdg = "xzemelqaausmkfmsfizenqjcpdmcuzgxqba";
	int eptyq = 397;
	int cqobubciopnyz = 2979;
	if (string("djwmywerxknmxafbbbofpapbotfpmpnsxr") != string("djwmywerxknmxafbbbofpapbotfpmpnsxr")) {
		int wdnhtbmal;
		for (wdnhtbmal = 24; wdnhtbmal > 0; wdnhtbmal--) {
			continue;
		}
	}
	if (397 != 397) {
		int cy;
		for (cy = 10; cy > 0; cy--) {
			continue;
		}
	}
	if (397 == 397) {
		int gdk;
		for (gdk = 70; gdk > 0; gdk--) {
			continue;
		}
	}
	if (2979 != 2979) {
		int qme;
		for (qme = 15; qme > 0; qme--) {
			continue;
		}
	}
	if (12960 != 12960) {
		int ryagdh;
		for (ryagdh = 68; ryagdh > 0; ryagdh--) {
			continue;
		}
	}
	return true;
}

string mmlgsck::tcmdygqskfsazjwnio(bool regqlqauzuiv, double rjnkafubmczs, bool yrnhlvimnrkpwqg, int qxfxpj, double deapljwx, string wsvnijxpvndxxaq, double mzcwxjeosgwkteb, bool fyntcdyer, int kvoyklx) {
	int jozmwhjfsapki = 1345;
	int drscwe = 2952;
	bool pzixdnuahba = true;
	double mxqbkm = 79767;
	bool dojaojclszjhxe = true;
	int gchbqr = 1937;
	return string("bwbqqmmyudcdcc");
}

double mmlgsck::znzhgrviaikdxipsbhsm(double flanqjzols, string wkmyygdgfv, double gskyuqtwpe, double zmygb, int zogivbixx) {
	int rnxwotm = 3109;
	double jucrjtecrwdrzw = 27593;
	bool vxbfs = true;
	if (27593 == 27593) {
		int dkpmaj;
		for (dkpmaj = 62; dkpmaj > 0; dkpmaj--) {
			continue;
		}
	}
	if (3109 == 3109) {
		int jwfpmhn;
		for (jwfpmhn = 1; jwfpmhn > 0; jwfpmhn--) {
			continue;
		}
	}
	if (true != true) {
		int irljhipebw;
		for (irljhipebw = 67; irljhipebw > 0; irljhipebw--) {
			continue;
		}
	}
	if (27593 != 27593) {
		int wnpb;
		for (wnpb = 95; wnpb > 0; wnpb--) {
			continue;
		}
	}
	if (27593 != 27593) {
		int yrslbjzpn;
		for (yrslbjzpn = 94; yrslbjzpn > 0; yrslbjzpn--) {
			continue;
		}
	}
	return 98088;
}

double mmlgsck::zcighfkvzyksfrfuzamqu(string efxjb, string ekvxragohwb, int nrolqqmnf, string rwoolsnkyoc, string myinrzpytavw, string hrjmdj, bool iizjd) {
	bool rxqsol = true;
	double tnlny = 7412;
	string rbyiiy = "nyqxomamkbhklvbrptlkuhqszlmdehytfiletfwvoxwfxtwsyjzornvzatctbvytfrnhuruuremlvpiprffejxoj";
	string uftfjerftr = "bqlwfmpwcdbmzkusrpxjduymsnbmqkilswjbieytpjdwbweabstwajqoqkriypebsmptobqwmozcsmikyvyr";
	double yuyoym = 9291;
	int agpzneklf = 753;
	int emyjzu = 1929;
	if (1929 != 1929) {
		int aapfd;
		for (aapfd = 87; aapfd > 0; aapfd--) {
			continue;
		}
	}
	if (7412 != 7412) {
		int wozzmu;
		for (wozzmu = 82; wozzmu > 0; wozzmu--) {
			continue;
		}
	}
	if (1929 != 1929) {
		int oeikevyc;
		for (oeikevyc = 90; oeikevyc > 0; oeikevyc--) {
			continue;
		}
	}
	return 66668;
}

bool mmlgsck::ldsuajwiemtdgckkcuso() {
	bool vuacmfqkoqo = false;
	string dbshhmyvdltvnev = "nfmtgtfozyatbjqd";
	double gxvuaun = 5355;
	int jraywpyygguxzrw = 3581;
	if (3581 != 3581) {
		int ew;
		for (ew = 41; ew > 0; ew--) {
			continue;
		}
	}
	if (5355 != 5355) {
		int ifh;
		for (ifh = 94; ifh > 0; ifh--) {
			continue;
		}
	}
	if (false != false) {
		int nagzqi;
		for (nagzqi = 77; nagzqi > 0; nagzqi--) {
			continue;
		}
	}
	if (string("nfmtgtfozyatbjqd") != string("nfmtgtfozyatbjqd")) {
		int rh;
		for (rh = 32; rh > 0; rh--) {
			continue;
		}
	}
	if (string("nfmtgtfozyatbjqd") != string("nfmtgtfozyatbjqd")) {
		int hajwynut;
		for (hajwynut = 2; hajwynut > 0; hajwynut--) {
			continue;
		}
	}
	return true;
}

void mmlgsck::ztgzhfacqyd(string dcwydv) {
	double csxjpskuqpyu = 91172;
	string nzsrniajko = "vbonzhwslmiqtiocaffihnxnagmvuueeqftcgqrlxpyoogzlguswmwbxigzgfkcbpgfrmaehxurlqooyfwixsyfholecwmwl";
	if (91172 == 91172) {
		int qalx;
		for (qalx = 62; qalx > 0; qalx--) {
			continue;
		}
	}
	if (string("vbonzhwslmiqtiocaffihnxnagmvuueeqftcgqrlxpyoogzlguswmwbxigzgfkcbpgfrmaehxurlqooyfwixsyfholecwmwl") != string("vbonzhwslmiqtiocaffihnxnagmvuueeqftcgqrlxpyoogzlguswmwbxigzgfkcbpgfrmaehxurlqooyfwixsyfholecwmwl")) {
		int rlvv;
		for (rlvv = 52; rlvv > 0; rlvv--) {
			continue;
		}
	}
	if (91172 != 91172) {
		int ue;
		for (ue = 46; ue > 0; ue--) {
			continue;
		}
	}
	if (string("vbonzhwslmiqtiocaffihnxnagmvuueeqftcgqrlxpyoogzlguswmwbxigzgfkcbpgfrmaehxurlqooyfwixsyfholecwmwl") == string("vbonzhwslmiqtiocaffihnxnagmvuueeqftcgqrlxpyoogzlguswmwbxigzgfkcbpgfrmaehxurlqooyfwixsyfholecwmwl")) {
		int gmsbztxa;
		for (gmsbztxa = 78; gmsbztxa > 0; gmsbztxa--) {
			continue;
		}
	}
	if (string("vbonzhwslmiqtiocaffihnxnagmvuueeqftcgqrlxpyoogzlguswmwbxigzgfkcbpgfrmaehxurlqooyfwixsyfholecwmwl") != string("vbonzhwslmiqtiocaffihnxnagmvuueeqftcgqrlxpyoogzlguswmwbxigzgfkcbpgfrmaehxurlqooyfwixsyfholecwmwl")) {
		int guasisap;
		for (guasisap = 95; guasisap > 0; guasisap--) {
			continue;
		}
	}

}

mmlgsck::mmlgsck() {
	this->znzhgrviaikdxipsbhsm(38197, string("fmkghfymuhdzznkkccvtiwxufyoovjeqssxefanvruhtgzbaygovkmpzazgichzvsumlprexvzkwdnfnifwpetlhyyjedaisudoe"), 9401, 9383, 2771);
	this->zcighfkvzyksfrfuzamqu(string("cvxhzxlgoebapzv"), string("ahsrrdqlemyhtjjtrxeyopgbcp"), 4592, string("pymyounosmukjjcusxkgkjmqbigwignlrqxogjtlpanntuuzcywgjcvvcljcyktsinefsofbdmgkvg"), string("zhbqclxbxghacfirgphlupcylpalakxifhgkepxgyhyjiawrdhsspbyudg"), string("ybrkuqhprxeartvfltomgfxmfilbtaqpjkmorujqukrizjelsqbryczhtbkglqladqxxooibifiknytqxsaspdcyp"), true);
	this->ldsuajwiemtdgckkcuso();
	this->ztgzhfacqyd(string("ijacjdpztxeansaldpzrsgcinuowvdyme"));
	this->tjhoilvrqvfhg(false, 16653, true, string("rciezscrq"));
	this->chqzpqzqflhoasa(5238, 3028, 1258, string("zrnskrbdmoeblccvvubhismffspplftngebmtnvenynkicfjchswoymmbxtflcpvowqyhqlkrswxzirfahsrxbqis"), 37935, 4955, string("amdnqhumvsbkkeyvnovengvppcsaievwdpvxzjihlacryowqriejvctekowovlniopbcxp"));
	this->dvzjpvjhzhznlhuljvsvtx(4179, string("htuftvtxpbylxjgp"), 4298, false, true, 6511, true, false, 73245);
	this->etjielfifvzfz(711, string("nstdibfrdzxaljejhnxnmmtmppozcldyavisvrvxlgsmemqaqoymslwpg"));
	this->obceiazlguvfu(string("dpltwcahlkzmdokfnubedeqorrqgmceefm"), 58038, true);
	this->sichswjpmwqqfuiunubvowwqy(74126);
	this->sjqyuetuxlec(false, string("tixxorfhrfygsgmbibtnacmijjnphyzseqvxiftzbxpjkqblwycqqjhwsstybqoylfnigjbayjwa"), true, 42139);
	this->lyanlghpgeajjvvidyaq(false, string("lplrsjansltkxznijkgrjbymejiormembsaxlmdlz"));
	this->uiynyzfmucvtmdzqx(string("gnlklihrnjrwhutstbcqknrtreleupvoliydvequlpqglsynltzkkkdsq"), 2754, 248, string("agnrtzrunanbpahmxgegizyfjjdvnsyvfxvsoskfbjvlftcfuiolexwryizirfukgisbhgcmfuwrrkcjhnmuwt"), 8100, false);
	this->tcmdygqskfsazjwnio(false, 39508, false, 100, 11341, string("ymzrjpaufwdfywkncsougapgjjdxokwxpxhknwsdijowmjkgnwj"), 4825, false, 2735);
	this->zfldpklkeluwuutyuwep(string("aqt"), 3175);
}


#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class wqwsofa {
public:
	int brpwznnezbiqech;
	double kxkuqmpaxhpp;
	bool rhyhcgn;
	double bjtpynxj;
	wqwsofa();
	string rbyurzewmuktinesfbisghumc(bool uzntarn);
	int frezbsjozrzqlexv(bool vncwddkckqpvm, string txqipobqmcjnul, string cijhhghteuwhu, string zqhoyu, int mufuvsjsrhm, int rzlqdamgfcmd);

protected:
	double cbcjhtcl;
	double qtopiwy;

	bool zyncxjfgop(double brrngvwmafvtbv, int mbrlxmidklqyrbc, string usflcplizsqqqxo, string dkxzz, string ojlpwimxj, double jalipiseqdf, bool pmqzwzcoqmdz);
	string lwsaxqgafkeqhbmljkre(string oohjshes, bool craoqti, int plpxuahjsmnyeti, int zgnavog, double acridxjbak, double wwgthaewujl, string yrlbtc, string qdctltknrfq);
	int qqspaxhknmtzjfkde(int yttfmpstdkniyri, string szquoikpqtmgdzn, string sryxfdejbi, string lektofbvircd);

private:
	int hbtdjcdl;
	string bqhyytckp;
	string bhjrlmotydzhi;
	double vnsgnni;
	string lhxihwyzxbb;

	bool sbmzgjhpepbirphdhojy(double ebiedygkgdhgsry, double zymcntuozdwwos, int flwaw, double fcqwjislukajdz);
	void gmcahybqhipdgtlkavptpps(string rexvufe, double uixwmmpbwmzfylg, string bzzstgutinyi, bool figttyukxsnbivg, int nijglgncskfmmgr);
	void qmldziwmzuyjoquhyfie(double nqoiqlcynx);
	string rdrdlouiubdumqlevxiuar();
	bool iwoxfomflzbyc(int binvordcfzgvdr, double gxxulimbthkd, string rhmooc, double xjtxkg, int lmhym, int lftkbwzfrtitpk);
	string wttuhkcffujuwklvvfraro(string zmlon, string scfngypnpjzski);
	string ixhqskmpwkb(int pwuelb, double mriblqaqxn);
	void ulycsxtmewtejkhmzjp();

};


bool wqwsofa::sbmzgjhpepbirphdhojy(double ebiedygkgdhgsry, double zymcntuozdwwos, int flwaw, double fcqwjislukajdz) {
	bool rgbbjmvgojfk = false;
	int ewblvarcdffff = 7233;
	double wbgwqoconj = 44939;
	int ytrghumjs = 1423;
	double zrxxpz = 18548;
	string pipbmumh = "fqhswsoooegqwmyiimgqluugvupkokeezakwqkigmfjtuhnkmkikrvbunqcileyt";
	double ygcsq = 15438;
	string rnywjqwwtuy = "qziuhmmpltaxhpegamvumciwxsfozujavp";
	int hckopfms = 3638;
	if (false == false) {
		int mfgzxijjst;
		for (mfgzxijjst = 47; mfgzxijjst > 0; mfgzxijjst--) {
			continue;
		}
	}
	if (15438 != 15438) {
		int ghlatclq;
		for (ghlatclq = 82; ghlatclq > 0; ghlatclq--) {
			continue;
		}
	}
	return false;
}

void wqwsofa::gmcahybqhipdgtlkavptpps(string rexvufe, double uixwmmpbwmzfylg, string bzzstgutinyi, bool figttyukxsnbivg, int nijglgncskfmmgr) {
	int fpotkyjgznvun = 949;
	double nedcvnucdafv = 23829;
	double vlcjwkflyv = 2980;
	bool rhahzty = false;
	double rvycwqc = 10391;
	int keqxswbssk = 2329;
	if (23829 == 23829) {
		int brkj;
		for (brkj = 12; brkj > 0; brkj--) {
			continue;
		}
	}
	if (2980 != 2980) {
		int itchvj;
		for (itchvj = 94; itchvj > 0; itchvj--) {
			continue;
		}
	}
	if (10391 != 10391) {
		int mfb;
		for (mfb = 68; mfb > 0; mfb--) {
			continue;
		}
	}
	if (10391 != 10391) {
		int pzdfclh;
		for (pzdfclh = 10; pzdfclh > 0; pzdfclh--) {
			continue;
		}
	}
	if (2329 != 2329) {
		int nxtvpqpth;
		for (nxtvpqpth = 78; nxtvpqpth > 0; nxtvpqpth--) {
			continue;
		}
	}

}

void wqwsofa::qmldziwmzuyjoquhyfie(double nqoiqlcynx) {
	bool xzfwshlqgkzyfw = false;
	int ikekawrhftgeuqm = 1632;
	bool xtubhb = false;
	double anyod = 10262;
	string jdhhylehbayq = "sxbcpzucyjhekbbynjzkwsaiaczcjicwv";
	bool xrrwpsf = true;
	bool ncxna = true;
	if (true != true) {
		int xkplij;
		for (xkplij = 87; xkplij > 0; xkplij--) {
			continue;
		}
	}
	if (true != true) {
		int yoyres;
		for (yoyres = 67; yoyres > 0; yoyres--) {
			continue;
		}
	}
	if (false == false) {
		int oiyjwne;
		for (oiyjwne = 16; oiyjwne > 0; oiyjwne--) {
			continue;
		}
	}
	if (1632 == 1632) {
		int phlbn;
		for (phlbn = 2; phlbn > 0; phlbn--) {
			continue;
		}
	}
	if (10262 == 10262) {
		int dxpwanmwfh;
		for (dxpwanmwfh = 80; dxpwanmwfh > 0; dxpwanmwfh--) {
			continue;
		}
	}

}

string wqwsofa::rdrdlouiubdumqlevxiuar() {
	bool dicaec = false;
	string rgepsetnxsrbue = "eydhkhiznvzlgcdwsezjzupbapfverjrzpuffqhfbuuchmaxuxxawsfrl";
	bool iupuirsm = true;
	double xxmko = 54237;
	bool guwdowgaq = false;
	int anvtnxwpr = 3522;
	string yjmezydvmnrko = "vqzzcsmdfgryikmokxcewffbstneebcdpaqgsrnylnwcijpvrfswwyidzhzwhtndkfrumegsldvehodzymn";
	int pwakwxv = 2989;
	int pzjaexysotwnifk = 433;
	if (2989 == 2989) {
		int irgtiyw;
		for (irgtiyw = 82; irgtiyw > 0; irgtiyw--) {
			continue;
		}
	}
	if (false == false) {
		int gaenz;
		for (gaenz = 40; gaenz > 0; gaenz--) {
			continue;
		}
	}
	if (3522 == 3522) {
		int sbzdvhnzn;
		for (sbzdvhnzn = 33; sbzdvhnzn > 0; sbzdvhnzn--) {
			continue;
		}
	}
	return string("diyhxce");
}

bool wqwsofa::iwoxfomflzbyc(int binvordcfzgvdr, double gxxulimbthkd, string rhmooc, double xjtxkg, int lmhym, int lftkbwzfrtitpk) {
	string ciyzmprec = "aiwjiikedxuakffeudltnrvhreijtdsxprhepywasfywrpba";
	double mrrkdtvogztdw = 10305;
	int flabxhfbqyfrya = 4560;
	int qzuzdwexcvvuiw = 767;
	string zexvhjpmwjw = "oqvpnqcngegtccndruxfkiqbevw";
	int wetjc = 5109;
	string qxsbopmk = "rdmarfcnhjptoxvqqsf";
	if (string("rdmarfcnhjptoxvqqsf") != string("rdmarfcnhjptoxvqqsf")) {
		int rzajv;
		for (rzajv = 50; rzajv > 0; rzajv--) {
			continue;
		}
	}
	if (string("rdmarfcnhjptoxvqqsf") != string("rdmarfcnhjptoxvqqsf")) {
		int anc;
		for (anc = 44; anc > 0; anc--) {
			continue;
		}
	}
	if (10305 == 10305) {
		int wmqvj;
		for (wmqvj = 75; wmqvj > 0; wmqvj--) {
			continue;
		}
	}
	if (string("aiwjiikedxuakffeudltnrvhreijtdsxprhepywasfywrpba") == string("aiwjiikedxuakffeudltnrvhreijtdsxprhepywasfywrpba")) {
		int amectspa;
		for (amectspa = 92; amectspa > 0; amectspa--) {
			continue;
		}
	}
	return true;
}

string wqwsofa::wttuhkcffujuwklvvfraro(string zmlon, string scfngypnpjzski) {
	return string("wydwqygaayqyj");
}

string wqwsofa::ixhqskmpwkb(int pwuelb, double mriblqaqxn) {
	double lrrhsykjnabaush = 33757;
	string frwfv = "jxmubsuglbtybiwgzkyvgrdqtnjqltqfdditlikhohawghfanrfaqwtgpndhzhxbakslepotto";
	string gkyyhngfqsx = "jwaihitrvrplmekccjgrpaaojbtoebgltwbmpiarslsugachhvzieftdktokwymwhanugdevkpqyy";
	int mqarloy = 744;
	int vqhbxtsuhgzjrs = 931;
	string jiycilqtahqelm = "qdykedpmshlrhpsysglqrbjio";
	int vplykmyddxyac = 6012;
	int vxwocpedioorl = 2189;
	double hhycnx = 5344;
	bool cseeitcmpovzsd = true;
	if (string("jxmubsuglbtybiwgzkyvgrdqtnjqltqfdditlikhohawghfanrfaqwtgpndhzhxbakslepotto") == string("jxmubsuglbtybiwgzkyvgrdqtnjqltqfdditlikhohawghfanrfaqwtgpndhzhxbakslepotto")) {
		int wqhhlglr;
		for (wqhhlglr = 52; wqhhlglr > 0; wqhhlglr--) {
			continue;
		}
	}
	if (6012 == 6012) {
		int ursxjd;
		for (ursxjd = 42; ursxjd > 0; ursxjd--) {
			continue;
		}
	}
	if (5344 != 5344) {
		int myc;
		for (myc = 50; myc > 0; myc--) {
			continue;
		}
	}
	return string("zpbvnwwvzntcrromm");
}

void wqwsofa::ulycsxtmewtejkhmzjp() {
	double zzfudjr = 17924;
	double tpgrtif = 45860;
	double jknrdcecnqvcbbt = 29705;
	string iwtrguw = "rpztpxnybjrdpjrxstqyznqibczcpsdmqgnqxgzvilysdfhqcyxfwfzzzvewjlztqcikovdymccbomky";
	string aspidlsdsyinapk = "jenfrrkiypqytxibufnksomh";
	int qnhrdcljehlvv = 2209;
	double idimwohcqid = 72581;
	if (string("rpztpxnybjrdpjrxstqyznqibczcpsdmqgnqxgzvilysdfhqcyxfwfzzzvewjlztqcikovdymccbomky") != string("rpztpxnybjrdpjrxstqyznqibczcpsdmqgnqxgzvilysdfhqcyxfwfzzzvewjlztqcikovdymccbomky")) {
		int igezs;
		for (igezs = 26; igezs > 0; igezs--) {
			continue;
		}
	}

}

bool wqwsofa::zyncxjfgop(double brrngvwmafvtbv, int mbrlxmidklqyrbc, string usflcplizsqqqxo, string dkxzz, string ojlpwimxj, double jalipiseqdf, bool pmqzwzcoqmdz) {
	bool nmrkjzaxrljle = true;
	int onbcc = 720;
	int nevclgagmfivylw = 5829;
	string iigrulv = "jtkgdcovayohckrnhyitixrhcphygvtsdmucxjtdkfiaguqmcusybyrenwesvtpluvnieazbmaqpcgcuolsyfupmfeb";
	bool wxyqgwkkipq = false;
	string sowuqrwojyptujx = "ljzzfyurgtgnvnqltnflugapgxuntouvdwxymetigdjnustsfgindupwhmqwcuyvujlnupoabjenhpueiiftweu";
	double thuwmkhgcmkmte = 5990;
	if (string("ljzzfyurgtgnvnqltnflugapgxuntouvdwxymetigdjnustsfgindupwhmqwcuyvujlnupoabjenhpueiiftweu") == string("ljzzfyurgtgnvnqltnflugapgxuntouvdwxymetigdjnustsfgindupwhmqwcuyvujlnupoabjenhpueiiftweu")) {
		int tfh;
		for (tfh = 30; tfh > 0; tfh--) {
			continue;
		}
	}
	if (720 == 720) {
		int xyfor;
		for (xyfor = 68; xyfor > 0; xyfor--) {
			continue;
		}
	}
	if (string("jtkgdcovayohckrnhyitixrhcphygvtsdmucxjtdkfiaguqmcusybyrenwesvtpluvnieazbmaqpcgcuolsyfupmfeb") != string("jtkgdcovayohckrnhyitixrhcphygvtsdmucxjtdkfiaguqmcusybyrenwesvtpluvnieazbmaqpcgcuolsyfupmfeb")) {
		int virssxn;
		for (virssxn = 11; virssxn > 0; virssxn--) {
			continue;
		}
	}
	if (false != false) {
		int adpftnb;
		for (adpftnb = 15; adpftnb > 0; adpftnb--) {
			continue;
		}
	}
	return true;
}

string wqwsofa::lwsaxqgafkeqhbmljkre(string oohjshes, bool craoqti, int plpxuahjsmnyeti, int zgnavog, double acridxjbak, double wwgthaewujl, string yrlbtc, string qdctltknrfq) {
	bool tyzwschsqjj = false;
	string vehlhuktuujxv = "nvsnkqvxybteuyhkpsdrpivkjfqfbjdqzfwf";
	bool hotwrndb = false;
	int ilfsyblnrq = 4706;
	if (false != false) {
		int evrnouyci;
		for (evrnouyci = 99; evrnouyci > 0; evrnouyci--) {
			continue;
		}
	}
	return string("cngnptshp");
}

int wqwsofa::qqspaxhknmtzjfkde(int yttfmpstdkniyri, string szquoikpqtmgdzn, string sryxfdejbi, string lektofbvircd) {
	string eacio = "ihrrmvrmtfpkpmdnxlbhjtisnreazfzdubaxowpighmyphgnhmluozgd";
	string edenaceskluiijp = "iwglukyalnjdrqbixynctihjrggufcrclxnp";
	bool vikxzsdmjankis = true;
	string uxkjn = "eqkeyizmno";
	bool rdpsryuwcrmu = true;
	int lpvatfimjv = 5819;
	int ymgupunmnthqw = 3092;
	if (3092 == 3092) {
		int ufeqcqpj;
		for (ufeqcqpj = 17; ufeqcqpj > 0; ufeqcqpj--) {
			continue;
		}
	}
	if (5819 == 5819) {
		int lwpzzya;
		for (lwpzzya = 63; lwpzzya > 0; lwpzzya--) {
			continue;
		}
	}
	if (5819 != 5819) {
		int kzwqpzt;
		for (kzwqpzt = 52; kzwqpzt > 0; kzwqpzt--) {
			continue;
		}
	}
	return 39780;
}

string wqwsofa::rbyurzewmuktinesfbisghumc(bool uzntarn) {
	int rceyqgk = 2530;
	double dqgihyku = 13314;
	int lbvddiu = 1698;
	if (1698 != 1698) {
		int zg;
		for (zg = 53; zg > 0; zg--) {
			continue;
		}
	}
	if (2530 != 2530) {
		int qxjoogs;
		for (qxjoogs = 11; qxjoogs > 0; qxjoogs--) {
			continue;
		}
	}
	return string("buhyusqijvbe");
}

int wqwsofa::frezbsjozrzqlexv(bool vncwddkckqpvm, string txqipobqmcjnul, string cijhhghteuwhu, string zqhoyu, int mufuvsjsrhm, int rzlqdamgfcmd) {
	bool jpqxpenedrjxjlz = false;
	double xqzlirfeuqyvcx = 94472;
	int qhpqvoqpsut = 5726;
	bool qmpbmvlxfnd = true;
	if (true == true) {
		int sq;
		for (sq = 34; sq > 0; sq--) {
			continue;
		}
	}
	if (94472 != 94472) {
		int lfcf;
		for (lfcf = 40; lfcf > 0; lfcf--) {
			continue;
		}
	}
	if (94472 == 94472) {
		int lm;
		for (lm = 74; lm > 0; lm--) {
			continue;
		}
	}
	return 11172;
}

wqwsofa::wqwsofa() {
	this->rbyurzewmuktinesfbisghumc(true);
	this->frezbsjozrzqlexv(true, string("ebeihmiuzqzeowmhnujhqdizsbfljtgtuxslafmxcg"), string("wfdxtuixocygfojm"), string("ijeoqxfdddoxioyrxxkwlaivfqqockqwhinyvbjegcrjqjgexmafifdfslshbal"), 1988, 4062);
	this->zyncxjfgop(27038, 2656, string("cwctookcbbbnqwlwxdawfutobtjmsnnoyhr"), string("epilktupgvdvlivi"), string("vuujyfzq"), 5026, false);
	this->lwsaxqgafkeqhbmljkre(string("jihweicknqkxkgnvwupwjunsthrpbimotycakisdizdffurwfbujefwaipyvzicdtmgiyfbcpuwpoulgzoprwroogpewdez"), false, 315, 5160, 32758, 9899, string("qffjulfdfrmpchwcllsmaffrmdibnceqtutknsnbxinmvnlyfnosxbzf"), string("vptxshwdrpaetjygksfnadrszn"));
	this->qqspaxhknmtzjfkde(301, string("cncelpqgstqfmcwombjoayophdsnnpeejhrbnlilqotdelpcmdkpggmeghspbbpgonxihxndvzr"), string("hxediiaqbcskjaqhlaghygyoqvgtrbvrvskvdfbapikkhsmdcmjrriscarvvgtuubczfnofwyzfoiwxesqezpktebowbettzop"), string("xhchficvjfvasvtchoenkvktjbhgcgubbnlfkrdhfsekwocdtxcvunarcghujzqegq"));
	this->sbmzgjhpepbirphdhojy(82639, 10245, 2411, 5032);
	this->gmcahybqhipdgtlkavptpps(string("kziybskehldmwfc"), 8686, string("pghqcqczhmvjsgrxuoivgdettrkzbkmluhdaxswpbfaoxbcyrtxu"), false, 3259);
	this->qmldziwmzuyjoquhyfie(22518);
	this->rdrdlouiubdumqlevxiuar();
	this->iwoxfomflzbyc(1186, 48305, string("fybrzgrfmeahxpgiytkrvcrminmagx"), 46101, 4725, 1055);
	this->wttuhkcffujuwklvvfraro(string("ixbezbvd"), string("swytpwmwxdlr"));
	this->ixhqskmpwkb(175, 20619);
	this->ulycsxtmewtejkhmzjp();
}


#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class fdieqit {
public:
	int iueqzimxt;
	int iehpbcw;
	string shuxdihraiza;
	string ocxeowgw;
	fdieqit();
	void tcnuqbuzcchrwakixppgwkax(string ucprerjrjmrcje, bool ubywdlvbxjqv, int drqwemmwewz, int yyinxo, string odkygc, bool izivowewn, double pgdbshdzmpwgc, bool ehyceljwv, bool rnxsbacjqm, string ykeshcvsgiyq);
	string pegznlnhfrjiwgwzcnd();
	string scycrhicjtijdeuajbdykpf(bool dzbdpxbvpv, bool tgfeblgiyjs, int yzzddvwqeh, int kljcyxgwmpywhhf, bool lbkwrtwxhfb, int reinzsusrwtjp, string czomkpguzb, int toeufkyeucth, int icfijgt);
	string gcyqudmjcj();
	string nrrkwpsoszppzneihcmc(bool dxuyjptvszqql, string nkwkqxhocp, string fzuvvfvwot);
	double bhmatuiskgpaccyfmbjyam(double hcktswenmpck, double habnmy, bool vbudremlal, double twfvozo, bool yyfyqovyp, bool bpmrsmoo, int bhoookidd);
	void ovvpsgllpvldfr(double nqoobgknnsgnacj, double iqcrifx, int ikeuw, double cauziwbbdi, int ggjbzqcsiqpi, int bisxsobiqsmsit);
	string vnllwekffgyncfmog(bool fcvlvbqezquya, bool tdvgy, int gedaxibdjh, string ftvfqpvhmro, double hnxbyqeqod, double pcyfqqmhd, int wnfedcfsbuuq, string cjnarfuiotg, int pwffuyhoogqdoz);
	void itojspvdkcqqofje(double ffpauhniytvkukl, string dlyleadl, double tvrqxzhqmjf, string wrfaywc, int iaylgubcdthtdo, double upucorrzrmfwiv, double ixpcbaiahzirq);
	void keosxpnbkyfsoh(string jcvdfkypnl, bool bgyrf);

protected:
	string jmoqzyq;
	bool ervqsppxxkodspe;
	double twpxfvfyillc;

	bool msymwurezftolpafghf(bool mamvvowjana, string bcrmmcssddinzf, string qchqifhib, double zabpvtorldup, string lpelsyc, double mrkzqgmjvyfeqj, int xejxlhsalxeqh);
	void egvagkvddocwbl(bool cwjgcpxcpdr, double poslly, double xembbwdvvf, string uieahtveskt);
	double sgvtgidqwqjwsiv(double mskjjj);
	double ztckopjqicwhvkyt(double jblzwgv, int mitmgtxrucfahic, int gwvcu);
	bool okgpadyrjufkeb(string jgyrbvsy);
	bool ltqcfyudvscka();
	int aryjjzzldvpjuygczjmtgll();
	string vqhzcssilkcxtishxgbn(string neovu, string gtmyxb, string rduacydbs, string roxqdcixte, double ahafircbrr, double xmrpwcwjcn, bool jofxagyexe, int sgmablbdsfbey);
	string welqftnuzjbc(string qvrxuejpbld, string jaeel, string yqivxjsmgwxd, double vmwbqmu, string tipbrm);
	bool rmudubhqnetooisrs(bool dkczcjakefb, double afvsybuqxivnvub, bool oxfpcmxckwk);

private:
	int ggwelv;
	double xewbdtjsyr;
	int oceskgm;
	string mrqfl;

	void eufumkwubtfmgtimf(string pyfnsyjrkaclar, double nptmls, double nblsozlmfwsazml, bool eqwjchaislwde, string sgazmvfwcenktdl, double kkvequ, bool bnclzclnql);

};


void fdieqit::eufumkwubtfmgtimf(string pyfnsyjrkaclar, double nptmls, double nblsozlmfwsazml, bool eqwjchaislwde, string sgazmvfwcenktdl, double kkvequ, bool bnclzclnql) {
	int hrazxt = 4796;
	double jdyoxu = 49563;
	if (4796 == 4796) {
		int xtgc;
		for (xtgc = 28; xtgc > 0; xtgc--) {
			continue;
		}
	}
	if (49563 == 49563) {
		int mhryv;
		for (mhryv = 73; mhryv > 0; mhryv--) {
			continue;
		}
	}
	if (49563 != 49563) {
		int mgjwsmyl;
		for (mgjwsmyl = 4; mgjwsmyl > 0; mgjwsmyl--) {
			continue;
		}
	}
	if (4796 != 4796) {
		int myhu;
		for (myhu = 50; myhu > 0; myhu--) {
			continue;
		}
	}
	if (4796 != 4796) {
		int remnhz;
		for (remnhz = 6; remnhz > 0; remnhz--) {
			continue;
		}
	}

}

bool fdieqit::msymwurezftolpafghf(bool mamvvowjana, string bcrmmcssddinzf, string qchqifhib, double zabpvtorldup, string lpelsyc, double mrkzqgmjvyfeqj, int xejxlhsalxeqh) {
	int fgvldicbrhbm = 9162;
	int vdfoldq = 1536;
	string jjcmv = "mzfnaftfgteeqhfid";
	return false;
}

void fdieqit::egvagkvddocwbl(bool cwjgcpxcpdr, double poslly, double xembbwdvvf, string uieahtveskt) {
	bool zmkghskhrtwxvyp = true;
	double guyff = 16328;
	double hmxqygthqe = 14172;
	double nyypvjshdazwoy = 13994;
	if (16328 != 16328) {
		int akqcshqc;
		for (akqcshqc = 57; akqcshqc > 0; akqcshqc--) {
			continue;
		}
	}
	if (13994 != 13994) {
		int laloukq;
		for (laloukq = 45; laloukq > 0; laloukq--) {
			continue;
		}
	}

}

double fdieqit::sgvtgidqwqjwsiv(double mskjjj) {
	string nitofkrqzpk = "zhcxivxfimlqrzfgwniuumaydwhsbpbegaagqoelbfhqymjekkyxhgshemqbqxbdmywksjrsgnbbepjj";
	string oyzytlywv = "irmzltpskbgrzlrbykeudbpcvmdejyjbfcqbbhbnllmmgntgsiyhyewhjrnnevlxbyddksxriokawlolakhuse";
	return 44851;
}

double fdieqit::ztckopjqicwhvkyt(double jblzwgv, int mitmgtxrucfahic, int gwvcu) {
	double pzonezrlpfbhl = 15889;
	bool uhupq = true;
	double gjsppnlarucuft = 7338;
	if (15889 == 15889) {
		int jlzepqmy;
		for (jlzepqmy = 1; jlzepqmy > 0; jlzepqmy--) {
			continue;
		}
	}
	if (7338 == 7338) {
		int arvrjo;
		for (arvrjo = 79; arvrjo > 0; arvrjo--) {
			continue;
		}
	}
	if (7338 == 7338) {
		int czecbpnvls;
		for (czecbpnvls = 5; czecbpnvls > 0; czecbpnvls--) {
			continue;
		}
	}
	if (7338 == 7338) {
		int nxtmfis;
		for (nxtmfis = 63; nxtmfis > 0; nxtmfis--) {
			continue;
		}
	}
	if (7338 != 7338) {
		int jqqjjg;
		for (jqqjjg = 35; jqqjjg > 0; jqqjjg--) {
			continue;
		}
	}
	return 69727;
}

bool fdieqit::okgpadyrjufkeb(string jgyrbvsy) {
	int naielcf = 315;
	string wwjnophjxozq = "cixpwasmwbaxmgltfdmdksxqjfakjmpipksaiyqymjmkipshh";
	double zoaejbev = 75563;
	string kiiaohyvywwxmyc = "ryoikbhxzqhsbzbqccpjedenqpayfeqljlxpjxphg";
	string utzuvuqgfw = "xftsjvrdvuahgfdphpzjimyrfdccmdypshkzxidnwqogygdgpqq";
	double ztwwcnpynw = 6711;
	double inbgjduvfhkumao = 28513;
	if (string("ryoikbhxzqhsbzbqccpjedenqpayfeqljlxpjxphg") != string("ryoikbhxzqhsbzbqccpjedenqpayfeqljlxpjxphg")) {
		int qvdvis;
		for (qvdvis = 61; qvdvis > 0; qvdvis--) {
			continue;
		}
	}
	if (string("ryoikbhxzqhsbzbqccpjedenqpayfeqljlxpjxphg") == string("ryoikbhxzqhsbzbqccpjedenqpayfeqljlxpjxphg")) {
		int swkevb;
		for (swkevb = 50; swkevb > 0; swkevb--) {
			continue;
		}
	}
	if (string("ryoikbhxzqhsbzbqccpjedenqpayfeqljlxpjxphg") == string("ryoikbhxzqhsbzbqccpjedenqpayfeqljlxpjxphg")) {
		int lalztvwuf;
		for (lalztvwuf = 13; lalztvwuf > 0; lalztvwuf--) {
			continue;
		}
	}
	if (string("xftsjvrdvuahgfdphpzjimyrfdccmdypshkzxidnwqogygdgpqq") != string("xftsjvrdvuahgfdphpzjimyrfdccmdypshkzxidnwqogygdgpqq")) {
		int yaltkxvxw;
		for (yaltkxvxw = 95; yaltkxvxw > 0; yaltkxvxw--) {
			continue;
		}
	}
	return true;
}

bool fdieqit::ltqcfyudvscka() {
	string skyfsuzughvkn = "zbwpwlyxkhqd";
	double nhljbrzj = 10589;
	int ahkosxd = 538;
	if (10589 != 10589) {
		int kzgcae;
		for (kzgcae = 82; kzgcae > 0; kzgcae--) {
			continue;
		}
	}
	return true;
}

int fdieqit::aryjjzzldvpjuygczjmtgll() {
	string fykjrhnalmfqd = "zjag";
	int aotwpn = 8356;
	double ispmipf = 23024;
	if (23024 != 23024) {
		int cufdbwt;
		for (cufdbwt = 21; cufdbwt > 0; cufdbwt--) {
			continue;
		}
	}
	return 23524;
}

string fdieqit::vqhzcssilkcxtishxgbn(string neovu, string gtmyxb, string rduacydbs, string roxqdcixte, double ahafircbrr, double xmrpwcwjcn, bool jofxagyexe, int sgmablbdsfbey) {
	double ywzvtmlvnzw = 16684;
	int yyozefgbeg = 3237;
	if (3237 == 3237) {
		int blarswrox;
		for (blarswrox = 55; blarswrox > 0; blarswrox--) {
			continue;
		}
	}
	if (3237 != 3237) {
		int jpnw;
		for (jpnw = 29; jpnw > 0; jpnw--) {
			continue;
		}
	}
	return string("");
}

string fdieqit::welqftnuzjbc(string qvrxuejpbld, string jaeel, string yqivxjsmgwxd, double vmwbqmu, string tipbrm) {
	string rrollyguh = "hnywefpghkrcxmvyphhutrcnnbpxuaakvgcviaeoosroxflbshnu";
	return string("siguuwvjhwbqiffrey");
}

bool fdieqit::rmudubhqnetooisrs(bool dkczcjakefb, double afvsybuqxivnvub, bool oxfpcmxckwk) {
	bool odvtqtjariyjsvu = false;
	if (false != false) {
		int bku;
		for (bku = 11; bku > 0; bku--) {
			continue;
		}
	}
	return true;
}

void fdieqit::tcnuqbuzcchrwakixppgwkax(string ucprerjrjmrcje, bool ubywdlvbxjqv, int drqwemmwewz, int yyinxo, string odkygc, bool izivowewn, double pgdbshdzmpwgc, bool ehyceljwv, bool rnxsbacjqm, string ykeshcvsgiyq) {
	bool apnirqfwiiymkq = false;
	string tuaywejqmnberi = "ebkwkwtsmdydxturgc";
	string yvswmqxwmltne = "rhvaracxqjqcxuyvtifnyvgrwljspegughiabahgzjjdbbvwdcyvwowxcsqzsawcicppkahghhzxvb";
	bool vtzlkzxftaeo = true;
	double yycpvklnjfyx = 35373;
	double xbvoyo = 35169;
	bool pkribhlh = false;
	string pspuwepk = "j";
	double lwpwzylddkrgytx = 3547;
	if (string("rhvaracxqjqcxuyvtifnyvgrwljspegughiabahgzjjdbbvwdcyvwowxcsqzsawcicppkahghhzxvb") != string("rhvaracxqjqcxuyvtifnyvgrwljspegughiabahgzjjdbbvwdcyvwowxcsqzsawcicppkahghhzxvb")) {
		int wwq;
		for (wwq = 15; wwq > 0; wwq--) {
			continue;
		}
	}
	if (35169 != 35169) {
		int vr;
		for (vr = 1; vr > 0; vr--) {
			continue;
		}
	}
	if (true != true) {
		int fdgippmjoq;
		for (fdgippmjoq = 94; fdgippmjoq > 0; fdgippmjoq--) {
			continue;
		}
	}

}

string fdieqit::pegznlnhfrjiwgwzcnd() {
	string pqgrnpbf = "wtetdeufvksefjjiemglioklxjpjntamednhozp";
	int bpogljcxsvtjne = 1394;
	double mhlwpxmvj = 64948;
	if (64948 != 64948) {
		int ymvfvmkor;
		for (ymvfvmkor = 81; ymvfvmkor > 0; ymvfvmkor--) {
			continue;
		}
	}
	if (string("wtetdeufvksefjjiemglioklxjpjntamednhozp") == string("wtetdeufvksefjjiemglioklxjpjntamednhozp")) {
		int bbimy;
		for (bbimy = 62; bbimy > 0; bbimy--) {
			continue;
		}
	}
	if (64948 != 64948) {
		int becsjqr;
		for (becsjqr = 2; becsjqr > 0; becsjqr--) {
			continue;
		}
	}
	if (string("wtetdeufvksefjjiemglioklxjpjntamednhozp") != string("wtetdeufvksefjjiemglioklxjpjntamednhozp")) {
		int nhbrqhi;
		for (nhbrqhi = 77; nhbrqhi > 0; nhbrqhi--) {
			continue;
		}
	}
	return string("ja");
}

string fdieqit::scycrhicjtijdeuajbdykpf(bool dzbdpxbvpv, bool tgfeblgiyjs, int yzzddvwqeh, int kljcyxgwmpywhhf, bool lbkwrtwxhfb, int reinzsusrwtjp, string czomkpguzb, int toeufkyeucth, int icfijgt) {
	bool ftrdu = true;
	int royillisjt = 4742;
	int lxcgbpjuzg = 477;
	int ssafhlgwvajhprm = 4413;
	if (477 != 477) {
		int ml;
		for (ml = 83; ml > 0; ml--) {
			continue;
		}
	}
	if (true != true) {
		int uclxy;
		for (uclxy = 73; uclxy > 0; uclxy--) {
			continue;
		}
	}
	if (4413 != 4413) {
		int clgumvcg;
		for (clgumvcg = 47; clgumvcg > 0; clgumvcg--) {
			continue;
		}
	}
	return string("nmd");
}

string fdieqit::gcyqudmjcj() {
	int kyopw = 72;
	string pdyuz = "fqliuvkvqcawtatkybpgjcyjvowzjsfpmrhnhddabjvqfopnhwogtjmmmqflybiblfrpuo";
	bool cytng = true;
	int gcdxgbyyujpwg = 8100;
	int ttbhzxf = 3880;
	if (string("fqliuvkvqcawtatkybpgjcyjvowzjsfpmrhnhddabjvqfopnhwogtjmmmqflybiblfrpuo") == string("fqliuvkvqcawtatkybpgjcyjvowzjsfpmrhnhddabjvqfopnhwogtjmmmqflybiblfrpuo")) {
		int xti;
		for (xti = 59; xti > 0; xti--) {
			continue;
		}
	}
	if (8100 != 8100) {
		int nami;
		for (nami = 59; nami > 0; nami--) {
			continue;
		}
	}
	if (72 == 72) {
		int tjoqctlioy;
		for (tjoqctlioy = 0; tjoqctlioy > 0; tjoqctlioy--) {
			continue;
		}
	}
	if (3880 == 3880) {
		int fyrhwjup;
		for (fyrhwjup = 20; fyrhwjup > 0; fyrhwjup--) {
			continue;
		}
	}
	return string("ruewacr");
}

string fdieqit::nrrkwpsoszppzneihcmc(bool dxuyjptvszqql, string nkwkqxhocp, string fzuvvfvwot) {
	int yekifjnus = 947;
	string rhnib = "pkxnduvkstqghbfslyjblutkxpeihocaiidreednompgomchzltpvcvobwhhmcrlhiibtchphgwogjqu";
	int egdwtuxpouy = 1894;
	bool uqetfvmkbl = true;
	double joamuknvd = 4755;
	double fbqtcpegj = 59787;
	int njnocmnvvtbu = 5560;
	if (1894 == 1894) {
		int uphi;
		for (uphi = 56; uphi > 0; uphi--) {
			continue;
		}
	}
	if (947 != 947) {
		int emfmgf;
		for (emfmgf = 18; emfmgf > 0; emfmgf--) {
			continue;
		}
	}
	if (5560 != 5560) {
		int hh;
		for (hh = 92; hh > 0; hh--) {
			continue;
		}
	}
	return string("ylcwakudns");
}

double fdieqit::bhmatuiskgpaccyfmbjyam(double hcktswenmpck, double habnmy, bool vbudremlal, double twfvozo, bool yyfyqovyp, bool bpmrsmoo, int bhoookidd) {
	int ycnjfnhngi = 531;
	bool qurgy = false;
	int ocpainvjqhv = 7082;
	int kynoylrpa = 1882;
	double ebccslrujzhnnep = 17264;
	if (false == false) {
		int eveobyh;
		for (eveobyh = 99; eveobyh > 0; eveobyh--) {
			continue;
		}
	}
	if (7082 != 7082) {
		int tgqydy;
		for (tgqydy = 23; tgqydy > 0; tgqydy--) {
			continue;
		}
	}
	return 27313;
}

void fdieqit::ovvpsgllpvldfr(double nqoobgknnsgnacj, double iqcrifx, int ikeuw, double cauziwbbdi, int ggjbzqcsiqpi, int bisxsobiqsmsit) {
	bool spzvwq = false;
	string efepfsksjikcj = "cjnybeoghcoarmslfkwpqc";
	string muqocz = "zwmfvqdwbyjfcraqyayudfqh";
	bool mhbwdnemgpedg = true;
	int hnbfyvzxoihhx = 755;
	double ikisifimb = 16108;
	double ukohzpuc = 12717;
	string iqmesiblg = "dobfofpsotpahefcgrrgdxcxtmxasnponmmdpstvrekxewccmzvqmnofqvqdrbtdfwptnzxtmujmfudw";
	int qgfwjxmjvwu = 1643;
	double uxdsks = 1812;
	if (string("dobfofpsotpahefcgrrgdxcxtmxasnponmmdpstvrekxewccmzvqmnofqvqdrbtdfwptnzxtmujmfudw") == string("dobfofpsotpahefcgrrgdxcxtmxasnponmmdpstvrekxewccmzvqmnofqvqdrbtdfwptnzxtmujmfudw")) {
		int pzmqhywn;
		for (pzmqhywn = 7; pzmqhywn > 0; pzmqhywn--) {
			continue;
		}
	}
	if (string("cjnybeoghcoarmslfkwpqc") == string("cjnybeoghcoarmslfkwpqc")) {
		int rhmzpfl;
		for (rhmzpfl = 41; rhmzpfl > 0; rhmzpfl--) {
			continue;
		}
	}
	if (string("dobfofpsotpahefcgrrgdxcxtmxasnponmmdpstvrekxewccmzvqmnofqvqdrbtdfwptnzxtmujmfudw") != string("dobfofpsotpahefcgrrgdxcxtmxasnponmmdpstvrekxewccmzvqmnofqvqdrbtdfwptnzxtmujmfudw")) {
		int pdk;
		for (pdk = 25; pdk > 0; pdk--) {
			continue;
		}
	}

}

string fdieqit::vnllwekffgyncfmog(bool fcvlvbqezquya, bool tdvgy, int gedaxibdjh, string ftvfqpvhmro, double hnxbyqeqod, double pcyfqqmhd, int wnfedcfsbuuq, string cjnarfuiotg, int pwffuyhoogqdoz) {
	int hbbmqsooxobbier = 468;
	bool zrfjfushflf = true;
	if (true == true) {
		int qfvvxafb;
		for (qfvvxafb = 11; qfvvxafb > 0; qfvvxafb--) {
			continue;
		}
	}
	if (468 != 468) {
		int syoru;
		for (syoru = 50; syoru > 0; syoru--) {
			continue;
		}
	}
	if (468 != 468) {
		int hjr;
		for (hjr = 54; hjr > 0; hjr--) {
			continue;
		}
	}
	return string("bb");
}

void fdieqit::itojspvdkcqqofje(double ffpauhniytvkukl, string dlyleadl, double tvrqxzhqmjf, string wrfaywc, int iaylgubcdthtdo, double upucorrzrmfwiv, double ixpcbaiahzirq) {
	double iuyizxdysd = 44542;
	string lctqufhtu = "yiroexdtdaelzqnqtqghjgoppajouwsblehidjkdavxcljvyttknktdmeogzgybxpedbuwzlnrib";
	bool uudzcyjp = false;
	string roydrrcxijdzrzt = "efitjddjfonxzqakjjrrnxihjwtgyxoghexrjxdhlrlhsgszbnphgblngyomrjmwqsipifteziwaxjkfvnaujcdogeq";
	int rwsppzrxgqbtgrd = 603;
	double cccrxrzj = 24831;
	double rxdoow = 20893;
	int jpdyhurgwx = 3579;
	string editlltwmjt = "bsoshclcivmkqelwmplhzdpxwdaejtgorsyvfonctiduumvpcfsgyuprrmalnbsympx";
	string mpihcikyiazu = "uqvlpltoxoxfhneyiiiedjyiovlxdvrwjwrfgnmqiwywarcgtewclldxyzqvuiscbbomriqvikqmdeu";
	if (string("efitjddjfonxzqakjjrrnxihjwtgyxoghexrjxdhlrlhsgszbnphgblngyomrjmwqsipifteziwaxjkfvnaujcdogeq") == string("efitjddjfonxzqakjjrrnxihjwtgyxoghexrjxdhlrlhsgszbnphgblngyomrjmwqsipifteziwaxjkfvnaujcdogeq")) {
		int dmmqq;
		for (dmmqq = 97; dmmqq > 0; dmmqq--) {
			continue;
		}
	}
	if (20893 != 20893) {
		int nytzzntx;
		for (nytzzntx = 22; nytzzntx > 0; nytzzntx--) {
			continue;
		}
	}
	if (string("bsoshclcivmkqelwmplhzdpxwdaejtgorsyvfonctiduumvpcfsgyuprrmalnbsympx") != string("bsoshclcivmkqelwmplhzdpxwdaejtgorsyvfonctiduumvpcfsgyuprrmalnbsympx")) {
		int xqhpxt;
		for (xqhpxt = 3; xqhpxt > 0; xqhpxt--) {
			continue;
		}
	}

}

void fdieqit::keosxpnbkyfsoh(string jcvdfkypnl, bool bgyrf) {
	double ioptxzszzxknif = 23048;
	string bdoljzukudtyj = "dktmaafqvteppsytuiwrneeggzosrxszkekttthmynbtzrtz";
	if (23048 == 23048) {
		int urfacn;
		for (urfacn = 6; urfacn > 0; urfacn--) {
			continue;
		}
	}

}

fdieqit::fdieqit() {
	this->tcnuqbuzcchrwakixppgwkax(string("rbvcr"), true, 1373, 4643, string("bgbjrvybwpkchzasvkwdpmkyulnuccycbkayprksbhytfivevgjtstupsovbpuqiasopsukrsczwrhhjhemvjzmguvpbb"), true, 63229, true, true, string("uxhstqmvoqjfittprzilwjmgiqmgi"));
	this->pegznlnhfrjiwgwzcnd();
	this->scycrhicjtijdeuajbdykpf(false, true, 1569, 2963, false, 1670, string("okpphxybunmkpwmglyyptoqthfcwetrhhzicdwspiouhcuzuxmbburerhhqoaejkxblkemj"), 473, 7598);
	this->gcyqudmjcj();
	this->nrrkwpsoszppzneihcmc(false, string("gnybgormuoloobblnsgxpjwdbzkrepbhz"), string("ijiwdlqflrrsvcwwnfbsiamgmhxmrcjgoryfiyngyxtvayyflkmgwdspnzvv"));
	this->bhmatuiskgpaccyfmbjyam(57358, 1485, true, 49714, true, true, 700);
	this->ovvpsgllpvldfr(34313, 53691, 1828, 64102, 3256, 3170);
	this->vnllwekffgyncfmog(false, false, 6419, string("emdjqpwzrazjdcdcvqrhzjojcgxbrtupmtdgwnyrvqovqenmthttnoigqddxmsxsnqamnsvihf"), 37825, 9156, 870, string("dlnqwserjthpqldqptfgqabiniwhwqukdhs"), 3061);
	this->itojspvdkcqqofje(10634, string("teanrhwgjantilurplbjgbddnzqmgqnhvw"), 44691, string("yriwhicruprrvmttzijkelhcqqlw"), 409, 39337, 19157);
	this->keosxpnbkyfsoh(string("hkrw"), false);
	this->msymwurezftolpafghf(false, string("dvknxushessroc"), string("kgftetvtyrafvdmqvprcfgxkaddvzapjldgkwtzibzdpdlhwifprqwkandyshpdgszcfircrd"), 59300, string("hdfijxhrqlbqvuijtkebrxxwppycwmvwiuihkvstcccrdjxltrtqtivfynujmak"), 65544, 6702);
	this->egvagkvddocwbl(true, 69558, 60877, string("fuzidkcmwxwzwcfyeutlwmxioyvhwov"));
	this->sgvtgidqwqjwsiv(18445);
	this->ztckopjqicwhvkyt(8836, 3519, 153);
	this->okgpadyrjufkeb(string("egzsqjgxaivmsmzizdrbbprkqiozndsyyspoqoetpyjnkqtbeubawtwdvrstipnjks"));
	this->ltqcfyudvscka();
	this->aryjjzzldvpjuygczjmtgll();
	this->vqhzcssilkcxtishxgbn(string("dqifkvxixfrdisqgdpgfrclzvdbzqvkiuyrsddpveyljjgbslqwakffpntgukvosntxbavnpfodwnkaqbhivvvihkf"), string("rhhyxvubvyaxxpsbqqyfnyxkjibjxkbovuwpdzmsxgpeppcxttqcfwhiowdpzcwmwqsplklpdoaozml"), string("jdvaszmbhdvqmyhlmnbynnnrmmnzhalktjmibvbpzilltdkwqed"), string("ymidhwucwzoodobvwhbyltdsqidevgkxsytpzepkxpovpuxperayfpmsatogvlztegrgmwgyazbiwxtl"), 70240, 8334, true, 140);
	this->welqftnuzjbc(string("vijsgn"), string("jhfxygfpzynvsbnedrhbtliqcecb"), string("tpobyfbricnwed"), 39472, string("klqkhwbtfyoettrrplcdbztrrnigcaswgoeoplwnqbnzguv"));
	this->rmudubhqnetooisrs(false, 18167, false);
	this->eufumkwubtfmgtimf(string("ulkwwbkgmgyvbxvkdifgmedlxeqyyfquplxqfsnjgjwpfdjccglqjbvqddhuhfjfoa"), 75743, 5165, false, string("omnofatibowmtxhrjtoimdmxdijtbmwvvejrlkvpegvyarwftjczkzzvnrzysohtetxy"), 20314, false);
}
