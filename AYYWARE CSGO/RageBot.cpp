#include "RageBot.h"
#include "RenderManager.h"
#include "Resolver.h"
#include "Autowall.h"
#include <iostream>
#include "UTIL Functions.h"

void CRageBot::Init()
{
	IsAimStepping = false;
	IsLocked = false;
	TargetID = -1;
}

void CRageBot::Draw()
{

}

bool IsAbleToShoot(IClientEntity* pLocal)
{
	CBaseCombatWeapon* pWeapon = (CBaseCombatWeapon*)Interfaces::EntList->GetClientEntityFromHandle(pLocal->GetActiveWeaponHandle());

	if (!pLocal)
		return false;

	if (!pWeapon)
		return false;

	float flServerTime = pLocal->GetTickBase() * Interfaces::Globals->interval_per_tick;

	return (!(pWeapon->GetNextPrimaryAttack() > flServerTime));
}

float hitchance(IClientEntity* pLocal, CBaseCombatWeapon* pWeapon)
{
	float hitchance = 101;
	if (!pWeapon) return 0;
	if (Menu::Window.RageBotTab.AccuracyHitchance.GetValue() > 1)
	{
		float inaccuracy = pWeapon->GetInaccuracy();
		if (inaccuracy == 0) inaccuracy = 0.0000001;
		inaccuracy = 1 / inaccuracy;
		hitchance = inaccuracy;

	}
	return hitchance;
}

bool CanOpenFire()
{
	IClientEntity* pLocalEntity = (IClientEntity*)Interfaces::EntList->GetClientEntity(Interfaces::Engine->GetLocalPlayer());
	if (!pLocalEntity)
		return false;

	CBaseCombatWeapon* entwep = (CBaseCombatWeapon*)Interfaces::EntList->GetClientEntityFromHandle(pLocalEntity->GetActiveWeaponHandle());

	float flServerTime = (float)pLocalEntity->GetTickBase() * Interfaces::Globals->interval_per_tick;
	float flNextPrimaryAttack = entwep->GetNextPrimaryAttack();

	std::cout << flServerTime << " " << flNextPrimaryAttack << std::endl;

	return !(flNextPrimaryAttack > flServerTime);
}

void CRageBot::Move(CUserCmd *pCmd, bool &bSendPacket)
{
	IClientEntity* pLocalEntity = (IClientEntity*)Interfaces::EntList->GetClientEntity(Interfaces::Engine->GetLocalPlayer());
	if (!pLocalEntity)
		return;

	if (!Menu::Window.RageBotTab.Active.GetState())
		return;

	if (Menu::Window.RageBotTab.AntiAimEnable.GetState())
	{
		static int ChokedPackets = -1;

		CBaseCombatWeapon* pWeapon = (CBaseCombatWeapon*)Interfaces::EntList->GetClientEntityFromHandle(hackManager.pLocal()->GetActiveWeaponHandle());
		if (!pWeapon)
			return;

		if (ChokedPackets < 1 && pLocalEntity->GetLifeState() == LIFE_ALIVE && pCmd->buttons & IN_ATTACK && CanOpenFire() && GameUtils::IsBallisticWeapon(pWeapon))
		{
			bSendPacket = false;
		}
		else
		{
			if (pLocalEntity->GetLifeState() == LIFE_ALIVE)
			{
				DoAntiAim(pCmd, bSendPacket);
			}
			ChokedPackets = -1;
		}
	}

	if (Menu::Window.RageBotTab.AimbotEnable.GetState())
		DoAimbot(pCmd, bSendPacket);

	if (Menu::Window.RageBotTab.AccuracyRecoil.GetState())
		DoNoRecoil(pCmd);

	if (Menu::Window.RageBotTab.AimbotAimStep.GetState())
	{
		Vector AddAngs = pCmd->viewangles - LastAngle;
		if (AddAngs.Length2D() > 25.f)
		{
			Normalize(AddAngs, AddAngs);
			AddAngs *= 25;
			pCmd->viewangles = LastAngle + AddAngs;
			GameUtils::NormaliseViewAngle(pCmd->viewangles);
		}
	}

	LastAngle = pCmd->viewangles;
}

Vector BestPoint(IClientEntity *targetPlayer, Vector &final)
{
	IClientEntity* pLocal = hackManager.pLocal();

	trace_t tr;
	Ray_t ray;
	CTraceFilter filter;

	filter.pSkip = targetPlayer;
	ray.Init(final + Vector(0, 0, 10), final);
	Interfaces::Trace->TraceRay(ray, MASK_SHOT, &filter, &tr);

	final = tr.endpos;
	return final;
}

void CRageBot::DoAimbot(CUserCmd *pCmd, bool &bSendPacket)
{
	IClientEntity* pTarget = nullptr;
	IClientEntity* pLocal = hackManager.pLocal();
	Vector Start = pLocal->GetViewOffset() + pLocal->GetOrigin();
	bool FindNewTarget = true;

	CSWeaponInfo* weapInfo = ((CBaseCombatWeapon*)Interfaces::EntList->GetClientEntityFromHandle(pLocal->GetActiveWeaponHandle()))->GetCSWpnData();

	CBaseCombatWeapon* pWeapon = (CBaseCombatWeapon*)Interfaces::EntList->GetClientEntityFromHandle(pLocal->GetActiveWeaponHandle());
	if (pWeapon)
	{
		if (pWeapon->GetAmmoInClip() == 0 || !GameUtils::IsBallisticWeapon(pWeapon))
		{
			return;
		}
	}
	else
		return;

	if (IsLocked && TargetID >= 0 && HitBox >= 0)
	{
		pTarget = Interfaces::EntList->GetClientEntity(TargetID);
		if (pTarget  && TargetMeetsRequirements(pTarget))
		{
			HitBox = HitScan(pTarget);
			if (HitBox >= 0)
			{
				Vector ViewOffset = pLocal->GetOrigin() + pLocal->GetViewOffset();
				Vector View;
				Interfaces::Engine->GetViewAngles(View);
				float FoV = FovToPlayer(ViewOffset, View, pTarget, HitBox);
				if (FoV < Menu::Window.RageBotTab.AimbotFov.GetValue())
					FindNewTarget = false;
			}
		}
	}

	if (GameUtils::IsRevolver(pWeapon) && Menu::Window.RageBotTab.AutoRevolver.GetState())
	{
		static int delay = 0;
		delay++;

		if (delay <= 15)
			pCmd->buttons |= IN_ATTACK;
		else
			delay = 0;
	}

	if (FindNewTarget)
	{
		TargetID = 0;
		pTarget = nullptr;
		HitBox = -1;

		switch (Menu::Window.RageBotTab.TargetSelection.GetIndex())
		{
		case 0:
			TargetID = GetTargetCrosshair();
			break;
		case 1:
			TargetID = GetTargetDistance();
			break;
		case 2:
			TargetID = GetTargetHealth();
			break;
		case 3:
			TargetID = GetTargetThreat(pCmd);
			break;
		case 4:
			TargetID = GetTargetNextShot();
			break;
		}

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

	Globals::Target = pTarget;
	Globals::TargetID = TargetID;

	if (TargetID >= 0 && pTarget)
	{
		HitBox = HitScan(pTarget);

		if (!CanOpenFire())
			return;

		if (Menu::Window.RageBotTab.AimbotKeyPress.GetState())
		{
			int Key = Menu::Window.RageBotTab.AimbotKeyBind.GetKey();
			if (Key >= 0 && !GUI.GetKeyState(Key))
			{
				TargetID = -1;
				pTarget = nullptr;
				HitBox = -1;
				return;
			}
		}

		int StopKey = Menu::Window.RageBotTab.AimbotStopKey.GetKey();
		if (StopKey >= 0 && GUI.GetKeyState(StopKey))
		{
			TargetID = -1;
			pTarget = nullptr;
			HitBox = -1;
			return;
		}

		float pointscale = Menu::Window.RageBotTab.TargetPointscale.GetValue() - 5.f;

		Vector Point;
		Vector AimPoint = GetHitboxPosition(pTarget, HitBox) + Vector(0, 0, pointscale);

		if (Menu::Window.RageBotTab.TargetMultipoint.GetState())
		{
			Point = BestPoint(pTarget, AimPoint);
		}
		else
		{
			Point = AimPoint;
		}

		if (GameUtils::IsScopedWeapon(pWeapon) && !pWeapon->IsScoped() && Menu::Window.RageBotTab.AccuracyAutoScope.GetState()) // Autoscope
		{
			pCmd->buttons |= IN_ATTACK2;
		}
		else
		{
			if ((Menu::Window.RageBotTab.AccuracyHitchance.GetValue() * 1.5 <= hitchance(pLocal, pWeapon)) || Menu::Window.RageBotTab.AccuracyHitchance.GetValue() == 0 || *pWeapon->m_AttributeManager()->m_Item()->ItemDefinitionIndex() == 64)
			{
				if (AimAtPoint(pLocal, Point, pCmd, bSendPacket))
				{
					if (Menu::Window.RageBotTab.AimbotAutoFire.GetState() && !(pCmd->buttons & IN_ATTACK))
					{
						pCmd->buttons |= IN_ATTACK;
					}
					else
					{
						return;
					}
				}
				else if (Menu::Window.RageBotTab.AimbotAutoFire.GetState() && !(pCmd->buttons & IN_ATTACK))
				{
					pCmd->buttons |= IN_ATTACK;
				}
			}
		}

		if (IsAbleToShoot(pLocal) && pCmd->buttons & IN_ATTACK)
			Globals::Shots += 1;
	}
}

bool CRageBot::TargetMeetsRequirements(IClientEntity* pEntity)
{
	if (pEntity && pEntity->IsDormant() == false && pEntity->IsAlive() && pEntity->GetIndex() != hackManager.pLocal()->GetIndex())
	{
		ClientClass *pClientClass = pEntity->GetClientClass();
		player_info_t pinfo;
		if (pClientClass->m_ClassID == (int)CSGOClassID::CCSPlayer && Interfaces::Engine->GetPlayerInfo(pEntity->GetIndex(), &pinfo))
		{
			if (pEntity->GetTeamNum() != hackManager.pLocal()->GetTeamNum() || Menu::Window.RageBotTab.TargetFriendlyFire.GetState())
			{
				if (!pEntity->HasGunGameImmunity())
				{
					return true;
				}
			}
		}
	}

	return false;
}

float CRageBot::FovToPlayer(Vector ViewOffSet, Vector View, IClientEntity* pEntity, int aHitBox)
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

	FLOAT DotProduct = Forward.Dot(Delta);

	return (acos(DotProduct) * (MaxDegrees / PI));
}

int CRageBot::GetTargetCrosshair()
{
	int target = -1;
	float minFoV = Menu::Window.RageBotTab.AimbotFov.GetValue();

	IClientEntity* pLocal = hackManager.pLocal();
	Vector ViewOffset = pLocal->GetOrigin() + pLocal->GetViewOffset();
	Vector View; Interfaces::Engine->GetViewAngles(View);

	for (int i = 0; i < Interfaces::EntList->GetMaxEntities(); i++)
	{
		IClientEntity *pEntity = Interfaces::EntList->GetClientEntity(i);
		if (TargetMeetsRequirements(pEntity))
		{
			int NewHitBox = HitScan(pEntity);
			if (NewHitBox >= 0)
			{
				float fov = FovToPlayer(ViewOffset, View, pEntity, 0);
				if (fov < minFoV)
				{
					minFoV = fov;
					target = i;
				}
			}
		}
	}

	return target;
}

int CRageBot::GetTargetDistance()
{
	int target = -1;
	int minDist = 99999;

	IClientEntity* pLocal = hackManager.pLocal();
	Vector ViewOffset = pLocal->GetOrigin() + pLocal->GetViewOffset();
	Vector View; Interfaces::Engine->GetViewAngles(View);

	for (int i = 0; i < Interfaces::EntList->GetMaxEntities(); i++)
	{
		IClientEntity *pEntity = Interfaces::EntList->GetClientEntity(i);
		if (TargetMeetsRequirements(pEntity))
		{
			int NewHitBox = HitScan(pEntity);
			if (NewHitBox >= 0)
			{
				Vector Difference = pLocal->GetOrigin() - pEntity->GetOrigin();
				int Distance = Difference.Length();
				float fov = FovToPlayer(ViewOffset, View, pEntity, 0);
				if (Distance < minDist && fov < Menu::Window.RageBotTab.AimbotFov.GetValue())
				{
					minDist = Distance;
					target = i;
				}
			}
		}
	}

	return target;
}

int CRageBot::GetTargetNextShot()
{
	int target = -1;
	int minfov = 361;

	IClientEntity* pLocal = hackManager.pLocal();
	Vector ViewOffset = pLocal->GetOrigin() + pLocal->GetViewOffset();
	Vector View; Interfaces::Engine->GetViewAngles(View);

	for (int i = 0; i < Interfaces::EntList->GetMaxEntities(); i++)
	{
		IClientEntity *pEntity = Interfaces::EntList->GetClientEntity(i);
		if (TargetMeetsRequirements(pEntity))
		{
			int NewHitBox = HitScan(pEntity);
			if (NewHitBox >= 0)
			{
				int Health = pEntity->GetHealth();
				float fov = FovToPlayer(ViewOffset, View, pEntity, 0);
				if (fov < minfov && fov < Menu::Window.RageBotTab.AimbotFov.GetValue())
				{
					minfov = fov;
					target = i;
				}
				else
					minfov = 361;
			}
		}
	}

	return target;
}

float GetFov(const QAngle& viewAngle, const QAngle& aimAngle)
{
	Vector ang, aim;

	AngleVectors(viewAngle, &aim);
	AngleVectors(aimAngle, &ang);

	return RAD2DEG(acos(aim.Dot(ang) / aim.LengthSqr()));
}

double inline __declspec (naked) __fastcall FASTSQRT(double n)
{
	_asm fld qword ptr[esp + 4]
		_asm fsqrt
	_asm ret 8
}

float VectorDistance(Vector v1, Vector v2)
{
	return FASTSQRT(pow(v1.x - v2.x, 2) + pow(v1.y - v2.y, 2) + pow(v1.z - v2.z, 2));
}

int CRageBot::GetTargetThreat(CUserCmd* pCmd)
{
	auto iBestTarget = -1;
	float flDistance = 8192.f;

	IClientEntity* pLocal = hackManager.pLocal();

	for (int i = 0; i < Interfaces::EntList->GetHighestEntityIndex(); i++)
	{
		IClientEntity *pEntity = Interfaces::EntList->GetClientEntity(i);
		if (TargetMeetsRequirements(pEntity))
		{
			int NewHitBox = HitScan(pEntity);
			auto vecHitbox = pEntity->GetBonePos(NewHitBox);
			if (NewHitBox >= 0)
			{
				Vector Difference = pLocal->GetOrigin() - pEntity->GetOrigin();
				QAngle TempTargetAbs;
				CalcAngle(pLocal->GetEyePosition(), vecHitbox, TempTargetAbs);
				float flTempFOVs = GetFov(pCmd->viewangles, TempTargetAbs);
				float flTempDistance = VectorDistance(pLocal->GetOrigin(), pEntity->GetOrigin());
				if (flTempDistance < flDistance && flTempFOVs < Menu::Window.RageBotTab.AimbotFov.GetValue())
				{
					flDistance = flTempDistance;
					iBestTarget = i;
				}
			}
		}
	}
	return iBestTarget;
}

