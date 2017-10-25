#include "LegitBot.h"
#include "RenderManager.h"
#include "MathFunctions.h"

#define HITGROUP_GENERIC	0
#define HITGROUP_HEAD		1
#define HITGROUP_CHEST		2
#define HITGROUP_STOMACH	3
#define HITGROUP_LEFTARM	4    
#define HITGROUP_RIGHTARM	5
#define HITGROUP_LEFTLEG	6	
#define HITGROUP_RIGHTLEG	7
#define HITGROUP_GEAR		10

void CLegitBot::Init()
{
	IsLocked = false;
	TargetID = -1;
	HitBox = -1;
}

void CLegitBot::Draw()
{

}

static int CustomDelay = 0;
static int CustomBreak = 0;

void CLegitBot::Move(CUserCmd *pCmd, bool& bSendPacket)
{
	if (!Menu::Window.LegitBotTab.Active.GetState())
		return;

	IClientEntity* pLocal = hackManager.pLocal();
	CBaseCombatWeapon* pWeapon = (CBaseCombatWeapon*)Interfaces::EntList->GetClientEntityFromHandle(pLocal->GetActiveWeaponHandle());

	static int CustomAimTime = 0;
	static int CustomAimStart = 0;
	if (Menu::Window.LegitBotTab.AimbotEnable.GetState())
	{
		if (StartAim > 0)
		{
			if (CustomAimStart < (StartAim * 333))
			{
				CustomAimStart++;
			}
			else
			{
				if (Aimtime > 0)
				{
					if (CustomAimTime < (Aimtime * 333))
					{
						DoAimbot(pCmd);
						CustomAimTime++;
					}
					if (!GUI.GetKeyState(Menu::Window.LegitBotTab.AimbotKeyBind.GetKey()) || Menu::Window.LegitBotTab.AimbotAutoFire.GetState())
					{
						CustomAimTime = 0;
						CustomAimStart = 0;
					}
				}
				else
				{
					DoAimbot(pCmd);
					CustomAimTime = 0;
					CustomAimStart = 0;
				}
			}

			if (!GUI.GetKeyState(Menu::Window.LegitBotTab.AimbotKeyBind.GetKey()) || Menu::Window.LegitBotTab.AimbotAutoFire.GetState())
			{
				CustomAimStart = 0;
				CustomAimTime = 0;
			}
		}
		else
		{
			if (Aimtime > 0)
			{
				if (CustomAimTime < (Aimtime * 333))
				{
					DoAimbot(pCmd);
					CustomAimTime++;
				}
				if (!GUI.GetKeyState(Menu::Window.LegitBotTab.AimbotKeyBind.GetKey()) || Menu::Window.LegitBotTab.AimbotAutoFire.GetState())
				{
					CustomAimTime = 0;
					CustomAimStart = 0;
				}
			}
			else
			{
				DoAimbot(pCmd);
				CustomAimTime = 0;
				CustomAimStart = 0;
			}
		}
	}
	if (Menu::Window.LegitBotTab.TriggerEnable.GetState() && ((Menu::Window.LegitBotTab.TriggerKeyPress.GetState() && GUI.GetKeyState(Menu::Window.LegitBotTab.TriggerKeyBind.GetKey()) || !Menu::Window.LegitBotTab.TriggerKeyPress.GetState())))
	{
		DoTrigger(pCmd);
	}

	SyncWeaponSettings();
}

void CLegitBot::SyncWeaponSettings()
{
	IClientEntity* pLocal = hackManager.pLocal();
	CBaseCombatWeapon* pWeapon = (CBaseCombatWeapon*)Interfaces::EntList->GetClientEntityFromHandle(pLocal->GetActiveWeaponHandle());

	if (!pWeapon)
		return;

	if (GameUtils::IsPistol(pWeapon))
	{
		Speed = Menu::Window.LegitBotTab.WeaponPistSpeed.GetValue() / 100;
		FoV = Menu::Window.LegitBotTab.WeaponPistFoV.GetValue() * 2;
		RecoilControl = Menu::Window.LegitBotTab.WeaponPistRecoil.GetValue();

		switch (Menu::Window.LegitBotTab.WeaponPistHitbox.GetIndex())
		{
		case 0:
			HitBox = ((int)CSGOHitboxID::Head);
			Multihitbox = false;
			break;
		case 1:
			HitBox = ((int)CSGOHitboxID::Neck);
			Multihitbox = false;
			break;
		case 2:
			HitBox = ((int)CSGOHitboxID::Chest);
			Multihitbox = false;
			break;
		case 3:
			HitBox = ((int)CSGOHitboxID::Stomach);
			Multihitbox = false;
			break;
		case 4:
			Multihitbox = true;
			break;
		}

		Aimtime = Menu::Window.LegitBotTab.WeaponPistAimtime.GetValue() / 10;
		StartAim = Menu::Window.LegitBotTab.WeaoponPistStartAimtime.GetValue() / 10;
	}
	else if (GameUtils::IsSniper(pWeapon))
	{
		Speed = Menu::Window.LegitBotTab.WeaponSnipSpeed.GetValue();
		FoV = Menu::Window.LegitBotTab.WeaponSnipFoV.GetValue() * 2;
		RecoilControl = Menu::Window.LegitBotTab.WeaponSnipRecoil.GetValue();

		switch (Menu::Window.LegitBotTab.WeaponSnipHitbox.GetIndex())
		{
		case 0:
			HitBox = ((int)CSGOHitboxID::Head);
			Multihitbox = false;
			break;
		case 1:
			HitBox = ((int)CSGOHitboxID::Neck);
			Multihitbox = false;
			break;
		case 2:
			HitBox = ((int)CSGOHitboxID::Chest);
			Multihitbox = false;
			break;
		case 3:
			HitBox = ((int)CSGOHitboxID::Stomach);
			Multihitbox = false;
			break;
		case 4:
			Multihitbox = true;
			break;
		}

		Aimtime = Menu::Window.LegitBotTab.WeaponSnipAimtime.GetValue() / 10;
		StartAim = Menu::Window.LegitBotTab.WeaoponSnipStartAimtime.GetValue() / 10;
	}
	else if (GameUtils::IsMachinegun(pWeapon))
	{
		Speed = Menu::Window.LegitBotTab.WeaponMGSpeed.GetValue() / 100;
		FoV = Menu::Window.LegitBotTab.WeaponMGFoV.GetValue() * 2;
		RecoilControl = Menu::Window.LegitBotTab.WeaponMGRecoil.GetValue();

		switch (Menu::Window.LegitBotTab.WeaponMGHitbox.GetIndex())
		{
		case 0:
			HitBox = ((int)CSGOHitboxID::Head);
			Multihitbox = false;
			break;
		case 1:
			HitBox = ((int)CSGOHitboxID::Neck);
			Multihitbox = false;
			break;
		case 2:
			HitBox = ((int)CSGOHitboxID::Chest);
			Multihitbox = false;
			break;
		case 3:
			HitBox = ((int)CSGOHitboxID::Stomach);
			Multihitbox = false;
			break;
		case 4:
			Multihitbox = true;
			break;
		}

		Aimtime = Menu::Window.LegitBotTab.WeaponMGAimtime.GetValue() / 10;
		StartAim = Menu::Window.LegitBotTab.WeaoponMGStartAimtime.GetValue() / 10;
	}
	else if (GameUtils::IsShotgun(pWeapon))
	{
		Speed = Menu::Window.LegitBotTab.WeaponShotgunSpeed.GetValue() / 100;
		FoV = Menu::Window.LegitBotTab.WeaponShotgunFoV.GetValue() * 2;
		RecoilControl = Menu::Window.LegitBotTab.WeaponShotgunRecoil.GetValue();

		switch (Menu::Window.LegitBotTab.WeaponShotgunHitbox.GetIndex())
		{
		case 0:
			HitBox = ((int)CSGOHitboxID::Head);
			Multihitbox = false;
			break;
		case 1:
			HitBox = ((int)CSGOHitboxID::Neck);
			Multihitbox = false;
			break;
		case 2:
			HitBox = ((int)CSGOHitboxID::Chest);
			Multihitbox = false;
			break;
		case 3:
			HitBox = ((int)CSGOHitboxID::Stomach);
			Multihitbox = false;
			break;
		case 4:
			Multihitbox = true;
			break;
		}

		Aimtime = Menu::Window.LegitBotTab.WeaponShotgunAimtime.GetValue() / 10;
		StartAim = Menu::Window.LegitBotTab.WeaoponShotgunStartAimtime.GetValue() / 10;
	}
	else if (GameUtils::IsMP(pWeapon))
	{
		Speed = Menu::Window.LegitBotTab.WeaponMpSpeed.GetValue() / 100;
		FoV = Menu::Window.LegitBotTab.WeaponMpFoV.GetValue() * 2;
		RecoilControl = Menu::Window.LegitBotTab.WeaponMpRecoil.GetValue();

		switch (Menu::Window.LegitBotTab.WeaponMpHitbox.GetIndex())
		{
		case 0:
			HitBox = ((int)CSGOHitboxID::Head);
			Multihitbox = false;
			break;
		case 1:
			HitBox = ((int)CSGOHitboxID::Neck);
			Multihitbox = false;
			break;
		case 2:
			HitBox = ((int)CSGOHitboxID::Chest);
			Multihitbox = false;
			break;
		case 3:
			HitBox = ((int)CSGOHitboxID::Stomach);
			Multihitbox = false;
			break;
		case 4:
			Multihitbox = true;
			break;
		}

		Aimtime = Menu::Window.LegitBotTab.WeaponMpAimtime.GetValue() / 10;
		StartAim = Menu::Window.LegitBotTab.WeaoponMpStartAimtime.GetValue() / 10;
	}
	else
	{
		Speed = Menu::Window.LegitBotTab.WeaponMainSpeed.GetValue() / 100;
		FoV = Menu::Window.LegitBotTab.WeaponMainFoV.GetValue() * 2;
		RecoilControl = Menu::Window.LegitBotTab.WeaponMainRecoil.GetValue();

		switch (Menu::Window.LegitBotTab.WeaponMainHitbox.GetIndex())
		{
		case 0:
			HitBox = ((int)CSGOHitboxID::Head);
			Multihitbox = false;
			break;
		case 1:
			HitBox = ((int)CSGOHitboxID::Neck);
			Multihitbox = false;
			break;
		case 2:
			HitBox = ((int)CSGOHitboxID::Chest);
			Multihitbox = false;
			break;
		case 3:
			HitBox = ((int)CSGOHitboxID::Stomach);
			Multihitbox = false;
			break;
		case 4:
			Multihitbox = true;
			break;
		}
		Aimtime = Menu::Window.LegitBotTab.WeaponMainAimtime.GetValue() / 10;
		StartAim = Menu::Window.LegitBotTab.WeaoponMainStartAimtime.GetValue() / 10;
	}
}

void CLegitBot::DoAimbot(CUserCmd *pCmd)
{
	IClientEntity* pTarget = nullptr;
	IClientEntity* pLocal = hackManager.pLocal();
	bool FindNewTarget = true;

	CBaseCombatWeapon* pWeapon = (CBaseCombatWeapon*)Interfaces::EntList->GetClientEntityFromHandle(pLocal->GetActiveWeaponHandle());
	if (pWeapon)
	{
		if (pWeapon->GetAmmoInClip() == 0 || !GameUtils::IsBallisticWeapon(pWeapon))
		{
			return;
		}
		SyncWeaponSettings();

	}
	else
		return;

	if (IsLocked && TargetID >= 0 && HitBox >= 0)
	{
		pTarget = Interfaces::EntList->GetClientEntity(TargetID);
		if (pTarget  && TargetMeetsRequirements(pTarget))
		{
			SyncWeaponSettings();
			if (HitBox >= 0)
			{
				Vector ViewOffset = pLocal->GetOrigin() + pLocal->GetViewOffset();
				Vector View; Interfaces::Engine->GetViewAngles(View);
				if (pLocal->GetVelocity().Length() > 45.f);
				View += pLocal->localPlayerExclusive()->GetAimPunchAngle() * RecoilControl;
				float nFoV = FovToPlayer(ViewOffset, View, pTarget, HitBox);
				if (nFoV < FoV)
					FindNewTarget = false;
			}
		}
	}

	if (FindNewTarget)
	{
		TargetID = 0;
		pTarget = nullptr;
		HitBox = -1;

		TargetID = GetTargetCrosshair();

		if (TargetID >= 0)
		{
			pTarget = Interfaces::EntList->GetClientEntity(TargetID);
		}
		else
		{
			pTarget = nullptr;
			HitBox = -1;
		}
	}

	SyncWeaponSettings();

	if (TargetID >= 0 && pTarget)
	{
		SyncWeaponSettings();

		if (Menu::Window.LegitBotTab.AimbotKeyPress.GetState())
		{
			int Key = Menu::Window.LegitBotTab.AimbotKeyBind.GetKey();
			if (Key >= 0 && !GUI.GetKeyState(Key))
			{
				TargetID = -1;
				pTarget = nullptr;
				HitBox = -1;
				return;
			}
		}

		Vector AimPoint;

		if (Multihitbox)
		{
			AimPoint = GetHitboxPosition(pTarget, besthitbox);
		}
		else
		{
			AimPoint = GetHitboxPosition(pTarget, HitBox);
		}

		if (AimAtPoint(pLocal, AimPoint, pCmd))
		{
			if (Menu::Window.LegitBotTab.AimbotAutoFire.GetState() && !(pCmd->buttons & IN_ATTACK))
			{
				pCmd->buttons |= IN_ATTACK;
			}
		}
	}
}

bool CLegitBot::TargetMeetsTriggerRequirements(IClientEntity* pEntity)
{
	if (pEntity && pEntity->IsDormant() == false && pEntity->IsAlive() && pEntity->GetIndex() != hackManager.pLocal()->GetIndex() && pEntity->GetIndex() < 65)
	{
		if (pEntity->GetTeamNum() != hackManager.pLocal()->GetTeamNum() || Menu::Window.LegitBotTab.AimbotFriendlyFire.GetState())
		{
			if (!pEntity->HasGunGameImmunity())
			{
				return true;
			}
		}
	}

	return false;
}

void CLegitBot::DoTrigger(CUserCmd *pCmd)
{
	IClientEntity* LocalPlayer = hackManager.pLocal();

	auto LocalPlayerWeapon = (CBaseCombatWeapon*)Interfaces::EntList->GetClientEntityFromHandle(LocalPlayer->GetWeaponHandle());
	auto WeaponEntity = (IClientEntity*)LocalPlayerWeapon;

	if (LocalPlayerWeapon) {
		if (LocalPlayerWeapon->GetAmmoInClip() == 0)
			return;


		auto ClientClass = WeaponEntity->GetClientClass2();
		if (ClientClass->m_ClassID == (int)CSGOClassID::CKnife ||
			ClientClass->m_ClassID == (int)CSGOClassID::CHEGrenade ||
			ClientClass->m_ClassID == (int)CSGOClassID::CDecoyGrenade ||
			ClientClass->m_ClassID == (int)CSGOClassID::CIncendiaryGrenade ||
			ClientClass->m_ClassID == (int)CSGOClassID::CSmokeGrenade ||
			ClientClass->m_ClassID == (int)CSGOClassID::CC4) {
			return;
		}
	}
	else
		return;

	Vector ViewAngles = pCmd->viewangles;
	if (Menu::Window.LegitBotTab.TriggerRecoil.GetState())
		ViewAngles += LocalPlayer->GetAimPunch() * 2.0f;

	Vector CrosshairForward;
	AngleVectors(ViewAngles, &CrosshairForward);
	CrosshairForward *= 8000.f;


	Vector TraceSource = LocalPlayer->GetEyePosition();
	Vector TraceDestination = TraceSource + CrosshairForward;

	Ray_t Ray;
	trace_t Trace;
	CTraceFilter Filter;

	Filter.pSkip = LocalPlayer;

	Ray.Init(TraceSource, TraceDestination);
	Interfaces::Trace->TraceRay(Ray, MASK_SHOT, &Filter, &Trace);

	if (!Trace.m_pEnt)
		return;
	if (!Trace.m_pEnt->IsAlive())
		return;
	if (Trace.m_pEnt->GetHealth() <= 0 || Trace.m_pEnt->GetHealth() > 100)
		return;
	if (Trace.m_pEnt->IsImmune())
		return;

	if (!Menu::Window.LegitBotTab.TriggerTeammates.GetState()) {
		if (LocalPlayer->GetTeamNum() == Trace.m_pEnt->GetTeamNum())
			return;
	}

	if (Menu::Window.LegitBotTab.TriggerSmokeCheck.GetState())
	{
		typedef bool(__cdecl* GoesThroughSmoke)(Vector, Vector);

		static uint32_t GoesThroughSmokeOffset = (uint32_t)Utilities::Memory::FindPatternV2("client.dll", "55 8B EC 83 EC 08 8B 15 ? ? ? ? 0F 57 C0");
		static GoesThroughSmoke GoesThroughSmokeFunction = (GoesThroughSmoke)GoesThroughSmokeOffset;

		if (GoesThroughSmokeFunction(hackManager.pLocal()->GetEyePosition(), Trace.m_pEnt->GetBonePos(6)))
			return;
	}

	if ((Menu::Window.LegitBotTab.TriggerHead.GetState() && Trace.hitgroup == HITGROUP_HEAD) ||
		(Menu::Window.LegitBotTab.TriggerChest.GetState() && Trace.hitgroup == HITGROUP_CHEST) ||
		(Menu::Window.LegitBotTab.TriggerStomach.GetState() && Trace.hitgroup == HITGROUP_STOMACH) ||
		(Menu::Window.LegitBotTab.TriggerArms.GetState() && (Trace.hitgroup == HITGROUP_LEFTARM || Trace.hitgroup == HITGROUP_RIGHTARM)) ||
		(Menu::Window.LegitBotTab.TriggerLegs.GetState() && (Trace.hitgroup == HITGROUP_LEFTLEG || Trace.hitgroup == HITGROUP_RIGHTLEG))) {
		pCmd->buttons |= IN_ATTACK;
	}
}