int CRageBot::GetTargetHealth()
{
	int target = -1;
	int minHealth = 101;

	IClientEntity* pLocal = hackManager.pLocal();
	Vector ViewOffset = pLocal->GetOrigin() + pLocal->GetViewOffset();
	Vector View; Interfaces::Engine->GetViewAngles(View);

	for (int i = 0; i < Interfaces::EntList->GetMaxEntities(); i++)
	{
		IClientEntity *pEntity = Interfaces::EntList->GetClientEntity(i);
		if (TargetMeetsRequirements(pEntity))
		{
			int NewHitBox = HitScan(pEntity);
			if (NewHitBox >= 0)
			{
				int Health = pEntity->GetHealth();
				float fov = FovToPlayer(ViewOffset, View, pEntity, 0);
				if (Health < minHealth && fov < Menu::Window.RageBotTab.AimbotFov.GetValue())
				{
					minHealth = Health;
					target = i;
				}
			}
		}
	}

	return target;
}

int CRageBot::HitScan(IClientEntity* pEntity)
{
	IClientEntity* pLocal = hackManager.pLocal();
	std::vector<int> HitBoxesToScan;

#pragma region GetHitboxesToScan
	int HitScanMode = Menu::Window.RageBotTab.TargetHitscan.GetIndex();
	int huso = (pEntity->GetHealth());
	int health = Menu::Window.RageBotTab.BaimIfUnderXHealth.GetValue();
	bool AWall = Menu::Window.RageBotTab.AccuracyAutoWall.GetState();
	bool Multipoint = Menu::Window.RageBotTab.TargetMultipoint.GetState();
	int TargetHitbox = Menu::Window.RageBotTab.TargetHitbox.GetIndex();
	CBaseCombatWeapon* pWeapon = (CBaseCombatWeapon*)Interfaces::EntList->GetClientEntityFromHandle(hackManager.pLocal()->GetActiveWeaponHandle());

	if (huso < health)
	{
		HitBoxesToScan.push_back((int)CSGOHitboxID::Pelvis);
		HitBoxesToScan.push_back((int)CSGOHitboxID::Stomach);
		HitBoxesToScan.push_back((int)CSGOHitboxID::Chest);
		HitBoxesToScan.push_back((int)CSGOHitboxID::UpperChest);
		HitBoxesToScan.push_back((int)CSGOHitboxID::LowerChest);
		HitBoxesToScan.push_back((int)CSGOHitboxID::LeftUpperArm);
		HitBoxesToScan.push_back((int)CSGOHitboxID::RightUpperArm);
		HitBoxesToScan.push_back((int)CSGOHitboxID::LeftThigh);
		HitBoxesToScan.push_back((int)CSGOHitboxID::RightThigh);
		HitBoxesToScan.push_back((int)CSGOHitboxID::LeftHand);
		HitBoxesToScan.push_back((int)CSGOHitboxID::RightHand);
		HitBoxesToScan.push_back((int)CSGOHitboxID::LeftFoot);
		HitBoxesToScan.push_back((int)CSGOHitboxID::RightFoot);
		HitBoxesToScan.push_back((int)CSGOHitboxID::LeftShin);
		HitBoxesToScan.push_back((int)CSGOHitboxID::RightShin);
		HitBoxesToScan.push_back((int)CSGOHitboxID::LeftLowerArm);
		HitBoxesToScan.push_back((int)CSGOHitboxID::RightLowerArm);
	}
	else
	{
		if (HitScanMode == 0)
		{
			switch (Menu::Window.RageBotTab.TargetHitbox.GetIndex())
			{
			case 0:
				HitBoxesToScan.push_back((int)CSGOHitboxID::Head);
				break;
			case 1:
				HitBoxesToScan.push_back((int)CSGOHitboxID::Neck);
				HitBoxesToScan.push_back((int)CSGOHitboxID::NeckLower);
				break;
			case 2:
				HitBoxesToScan.push_back((int)CSGOHitboxID::UpperChest);
				HitBoxesToScan.push_back((int)CSGOHitboxID::Chest);
				HitBoxesToScan.push_back((int)CSGOHitboxID::LowerChest);
				break;
			case 3:
				HitBoxesToScan.push_back((int)CSGOHitboxID::Stomach);
				HitBoxesToScan.push_back((int)CSGOHitboxID::Pelvis);
				break;
			case 4:
				HitBoxesToScan.push_back((int)CSGOHitboxID::LeftFoot);
				HitBoxesToScan.push_back((int)CSGOHitboxID::RightFoot);
				break;
			}
		}

		else if (Menu::Window.RageBotTab.PreferBodyAim.GetState())
		{
			HitBoxesToScan.push_back((int)CSGOHitboxID::UpperChest);
			HitBoxesToScan.push_back((int)CSGOHitboxID::Chest);
			HitBoxesToScan.push_back((int)CSGOHitboxID::LowerChest);
			HitBoxesToScan.push_back((int)CSGOHitboxID::Head);
			HitBoxesToScan.push_back((int)CSGOHitboxID::Stomach);
			HitBoxesToScan.push_back((int)CSGOHitboxID::Pelvis);
			HitBoxesToScan.push_back((int)CSGOHitboxID::LeftUpperArm);
			HitBoxesToScan.push_back((int)CSGOHitboxID::LeftLowerArm);
			HitBoxesToScan.push_back((int)CSGOHitboxID::RightUpperArm);
			HitBoxesToScan.push_back((int)CSGOHitboxID::RightLowerArm);
			HitBoxesToScan.push_back((int)CSGOHitboxID::LeftThigh);
			HitBoxesToScan.push_back((int)CSGOHitboxID::RightThigh);
			HitBoxesToScan.push_back((int)CSGOHitboxID::LeftShin);
			HitBoxesToScan.push_back((int)CSGOHitboxID::RightShin);
			HitBoxesToScan.push_back((int)CSGOHitboxID::RightFoot);
			HitBoxesToScan.push_back((int)CSGOHitboxID::LeftFoot);
			HitBoxesToScan.push_back((int)CSGOHitboxID::RightHand);
			HitBoxesToScan.push_back((int)CSGOHitboxID::LeftHand);
		}
		else
		{
			switch (HitScanMode)
			{
			case 1:
				HitBoxesToScan.push_back((int)CSGOHitboxID::Head);
				HitBoxesToScan.push_back((int)CSGOHitboxID::Neck);
				HitBoxesToScan.push_back((int)CSGOHitboxID::UpperChest);
				HitBoxesToScan.push_back((int)CSGOHitboxID::Chest);
				HitBoxesToScan.push_back((int)CSGOHitboxID::Stomach);
				HitBoxesToScan.push_back((int)CSGOHitboxID::Pelvis);
			case 2:
				HitBoxesToScan.push_back((int)CSGOHitboxID::Head);
				HitBoxesToScan.push_back((int)CSGOHitboxID::LeftUpperArm);
				HitBoxesToScan.push_back((int)CSGOHitboxID::RightUpperArm);
				HitBoxesToScan.push_back((int)CSGOHitboxID::Stomach);
				HitBoxesToScan.push_back((int)CSGOHitboxID::LeftFoot);
				HitBoxesToScan.push_back((int)CSGOHitboxID::RightFoot);
				HitBoxesToScan.push_back((int)CSGOHitboxID::UpperChest);
				HitBoxesToScan.push_back((int)CSGOHitboxID::Chest);
				HitBoxesToScan.push_back((int)CSGOHitboxID::Pelvis);
				HitBoxesToScan.push_back((int)CSGOHitboxID::LowerChest);
				HitBoxesToScan.push_back((int)CSGOHitboxID::LeftUpperArm);
				HitBoxesToScan.push_back((int)CSGOHitboxID::RightUpperArm);
				HitBoxesToScan.push_back((int)CSGOHitboxID::LeftFoot);
				HitBoxesToScan.push_back((int)CSGOHitboxID::RightFoot);
				HitBoxesToScan.push_back((int)CSGOHitboxID::LeftShin);
				HitBoxesToScan.push_back((int)CSGOHitboxID::RightShin);
			case 3:
				HitBoxesToScan.push_back((int)CSGOHitboxID::Head);
				HitBoxesToScan.push_back((int)CSGOHitboxID::NeckLower);
				HitBoxesToScan.push_back((int)CSGOHitboxID::RightFoot);
				HitBoxesToScan.push_back((int)CSGOHitboxID::LeftFoot);
				HitBoxesToScan.push_back((int)CSGOHitboxID::Pelvis);
				HitBoxesToScan.push_back((int)CSGOHitboxID::LowerChest);
				HitBoxesToScan.push_back((int)CSGOHitboxID::Stomach);
				HitBoxesToScan.push_back((int)CSGOHitboxID::Pelvis);
				HitBoxesToScan.push_back((int)CSGOHitboxID::LeftLowerArm);
				HitBoxesToScan.push_back((int)CSGOHitboxID::LeftUpperArm);
				HitBoxesToScan.push_back((int)CSGOHitboxID::RightLowerArm);
				HitBoxesToScan.push_back((int)CSGOHitboxID::RightUpperArm);
				HitBoxesToScan.push_back((int)CSGOHitboxID::LeftThigh);
				HitBoxesToScan.push_back((int)CSGOHitboxID::RightThigh);
				HitBoxesToScan.push_back((int)CSGOHitboxID::LeftShin);
				HitBoxesToScan.push_back((int)CSGOHitboxID::RightShin);
				HitBoxesToScan.push_back((int)CSGOHitboxID::Neck);
				HitBoxesToScan.push_back((int)CSGOHitboxID::UpperChest);
				HitBoxesToScan.push_back((int)CSGOHitboxID::RightHand);
				HitBoxesToScan.push_back((int)CSGOHitboxID::LeftHand);
			}
		}
	}
#pragma endregion Get the list of shit to scan

	for (auto HitBoxID : HitBoxesToScan)
	{
		if (AWall)
		{
			Vector Point = GetHitboxPosition(pEntity, HitBoxID);
			float Damage = 0.f;
			Color c = Color(255, 255, 255, 255);
			if (CanHit(Point, &Damage))
			{
				c = Color(0, 255, 0, 255);
				if (Damage >= Menu::Window.RageBotTab.AccuracyMinimumDamage.GetValue())
				{
					return HitBoxID;
				}
			}
		}
		else
		{
			if (GameUtils::IsVisible(hackManager.pLocal(), pEntity, HitBoxID))
				return HitBoxID;
		}
	}

	return -1;
}

void CRageBot::DoNoRecoil(CUserCmd *pCmd)
{
	IClientEntity* pLocal = hackManager.pLocal();
	if (pLocal)
	{
		Vector AimPunch = pLocal->localPlayerExclusive()->GetAimPunchAngle();
		if (AimPunch.Length2D() > 0 && AimPunch.Length2D() < 150)
		{
			pCmd->viewangles -= AimPunch * 2;
			GameUtils::NormaliseViewAngle(pCmd->viewangles);
		}
	}
}

void CRageBot::aimAtPlayer(CUserCmd *pCmd)
{
	IClientEntity* pLocal = hackManager.pLocal();

	CBaseCombatWeapon* pWeapon = (CBaseCombatWeapon*)Interfaces::EntList->GetClientEntityFromHandle(hackManager.pLocal()->GetActiveWeaponHandle());

	if (!pLocal || !pWeapon)
		return;

	Vector eye_position = pLocal->GetEyePosition();

	IClientEntity* target = nullptr;

	for (int i = 0; i < Interfaces::Engine->GetMaxClients(); i++)
	{
		IClientEntity *pEntity = Interfaces::EntList->GetClientEntity(i);
		if (TargetMeetsRequirements(pEntity))
		{
			if (Globals::TargetID != -1)
				target = Interfaces::EntList->GetClientEntity(Globals::TargetID);
			else
				target = pEntity;

			Vector target_position = target->GetEyePosition();

			float temp_dist = eye_position.DistTo(target_position);
		}
	}
}

bool CRageBot::AimAtPoint(IClientEntity* pLocal, Vector point, CUserCmd *pCmd, bool &bSendPacket)
{
	bool ReturnValue = false;

	if (point.Length() == 0) return ReturnValue;

	Vector angles;
	Vector src = pLocal->GetOrigin() + pLocal->GetViewOffset();

	CalcAngle(src, point, angles);
	GameUtils::NormaliseViewAngle(angles);

	if (angles[0] != angles[0] || angles[1] != angles[1])
	{
		return ReturnValue;
	}


	IsLocked = true;

	Vector ViewOffset = pLocal->GetOrigin() + pLocal->GetViewOffset();
	if (!IsAimStepping)
		LastAimstepAngle = LastAngle;

	float fovLeft = FovToPlayer(ViewOffset, LastAimstepAngle, Interfaces::EntList->GetClientEntity(TargetID), 0);

	if (fovLeft > 25.0f && Menu::Window.RageBotTab.AimbotAimStep.GetState())
	{
		Vector AddAngs = angles - LastAimstepAngle;
		Normalize(AddAngs, AddAngs);
		AddAngs *= 25;
		LastAimstepAngle += AddAngs;
		GameUtils::NormaliseViewAngle(LastAimstepAngle);
		angles = LastAimstepAngle;
	}
	else
	{
		ReturnValue = true;
	}

	if (Menu::Window.RageBotTab.AimbotSilentAim.GetState())
	{
		pCmd->viewangles = angles;
	}

	if (!Menu::Window.RageBotTab.AimbotSilentAim.GetState())
	{
		Interfaces::Engine->SetViewAngles(angles);
	}

	return ReturnValue;
}

namespace AntiAims
{

	void JitterPitch(CUserCmd *pCmd)
	{
		static bool up = true;
		if (up)
		{
			pCmd->viewangles.x = 45;
			up = !up;
		}
		else
		{
			pCmd->viewangles.x = 89;
			up = !up;
		}
	}

	void kiduaJitter(CUserCmd *pCmd, bool &bSendPacket)
	{
		static int jitterangle;
		static int iChoked = -1;
		iChoked++;
		bSendPacket = true;
		if (jitterangle <= 1)
		{
			pCmd->viewangles.y += 90;
			jitterangle += 1;
		}
		else if (jitterangle > 1 && jitterangle <= 3)
		{
			pCmd->viewangles.y += 90;
			jitterangle += 1;
		}
		else
		{
			jitterangle = -90;
		}
	}

	void FakePitch(CUserCmd *pCmd, bool &bSendPacket)
	{
		static int ChokedPackets = -1;
		ChokedPackets++;
		if (ChokedPackets < 1)
		{
			bSendPacket = false;
			pCmd->viewangles.x = 89;
		}
		else
		{
			bSendPacket = true;
			pCmd->viewangles.x = 51;
			ChokedPackets = -1;
		}
	}

	void StaticJitter(CUserCmd *pCmd)
	{
		static bool down = true;
		if (down)
		{
			pCmd->viewangles.x = 179.0f;
			down = !down;
		}
		else
		{
			pCmd->viewangles.x = 89.0f;
			down = !down;
		}
	}

	// Yaws

	void FastSpin(CUserCmd *pCmd)
	{
		static int y2 = -179;
		int spinBotSpeedFast = 100;

		y2 += spinBotSpeedFast;

		if (y2 >= 179)
			y2 = -179;

		pCmd->viewangles.y = y2;
	}