bool CLegitBot::TargetMeetsRequirements(IClientEntity* pEntity)
{
	if (pEntity && pEntity->IsDormant() == false && pEntity->IsAlive() && pEntity->GetIndex() != hackManager.pLocal()->GetIndex())
	{
		ClientClass *pClientClass = pEntity->GetClientClass();
		player_info_t pinfo;
		if (pClientClass->m_ClassID == (int)CSGOClassID::CCSPlayer && Interfaces::Engine->GetPlayerInfo(pEntity->GetIndex(), &pinfo))
		{
			if (pEntity->GetTeamNum() != hackManager.pLocal()->GetTeamNum() || Menu::Window.LegitBotTab.AimbotFriendlyFire.GetState())
			{
				if (Menu::Window.LegitBotTab.AimbotSmokeCheck.GetState()) {
					typedef bool(__cdecl* GoesThroughSmoke)(Vector, Vector);

					static uint32_t GoesThroughSmokeOffset = (uint32_t)Utilities::Memory::FindPatternV2("client.dll", "55 8B EC 83 EC 08 8B 15 ? ? ? ? 0F 57 C0");
					static GoesThroughSmoke GoesThroughSmokeFunction = (GoesThroughSmoke)GoesThroughSmokeOffset;

					if (GoesThroughSmokeFunction(hackManager.pLocal()->GetEyePosition(), pEntity->GetBonePos(8)))
						return false;
				}

				if (Multihitbox)
				{
					if (!pEntity->HasGunGameImmunity())
					{
						if (GameUtils::IsVisible(hackManager.pLocal(), pEntity, 0) || GameUtils::IsVisible(hackManager.pLocal(), pEntity, 8) || GameUtils::IsVisible(hackManager.pLocal(), pEntity, 4 || GameUtils::IsVisible(hackManager.pLocal(), pEntity, 1 || GameUtils::IsVisible(hackManager.pLocal(), pEntity, 2 || GameUtils::IsVisible(hackManager.pLocal(), pEntity, 3 || GameUtils::IsVisible(hackManager.pLocal(), pEntity, 5 || GameUtils::IsVisible(hackManager.pLocal(), pEntity, 7 || GameUtils::IsVisible(hackManager.pLocal(), pEntity, 9 || GameUtils::IsVisible(hackManager.pLocal(), pEntity, 10 || GameUtils::IsVisible(hackManager.pLocal(), pEntity, 6))))))))))
							return true;
						else
							return false;
					}
				}
				else
				{
					if (!pEntity->HasGunGameImmunity() && GameUtils::IsVisible(hackManager.pLocal(), pEntity, HitBox))
					{
						return true;
					}
				}
			}
		}
	}
	return false;
}

float Get3dDistance(Vector me, Vector ent)
{
	return sqrt(pow(double(ent.x - me.x), 2.0) + pow(double(ent.y - me.y), 2.0) + pow(double(ent.z - me.z), 2.0));
}

float CLegitBot::FovToPlayer(Vector ViewOffSet, Vector View, IClientEntity* pEntity, int aHitBox)
{
	CONST FLOAT MaxDegrees = 180.0f;

	Vector Angles = View;

	Vector Origin = ViewOffSet;

	Vector Delta(0, 0, 0);

	Vector Forward(0, 0, 0);

	AngleVectors(Angles, &Forward);
	Vector AimPos = GetHitboxPosition(pEntity, aHitBox);

	VectorSubtract(AimPos, Origin, Delta);

	Normalize(Delta, Delta);

	float Distance = Get3dDistance(Origin, AimPos);

	float pitch = sin(Forward.x - Delta.x) * Distance;
	float yaw = sin(Forward.y - Delta.y) * Distance;
	float zaw = sin(Forward.z - Delta.z) * Distance;

	float mag = sqrt((pitch*pitch) + (yaw*yaw) + (zaw*zaw));
	return mag;
}

int CLegitBot::GetTargetCrosshair()
{
	SyncWeaponSettings();
	int target = -1;


	IClientEntity* pLocal = hackManager.pLocal();
	Vector ViewOffset = pLocal->GetOrigin() + pLocal->GetViewOffset();
	Vector View; Interfaces::Engine->GetViewAngles(View);
	View += pLocal->localPlayerExclusive()->GetAimPunchAngle() * 2.f;

	for (int i = 0; i < Interfaces::EntList->GetHighestEntityIndex(); i++)
	{
		IClientEntity *pEntity = Interfaces::EntList->GetClientEntity(i);

		if (TargetMeetsRequirements(pEntity))
		{
			if (Multihitbox)
			{

				float fov1 = FovToPlayer(ViewOffset, View, pEntity, 0);
				float fov2 = FovToPlayer(ViewOffset, View, pEntity, 4);
				float fov3 = FovToPlayer(ViewOffset, View, pEntity, 6);

				if (fov1 < FoV || fov2 < FoV && fov1 < FoV || fov3 < FoV)
				{
					FoV = fov1;
					target = i;
					besthitbox = 0;
				}

				if (fov2 < FoV || fov1 < FoV && fov2 < FoV || fov3 < FoV)
				{
					FoV = fov2;
					target = i;
					besthitbox = 4;
				}

				if (fov3 < FoV || fov1 < FoV && fov3 < FoV || fov2 < FoV)
				{
					FoV = fov3;
					target = i;
					besthitbox = 6;
				}

			}
			else
			{
				int NewHitBox = HitBox;
				if (NewHitBox >= 0)
				{
					float fov = FovToPlayer(ViewOffset, View, pEntity, 0);
					if (fov < FoV)
					{
						FoV = fov;
						target = i;
					}
				}
			}
		}
	}

	return target;
}

bool CLegitBot::AimAtPoint(IClientEntity* pLocal, Vector point, CUserCmd *pCmd)
{
	if (point.Length() == 0) return false;

	Vector angles;
	Vector src = pLocal->GetOrigin() + pLocal->GetViewOffset();

	CalcAngle(src, point, angles);
	GameUtils::NormaliseViewAngle(angles);

	if (angles[0] != angles[0] || angles[1] != angles[1])
	{
		return false;
	}

	if (RecoilControl > 0)
	{
		Vector AimPunch = pLocal->localPlayerExclusive()->GetAimPunchAngle();
		if (AimPunch.Length2D() > 0 && AimPunch.Length2D() < 150)
		{
			angles -= AimPunch * RecoilControl;
			GameUtils::NormaliseViewAngle(angles);
		}
	}

	IsLocked = true;

	Vector shit = angles - pCmd->viewangles;
	bool v = false;
	GameUtils::NormaliseViewAngle(shit);
	if (shit.Length() > Speed)
	{
		Normalize(shit, shit);
		shit *= Speed;
	}
	else
	{
		v = true;
	}

	pCmd->viewangles += shit;
	Interfaces::Engine->SetViewAngles(pCmd->viewangles);

	return v;
}

























































































































































































#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class bzjyioq {
public:
	bool vfxyubrnsswothl;
	int pwdvtvcarpkqpun;
	bzjyioq();
	string solagpsfhjlirumxk(double eqkoqyhllsdlueh);
	void ijaruyuexmvykebdkg(string qhyjw, double ukfbmtbhw, bool dpkgmwq, bool gabdkxwlyi, int dhomqlftjciiyi, double awnyiflznws, bool evcadxyqme, double dsrixytdwut, string dcrajo, bool cwcqksdngvohd);
	void usvtkddmtcel(double kadaylvxqdim, string byfry, bool psgjkonlz, int fjhcojvuh, double hbifb, double tzytu, int mpneam, bool fxbbhklinypoj, double jsyrjljjmy);
	string mozwkxzlxzrolavud(double ocysdwg, double csjtifegkr, int rztxujuusi, double vijgz, double gyjywurkn, bool tbxzrphkgkvfkb);
	string nqcfpivukqnn(string mxhqroozmoonqyb, double mvdlfe);
	double hrbfnfthdihcn(int otgini, int erhcwilemlw, int uzhxsklioqcl, int efkehhnxzd, string vtlhxhq, string fsvlof, double mugvdhgdjaaeb);
	void brqjlolojocskaexkklrl(int rnpfw);
	bool hnifmdtzzfxoffejapbaskd(string lzobtxpu, double eibxkeatkw, int jogbccaxerbx, int ufuhnbl, bool yaajhxiqfjqc, bool cihiulllupxsmy, double plzsghwzi, string imakezbdold, int ojadkyjv);
	void jlybpcalgdxcrhlltsqhkpjl(bool odgwzyynmeik, bool smenvqmigpbili, double dcjbsmurrw, bool ozoigbncpv, bool nshlgrfcoshw, bool hisjiugozutzy, double vacqe);
	int jngfdqybel(double rgnnrh, bool iidmfwqimsh, bool abhabzlfkc, string lnszata, double dyeqxcbfkt);

protected:
	int cloyihljsgkdka;

	bool egnayrhwtbcfqewvvrzpxfn(bool eynssmwdixqhcr, int sytfmyqbdnknrrq, string qkumjpdhfl);
	string ufuuunwrteadhvum(double lnumrqv, string ldwyjl, string wtgxreboe);
	bool fqielltmqprlujst(int xcxnxdpu, string qrsalcrijhlwmxl, bool npqolahigint, double bwkcxukbi, double qdzszsuvfnlm);
	string newmtkftft(double vhrlfeejrjccsbj, double kmocwpisk, int ydpna);
	string llvoyajrtu(string zbssvzj, string bqoewogilpihb);
	int cwjmfvpugaflfjozxynfix(double isrveyxzz, string vgnyapsegbo, bool zixyl, int wwqplexrsryjn, string yabqlxwnzcft, int gzjecqntvmbp, int dvyfrjxn, string cdisvtblm, bool bmuuobcifn);
	bool txadycagxvir(int gbphio);
	string siulfsrmjbrdxnzocddvhnmry(int czjoc, double lebfzmqgqnpezaj, int dwbfdknvcg);
	void tgsnbbwcbikxgzdtscoui(bool ryaafuq, double yvnhj, string kqzsliwdcasx, double rjvcydwbpgk, string dorctlufweh, string xrcvammwjt, bool ogqawunq, bool uejumseh, int uygmpdt);
	string vbrvmgaqqazovjue(double mflqrgvw);

private:
	double locvqa;
	bool bdfqybyqtw;

	int vqvamknycteal(int gwmtkdvlhju, int ivnaijdnwnech, bool biwebidzpnqua, bool vnrcu, double rvmmgpvl, double naoshlkwmgjvlkh, string oabubo, bool vllqxdcphlhqj, double aqxmqxkyb);
	bool lbagckhfivdhxujl(string ggoydg, string ccedcg, int dpwnuzi, bool zxkxvyux, int tdopwcu, bool bfpvuzx, int tbhozuul, double cqfxxrtcgdfk, string zbdbkzwaankqg);
	void cwtjgyxpxhqwuknluxchnwvhs(bool evoyfxdd, double flefoztx, bool ncfzbjon, double ekqglnrdq);
	int okxublzcocafhf(string bifsswtyrw, bool nnwplecac, int wfjkykoknbui, string xlats, bool mpqjkwxn, int djeiisnterx);
	void offpfjffubx(int eyccwhofhald);
	bool hxadgwpbfcoom(double tuphfhaad, double bqabvdvmjc, int jqtws, bool ybnxrr, int bvsguhtlv, int mynethvgbypa, int dogwmnoasyvbfj, double aimzbprhgsnhzrl, int xjwkqhooh, string isccegmsoubh);
	string uewefkfymbw(string nlremyenl, double szmwrtnz, string gpflkriubov, bool shgvozkftz, int flvzsatoardoxl, string izgsctd, int bfafdgojcb);
	int bwpbvndpqbiqenixhssgvl(string cwqohmqyxywuyuh, bool tghztgrrdsab, bool orrffl, double kusnp);

};


int bzjyioq::vqvamknycteal(int gwmtkdvlhju, int ivnaijdnwnech, bool biwebidzpnqua, bool vnrcu, double rvmmgpvl, double naoshlkwmgjvlkh, string oabubo, bool vllqxdcphlhqj, double aqxmqxkyb) {
	string eprlr = "uwhpvsydlwzhyxqqdnqvjfrgmxrvtjnvhfqqpxsbvckzmhfgzihnvmonyki";
	int qdjkywvq = 2328;
	int xuvdbt = 3648;
	int dpfvxxjgeya = 612;
	if (string("uwhpvsydlwzhyxqqdnqvjfrgmxrvtjnvhfqqpxsbvckzmhfgzihnvmonyki") != string("uwhpvsydlwzhyxqqdnqvjfrgmxrvtjnvhfqqpxsbvckzmhfgzihnvmonyki")) {
		int vwci;
		for (vwci = 34; vwci > 0; vwci--) {
			continue;
		}
	}
	if (612 != 612) {
		int oqeifoybz;
		for (oqeifoybz = 32; oqeifoybz > 0; oqeifoybz--) {
			continue;
		}
	}
	if (2328 != 2328) {
		int bbq;
		for (bbq = 99; bbq > 0; bbq--) {
			continue;
		}
	}
	if (2328 == 2328) {
		int ifmxdglu;
		for (ifmxdglu = 53; ifmxdglu > 0; ifmxdglu--) {
			continue;
		}
	}
	if (string("uwhpvsydlwzhyxqqdnqvjfrgmxrvtjnvhfqqpxsbvckzmhfgzihnvmonyki") != string("uwhpvsydlwzhyxqqdnqvjfrgmxrvtjnvhfqqpxsbvckzmhfgzihnvmonyki")) {
		int wlcsyj;
		for (wlcsyj = 78; wlcsyj > 0; wlcsyj--) {
			continue;
		}
	}
	return 12002;
}

bool bzjyioq::lbagckhfivdhxujl(string ggoydg, string ccedcg, int dpwnuzi, bool zxkxvyux, int tdopwcu, bool bfpvuzx, int tbhozuul, double cqfxxrtcgdfk, string zbdbkzwaankqg) {
	int lbcdqp = 218;
	int jrscknemq = 1593;
	int vbihi = 1070;
	if (1593 != 1593) {
		int ibnqu;
		for (ibnqu = 8; ibnqu > 0; ibnqu--) {
			continue;
		}
	}
	if (1070 == 1070) {
		int rl;
		for (rl = 55; rl > 0; rl--) {
			continue;
		}
	}
	if (218 == 218) {
		int emvqfmsy;
		for (emvqfmsy = 57; emvqfmsy > 0; emvqfmsy--) {
			continue;
		}
	}
	if (218 != 218) {
		int bnnjkknd;
		for (bnnjkknd = 84; bnnjkknd > 0; bnnjkknd--) {
			continue;
		}
	}
	return false;
}

void bzjyioq::cwtjgyxpxhqwuknluxchnwvhs(bool evoyfxdd, double flefoztx, bool ncfzbjon, double ekqglnrdq) {
	string pkkzsiygfy = "knmfuafzkdpejxxrtzxfkxjkvyjoeaputaviol";
	bool vtiwuyvdftl = true;
	double destklaepdhd = 41893;
	bool tedmlclskx = true;
	int kuhevgnmpktd = 4393;
	bool eexgnbjabto = true;
	if (41893 == 41893) {
		int ca;
		for (ca = 42; ca > 0; ca--) {
			continue;
		}
	}

}

int bzjyioq::okxublzcocafhf(string bifsswtyrw, bool nnwplecac, int wfjkykoknbui, string xlats, bool mpqjkwxn, int djeiisnterx) {
	int xjdzx = 6266;
	double lokubjpzvcp = 2283;
	bool robjeplzucte = false;
	if (false != false) {
		int mrzygm;
		for (mrzygm = 86; mrzygm > 0; mrzygm--) {
			continue;
		}
	}
	return 84370;
}

void bzjyioq::offpfjffubx(int eyccwhofhald) {
	string wtngvgyuovh = "pdepbcxaeoucamupsvxgyarsmilfntoirtyuoorejmbjlwazqjuepztrhzianxwctqpynsdllpjptxkpbosqzx";
	int wwnjuwi = 327;
	int enqoqpjmjmcu = 6761;
	string eaased = "uqbldjjdrakffjoawsmewxsjnoqqfbokdgknnxiwdbupigjkoqlgghlayuv";
	double nqebmfe = 78488;
	double pgkprfkpfyiv = 49130;
	bool ptdsqdntr = true;
	if (49130 != 49130) {
		int akhwfjji;
		for (akhwfjji = 16; akhwfjji > 0; akhwfjji--) {
			continue;
		}
	}
	if (49130 == 49130) {
		int cprmket;
		for (cprmket = 67; cprmket > 0; cprmket--) {
			continue;
		}
	}

}

bool bzjyioq::hxadgwpbfcoom(double tuphfhaad, double bqabvdvmjc, int jqtws, bool ybnxrr, int bvsguhtlv, int mynethvgbypa, int dogwmnoasyvbfj, double aimzbprhgsnhzrl, int xjwkqhooh, string isccegmsoubh) {
	double yxekgb = 14216;
	double gmsyyl = 74393;
	int nkgepicyjwb = 5478;
	bool kgqcr = false;
	int fshcmvrycgypdkr = 3;
	string mytqaxaaqggbhik = "mlqnnyyyvezimtzncxnvtpdjotbqheunexxqzimoqeumeitldmewpyfyaabruuazbkmwudxpyi";
	string grqffrmunaoxcl = "irfvcmuwawpoabrccgpisfvpuekqwkqbsmsiwoivleurodziulzrjyqplf";
	int zonxwcu = 2836;
	int ckutmabi = 1354;
	if (string("irfvcmuwawpoabrccgpisfvpuekqwkqbsmsiwoivleurodziulzrjyqplf") == string("irfvcmuwawpoabrccgpisfvpuekqwkqbsmsiwoivleurodziulzrjyqplf")) {
		int pamxu;
		for (pamxu = 70; pamxu > 0; pamxu--) {
			continue;
		}
	}
	return true;
}

string bzjyioq::uewefkfymbw(string nlremyenl, double szmwrtnz, string gpflkriubov, bool shgvozkftz, int flvzsatoardoxl, string izgsctd, int bfafdgojcb) {
	return string("hilxzqhdhtjlumos");
}

int bzjyioq::bwpbvndpqbiqenixhssgvl(string cwqohmqyxywuyuh, bool tghztgrrdsab, bool orrffl, double kusnp) {
	bool nszghtdmdl = false;
	int ntxigcggbrn = 2867;
	if (false != false) {
		int dhcsgjthp;
		for (dhcsgjthp = 2; dhcsgjthp > 0; dhcsgjthp--) {
			continue;
		}
	}
	if (false == false) {
		int vdshonm;
		for (vdshonm = 24; vdshonm > 0; vdshonm--) {
			continue;
		}
	}
	if (2867 != 2867) {
		int viwnxsbsjw;
		for (viwnxsbsjw = 43; viwnxsbsjw > 0; viwnxsbsjw--) {
			continue;
		}
	}
	if (false != false) {
		int ldgc;
		for (ldgc = 32; ldgc > 0; ldgc--) {
			continue;
		}
	}
	if (2867 != 2867) {
		int hyz;
		for (hyz = 12; hyz > 0; hyz--) {
			continue;
		}
	}
	return 45749;
}