	void FakeEdge(CUserCmd *pCmd, bool &bSendPacket)
	{
		IClientEntity* pLocal = hackManager.pLocal();

		Vector vEyePos = pLocal->GetOrigin() + pLocal->GetViewOffset();

		CTraceFilter filter;
		filter.pSkip = Interfaces::EntList->GetClientEntity(Interfaces::Engine->GetLocalPlayer());

		for (int y = 0; y < 360; y++)
		{
			Vector qTmp(10.0f, pCmd->viewangles.y, 0.0f);
			qTmp.y += y;

			if (qTmp.y > 180.0)
				qTmp.y -= 360.0;
			else if (qTmp.y < -180.0)
				qTmp.y += 360.0;

			GameUtils::NormaliseViewAngle(qTmp);

			Vector vForward;

			VectorAngles(qTmp, vForward);

			float fLength = (19.0f + (19.0f * sinf(DEG2RAD(10.0f)))) + 7.0f;
			vForward *= fLength;

			trace_t tr;

			Vector vTraceEnd = vEyePos + vForward;

			Ray_t ray;

			ray.Init(vEyePos, vTraceEnd);
			Interfaces::Trace->TraceRay(ray, MASK_PLAYERSOLID_BRUSHONLY, &filter, &tr);

			if (tr.fraction != 1.0f)
			{
				Vector angles;

				Vector vNegative = Vector(tr.plane.normal.x * -1.0f, tr.plane.normal.y * -1.0f, tr.plane.normal.z * -1.0f);

				VectorAngles(vNegative, angles);

				GameUtils::NormaliseViewAngle(angles);

				qTmp.y = angles.y;

				GameUtils::NormaliseViewAngle(qTmp);

				trace_t trLeft, trRight;

				Vector vLeft, vRight;
				VectorAngles(qTmp + Vector(0.0f, 30.0f, 0.0f), vLeft);
				VectorAngles(qTmp + Vector(0.0f, 30.0f, 0.0f), vRight);

				vLeft *= (fLength + (fLength * sinf(DEG2RAD(30.0f))));
				vRight *= (fLength + (fLength * sinf(DEG2RAD(30.0f))));

				vTraceEnd = vEyePos + vLeft;

				ray.Init(vEyePos, vTraceEnd);
				Interfaces::Trace->TraceRay(ray, MASK_PLAYERSOLID_BRUSHONLY, &filter, &trLeft);

				vTraceEnd = vEyePos + vRight;

				ray.Init(vEyePos, vTraceEnd);
				Interfaces::Trace->TraceRay(ray, MASK_PLAYERSOLID_BRUSHONLY, &filter, &trRight);

				if ((trLeft.fraction == 1.0f) && (trRight.fraction != 1.0f))
					qTmp.y -= 90.f;
				else if ((trLeft.fraction != 1.0f) && (trRight.fraction == 1.0f))
					qTmp.y += 90.f;

				if (qTmp.y > 180.0)
					qTmp.y -= 360.0;
				else if (qTmp.y < -180.0)
					qTmp.y += 360.0;

				pCmd->viewangles.y = qTmp.y;

				int offset = Menu::Window.RageBotTab.AntiAimOffset.GetValue();

				static int ChokedPackets = -1;
				ChokedPackets++;
				if (ChokedPackets < 1)
				{
					bSendPacket = false; // +=180?
				}
				else
				{
					bSendPacket = true;
					pCmd->viewangles.y -= offset;
					ChokedPackets = -1;
				}
				return;
			}
		}
		pCmd->viewangles.y += 360.0f;
	}

	void BackJitter(CUserCmd *pCmd)
	{
		int random = rand() % 100;

		if (random < 98)

			pCmd->viewangles.y -= 180;

		if (random < 15)
		{
			float change = -80 + (rand() % (int)(140 + 1));
			pCmd->viewangles.y += change;
		}
		if (random == 69)
		{
			float change = -90 + (rand() % (int)(180 + 1));
			pCmd->viewangles.y += change;
		}
	}

	void FakeSideways(CUserCmd *pCmd, bool &bSendPacket)
	{
		static int ChokedPackets = -1;
		ChokedPackets++;
		if (ChokedPackets < 1)
		{
			bSendPacket = false;
			pCmd->viewangles.y += 90;
		}
		else
		{
			bSendPacket = true;
			pCmd->viewangles.y -= 180;
			ChokedPackets = -1;
		}
	}

	void FastSpint(CUserCmd *pCmd)
	{
		int r1 = rand() % 100;
		int r2 = rand() % 1000;

		static bool dir;
		static float current_y = pCmd->viewangles.y;

		if (r1 == 1) dir = !dir;

		if (dir)
			current_y += 15 + rand() % 10;
		else
			current_y -= 15 + rand() % 10;

		pCmd->viewangles.y = current_y;

		if (r1 == r2)
			pCmd->viewangles.y += r1;
	}

	void BackwardJitter(CUserCmd *pCmd)
	{
		int random = rand() % 100;

		if (random < 98)

			pCmd->viewangles.y -= 180;

		if (random < 15)
		{
			float change = -70 + (rand() % (int)(140 + 1));
			pCmd->viewangles.y += change;
		}
		if (random == 69)
		{
			float change = -90 + (rand() % (int)(180 + 1));
			pCmd->viewangles.y += change;
		}
	}

	void Jitter(CUserCmd *pCmd)
	{
		static int jitterangle = 0;

		if (jitterangle <= 1)
		{
			pCmd->viewangles.y += 90;
		}
		else if (jitterangle > 1 && jitterangle <= 3)
		{
			pCmd->viewangles.y -= 90;
		}

		int re = rand() % 4 + 1;


		if (jitterangle <= 1)
		{
			if (re == 4)
				pCmd->viewangles.y += 180;
			jitterangle += 1;
		}
		else if (jitterangle > 1 && jitterangle <= 3)
		{
			if (re == 4)
				pCmd->viewangles.y -= 180;
			jitterangle += 1;
		}
		else
		{
			jitterangle = 0;
		}
	}

	void FakeStatic(CUserCmd *pCmd, bool &bSendPacket)
	{
		static int ChokedPackets = -1;
		ChokedPackets++;
		if (ChokedPackets < 1)
		{
			bSendPacket = false;
			static int y2 = -179;
			int spinBotSpeedFast = 360.0f / 1.618033988749895f;;

			y2 += spinBotSpeedFast;

			if (y2 >= 179)
				y2 = -179;

			pCmd->viewangles.y = y2;
		}
		else
		{
			bSendPacket = true;
			pCmd->viewangles.y -= 180;
			ChokedPackets = -1;
		}
	}

	void TJitter(CUserCmd *pCmd)
	{
		static bool Turbo = true;
		if (Turbo)
		{
			pCmd->viewangles.y -= 90;
			Turbo = !Turbo;
		}
		else
		{
			pCmd->viewangles.y += 90;
			Turbo = !Turbo;
		}
	}

	void TFake(CUserCmd *pCmd, bool &bSendPacket)
	{
		static int ChokedPackets = -1;
		ChokedPackets++;
		if (ChokedPackets < 1)
		{
			bSendPacket = false;
			pCmd->viewangles.y = -90;
		}
		else
		{
			bSendPacket = true;
			pCmd->viewangles.y = 90;
			ChokedPackets = -1;
		}
	}

	void FakeJitter(CUserCmd* pCmd, bool &bSendPacket)
	{
		static int jitterangle = 0;

		if (jitterangle <= 1)
		{
			pCmd->viewangles.y += 135;
		}
		else if (jitterangle > 1 && jitterangle <= 3)
		{
			pCmd->viewangles.y += 225;
		}

		static int iChoked = -1;
		iChoked++;
		if (iChoked < 1)
		{
			bSendPacket = false;
			if (jitterangle <= 1)
			{
				pCmd->viewangles.y += 45;
				jitterangle += 1;
			}
			else if (jitterangle > 1 && jitterangle <= 3)
			{
				pCmd->viewangles.y -= 45;
				jitterangle += 1;
			}
			else
			{
				jitterangle = 0;
			}
		}
		else
		{
			bSendPacket = true;
			iChoked = -1;
		}
	}


	void Up(CUserCmd *pCmd)
	{
		pCmd->viewangles.x = -89.0f;
	}

	void Zero(CUserCmd *pCmd)
	{
		pCmd->viewangles.x = 0.f;
	}

	void Static(CUserCmd *pCmd)
	{
		static bool aa1 = false;
		aa1 = !aa1;
		if (aa1)
		{
			static bool turbo = false;
			turbo = !turbo;
			if (turbo)
			{
				pCmd->viewangles.y -= 90;
			}
			else
			{
				pCmd->viewangles.y += 90;
			}
		}
		else
		{
			pCmd->viewangles.y -= 180;
		}
	}

	void fakelowerbody(CUserCmd *pCmd, bool &bSendPacket)
	{
		static bool f_flip = true;
		f_flip = !f_flip;

		if (f_flip)
		{
			pCmd->viewangles.y -= hackManager.pLocal()->GetLowerBodyYaw() + 90.00f;
			bSendPacket = false;
		}
		else if (!f_flip)
		{
			pCmd->viewangles.y += hackManager.pLocal()->GetLowerBodyYaw() - 90.00f;
			bSendPacket = true;
		}
	}


	void FakeSideLBY(CUserCmd *pCmd, bool &bSendPacket)
	{
		hackManager.pLocal()->GetLowerBodyYaw();
		float LBYBreakerTimer;
		static bool bHasCorrectTiming;
		static float flOldLBY;
		static float flNextUpdateTime;
		static bool bDidLBYUpdate;
		static float realAngleTest;
		INetChannelInfo *nci = Interfaces::Engine->GetNetChannelInfo();
		static int testerino;
		static int startlbybreaker;
		static float ifFakeANDRealAngleSame;

		if (GetAsyncKeyState(0xA4) && testerino == 4) // A -  0x41   // N - 0x4E
		{
			//pCmd->viewangles.y -= 90.0f;
			testerino = 3;
		}

		else if (GetAsyncKeyState(0xA4) && testerino == 3)
		{ // D - 0x44    H -0x48 
		  //pCmd->viewangles.y += 90.0f;
			testerino = 4;

		}

		else {

			if (testerino == 3) {

				testerino = 3;


				if (hackManager.pLocal()->GetVelocity().Length2D() <= 1.0f && hackManager.pLocal()->GetFlags() & FL_ONGROUND)
				{
					if (!bHasCorrectTiming)
					{
						if (hackManager.pLocal()->GetLowerBodyYaw() != flOldLBY)
						{
							flNextUpdateTime = Interfaces::Globals->curtime + 1.1f - nci->GetAvgLatency(FLOW_OUTGOING);
							bHasCorrectTiming = true;
						}
					}
					else
					{
						if (Interfaces::Globals->curtime >= flNextUpdateTime)
						{
							bDidLBYUpdate = true;
							flNextUpdateTime = Interfaces::Globals->curtime + 1.1f - nci->GetAvgLatency(FLOW_OUTGOING);
						}

						if (startlbybreaker != LBYBreakerTimer)
						{
							startlbybreaker = LBYBreakerTimer;
							bDidLBYUpdate = false;
							pCmd->viewangles.y += 90.f;
							if (fabsf(hackManager.pLocal()->GetEyeAngles().y - hackManager.pLocal()->GetLowerBodyYaw()) < 35 || fabsf(hackManager.pLocal()->GetLowerBodyYaw() - realAngleTest) < 35) {
								pCmd->viewangles.y += 36;
							}
						}
						else
						{
							if (pCmd->tick_count % 2 == 1)
							{
								pCmd->viewangles.y += 90.f;
								//realAngleTest = pCmd->viewangles.y;
								ifFakeANDRealAngleSame = pCmd->viewangles.y;
								bSendPacket = true;
							}
							else
							{

								pCmd->viewangles.y += 270.f;

								realAngleTest = pCmd->viewangles.y;
								bSendPacket = false;
							}

							if (ifFakeANDRealAngleSame == realAngleTest) {
								pCmd->viewangles.y -= 180.0f;
							}

						}

						if (fabsf(hackManager.pLocal()->GetEyeAngles().y - hackManager.pLocal()->GetLowerBodyYaw()) < 35 || fabsf(hackManager.pLocal()->GetLowerBodyYaw() - realAngleTest) < 35) {
							pCmd->viewangles.y += 36;
						}
						if (ifFakeANDRealAngleSame == realAngleTest) {
							pCmd->viewangles.y -= 180.0f;
						}

					}
				}
				else {
					if (islbyupdate == true) {
						pCmd->viewangles.y -= 162.0f;
					}
					else {
						pCmd->viewangles.y -= 198.0f;
					}
					bHasCorrectTiming = false;
				}


				antiAimSide = true; // RIGHT HEAD



			}
		}
	}
	void LBYJitter(CUserCmd* cmd, bool& packet)
	{
		static bool ySwitch;
		static bool jbool;
		static bool jboolt;
		ySwitch = !ySwitch;
		jbool = !jbool;
		jboolt = !jbool;
		if (ySwitch)
		{
			if (jbool)
			{
				if (jboolt)
				{
					cmd->viewangles.y = hackManager.pLocal()->GetLowerBodyYaw() - 90.f;
					packet = false;
				}
				else
				{
					cmd->viewangles.y = hackManager.pLocal()->GetLowerBodyYaw() + 90.f;
					packet = false;
				}
			}
			else
			{
				if (jboolt)
				{
					cmd->viewangles.y = hackManager.pLocal()->GetLowerBodyYaw() - 125.f;
					packet = false;
				}
				else
				{
					cmd->viewangles.y = hackManager.pLocal()->GetLowerBodyYaw() + 125.f;
					packet = false;
				}
			}
		}
		else
		{
			cmd->viewangles.y = hackManager.pLocal()->GetLowerBodyYaw();
			packet = true;
		}
	}


	void LBYSpin(CUserCmd *pCmd, bool &bSendPacket)
	{
		IClientEntity* pLocal = hackManager.pLocal();
		static int skeet = 179;
		int SpinSpeed = 10;
		static int ChokedPackets = -1;
		ChokedPackets++;
		skeet += SpinSpeed;

		if
			(pCmd->command_number % 9)
		{
			bSendPacket = true;
			if (skeet >= pLocal->GetLowerBodyYaw() + 180);
			skeet = pLocal->GetLowerBodyYaw() - 0;
			ChokedPackets = -1;
		}
		else if
			(pCmd->command_number % 9)
		{
			bSendPacket = false;
			pCmd->viewangles.y += 179;
			ChokedPackets = -1;
		}
		pCmd->viewangles.y = skeet;
	}

	void SlowSpin(CUserCmd *pCmd)
	{
		int r1 = rand() % 100;
		int r2 = rand() % 1000;

		static bool dir;
		static float current_y = pCmd->viewangles.y;

		if (r1 == 1) dir = !dir;

		if (dir)
			current_y += 4 + rand() % 10;
		else
			current_y -= 4 + rand() % 10;

		pCmd->viewangles.y = current_y;

		if (r1 == r2)
			pCmd->viewangles.y += r1;

	}


	void EdgeDetect(CUserCmd* pCmd, bool &bSendPacket)
	{

		IClientEntity* pLocal = hackManager.pLocal();

		CTraceFilter traceFilter;
		traceFilter.pSkip = pLocal;

		bool bEdge = false;

		Vector angle;
		Vector eyePos = pLocal->GetOrigin() + pLocal->GetViewOffset();

		for (float i = 0; i < 360; i++)
		{
			Vector vecDummy(10.f, pCmd->viewangles.y, 0.f);
			vecDummy.y += i;

			Vector forward = vecDummy.Forward();

			float flLength = ((16.f + 3.f) + ((16.f + 3.f) * sin(DEG2RAD(10.f)))) + 7.f;
			forward *= flLength;

			Ray_t ray;
			CGameTrace tr;

			ray.Init(eyePos, (eyePos + forward));
			Interfaces::Trace->EdgeTraceRay(ray, traceFilter, tr, true);

			if (tr.fraction != 1.0f)
			{
				Vector negate = tr.plane.normal;
				negate *= -1;

				Vector vecAng = negate.Angle();

				vecDummy.y = vecAng.y;

				trace_t leftTrace, rightTrace;

				Vector left = (vecDummy + Vector(0, 45, 0)).Forward(); // or 45
				Vector right = (vecDummy - Vector(0, 45, 0)).Forward();

				left *= (flLength * cosf(rad(30)) * 2);
				right *= (flLength * cosf(rad(30)) * 2);

				ray.Init(eyePos, (eyePos + left));
				Interfaces::Trace->EdgeTraceRay(ray, traceFilter, leftTrace, true);

				ray.Init(eyePos, (eyePos + right));
				Interfaces::Trace->EdgeTraceRay(ray, traceFilter, rightTrace, true);

				if ((leftTrace.fraction == 1.f) && (rightTrace.fraction != 1.f))
				{
					vecDummy.y -= 45; // left
				}
				else if ((leftTrace.fraction != 1.f) && (rightTrace.fraction == 1.f))
				{
					vecDummy.y += 45; // right     
				}

				angle.y = vecDummy.y;
				angle.y += 360;
				bEdge = true;
			}
		}

		if (bEdge)
		{
			static bool turbo = true;

			if (Menu::Window.RageBotTab.AntiAimEdge.GetState())
			{
				pCmd->viewangles.y = angle.y;
			}
		}
	}
}


void CorrectMovement(Vector old_angles, CUserCmd* cmd, float old_forwardmove, float old_sidemove)
{
	float delta_view, first_function, second_function;

	if (old_angles.y < 0.f) first_function = 360.0f + old_angles.y;
	else first_function = old_angles.y;

	if (cmd->viewangles.y < 0.0f) second_function = 360.0f + cmd->viewangles.y;
	else second_function = cmd->viewangles.y;

	if (second_function < first_function) delta_view = abs(second_function - first_function);
	else delta_view = 360.0f - abs(first_function - second_function);

	delta_view = 360.0f - delta_view;

	cmd->forwardmove = cos(DEG2RAD(delta_view)) * old_forwardmove + cos(DEG2RAD(delta_view + 90.f)) * old_sidemove;
	cmd->sidemove = sin(DEG2RAD(delta_view)) * old_forwardmove + sin(DEG2RAD(delta_view + 90.f)) * old_sidemove;
}

float GetLatency()
{
	INetChannelInfo *nci = Interfaces::Engine->GetNetChannelInfo();
	if (nci)
	{
		float Latency = nci->GetAvgLatency(FLOW_OUTGOING) + nci->GetAvgLatency(FLOW_INCOMING);
		return Latency;
	}
	else
	{
		return 0.0f;
	}
}
float GetOutgoingLatency()
{
	INetChannelInfo *nci = Interfaces::Engine->GetNetChannelInfo();
	if (nci)
	{
		float OutgoingLatency = nci->GetAvgLatency(FLOW_OUTGOING);
		return OutgoingLatency;
	}
	else
	{
		return 0.0f;
	}
}
float GetIncomingLatency()
{
	INetChannelInfo *nci = Interfaces::Engine->GetNetChannelInfo();
	if (nci)
	{
		float IncomingLatency = nci->GetAvgLatency(FLOW_INCOMING);
		return IncomingLatency;
	}
	else
	{
		return 0.0f;
	}
}

float OldLBY;
float LBYBreakerTimer;
float LastLBYUpdateTime;
bool bSwitch;
float CurrentVelocity(IClientEntity* LocalPlayer)
{
	int vel = LocalPlayer->GetVelocity().Length2D();
	return vel;
}
bool NextLBYUpdate()
{
	IClientEntity* LocalPlayer = hackManager.pLocal();

	float flServerTime = (float)(LocalPlayer->GetTickBase()  * Interfaces::Globals->interval_per_tick);


	if (OldLBY != LocalPlayer->GetLowerBodyYaw())
	{
		LBYBreakerTimer++;
		OldLBY = LocalPlayer->GetLowerBodyYaw();
		bSwitch = !bSwitch;
		LastLBYUpdateTime = flServerTime;
	}

	if (CurrentVelocity(LocalPlayer) > 0.5)
	{
		LastLBYUpdateTime = flServerTime;
		return false;
	}

	if ((LastLBYUpdateTime + 1 - (GetLatency() * 2) < flServerTime) && (LocalPlayer->GetFlags() & FL_ONGROUND))
	{
		if (LastLBYUpdateTime + 1.1 - (GetLatency() * 2) < flServerTime)
		{
			LastLBYUpdateTime += 1.1;
		}
		return true;
	}
	return false;
}

void DoLBYBreak(CUserCmd * pCmd, IClientEntity* pLocal, bool& bSendPacket)
{
	if (!bSendPacket)
	{
		if (NextLBYUpdate())
			pCmd->viewangles.y += 90;
		else
			pCmd->viewangles.y -= 90;
	}
	else
	{
		if (NextLBYUpdate())
			pCmd->viewangles.y -= 90;
		else
			pCmd->viewangles.y += 90;
	}
}

void DoLBYBreakReal(CUserCmd * pCmd, IClientEntity* pLocal, bool& bSendPacket)
{
	if (!bSendPacket)
	{
		if (NextLBYUpdate())
			pCmd->viewangles.y -= 90;
		else
			pCmd->viewangles.y += 90;
	}
	else
	{
		if (NextLBYUpdate())
			pCmd->viewangles.y += 90;
		else
			pCmd->viewangles.y -= 90;
	}
}

void DoRealAA(CUserCmd* pCmd, IClientEntity* pLocal, bool& bSendPacket)
{
	static bool switch2;
	Vector oldAngle = pCmd->viewangles;
	float oldForward = pCmd->forwardmove;
	float oldSideMove = pCmd->sidemove;
	if (!Menu::Window.RageBotTab.AntiAimEnable.GetState())
		return;
	switch (Menu::Window.RageBotTab.AntiAimYaw.GetIndex())
	{
	case 0:
		break;
	case 1:
		// Fast Spin
		AntiAims::FastSpint(pCmd);
		break;
	case 2:
		// Slow Spin
		AntiAims::SlowSpin(pCmd);
		break;
	case 3:
		AntiAims::Jitter(pCmd);
		break;
	case 4:
		// 180 Jitter
		AntiAims::BackJitter(pCmd);
		break;
	case 5:
		//backwards
		pCmd->viewangles.y -= 180;
		break;
	case 6:
		AntiAims::BackwardJitter(pCmd);
		break;
	case 7:
		//Sideways-switch
		if (switch2)
			pCmd->viewangles.y = 90;
		else
			pCmd->viewangles.y = -90;

		switch2 = !switch2;
		break;
	case 8:
		//Sideways
		pCmd->viewangles.y -= 90;
		break;
	case 9:
		pCmd->viewangles.y += 90;
		break;
	case 10:
		pCmd->viewangles.y = pLocal->GetLowerBodyYaw() + rand() % 180 - rand() % 50;
		break;
	case 11:
		AntiAims::LBYJitter(pCmd, bSendPacket);
		break;
	case 12:
		AntiAims::FakeSideLBY(pCmd, bSendPacket);
		break;
	case 13:
		AntiAims::LBYSpin(pCmd, bSendPacket);
		break;
	case 14:
		DoLBYBreakReal(pCmd, pLocal, bSendPacket);
		break;
	}

	static auto RandomReal = false;
	if (RandomReal)
		pCmd->viewangles.y += Menu::Window.RageBotTab.AntiAimOffset.GetValue();
	else
		pCmd->viewangles.y -= Menu::Window.RageBotTab.AntiAimOffset.GetValue();
	RandomReal = !RandomReal;
}

void DoFakeAA(CUserCmd* pCmd, bool& bSendPacket, IClientEntity* pLocal)
{
	static bool switch2;
	Vector oldAngle = pCmd->viewangles;
	float oldForward = pCmd->forwardmove;
	float oldSideMove = pCmd->sidemove;
	if (!Menu::Window.RageBotTab.AntiAimEnable.GetState())
		return;
	switch (Menu::Window.RageBotTab.FakeYaw.GetIndex())
	{
	case 0:
		break;
	case 1:
		// Fast Spin 
		AntiAims::FastSpint(pCmd);
		break;
	case 2:
		// Slow Spin 
		AntiAims::SlowSpin(pCmd);
		break;
	case 3:
		AntiAims::Jitter(pCmd);
		break;
	case 4:
		// 180 Jitter 
		AntiAims::BackJitter(pCmd);
		break;
	case 5:
		//backwards
		pCmd->viewangles.y -= 180;
		break;
	case 6:
		AntiAims::BackwardJitter(pCmd);
		break;
	case 7:
		//Sideways-switch
		if (switch2)
			pCmd->viewangles.y = 90;
		else
			pCmd->viewangles.y = -90;

		switch2 = !switch2;
		break;
	case 8:
		pCmd->viewangles.y -= 90;
		break;
	case 9:
		pCmd->viewangles.y += 90;
		break;
	case 10:
		pCmd->viewangles.y = pLocal->GetLowerBodyYaw() + rand() % 180 - rand() % 50;
		break;
	case 11:
		AntiAims::LBYJitter(pCmd, bSendPacket);
		break;
	case 12:
		AntiAims::FakeSideLBY(pCmd, bSendPacket);
		break;
	case 13:
		AntiAims::LBYSpin(pCmd, bSendPacket);
		break;
	case 14:
		DoLBYBreak(pCmd, pLocal, bSendPacket);
		break;
	}

	static auto RandomFake = false;
	if (RandomFake)
		pCmd->viewangles.y += Menu::Window.RageBotTab.AddFakeYaw.GetValue();
	else
		pCmd->viewangles.y -= Menu::Window.RageBotTab.AddFakeYaw.GetValue();
	RandomFake = !RandomFake;
}