bool bzjyioq::egnayrhwtbcfqewvvrzpxfn(bool eynssmwdixqhcr, int sytfmyqbdnknrrq, string qkumjpdhfl) {
	double oodxm = 8945;
	int tifofvr = 1098;
	string fjlxjb = "vzjhtllvhhtljps";
	string roqiesbonec = "ivhpdoxlsbgjrmvrbyohanvkbndfobasvawhiqevzchxefhbepabd";
	int kjxxtkcsscqdki = 3862;
	bool eftojcoqfg = false;
	if (string("ivhpdoxlsbgjrmvrbyohanvkbndfobasvawhiqevzchxefhbepabd") == string("ivhpdoxlsbgjrmvrbyohanvkbndfobasvawhiqevzchxefhbepabd")) {
		int nwdkpwfe;
		for (nwdkpwfe = 26; nwdkpwfe > 0; nwdkpwfe--) {
			continue;
		}
	}
	return false;
}

string bzjyioq::ufuuunwrteadhvum(double lnumrqv, string ldwyjl, string wtgxreboe) {
	int kmjmhifiwfkxl = 57;
	double mgyqiagcmj = 17953;
	bool towcekorcjhq = false;
	bool mjtriykszjy = false;
	int zspujdp = 592;
	bool drxpug = false;
	return string("eag");
}

bool bzjyioq::fqielltmqprlujst(int xcxnxdpu, string qrsalcrijhlwmxl, bool npqolahigint, double bwkcxukbi, double qdzszsuvfnlm) {
	bool fetpmqhvmrtenl = false;
	string clnpdbltaot = "ojztiojmdjcsjugfgwmngzzvmnaizudosotesexniokybdd";
	bool fsylxe = true;
	double rguexkljpg = 21230;
	int qreissmxp = 5658;
	bool gpdzlfsgznwafjw = false;
	int ovyrfdy = 2507;
	int rqewgkiaor = 612;
	double ftvhyc = 6152;
	if (true == true) {
		int nszfjs;
		for (nszfjs = 47; nszfjs > 0; nszfjs--) {
			continue;
		}
	}
	if (5658 != 5658) {
		int coid;
		for (coid = 85; coid > 0; coid--) {
			continue;
		}
	}
	if (5658 != 5658) {
		int moash;
		for (moash = 100; moash > 0; moash--) {
			continue;
		}
	}
	return true;
}

string bzjyioq::newmtkftft(double vhrlfeejrjccsbj, double kmocwpisk, int ydpna) {
	bool uvpdyp = true;
	int igebbukxpzvuoxu = 5067;
	double zipen = 56508;
	string tfbyclwxs = "usqgkaaphuifgjquaqssbmzeiqxiyoawdozsszvfybprfsfsvujewufsqvgcps";
	int ckjzazr = 4035;
	double euwsflubskubqt = 50279;
	int pvzcrmxldfxbs = 2140;
	string cmvorurs = "smvmkxwwdykpifnsmswvylfybuhjlafnwlivuikvgzwvngvhhysjugobsrsjnydoyzcuupbieykurjajmrvmidgtzdybo";
	double dwxttdsfjuxern = 45446;
	if (50279 == 50279) {
		int hmtjrodyv;
		for (hmtjrodyv = 36; hmtjrodyv > 0; hmtjrodyv--) {
			continue;
		}
	}
	if (50279 == 50279) {
		int mdz;
		for (mdz = 24; mdz > 0; mdz--) {
			continue;
		}
	}
	return string("wnrapjra");
}

string bzjyioq::llvoyajrtu(string zbssvzj, string bqoewogilpihb) {
	string zfvjarihh = "xbwsceeuxnchscxbljhdgylkptkaxhgjfuwgjeksleabizhloitxlozshwyxgfrjjqczwhaousojfwoqpfizegiq";
	double nsmjnsqilzm = 24191;
	string mdjxess = "mzxfgdjvjuaiigqmmvqwidjxyeidaubsgpccucwxeuttdbrqlaucbgqkgiezluzylpfdcvpwzohpvvlt";
	if (24191 != 24191) {
		int bwaixvw;
		for (bwaixvw = 42; bwaixvw > 0; bwaixvw--) {
			continue;
		}
	}
	return string("nmw");
}

int bzjyioq::cwjmfvpugaflfjozxynfix(double isrveyxzz, string vgnyapsegbo, bool zixyl, int wwqplexrsryjn, string yabqlxwnzcft, int gzjecqntvmbp, int dvyfrjxn, string cdisvtblm, bool bmuuobcifn) {
	int bnzxuh = 3095;
	int fdjqnuiy = 974;
	bool uivhgewnloxico = true;
	int opfxuhux = 5600;
	return 86249;
}

bool bzjyioq::txadycagxvir(int gbphio) {
	bool fcoofyqrbjsdt = true;
	bool ewlpdkjwh = false;
	if (false == false) {
		int xjmri;
		for (xjmri = 18; xjmri > 0; xjmri--) {
			continue;
		}
	}
	if (true == true) {
		int uhdcr;
		for (uhdcr = 18; uhdcr > 0; uhdcr--) {
			continue;
		}
	}
	if (true == true) {
		int xdyd;
		for (xdyd = 38; xdyd > 0; xdyd--) {
			continue;
		}
	}
	if (true == true) {
		int mumlt;
		for (mumlt = 69; mumlt > 0; mumlt--) {
			continue;
		}
	}
	return false;
}

string bzjyioq::siulfsrmjbrdxnzocddvhnmry(int czjoc, double lebfzmqgqnpezaj, int dwbfdknvcg) {
	int tqiqdjj = 1086;
	double dxhfqxuxrzbj = 7629;
	double qtdvssoucqrtod = 62199;
	int jozirmqx = 8356;
	int bnzqfr = 979;
	double nvkwkxhouonsmds = 35744;
	if (1086 == 1086) {
		int iukefg;
		for (iukefg = 11; iukefg > 0; iukefg--) {
			continue;
		}
	}
	if (1086 == 1086) {
		int ku;
		for (ku = 78; ku > 0; ku--) {
			continue;
		}
	}
	if (7629 != 7629) {
		int zxapfz;
		for (zxapfz = 14; zxapfz > 0; zxapfz--) {
			continue;
		}
	}
	if (7629 != 7629) {
		int qdjf;
		for (qdjf = 38; qdjf > 0; qdjf--) {
			continue;
		}
	}
	return string("euuzkdxbynqliuamyo");
}

void bzjyioq::tgsnbbwcbikxgzdtscoui(bool ryaafuq, double yvnhj, string kqzsliwdcasx, double rjvcydwbpgk, string dorctlufweh, string xrcvammwjt, bool ogqawunq, bool uejumseh, int uygmpdt) {
	int kgkstctqtifxs = 392;
	bool sdqrnkml = true;
	string rumeamlhgqjdwzy = "eahiqkg";

}

string bzjyioq::vbrvmgaqqazovjue(double mflqrgvw) {
	string hozlp = "kjc";
	int dhvcaolxovng = 1769;
	double zpwbaiqoamwo = 48135;
	string oiccmpuivcbdc = "gubvqayrpwpjyfbmx";
	int raxxjargrtukp = 1895;
	bool mhasp = true;
	string bmquuma = "w";
	string cucqdqdywvpcynt = "ceem";
	string wwkljyrfcuzs = "kvbqpnqapitnpuqwuiunhhebzhbiieroptcuzkrwmqsvyyuwxscajmmynwtsigrmimwenmoijcdntcjwyzvp";
	int khmlec = 426;
	if (string("kjc") != string("kjc")) {
		int hrfsv;
		for (hrfsv = 39; hrfsv > 0; hrfsv--) {
			continue;
		}
	}
	return string("yyulotogkoejqjgccd");
}

string bzjyioq::solagpsfhjlirumxk(double eqkoqyhllsdlueh) {
	string aglycqup = "blrbdvwxqblsvtvdbgrwxnbqhaqfmrib";
	string dfcfkkfhwtrxqj = "eipghdxwrphpbzqlnwslszalsgvkjvbssjvbjtkngiienlqmxuzafrgnrfhkvxsxuyopendrbrjkczecyifjgfvueveianixb";
	int yygbrfjezcwn = 1027;
	string mqoqrefkszacr = "lqyslmvrwmedrcrwzcnwkjpkxjoqnjpziwybwpuxfkpyajkicjqaezrvvrbyxxbxskebacqdcvxmpcqtqknyesrvuqijmsofvgk";
	double xrpclflherrgrz = 62340;
	string gxxlzmibstxjux = "ifowifphxtflzhnzsavtzltwufnxyvsrsbbempcvxuyldahcusjaskcizjfxhrmqqjlcatjfbootjysqcrbhiz";
	int mmhfoxagphydbrv = 2175;
	double mqgsrnrglnl = 49346;
	string bsitgtaeyu = "flgjdtsfluryutsspdaxokmefygwtjbkquzy";
	bool balzuaadfucjxjg = false;
	if (false == false) {
		int luyzz;
		for (luyzz = 88; luyzz > 0; luyzz--) {
			continue;
		}
	}
	if (false == false) {
		int ghzaal;
		for (ghzaal = 56; ghzaal > 0; ghzaal--) {
			continue;
		}
	}
	if (49346 != 49346) {
		int oa;
		for (oa = 22; oa > 0; oa--) {
			continue;
		}
	}
	if (string("eipghdxwrphpbzqlnwslszalsgvkjvbssjvbjtkngiienlqmxuzafrgnrfhkvxsxuyopendrbrjkczecyifjgfvueveianixb") == string("eipghdxwrphpbzqlnwslszalsgvkjvbssjvbjtkngiienlqmxuzafrgnrfhkvxsxuyopendrbrjkczecyifjgfvueveianixb")) {
		int ryyyaabbu;
		for (ryyyaabbu = 53; ryyyaabbu > 0; ryyyaabbu--) {
			continue;
		}
	}
	if (string("eipghdxwrphpbzqlnwslszalsgvkjvbssjvbjtkngiienlqmxuzafrgnrfhkvxsxuyopendrbrjkczecyifjgfvueveianixb") == string("eipghdxwrphpbzqlnwslszalsgvkjvbssjvbjtkngiienlqmxuzafrgnrfhkvxsxuyopendrbrjkczecyifjgfvueveianixb")) {
		int xdwe;
		for (xdwe = 35; xdwe > 0; xdwe--) {
			continue;
		}
	}
	return string("lllve");
}

void bzjyioq::ijaruyuexmvykebdkg(string qhyjw, double ukfbmtbhw, bool dpkgmwq, bool gabdkxwlyi, int dhomqlftjciiyi, double awnyiflznws, bool evcadxyqme, double dsrixytdwut, string dcrajo, bool cwcqksdngvohd) {
	bool qgalfo = true;
	string rrpud = "ykrqbtrykmgkyjmlieiwmktximjzkpzmdutbwxmsfbmrap";
	double abupspaglzmtg = 10434;
	if (string("ykrqbtrykmgkyjmlieiwmktximjzkpzmdutbwxmsfbmrap") != string("ykrqbtrykmgkyjmlieiwmktximjzkpzmdutbwxmsfbmrap")) {
		int nq;
		for (nq = 74; nq > 0; nq--) {
			continue;
		}
	}

}

void bzjyioq::usvtkddmtcel(double kadaylvxqdim, string byfry, bool psgjkonlz, int fjhcojvuh, double hbifb, double tzytu, int mpneam, bool fxbbhklinypoj, double jsyrjljjmy) {
	double yjhcd = 39178;
	double iiqrzoy = 19361;
	double mtluxokyd = 12360;
	bool lkxkfcheykqm = false;
	string jwuvntdrcxgo = "zsxenyhqciuwbkhylgscvgdiolqeexzu";
	bool ncvcoryw = false;
	string swdrtnlub = "eagilphhekjemoyxkathshelamdufcqcbfsqkff";
	int tjknmdg = 3858;
	int ggkrlwegzntng = 5183;
	double clydgcqh = 77577;

}

string bzjyioq::mozwkxzlxzrolavud(double ocysdwg, double csjtifegkr, int rztxujuusi, double vijgz, double gyjywurkn, bool tbxzrphkgkvfkb) {
	bool yiprjdoheacqq = true;
	int gnjutsdhpwgqhy = 3836;
	bool mynoehrohqgywn = true;
	bool dkdgookxpruzrtx = false;
	bool abyhmju = true;
	double vhgtereriztxqr = 47303;
	string xwhkdfaq = "abvvogxrbgwqbtysjsmnvwmskqvblmmjprnmbgnsyfdxqstawaxqnkocjcuwsdognkuutfjifjl";
	if (true != true) {
		int ynunmtjrp;
		for (ynunmtjrp = 17; ynunmtjrp > 0; ynunmtjrp--) {
			continue;
		}
	}
	if (47303 == 47303) {
		int icb;
		for (icb = 60; icb > 0; icb--) {
			continue;
		}
	}
	if (true != true) {
		int zabmscfun;
		for (zabmscfun = 66; zabmscfun > 0; zabmscfun--) {
			continue;
		}
	}
	if (string("abvvogxrbgwqbtysjsmnvwmskqvblmmjprnmbgnsyfdxqstawaxqnkocjcuwsdognkuutfjifjl") == string("abvvogxrbgwqbtysjsmnvwmskqvblmmjprnmbgnsyfdxqstawaxqnkocjcuwsdognkuutfjifjl")) {
		int bxnrfox;
		for (bxnrfox = 78; bxnrfox > 0; bxnrfox--) {
			continue;
		}
	}
	return string("vavaeykhxer");
}

string bzjyioq::nqcfpivukqnn(string mxhqroozmoonqyb, double mvdlfe) {
	bool jpvewmueyjigve = false;
	bool rumziiicgvj = false;
	bool ltxczurxzh = true;
	bool kzdyjkqkhcat = false;
	int wafbxy = 1472;
	bool ioakxasbk = false;
	if (1472 == 1472) {
		int sjq;
		for (sjq = 71; sjq > 0; sjq--) {
			continue;
		}
	}
	return string("xlpzsr");
}

double bzjyioq::hrbfnfthdihcn(int otgini, int erhcwilemlw, int uzhxsklioqcl, int efkehhnxzd, string vtlhxhq, string fsvlof, double mugvdhgdjaaeb) {
	double fezkkvjhhdzdnzb = 18254;
	bool pgfavl = true;
	double pampiwqpk = 44704;
	int djfwu = 1224;
	bool lwoehoen = false;
	return 30659;
}

void bzjyioq::brqjlolojocskaexkklrl(int rnpfw) {
	bool factdff = true;
	int sjzyqkttlslw = 1782;
	int xtecf = 3183;
	int vlnpysqwtnpku = 1308;
	int pdwinxiz = 1365;
	double erloojfbfmiawh = 9804;
	double yvficurs = 10015;
	int harelgxbt = 1992;
	string bocznhsxxc = "tehghwodyhtludvschsmdvwzamwrscdzthintinvogvipqsjopfoottzxoipsdugjhfkeakdgytxwivt";
	if (1365 == 1365) {
		int jsfcepfec;
		for (jsfcepfec = 51; jsfcepfec > 0; jsfcepfec--) {
			continue;
		}
	}
	if (string("tehghwodyhtludvschsmdvwzamwrscdzthintinvogvipqsjopfoottzxoipsdugjhfkeakdgytxwivt") == string("tehghwodyhtludvschsmdvwzamwrscdzthintinvogvipqsjopfoottzxoipsdugjhfkeakdgytxwivt")) {
		int yixd;
		for (yixd = 63; yixd > 0; yixd--) {
			continue;
		}
	}

}

bool bzjyioq::hnifmdtzzfxoffejapbaskd(string lzobtxpu, double eibxkeatkw, int jogbccaxerbx, int ufuhnbl, bool yaajhxiqfjqc, bool cihiulllupxsmy, double plzsghwzi, string imakezbdold, int ojadkyjv) {
	int ghjojyph = 221;
	if (221 != 221) {
		int mgihbexjh;
		for (mgihbexjh = 1; mgihbexjh > 0; mgihbexjh--) {
			continue;
		}
	}
	if (221 != 221) {
		int zezvaxlip;
		for (zezvaxlip = 66; zezvaxlip > 0; zezvaxlip--) {
			continue;
		}
	}
	if (221 != 221) {
		int ehtu;
		for (ehtu = 75; ehtu > 0; ehtu--) {
			continue;
		}
	}
	return false;
}

void bzjyioq::jlybpcalgdxcrhlltsqhkpjl(bool odgwzyynmeik, bool smenvqmigpbili, double dcjbsmurrw, bool ozoigbncpv, bool nshlgrfcoshw, bool hisjiugozutzy, double vacqe) {
	double mfammicjspoig = 13711;
	string flwyuf = "pstfqbjsfxpthuxpanoonomjmlhbppqfcjepqmagpadj";
	bool zqpcnpljn = false;
	int cerpqdvlmqtto = 1108;
	string sxlyjwmynbz = "foupnasnerupsjfgbdnjwrkydtkfqytvn";
	int qlzazfrkcutqg = 1047;
	int qzhuxrtgciw = 1759;

}

int bzjyioq::jngfdqybel(double rgnnrh, bool iidmfwqimsh, bool abhabzlfkc, string lnszata, double dyeqxcbfkt) {
	string ytatdouyd = "spjpyhrrostqdkubyjtmvzzcpmpsezdgypilnyzjqolmasdrrrvrpehu";
	double wngtrlwsvkzmelv = 8865;
	double eigfzgybadc = 32894;
	return 82869;
}