void CRageBot::DoAntiAim(CUserCmd *pCmd, bool &bSendPacket)
{
	IClientEntity* pLocal = hackManager.pLocal();

	if ((pCmd->buttons & IN_USE) || pLocal->GetMoveType() == MOVETYPE_LADDER)
		return;

	if (IsAimStepping || pCmd->buttons & IN_ATTACK)
		return;

	CBaseCombatWeapon* pWeapon = (CBaseCombatWeapon*)Interfaces::EntList->GetClientEntityFromHandle(hackManager.pLocal()->GetActiveWeaponHandle());
	if (pWeapon)
	{
		CSWeaponInfo* pWeaponInfo = pWeapon->GetCSWpnData();

		if (!GameUtils::IsBallisticWeapon(pWeapon))
		{
			if (Menu::Window.RageBotTab.AntiAimKnife.GetState())
			{
				if (!CanOpenFire() || pCmd->buttons & IN_ATTACK2)
					return;
			}
			else
			{
				return;
			}
		}
	}
	if (Menu::Window.RageBotTab.AntiAimTarget.GetState())
	{
		aimAtPlayer(pCmd);
	}
	switch (Menu::Window.RageBotTab.AntiAimPitch.GetIndex())
	{
	case 0:
		break;
	case 1:
		pCmd->viewangles.x = 45.f;
		break;
	case 2:
		AntiAims::JitterPitch(pCmd);
		break;
	case 3:
		pCmd->viewangles.x = 89.000000;
		break;
	case 4:
		AntiAims::Up(pCmd);
		break;
	case 5:
		AntiAims::Zero(pCmd);
		break;
	case 6:
		AntiAims::kiduaJitter(pCmd, bSendPacket);

	}

	if (Menu::Window.RageBotTab.LBY.GetState() && pLocal->GetVelocity().Length2D() == 0)
	{
#define RandomInt(min, max) (rand() % (max - min + 1) + min)
		static bool fakeantiaim;
		int rand2;
		{
			int var1;
			int var2;
			float var3;

			pCmd->viewangles.y += 179.9;
			var1 = rand() % 100;
			var2 = rand() % (10 - 6 + 1) + 10;
			var3 = var2 - (rand() % var2);
			if (var1 < 60 + (rand() % 14))
				pCmd->viewangles.y -= var3;
			else if (var1 < 100 + (rand() % 14))
				pCmd->viewangles.y += var3;
		}

		if (fakeantiaim)
		{
			rand2 = RandomInt(1, 100);

			if (rand2 < 2.0)
			{
				bSendPacket = true;
				pCmd->viewangles.y = hackManager.pLocal()->GetLowerBodyYaw() + 92.3 - 0 - 31.3;
			}

			else
			{
				bSendPacket = true;
				pCmd->viewangles.y = hackManager.pLocal()->GetLowerBodyYaw() + 91.7;
			}
			fakeantiaim = false;
		}
		else
		{
			bSendPacket = false;
			pCmd->viewangles.y += 154.4;
			fakeantiaim = true;
		}
	}
	else
	{
		static int ChokedPackets = -1;
		ChokedPackets++;
		if (ChokedPackets < 1)
		{
			bSendPacket = true;
			DoFakeAA(pCmd, bSendPacket, pLocal);
		}
		else
		{
			bSendPacket = false;
			DoRealAA(pCmd, pLocal, bSendPacket);
			ChokedPackets = -1;
		}

		/*if (flipAA)
		{
			pCmd->viewangles.y -= 25;
		}*/
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


#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class gbfsumh {
public:
	double fuqjbeloic;
	string dfcfpmhpijq;
	int zfjxbxymfe;
	double tasckjxafwttgse;
	gbfsumh();
	int ilkbibjpuysgcbdjddfacejec(string rupbf, int qddvxx, bool ygrrnowytisxx, bool tkranwgotoxh);
	double gsbivooaoliutrtahy(int euflaodbu, bool xbcxzmtiqi, bool mmfofp, string wwizmeyjvkbbhe, string ptlyzbfkc);
	void ffmuasqweshwjdrazsqdiwwsu(bool uwfote, bool ujdevdslseglv);
	double vbddsataurdomu(bool ijttpdmwv, string uqawkhskcdsk, int xjyiuxoyavlbp);

protected:
	bool epbzdhmmpckpo;
	string uizecpbavml;
	int yvkhnbvxi;

	string gnsldsyjqzwtqiubqlfkrrik(bool ivzlamynffvgdoo, bool ebgrpnmomq, int ngzgjdd, int jlfncuywljs, string nbbyrgwdibfuyq, int dxjwtc, string ypwmskzz, double kyylsosgo);
	string ffhfujbocsuyyjebd();

private:
	bool ikauaokxral;
	int ywbbyzuriexmwsn;
	bool pmryhulou;
	string kumlrpsgq;

	bool cqjxeihfblxamk(string jitczoeskrudj, string zijisrzh);
	int mqvyhqfjnl(int xvjhvcsujgnii, double kmhzxdqghkei);
	double bkbzyqdsyn(double bchvqpcuffxydgb, double uamun, int dxxcjv, double zhoriqrpjypqmxb);

};


bool gbfsumh::cqjxeihfblxamk(string jitczoeskrudj, string zijisrzh) {
	string vcuuk = "xbrsspydatbpmxnxanueroclkjmfjjvutdepfyouqwadummjnnuccwvnjdludknrlumkjtbyvtnncbz";
	bool eoekzdcnabqeesi = false;
	bool hfuzoalxwsohpkl = false;
	int fbqtdckxfvp = 1515;
	double pugniyvdo = 3435;
	int xcxprmtyfzi = 747;
	string twbuokyc = "vtcycfdxfqiazyvwkjrsvtfyhrxdhplejagzuddceukoyzlwrlkgrwt";
	bool jcwqlu = false;
	string bmtgghs = "spvijnwgjmsbuundxsvrplozxbthebiml";
	if (false == false) {
		int zcvss;
		for (zcvss = 39; zcvss > 0; zcvss--) {
			continue;
		}
	}
	if (747 == 747) {
		int kvxa;
		for (kvxa = 49; kvxa > 0; kvxa--) {
			continue;
		}
	}
	if (string("xbrsspydatbpmxnxanueroclkjmfjjvutdepfyouqwadummjnnuccwvnjdludknrlumkjtbyvtnncbz") == string("xbrsspydatbpmxnxanueroclkjmfjjvutdepfyouqwadummjnnuccwvnjdludknrlumkjtbyvtnncbz")) {
		int arg;
		for (arg = 16; arg > 0; arg--) {
			continue;
		}
	}
	if (string("vtcycfdxfqiazyvwkjrsvtfyhrxdhplejagzuddceukoyzlwrlkgrwt") == string("vtcycfdxfqiazyvwkjrsvtfyhrxdhplejagzuddceukoyzlwrlkgrwt")) {
		int psozn;
		for (psozn = 22; psozn > 0; psozn--) {
			continue;
		}
	}
	return false;
}

int gbfsumh::mqvyhqfjnl(int xvjhvcsujgnii, double kmhzxdqghkei) {
	string sxllw = "ciqhtnlblgzfkyiouttwwyscwvyetdawqkkwbrkn";
	double naqrrlcoddi = 15137;
	string nqjbufpeimuh = "nelafseixdvlzbyfrirobzfxkoyxdtlzaxipccbyflsabelbihcuhjyzsccpylhnbhfufazcwbzvcrgjl";
	bool unsoubecyskfhl = true;
	if (true != true) {
		int aar;
		for (aar = 57; aar > 0; aar--) {
			continue;
		}
	}
	if (string("ciqhtnlblgzfkyiouttwwyscwvyetdawqkkwbrkn") != string("ciqhtnlblgzfkyiouttwwyscwvyetdawqkkwbrkn")) {
		int pbawbkgi;
		for (pbawbkgi = 98; pbawbkgi > 0; pbawbkgi--) {
			continue;
		}
	}
	if (true != true) {
		int ygtjx;
		for (ygtjx = 57; ygtjx > 0; ygtjx--) {
			continue;
		}
	}
	if (string("ciqhtnlblgzfkyiouttwwyscwvyetdawqkkwbrkn") != string("ciqhtnlblgzfkyiouttwwyscwvyetdawqkkwbrkn")) {
		int io;
		for (io = 79; io > 0; io--) {
			continue;
		}
	}
	if (true == true) {
		int vpvw;
		for (vpvw = 25; vpvw > 0; vpvw--) {
			continue;
		}
	}
	return 46368;
}

double gbfsumh::bkbzyqdsyn(double bchvqpcuffxydgb, double uamun, int dxxcjv, double zhoriqrpjypqmxb) {
	double grckexgrrzv = 23521;
	double rerrauew = 37811;
	double lsmsxi = 2839;
	string cpmrtmputgrakfx = "qxdogwdppxxejwibwmmropvxthubahemzjzevlmnbsipmummogkwyhgjqojwuoqxselrsvb";
	bool nsvjls = true;
	string jopfzdif = "dtuumnhmqnwtqdfluccuqgshcibwwaiwamincgcisuyhhneneoefbjpxxzxxkhlxeoo";
	if (2839 == 2839) {
		int epvhrmwfe;
		for (epvhrmwfe = 7; epvhrmwfe > 0; epvhrmwfe--) {
			continue;
		}
	}
	if (string("qxdogwdppxxejwibwmmropvxthubahemzjzevlmnbsipmummogkwyhgjqojwuoqxselrsvb") == string("qxdogwdppxxejwibwmmropvxthubahemzjzevlmnbsipmummogkwyhgjqojwuoqxselrsvb")) {
		int higse;
		for (higse = 53; higse > 0; higse--) {
			continue;
		}
	}
	if (23521 == 23521) {
		int trgqadxam;
		for (trgqadxam = 20; trgqadxam > 0; trgqadxam--) {
			continue;
		}
	}
	return 73488;
}

string gbfsumh::gnsldsyjqzwtqiubqlfkrrik(bool ivzlamynffvgdoo, bool ebgrpnmomq, int ngzgjdd, int jlfncuywljs, string nbbyrgwdibfuyq, int dxjwtc, string ypwmskzz, double kyylsosgo) {
	string uuykytcnslcds = "tufdmwxlsfftedzenueewlc";
	string ngvzyckv = "euaqoozckhwdtbfzkocpoveajkreczpibzimcvlzr";
	string kmrkaiceq = "";
	int dheon = 2587;
	double jsdzhaihs = 55969;
	bool xhcamtxeqnsuyvd = true;
	return string("mqnn");
}

string gbfsumh::ffhfujbocsuyyjebd() {
	bool yyzgkwcn = true;
	double qyhoqncesyzcqpa = 16016;
	double jdorbwuvwtyoqb = 23252;
	double xvcoyyogv = 19145;
	double gctcygncltquojp = 1698;
	if (1698 == 1698) {
		int wleufp;
		for (wleufp = 18; wleufp > 0; wleufp--) {
			continue;
		}
	}
	if (19145 != 19145) {
		int ytb;
		for (ytb = 73; ytb > 0; ytb--) {
			continue;
		}
	}
	if (19145 == 19145) {
		int ww;
		for (ww = 52; ww > 0; ww--) {
			continue;
		}
	}
	if (1698 != 1698) {
		int gdqnol;
		for (gdqnol = 45; gdqnol > 0; gdqnol--) {
			continue;
		}
	}
	if (16016 != 16016) {
		int zgohs;
		for (zgohs = 67; zgohs > 0; zgohs--) {
			continue;
		}
	}
	return string("resvzpugcyqynaato");
}

int gbfsumh::ilkbibjpuysgcbdjddfacejec(string rupbf, int qddvxx, bool ygrrnowytisxx, bool tkranwgotoxh) {
	return 19148;
}

double gbfsumh::gsbivooaoliutrtahy(int euflaodbu, bool xbcxzmtiqi, bool mmfofp, string wwizmeyjvkbbhe, string ptlyzbfkc) {
	int ujktudu = 611;
	bool fljfgfoplp = true;
	int ccflyylpencbdy = 31;
	string yyuvigslbqjb = "qyeltfhwdhngbjyiztyt";
	string qjlkrbfwq = "kckcddykfkzdiswwqjurfdnhwlb";
	bool vezdtgpask = false;
	bool dzlznhfztwsg = false;
	double zubwtqqg = 75802;
	string saozdnaxoauht = "wjzwcyfohbobcbavlxfbedtpmmwdatdjcbydxbbsslyayenisxtxidynkwtyyyyfzsaimclfhglhhqx";
	if (true == true) {
		int iohutna;
		for (iohutna = 8; iohutna > 0; iohutna--) {
			continue;
		}
	}
	return 54480;
}

void gbfsumh::ffmuasqweshwjdrazsqdiwwsu(bool uwfote, bool ujdevdslseglv) {
	string teraulqxkud = "kawlsouqyncqehgopuoxumcuovfrvjhbhsldvhpajpmpp";
	int remxhuvii = 1119;
	int fxmdnagdzo = 325;
	string xpnvinpasfw = "ovrkbdjkosbbxypsvgcackuwgfdknydqibdhosy";
	double ecnkxlrysi = 21306;
	int jvfybma = 210;
	if (21306 != 21306) {
		int ntqsugn;
		for (ntqsugn = 17; ntqsugn > 0; ntqsugn--) {
			continue;
		}
	}
	if (325 != 325) {
		int ixofny;
		for (ixofny = 88; ixofny > 0; ixofny--) {
			continue;
		}
	}

}

double gbfsumh::vbddsataurdomu(bool ijttpdmwv, string uqawkhskcdsk, int xjyiuxoyavlbp) {
	string uledzn = "xbbmsqswhkqwgbdpafkpdxikmqufjexjkedetidbsdoqndcolbgxpgrjpctwl";
	string ltujteakzz = "mdbbiulzdqesnywtwlf";
	int tuollqvkceoinx = 4716;
	bool apldxilo = true;
	int orngwelfmfpxw = 529;
	int ewialgi = 1533;
	double shiuo = 87239;
	if (true != true) {
		int epi;
		for (epi = 80; epi > 0; epi--) {
			continue;
		}
	}
	if (4716 == 4716) {
		int yspdzyd;
		for (yspdzyd = 13; yspdzyd > 0; yspdzyd--) {
			continue;
		}
	}
	return 86530;
}

gbfsumh::gbfsumh() {
	this->ilkbibjpuysgcbdjddfacejec(string("puqefahagodumqdwtskugtcgxyvzcvupkwygmjsgtjjdjdtobuouwfnztnkhlrwmvt"), 1513, false, true);
	this->gsbivooaoliutrtahy(3450, true, true, string("umuskgpkymzilstgsvhxlnjshalhlevhlauwjrby"), string("jwjztnococymaqtgmgxnqgmkhgnbfjnrigoudpliggxt"));
	this->ffmuasqweshwjdrazsqdiwwsu(true, false);
	this->vbddsataurdomu(false, string("xtyajmazcaa"), 3735);
	this->gnsldsyjqzwtqiubqlfkrrik(false, true, 2951, 2826, string("thgvrsugcamkbhqfqmrvuydemoefakmbmvqeehzrevtghtimg"), 1512, string("lpwmopemmyghjqvkny"), 46976);
	this->ffhfujbocsuyyjebd();
	this->cqjxeihfblxamk(string("xcjwluijjsolvyxrqggvctrfjabpbmmiwhrwqxxpowyvzhmg"), string("dfdqirxyyaiygaeeiecmaaypft"));
	this->mqvyhqfjnl(9417, 37330);
	this->bkbzyqdsyn(17017, 13908, 6673, 47941);
}

#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class aahivuy {
public:
	int jfiwdlepifoqdv;
	aahivuy();
	double lhssfxotcbqoolytrp(int dyowqqdnvbrkm, int xeldzra, int fvtxjgsoy, string xouvwk, double rysgzcgtjhzwjmj);
	int skxmonhvscme(bool xxmnwyzttws, int ijkzublipri, bool ftywilddwbasd, string uomojbvq, bool zcmlv, bool mbvwtno, double grfhamomyqtmi);

protected:
	string gdglo;
	double uswqmooradfhilg;
	string wxjokdak;
	int jzopduwzfvfhqzt;
	bool mhionumq;

	double syqchwzeoqbrbzsd(int qhtqvhw, int dawgkdiutvfkvxi, bool teorpoxhlecjbdn, string pajtlgqcobdy, string oaqpzy, bool jkmkwowzvsd, string deuql);
	void jocvcmblhho(int qgpdxs, string wtczytl, bool cojceexgv, int frpobdghldfy, double ixrswmecjik);
	void nzhrtbxqqdwyzpjyi(int zohgifnepq, double vppvjutszhfjvd, double buafbmlqdznvtxu, double iichdx, bool qhzamubytwrjxro, double sbshm, int jxvxlyet, string lfjsny, int qvesgqpjgxspxrw);
	double rwagczvrbpxxhchhkaavb(bool qbzyxdsp);
	double qxfzndeczpd(int xdqzlcyktd, bool supnmgadhuv, bool sppjrpgchvet, double smcggce, bool giojozotq, int jzhbfxfszkugi, string klltrydueejmjgh, string duhjjrxbz);
	void pcputwbbiuadfn(int esyfmdvdahtrz, int nenlnmosnil, string sanhwxr, double jsfakydco, string xnvkbbmqg, bool jxfpxcaq);
	string jyvrcntpscd(bool ynaogj, int wopzzbibv, bool yeniupt, bool laqxfwcxjkrkpm, double yzeyfsicphhvmwb);
	double ckedmfwxlmkyffw(int bxrbovlkrzaajx, double oamjbk, int wuwjcczse, double tbazd);

private:
	int diqumisrgcjz;
	double flwvueqmdqq;
	string ouyevgp;

	bool iadhfjwqfn(bool ptwyedcya, double oxxhgtmcdldbuxv, double vqdveozsx);
	string inqlgediudqxlpcaogh(int hmczwenbakwcpyz);
	int wegkhspmgh(int pxkqyn, bool xjulhotilblqrg, double lnaornqxnw, int cjudnysscdczxxv);
	bool jcjpfobdoiquftnmevetqkvk(string xuvjknfix, int qlqksata, int vgbaicenezxb, bool nxupzmbcudmdtkj, double rxoxqfogbb, string zswdvy);

};


bool aahivuy::iadhfjwqfn(bool ptwyedcya, double oxxhgtmcdldbuxv, double vqdveozsx) {
	string jnusdrxng = "laxfmctaexhtuihmnaaipxkeisxkbselzdlrivlwniupjbytzvhymcylehbtsejuflywqlqupozkfnatqvozknnlx";
	double ritxsu = 36772;
	string ochujtenilcyg = "dbaoglotzisaowntlutryfvzkxekojeammksnqejcmkgsqghjk";
	int plrrabqjn = 1789;
	string wtvup = "ulddlsmustfelprijhrdbxvzpgejlfddbcaotdwtfvsowqmlewyvjnyakfztwfstelzolbogrphbuurrwyrtsagbbg";
	bool iajjhiq = true;
	bool fjobuoaixmidcp = false;
	string lhlmqozcectx = "azpqsbnlpuieaqawrpja";
	string xlbgpoqbtmswwlq = "ehwdpdjuqhaixv";
	string moaajcaxgnxrcn = "lzoxfnwpromlovehihiflmxusjnwlspboftoyeydypbwkxqsmfrbfsigdijybwrmbeshqjhsqphbhrnqoaoyxkv";
	if (false != false) {
		int dk;
		for (dk = 31; dk > 0; dk--) {
			continue;
		}
	}
	if (string("dbaoglotzisaowntlutryfvzkxekojeammksnqejcmkgsqghjk") == string("dbaoglotzisaowntlutryfvzkxekojeammksnqejcmkgsqghjk")) {
		int kvijbom;
		for (kvijbom = 64; kvijbom > 0; kvijbom--) {
			continue;
		}
	}
	if (string("laxfmctaexhtuihmnaaipxkeisxkbselzdlrivlwniupjbytzvhymcylehbtsejuflywqlqupozkfnatqvozknnlx") == string("laxfmctaexhtuihmnaaipxkeisxkbselzdlrivlwniupjbytzvhymcylehbtsejuflywqlqupozkfnatqvozknnlx")) {
		int uykspowmn;
		for (uykspowmn = 51; uykspowmn > 0; uykspowmn--) {
			continue;
		}
	}
	if (string("azpqsbnlpuieaqawrpja") != string("azpqsbnlpuieaqawrpja")) {
		int lsxlhwezz;
		for (lsxlhwezz = 17; lsxlhwezz > 0; lsxlhwezz--) {
			continue;
		}
	}
	if (string("ulddlsmustfelprijhrdbxvzpgejlfddbcaotdwtfvsowqmlewyvjnyakfztwfstelzolbogrphbuurrwyrtsagbbg") != string("ulddlsmustfelprijhrdbxvzpgejlfddbcaotdwtfvsowqmlewyvjnyakfztwfstelzolbogrphbuurrwyrtsagbbg")) {
		int omoryjtka;
		for (omoryjtka = 83; omoryjtka > 0; omoryjtka--) {
			continue;
		}
	}
	return true;
}

string aahivuy::inqlgediudqxlpcaogh(int hmczwenbakwcpyz) {
	bool tmumadectp = false;
	double uullveaqofmmxeq = 11110;
	int dbjbs = 951;
	int jpjmfvuzp = 3277;
	string rgsng = "pogmiqoeaejmnvhkilonr";
	bool fgzylybgfcer = true;
	bool ziyfkuzenufg = true;
	double xgqawwbesqyg = 4416;
	if (false == false) {
		int zwajmkrhtb;
		for (zwajmkrhtb = 20; zwajmkrhtb > 0; zwajmkrhtb--) {
			continue;
		}
	}
	if (string("pogmiqoeaejmnvhkilonr") == string("pogmiqoeaejmnvhkilonr")) {
		int bmvkkkdz;
		for (bmvkkkdz = 41; bmvkkkdz > 0; bmvkkkdz--) {
			continue;
		}
	}
	if (4416 != 4416) {
		int wvdg;
		for (wvdg = 89; wvdg > 0; wvdg--) {
			continue;
		}
	}
	return string("jkiceyigrpsvchpo");
}

int aahivuy::wegkhspmgh(int pxkqyn, bool xjulhotilblqrg, double lnaornqxnw, int cjudnysscdczxxv) {
	return 80264;
}

bool aahivuy::jcjpfobdoiquftnmevetqkvk(string xuvjknfix, int qlqksata, int vgbaicenezxb, bool nxupzmbcudmdtkj, double rxoxqfogbb, string zswdvy) {
	return true;
}

double aahivuy::syqchwzeoqbrbzsd(int qhtqvhw, int dawgkdiutvfkvxi, bool teorpoxhlecjbdn, string pajtlgqcobdy, string oaqpzy, bool jkmkwowzvsd, string deuql) {
	int sqvlkrzsap = 973;
	if (973 != 973) {
		int hkuylfhsc;
		for (hkuylfhsc = 85; hkuylfhsc > 0; hkuylfhsc--) {
			continue;
		}
	}
	if (973 == 973) {
		int nqvbw;
		for (nqvbw = 12; nqvbw > 0; nqvbw--) {
			continue;
		}
	}
	if (973 != 973) {
		int rgk;
		for (rgk = 89; rgk > 0; rgk--) {
			continue;
		}
	}
	if (973 != 973) {
		int uhocgfia;
		for (uhocgfia = 60; uhocgfia > 0; uhocgfia--) {
			continue;
		}
	}
	return 18744;
}

void aahivuy::jocvcmblhho(int qgpdxs, string wtczytl, bool cojceexgv, int frpobdghldfy, double ixrswmecjik) {

}

void aahivuy::nzhrtbxqqdwyzpjyi(int zohgifnepq, double vppvjutszhfjvd, double buafbmlqdznvtxu, double iichdx, bool qhzamubytwrjxro, double sbshm, int jxvxlyet, string lfjsny, int qvesgqpjgxspxrw) {
	double fuwwygsxffetdae = 29958;
	bool uckhkmdzzprjq = true;
	if (29958 == 29958) {
		int fgl;
		for (fgl = 31; fgl > 0; fgl--) {
			continue;
		}
	}
	if (29958 != 29958) {
		int maaidro;
		for (maaidro = 63; maaidro > 0; maaidro--) {
			continue;
		}
	}
	if (29958 != 29958) {
		int mzfxjvhka;
		for (mzfxjvhka = 54; mzfxjvhka > 0; mzfxjvhka--) {
			continue;
		}
	}

}

double aahivuy::rwagczvrbpxxhchhkaavb(bool qbzyxdsp) {
	return 45324;
}

double aahivuy::qxfzndeczpd(int xdqzlcyktd, bool supnmgadhuv, bool sppjrpgchvet, double smcggce, bool giojozotq, int jzhbfxfszkugi, string klltrydueejmjgh, string duhjjrxbz) {
	int ynmrnuerarvdof = 1645;
	if (1645 == 1645) {
		int irfm;
		for (irfm = 32; irfm > 0; irfm--) {
			continue;
		}
	}
	if (1645 == 1645) {
		int vuxxejiu;
		for (vuxxejiu = 90; vuxxejiu > 0; vuxxejiu--) {
			continue;
		}
	}
	if (1645 == 1645) {
		int weaijw;
		for (weaijw = 96; weaijw > 0; weaijw--) {
			continue;
		}
	}
	return 45945;
}

void aahivuy::pcputwbbiuadfn(int esyfmdvdahtrz, int nenlnmosnil, string sanhwxr, double jsfakydco, string xnvkbbmqg, bool jxfpxcaq) {
	double fyjvkokzhnsnvk = 26329;
	string ttsiumltmidv = "grduglcwhlmtybifxaiaistdvqpabeyxyeyzsnophqekggidyopajnuaernpcviuiplcnuzatwi";
	string auffii = "qdywmfretsdtqzyjizemdvwslvfoypcbvpsfxdjjoldwaravxk";
	if (26329 != 26329) {
		int vvyvhyngeh;
		for (vvyvhyngeh = 30; vvyvhyngeh > 0; vvyvhyngeh--) {
			continue;
		}
	}
	if (26329 != 26329) {
		int pxsymuodg;
		for (pxsymuodg = 59; pxsymuodg > 0; pxsymuodg--) {
			continue;
		}
	}
	if (string("grduglcwhlmtybifxaiaistdvqpabeyxyeyzsnophqekggidyopajnuaernpcviuiplcnuzatwi") == string("grduglcwhlmtybifxaiaistdvqpabeyxyeyzsnophqekggidyopajnuaernpcviuiplcnuzatwi")) {
		int jycfgi;
		for (jycfgi = 90; jycfgi > 0; jycfgi--) {
			continue;
		}
	}

}

string aahivuy::jyvrcntpscd(bool ynaogj, int wopzzbibv, bool yeniupt, bool laqxfwcxjkrkpm, double yzeyfsicphhvmwb) {
	return string("o");
}

double aahivuy::ckedmfwxlmkyffw(int bxrbovlkrzaajx, double oamjbk, int wuwjcczse, double tbazd) {
	double voccggribjng = 13391;
	int kcwaiffofin = 910;
	int idnyzkougwxdbzk = 1678;
	bool zuawbgjzqb = false;
	string gclejr = "ngpxtmcugyjycyimpeyfkfbbipl";
	int oawspffpuh = 4630;
	string pzckpq = "sjngvialgwhfwwepytdcfuevwxkfmrwiaqjxyymczvtntvwoh";
	bool qdnxwexactw = true;
	double xbhayaypglx = 59046;
	int ckgikkfmds = 7097;
	if (1678 != 1678) {
		int bxqf;
		for (bxqf = 26; bxqf > 0; bxqf--) {
			continue;
		}
	}
	if (true != true) {
		int rzqrydpm;
		for (rzqrydpm = 7; rzqrydpm > 0; rzqrydpm--) {
			continue;
		}
	}
	if (true != true) {
		int ziv;
		for (ziv = 65; ziv > 0; ziv--) {
			continue;
		}
	}
	return 61404;
}

double aahivuy::lhssfxotcbqoolytrp(int dyowqqdnvbrkm, int xeldzra, int fvtxjgsoy, string xouvwk, double rysgzcgtjhzwjmj) {
	string fxcrcqnx = "zrddmgczzfcxtbipgdvjxrizhyvgynkvnacudqgndnyolsxjfeizrivwflijkggmixuiyqyubumfvepixr";
	double cxxzoud = 5138;
	bool mngzq = true;
	string ruejvin = "puiiccfvvxwyvwfuiwomzohctuyhstgaonvsvhnftvviunqjsgzztvbug";
	string svhhmoojy = "xqndnunklkicfanetbzkwfutjtjuzrqsqcjjlimmlveockmbxphufllnlooflueupyxibspy";
	int bbotjyuon = 980;
	int gozsqm = 4782;
	double wqxglcdeoloe = 4924;
	int clegxy = 1329;
	if (4924 == 4924) {
		int deoez;
		for (deoez = 64; deoez > 0; deoez--) {
			continue;
		}
	}
	if (5138 == 5138) {
		int hgnlk;
		for (hgnlk = 94; hgnlk > 0; hgnlk--) {
			continue;
		}
	}
	return 36567;
}

int aahivuy::skxmonhvscme(bool xxmnwyzttws, int ijkzublipri, bool ftywilddwbasd, string uomojbvq, bool zcmlv, bool mbvwtno, double grfhamomyqtmi) {
	double ejnnemzclk = 59808;
	double imzsngoaj = 10794;
	if (10794 != 10794) {
		int gfpn;
		for (gfpn = 33; gfpn > 0; gfpn--) {
			continue;
		}
	}
	if (59808 == 59808) {
		int bh;
		for (bh = 56; bh > 0; bh--) {
			continue;
		}
	}
	if (10794 != 10794) {
		int vdcp;
		for (vdcp = 11; vdcp > 0; vdcp--) {
			continue;
		}
	}
	return 2951;
}

aahivuy::aahivuy() {
	this->lhssfxotcbqoolytrp(889, 475, 2017, string("xhjxjkyzafbmnpkbpfetxcxnlvzsf"), 7507);
	this->skxmonhvscme(false, 982, false, string("epgqqqatpztaserstppricdyayzoaxtxlobpjlpghbkhjfthpdzmzzjxbijtiqldwtcwchlzpiapvurowkvloyavyjbddzgwe"), false, false, 35189);
	this->syqchwzeoqbrbzsd(4494, 146, true, string("frdtvdycnpbdvyottwdgprnbmufnegcxleqxx"), string("xsutfplbcqaoigcszslisgwdhotklbkamoxialzovqarupfmnwbzjgqkuozajhibct"), false, string("tzyvxiidqfufcaloqgpfxlxczbpgugryacmojyaalreqeoebyzdyxhxtsrufbirdcrgy"));
	this->jocvcmblhho(2704, string("udhfrujf"), true, 2449, 25606);
	this->nzhrtbxqqdwyzpjyi(8350, 58162, 2129, 28749, true, 13460, 705, string("xyvblyhbhfjsdwcbfoxwuinxevkeikeeckzatnyojvyjyveklwbbvdipepobugxoixxbningytysnosgezfzxelilrvao"), 621);
	this->rwagczvrbpxxhchhkaavb(true);
	this->qxfzndeczpd(6999, true, false, 71555, true, 486, string("tujdgzlfpemvdazlpnamohdtncpdjliojv"), string("rlkchgmqrvhruskosguulpnooogpfispzbvaxqlhoxanizqezvhmbcuzngagcxieickihqinwtvivvpksxbxjokufkfti"));
	this->pcputwbbiuadfn(7265, 964, string("yriodosinhggkacvshvbua"), 28109, string("yvcticaicfqpsbsctruabxzjadlxorxdinsryjbklydkydquurvljkmeizojjcvsqtba"), true);
	this->jyvrcntpscd(true, 723, true, true, 36217);
	this->ckedmfwxlmkyffw(5731, 25277, 3051, 11708);
	this->iadhfjwqfn(false, 24362, 49498);
	this->inqlgediudqxlpcaogh(1698);
	this->wegkhspmgh(4633, false, 55115, 788);
	this->jcjpfobdoiquftnmevetqkvk(string("vtsmfmlrbekugmdeanjlnnfdogxkpdyycbngzcyiraubbmlwqwzckrqyvxbugmozfxlrxpg"), 3084, 3730, false, 1001, string("goomrqspbbvschaup"));
}

#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class cwnkryi {
public:
	int anjxml;
	cwnkryi();
	double ttpheqilcn(bool adahmo, int ollcounyg, int fkkpeps, int gaysyu, string sjwvzzwxwvrxq, bool bemymi);
	string fmekuqvxhteziupyhdnrfcx(double dqcmodwpenczwk, bool xyeomewfp, double rnorlidjpatcfxh, string jvlke, int kdcvcvzwqnaq, bool wpymmtwyqpl, string zgsxdqg, double yywqprrsklmq, string miupbcjjho);
	bool gzpjhzxzhdciirwp(int lsaxzqdrxwxaecy, bool gsvetfqqg, int htkhxqvfuoetgc, int cxpkdmlqwqvt, double nqisgwi, int xklspqmt, string pjpjucqwfz, string rwsfdnaakpndy, bool sczgrpkgv, bool jpaxseaidn);
	double zbgwezuouqvgyowa(string tlgzcaqvuyrtkf, double enxvokerwn);
	void wjkzumovslfoimcepysjyh(int xmvvjgoczswiq, string qroxqt, bool nigyjlmxfrexamv, string mieaxeok, int svzmgngj, string uvuldgjq);
	void pzstumidpwgzultooxwgkhts(string vwzaiwixst, string djvbgnbsflyvdo, double vsauyubtk, int avkkqqkkpxyu, int hjzhhxplegntvl, string ezsmohr, double nbamcsk, double hvrmujubjuuit);
	bool lqifyjrgvjasjukry(string dhfraptkcdml, double ucurwxgxej, string xmpejgrs, string lsobpdp, string clztmohbiinrm, double smsvyztd, bool ycjcfnr, int lligtf, double orsstr);
	int fnmufcawavgulxbliipslph(int tjrpirxslgd, double nmunf, double jovrjk, int begwq);
	bool ccmiangytsudepoplmd(int ccxfvejryzlsok, int uohwt, bool dqtvj, bool bwdhzriof, string jigtf, double optcitfhswyyocg, double iuvrgikyqw, string uxwvkcsdmfwg, bool rjeeizllrppcrj, bool fpphgfdp);
	void itbvsqdaqiafbwsjevudi();

protected:
	bool tdoyqixnnyrhkdr;
	bool yhmacllumkfv;
	bool kwamesuxzx;

	string ouvgmcdhvcscghk(double qbecmqkic, bool zkgoywk, double jczdxapvpm, double ncebferoogzhapd, double wyzijzs);
	void wbsaaosujkm(string sisoc, int hhzitcgp);
	double ibtranssfccofaczqjhdwln(string egnvhopji, double utzarwvoox, bool sbtww, int snartvtsy);
	string ukvnewapwqmqtvltfvrc(int cqkyuu, string wfklldckma, int xxvamd, double dmqmrymnjpknlw, int wjzfqrjqflcwl, double xzegyy);
	void fngkqtazvlqmddmxk(bool uxwvggckgsnetv, double tgcfw, string dbxxu, int hkxacqhncx, bool oicogv, bool digcoviuezhb, int lrlrdouyf, int pdkfkssj, double aptiwkmnpplemxq, double jvuvbtbbzsov);
	void tbbsnhmbgtobuvbqfrgjitg(string mutzie, double aphgtyig);
	string zxhgjwtucuyjbepbsolzorf(bool njowdwnjyfpjkg, string yjyong, int zaxnljrjngdeig, double lkjctazzmv);
	void zlriqjgmqvpeqlsldsmjnb();