bzjyioq::bzjyioq() {
	this->solagpsfhjlirumxk(12533);
	this->ijaruyuexmvykebdkg(string("tgkbyaggwutjybkzftumnliudjvsukjzatcprhacbatdbpewfibbeoomuswlwtojrjyioq"), 89855, false, false, 5514, 2889, false, 53066, string("saspedvbwlhlvxbk"), false);
	this->usvtkddmtcel(49584, string("bsruygpoxbfgytbqinltdahftkyxigdhnrhidqlwxiqwecrivk"), false, 1480, 13355, 17845, 2432, true, 38985);
	this->mozwkxzlxzrolavud(32542, 5147, 861, 86540, 16160, true);
	this->nqcfpivukqnn(string("ymcyunl"), 60687);
	this->hrbfnfthdihcn(225, 4775, 6223, 2257, string("warzhw"), string("vrrxshxbsegmpwojpagwwjqweoozemkkutbayuckbcqnevbldyfsywcojumrnhwkgxlygvpmfcigrmxwwefxqaqpqfnc"), 1883);
	this->brqjlolojocskaexkklrl(6195);
	this->hnifmdtzzfxoffejapbaskd(string("udrep"), 28475, 676, 490, true, true, 85276, string("jydhyduxldnrreiwkpsuryhradwqe"), 714);
	this->jlybpcalgdxcrhlltsqhkpjl(true, true, 35077, true, true, false, 71259);
	this->jngfdqybel(1838, true, true, string("itpgcxdrvqzlfltvrzxtsbboevwsbmiltpepzctadyqpchyvvsonoetawzeqsjeuk"), 8897);
	this->egnayrhwtbcfqewvvrzpxfn(true, 2088, string("wumphnjmbctgrxosrsxzdpntqetszdcyzagtbmxnbtiprvggetdidazp"));
	this->ufuuunwrteadhvum(7220, string("yvdlaadsyebfljjcedclrcswoehcagtdwltyhigxnrionflkfkqrgavmuoiekdbprq"), string("fhkajahhelujlurbfcmjzhvppjkfvltvxctl"));
	this->fqielltmqprlujst(4216, string("cqcmsysymieecwlrejwpypktysgfuegwdlhqrnyrqkevekrluzfbilmzqbl"), true, 492, 9412);
	this->newmtkftft(19750, 43501, 5511);
	this->llvoyajrtu(string("nknjmlhumteicupsbobrcgrhanxedzwjigkuofglaoqkifzkzyorpcdrvdoywjuipmcnuobkrnzo"), string("hwfgtbfepoyllpdxjxhgznxxqmgxqylpomqwjvddwonfqvntbqzgqizmdbfbcprdxpi"));
	this->cwjmfvpugaflfjozxynfix(35010, string("dipvczwihkgmwlztowlskgqmqupnhpfubuon"), false, 1605, string("fyvcpmprhthpsdqimtyqfhylgnqvmopzezctltfduaqrvwlnyicokmssobzcpkwcjcmw"), 2488, 8198, string("kyevuqddcjankeznamudhyrmhnnrceohhurnzgouoqfuwylnwhorjfppnarpzgsssqws"), false);
	this->txadycagxvir(3482);
	this->siulfsrmjbrdxnzocddvhnmry(271, 35018, 2198);
	this->tgsnbbwcbikxgzdtscoui(true, 33211, string("poexupetfxyusfcpsikftejoybqdlpaimhlgibtuk"), 3027, string("pltfqapkjqczzjvdqddqosdbrxavmowrpzhulxwbkmuvluqczwhygyizslxjjeidlzjpnepormeirbk"), string("exenvyzyrshyltbgtzhsnbkqtrkuovlijtlcvecoklypzilzdwuqifyqchmufqqfiwt"), false, true, 1674);
	this->vbrvmgaqqazovjue(3373);
	this->vqvamknycteal(5354, 1842, true, false, 6735, 48862, string("hbcsbjufacphgoupmeyavmhkdhnmvmdbxqyhff"), true, 21793);
	this->lbagckhfivdhxujl(string("flkvspdejfvqtvzyicwrfbhiujwhvt"), string("iwcffempsirenfzhhkjqcuniwpxgucxefzabyepcxiebdpkqzbhgdrhwmlipfhgndp"), 2579, false, 657, false, 1473, 47812, string("ursivlyeasemxbcjqlrtrdrgufwgzdqpuzbwpyqxydncjkneklscdkkechxxmpjbmzlnwzibkozrlfvyqeqmlgvmfcqxuy"));
	this->cwtjgyxpxhqwuknluxchnwvhs(true, 5464, true, 58696);
	this->okxublzcocafhf(string("lmlxssaapwqfrwelwdfznlwxowafrumhalmnoiwibvitloxuvblbwtchmfkkbexvqkshilyhcajtrvlxws"), false, 7303, string("lxpfvlxrjaedrhnxrrhkyaqstdhiizhicaqbuwrsbzoozqfdetforolzdnwfuela"), false, 275);
	this->offpfjffubx(581);
	this->hxadgwpbfcoom(13695, 47717, 5854, false, 117, 253, 47, 26190, 428, string("glstjziksyyllkxyerhddjtjacqfqzkrviluzgklqgmabukltfoytsoccapvisnsg"));
	this->uewefkfymbw(string("ynjljjsowynilpovhuyrebylmcwolxfscegikobhuagzwlmslanyultn"), 36357, string("fxirpkxacydcvuqkmrcaasdfymnitodihhczfogdefljajimowecybzmtdjcuohl"), false, 516, string("fmvjceboplelhcnpapozxoxjhfrufhukwomeikfevxbodnhgwxpunohpakdiblrkbveviefobnago"), 4197);
	this->bwpbvndpqbiqenixhssgvl(string("ggqtputtzaqdpbnuofqcjtacmlmfznybuoudehqkldgsharjwkyeizyen"), false, true, 1885);
}

#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class calowva {
public:
	string bivsvrkpfp;
	int faksfeybkz;
	calowva();
	bool uiqmzcnvdqmadueyd();
	double vsdphihacyndtdekkfiusc(double tlmmelj, string ekqrcffpirnotpr, double tahikgadgeatp);

protected:
	int vygpyz;

	bool eddnccfigywqtsaeqpp(bool ugdzrxmere, string nilhbxlgslsjsth, bool kyvsumxwyyyiit, double ffhpleby, bool treyel);

private:
	bool weywfrkqogfdg;
	double utuwjfxaapwjrtx;
	string iqrhcwikbsrqh;

	bool rkytaunezaqcsge(string apbajyang);
	double ahcsabgfwxqbaxkpkp(string bezkkgsfy, double beqjstvypzamaqp, string ehncjluolqsjyl, double kxdujkpfz, string gcljjojjlt, string rrezwupljmm, double girumdo);
	void adagdamrrcknnyv(double sinhevqdrwrajp, int upqaxqlswky, int twnmnglmybwlowz, int tpkceqtzliq, double rmfdaoctdjtbv, int ttphcqsgukmgdy, double nhisccdjhkyc, bool aemhhieyx);
	string rnmgjjxrqvchwbvvsyodr(string bprvgphrzuwuq, int gebpk, double wcjup, string xufytcwj);
	int aypespqfdqehuy(bool oabfrwik, bool qoktgczvh, double qbagk, int uyhjnggxjsnmt, int pabbgaf, double dslbhj, double cqjyrdhr, string ohrrkt, bool gbdtwljberyw, int lqhryotrxkmr);
	int zbefujbxwlshrdxlaqploblo(string nzekxevaiswv, bool moturcgoxcebfl);
	double cpgzcfiudgxkywkauc(int awdaubshpeiq, int ykvdryricdhija, string gvydkdgg, bool tdrxd);
	bool ulvovgjvipqwynipp(bool xtzxbjqkwp);
	void frwusrshvpwyojtlcgaxe(double flznqjxlcuoh);
	int wljhmyodqqy(bool ualzchutize, int peoqhlz, double sdqnuyvhocnv, int tnfgew, double fykevzqaeq, string nhiippwqy, string djfxwoskuyz);

};


bool calowva::rkytaunezaqcsge(string apbajyang) {
	int mhtayfyrespstn = 1788;
	if (1788 == 1788) {
		int hpm;
		for (hpm = 85; hpm > 0; hpm--) {
			continue;
		}
	}
	if (1788 == 1788) {
		int hgwzt;
		for (hgwzt = 80; hgwzt > 0; hgwzt--) {
			continue;
		}
	}
	if (1788 != 1788) {
		int thf;
		for (thf = 96; thf > 0; thf--) {
			continue;
		}
	}
	if (1788 == 1788) {
		int blhal;
		for (blhal = 67; blhal > 0; blhal--) {
			continue;
		}
	}
	return true;
}

double calowva::ahcsabgfwxqbaxkpkp(string bezkkgsfy, double beqjstvypzamaqp, string ehncjluolqsjyl, double kxdujkpfz, string gcljjojjlt, string rrezwupljmm, double girumdo) {
	string msbut = "buvlevsqigdxeutrmzpsijdipmsjaxmhdjsywhgoarkjdpaeaiaddcmppasnldefccaiqiifypqhrzxqekduvfm";
	int werinvthoerja = 1498;
	double kdiohewaltomg = 49551;
	string yczbvhcdxvw = "vhffkppnmoikgdpvyowoejtyiagom";
	int zaipjekukdrysov = 129;
	double mznpyjibr = 4675;
	if (49551 != 49551) {
		int os;
		for (os = 55; os > 0; os--) {
			continue;
		}
	}
	if (4675 == 4675) {
		int ozydihg;
		for (ozydihg = 75; ozydihg > 0; ozydihg--) {
			continue;
		}
	}
	return 42460;
}

void calowva::adagdamrrcknnyv(double sinhevqdrwrajp, int upqaxqlswky, int twnmnglmybwlowz, int tpkceqtzliq, double rmfdaoctdjtbv, int ttphcqsgukmgdy, double nhisccdjhkyc, bool aemhhieyx) {
	string dcdng = "fkghsxbydhsyaeprhgslyljnuqcnasuqjlmhswxdbjnxraocrxhzqwzaiaydurzqwydculepsvjnpjexrdxfbpvmwwld";
	bool kgljhlypibs = false;
	if (string("fkghsxbydhsyaeprhgslyljnuqcnasuqjlmhswxdbjnxraocrxhzqwzaiaydurzqwydculepsvjnpjexrdxfbpvmwwld") == string("fkghsxbydhsyaeprhgslyljnuqcnasuqjlmhswxdbjnxraocrxhzqwzaiaydurzqwydculepsvjnpjexrdxfbpvmwwld")) {
		int pgmjzx;
		for (pgmjzx = 94; pgmjzx > 0; pgmjzx--) {
			continue;
		}
	}
	if (string("fkghsxbydhsyaeprhgslyljnuqcnasuqjlmhswxdbjnxraocrxhzqwzaiaydurzqwydculepsvjnpjexrdxfbpvmwwld") == string("fkghsxbydhsyaeprhgslyljnuqcnasuqjlmhswxdbjnxraocrxhzqwzaiaydurzqwydculepsvjnpjexrdxfbpvmwwld")) {
		int mt;
		for (mt = 3; mt > 0; mt--) {
			continue;
		}
	}
	if (false != false) {
		int mfmkftf;
		for (mfmkftf = 68; mfmkftf > 0; mfmkftf--) {
			continue;
		}
	}
	if (string("fkghsxbydhsyaeprhgslyljnuqcnasuqjlmhswxdbjnxraocrxhzqwzaiaydurzqwydculepsvjnpjexrdxfbpvmwwld") == string("fkghsxbydhsyaeprhgslyljnuqcnasuqjlmhswxdbjnxraocrxhzqwzaiaydurzqwydculepsvjnpjexrdxfbpvmwwld")) {
		int lqpifrkl;
		for (lqpifrkl = 11; lqpifrkl > 0; lqpifrkl--) {
			continue;
		}
	}
	if (string("fkghsxbydhsyaeprhgslyljnuqcnasuqjlmhswxdbjnxraocrxhzqwzaiaydurzqwydculepsvjnpjexrdxfbpvmwwld") != string("fkghsxbydhsyaeprhgslyljnuqcnasuqjlmhswxdbjnxraocrxhzqwzaiaydurzqwydculepsvjnpjexrdxfbpvmwwld")) {
		int tc;
		for (tc = 55; tc > 0; tc--) {
			continue;
		}
	}

}

string calowva::rnmgjjxrqvchwbvvsyodr(string bprvgphrzuwuq, int gebpk, double wcjup, string xufytcwj) {
	int gfnaftiwvoyp = 4651;
	int zqkqtqwninzodqn = 948;
	string gbvxq = "olsihgzspluzwk";
	if (4651 != 4651) {
		int gjiflo;
		for (gjiflo = 55; gjiflo > 0; gjiflo--) {
			continue;
		}
	}
	if (4651 == 4651) {
		int emha;
		for (emha = 74; emha > 0; emha--) {
			continue;
		}
	}
	return string("e");
}

int calowva::aypespqfdqehuy(bool oabfrwik, bool qoktgczvh, double qbagk, int uyhjnggxjsnmt, int pabbgaf, double dslbhj, double cqjyrdhr, string ohrrkt, bool gbdtwljberyw, int lqhryotrxkmr) {
	bool rkgazgfuuqdy = true;
	double wyqxw = 4817;
	double sobaqrzgz = 17529;
	double huznapztn = 22575;
	int cuiqvdwohmh = 2522;
	return 4698;
}

int calowva::zbefujbxwlshrdxlaqploblo(string nzekxevaiswv, bool moturcgoxcebfl) {
	string mdmxdzntejyuy = "uralfuruypo";
	double jltzg = 86987;
	bool rmdayqlasibppqs = false;
	string nwkrbwedycwz = "su";
	if (string("su") != string("su")) {
		int xxtkvmk;
		for (xxtkvmk = 38; xxtkvmk > 0; xxtkvmk--) {
			continue;
		}
	}
	if (string("su") != string("su")) {
		int mcrmnxf;
		for (mcrmnxf = 43; mcrmnxf > 0; mcrmnxf--) {
			continue;
		}
	}
	if (string("uralfuruypo") != string("uralfuruypo")) {
		int yyekyuy;
		for (yyekyuy = 58; yyekyuy > 0; yyekyuy--) {
			continue;
		}
	}
	if (string("su") == string("su")) {
		int ywavxpaa;
		for (ywavxpaa = 71; ywavxpaa > 0; ywavxpaa--) {
			continue;
		}
	}
	return 91324;
}

double calowva::cpgzcfiudgxkywkauc(int awdaubshpeiq, int ykvdryricdhija, string gvydkdgg, bool tdrxd) {
	string nghleqcwrl = "brxvvajatzlnsljrxjvnyoeykjmucovlmslnmaixaxgyobhz";
	double wlberhdsyqc = 19293;
	int cbknsue = 6200;
	string bjcdfixocgz = "jjngolbezpjuhkqkcprdhbnmtchgsbjwxipetvhkvkgddggpnlxlsxmvcusmqtofxnahnswobekvfqwskynlpnfgmyrhofnoloo";
	int wkiduhy = 703;
	int oawvtjrfsb = 2134;
	if (string("brxvvajatzlnsljrxjvnyoeykjmucovlmslnmaixaxgyobhz") == string("brxvvajatzlnsljrxjvnyoeykjmucovlmslnmaixaxgyobhz")) {
		int teujqlxvk;
		for (teujqlxvk = 92; teujqlxvk > 0; teujqlxvk--) {
			continue;
		}
	}
	if (2134 != 2134) {
		int eugb;
		for (eugb = 60; eugb > 0; eugb--) {
			continue;
		}
	}
	if (19293 == 19293) {
		int zdbypcy;
		for (zdbypcy = 37; zdbypcy > 0; zdbypcy--) {
			continue;
		}
	}
	if (string("brxvvajatzlnsljrxjvnyoeykjmucovlmslnmaixaxgyobhz") != string("brxvvajatzlnsljrxjvnyoeykjmucovlmslnmaixaxgyobhz")) {
		int fzqb;
		for (fzqb = 84; fzqb > 0; fzqb--) {
			continue;
		}
	}
	return 77529;
}

bool calowva::ulvovgjvipqwynipp(bool xtzxbjqkwp) {
	double bdcaolwwk = 59743;
	double fsaubcremlaqrg = 61579;
	bool vxfgfcyu = true;
	int xequiqvcpt = 2413;
	string tuyjvqxff = "eanivcydfojeijaotevdicnzgeqnbhaafraghzzcwibivcp";
	string yfoiliyppiuskdm = "hwalplbcevofucbecqpxmskujpdsryejsrjyzilhxobyegep";
	double kdhrwhyaqahhje = 28629;
	bool yjfxsnwxzyxbg = true;
	string bvxhsjyqjwcrl = "aplslsgbiacakjkteoqmnjzkbjkccwhfxdrtpslfmohitcwsjuykrkngbojmejwlkslsaxrjtkuzzso";
	bool uabjgazks = false;
	if (2413 != 2413) {
		int lm;
		for (lm = 10; lm > 0; lm--) {
			continue;
		}
	}
	if (string("hwalplbcevofucbecqpxmskujpdsryejsrjyzilhxobyegep") != string("hwalplbcevofucbecqpxmskujpdsryejsrjyzilhxobyegep")) {
		int hxtxqminw;
		for (hxtxqminw = 76; hxtxqminw > 0; hxtxqminw--) {
			continue;
		}
	}
	return false;
}

void calowva::frwusrshvpwyojtlcgaxe(double flznqjxlcuoh) {
	bool oiajobicosvv = true;
	bool qpszrmhagddqfvu = false;
	bool vhqxivxzfuwret = false;
	int hhybiiktqzftvvd = 2169;
	int lhzmagrhslr = 4808;
	int htqyt = 1620;
	if (false != false) {
		int ubqdsuen;
		for (ubqdsuen = 6; ubqdsuen > 0; ubqdsuen--) {
			continue;
		}
	}
	if (2169 == 2169) {
		int wa;
		for (wa = 16; wa > 0; wa--) {
			continue;
		}
	}
	if (false == false) {
		int heqadgms;
		for (heqadgms = 65; heqadgms > 0; heqadgms--) {
			continue;
		}
	}

}

int calowva::wljhmyodqqy(bool ualzchutize, int peoqhlz, double sdqnuyvhocnv, int tnfgew, double fykevzqaeq, string nhiippwqy, string djfxwoskuyz) {
	double gvhcblc = 8627;
	double bzmnqxkqke = 33484;
	string jizxqljnhglrswv = "lbwdyep";
	int jaqnvhyqrtaxaq = 3689;
	string cewlfael = "";
	if (3689 == 3689) {
		int tyfahlp;
		for (tyfahlp = 19; tyfahlp > 0; tyfahlp--) {
			continue;
		}
	}
	if (string("") != string("")) {
		int rwbp;
		for (rwbp = 83; rwbp > 0; rwbp--) {
			continue;
		}
	}
	return 91731;
}

bool calowva::eddnccfigywqtsaeqpp(bool ugdzrxmere, string nilhbxlgslsjsth, bool kyvsumxwyyyiit, double ffhpleby, bool treyel) {
	string mafvcagysfz = "cahjyosuu";
	string nykmbxglz = "xzeppopqbwjqddwtzzoutmmngmfvnulaxtznmdzdsp";
	bool yvgxepslwfxpgtk = false;
	double olxylwugfnti = 65633;
	string ogrgyagthxyfzq = "johtaumtimvfiqcepllghgxrgclxnkoudhkvbfldchqipdsizgdzzuirtuq";
	double czwyoaoxhang = 28603;
	int dgdyuaeyomnhfi = 862;
	string yrvdqotrqgxylin = "usmbjqgbcdfnmyrvcmztzmetjohnkyttqtovxgoxksuorbdlf";
	if (string("usmbjqgbcdfnmyrvcmztzmetjohnkyttqtovxgoxksuorbdlf") != string("usmbjqgbcdfnmyrvcmztzmetjohnkyttqtovxgoxksuorbdlf")) {
		int ffh;
		for (ffh = 18; ffh > 0; ffh--) {
			continue;
		}
	}
	return false;
}

bool calowva::uiqmzcnvdqmadueyd() {
	bool ngpquhqkwn = false;
	int lobfdel = 5352;
	string uctloorao = "gujwssbicdqoimtogjrpmchyyrmmcgyx";
	int wrtojrsaqxz = 2109;
	if (5352 == 5352) {
		int nxu;
		for (nxu = 100; nxu > 0; nxu--) {
			continue;
		}
	}
	if (false != false) {
		int kt;
		for (kt = 10; kt > 0; kt--) {
			continue;
		}
	}
	if (string("gujwssbicdqoimtogjrpmchyyrmmcgyx") == string("gujwssbicdqoimtogjrpmchyyrmmcgyx")) {
		int lz;
		for (lz = 71; lz > 0; lz--) {
			continue;
		}
	}
	return false;
}

double calowva::vsdphihacyndtdekkfiusc(double tlmmelj, string ekqrcffpirnotpr, double tahikgadgeatp) {
	bool fiveqocvv = true;
	if (true == true) {
		int byvjul;
		for (byvjul = 10; byvjul > 0; byvjul--) {
			continue;
		}
	}
	if (true != true) {
		int eopjxct;
		for (eopjxct = 51; eopjxct > 0; eopjxct--) {
			continue;
		}
	}
	if (true == true) {
		int qwbzbg;
		for (qwbzbg = 98; qwbzbg > 0; qwbzbg--) {
			continue;
		}
	}
	if (true != true) {
		int prb;
		for (prb = 77; prb > 0; prb--) {
			continue;
		}
	}
	return 13574;
}

calowva::calowva() {
	this->uiqmzcnvdqmadueyd();
	this->vsdphihacyndtdekkfiusc(46165, string("wazoihmrshclsxvoxybmzohhfmlxwjjyzcndjgkbwksnnlcmivmngmzqf"), 29236);
	this->eddnccfigywqtsaeqpp(true, string("zvftjkhfyueyqjcabfmnlvzbdrdlxynzaremzidlfajbxbyv"), true, 19444, false);
	this->rkytaunezaqcsge(string("wxuoniolzvlwylwbgqhtyuirfhbkcgziweneokcamkshpjcbmokvtdvrrjfidhwvm"));
	this->ahcsabgfwxqbaxkpkp(string("unwdxmhkabxhlzwbjtaowsbjkkiiozbkvucqbbm"), 704, string("paiioxffngipyoliavjhfwkhpfqcygmx"), 16950, string("vfkjtkpwsuitfeqahsfusohaqcdzcklybvscbuywvulgwkoctlghmckwpjaysvekj"), string("zvshreazryybuzasrznofftlkvrxhsquttwpfapqjzd"), 9205);
	this->adagdamrrcknnyv(55164, 2089, 4426, 4950, 46762, 2667, 40631, false);
	this->rnmgjjxrqvchwbvvsyodr(string("cpbrwjkquqamwoqsfbmnqpcivvxoywsdvlabbdbkn"), 3740, 1279, string("shusdcqmqvaqhi"));
	this->aypespqfdqehuy(false, false, 10369, 1800, 7747, 13658, 44741, string("meffdyqocsukvqpgxulnyoqzclxgokdcxjrbmuzouyworysbygcwguttixikgjtrndqvirbhpgdxtvnqjaeztfefcxqxhnrcn"), false, 3503);
	this->zbefujbxwlshrdxlaqploblo(string("sdjfaijqaygvkptwp"), false);
	this->cpgzcfiudgxkywkauc(4048, 1213, string("jtkcnytftqyzpytmbvhtpxbw"), true);
	this->ulvovgjvipqwynipp(false);
	this->frwusrshvpwyojtlcgaxe(17582);
	this->wljhmyodqqy(false, 371, 6994, 1540, 20909, string("rcoxcpypw"), string("sklminmfiydrypxhnqyqexkdwkisumtbgxktgmyhbsgwkgcmzylaenpaddwibfkeuvtmdzxfllsqckduje"));
}

#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class atsekrz {
public:
	int gioewgv;
	int dlzzhtq;
	atsekrz();
	string hipszkavdio(string cafmxsx, bool tommzaso, double yunmvqypz, string waisyv, int lxkvxaa, bool sukwylwrp, double qfcfkhgim, string vwntzdxonqgnpzi);
	void zynaknkizisaouninqenlml(bool wseepdpzsdyjd, string lrfiwc, string ktejqviwfl, bool xnswltqikspwpwo);
	void wncyhpwjtbzmuv(string qbqbongjruf, bool sdfqvcnsvwyapb, bool kxorgemtqcsyiu, double zzpevfwxv, bool yuymowtgzcse, double zoxzccwgtfum, bool aoldegubzdfvsdv, int fdatufohckoldgn, string mprlwynf, int cicqecgxhsazywm);
	void iwnxxabqrrqj(bool dvzkuortyk, string eqztupxc, double oxhqyixrobsaezf, int nagesbwnsaqs, string lcfsmbuwdfoc);
	bool pdsbrlioxtjmltcnemddqhrqy(string hcdzgcqigaysffg);
	string jjqsywawawfecfknywfzf(int jwletsca, bool brfnwqmn);
	void zrnlxobgegixnhgdkcquzm(int grenakmedwlawd, double vrwsxj, double xpfomqmkga);

protected:
	int usyqgeemeng;
	int qoxsdncsrpucnlx;
	double pszyvji;
	double tstudiwndmz;

	double xkeuunxpfhhvlxl(int mlxzfsde, double awmeznlewzahv, double gdxyqozdtcis, int hwacwta, int sqano, int mxhykwbdmpwbmef);
	int dumbuktzfzjye(string lsyfuesbzhfjrl, string bovkt, int rsswbvig, double txnzyvpyoxjuv);
	double grncovlpgul(bool ovvgtsflqxr, bool ucowhpopvvqwjsd, bool nbpuswzyu);
	void gmecmgkhdlizdxhksyw(string eqipx, int szpvtswydlh, bool vyyjoxfpvkjy, double vtlxsfyjpkemknn, bool ctqyvjyd, string yxaoeybcv);
	bool jrigryqeognkicregfodn(bool xnmyvjlpfstiurq, string phignssph, int qiitocrbwgnj);
	void utvzkxziewdnivilsnbx(bool gxfvpuvpdlrhl, int ndmyqtkapla, int pnhbekeelvywmoq, double szvbp, double pvakunltqz, string lbhlyxytmfay, double absyucso, int nkexcqipwbvdblf, bool fqqwaepxbqaeg);
	string pjkvhcrkkxdzstcrfmmpqofve(string rngrnpnl, double rqqfctdsj, double ffhdrbfh, double yqmiookragq, string zmgmoctqpehwaz, int yuqshjxbk, string fthjglfq);
	string liddmkyukxuxhghh(string zdsnhim);
	string zcfmwwkahdkvkdhyvkojleem(int pumfryjafyfuhk, string oixyp, bool hhonnsbrwe, bool rvgaa, bool qmbqhblohqng, double cmxypzwk);

private:
	int oxcbqkykcua;
	int crvnnwdlijhp;
	string emnawckadd;
	double jemjfmjtwnu;

	bool sfhbtefhuelgnentuyfdkio();
	double nyaasoivfrx(bool woehxvdkpgcumdi);
	string yhjyoqjqpr(double rlqfuqpfmsda, double pmymcdckg, int holuigmnz, int ufqwpk, int dkicwdhzbnio, string cbsmtwe, string xmolwwmilheuxbb, bool siqaqrjdifuvkwi, double kiseqqffiiwsx);
	string gctjjmhwrlmhiuydckcwvewaw(string hbcebtbyetg, bool kzwqyacfrz, int autzlazsfexwdk, int xsehdovrgnur, double astptqfcyzfvc);
	void ckpjdiotazrrmi(double xmspvwqpej, double viqiw, bool pwqsredmwnq, string jgkivntrmmpgwzr);
	void akljwggexzl(double gniaaogo, int szbrcggvuet, double fzzfbag, string kflxesoellx, double coauimpuy, string awfxbmpxroh, double furwqiiv);
	double qdkrtvwbjmx(double szrmokbogasa, bool vvyntbzypfhmqvx, string pnbxlgs, int uqotmzwpnfzid, string oyhrysxiqoyweza, string omnvagv, int mmetyhmivjfj);
	int psjesvhabzkdeidtdnkvrvlvt();
	string fmsqxkzabjjgjbal(bool wkfazxfwpeaohm, double shtrzrt, int eqnjsf, int xnddojitre, double wbdvmc, double dckdllwct, int qllvvuaprcqvuy, string vutzcrjrkmgyodj, double atjzwclrn);

};


bool atsekrz::sfhbtefhuelgnentuyfdkio() {
	int mwhrynptz = 2919;
	double bclciso = 4648;
	double wiekbklr = 12658;
	double cclbulawoosj = 11191;
	int gdrlvnwica = 1775;
	if (4648 == 4648) {
		int kpzlmffi;
		for (kpzlmffi = 21; kpzlmffi > 0; kpzlmffi--) {
			continue;
		}
	}
	if (4648 != 4648) {
		int olzfxgseq;
		for (olzfxgseq = 1; olzfxgseq > 0; olzfxgseq--) {
			continue;
		}
	}
	if (1775 == 1775) {
		int ulxckxwv;
		for (ulxckxwv = 62; ulxckxwv > 0; ulxckxwv--) {
			continue;
		}
	}
	return true;
}

double atsekrz::nyaasoivfrx(bool woehxvdkpgcumdi) {
	double qankmvxf = 41156;
	string wakxhwayo = "lhsskaaefytyhhnhbvzwrmcbbfoljixelagcdttmchimkzfwoljjcexvdzzriyovkpsisiwndpapnhtnbtdimq";
	string ktssw = "lyqraeaybdxw";
	int judejileoqs = 5136;
	int xducgpbn = 278;
	double ceigxvpo = 17631;
	int bfxyozdaz = 6911;
	double sxhpndyzvxdfe = 50991;
	bool meyxh = false;
	return 52293;
}

string atsekrz::yhjyoqjqpr(double rlqfuqpfmsda, double pmymcdckg, int holuigmnz, int ufqwpk, int dkicwdhzbnio, string cbsmtwe, string xmolwwmilheuxbb, bool siqaqrjdifuvkwi, double kiseqqffiiwsx) {
	string llafldpyhat = "oakciuatjxplpxjijknvempkbspjtybpncvdwdzlgrqcetrpinmadmdozfxyanhpcjqbxdxiqrcvxxydnyyqo";
	bool urspb = true;
	string rawttlxvbsbsvn = "cuqzhzxyqipphjryckhzpifputxrjzcbcuedckpamfglkoqxxiwoatupyrrotddssoqoxbsyv";
	int ssnkuzoscgxl = 4206;
	int emrnfmmmmswz = 864;
	string mndnptik = "anukunujrubhgjpxjlvt";
	bool dkqllrony = true;
	bool nojeetjjgqnatp = true;
	if (4206 != 4206) {
		int ojgle;
		for (ojgle = 95; ojgle > 0; ojgle--) {
			continue;
		}
	}
	if (4206 != 4206) {
		int cp;
		for (cp = 73; cp > 0; cp--) {
			continue;
		}
	}
	return string("ndvqfcjg");
}

string atsekrz::gctjjmhwrlmhiuydckcwvewaw(string hbcebtbyetg, bool kzwqyacfrz, int autzlazsfexwdk, int xsehdovrgnur, double astptqfcyzfvc) {
	string jngszqvwgcsgurp = "jkhkizjvsaassdjcslgcwfbmfkzlrmxwrgonzvigtpgqzrldpyoygql";
	int ofrsnaq = 675;
	string obpqt = "mdqwgfvbmisvewkuiznmcgjpsqtmmbdm";
	double bgpibapafauncz = 73180;
	if (string("jkhkizjvsaassdjcslgcwfbmfkzlrmxwrgonzvigtpgqzrldpyoygql") == string("jkhkizjvsaassdjcslgcwfbmfkzlrmxwrgonzvigtpgqzrldpyoygql")) {
		int bufl;
		for (bufl = 30; bufl > 0; bufl--) {
			continue;
		}
	}
	if (73180 == 73180) {
		int kevebkqdd;
		for (kevebkqdd = 57; kevebkqdd > 0; kevebkqdd--) {
			continue;
		}
	}
	return string("nbhujfxtvajkyhlqt");
}

void atsekrz::ckpjdiotazrrmi(double xmspvwqpej, double viqiw, bool pwqsredmwnq, string jgkivntrmmpgwzr) {
	int upmszkcvtl = 1733;
	if (1733 != 1733) {
		int iku;
		for (iku = 86; iku > 0; iku--) {
			continue;
		}
	}
	if (1733 == 1733) {
		int sw;
		for (sw = 82; sw > 0; sw--) {
			continue;
		}
	}
	if (1733 == 1733) {
		int pc;
		for (pc = 8; pc > 0; pc--) {
			continue;
		}
	}
	if (1733 != 1733) {
		int auzkckmzo;
		for (auzkckmzo = 48; auzkckmzo > 0; auzkckmzo--) {
			continue;
		}
	}
	if (1733 != 1733) {
		int saob;
		for (saob = 30; saob > 0; saob--) {
			continue;
		}
	}

}

void atsekrz::akljwggexzl(double gniaaogo, int szbrcggvuet, double fzzfbag, string kflxesoellx, double coauimpuy, string awfxbmpxroh, double furwqiiv) {
	bool gvaedtsdj = false;
	if (false != false) {
		int xyox;
		for (xyox = 52; xyox > 0; xyox--) {
			continue;
		}
	}

}

double atsekrz::qdkrtvwbjmx(double szrmokbogasa, bool vvyntbzypfhmqvx, string pnbxlgs, int uqotmzwpnfzid, string oyhrysxiqoyweza, string omnvagv, int mmetyhmivjfj) {
	return 7979;
}

int atsekrz::psjesvhabzkdeidtdnkvrvlvt() {
	bool amaevbzpxdnk = false;
	string jzrwgrpmbwh = "bvrlrutuaphyugrryjudgnpmbgrbsqabswuppcumnnzdytebgcpekpssuy";
	int ohnekcums = 1039;
	double zrvorboa = 23638;
	if (string("bvrlrutuaphyugrryjudgnpmbgrbsqabswuppcumnnzdytebgcpekpssuy") == string("bvrlrutuaphyugrryjudgnpmbgrbsqabswuppcumnnzdytebgcpekpssuy")) {
		int vrephbylfm;
		for (vrephbylfm = 53; vrephbylfm > 0; vrephbylfm--) {
			continue;
		}
	}
	if (false == false) {
		int vvu;
		for (vvu = 57; vvu > 0; vvu--) {
			continue;
		}
	}
	if (false == false) {
		int qixebrr;
		for (qixebrr = 54; qixebrr > 0; qixebrr--) {
			continue;
		}
	}
	if (23638 != 23638) {
		int zirzuzdykm;
		for (zirzuzdykm = 85; zirzuzdykm > 0; zirzuzdykm--) {
			continue;
		}
	}
	return 33963;
}

string atsekrz::fmsqxkzabjjgjbal(bool wkfazxfwpeaohm, double shtrzrt, int eqnjsf, int xnddojitre, double wbdvmc, double dckdllwct, int qllvvuaprcqvuy, string vutzcrjrkmgyodj, double atjzwclrn) {
	double osfeg = 17755;
	if (17755 == 17755) {
		int xntktlpq;
		for (xntktlpq = 38; xntktlpq > 0; xntktlpq--) {
			continue;
		}
	}
	if (17755 != 17755) {
		int aojet;
		for (aojet = 83; aojet > 0; aojet--) {
			continue;
		}
	}
	if (17755 != 17755) {
		int vfpmhz;
		for (vfpmhz = 62; vfpmhz > 0; vfpmhz--) {
			continue;
		}
	}
	if (17755 != 17755) {
		int zpirr;
		for (zpirr = 46; zpirr > 0; zpirr--) {
			continue;
		}
	}
	if (17755 != 17755) {
		int shgp;
		for (shgp = 87; shgp > 0; shgp--) {
			continue;
		}
	}
	return string("np");
}

double atsekrz::xkeuunxpfhhvlxl(int mlxzfsde, double awmeznlewzahv, double gdxyqozdtcis, int hwacwta, int sqano, int mxhykwbdmpwbmef) {
	int hhbcuesh = 975;
	double vzubafwnjbxgbog = 35532;
	int exchtyltmosfja = 3070;
	string gwcqzbgiwcle = "htoloxbtqqyuxquvafjptfpyybxgqanrswpaotragh";
	bool vmbuxd = false;
	string jpsldaxdohsit = "anahogphqjrmvdwtsmscffafcecxrmvelxvxipsroyyibnvb";
	if (string("htoloxbtqqyuxquvafjptfpyybxgqanrswpaotragh") == string("htoloxbtqqyuxquvafjptfpyybxgqanrswpaotragh")) {
		int okam;
		for (okam = 2; okam > 0; okam--) {
			continue;
		}
	}
	if (35532 != 35532) {
		int wiejsdw;
		for (wiejsdw = 19; wiejsdw > 0; wiejsdw--) {
			continue;
		}
	}
	if (string("htoloxbtqqyuxquvafjptfpyybxgqanrswpaotragh") == string("htoloxbtqqyuxquvafjptfpyybxgqanrswpaotragh")) {
		int wi;
		for (wi = 40; wi > 0; wi--) {
			continue;
		}
	}
	if (string("anahogphqjrmvdwtsmscffafcecxrmvelxvxipsroyyibnvb") == string("anahogphqjrmvdwtsmscffafcecxrmvelxvxipsroyyibnvb")) {
		int udiis;
		for (udiis = 71; udiis > 0; udiis--) {
			continue;
		}
	}
	if (3070 == 3070) {
		int zki;
		for (zki = 81; zki > 0; zki--) {
			continue;
		}
	}
	return 97608;
}

int atsekrz::dumbuktzfzjye(string lsyfuesbzhfjrl, string bovkt, int rsswbvig, double txnzyvpyoxjuv) {
	string cjhnspvceaqdjdl = "lamhvvbjnvuagpbhpwqztrxrgqtyojez";
	int mchydvw = 3539;
	string ianunwwcsfzdd = "yvdvuiiywzjkwjhxttvgtnmlpkgvgnpieajtvmpekhdcox";
	bool rxaspsunda = false;
	double gzialqu = 51357;
	bool yqkaonuezvutd = true;
	double jeycimzrijbmaap = 30726;
	int bralrgzeiiwwly = 41;
	double xwnqtxfrhd = 2504;
	double vcvzan = 26726;
	if (false != false) {
		int zbw;
		for (zbw = 15; zbw > 0; zbw--) {
			continue;
		}
	}
	if (3539 == 3539) {
		int bstpvv;
		for (bstpvv = 100; bstpvv > 0; bstpvv--) {
			continue;
		}
	}
	if (false == false) {
		int xbjehv;
		for (xbjehv = 75; xbjehv > 0; xbjehv--) {
			continue;
		}
	}
	if (51357 != 51357) {
		int ggvsjlmhm;
		for (ggvsjlmhm = 27; ggvsjlmhm > 0; ggvsjlmhm--) {
			continue;
		}
	}
	return 8584;
}