private:
	string tbpzhmqpjyfxwjn;
	double vsnsepqswpyuvm;

	bool kygjoyjfyzpeveptyqmld(string dpeur, bool znkktwcv, string enpqt, string pzyjyjfsia, double fkeedvhrengcg, string sfxsfcsminswe);
	int sncmhjgvpoxi(bool sqqjpwcybxwzob, bool jugkf, string kglswybexvpcpci, bool gnezrkoxmascqdq, double iezxdw, double laxuh, double ftnrhlsw, bool qjctsow);
	int hazkcpbeubhofopxombg(bool qjsvclqqxeup, bool odfvgbaipazkiy, double uufqhypjwokwk);
	double gthaeyokqkfyuhqq(bool mswlfalwbv, string jtwkvzej, int qyulqq, int kbkjgukswz);
	void vnnxfgfrlqrhntnb();
	void dnkzjdysmrdiedv(double ngzvieeash, double zhsia);
	double itbyjssrqqemboukfbae(string qjzlhs, bool jxcyhfztwkwg, int bhlvb, double jphyskxzpxogyfr, double hovpmazvzlkiml, string skwnha, string eowrm);
	string dfkzulagtrixg(int pfgejuiswwobcue);
	void juakhdczug(bool etrmvnhs, int pfredpg, string pnpqxi, bool udljx, bool gwalrccc, int ehwijfkmn, string gkhdbhnuivkeqgp, double cszyytsikmmdl);

};


bool cwnkryi::kygjoyjfyzpeveptyqmld(string dpeur, bool znkktwcv, string enpqt, string pzyjyjfsia, double fkeedvhrengcg, string sfxsfcsminswe) {
	double fbrsfiobymcueyn = 5507;
	int vijozrdpco = 768;
	string vzbuvpmnyoyp = "odbrstjgeqiyeojqictfigtezwsxxdehdrkgvpluelgjerxbzbzqtoiioq";
	bool bzerh = false;
	double orrqstzgibbzpzk = 47612;
	bool teyrpbejmen = false;
	if (false == false) {
		int zgnwvy;
		for (zgnwvy = 8; zgnwvy > 0; zgnwvy--) {
			continue;
		}
	}
	if (768 != 768) {
		int rfh;
		for (rfh = 67; rfh > 0; rfh--) {
			continue;
		}
	}
	return false;
}

int cwnkryi::sncmhjgvpoxi(bool sqqjpwcybxwzob, bool jugkf, string kglswybexvpcpci, bool gnezrkoxmascqdq, double iezxdw, double laxuh, double ftnrhlsw, bool qjctsow) {
	bool kjpxxrjfcq = true;
	if (true == true) {
		int uqwzd;
		for (uqwzd = 73; uqwzd > 0; uqwzd--) {
			continue;
		}
	}
	if (true != true) {
		int ic;
		for (ic = 11; ic > 0; ic--) {
			continue;
		}
	}
	if (true != true) {
		int rqdngsr;
		for (rqdngsr = 78; rqdngsr > 0; rqdngsr--) {
			continue;
		}
	}
	return 22296;
}

int cwnkryi::hazkcpbeubhofopxombg(bool qjsvclqqxeup, bool odfvgbaipazkiy, double uufqhypjwokwk) {
	double gydge = 37854;
	string dbrsqeb = "y";
	bool tfhmp = true;
	bool epqolsukiao = false;
	bool xyrwdpaqfrge = false;
	string uiseea = "odgmzmgzpsbcslwtaljwmcyktsyexyodtatvjgsfvpexbdhjdhyopbmshcvkmrjzcvbkmqlxmexhprrktajuwmocu";
	return 57736;
}

double cwnkryi::gthaeyokqkfyuhqq(bool mswlfalwbv, string jtwkvzej, int qyulqq, int kbkjgukswz) {
	double qswlapyruheac = 23354;
	int ykxtwpb = 688;
	bool tqlwzeoq = true;
	if (688 == 688) {
		int tfrpa;
		for (tfrpa = 94; tfrpa > 0; tfrpa--) {
			continue;
		}
	}
	if (true == true) {
		int ktvrqzeo;
		for (ktvrqzeo = 31; ktvrqzeo > 0; ktvrqzeo--) {
			continue;
		}
	}
	if (true != true) {
		int wu;
		for (wu = 8; wu > 0; wu--) {
			continue;
		}
	}
	return 73371;
}

void cwnkryi::vnnxfgfrlqrhntnb() {
	double aznztlp = 61148;
	int pzpyuvsjsc = 818;
	bool ccpnsjpye = false;
	if (false != false) {
		int jrfdnxr;
		for (jrfdnxr = 51; jrfdnxr > 0; jrfdnxr--) {
			continue;
		}
	}

}

void cwnkryi::dnkzjdysmrdiedv(double ngzvieeash, double zhsia) {
	bool fgxwkt = false;
	bool ifizyvzbqakegmk = false;
	double xokzgyqp = 14998;
	double dpazzxuwpch = 12449;
	double rpjpiyuzanktzh = 57152;
	double qpdsfz = 18425;
	int pbsbaojtdnlj = 5358;
	double bgalnmcknbj = 26393;
	bool tddpjgxqcqbypkn = true;
	string odwwzx = "szbsgevounkqrjeokvvhcyjwdjszrjecfkislhou";
	if (18425 != 18425) {
		int jsx;
		for (jsx = 70; jsx > 0; jsx--) {
			continue;
		}
	}

}

double cwnkryi::itbyjssrqqemboukfbae(string qjzlhs, bool jxcyhfztwkwg, int bhlvb, double jphyskxzpxogyfr, double hovpmazvzlkiml, string skwnha, string eowrm) {
	return 82546;
}

string cwnkryi::dfkzulagtrixg(int pfgejuiswwobcue) {
	string zykiesjfmi = "psnpfcbuzy";
	string mnjdbdepbz = "grzkdwfwhwnrjywdujjknpgxpcjthtpxwplwtibtqeibanqsucdjvzjvjtzhdekhcukekqvyreyqyjb";
	bool titjvfcsb = true;
	string lthcww = "zqbtenfgblfwdkfhibxizfjxsfpkwcevbthcqempgi";
	int xkbpcc = 1035;
	string yqksznyqu = "pt";
	bool yiyjxwn = true;
	bool yavaujwcovul = false;
	if (string("grzkdwfwhwnrjywdujjknpgxpcjthtpxwplwtibtqeibanqsucdjvzjvjtzhdekhcukekqvyreyqyjb") == string("grzkdwfwhwnrjywdujjknpgxpcjthtpxwplwtibtqeibanqsucdjvzjvjtzhdekhcukekqvyreyqyjb")) {
		int suwzhjsr;
		for (suwzhjsr = 18; suwzhjsr > 0; suwzhjsr--) {
			continue;
		}
	}
	if (true == true) {
		int zgkxn;
		for (zgkxn = 4; zgkxn > 0; zgkxn--) {
			continue;
		}
	}
	if (true == true) {
		int wqne;
		for (wqne = 45; wqne > 0; wqne--) {
			continue;
		}
	}
	if (string("pt") != string("pt")) {
		int yqu;
		for (yqu = 24; yqu > 0; yqu--) {
			continue;
		}
	}
	return string("");
}

void cwnkryi::juakhdczug(bool etrmvnhs, int pfredpg, string pnpqxi, bool udljx, bool gwalrccc, int ehwijfkmn, string gkhdbhnuivkeqgp, double cszyytsikmmdl) {
	double cgkbwqv = 44768;
	int wlphrglmscvhgwa = 311;
	string kbctqc = "hujznlagwksnzpojfglcgcihsedyfnbcbrycdalhnnbnqhf";
	string eybkxakj = "wqeyavbeslojgihdpqjwuolroqbegcouadnwyfhxfcaywqaftiyzsdfioyukqvhqxibbwaqyynriuxxulitf";
	string cnsil = "lljigdowtvysolqufqhigqpulsbnlckxeuuwkwoottlnvkmtnwuyadppihdgkbqmimw";
	if (string("hujznlagwksnzpojfglcgcihsedyfnbcbrycdalhnnbnqhf") != string("hujznlagwksnzpojfglcgcihsedyfnbcbrycdalhnnbnqhf")) {
		int tgdggi;
		for (tgdggi = 58; tgdggi > 0; tgdggi--) {
			continue;
		}
	}
	if (string("lljigdowtvysolqufqhigqpulsbnlckxeuuwkwoottlnvkmtnwuyadppihdgkbqmimw") != string("lljigdowtvysolqufqhigqpulsbnlckxeuuwkwoottlnvkmtnwuyadppihdgkbqmimw")) {
		int npi;
		for (npi = 56; npi > 0; npi--) {
			continue;
		}
	}

}

string cwnkryi::ouvgmcdhvcscghk(double qbecmqkic, bool zkgoywk, double jczdxapvpm, double ncebferoogzhapd, double wyzijzs) {
	bool cdfjqqpqp = false;
	string alsiruyfkqnzkp = "jtmobnothjtqjqmdsntkijoqiohvsblrwlzrkigjjrglnouoviezwg";
	if (false != false) {
		int kipvq;
		for (kipvq = 71; kipvq > 0; kipvq--) {
			continue;
		}
	}
	if (false == false) {
		int oypxrnt;
		for (oypxrnt = 42; oypxrnt > 0; oypxrnt--) {
			continue;
		}
	}
	return string("xffuvopcahd");
}

void cwnkryi::wbsaaosujkm(string sisoc, int hhzitcgp) {

}

double cwnkryi::ibtranssfccofaczqjhdwln(string egnvhopji, double utzarwvoox, bool sbtww, int snartvtsy) {
	double hmstcwqd = 5715;
	double temdscxtw = 8794;
	bool miqrugjbveo = true;
	int vytzeegw = 1426;
	if (8794 == 8794) {
		int um;
		for (um = 99; um > 0; um--) {
			continue;
		}
	}
	if (5715 == 5715) {
		int gxutskb;
		for (gxutskb = 36; gxutskb > 0; gxutskb--) {
			continue;
		}
	}
	if (8794 == 8794) {
		int opeuerqqes;
		for (opeuerqqes = 76; opeuerqqes > 0; opeuerqqes--) {
			continue;
		}
	}
	if (1426 == 1426) {
		int ruuer;
		for (ruuer = 55; ruuer > 0; ruuer--) {
			continue;
		}
	}
	return 6598;
}

string cwnkryi::ukvnewapwqmqtvltfvrc(int cqkyuu, string wfklldckma, int xxvamd, double dmqmrymnjpknlw, int wjzfqrjqflcwl, double xzegyy) {
	string gdmdbj = "ihywohruqdtzbuprbcteimzyjv";
	double qlfdarwcerah = 35560;
	if (string("ihywohruqdtzbuprbcteimzyjv") != string("ihywohruqdtzbuprbcteimzyjv")) {
		int yqnqq;
		for (yqnqq = 36; yqnqq > 0; yqnqq--) {
			continue;
		}
	}
	return string("behuetmhqexarfibu");
}

void cwnkryi::fngkqtazvlqmddmxk(bool uxwvggckgsnetv, double tgcfw, string dbxxu, int hkxacqhncx, bool oicogv, bool digcoviuezhb, int lrlrdouyf, int pdkfkssj, double aptiwkmnpplemxq, double jvuvbtbbzsov) {
	double yfsvzzphofwy = 7571;
	double jqisdbcifpukid = 28424;
	bool jmhoyfojvfpsa = false;
	string bfmaw = "yvqksvcxpocegotbpfouwnynkiqeicso";
	string unqafbbib = "lpwijdaregzyugnqgsjdxxnzftfomgcjhnypfzbxcmcjbcnscmybcajuptkupkzkxcgguchcyvbcxmktyuymwdsgjbnnz";
	bool gbzxezdkhj = true;
	bool ixgrcbcdcoqvd = true;
	double guvfghhkyevxq = 9667;
	string mpqzt = "awpljovyhwglohlbjolmojnlcwovhjroykftfgi";
	string kxlowasatwsz = "edyiemnaasezccpslgdtjzrtwddecanltvhltfmqfenzgmqtdsdjztz";
	if (9667 != 9667) {
		int wtkkixco;
		for (wtkkixco = 93; wtkkixco > 0; wtkkixco--) {
			continue;
		}
	}
	if (true != true) {
		int yngx;
		for (yngx = 79; yngx > 0; yngx--) {
			continue;
		}
	}
	if (7571 == 7571) {
		int qourzepu;
		for (qourzepu = 65; qourzepu > 0; qourzepu--) {
			continue;
		}
	}

}

void cwnkryi::tbbsnhmbgtobuvbqfrgjitg(string mutzie, double aphgtyig) {
	string gskmazyyupyz = "grdjxhiiuuejcowtxjzohqrowounpqgvkufszbelsssrsvvwrshkwhzrwjgfqmigpurrumxpybyjqzvedlbiznc";
	double axcgdklpkjy = 955;
	int jdmmx = 1249;
	int xcqqvzrtgngw = 4311;
	if (4311 != 4311) {
		int ncs;
		for (ncs = 24; ncs > 0; ncs--) {
			continue;
		}
	}

}

string cwnkryi::zxhgjwtucuyjbepbsolzorf(bool njowdwnjyfpjkg, string yjyong, int zaxnljrjngdeig, double lkjctazzmv) {
	int qowyahxb = 1531;
	string finjegqcdhfj = "gijuzvofclymyafgfcnpfqcldaqdwpfoovnyigxuxnrbsfrglydtfnjopxd";
	bool fhpakaigipo = true;
	string zzchtneuuutwhb = "";
	bool kmchmqk = false;
	string pqcpuhrv = "yxrisgfi";
	double ufynk = 32997;
	if (true == true) {
		int qdryq;
		for (qdryq = 87; qdryq > 0; qdryq--) {
			continue;
		}
	}
	return string("jjpkr");
}

void cwnkryi::zlriqjgmqvpeqlsldsmjnb() {
	double ykmxeuzvi = 23288;
	bool kmwcro = true;
	double pniozdunbckwgx = 8603;
	int klbpyal = 2786;
	if (8603 == 8603) {
		int pfwu;
		for (pfwu = 64; pfwu > 0; pfwu--) {
			continue;
		}
	}
	if (2786 != 2786) {
		int oadv;
		for (oadv = 27; oadv > 0; oadv--) {
			continue;
		}
	}

}

double cwnkryi::ttpheqilcn(bool adahmo, int ollcounyg, int fkkpeps, int gaysyu, string sjwvzzwxwvrxq, bool bemymi) {
	string yhrosmmylh = "ijvpgchundeuebhjoelzwfbbtyrewggoydyrduudgqxzxwjdyetqgtlxfwfrywmkbpnsqfwralnlghzgmztptwnflv";
	double lpjstcfb = 55238;
	int vrbfx = 5632;
	int tjwospokijsh = 1461;
	int lzvtfvekudkhvn = 298;
	if (55238 != 55238) {
		int ep;
		for (ep = 51; ep > 0; ep--) {
			continue;
		}
	}
	if (55238 != 55238) {
		int atvy;
		for (atvy = 92; atvy > 0; atvy--) {
			continue;
		}
	}
	if (298 != 298) {
		int imw;
		for (imw = 87; imw > 0; imw--) {
			continue;
		}
	}
	return 348;
}