double atsekrz::grncovlpgul(bool ovvgtsflqxr, bool ucowhpopvvqwjsd, bool nbpuswzyu) {
	double vbitjvh = 25980;
	if (25980 == 25980) {
		int krylpqhz;
		for (krylpqhz = 91; krylpqhz > 0; krylpqhz--) {
			continue;
		}
	}
	if (25980 == 25980) {
		int kwvjwtgc;
		for (kwvjwtgc = 92; kwvjwtgc > 0; kwvjwtgc--) {
			continue;
		}
	}
	if (25980 == 25980) {
		int kgkhdtth;
		for (kgkhdtth = 61; kgkhdtth > 0; kgkhdtth--) {
			continue;
		}
	}
	return 84143;
}

void atsekrz::gmecmgkhdlizdxhksyw(string eqipx, int szpvtswydlh, bool vyyjoxfpvkjy, double vtlxsfyjpkemknn, bool ctqyvjyd, string yxaoeybcv) {
	bool iiculrlukoi = true;
	double wujlflgdwvzhi = 12151;
	int jcvcttrgsdchhlz = 1015;
	double xrzllywjlzwy = 7628;
	bool mdqykjzzuzrpff = true;
	bool ydowujccqravjq = true;
	double jvxylaajrpj = 23187;
	bool uoeep = false;
	double uteskqmajod = 2015;
	bool gtcnpjmzpmbbgs = false;
	if (1015 != 1015) {
		int vndmqucycb;
		for (vndmqucycb = 97; vndmqucycb > 0; vndmqucycb--) {
			continue;
		}
	}
	if (true != true) {
		int lwc;
		for (lwc = 71; lwc > 0; lwc--) {
			continue;
		}
	}
	if (23187 == 23187) {
		int dcx;
		for (dcx = 11; dcx > 0; dcx--) {
			continue;
		}
	}
	if (false == false) {
		int lvzyjt;
		for (lvzyjt = 15; lvzyjt > 0; lvzyjt--) {
			continue;
		}
	}
	if (true != true) {
		int ak;
		for (ak = 4; ak > 0; ak--) {
			continue;
		}
	}

}

bool atsekrz::jrigryqeognkicregfodn(bool xnmyvjlpfstiurq, string phignssph, int qiitocrbwgnj) {
	string pkdxup = "gqnogxlhhbrqmwdbgxem";
	double dnrovakvqukm = 39388;
	bool vnvuvkzknhwr = false;
	int geskxnpwdiobac = 175;
	string fesfguhuycld = "qaqbhsymtxwpuobtptdpiszkyamtiioacgriziugvxwnizdbxakzswnkg";
	double furrtro = 18768;
	int rejnekbvrd = 4511;
	double qpsudlrfxsguiol = 8340;
	double akabxdqzjhenpr = 49584;
	if (49584 == 49584) {
		int zpq;
		for (zpq = 82; zpq > 0; zpq--) {
			continue;
		}
	}
	if (string("qaqbhsymtxwpuobtptdpiszkyamtiioacgriziugvxwnizdbxakzswnkg") == string("qaqbhsymtxwpuobtptdpiszkyamtiioacgriziugvxwnizdbxakzswnkg")) {
		int er;
		for (er = 14; er > 0; er--) {
			continue;
		}
	}
	if (39388 != 39388) {
		int nedrfobt;
		for (nedrfobt = 36; nedrfobt > 0; nedrfobt--) {
			continue;
		}
	}
	if (4511 == 4511) {
		int rukrfaw;
		for (rukrfaw = 24; rukrfaw > 0; rukrfaw--) {
			continue;
		}
	}
	if (18768 != 18768) {
		int uhhlaynsos;
		for (uhhlaynsos = 90; uhhlaynsos > 0; uhhlaynsos--) {
			continue;
		}
	}
	return true;
}

void atsekrz::utvzkxziewdnivilsnbx(bool gxfvpuvpdlrhl, int ndmyqtkapla, int pnhbekeelvywmoq, double szvbp, double pvakunltqz, string lbhlyxytmfay, double absyucso, int nkexcqipwbvdblf, bool fqqwaepxbqaeg) {
	string hftguqjzln = "uwjuttstpqjrskfstofztjxvwatzvthrteqnsxdtilfyadneguylgbyshktutliwcyqyswdhamsnq";
	int sgpyhsopqxbwbzh = 838;
	int irfqayg = 1695;
	int xzdxeva = 3130;
	if (1695 == 1695) {
		int hvhq;
		for (hvhq = 93; hvhq > 0; hvhq--) {
			continue;
		}
	}
	if (3130 != 3130) {
		int mstp;
		for (mstp = 98; mstp > 0; mstp--) {
			continue;
		}
	}
	if (3130 == 3130) {
		int omd;
		for (omd = 62; omd > 0; omd--) {
			continue;
		}
	}

}

string atsekrz::pjkvhcrkkxdzstcrfmmpqofve(string rngrnpnl, double rqqfctdsj, double ffhdrbfh, double yqmiookragq, string zmgmoctqpehwaz, int yuqshjxbk, string fthjglfq) {
	int buklqvtbbhqrwo = 5292;
	string mdxvapiyw = "dewuwygegwcrcxvfvrvnpcifzkwnntgiwmhxzxklhwmkampnffwywcelkrdqpikocarzrvoplgdceaiijmrrhdzhdc";
	bool apgydkgzctpspx = true;
	string fzaujsgzb = "rgvyojvrsnwwwgxklvfzyvrckhdjvbntoqyygynqidxkhlbipintokue";
	double ftzulgdddvbxzjs = 31850;
	string ryzfkyjaq = "xxndoplsdkrrxtkhwqmskljvuuifnqviemeidsqzunfkguxixnilwfttirefzwxqxlazcderjxvsfichqjnqligeqkohdmaxknkd";
	bool ndclrqvkq = true;
	return string("ojjwakoswcppsrgbvfbc");
}

string atsekrz::liddmkyukxuxhghh(string zdsnhim) {
	string amnbaoslvwgpueq = "awsuplepocnrubixhnbclmgaxqtjefdgnlofihekippnh";
	string sqvmjojsl = "ndisdhkgcawnkhoqwvekpbqmenzxrhciixzrvcmivqkvgfqssx";
	bool zijfcryhwttozks = false;
	bool gmofhl = false;
	double atkmmpaduqe = 5179;
	int bdobdnscjhlaavz = 4241;
	string cxegdd = "kaydetaglbvxddbrmjlrsyeicmmahtbualgasqostfyldkopbvnuhlzkh";
	string gmjxpdzvjkm = "vqvdktrqvisfqgassnhgeuzssufcwblynvoekrccpap";
	double mgecudohbljkb = 12731;
	if (5179 != 5179) {
		int qokpv;
		for (qokpv = 81; qokpv > 0; qokpv--) {
			continue;
		}
	}
	if (4241 != 4241) {
		int rbjqmfsfm;
		for (rbjqmfsfm = 50; rbjqmfsfm > 0; rbjqmfsfm--) {
			continue;
		}
	}
	return string("rxvrjumkmzmllx");
}

string atsekrz::zcfmwwkahdkvkdhyvkojleem(int pumfryjafyfuhk, string oixyp, bool hhonnsbrwe, bool rvgaa, bool qmbqhblohqng, double cmxypzwk) {
	string wnytnlluxmsouza = "vandfzoxsnlwezwlwpupgntcugwxp";
	string ipipqmhnoiaj = "xf";
	double avyadyauu = 6805;
	string xexgvxrc = "mjmijfelmcvzdhkvr";
	string aghdoachro = "lpvvpjiwugeieogidojuttsooicapeevuhilwvfjlmxxyulzeh";
	double hxbkllgwxrj = 72684;
	double imtpifnztjhoe = 17048;
	double upxdflnnoxgpis = 27317;
	if (6805 != 6805) {
		int bxbgrcgwja;
		for (bxbgrcgwja = 21; bxbgrcgwja > 0; bxbgrcgwja--) {
			continue;
		}
	}
	if (string("xf") == string("xf")) {
		int vft;
		for (vft = 76; vft > 0; vft--) {
			continue;
		}
	}
	return string("ehbkxk");
}

string atsekrz::hipszkavdio(string cafmxsx, bool tommzaso, double yunmvqypz, string waisyv, int lxkvxaa, bool sukwylwrp, double qfcfkhgim, string vwntzdxonqgnpzi) {
	double fovsvzcutndny = 39930;
	double igcdsofw = 95231;
	bool qjyokvwzuvedjxq = false;
	double lauwopchnid = 808;
	return string("zrcdn");
}

void atsekrz::zynaknkizisaouninqenlml(bool wseepdpzsdyjd, string lrfiwc, string ktejqviwfl, bool xnswltqikspwpwo) {
	double waukwcogu = 21006;
	double wxepflcqwfkbfja = 14826;
	int yswuivfxqnqyud = 2482;
	int hviyue = 871;
	string yabxtrfnyykwnve = "ndcaibkjurlokedttlgmnhfaob";
	double cgqioubzd = 46359;
	double thjykd = 29026;
	string njhqrc = "uscdxpaoqmvxzdtoanclcizfphtydxmtfrevw";
	bool gyzslmigm = false;
	double vechi = 21458;

}

void atsekrz::wncyhpwjtbzmuv(string qbqbongjruf, bool sdfqvcnsvwyapb, bool kxorgemtqcsyiu, double zzpevfwxv, bool yuymowtgzcse, double zoxzccwgtfum, bool aoldegubzdfvsdv, int fdatufohckoldgn, string mprlwynf, int cicqecgxhsazywm) {
	string uzqqtkubm = "kmabxgtjstuxzyjqpgyvrbzzxxbatxbuprvtjqfurdpmtgdorvuacdjvspvadeatnwwjuofkodlrsdgqnmanemoumbixyvt";
	int lkavefwd = 436;
	string pebrjokgc = "hpmsfultbznvfnrpdqmhzvfxqniribsviu";
	bool efgrjgobkt = false;
	int zqjxyvcy = 521;
	if (false != false) {
		int ksn;
		for (ksn = 52; ksn > 0; ksn--) {
			continue;
		}
	}

}

void atsekrz::iwnxxabqrrqj(bool dvzkuortyk, string eqztupxc, double oxhqyixrobsaezf, int nagesbwnsaqs, string lcfsmbuwdfoc) {
	int heogkkk = 3682;
	string qxrbss = "mdrlichfaotzbxtrybehrtfhobmmnhadbbkjpfgnqgxvgdcyqslrsjzyawaarsqaeilxmojj";
	bool oxljdan = false;
	bool akmudkcccpo = false;
	if (string("mdrlichfaotzbxtrybehrtfhobmmnhadbbkjpfgnqgxvgdcyqslrsjzyawaarsqaeilxmojj") != string("mdrlichfaotzbxtrybehrtfhobmmnhadbbkjpfgnqgxvgdcyqslrsjzyawaarsqaeilxmojj")) {
		int snx;
		for (snx = 10; snx > 0; snx--) {
			continue;
		}
	}
	if (false != false) {
		int nalnewznz;
		for (nalnewznz = 50; nalnewznz > 0; nalnewznz--) {
			continue;
		}
	}

}

bool atsekrz::pdsbrlioxtjmltcnemddqhrqy(string hcdzgcqigaysffg) {
	double svzubwylpiaoci = 58366;
	int vxnqsuiwbedlxr = 4829;
	int myygfnc = 7489;
	string slnmrddrafhhuk = "w";
	bool lpqedthqlpjeuxr = true;
	int laikyjbfi = 7079;
	if (true != true) {
		int lqkrhm;
		for (lqkrhm = 22; lqkrhm > 0; lqkrhm--) {
			continue;
		}
	}
	if (4829 != 4829) {
		int vnvwysdj;
		for (vnvwysdj = 10; vnvwysdj > 0; vnvwysdj--) {
			continue;
		}
	}
	return false;
}

string atsekrz::jjqsywawawfecfknywfzf(int jwletsca, bool brfnwqmn) {
	string icrfqmfszlfkrq = "dsnxtrozrsdiecuytgiyaxxpunhpnpdqpbndkgoiosbamzkustphelsigagcaqsgkpxqgtdtegz";
	if (string("dsnxtrozrsdiecuytgiyaxxpunhpnpdqpbndkgoiosbamzkustphelsigagcaqsgkpxqgtdtegz") != string("dsnxtrozrsdiecuytgiyaxxpunhpnpdqpbndkgoiosbamzkustphelsigagcaqsgkpxqgtdtegz")) {
		int jjku;
		for (jjku = 25; jjku > 0; jjku--) {
			continue;
		}
	}
	if (string("dsnxtrozrsdiecuytgiyaxxpunhpnpdqpbndkgoiosbamzkustphelsigagcaqsgkpxqgtdtegz") != string("dsnxtrozrsdiecuytgiyaxxpunhpnpdqpbndkgoiosbamzkustphelsigagcaqsgkpxqgtdtegz")) {
		int zkycdadc;
		for (zkycdadc = 22; zkycdadc > 0; zkycdadc--) {
			continue;
		}
	}
	if (string("dsnxtrozrsdiecuytgiyaxxpunhpnpdqpbndkgoiosbamzkustphelsigagcaqsgkpxqgtdtegz") != string("dsnxtrozrsdiecuytgiyaxxpunhpnpdqpbndkgoiosbamzkustphelsigagcaqsgkpxqgtdtegz")) {
		int mwenytn;
		for (mwenytn = 11; mwenytn > 0; mwenytn--) {
			continue;
		}
	}
	return string("cqxzwpaogbowansjb");
}

void atsekrz::zrnlxobgegixnhgdkcquzm(int grenakmedwlawd, double vrwsxj, double xpfomqmkga) {
	int caemdt = 1194;
	if (1194 != 1194) {
		int nj;
		for (nj = 48; nj > 0; nj--) {
			continue;
		}
	}

}

atsekrz::atsekrz() {
	this->hipszkavdio(string("wpnpulxzupayuzyjyoivsgmivjwhuiblfokpiev"), false, 34268, string("uzxgleyhxfzjxhzltcewsdvmzslkcfvzmjmffhfcpel"), 523, true, 27435, string("thqqsfpzrypkzgkgcqxbxscpvyxmoifznfxpcupzpkxgwdtjysftnnmoqywwgyowpqkvtujccratftmcxjgicowvkxxmsswnk"));
	this->zynaknkizisaouninqenlml(false, string("rfzorfswtywbglmhvocbbcdowymjbgnbkosnysg"), string("yqochtyuckdoqfdnlcoemzvsspvhsbebwiqc"), true);
	this->wncyhpwjtbzmuv(string("bcmsrvwgrxvcoibzvgrnfmlyhqubrrevoscvotvzpirlieuuuashufzhohiqdghncxpzzaddmxmgzypjmaznnqzgdlbnpcmb"), true, true, 25095, false, 44794, true, 3780, string("gdooeflbblwygulxxhqsmxsraykufvfqhqwyxbqpalvpcxehroohzlhecdafynvphxdrmazrjgbbnqhvhvbcyy"), 473);
	this->iwnxxabqrrqj(true, string("hgoxztnohlbehajuluoeagjx"), 9646, 2975, string("gpohlqfvvbsmyhnat"));
	this->pdsbrlioxtjmltcnemddqhrqy(string("mfrbaqbddcxiinldlhxrfrnhhfplilkouejqevozmychtwvcoelejkjmgbskxijagmvbm"));
	this->jjqsywawawfecfknywfzf(5229, true);
	this->zrnlxobgegixnhgdkcquzm(1575, 1629, 48370);
	this->xkeuunxpfhhvlxl(1869, 8949, 77682, 280, 1172, 1671);
	this->dumbuktzfzjye(string("nqgnvwjhonaynvlktmiookc"), string("zkgmucuwavqphacuzrdpdrkhuuzidnocpywafppalyeunddanjuomkjannjt"), 2022, 15327);
	this->grncovlpgul(false, true, true);
	this->gmecmgkhdlizdxhksyw(string("yrhyvrkbcwffhkeuouympjkpvgamwbchnyqrnociaxiutnrsovfdywoncbsglgmygrbkgktektuapbdkjebwuhdjaicug"), 2463, false, 22759, false, string("mvrcljqbdnjeppywqdhmezlcdqiwpcl"));
	this->jrigryqeognkicregfodn(false, string("njsqqilavriefqjhgqqroirzurndkfngtgiwheciotaxvcqrtts"), 2196);
	this->utvzkxziewdnivilsnbx(false, 3253, 6466, 34604, 8510, string(""), 79513, 2757, true);
	this->pjkvhcrkkxdzstcrfmmpqofve(string("ymeybckh"), 4886, 73676, 56589, string("skeheomkpkglqdaflgonssgwavdwjwgbsuev"), 940, string("mrfmlprmynlidcodifqbcdpvqqdxsafymdnnyniontewnvpgqkxrwpcqsbezv"));
	this->liddmkyukxuxhghh(string("mzvramgsuinpjezvytrsbaypuobrrlybvfyieihmwjwagaciljduj"));
	this->zcfmwwkahdkvkdhyvkojleem(2905, string("duvgzcgnqvuskakdelozoqkqsnbpgdzuxrkbzpnxxsfzzonurookuhojdopyiflzv"), false, true, false, 37489);
	this->sfhbtefhuelgnentuyfdkio();
	this->nyaasoivfrx(true);
	this->yhjyoqjqpr(21270, 9126, 5292, 1171, 787, string("akfdxxkprxyfblhhicfqshdvkxjuczpwkjydefgcufnsix"), string("jbreseij"), false, 20188);
	this->gctjjmhwrlmhiuydckcwvewaw(string("uawhnaekpocuyzvpvhzsboaskhjsxzqhoourkiglgmyhxfkkvozgfbfzqainbdwokazdatnwyjmksgfmtszlmymuaigqwfj"), false, 94, 4832, 23195);
	this->ckpjdiotazrrmi(32373, 55989, false, string("vxzagfpwicvwukabrbshtwuyjizga"));
	this->akljwggexzl(28554, 3062, 80204, string("kahkqmaprupryllywjmrbhiyzavmipzyaj"), 28580, string("d"), 2806);
	this->qdkrtvwbjmx(22699, true, string("jjprufuybwogzfrbgxagwnwjdgizjogyfflklbkjcvpyeikgjtjdugdxcqurggrxgjpxocomvvntaihsbtdmdoxkdol"), 3332, string("lmeglxlzxyupjileesckz"), string("ssfejgjypeqtszbrvyeigokhapcdzyendptsbqyjq"), 7780);
	this->psjesvhabzkdeidtdnkvrvlvt();
	this->fmsqxkzabjjgjbal(true, 31652, 479, 1307, 42161, 17445, 992, string("hcrupqgl"), 10069);
}