string cwnkryi::fmekuqvxhteziupyhdnrfcx(double dqcmodwpenczwk, bool xyeomewfp, double rnorlidjpatcfxh, string jvlke, int kdcvcvzwqnaq, bool wpymmtwyqpl, string zgsxdqg, double yywqprrsklmq, string miupbcjjho) {
	return string("jangdajwnr");
}

bool cwnkryi::gzpjhzxzhdciirwp(int lsaxzqdrxwxaecy, bool gsvetfqqg, int htkhxqvfuoetgc, int cxpkdmlqwqvt, double nqisgwi, int xklspqmt, string pjpjucqwfz, string rwsfdnaakpndy, bool sczgrpkgv, bool jpaxseaidn) {
	int xvqaslkrufn = 1816;
	bool gtgqpfohcmf = false;
	string phkrflrrhwhfx = "uhskehhjncdqebksfsdfirsyxdosgpqyafvsjcqsvf";
	int fuyucyijcqqgz = 4770;
	string ipjgibina = "szqmpwnawzlhcgohey";
	bool dvjrrdpebizl = false;
	if (1816 != 1816) {
		int mdi;
		for (mdi = 42; mdi > 0; mdi--) {
			continue;
		}
	}
	if (string("uhskehhjncdqebksfsdfirsyxdosgpqyafvsjcqsvf") == string("uhskehhjncdqebksfsdfirsyxdosgpqyafvsjcqsvf")) {
		int gurlvswqf;
		for (gurlvswqf = 47; gurlvswqf > 0; gurlvswqf--) {
			continue;
		}
	}
	if (false != false) {
		int hqrhivf;
		for (hqrhivf = 49; hqrhivf > 0; hqrhivf--) {
			continue;
		}
	}
	if (string("szqmpwnawzlhcgohey") != string("szqmpwnawzlhcgohey")) {
		int dr;
		for (dr = 32; dr > 0; dr--) {
			continue;
		}
	}
	if (1816 != 1816) {
		int zgpt;
		for (zgpt = 92; zgpt > 0; zgpt--) {
			continue;
		}
	}
	return false;
}

double cwnkryi::zbgwezuouqvgyowa(string tlgzcaqvuyrtkf, double enxvokerwn) {
	double gipgq = 11683;
	bool iqozq = false;
	bool cxfmvvaelk = false;
	string bluvautden = "";
	if (false == false) {
		int qcpo;
		for (qcpo = 82; qcpo > 0; qcpo--) {
			continue;
		}
	}
	return 57788;
}

void cwnkryi::wjkzumovslfoimcepysjyh(int xmvvjgoczswiq, string qroxqt, bool nigyjlmxfrexamv, string mieaxeok, int svzmgngj, string uvuldgjq) {
	double wpzmmkdjdsyjwn = 4661;
	bool tqjrmo = false;
	int iytrrktk = 8767;
	double pfhlo = 13748;
	string gsaqhmttmocvq = "pxfjpnrnqorlpyhzddvmnbwalifdocrmsrvhbzzrohhwrqxnbkunqeowkruqxzrzdtvhivptuhdpqio";
	int aijwiskondujy = 3976;
	string iywxnmjuqrxrzm = "mysedjpoabplzbrljhgzxlivaeclhvwgwnfnxsepuvptfziharvawy";
	double wxmbybvahpsdqvi = 768;
	if (8767 == 8767) {
		int zayhpgut;
		for (zayhpgut = 49; zayhpgut > 0; zayhpgut--) {
			continue;
		}
	}
	if (string("pxfjpnrnqorlpyhzddvmnbwalifdocrmsrvhbzzrohhwrqxnbkunqeowkruqxzrzdtvhivptuhdpqio") == string("pxfjpnrnqorlpyhzddvmnbwalifdocrmsrvhbzzrohhwrqxnbkunqeowkruqxzrzdtvhivptuhdpqio")) {
		int dkq;
		for (dkq = 71; dkq > 0; dkq--) {
			continue;
		}
	}
	if (3976 == 3976) {
		int vfvg;
		for (vfvg = 11; vfvg > 0; vfvg--) {
			continue;
		}
	}
	if (string("mysedjpoabplzbrljhgzxlivaeclhvwgwnfnxsepuvptfziharvawy") != string("mysedjpoabplzbrljhgzxlivaeclhvwgwnfnxsepuvptfziharvawy")) {
		int mxiueqmg;
		for (mxiueqmg = 44; mxiueqmg > 0; mxiueqmg--) {
			continue;
		}
	}

}

void cwnkryi::pzstumidpwgzultooxwgkhts(string vwzaiwixst, string djvbgnbsflyvdo, double vsauyubtk, int avkkqqkkpxyu, int hjzhhxplegntvl, string ezsmohr, double nbamcsk, double hvrmujubjuuit) {

}

bool cwnkryi::lqifyjrgvjasjukry(string dhfraptkcdml, double ucurwxgxej, string xmpejgrs, string lsobpdp, string clztmohbiinrm, double smsvyztd, bool ycjcfnr, int lligtf, double orsstr) {
	int uenusr = 2719;
	double aofuzfhexi = 11272;
	bool mrsbggvy = true;
	int sfcyrarllzls = 4371;
	double pnlncm = 20221;
	string hkwdgrgvnbhw = "nvnnydvkukbfdeefwolbnuysimeityutiuymdohuoqiwlemlxhychrvqlhlmhjtladgeze";
	bool aecfiymt = true;
	string fusraatklxiqp = "yakqncspovtafgoj";
	int spljcoixpqa = 2537;
	int uikoioww = 268;
	if (2719 != 2719) {
		int pxvattbi;
		for (pxvattbi = 24; pxvattbi > 0; pxvattbi--) {
			continue;
		}
	}
	if (2537 != 2537) {
		int kzxparo;
		for (kzxparo = 67; kzxparo > 0; kzxparo--) {
			continue;
		}
	}
	return true;
}

int cwnkryi::fnmufcawavgulxbliipslph(int tjrpirxslgd, double nmunf, double jovrjk, int begwq) {
	int xnthtsfxf = 1161;
	double yaivqexljzsm = 38449;
	string wevjhrtrv = "dqufrzshcrihgbmtzpdbgwnuueabljebdnovgqmspbtozlqqgqa";
	int njcbrmo = 2763;
	string irimijpajql = "itdzykivggrarsygf";
	if (2763 == 2763) {
		int lfkoxpgj;
		for (lfkoxpgj = 72; lfkoxpgj > 0; lfkoxpgj--) {
			continue;
		}
	}
	if (1161 != 1161) {
		int rypmqd;
		for (rypmqd = 68; rypmqd > 0; rypmqd--) {
			continue;
		}
	}
	if (38449 != 38449) {
		int qzolfwl;
		for (qzolfwl = 31; qzolfwl > 0; qzolfwl--) {
			continue;
		}
	}
	return 57534;
}

bool cwnkryi::ccmiangytsudepoplmd(int ccxfvejryzlsok, int uohwt, bool dqtvj, bool bwdhzriof, string jigtf, double optcitfhswyyocg, double iuvrgikyqw, string uxwvkcsdmfwg, bool rjeeizllrppcrj, bool fpphgfdp) {
	int qaodtwnhfp = 219;
	bool vpekc = false;
	bool aqifsikykvrcsx = true;
	int ulbnhmf = 5960;
	if (false != false) {
		int fausl;
		for (fausl = 6; fausl > 0; fausl--) {
			continue;
		}
	}
	return true;
}

void cwnkryi::itbvsqdaqiafbwsjevudi() {
	string bldqejhlpehk = "idcpppqltglmvlgrbkspacbymqlmsdzkxzqheijsdtbnatktrckdqrakcqlvtng";
	double xpaxy = 11528;
	double ugbnhrd = 3670;
	if (string("idcpppqltglmvlgrbkspacbymqlmsdzkxzqheijsdtbnatktrckdqrakcqlvtng") == string("idcpppqltglmvlgrbkspacbymqlmsdzkxzqheijsdtbnatktrckdqrakcqlvtng")) {
		int hauno;
		for (hauno = 65; hauno > 0; hauno--) {
			continue;
		}
	}
	if (string("idcpppqltglmvlgrbkspacbymqlmsdzkxzqheijsdtbnatktrckdqrakcqlvtng") != string("idcpppqltglmvlgrbkspacbymqlmsdzkxzqheijsdtbnatktrckdqrakcqlvtng")) {
		int vcbjnewzuk;
		for (vcbjnewzuk = 50; vcbjnewzuk > 0; vcbjnewzuk--) {
			continue;
		}
	}
	if (11528 == 11528) {
		int msoyvyzjg;
		for (msoyvyzjg = 2; msoyvyzjg > 0; msoyvyzjg--) {
			continue;
		}
	}
	if (string("idcpppqltglmvlgrbkspacbymqlmsdzkxzqheijsdtbnatktrckdqrakcqlvtng") != string("idcpppqltglmvlgrbkspacbymqlmsdzkxzqheijsdtbnatktrckdqrakcqlvtng")) {
		int yrkhfoaggi;
		for (yrkhfoaggi = 22; yrkhfoaggi > 0; yrkhfoaggi--) {
			continue;
		}
	}

}

cwnkryi::cwnkryi() {
	this->ttpheqilcn(false, 2526, 6395, 1978, string("smvypidncctbo"), false);
	this->fmekuqvxhteziupyhdnrfcx(11100, false, 31929, string("xrbemaoatuzklwyrbmalavlugnspbwzuqxfuadlmlsfsdfrosllusuurltefosnzitpmxczwvfprvljbalxzjlavm"), 691, true, string("jadpsnngeagijkyfhiizqupugkxjcanixbrgughxnjmsissoxueestyyszsxousmfgnnjrxenorwsfyhynyypayfbx"), 1644, string("ohqewhlnfwhgarjckyoufmlmvxxquodthbcwsmthbczfworjcnpeeuidhbzmveuzsqvjojaitdtaikyzvgefyotukfbyksv"));
	this->gzpjhzxzhdciirwp(1212, true, 446, 3669, 4738, 6314, string("jnlpsfcffuwuaegpzbfukpamumqwrqhkeqcvamwtfzdzcixa"), string("tjlahouqoetpofztduzcdajbrnpqxsrpfhdpyovjkmuprhzuruqbtoaxpbfmsvaja"), false, true);
	this->zbgwezuouqvgyowa(string("gudcejscjbbeckwibcyemknbqfbxizpnyjeqhpgjbeoqhbtqw"), 38939);
	this->wjkzumovslfoimcepysjyh(4590, string("bcevxjvdbfdqhbiiseby"), false, string("cjcavlthkukktfphczearpilchzzhxozzgdwcrcaphpmyjikpmwzwotfbhbmcxw"), 3724, string("pexvyuwfpxgkpwywgboaguewhmlmskwu"));
	this->pzstumidpwgzultooxwgkhts(string("jgyxlnvebxlaofkadaofjbjpnfnbfjzktbanvemsijxzugtzzevt"), string("abjosnuokdjequwzcujghmxxoyrtjemeokzttyndoeompqnmbg"), 31682, 879, 3750, string("qfovmiihgkpvxhrswnbvegamihqjkxogexxxqtyudbqnkms"), 67557, 15993);
	this->lqifyjrgvjasjukry(string("btnuxnkkzblsaawvcxvxlymkclpbwfquegxnbxlmvpzqpxuefbobuwkefmtcowxynjmyvnwrxdezwlxxcljzyrvedcswbmckkvh"), 35123, string("vrinxcpvfnmfmsgqypbdnchmjgcctzhscxghmxzjnmdnx"), string("mwnlqusxjgvkltjulremcqtttgugiwyipvsorshvamorajppjuzrffhpamqpwnnegwmklq"), string("aplzzxxtlezmnmohxciidaavmjnzzwnxblbnsznvfncoktgkbfdlz"), 14009, false, 307, 54943);
	this->fnmufcawavgulxbliipslph(5225, 2722, 14857, 244);
	this->ccmiangytsudepoplmd(5099, 101, true, false, string("rufkpvtpqfizypddkhmbffvzgrjjcewgmzkhdazsgkkkqnsa"), 59444, 3156, string("goojrxefdnqysuluuwpvdtzqorgsgulzvjkywhfnxwswagczx"), false, true);
	this->itbvsqdaqiafbwsjevudi();
	this->ouvgmcdhvcscghk(8814, false, 22265, 19717, 19187);
	this->wbsaaosujkm(string("blucaktysqowgycqyylukutpxerebykmtxeglsnyhlascinc"), 4559);
	this->ibtranssfccofaczqjhdwln(string("ogbgbsutbgh"), 21837, true, 737);
	this->ukvnewapwqmqtvltfvrc(1260, string("dukvc"), 6911, 12296, 1685, 19900);
	this->fngkqtazvlqmddmxk(true, 58845, string(""), 7525, true, true, 824, 7972, 10315, 47185);
	this->tbbsnhmbgtobuvbqfrgjitg(string("vmvjulrhegthkfepwbvteydhhjdbnyqikgrpo"), 14330);
	this->zxhgjwtucuyjbepbsolzorf(false, string("qwuqzmilrwnubepnpvlinahazniayjvfsuplmoopcnhvpggldggjlijfgcwqlypmlcntoreyrqdrxmwxxdxolnug"), 359, 5045);
	this->zlriqjgmqvpeqlsldsmjnb();
	this->kygjoyjfyzpeveptyqmld(string("mdcdakspmwqesgfbvcggnexvxltpvmfqskitslzhxmrpvrnnftygtzsntxiqtiiabkcvzyvtdfxiymkrklh"), true, string("beyrjd"), string("abnohgewaqpatwhnxrcjaenwprcdubynioirvtiktntwlkiukrujepftsizpjpsucfmlvnzyrfkwcvmrunngmhwebmcnm"), 4496, string("wynxixnuhthbiwlejikmebpdhgjtmbbgajwgjjvmxcpoezbgabnkslziauisvlekbjoxggllqomtjvdsufvwmejbwoxvslbcu"));
	this->sncmhjgvpoxi(false, true, string("ipmtwerzyrquxexiragqgkppdmsqzbdfpqibpatwrtp"), true, 31047, 17415, 34487, true);
	this->hazkcpbeubhofopxombg(false, false, 5325);
	this->gthaeyokqkfyuhqq(true, string("orfhzmqfzludmcdnrsuecviFytfwqhwagprdbndcfgtbevhcxlffpwssafekfnpovmolwhvqybibxwehazerzfiyey"), 769, 1370);
	this->vnnxfgfrlqrhntnb();
	this->dnkzjdysmrdiedv(41118, 15449);
	this->itbyjssrqqemboukfbae(string("jcxel"), false, 1776, 3350, 31933, string("fmbe"), string("nastakyzconypnufdvtzagxihjvgicouclqvwnysaetfbcuag"));
	this->dfkzulagtrixg(4483);
	this->juakhdczug(false, 3946, string("rvoyajzblhrljlilbmtupyukcpkvotwtlfbgwunbsswddhsuqkandvexcubkffwzpalxrpjuddoesbnaweolltqspketndbzyxs"), false, true, 6845, string("dfkrelhhekzrerqrckgiffjgtbuwxtnpsjiiovlshukimh"), 2391);
}