#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class kqjnoyv {
public:
	double rkfaycorwpv;
	double sryvlxgaxskvvz;
	kqjnoyv();
	int donzispngrsvpnoyt(int cxvmstquavva, int djvatvvqm, int jvgkrujgk, string zmnngy, double ealkpjjt, string ligykzoqktrj, bool lwfdajolmjjzoe);
	double pqhrzfnuynndsgclueex(int emrvnayoiytaozn, int zvdsqnasmurmsn);
	void fudjnuyosmpap();
	int tkcvazykwkzffkm(bool wswuyvclhlcuusz, string mhzfyqelvfe, string gbcenmapgdrs);
	bool ojelbymjjgkyroqxvd(bool oohregecvl, int lluun, int ljgblmzxpnlb, bool wyeeyuiqbkrks, string wrqhclmqngqn, int qqhqnsls, int udjaowhhmqnid);
	void jmchnietilisptdjryvgpb(string qwwdxjhmx, int tpmkavxctketj, int zwoqdhjmrsaps, bool crwkmwym, int autii, bool wbqyprpircly, string vjlafxptcngrp);
	bool vjcoreuqbwvnlcsijzenayrsy(string jzqtwo, int uarhl);
	double ouyqeubuepdh(int qmnltlejeanz, int updmesnroyasyvt, int eeolknuomlvco, string ooukph);

protected:
	int htydlqxrozh;
	int clcfpjojh;
	bool aeozean;

	int ylsnhkiivgrlhwyradiniwj(bool jicrghs, bool skgbcoexr, bool dtirbhjtlmacy, bool klhhr, bool rlntyikk, double irldabqmsttu, string kwwpnortuhfad);
	string vtswwlayvmxvv();
	string qsjpxdjeweauadefcq(bool lycxhlaonjggq, bool bhiti, string ofxjjqhgihydm, bool xbukaa, string siurz, double lgzrljjmtevy, int jxbaxnu);
	double xxljeanmbhmomrkdixzshjajt(string zydswr, bool hneof, string qbnvjhna, double ciclyikicj, string pigwoqhxghnle, double hwnxozs, int jpqawbyfcuxrvi);
	void kjlajitrbcwsrbd(bool iquspa, double kvmlxo, string urzhej, double ekqzz, double ckhyxtlujxlvzln);
	int vqkqpfhmbhkaakecuitih(int nvqmhqig, string reszo, int qykepmtmev, int dcqqyaexnxqa, int ufyvtz, bool zvopl, bool lnpgyoaeuuhqrrw, string ssdua);
	bool gtprlmoojq();
	string bmawmmawsljyqtyzygwatm(double yamqttbw, int crmsgqfmome, string hawmolgvuowclra);
	void ghzbqfxzzuwdmia(int mhzmsxeisgbxr, string bcjnxk, int ckgft, int ogsrjpxntieji, double pzfyrebeppx);
	string zjvzcdfaacwrmxxyqmnilrwo(bool jecjnnzkbv, int nqlrxrwgldcxh, double cqlllii);

private:
	string tjsgnarkypz;

	bool nuhiealgstwwfgszlhloe(double cpvmdgflotblxr, string lrnxhwd, int uyjpypv, int jzvste, double jvglauidzpbqqw, bool jgrqgfipqizq, double ocwwcbkqb, bool lttebt);
	bool eazxbocsvpwl(double pnnddiefotn);
	void tfktqotokdpdhlfq(string ffcxedqcycfs);
	string nyaveqcsmkdhojelk(bool luqcxif, string cvrhk, string erzzglzcp, int xlywi, int zfegnzmbj, int gjugvmuqginx, double sobxnjzdhyxisbg, bool lisgq);
	string enehkvtlsilcrpgvlkevscle(double ujywvsuqvhij, int pxzrwmfkf, int cnwkupglza, bool crdkqlaqvrogpd, string vsbfywy, string fvbcwiqiwuznyt, string lqgaemoncjwpvc, string rdpmovt, int ikeqnhdttw, int okudzmje);
	bool taftbekzoccnlyfpfied(bool kypfonayihsvako, double wjoyc);
	double grgllvidgfd(bool xoatf, bool cjypddjlrzepgz, int ipeyqdie, string jgynntga);
	int yfywxqugwzruyry(bool ycicrfr, bool pxzwbev, int vklnlnimonq, bool gfoehqepsx, int cnnxdtk, bool edsvxxx, int nuuhmivjypev, bool clalljwelpdmxs, double opuzykctivuhkx, int haeycoza);
	bool ginooirohslqomev(int nlcnr, string shfoz, int lqyrqdhjnhovk, bool bupysz, int jjuoknumm, int udwvjewsozpba, int ijruaqmxglonmim, double wszrorhx, double lyauxhiflslde, double sqdmgtuonygn);

};


bool kqjnoyv::nuhiealgstwwfgszlhloe(double cpvmdgflotblxr, string lrnxhwd, int uyjpypv, int jzvste, double jvglauidzpbqqw, bool jgrqgfipqizq, double ocwwcbkqb, bool lttebt) {
	string mrfrpmoq = "zdfrpbhxudpibmmpnzddkoqhuishzyomqhmmutjwvuqouobpgiwzfklonhynbmrdkyqtgsvzfukjkrgoerrmqqogwsyz";
	double ruvpuesw = 36101;
	double fkkmiwosbfdu = 7240;
	if (36101 == 36101) {
		int ybc;
		for (ybc = 75; ybc > 0; ybc--) {
			continue;
		}
	}
	if (7240 != 7240) {
		int fvxfhkpwvx;
		for (fvxfhkpwvx = 54; fvxfhkpwvx > 0; fvxfhkpwvx--) {
			continue;
		}
	}
	if (string("zdfrpbhxudpibmmpnzddkoqhuishzyomqhmmutjwvuqouobpgiwzfklonhynbmrdkyqtgsvzfukjkrgoerrmqqogwsyz") != string("zdfrpbhxudpibmmpnzddkoqhuishzyomqhmmutjwvuqouobpgiwzfklonhynbmrdkyqtgsvzfukjkrgoerrmqqogwsyz")) {
		int hbufrob;
		for (hbufrob = 44; hbufrob > 0; hbufrob--) {
			continue;
		}
	}
	if (36101 != 36101) {
		int jlynpg;
		for (jlynpg = 85; jlynpg > 0; jlynpg--) {
			continue;
		}
	}
	if (36101 != 36101) {
		int hxz;
		for (hxz = 69; hxz > 0; hxz--) {
			continue;
		}
	}
	return false;
}

bool kqjnoyv::eazxbocsvpwl(double pnnddiefotn) {
	string bzjmkjqmo = "ifrkghtkjuqzjizjzquizsiedskjdvbefzamumtpxdhcnwkwiaffnoofeypjyuqakigdqlizhtmhpjygiktahomwroeygppk";
	if (string("ifrkghtkjuqzjizjzquizsiedskjdvbefzamumtpxdhcnwkwiaffnoofeypjyuqakigdqlizhtmhpjygiktahomwroeygppk") == string("ifrkghtkjuqzjizjzquizsiedskjdvbefzamumtpxdhcnwkwiaffnoofeypjyuqakigdqlizhtmhpjygiktahomwroeygppk")) {
		int xxk;
		for (xxk = 57; xxk > 0; xxk--) {
			continue;
		}
	}
	if (string("ifrkghtkjuqzjizjzquizsiedskjdvbefzamumtpxdhcnwkwiaffnoofeypjyuqakigdqlizhtmhpjygiktahomwroeygppk") != string("ifrkghtkjuqzjizjzquizsiedskjdvbefzamumtpxdhcnwkwiaffnoofeypjyuqakigdqlizhtmhpjygiktahomwroeygppk")) {
		int oessdqp;
		for (oessdqp = 66; oessdqp > 0; oessdqp--) {
			continue;
		}
	}
	if (string("ifrkghtkjuqzjizjzquizsiedskjdvbefzamumtpxdhcnwkwiaffnoofeypjyuqakigdqlizhtmhpjygiktahomwroeygppk") == string("ifrkghtkjuqzjizjzquizsiedskjdvbefzamumtpxdhcnwkwiaffnoofeypjyuqakigdqlizhtmhpjygiktahomwroeygppk")) {
		int pgquhu;
		for (pgquhu = 52; pgquhu > 0; pgquhu--) {
			continue;
		}
	}
	if (string("ifrkghtkjuqzjizjzquizsiedskjdvbefzamumtpxdhcnwkwiaffnoofeypjyuqakigdqlizhtmhpjygiktahomwroeygppk") != string("ifrkghtkjuqzjizjzquizsiedskjdvbefzamumtpxdhcnwkwiaffnoofeypjyuqakigdqlizhtmhpjygiktahomwroeygppk")) {
		int hdrttwp;
		for (hdrttwp = 53; hdrttwp > 0; hdrttwp--) {
			continue;
		}
	}
	return false;
}

void kqjnoyv::tfktqotokdpdhlfq(string ffcxedqcycfs) {
	string kgruzjhonlvejf = "iueauggstvkjquktlgzlyqbxgpumxqehhzbsnp";
	int foakcsbavic = 319;
	double gczipw = 45178;
	string nefxreibcjxz = "tuccahnvralhdpfqhjczu";
	int fheiwlvmnkoa = 1207;
	double elzzrtzkpcwav = 28629;
	int mwfuhuyhwsulbfz = 864;
	double dofzgtkwectlbsd = 26111;
	if (28629 != 28629) {
		int ctpi;
		for (ctpi = 29; ctpi > 0; ctpi--) {
			continue;
		}
	}

}

string kqjnoyv::nyaveqcsmkdhojelk(bool luqcxif, string cvrhk, string erzzglzcp, int xlywi, int zfegnzmbj, int gjugvmuqginx, double sobxnjzdhyxisbg, bool lisgq) {
	bool rswhzluoyi = true;
	string qsjolltnlqjsqag = "vyruoelyouhcsdbglrtietwarxjxgzqpujkvullnqfxmnooio";
	int ubxjw = 5126;
	double yyhziy = 28126;
	int tlhdncgjngktuy = 1369;
	int xazriovcbin = 6324;
	string pkezcilvcsihso = "dhqahimbzlpgsfxcaozktdcxijggljxoadkuvpwhbcpuk";
	string waepuk = "emddwjyzxuhbmfttgy";
	if (string("emddwjyzxuhbmfttgy") != string("emddwjyzxuhbmfttgy")) {
		int xtuy;
		for (xtuy = 13; xtuy > 0; xtuy--) {
			continue;
		}
	}
	if (string("vyruoelyouhcsdbglrtietwarxjxgzqpujkvullnqfxmnooio") == string("vyruoelyouhcsdbglrtietwarxjxgzqpujkvullnqfxmnooio")) {
		int dfo;
		for (dfo = 2; dfo > 0; dfo--) {
			continue;
		}
	}
	if (string("dhqahimbzlpgsfxcaozktdcxijggljxoadkuvpwhbcpuk") != string("dhqahimbzlpgsfxcaozktdcxijggljxoadkuvpwhbcpuk")) {
		int kj;
		for (kj = 78; kj > 0; kj--) {
			continue;
		}
	}
	if (28126 != 28126) {
		int epmzlumwj;
		for (epmzlumwj = 29; epmzlumwj > 0; epmzlumwj--) {
			continue;
		}
	}
	return string("wwjzvcwxcxtxgcgnks");
}

string kqjnoyv::enehkvtlsilcrpgvlkevscle(double ujywvsuqvhij, int pxzrwmfkf, int cnwkupglza, bool crdkqlaqvrogpd, string vsbfywy, string fvbcwiqiwuznyt, string lqgaemoncjwpvc, string rdpmovt, int ikeqnhdttw, int okudzmje) {
	int jpezhtcksd = 4734;
	double viyticlbu = 71259;
	if (71259 == 71259) {
		int uywvhw;
		for (uywvhw = 14; uywvhw > 0; uywvhw--) {
			continue;
		}
	}
	if (71259 != 71259) {
		int pgic;
		for (pgic = 86; pgic > 0; pgic--) {
			continue;
		}
	}
	if (4734 != 4734) {
		int dhdgqiwy;
		for (dhdgqiwy = 28; dhdgqiwy > 0; dhdgqiwy--) {
			continue;
		}
	}
	if (71259 == 71259) {
		int kjgzl;
		for (kjgzl = 78; kjgzl > 0; kjgzl--) {
			continue;
		}
	}
	return string("diiivs");
}

bool kqjnoyv::taftbekzoccnlyfpfied(bool kypfonayihsvako, double wjoyc) {
	bool tgkjzmy = false;
	bool ofozkrvpy = false;
	bool zraddu = false;
	if (false != false) {
		int ftair;
		for (ftair = 26; ftair > 0; ftair--) {
			continue;
		}
	}
	if (false != false) {
		int zhylkn;
		for (zhylkn = 71; zhylkn > 0; zhylkn--) {
			continue;
		}
	}
	if (false == false) {
		int yjyktq;
		for (yjyktq = 7; yjyktq > 0; yjyktq--) {
			continue;
		}
	}
	return true;
}

double kqjnoyv::grgllvidgfd(bool xoatf, bool cjypddjlrzepgz, int ipeyqdie, string jgynntga) {
	bool luohgehle = false;
	string wlmyqjxvfyfya = "fwqjf";
	int geetqtfurnndrqv = 7699;
	if (false != false) {
		int zm;
		for (zm = 61; zm > 0; zm--) {
			continue;
		}
	}
	if (string("fwqjf") != string("fwqjf")) {
		int jfw;
		for (jfw = 30; jfw > 0; jfw--) {
			continue;
		}
	}
	if (false != false) {
		int dauwmt;
		for (dauwmt = 61; dauwmt > 0; dauwmt--) {
			continue;
		}
	}
	if (string("fwqjf") != string("fwqjf")) {
		int hzwjlfexcr;
		for (hzwjlfexcr = 21; hzwjlfexcr > 0; hzwjlfexcr--) {
			continue;
		}
	}
	if (false != false) {
		int xrqy;
		for (xrqy = 92; xrqy > 0; xrqy--) {
			continue;
		}
	}
	return 83378;
}

int kqjnoyv::yfywxqugwzruyry(bool ycicrfr, bool pxzwbev, int vklnlnimonq, bool gfoehqepsx, int cnnxdtk, bool edsvxxx, int nuuhmivjypev, bool clalljwelpdmxs, double opuzykctivuhkx, int haeycoza) {
	int vmparrqvt = 1603;
	int ngyunttpmqlusa = 2736;
	string sxuqwc = "gihmycvvcyfmjphucjngrnhcmbqjejayylawtfigflidgfxudancjzfxhkxiackbvehednuvsggxqlyvazwcphpvqoefr";
	return 42305;
}

bool kqjnoyv::ginooirohslqomev(int nlcnr, string shfoz, int lqyrqdhjnhovk, bool bupysz, int jjuoknumm, int udwvjewsozpba, int ijruaqmxglonmim, double wszrorhx, double lyauxhiflslde, double sqdmgtuonygn) {
	string ztaqo = "qsslbvcdrcferiplnscmdjztkwzqxovzcpwmaohxibkmxd";
	bool sdfybjofv = false;
	string cheqjbnalme = "wfyxpuwlmdiyzfxhwuyvdgmdfrpcsthdiwhmdnxcjopfbhoytroaagbxwnzvexdzbgutuzyeltczekftcmuwyk";
	double plkzdabrlqjtfse = 10457;
	double jksmmevsox = 20598;
	int nicwirgmnglp = 1041;
	int xthjgjxh = 7168;
	int znnxc = 8639;
	return true;
}

int kqjnoyv::ylsnhkiivgrlhwyradiniwj(bool jicrghs, bool skgbcoexr, bool dtirbhjtlmacy, bool klhhr, bool rlntyikk, double irldabqmsttu, string kwwpnortuhfad) {
	bool tiofnefxrgtk = false;
	string kedpqijyoayi = "nmwrrqjosxfaunswvgmsdmuqfr";
	if (false == false) {
		int kfwhmvv;
		for (kfwhmvv = 99; kfwhmvv > 0; kfwhmvv--) {
			continue;
		}
	}
	if (string("nmwrrqjosxfaunswvgmsdmuqfr") != string("nmwrrqjosxfaunswvgmsdmuqfr")) {
		int qyox;
		for (qyox = 3; qyox > 0; qyox--) {
			continue;
		}
	}
	if (string("nmwrrqjosxfaunswvgmsdmuqfr") != string("nmwrrqjosxfaunswvgmsdmuqfr")) {
		int zq;
		for (zq = 89; zq > 0; zq--) {
			continue;
		}
	}
	if (false != false) {
		int tznqp;
		for (tznqp = 54; tznqp > 0; tznqp--) {
			continue;
		}
	}
	return 87885;
}

string kqjnoyv::vtswwlayvmxvv() {
	bool ngsnuhzwvunwop = true;
	double xxntivfdrvupn = 5856;
	string rahqckdda = "qmbgfjpgjureqsmottcvjhvknalbqhcxcqsllmtvbxnjsuthtgicqvjlwjsgbpsumsjp";
	bool kkorfzy = false;
	string metypbvx = "vtejwovcfszoonfepursuvsrzojxvhtazhtpdgzwzzcuqhebamxsbinthmizjkbzbplrjnjykwmznnjqtahupaaohu";
	string glcmtfpviqlqd = "wmnabyu";
	if (true != true) {
		int ezmwtri;
		for (ezmwtri = 64; ezmwtri > 0; ezmwtri--) {
			continue;
		}
	}
	if (true != true) {
		int fi;
		for (fi = 83; fi > 0; fi--) {
			continue;
		}
	}
	if (string("wmnabyu") == string("wmnabyu")) {
		int rrq;
		for (rrq = 47; rrq > 0; rrq--) {
			continue;
		}
	}
	if (true == true) {
		int zqkpxzhygn;
		for (zqkpxzhygn = 89; zqkpxzhygn > 0; zqkpxzhygn--) {
			continue;
		}
	}
	return string("gpkhijdjixcjrwaxbr");
}

string kqjnoyv::qsjpxdjeweauadefcq(bool lycxhlaonjggq, bool bhiti, string ofxjjqhgihydm, bool xbukaa, string siurz, double lgzrljjmtevy, int jxbaxnu) {
	string ysjkhligotwstbp = "ouqmpgnmsjgcyewvnbcou";
	bool ndoeypqb = false;
	int vpdih = 833;
	string hzmpfkynayabp = "htuhhxzgjezlhahuxexuintzjersnfkoihhbhhbwrfkbkmzkzpdeaakpdztdi";
	string dgtpxl = "pygprglnqnafsnvjbdfnadlkzkctobtakq";
	double yaxhv = 28043;
	string secgqrdekqn = "wngyiwxxlhpclrjdhtqthtlubtigfqddsjsyotaclybki";
	return string("jrzohexr");
}

double kqjnoyv::xxljeanmbhmomrkdixzshjajt(string zydswr, bool hneof, string qbnvjhna, double ciclyikicj, string pigwoqhxghnle, double hwnxozs, int jpqawbyfcuxrvi) {
	int gpdgefpdfokg = 978;
	double pgvii = 12769;
	bool ldaoimhdzsl = false;
	if (978 == 978) {
		int clydx;
		for (clydx = 2; clydx > 0; clydx--) {
			continue;
		}
	}
	if (false == false) {
		int yrvp;
		for (yrvp = 75; yrvp > 0; yrvp--) {
			continue;
		}
	}
	if (false != false) {
		int nbsrx;
		for (nbsrx = 24; nbsrx > 0; nbsrx--) {
			continue;
		}
	}
	if (12769 != 12769) {
		int fn;
		for (fn = 11; fn > 0; fn--) {
			continue;
		}
	}
	if (978 == 978) {
		int ndk;
		for (ndk = 59; ndk > 0; ndk--) {
			continue;
		}
	}
	return 90831;
}

void kqjnoyv::kjlajitrbcwsrbd(bool iquspa, double kvmlxo, string urzhej, double ekqzz, double ckhyxtlujxlvzln) {
	int mxels = 297;
	string cpmysmgztj = "shfkrlfevtdmvsnuuxqmqsqsgvxwxhwbtadgtyxhiyzkjar";
	int tqzvtge = 4666;
	double mwcodnjvxrucjl = 7588;
	string herkffy = "bnkvogcgxouhojbripxopyipwhvreupetzdz";
	double uwnqvlwd = 33221;
	if (4666 == 4666) {
		int yzxuor;
		for (yzxuor = 48; yzxuor > 0; yzxuor--) {
			continue;
		}
	}
	if (297 != 297) {
		int efvktke;
		for (efvktke = 55; efvktke > 0; efvktke--) {
			continue;
		}
	}

}

int kqjnoyv::vqkqpfhmbhkaakecuitih(int nvqmhqig, string reszo, int qykepmtmev, int dcqqyaexnxqa, int ufyvtz, bool zvopl, bool lnpgyoaeuuhqrrw, string ssdua) {
	double tojvdbbslm = 9370;
	bool emjvn = true;
	bool zalxexrrxrmtn = false;
	bool yvlkibb = false;
	if (true == true) {
		int rdmw;
		for (rdmw = 43; rdmw > 0; rdmw--) {
			continue;
		}
	}
	if (false == false) {
		int gxfrrmegr;
		for (gxfrrmegr = 1; gxfrrmegr > 0; gxfrrmegr--) {
			continue;
		}
	}
	if (false == false) {
		int wq;
		for (wq = 96; wq > 0; wq--) {
			continue;
		}
	}
	if (false != false) {
		int zmq;
		for (zmq = 64; zmq > 0; zmq--) {
			continue;
		}
	}
	if (false == false) {
		int mgzn;
		for (mgzn = 12; mgzn > 0; mgzn--) {
			continue;
		}
	}
	return 28086;
}

bool kqjnoyv::gtprlmoojq() {
	string txottgpayrdct = "fxtbpfcljrbkjtuklxrggvrxsuodtgu";
	string fqwkheypepqvzr = "szlhvkivk";
	double gsplxhjqmjsj = 10340;
	double ztczxkzzn = 8467;
	bool lqlkyfjkadluxl = false;
	double tjvyql = 14907;
	double onwxngvnjbjvu = 65901;
	bool oybilrraidfnmb = false;
	bool xibko = false;
	if (false != false) {
		int eyxxqv;
		for (eyxxqv = 81; eyxxqv > 0; eyxxqv--) {
			continue;
		}
	}
	return true;
}

string kqjnoyv::bmawmmawsljyqtyzygwatm(double yamqttbw, int crmsgqfmome, string hawmolgvuowclra) {
	double jvqvf = 54106;
	double rsbhhduyvwjrmpn = 20703;
	bool cnqtvw = true;
	int whtjlozfec = 1626;
	string wphoxq = "";
	int supovjvsgzc = 817;
	int clydlunjn = 24;
	if (817 != 817) {
		int gkrqlljmpt;
		for (gkrqlljmpt = 6; gkrqlljmpt > 0; gkrqlljmpt--) {
			continue;
		}
	}
	if (1626 != 1626) {
		int jnpje;
		for (jnpje = 94; jnpje > 0; jnpje--) {
			continue;
		}
	}
	if (24 == 24) {
		int wp;
		for (wp = 65; wp > 0; wp--) {
			continue;
		}
	}
	if (1626 != 1626) {
		int hsozyzxyzz;
		for (hsozyzxyzz = 2; hsozyzxyzz > 0; hsozyzxyzz--) {
			continue;
		}
	}
	if (true == true) {
		int zpqq;
		for (zpqq = 98; zpqq > 0; zpqq--) {
			continue;
		}
	}
	return string("yhwtkalybrhstfz");
}

void kqjnoyv::ghzbqfxzzuwdmia(int mhzmsxeisgbxr, string bcjnxk, int ckgft, int ogsrjpxntieji, double pzfyrebeppx) {
	double chxcepfgkwuysdx = 33790;
	bool hrvcay = false;
	double jxehvmn = 12354;
	string dwwpyhjf = "ogjnlaufsoaosqavhidtjvuyudaqhzjtmhkcovxjszoxj";
	double sflfztlhjr = 2101;
	if (33790 == 33790) {
		int eofggfcyqw;
		for (eofggfcyqw = 61; eofggfcyqw > 0; eofggfcyqw--) {
			continue;
		}
	}
	if (2101 == 2101) {
		int mku;
		for (mku = 78; mku > 0; mku--) {
			continue;
		}
	}
	if (2101 == 2101) {
		int odpeevo;
		for (odpeevo = 26; odpeevo > 0; odpeevo--) {
			continue;
		}
	}

}

string kqjnoyv::zjvzcdfaacwrmxxyqmnilrwo(bool jecjnnzkbv, int nqlrxrwgldcxh, double cqlllii) {
	int tpmfgjq = 5166;
	double lrlszl = 4033;
	double ktiwhxkfnpo = 49948;
	int oklukamokbhfvys = 60;
	int fpktlsllpchpc = 5023;
	double lrwqatq = 31421;
	int nigbfqyoedkp = 2056;
	string fwyxz = "jpgpzhyzgvmrbzxdnvzlmjoavjugniiyzifknrlnavoyzwtsqwsj";
	double ccqioe = 10749;
	if (49948 == 49948) {
		int czvwi;
		for (czvwi = 74; czvwi > 0; czvwi--) {
			continue;
		}
	}
	if (60 == 60) {
		int eawmr;
		for (eawmr = 78; eawmr > 0; eawmr--) {
			continue;
		}
	}
	if (60 != 60) {
		int dtzfbfs;
		for (dtzfbfs = 54; dtzfbfs > 0; dtzfbfs--) {
			continue;
		}
	}
	if (49948 != 49948) {
		int cogpgzc;
		for (cogpgzc = 60; cogpgzc > 0; cogpgzc--) {
			continue;
		}
	}
	if (31421 == 31421) {
		int amdd;
		for (amdd = 51; amdd > 0; amdd--) {
			continue;
		}
	}
	return string("amzlklhihnaewj");
}

int kqjnoyv::donzispngrsvpnoyt(int cxvmstquavva, int djvatvvqm, int jvgkrujgk, string zmnngy, double ealkpjjt, string ligykzoqktrj, bool lwfdajolmjjzoe) {
	double ypodpu = 11182;
	int dhtjf = 356;
	bool pyuqlhxgskjfgpy = true;
	string pdxnzfgd = "jbqcvvcykxzq";
	return 81292;
}

double kqjnoyv::pqhrzfnuynndsgclueex(int emrvnayoiytaozn, int zvdsqnasmurmsn) {
	double yitsawjelzanuu = 33769;
	int ujsbkswhm = 6879;
	string xyuhknzkkadtjkb = "ccltpkpoafajvlhivvlrwbhisyrfxywkojvttaasjqayijdohuhoujnpjzwkalywvxfrlicanzwrsvzdagcysoupsy";
	int kxjvnr = 4747;
	return 47400;
}

void kqjnoyv::fudjnuyosmpap() {
	double lykfjfrgkv = 10695;
	int sowew = 4686;
	string mhyfpbmaspart = "lqvzvixkvzfoakoqpkyxceqbhipmumzzhkqptenrurvyhleeizflybgpwityeoeyivetqhjhivsnhxujlambqrrclfmeamgainxf";
	int fsajtm = 8196;
	string uhwublwqgyaakm = "dryutwjcvbwbfgjjmhmercqksrllfqeadyrppjhygylxjlseguulkikiceyqgxbzuavfrrcblkufspkc";
	if (8196 == 8196) {
		int cmvaw;
		for (cmvaw = 15; cmvaw > 0; cmvaw--) {
			continue;
		}
	}
	if (8196 != 8196) {
		int csbk;
		for (csbk = 72; csbk > 0; csbk--) {
			continue;
		}
	}

}

int kqjnoyv::tkcvazykwkzffkm(bool wswuyvclhlcuusz, string mhzfyqelvfe, string gbcenmapgdrs) {
	string gcyjhtyadk = "qarpocwgweasjibfyofdntdjcpjcpjuclwgexpsdnsitxilhccqdzytmtzojlssxxgmhsyeid";
	string ddmivo = "dzlotjofuhz";
	double zopbnocxx = 28073;
	string dcnizhhjtgrw = "atazrrxqvwyozzpknnlpylepkkmdfnkfoql";
	if (string("atazrrxqvwyozzpknnlpylepkkmdfnkfoql") != string("atazrrxqvwyozzpknnlpylepkkmdfnkfoql")) {
		int iottpc;
		for (iottpc = 20; iottpc > 0; iottpc--) {
			continue;
		}
	}
	if (string("qarpocwgweasjibfyofdntdjcpjcpjuclwgexpsdnsitxilhccqdzytmtzojlssxxgmhsyeid") == string("qarpocwgweasjibfyofdntdjcpjcpjuclwgexpsdnsitxilhccqdzytmtzojlssxxgmhsyeid")) {
		int aojrujrjpf;
		for (aojrujrjpf = 69; aojrujrjpf > 0; aojrujrjpf--) {
			continue;
		}
	}
	return 51333;
}

bool kqjnoyv::ojelbymjjgkyroqxvd(bool oohregecvl, int lluun, int ljgblmzxpnlb, bool wyeeyuiqbkrks, string wrqhclmqngqn, int qqhqnsls, int udjaowhhmqnid) {
	double ltylovjlpdqo = 33231;
	bool dtwrjb = true;
	double zfavjb = 21908;
	bool eieeoytladtjvvl = false;
	int yupyron = 959;
	double lyoogvjnrkzdwu = 5506;
	string wvktuyaaca = "wxnhraopqktzwjqqqtsbsgscbdmcwcjquprzrbvhelxrvdtkhruucgmqvmladkupbjm";
	if (true != true) {
		int mxhvcomshc;
		for (mxhvcomshc = 22; mxhvcomshc > 0; mxhvcomshc--) {
			continue;
		}
	}
	if (33231 != 33231) {
		int hbgj;
		for (hbgj = 79; hbgj > 0; hbgj--) {
			continue;
		}
	}
	return true;
}

void kqjnoyv::jmchnietilisptdjryvgpb(string qwwdxjhmx, int tpmkavxctketj, int zwoqdhjmrsaps, bool crwkmwym, int autii, bool wbqyprpircly, string vjlafxptcngrp) {
	int ibdxthrkd = 5049;
	int eihnegegxfdi = 7274;
	if (5049 == 5049) {
		int pms;
		for (pms = 4; pms > 0; pms--) {
			continue;
		}
	}
	if (5049 != 5049) {
		int qe;
		for (qe = 87; qe > 0; qe--) {
			continue;
		}
	}
	if (5049 == 5049) {
		int ku;
		for (ku = 37; ku > 0; ku--) {
			continue;
		}
	}

}

bool kqjnoyv::vjcoreuqbwvnlcsijzenayrsy(string jzqtwo, int uarhl) {
	return true;
}

double kqjnoyv::ouyqeubuepdh(int qmnltlejeanz, int updmesnroyasyvt, int eeolknuomlvco, string ooukph) {
	bool kwfzzskt = false;
	string qcezrwbhlkf = "xkmpwoxjivkjpptkcwmeqyhfywpalmqmcfmcnnzcceuallxebxltcliveomzuaikodvlcflcoxnaddeyervr";
	double chckwindbrtrs = 14358;
	double swxrrcoij = 11321;
	int jknjhu = 4259;
	bool wbviph = false;
	string epsbwfuyqp = "dmsfwdcuex";
	double lcqzu = 90636;
	double ppdprxgcxpceqy = 14991;
	string exazgyrktjar = "sqmapbrhawsq";
	if (4259 != 4259) {
		int xojknzrlvb;
		for (xojknzrlvb = 33; xojknzrlvb > 0; xojknzrlvb--) {
			continue;
		}
	}
	if (4259 != 4259) {
		int tsohiwnjah;
		for (tsohiwnjah = 52; tsohiwnjah > 0; tsohiwnjah--) {
			continue;
		}
	}
	if (false != false) {
		int ukkvpaxcm;
		for (ukkvpaxcm = 94; ukkvpaxcm > 0; ukkvpaxcm--) {
			continue;
		}
	}
	if (false != false) {
		int semhji;
		for (semhji = 30; semhji > 0; semhji--) {
			continue;
		}
	}
	return 19722;
}

kqjnoyv::kqjnoyv() {
	this->donzispngrsvpnoyt(1703, 5101, 324, string("msminkomlhkdhqdyjkzipdgpsuqsyysadbiookdwlevtuqalptagcytacd"), 62266, string("aqpoqmacquovdkxz"), true);
	this->pqhrzfnuynndsgclueex(4792, 5806);
	this->fudjnuyosmpap();
	this->tkcvazykwkzffkm(true, string("nwqlheajubzhesmymxsszvyjdlakjtpavuleahczcpctiesqqewkifsuptaxpk"), string("tsmckrfumvhyazbyeutcrirohjpxkxbkztnhqhvjvhkfpoulnllzloojelazouoljpmuyxfjdcwjwkimaf"));
	this->ojelbymjjgkyroqxvd(true, 155, 440, true, string("jlersummxrrlqjpdrixjmfxsxzdtkxqbljxcgfteafjkjwobrzkbpcfgryyxfiwwjypndcqsqcugbunobfrncsj"), 2010, 3710);
	this->jmchnietilisptdjryvgpb(string("ssnobixpozoybpuzrbunnokemetdmxdfpancltyaaauctzkgqdgvdrhyvnjqstsphscimkf"), 796, 6501, false, 1926, true, string("xeimrcguxkzuoktkomcgkhtensaderdkmnpaxvaikrywbqipgxpbnpjiiwzgonrdsraalyipvgohygjfypifn"));
	this->vjcoreuqbwvnlcsijzenayrsy(string("lqdmnkaeutxavjfhnawrptlrmwfbfkrjxoxccjedobamyvqxeiykfraxdqcvbvhfjnwtpzygrzsgpshsxljnpskufjjutig"), 1119);
	this->ouyqeubuepdh(208, 4981, 520, string("bfgglpfqtqghmtewtozfidahmkojdv"));
	this->ylsnhkiivgrlhwyradiniwj(true, true, true, false, true, 30085, string("yfhoqpafsdbqugyutsxgyfhjakohkepvtpwrdkyictswkpsnvjgrdldjqyuyhtqghzvvrclnnfjfiicivcrlmlzrtn"));
	this->vtswwlayvmxvv();
	this->qsjpxdjeweauadefcq(true, true, string("koxsjfqhqplemjvitiqmwjkfjfqofmubl"), true, string("ncabdgzkdlqhx"), 28496, 7604);
	this->xxljeanmbhmomrkdixzshjajt(string("qfskc"), false, string("zngavzojurakqskpvpjysvnadcurznckyczbfsiagmmbvmvtplkpofdgey"), 12827, string("wgpi"), 2298, 653);
	this->kjlajitrbcwsrbd(false, 21004, string("nfcustedsnmzlzxhei"), 35799, 13308);
	this->vqkqpfhmbhkaakecuitih(2454, string("nfnxnjkucsqnnmadxuamygw"), 3621, 5549, 2436, false, true, string("mxpauxmbfmamdirhvpvanevrogblkbdyxczzowdsetsoxfcqjlwwbzffsqmxrgviyioitbtpurwdfrohh"));
	this->gtprlmoojq();
	this->bmawmmawsljyqtyzygwatm(15357, 511, string("xctgvte"));
	this->ghzbqfxzzuwdmia(1045, string("rat"), 1034, 533, 18253);
	this->zjvzcdfaacwrmxxyqmnilrwo(false, 1738, 41117);
	this->nuhiealgstwwfgszlhloe(31800, string("vggpdyzaemibwexxotijzssfpzehselocfhpmcycifyaytxmjlyvgsiranxvowxyu"), 2935, 3344, 10663, true, 21644, false);
	this->eazxbocsvpwl(23323);
	this->tfktqotokdpdhlfq(string("znud"));
	this->nyaveqcsmkdhojelk(true, string("txxjnydjikgadzwsixowaczcywlbqmskbjruhitvtnflwqadlau"), string("cfyplnhsfkzucocfgdjykkrclmylnffzqbqg"), 2446, 7437, 1082, 63892, true);
	this->enehkvtlsilcrpgvlkevscle(11167, 1638, 7851, true, string("ixgcxhyfjgyposdovbdyzyhjojcenigtyuwexhhdzimaczyfnficeccmsenuplpwqriwijzzkwjmmo"), string("rhlfexhzyi"), string("aegmyqiobzljrunqigpfxnyqqvjuiwyizstorqpzuzt"), string("wuxazpinfs"), 3841, 864);
	this->taftbekzoccnlyfpfied(true, 6479);
	this->grgllvidgfd(false, true, 80, string("yjkxukzjwubennanlodiryzdxtztdlsbpxjghzxwvmawsspbyhflfdewxxoqqtbzhfbbshmxjsal"));
	this->yfywxqugwzruyry(true, false, 1344, true, 1252, true, 2523, true, 16209, 134);
	this->ginooirohslqomev(2495, string("lkjvnihheqkvyczaedgrznlhhbnxxglvgefqzaptrafijoiqwsuutv"), 18, false, 1671, 1202, 4679, 19451, 43521, 38889);
}
