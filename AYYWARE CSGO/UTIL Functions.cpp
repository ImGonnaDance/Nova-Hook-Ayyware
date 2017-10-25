/*
ApocalypseCheats
*/

#include "UTIL Functions.h"



#include "Utilities.h"
#include "Menu.h"
#include "Hacks.h"
#include "Autowall.h"

#include "RenderManager.h"



ServerRankRevealAllFn GameUtils::ServerRankRevealAllEx;
IsReadyFn GameUtils::IsReady;

DWORD GameUtils::FindPattern1(std::string moduleName, std::string pattern)
{
	const char* pat = pattern.c_str();
	DWORD firstMatch = 0;
	DWORD rangeStart = (DWORD)GetModuleHandleA(moduleName.c_str());
	MODULEINFO miModInfo; GetModuleInformation(GetCurrentProcess(), (HMODULE)rangeStart, &miModInfo, sizeof(MODULEINFO));
	DWORD rangeEnd = rangeStart + miModInfo.SizeOfImage;
	for (DWORD pCur = rangeStart; pCur < rangeEnd; pCur++)
	{
		if (!*pat)
			return firstMatch;

		if (*(PBYTE)pat == '\?' || *(BYTE*)pCur == getByte(pat))
		{
			if (!firstMatch)
				firstMatch = pCur;

			if (!pat[2])
				return firstMatch;

			if (*(PWORD)pat == '\?\?' || *(PBYTE)pat != '\?')
				pat += 3;

			else
				pat += 2;    //one ?
		}
		else
		{
			pat = pattern.c_str();
			firstMatch = 0;
		}
	}
	return NULL;
}

void UTIL_TraceLine(const Vector& vecAbsStart, const Vector& vecAbsEnd, unsigned int mask, const IClientEntity *ignore, int collisionGroup, trace_t *ptr)
{
	typedef int(__fastcall* UTIL_TraceLine_t)(const Vector&, const Vector&, unsigned int, const IClientEntity*, int, trace_t*);
	static UTIL_TraceLine_t TraceLine = (UTIL_TraceLine_t)Utilities::Memory::FindPattern("client.dll", (PBYTE)"\x55\x8B\xEC\x83\xE4\xF0\x83\xEC\x7C\x56\x52", "xxxxxxxxxxx");
	TraceLine(vecAbsStart, vecAbsEnd, mask, ignore, collisionGroup, ptr);
}


void UTIL_ClipTraceToPlayers(const Vector& vecAbsStart, const Vector& vecAbsEnd, unsigned int mask, ITraceFilter* filter, trace_t* tr)
{
	static DWORD dwAddress = Utilities::Memory::FindPattern("client.dll", (BYTE*)"\x53\x8B\xDC\x83\xEC\x08\x83\xE4\xF0\x83\xC4\x04\x55\x8B\x6B\x04\x89\x6C\x24\x04\x8B\xEC\x81\xEC\x00\x00\x00\x00\x8B\x43\x10", "xxxxxxxxxxxxxxxxxxxxxxxx????xxx");

	if (!dwAddress)
		return;

	_asm
	{
		MOV		EAX, filter
		LEA		ECX, tr
		PUSH	ECX
		PUSH	EAX
		PUSH	mask
		LEA		EDX, vecAbsEnd
		LEA		ECX, vecAbsStart
		CALL	dwAddress
		ADD		ESP, 0xC
	}
}

bool IsBreakableEntity(IClientEntity* ent)
{
	typedef bool(__thiscall* IsBreakbaleEntity_t)(IClientEntity*);
	IsBreakbaleEntity_t IsBreakbaleEntityFn = (IsBreakbaleEntity_t)Utilities::Memory::FindPattern("client.dll", (PBYTE)"\x55\x8B\xEC\x51\x56\x8B\xF1\x85\xF6\x74\x68", "xxxxxxxxxxx");
	if (IsBreakbaleEntityFn)
		return IsBreakbaleEntityFn(ent);
	else
		return false;
}

bool TraceToExit(Vector& end, trace_t& tr, Vector start, Vector vEnd, trace_t* trace)
{
	typedef bool(__fastcall* TraceToExitFn)(Vector&, trace_t&, float, float, float, float, float, float, trace_t*);
	static TraceToExitFn TraceToExit = (TraceToExitFn)Utilities::Memory::FindPattern("client.dll", (BYTE*)"\x55\x8B\xEC\x83\xEC\x30\xF3\x0F\x10\x75", "xxxxxxxxxx");

	if (!TraceToExit)
		return false;

	return TraceToExit(end, tr, start.x, start.y, start.z, vEnd.x, vEnd.y, vEnd.z, trace);
}

/*unsigned short GameUtils::UTIL_GetAchievementEventMask()
{
static uintptr_t GetAchievementEventMask_func = Utilities::Memory::FindPattern("client.dll", (PBYTE)"\x55\x8B\xEC\x81\xEC\x00\x00\x00\x00\x8B\x0D\x00\x00\x00\x00\x68\x00\x00\x00\x00", "xxxxx????xx????x????");
typedef unsigned short(__fastcall* UTIL_GetAchievementEventMask_t)();
static UTIL_GetAchievementEventMask_t pUTIL_GetAchievementEventMask =
reinterpret_cast<UTIL_GetAchievementEventMask_t>(GetAchievementEventMask_func);
return pUTIL_GetAchievementEventMask();
}*/

void GameUtils::NormaliseViewAngle(Vector &angle)
{
	if (!Menu::Window.MiscTab.OtherSafeMode.GetState())
	{
		return;
	}
	else
	{
		while (angle.y <= -180) angle.y += 360;
		while (angle.y > 180) angle.y -= 360;
		while (angle.x <= -180) angle.x += 360;
		while (angle.x > 180) angle.x -= 360;


		if (angle.x > 89) angle.x = 89;
		if (angle.x < -89) angle.x = -89;
		if (angle.y < -180) angle.y = -179.999;
		if (angle.y > 180) angle.y = 179.999;

		angle.z = 0;
	}
}

void GameUtils::CL_FixMove(CUserCmd* pCmd, Vector viewangles)
{
	/*pCmd->forwardmove = DotProduct(forward * vForwardNorm, aimforward) + DotProduct(right * vRightNorm, aimforward) + DotProduct(up * vUpNorm, aimforward);
	pCmd->sidemove = DotProduct(forward * vForwardNorm, aimright) + DotProduct(right * vRightNorm, aimright) + DotProduct(up * vUpNorm, aimright);
	pCmd->upmove = DotProduct(forward * vForwardNorm, aimup) + DotProduct(right * vRightNorm, aimup) + DotProduct(up * vUpNorm, aimup);*/
}

char shit[16];
trace_t Trace;
char shit2[16];
IClientEntity* entCopy;

bool GameUtils::IsG(void* weapon)
{
	if (weapon == nullptr) return false;
	CBaseCombatWeapon *pWeapon = (CBaseCombatWeapon*)weapon;
	int id = *pWeapon->m_AttributeManager()->m_Item()->ItemDefinitionIndex();
	static const std::vector<int> v = { WEAPON_FLASHBANG,WEAPON_HEGRENADE,WEAPON_SMOKEGRENADE,WEAPON_MOLOTOV,WEAPON_DECOY,WEAPON_INCGRENADE };
	return (std::find(v.begin(), v.end(), id) != v.end());
}

bool GameUtils::IsVisible(IClientEntity* pLocal, IClientEntity* pEntity, int BoneID)
{
	if (BoneID < 0) return false;

	entCopy = pEntity;
	Vector start = pLocal->GetOrigin() + pLocal->GetViewOffset();
	Vector end = GetHitboxPosition(pEntity, BoneID);//pEntity->GetBonePos(BoneID);
	char shit3[32];

	//Interfaces::Trace->TraceRay(Ray,MASK_SOLID, NULL/*&filter*/, &Trace);
	UTIL_TraceLine(start, end, MASK_SOLID, pLocal, 0, &Trace);

	if (Trace.m_pEnt == entCopy)
	{
		return true;
	}

	if (Trace.fraction == 1.0f)
	{
		return true;
	}

	return false;

}

bool GameUtils::IsGun(void* weapon) {
	if (weapon == nullptr) return false;
	CBaseCombatWeapon *pWeapon = (CBaseCombatWeapon*)weapon;
	int id = *pWeapon->m_AttributeManager()->m_Item()->ItemDefinitionIndex();
	switch (id) {
	case WEAPON_DEAGLE:
	case WEAPON_ELITE:
	case WEAPON_FIVESEVEN:
	case WEAPON_GLOCK:
	case WEAPON_AK47:
	case WEAPON_AUG:
	case WEAPON_AWP:
	case WEAPON_FAMAS:
	case WEAPON_G3SG1:
	case WEAPON_GALILAR:
	case WEAPON_M249:
	case WEAPON_M4A1:
	case WEAPON_MAC10:
	case WEAPON_P90:
	case WEAPON_UMP45:
	case WEAPON_XM1014:
	case WEAPON_BIZON:
	case WEAPON_MAG7:
	case WEAPON_NEGEV:
	case WEAPON_SAWEDOFF:
	case WEAPON_TEC9:
		return true;
	case WEAPON_TASER:
		return true;
	case WEAPON_HKP2000:
	case WEAPON_MP7:
	case WEAPON_MP9:
	case WEAPON_NOVA:
	case WEAPON_P250:
	case WEAPON_SCAR20:
	case WEAPON_SG556:
	case WEAPON_SSG08:
		return true;
	case WEAPON_KNIFE_CT:
	case WEAPON_KNIFE_T:
	case WEAPON_FLASHBANG:
	case WEAPON_HEGRENADE:
	case WEAPON_SMOKEGRENADE:
	case WEAPON_MOLOTOV:
	case WEAPON_DECOY:
	case WEAPON_INCGRENADE:
	case WEAPON_C4:
		return false;
	case WEAPON_M4A1_SILENCER:
	case WEAPON_USP_SILENCER:
	case WEAPON_CZ75A:
	case WEAPON_REVOLVER:
		return true;
	default:
		return false;
	}
}


bool GameUtils::IsBallisticWeapon(void* weapon)
{
	if (weapon == nullptr) return false;
	CBaseCombatWeapon *pWeapon = (CBaseCombatWeapon*)weapon;
	int id = *pWeapon->m_AttributeManager()->m_Item()->ItemDefinitionIndex();
	return !(id >= WEAPON_KNIFE_CT && id <= WEAPON_KNIFE_T || id == 0 || id >= WEAPON_KNIFE_BAYONET);
}

bool GameUtils::IsPistol(void* weapon)
{
	if (weapon == nullptr) return false;
	CBaseCombatWeapon *pWeapon = (CBaseCombatWeapon*)weapon;
	int id = *pWeapon->m_AttributeManager()->m_Item()->ItemDefinitionIndex();
	static const std::vector<int> v = { WEAPON_DEAGLE,WEAPON_CZ75A,WEAPON_ELITE,WEAPON_USP_SILENCER,WEAPON_P250,WEAPON_HKP2000, WEAPON_TEC9,WEAPON_FIVESEVEN,WEAPON_GLOCK };
	return (std::find(v.begin(), v.end(), id) != v.end());
}

bool GameUtils::IsRevolver(void* weapon)
{
	if (weapon == nullptr) return false;
	CBaseCombatWeapon *pWeapon = (CBaseCombatWeapon*)weapon;
	int id = *pWeapon->m_AttributeManager()->m_Item()->ItemDefinitionIndex();
	static const std::vector<int> v = { WEAPON_REVOLVER };
	return (std::find(v.begin(), v.end(), id) != v.end());
}

bool GameUtils::IsSniper(void* weapon)
{
	if (weapon == nullptr) return false;
	CBaseCombatWeapon *pWeapon = (CBaseCombatWeapon*)weapon;
	int id = *pWeapon->m_AttributeManager()->m_Item()->ItemDefinitionIndex();
	static const std::vector<int> v = { WEAPON_AWP,WEAPON_SSG08,WEAPON_G3SG1,WEAPON_SCAR20 };
	return (std::find(v.begin(), v.end(), id) != v.end());
}

bool GameUtils::IsScopedWeapon(void* weapon)
{
	if (weapon == nullptr) return false;
	CBaseCombatWeapon *pWeapon = (CBaseCombatWeapon*)weapon;
	int id = *pWeapon->m_AttributeManager()->m_Item()->ItemDefinitionIndex();
	static const std::vector<int> v = { WEAPON_AWP,WEAPON_SSG08,WEAPON_G3SG1,WEAPON_SCAR20, WEAPON_AUG, WEAPON_SG556 };
	return (std::find(v.begin(), v.end(), id) != v.end());
}

bool GameUtils::IsShotgun(void* weapon)
{
	if (weapon == nullptr) return false;
	IClientEntity* weaponEnt = (IClientEntity*)weapon;
	ClientClass* pWeaponClass = weaponEnt->GetClientClass();

	if (pWeaponClass->m_ClassID == (int)CSGOClassID::CWeaponMag7 || pWeaponClass->m_ClassID == (int)CSGOClassID::CWeaponXM1014 || pWeaponClass->m_ClassID == (int)CSGOClassID::CWeaponSawedoff || pWeaponClass->m_ClassID == (int)CSGOClassID::CWeaponNOVA)
		return true;
	else
		return false;
}

bool GameUtils::IsMachinegun(void* weapon)
{
	if (weapon == nullptr) return false;
	IClientEntity* weaponEnt = (IClientEntity*)weapon;
	ClientClass* pWeaponClass = weaponEnt->GetClientClass();

	if (pWeaponClass->m_ClassID == (int)CSGOClassID::CWeaponNegev || pWeaponClass->m_ClassID == (int)CSGOClassID::CWeaponM249)
		return true;
	else
		return false;
}

bool GameUtils::IsMP(void* weapon)
{
	if (weapon == nullptr) return false;
	IClientEntity* weaponEnt = (IClientEntity*)weapon;
	ClientClass* pWeaponClass = weaponEnt->GetClientClass();

	if (pWeaponClass->m_ClassID == (int)CSGOClassID::CWeaponMAC10 || pWeaponClass->m_ClassID == (int)CSGOClassID::CWeaponP90 || pWeaponClass->m_ClassID == (int)CSGOClassID::CWeaponUMP45 || pWeaponClass->m_ClassID == (int)CSGOClassID::CWeaponBizon || pWeaponClass->m_ClassID == (int)CSGOClassID::CWeaponMP7 || pWeaponClass->m_ClassID == (int)CSGOClassID::CWeaponMP9)
		return true;
	else
		return false;
}


void SayInChat(const char *text)
{
	char buffer[250];
	sprintf_s(buffer, "say \"%s\"", text);
	Interfaces::Engine->ClientCmd_Unrestricted(buffer);
}

void SayInTeamChat(const char *text)
{
	char buffer[250];
	sprintf_s(buffer, "say_team \"%s\"", text);
	Interfaces::Engine->ClientCmd_Unrestricted(buffer);
}


Vector GetHitboxPosition(IClientEntity* pEntity, int Hitbox)
{
	matrix3x4 matrix[128];
	Vector vMin, vMax, vCenter, sCenter;
	if (pEntity->SetupBones(matrix, 128, 0x100, 0.f))
	{
		studiohdr_t* hdr = Interfaces::ModelInfo->GetStudiomodel(pEntity->GetModel());
		mstudiohitboxset_t* set = hdr->GetHitboxSet(0);

		mstudiobbox_t* hitbox = set->GetHitbox(Hitbox);

		//Points to MultiPoint
		Vector Point[] =
		{
			Vector(hitbox->bbmin.x, hitbox->bbmin.y, hitbox->bbmin.z),
			Vector(hitbox->bbmin.x, hitbox->bbmax.y, hitbox->bbmin.z),
			Vector(hitbox->bbmax.x, hitbox->bbmax.y, hitbox->bbmin.z),
			Vector(hitbox->bbmax.x, hitbox->bbmin.y, hitbox->bbmin.z),
			Vector(hitbox->bbmax.x, hitbox->bbmax.y, hitbox->bbmax.z),
			Vector(hitbox->bbmin.x, hitbox->bbmax.y, hitbox->bbmax.z),
			Vector(hitbox->bbmin.x, hitbox->bbmin.y, hitbox->bbmax.z),
			Vector(hitbox->bbmax.x, hitbox->bbmin.y, hitbox->bbmax.z)
		};

		if (hitbox)
		{

			


			//Just Hitscan
			VectorTransform(hitbox->bbmin, matrix[hitbox->bone], vMin);
			VectorTransform(hitbox->bbmax, matrix[hitbox->bone], vMax);

			vCenter = ((vMin + vMax) *0.5f);

			return vCenter;

			//Multipoint
			if (Menu::Window.RageBotTab.TargetMultipoint.GetState())
			{
				int iCount = 7;
				for (int i = 0; i <= iCount; i++)
				{
					Vector vTargetPos;
					switch (i)
					{
					case 0:
					default:
						vTargetPos = vCenter; break;
					case 1:
						vTargetPos = (Point[7] + Point[1]) * Menu::Window.RageBotTab.TargetPointscale.GetValue(); break;
					case 2:
						vTargetPos = (Point[3] + Point[4]) * Menu::Window.RageBotTab.TargetPointscale.GetValue(); break;
					case 3:
						vTargetPos = (Point[4] + Point[0]) * Menu::Window.RageBotTab.TargetPointscale.GetValue(); break;
					case 4:
						vTargetPos = (Point[2] + Point[7]) * Menu::Window.RageBotTab.TargetPointscale.GetValue(); break;
					case 5:
						vTargetPos = (Point[6] + Point[7]) * Menu::Window.RageBotTab.TargetPointscale.GetValue(); break;
					case 6:
						vTargetPos = (Point[5] + Point[3]) * Menu::Window.RageBotTab.TargetPointscale.GetValue(); break;
					case 7:
						vTargetPos = (Point[1] + Point[2]) * Menu::Window.RageBotTab.TargetPointscale.GetValue(); break;
					}
					return vTargetPos;
				}
			}

			
		}
	}

	return Vector(0, 0, 0);
}


bool GetBestPoint(IClientEntity* pEntity, Hitbox* hitbox, BestPoint *point)
{
Vector center = hitbox->points[0];
std::vector<int> HitBoxesToScan;

if (hitbox->hitbox == ((int)CSGOHitboxID::Head))
{
Vector high = ((hitbox->points[3] + hitbox->points[5]) * .5f);

float pitch = pEntity->GetEyeAngles().x;
if ((pitch > 0.f) && (pitch <= 89.f))
{
Vector height = (((high - hitbox->points[0]) / 3) * 4);
Vector new_height = (hitbox->points[0] + (height * (pitch / 89.f)));

hitbox->points[0] = new_height;
point->flags |= FL_HIGH;
}
else if ((pitch < 292.5f) && (pitch >= 271.f))
{
hitbox->points[0] -= Vector(0.f, 0.f, 1.f);
point->flags |= FL_LOW;
}
}

for (int index = 0; index <= 8; ++index)
{
	int temp_damage;
	float Damage = 0.f;
	if (CanHit(hitbox->points[index], &Damage)) {
		 temp_damage = Damage;
	}
	else {
		 temp_damage = 0;
	}

//int temp_damage = CanHit(hitbox->points[index], &Damage);

	if ((point->dmg < temp_damage))
	{
		point->dmg = temp_damage;
		point->point = hitbox->points[index];
		point->index = index;
	}
}

return (point->dmg > Menu::Window.RageBotTab.AccuracyMinimumDamage.GetValue());
}

/*Vector GetHitboxPosition(IClientEntity* pEntity, int Hitbox)
{
matrix3x4 matrix[128];

if (pEntity->SetupBones(matrix, 128, 0x00000100, GetTickCount64()))
{
studiohdr_t* hdr = Interfaces::ModelInfo->GetStudiomodel(pEntity->GetModel());
mstudiohitboxset_t* set = hdr->GetHitboxSet(0);

mstudiobbox_t* hitbox = set->GetHitbox(Hitbox);

if (hitbox)
{
Vector points[9] =
{
((hitbox->bbmin + hitbox->bbmax) * .5f),
Vector(hitbox->bbmin.x, hitbox->bbmin.y, hitbox->bbmin.z),
Vector(hitbox->bbmin.x, hitbox->bbmax.y, hitbox->bbmin.z),
Vector(hitbox->bbmax.x, hitbox->bbmax.y, hitbox->bbmin.z),
Vector(hitbox->bbmax.x, hitbox->bbmin.y, hitbox->bbmin.z),
Vector(hitbox->bbmax.x, hitbox->bbmax.y, hitbox->bbmax.z),
Vector(hitbox->bbmin.x, hitbox->bbmax.y, hitbox->bbmax.z),
Vector(hitbox->bbmin.x, hitbox->bbmin.y, hitbox->bbmax.z),
Vector(hitbox->bbmax.x, hitbox->bbmin.y, hitbox->bbmax.z)
};

Vector vPoint = ((hitbox->bbmin + hitbox->bbmax) * .5f);

for (int index = 0; index <= 8; ++index)
{
if (index != 0)
{
// scale down the hitbox size
points[index] = ((((points[index] + points[0]) * .5f) + points[index]) * .5f);
}

// transform the vector
VectorTransform(points[index], matrix[hitbox->bone], vPoint[index]);
}

return vPoint;
}
}

return Vector(0, 0, 0);
}*/

/*Vector GetHitboxPosition(IClientEntity* pEntity, int Hitbox)
{
matrix3x4 matrix[128];

if (pEntity->SetupBones(matrix, 128, 0x00000100, GetTickCount64()))
{
studiohdr_t* hdr = Interfaces::ModelInfo->GetStudiomodel(pEntity->GetModel());
mstudiohitboxset_t* set = hdr->GetHitboxSet(0);

mstudiobbox_t* hitbox = set->GetHitbox(Hitbox);

if (hitbox)
{
Vector vMin, vMax, vCenter, sCenter;
VectorTransform(hitbox->bbmin, matrix[hitbox->bone], vMin);
VectorTransform(hitbox->bbmax, matrix[hitbox->bone], vMax);
vCenter = (vMin + vMax) *0.5f;
return vCenter;
}
}

return Vector(0, 0, 0);
}*/

/*Vector GetHitboxPosition(IClientEntity* pEntity, int Hitbox)
{
matrix3x4 matrix[128];

if (!pEntity->SetupBones(matrix, 128, 0x00000100, GetTickCount64()))
return Vector(0,0,0);

studiohdr_t* hdr = Interfaces::ModelInfo->GetStudiomodel(pEntity->GetModel());
mstudiohitboxset_t* set = hdr->GetHitboxSet(0);

mstudiobbox_t* hitbox = set->GetHitbox(Hitbox);

if (!hitbox)
return Vector(0,0,0);

Vector vMin, vMax, vCenter, sCenter;
VectorTransform(hitbox->bbmin, matrix[hitbox->bone], vMin);
VectorTransform(hitbox->bbmax, matrix[hitbox->bone], vMax);
vCenter = (vMin + vMax) *0.5f;
return vCenter;
}*/

/*bool CRageBot::GetHitbox(CBaseEntity* target, Hitbox* hitbox)
{
matrix3x4a_t matrix[MAXSTUDIOBONES];

if (!target->SetupBones(matrix, MAXSTUDIOBONES, BONE_USED_BY_HITBOX, 0))
return false;


studiohdr_t *hdr = I::ModelInfo->GetStudioModel(target->GetModel());

if (!hdr)
return false;

mstudiohitboxset_t *hitboxSet = hdr->pHitboxSet(target->GetHitboxSet());
mstudiobbox_t *untransformedBox = hitboxSet->pHitbox(hitbox->hitbox);

Vector bbmin = untransformedBox->bbmin;
Vector bbmax = untransformedBox->bbmax;

if (untransformedBox->radius != -1.f)
{
bbmin -= Vector(untransformedBox->radius, untransformedBox->radius, untransformedBox->radius);
bbmax += Vector(untransformedBox->radius, untransformedBox->radius, untransformedBox->radius);
}

Vector points[] =
{ ((bbmin + bbmax) * .5f),
Vector(bbmin.x, bbmin.y, bbmin.z),
Vector(bbmin.x, bbmax.y, bbmin.z),
Vector(bbmax.x, bbmax.y, bbmin.z),
Vector(bbmax.x, bbmin.y, bbmin.z),
Vector(bbmax.x, bbmax.y, bbmax.z),
Vector(bbmin.x, bbmax.y, bbmax.z),
Vector(bbmin.x, bbmin.y, bbmax.z),
Vector(bbmax.x, bbmin.y, bbmax.z)
};


for (int index = 0; index <= 8; ++index)
{
if (index != 0)
points[index] = ((((points[index] + points[0]) * .5f) + points[index]) * .5f);

M::VectorTransform(points[index], matrix[untransformedBox->bone], hitbox->points[index]);
}

return true;
}*/

/*Vector GetHitscan(IClientEntity* pEntity, int Hitbox)
{
matrix3x4 matrix[128];

if (!pEntity->SetupBones(matrix, 128, 0x00000100, GetTickCount64()))
return Vector(0, 0, 0);

studiohdr_t* hdr = Interfaces::ModelInfo->GetStudiomodel(pEntity->GetModel());
mstudiohitboxset_t* set = hdr->GetHitboxSet(0);

for (int i = 0; i < 18; i++)
{
mstudiobbox_t* hitbox = set->GetHitbox(i);
if (!hitbox)
continue;

Vector vAimPoint;

Vector corners[17] =
{
Vector((hitbox->bbmin + hitbox->bbmax) * 0.5),
Vector(hitbox->bbmax.x - 1.65, hitbox->bbmax.y - 1.65, hitbox->bbmin.z + 1.65),
Vector(hitbox->bbmax.x - 1.65, hitbox->bbmax.y - 1.65, hitbox->bbmax.z - 1.65),
Vector(hitbox->bbmin.x + 1.65, hitbox->bbmax.y - 1.65, hitbox->bbmin.z + 1.65),
Vector(hitbox->bbmin.x + 1.65, hitbox->bbmax.y - 1.65, hitbox->bbmax.z - 1.65),
Vector(hitbox->bbmax.x - 1.65, hitbox->bbmin.y + 1.65, hitbox->bbmin.z + 1.65),
Vector(hitbox->bbmax.x - 1.65, hitbox->bbmin.y + 1.65, hitbox->bbmax.z - 1.65),
Vector(hitbox->bbmin.x + 1.65, hitbox->bbmin.y + 1.65, hitbox->bbmin.z + 1.65),
Vector(hitbox->bbmin.x + 1.65, hitbox->bbmin.y + 1.65, hitbox->bbmax.z - 1.65),
Vector(hitbox->bbmin.x, hitbox->bbmin.y, hitbox->bbmin.z),
Vector(hitbox->bbmin.x, hitbox->bbmin.y, hitbox->bbmax.z),
Vector(hitbox->bbmin.x, hitbox->bbmax.y, hitbox->bbmin.z),
Vector(hitbox->bbmin.x, hitbox->bbmax.y, hitbox->bbmax.z),
Vector(hitbox->bbmax.x, hitbox->bbmin.y, hitbox->bbmin.z),
Vector(hitbox->bbmax.x, hitbox->bbmin.y, hitbox->bbmax.z),
Vector(hitbox->bbmax.x, hitbox->bbmax.y, hitbox->bbmin.z),
Vector(hitbox->bbmax.x, hitbox->bbmax.y, hitbox->bbmax.z)
};

for (int j = 0; j < 17; j++)
{
if (!corners[j])
continue;

VectorTransform(corners[j], matrix[hitbox->bone], vAimPoint);
}

return vAimPoint;
}
}*/

Vector GetEyePosition(IClientEntity* pEntity)
{
	Vector vecViewOffset = *reinterpret_cast<Vector*>(reinterpret_cast<int>(pEntity) + 0x104);

	return pEntity->GetOrigin() + vecViewOffset;
}

int GameUtils::GetPlayerCompRank(IClientEntity* pEntity) // 0x75671f7f is crc32 for comp rank netvar, 0x2ED6198 is CCSPlayerResource, 0x1A44 is netvar offset
{
	DWORD m_iCompetitiveRanking = NetVar.GetNetVar(0x75671F7F); //NetVar.GetNetVar(0x75671F7F);
	DWORD GameResources = *(DWORD*)(Utilities::Memory::FindPattern("client.dll", (PBYTE)"\x8B\x3D\x00\x00\x00\x00\x85\xFF\x0F\x84\x00\x00\x00\x00\x81\xC7", "xx????xxxx????xx") + 0x2);

	return *(int*)((DWORD)GameResources + 0x1A44 + (int)pEntity->GetIndex() * 4);
}

extern void GameUtils::ServerRankRevealAll()
{
	static float fArray[3] = { 0.f, 0.f, 0.f };

	GameUtils::ServerRankRevealAllEx = (ServerRankRevealAllFn)(Offsets::Functions::dwGetPlayerCompRank);
	//GameUtils::ServerRankRevealAllEx = (ServerRankRevealAllFn)(offsets.ServerRankRevealAllEx);
	GameUtils::ServerRankRevealAllEx(fArray);
}

void ForceUpdate()
{
	// Shh
	static DWORD clientstateaddr = Utilities::Memory::FindPattern("engine.dll", (PBYTE)"\x8B\x3D\x00\x00\x00\x00\x8A\xF9\xF3\x0F\x11\x45\xF8\x83\xBF\xE8\x00\x00\x00\x02", "xx????xxxxxxxxxxxxxx");
	static uintptr_t pEngineBase = (uintptr_t)GetModuleHandleA("engine.dll");

	static uintptr_t pClientState = **(uintptr_t**)(Utilities::Memory::FindPattern("engine.dll", (PBYTE)"\x8B\x3D\x00\x00\x00\x00\x8A\xF9", "xx????xx") + 2);

	static uintptr_t dwAddr1 = Utilities::Memory::FindPattern("engine.dll", (PBYTE)"\xE8\x00\x00\x00\x00\x68\x00\x00\x00\x00\x68\x00\x00\x00\x00\xC7\x87\x00\x00\x00\x00\x00\x00\x00\x00", "x????x????x????xx????????");

	//E8 call is being used here
	static uintptr_t dwRelAddr = *(uintptr_t*)(dwAddr1 + 1);
	static uintptr_t sub_B5E60 = ((dwAddr1 + 5) + dwRelAddr);

	__asm
	{
		pushad
		mov edi, pClientState
		lea ecx, dword ptr[edi + 0x8]
		call sub_B5E60
		mov dword ptr[edi + 0x154], 0xFFFFFFFF
		popad
	}
}

/*NET_SetConVar::NET_SetConVar(const char* name, const char* value)
{
typedef void(__thiscall* SetConVarConstructor_t)(void*);
static SetConVarConstructor_t pNET_SetConVar = (SetConVarConstructor_t)(Utilities::Memory::FindPattern("engine.dll", (PBYTE)"\x83\xE9\x04\xE9\x00\x00\x00\x00\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\x56\x8B\xF1\xC7\x06\x00\x00\x00\x00\x8D\x4E\x08", "xxxx????xxxxxxxxxxxxxxxx????xxx") + 19);
pNET_SetConVar(this);

typedef void(__thiscall* SetConVarInit_t)(void*, const char*, const char*);
static SetConVarInit_t pNET_SetConVarInit = (SetConVarInit_t)Utilities::Memory::FindPattern("engine.dll", (PBYTE)"\x55\x8B\xEC\x56\x8B\xF1\x57\x83\x4E\x14\x01\x83\x7E\x0C\x00", "xxxxxxxxxxxxxxx");
pNET_SetConVarInit(this, name, value);
}

NET_SetConVar::~NET_SetConVar()
{
typedef void(__thiscall* SetConVarConstructor_t)(void*);
static SetConVarConstructor_t pNET_SetConVar = (SetConVarConstructor_t)(Utilities::Memory::FindPattern("engine.dll", (PBYTE)"\x00\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\x56\x8B\xF1\x57\x8D\x7E\x04\xC7\x07\x00\x00\x00\x00", "xxxxxxxxxxxxxxxxxxxxx????") + 12);
pNET_SetConVar(this);
}*/

#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class nrdnlzx {
public:
	bool nnvtgrpdhfv;
	string czhqhgcbza;
	int rzevxrubngw;
	bool pvcfdvskheqtw;
	nrdnlzx();
	bool tknwicyhvnqur(int xmhygksrjlu, bool ovwqzxsgd, string cflzkgjdafzrg, double beuydmihsvtp, double mugbpmboerkhp, bool pkflk, double ivaxixiji);
	string tomgqilwocbepzc(double qvehbu);
	double cvujvydmfdsht(string trifcudffhc);
	void hjsgypjzxbjmeixbgb(bool eaodbcrx, double gwpjnag, string jgxpwu, double kazsy, int osbrwxrqxp, double imexdrv);
	double httjyowwufkkypdhvqle(double zhkqhmvkdks, double owuxomulfelwk, bool cyhpowilmnnqd, string omthsqhpfcw, string drgcmlocyzhjuwm, bool ucaafg);
	string tvyvtilcaqbxsdrszu(bool twpdktqkqulcqik, string pkwzsxmztu, string pogrivdfp, double mlifvcllwyb, double yirvbjevmdax, bool iaypcq, bool uokww, string egscho, double mmgezivcrf, int cdghrixiosqbwh);
	bool iqqorilaehu(string wznlhn, string edfyicsiqi, int wrwnhlwzi, bool oefwvwq);

protected:
	double ovnxlfbnl;
	double vqqeyvgjd;
	int ttzqavwehjbzq;
	double gfvwgobjyyi;
	int odybzyxnhgbkal;

	bool mugriuvjdtvhpokpugf(bool qotpaviyc, int osighbaxixp, string sdxrwmiftwrukq, int vdfkmhk);
	bool ljyszdprwwwhlqsdbxg(bool nbvxhsw, string dorihp, string zmodqguskfg, int jmbdxqmabzz, double pfejpbibrtlpwkz, bool lrumtpnc, double orqvxnlafdzgz, double yeqjgdorllf);
	bool zlrnpukezvjebcemtdpcn(double qkwsvviyq, double fqgsdhfomb, int dokzjq);
	void ggwgdxqoeokjbiusbkp(bool mwpisqyygsd, string eljzuj, int gtxyibezb, double mywkrquwtdamhim, bool llyazmb, bool yumavok, double hxcgmtg, int rnenckbxt);
	int evdvzuznas(double xzjrpigvww, int wycbidr, double uhkidm, int uqnakxehqc, int bxnjy);
	double shhxtludbhjistdhzebf(double ocimhh, int pansyclkownpxqv, bool mwhiqdvjhlqqo);
	void xafetblmhyvcfosnesspn(bool lohoudud, bool prucyarso, string zjhpipzeielgb, bool cmrnhdeuxuix, string fdxpee, double drkouwqdr);
	string dxqbfuatlgrh(string ijavkqqwva, double piidugmribmpbtk, bool akgmzfcfji, double eedxxvanl, bool onxui, bool rxeoushwrlxs, string dgyluigaozwa, int psglchbmrri, string picgjj, bool awxsxyo);

private:
	bool osmudwpvrbheb;
	bool hscxzfgjwyirb;
	double vioechyceol;
	int gmagm;

	int itlbwrcqgnqqmshii(bool jkmbwljnrwfcxlt, double slhtwmplwuaubd, double iuhec, string iyjlescibi);
	void imnufegdjvdkrlonmw(string csqhlhvdzjawupj, int yvdpljftnja, string bcwih, double jfceekboldhdcr, bool twegh);
	string ozkgtrjowqxxynaqhg(int nrzsj, double tlqrdan, bool mcarukgscjqpdap, int ckkyyov, double rzhxyyc, bool bklcbuibes, int bnypnwiweahdmc);

};


int nrdnlzx::itlbwrcqgnqqmshii(bool jkmbwljnrwfcxlt, double slhtwmplwuaubd, double iuhec, string iyjlescibi) {
	int nxzaiuzumiypz = 1239;
	int tdsmi = 1756;
	bool jgqifdwbf = false;
	int diijohsf = 2116;
	int iaidgus = 3719;
	double lumpntlaj = 51159;
	bool nrhiakacnwvyvq = true;
	int escwrlyiazxbmmw = 2194;
	if (2194 == 2194) {
		int uwskefosa;
		for (uwskefosa = 14; uwskefosa > 0; uwskefosa--) {
			continue;
		}
	}
	if (2116 == 2116) {
		int oudybgnb;
		for (oudybgnb = 77; oudybgnb > 0; oudybgnb--) {
			continue;
		}
	}
	if (true == true) {
		int bzt;
		for (bzt = 61; bzt > 0; bzt--) {
			continue;
		}
	}
	if (false != false) {
		int ntdzm;
		for (ntdzm = 40; ntdzm > 0; ntdzm--) {
			continue;
		}
	}
	return 59038;
}

void nrdnlzx::imnufegdjvdkrlonmw(string csqhlhvdzjawupj, int yvdpljftnja, string bcwih, double jfceekboldhdcr, bool twegh) {
	string ooqtfrcnhw = "ciiygirkhejxelpajvurwrrbo";
	string vyqalbzlq = "lajwwokvgacnlmahfvhkcmgpuztrasggbfacaqweluwbmfamfvb";
	string twrmwix = "zmavotfhwsqufncvyglltwaaamycwuqeetvxozfavmksganojybmbmgxnrgaypcskn";
	bool ldrjpvd = false;
	double wnifirgflcg = 47690;
	bool kcizjexkf = false;
	double jibzncdx = 48538;
	int yvvzzaguq = 320;
	if (false != false) {
		int blkhb;
		for (blkhb = 4; blkhb > 0; blkhb--) {
			continue;
		}
	}

}

string nrdnlzx::ozkgtrjowqxxynaqhg(int nrzsj, double tlqrdan, bool mcarukgscjqpdap, int ckkyyov, double rzhxyyc, bool bklcbuibes, int bnypnwiweahdmc) {
	double eerdidne = 27405;
	if (27405 == 27405) {
		int emzsd;
		for (emzsd = 29; emzsd > 0; emzsd--) {
			continue;
		}
	}
	if (27405 == 27405) {
		int lhybecynmg;
		for (lhybecynmg = 49; lhybecynmg > 0; lhybecynmg--) {
			continue;
		}
	}
	if (27405 == 27405) {
		int ysfrja;
		for (ysfrja = 78; ysfrja > 0; ysfrja--) {
			continue;
		}
	}
	return string("jdrmabpeh");
}

bool nrdnlzx::mugriuvjdtvhpokpugf(bool qotpaviyc, int osighbaxixp, string sdxrwmiftwrukq, int vdfkmhk) {
	bool ggglqyagnvcen = true;
	bool cuxvfol = false;
	string fllrnxpzrqalcz = "lonujltvnurqdnfeyeszlyfnijhpjamrcqadyjzxwpdoqjkvylk";
	double kpisvvomuulbtfm = 2591;
	if (true == true) {
		int wxzagmqc;
		for (wxzagmqc = 57; wxzagmqc > 0; wxzagmqc--) {
			continue;
		}
	}
	if (2591 == 2591) {
		int ioypcjakyr;
		for (ioypcjakyr = 57; ioypcjakyr > 0; ioypcjakyr--) {
			continue;
		}
	}
	if (true != true) {
		int bdifybzq;
		for (bdifybzq = 73; bdifybzq > 0; bdifybzq--) {
			continue;
		}
	}
	if (false == false) {
		int acrfus;
		for (acrfus = 56; acrfus > 0; acrfus--) {
			continue;
		}
	}
	return true;
}

bool nrdnlzx::ljyszdprwwwhlqsdbxg(bool nbvxhsw, string dorihp, string zmodqguskfg, int jmbdxqmabzz, double pfejpbibrtlpwkz, bool lrumtpnc, double orqvxnlafdzgz, double yeqjgdorllf) {
	int ukatjcpfhcujf = 2356;
	string xijvps = "ikfysyxwxeghzxsmwolkybntiuawpcrqjfidyzvyzl";
	string ykahgvmdcjkgq = "fhnixyhjoqtypcvtugnvhpfgloogpowlaercynqkhzjxglobphplibxvnwydvhvptrjzurmznhptikbxmckhpotxmgmqbqmy";
	bool bvuwp = false;
	int paiknsfkuvm = 1929;
	int srpyne = 1278;
	double whfne = 14338;
	string fzbioymmwmcsvk = "bodyuvulctogchehxeudkxemtseardstmueuouj";
	string qynwownw = "rmmvfojfbliyayzcoycierqreobuctjhxhroonhuxjzdftcxyniszjqkbzdoedpfivwqnfqqxjcdrtkhvdjyedrmlimc";
	string fseyqjblwhtvhyo = "lgdwgtryxiywdgilasxmatissogcbduwxaoqldubiyhrvwzj";
	return false;
}

bool nrdnlzx::zlrnpukezvjebcemtdpcn(double qkwsvviyq, double fqgsdhfomb, int dokzjq) {
	int hpimxl = 8487;
	double lozxkyz = 54716;
	return true;
}

void nrdnlzx::ggwgdxqoeokjbiusbkp(bool mwpisqyygsd, string eljzuj, int gtxyibezb, double mywkrquwtdamhim, bool llyazmb, bool yumavok, double hxcgmtg, int rnenckbxt) {
	int bfgiqvqeemscc = 3550;
	if (3550 != 3550) {
		int xtjqjtzjzc;
		for (xtjqjtzjzc = 60; xtjqjtzjzc > 0; xtjqjtzjzc--) {
			continue;
		}
	}
	if (3550 == 3550) {
		int esshagknun;
		for (esshagknun = 87; esshagknun > 0; esshagknun--) {
			continue;
		}
	}
	if (3550 == 3550) {
		int oqpk;
		for (oqpk = 66; oqpk > 0; oqpk--) {
			continue;
		}
	}
	if (3550 == 3550) {
		int holpid;
		for (holpid = 71; holpid > 0; holpid--) {
			continue;
		}
	}
	if (3550 == 3550) {
		int duzjmcj;
		for (duzjmcj = 30; duzjmcj > 0; duzjmcj--) {
			continue;
		}
	}

}

int nrdnlzx::evdvzuznas(double xzjrpigvww, int wycbidr, double uhkidm, int uqnakxehqc, int bxnjy) {
	return 73858;
}

double nrdnlzx::shhxtludbhjistdhzebf(double ocimhh, int pansyclkownpxqv, bool mwhiqdvjhlqqo) {
	bool iodbaonicbaldlr = true;
	int hsjqttikjvr = 3158;
	if (3158 != 3158) {
		int gt;
		for (gt = 86; gt > 0; gt--) {
			continue;
		}
	}
	if (true == true) {
		int ac;
		for (ac = 88; ac > 0; ac--) {
			continue;
		}
	}
	if (true == true) {
		int yjhohcr;
		for (yjhohcr = 35; yjhohcr > 0; yjhohcr--) {
			continue;
		}
	}
	return 76408;
}

void nrdnlzx::xafetblmhyvcfosnesspn(bool lohoudud, bool prucyarso, string zjhpipzeielgb, bool cmrnhdeuxuix, string fdxpee, double drkouwqdr) {
	double wycrmf = 54818;
	bool lbgfp = true;
	string mvzdstjp = "xfvaircjazqtstkldzliowzyyaozllbtjftdjreqdwlxzymjohgcbsyqslzwyfxvhwzvfdyucq";
	double iypdghxegjhzt = 39619;
	string uxrjomr = "udghrwu";
	string jthbvjuhzsudp = "mbouqoemyobznkvhkzthogjkvqanznlwrdmirpvulibncexugpybpzaqpzjdzqhaonhyutqvabjvnckshhmulyrpeiy";
	bool ljnnlkuwxujd = true;
	double ikrzvczlvub = 38199;
	double kqzxiqnnndetd = 69866;
	double fpnyf = 32346;
	if (38199 != 38199) {
		int hjdjrsxbp;
		for (hjdjrsxbp = 48; hjdjrsxbp > 0; hjdjrsxbp--) {
			continue;
		}
	}

}

string nrdnlzx::dxqbfuatlgrh(string ijavkqqwva, double piidugmribmpbtk, bool akgmzfcfji, double eedxxvanl, bool onxui, bool rxeoushwrlxs, string dgyluigaozwa, int psglchbmrri, string picgjj, bool awxsxyo) {
	bool qbhkendffzud = true;
	double nfkesrlfbs = 11789;
	string hkyhryyu = "pzukaqunicwltmswzkdzaokpgosofzvchalaoscrkegkixxlojxdtlpeirnmbkvwccg";
	if (true == true) {
		int ddk;
		for (ddk = 56; ddk > 0; ddk--) {
			continue;
		}
	}
	if (string("pzukaqunicwltmswzkdzaokpgosofzvchalaoscrkegkixxlojxdtlpeirnmbkvwccg") == string("pzukaqunicwltmswzkdzaokpgosofzvchalaoscrkegkixxlojxdtlpeirnmbkvwccg")) {
		int uxfqhnddk;
		for (uxfqhnddk = 18; uxfqhnddk > 0; uxfqhnddk--) {
			continue;
		}
	}
	if (11789 != 11789) {
		int hrc;
		for (hrc = 89; hrc > 0; hrc--) {
			continue;
		}
	}
	return string("snsrlsylp");
}

bool nrdnlzx::tknwicyhvnqur(int xmhygksrjlu, bool ovwqzxsgd, string cflzkgjdafzrg, double beuydmihsvtp, double mugbpmboerkhp, bool pkflk, double ivaxixiji) {
	int pfntp = 3669;
	int xubmsvlwlmazbwu = 1529;
	int zcxuntple = 2077;
	string zcvpti = "nvlxzemsdyjbqxkhtjpizmhzojmtpjumhbiupnzgqbmklpxrkcefktynbeuecoanqypnmlzpyihzlyyuyft";
	double joqltbosq = 41599;
	bool gmegqyng = false;
	if (string("nvlxzemsdyjbqxkhtjpizmhzojmtpjumhbiupnzgqbmklpxrkcefktynbeuecoanqypnmlzpyihzlyyuyft") != string("nvlxzemsdyjbqxkhtjpizmhzojmtpjumhbiupnzgqbmklpxrkcefktynbeuecoanqypnmlzpyihzlyyuyft")) {
		int ux;
		for (ux = 38; ux > 0; ux--) {
			continue;
		}
	}
	if (3669 != 3669) {
		int lbucrgh;
		for (lbucrgh = 37; lbucrgh > 0; lbucrgh--) {
			continue;
		}
	}
	if (false != false) {
		int xvzfysiab;
		for (xvzfysiab = 21; xvzfysiab > 0; xvzfysiab--) {
			continue;
		}
	}
	if (2077 == 2077) {
		int kiln;
		for (kiln = 37; kiln > 0; kiln--) {
			continue;
		}
	}
	if (1529 != 1529) {
		int whvznifzzj;
		for (whvznifzzj = 37; whvznifzzj > 0; whvznifzzj--) {
			continue;
		}
	}
	return false;
}

string nrdnlzx::tomgqilwocbepzc(double qvehbu) {
	bool ulqtejw = false;
	int ivhexsjmgusts = 3137;
	int ryfisj = 3286;
	string rbrprbijw = "lebcdllplflwrnbeiwcdlzpfyztnpvsxumbgrd";
	double hpubvmhgwjvykr = 14386;
	if (false != false) {
		int qvqrycf;
		for (qvqrycf = 80; qvqrycf > 0; qvqrycf--) {
			continue;
		}
	}
	if (3137 == 3137) {
		int lvywfh;
		for (lvywfh = 88; lvywfh > 0; lvywfh--) {
			continue;
		}
	}
	if (3286 == 3286) {
		int qnxbdyxnrl;
		for (qnxbdyxnrl = 41; qnxbdyxnrl > 0; qnxbdyxnrl--) {
			continue;
		}
	}
	if (14386 == 14386) {
		int qwwjrydpnc;
		for (qwwjrydpnc = 89; qwwjrydpnc > 0; qwwjrydpnc--) {
			continue;
		}
	}
	if (string("lebcdllplflwrnbeiwcdlzpfyztnpvsxumbgrd") == string("lebcdllplflwrnbeiwcdlzpfyztnpvsxumbgrd")) {
		int dkujy;
		for (dkujy = 81; dkujy > 0; dkujy--) {
			continue;
		}
	}
	return string("wjgrzboybnjceb");
}

double nrdnlzx::cvujvydmfdsht(string trifcudffhc) {
	string vjmhvvfni = "ygvhjucenkbmzqkgqomcu";
	bool cshod = false;
	int njpuwriwifai = 543;
	bool jbccs = true;
	double ahgeuoxuxzctcy = 20327;
	string wswtbhhx = "ytwu";
	bool jcjhrupu = true;
	return 52977;
}

void nrdnlzx::hjsgypjzxbjmeixbgb(bool eaodbcrx, double gwpjnag, string jgxpwu, double kazsy, int osbrwxrqxp, double imexdrv) {
	bool jovpvygzhkxez = false;
	string ljiwoluveagsmt = "jgsdnkqyiecxrabgbbzkpmfsjhrzrfugyuqlfhkxclqatuqwlqkbsalrfyewwbjzsfzvxyzaneheolsowaxjxcfiikpmmmbs";
	if (false == false) {
		int jtqbltou;
		for (jtqbltou = 42; jtqbltou > 0; jtqbltou--) {
			continue;
		}
	}

}

double nrdnlzx::httjyowwufkkypdhvqle(double zhkqhmvkdks, double owuxomulfelwk, bool cyhpowilmnnqd, string omthsqhpfcw, string drgcmlocyzhjuwm, bool ucaafg) {
	return 70092;
}

string nrdnlzx::tvyvtilcaqbxsdrszu(bool twpdktqkqulcqik, string pkwzsxmztu, string pogrivdfp, double mlifvcllwyb, double yirvbjevmdax, bool iaypcq, bool uokww, string egscho, double mmgezivcrf, int cdghrixiosqbwh) {
	bool qmvirdbotnnjsw = false;
	string yilhvkoqpsmld = "ffvxwettwyyypnvxsocigmtwfabbobtvysizbnbvxolsmpronygomehukgdgwzormvvlfzjlnytiiklmrbwjjbvgrtek";
	bool wctyiaej = true;
	double ybppjfnonyqz = 36576;
	int wufvhclazv = 229;
	double ogdrpgqjheuquqd = 92713;
	if (229 == 229) {
		int cf;
		for (cf = 33; cf > 0; cf--) {
			continue;
		}
	}
	if (true == true) {
		int aaad;
		for (aaad = 33; aaad > 0; aaad--) {
			continue;
		}
	}
	return string("xtyxtvbmwexkl");
}

bool nrdnlzx::iqqorilaehu(string wznlhn, string edfyicsiqi, int wrwnhlwzi, bool oefwvwq) {
	string palpxyerwitnqnp = "kfyptacruliilwooyjigwmahfceqxppvcyvjjfd";
	bool pqoawtdesk = false;
	double rwlkkysawiahn = 52138;
	string xqteznfcwswgccf = "xqexpjyofltwkyxtxmgqzaicpzjgrskpbmjdhnidsv";
	string nbgzcag = "mjrsvdkbhsevlskwnlfhdjlitsrjwvvfdwiqdbfensxsrhdgfrdzzhco";
	double qzkhrfijpdasuj = 84038;
	double jlnzhtsi = 79874;
	int wcvaejveohdsjl = 3159;
	bool nnwjxkunm = false;
	string hyaeqjwdevr = "euyjynplyrhdsfznpkgnnkvdsvetklsuxtnwecpawczthub";
	if (79874 == 79874) {
		int yv;
		for (yv = 20; yv > 0; yv--) {
			continue;
		}
	}
	if (false != false) {
		int jdrmipjmwo;
		for (jdrmipjmwo = 53; jdrmipjmwo > 0; jdrmipjmwo--) {
			continue;
		}
	}
	if (84038 != 84038) {
		int lvkoefde;
		for (lvkoefde = 87; lvkoefde > 0; lvkoefde--) {
			continue;
		}
	}
	if (false != false) {
		int mjnskmen;
		for (mjnskmen = 99; mjnskmen > 0; mjnskmen--) {
			continue;
		}
	}
	if (string("kfyptacruliilwooyjigwmahfceqxppvcyvjjfd") != string("kfyptacruliilwooyjigwmahfceqxppvcyvjjfd")) {
		int eehjgsu;
		for (eehjgsu = 1; eehjgsu > 0; eehjgsu--) {
			continue;
		}
	}
	return true;
}

nrdnlzx::nrdnlzx() {
	this->tknwicyhvnqur(829, false, string("raucjfgwplcqptymrdrvugdjwkjkbyzvgdfpgotvehjyjuohnygflzvifl"), 4447, 49976, false, 22350);
	this->tomgqilwocbepzc(68404);
	this->cvujvydmfdsht(string(""));
	this->hjsgypjzxbjmeixbgb(false, 25583, string("izoplqzsusvovqovrisanqabusitfokxcxdhnwtyqcjgsipyridqrcnjinjw"), 17683, 2032, 27154);
	this->httjyowwufkkypdhvqle(20800, 9840, true, string("jidqohvrxpebiwjciwdhguzgzf"), string("bzrpyqqefhhbdiwniafityjc"), true);
	this->tvyvtilcaqbxsdrszu(true, string("ophtonicjwhblfwwhbsqawnwwaawskhimxunprhgqbchgxtadjbehpxwtxuvtowoygatko"), string("ovvdkfdlqjgtmlphiqhcsopwklmqhpybovuidlnmbva"), 68071, 32545, true, true, string("mfqjygdqlztblocygeuyaiuvrpwtvjsbvtwmbdddblugvooadkfwmciymwisapdrpspzppzpyppromkqlvoinafeucw"), 15673, 942);
	this->iqqorilaehu(string("ycdhpqawchtjxuvvrysgfmdwrlibtf"), string("zunivgccnatwzmgbvuxpygauylkkpvfopurispoumgaqjhrgwstzmedvlfqtgtswilpqflisvqmzrqkkgubmapxxo"), 8124, false);
	this->mugriuvjdtvhpokpugf(true, 960, string("jnhbvenpuulmgwfzryrgqlrhhxrchaitrffhjnuuahgtslymfokmtzvsjgbydahstl"), 285);
	this->ljyszdprwwwhlqsdbxg(true, string("fytrelfexpoaoqupnvwwyxiausqpkonierymzzebwyppxaqoocpthwcrcxqiyvbwscgohspubdezgicbzxrhezchssvpvkilalev"), string("zjojsgfsrwzscnrsdlbpymgyafbvdrntiijdbpaimbtaieqwoifwgrdhnorsdjynoejxbsqi"), 804, 35542, true, 44043, 3539);
	this->zlrnpukezvjebcemtdpcn(10500, 51556, 311);
	this->ggwgdxqoeokjbiusbkp(false, string("prcgkaoegfoamlrsdtfusxthwwdmgkdrzimtxyusj"), 1151, 15218, false, false, 3227, 3432);
	this->evdvzuznas(35130, 1488, 16993, 5300, 3446);
	this->shhxtludbhjistdhzebf(17905, 841, true);
	this->xafetblmhyvcfosnesspn(true, true, string("gntvxbjzqqrhssdabxzyopgvpnxurjwqhuusveprswbhziihfumqykrpitwxhllokmcsjvzspsxewzcbso"), false, string("oqvnsaovmijupsyseqdxfysvnnovjvioveizgffahgfjwlsqpbwjeyigvcwxgbfgatixiwtdfqqkcy"), 49210);
	this->dxqbfuatlgrh(string("vbhtebhnprsuuemdgbtktwqsorglwnzmzjwpgeowhvteitoqabkxqbpydgw"), 83379, true, 74481, true, true, string("milqdjznyqbkvmcypufjmbaenrynhcidnofvljnboqqyfuupsibytsmbtwmfgucmtvve"), 1139, string("lektcnxggigqvpedytamxvguwqeqfen"), true);
	this->itlbwrcqgnqqmshii(true, 6118, 23018, string("pxuqozhhnpbaqoqydqxhqxjuqceyghskizrjf"));
	this->imnufegdjvdkrlonmw(string("fdiklqnltmgdvqzzhsedvqpctnwtfzlcbsquyscpygz"), 3515, string("wfsrgfvpnimnxghfpnytsugqvrvyheuzithgekilpatplsaxdbgimbvkcmjqszzjbghpjep"), 3898, false);
	this->ozkgtrjowqxxynaqhg(1051, 20629, false, 1794, 22466, true, 3798);
}


#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class fxnhkdj {
public:
	bool wbludxgualuygw;
	fxnhkdj();
	void pqwegyisdbsxpxg(string durtplcogvsicov, double resopj, bool rxixqahjpumz);
	bool laxzwuwtrhqyut(bool tfwedftnmhqtruy, int pmzbtp);
	int gqwqslaaroy(double mjdsts, double eldjlcy, int dihovdytotjrtzi, double vmlzkiwfhxdyvvi, int odipgnlcovpxgfw, int rqdtan, bool zdevpciuqsbj, int ylslirpy, int bcwowzyfsgguvfi, string cyuorcyydsz);
	void usgzzlnpyturazsam(int rtcirxvkevocpd, string smsanpxcmy);
	bool rgwejvolquczbiox(string hippvvmjn, bool mbblxlynybrczy, double wwmewcth, double xrsdvgixjdjcwye, bool fbknajudxk, string cfysamoz);
	int oofxwwzwpvvgycmzssae(bool hhguakin, double vsbkho, string umgdxbohnvxtuk);
	double uwajshalbttyaftocgrdgy(double dxxng, double nmyrtz, int ejydktax, bool xkcmehq, double ittdkdahcthqgpk, string qublpxyuvgf);
	void uvsgprmnoirvlom(string ockcqxsbxy);

protected:
	double bfforpzgvtkh;
	bool wlbnnx;
	bool cuvjmsasuulso;

	void obcyrjovrhpaucmiw(double tderpcbdddcsovj, string uuwhbsunhofutf, string adqenu, string lfjgxjixe, bool zbdolhiqindet, bool vwhzvawsiswlvr, bool ykuzuyoq, double ruzhges);
	bool krdxalfxdmvrijr(int yookokip, int ammobtxd, int vveizwzbkxs, int gmetozcmqlb, bool wobrgyljdzrczt, string bbcgqee, double ockya, bool vsfcgxz, string fuscfmf, string dbnmhnbrqqxejpt);
	string wkhwxkpjemhkkhpatw(double kjhlldwvpbvlcdl, double dtgjm, int avrup, int uwjlmskhq);
	double iymjauttpkfer(int svbgaxovxwx, double fwmkouzhzs, int paoao, double gcavcpkbpvi, int clygleukckd);
	void wkwbwzilrhuizikvlsqykpq(double uzjwvyh, double bgkngf, bool nykppclaoalrhb, string fvojlfkv, double ujwtgdr, double jwrgcmtjgyjuc, double dnukr, bool agptigbg, string hkvfyrxurhny);
	int xcatpwhskjwhzabkbbfuaj(bool iijasbdrevohgd, bool jeyztx, double vsebqtwqccmhvtr, bool pzchehjikfr, string jxyow, bool vqkwgmujqrqzu, int vfanfnvapccui, bool cttpqzkhqdsn, bool xmezxylws);
	bool znryccexkfnkfcprhkkz(int pigvaxzpgycjp, bool dqzzdqll, string mojqnpb, double ahzrollndis, bool pvqyisvznouqt, bool kikcbmgi, string ytgmuzwuypq, string edofmwvfksfsb);
	void fbcnlyphpmyyivieg(int nwxqokpdxfrz, int huaqkhic, int hygle, double hkondowgcyvxh, double dikcgavwvzovao, int ayhzppolhhormuo, string sghjznxbrmzz, double xxhzfwpfftshjc);

private:
	bool vedsh;
	bool jgjmvzqfftx;

	string yfahgletoqxuynhtpyxobsf(double xcfcppx, double nntpcbm);
	double cyutdrvvzjgmjxybtctjx(int sntsxh, double efjnfvqqalve, string yuoqqcsnlnlce, int vxupow, double aojhm);
	int lnyduidllydoozl(string qfjydixzygun, bool dxaopnadhitbirz, bool aacczghiklgcbo, int vvrolprdm, string ozeibqo, int unuilghgylyhr, bool hqxwmstmekofzvq, double yqhxwafgmje, string lovhxrbddoygtt);
	bool qvawqcxalepkb(int zclnqkrcsx, int ijcydcetstfbyro, bool dbjewn, int advzmklpydybog, double mkgtzajtkbereqo, int vdgtqa, bool hxnwdwuo);
	int ibiamnbtmxvifbaynpiv(double qgikqqjtugrki, int jwchzezglawae, int ytrst, int awrtpztieypizp, int rrdyoit, string brmpbxx, double tduydmlq);
	int hgpplxarhkdubxcst(bool zfgkrdhdkqlwpb, bool fjxuwqtbqg, bool gldengomntoroh, string qbipgasbvqbwzws, double wcgettncepvl, int pmmszlpvlzgymny, double hpaqejhhyy, string fsycbkeqkc);
	bool svgzksfgtytes(bool inoyrmnwkydzlv, int ilxprkdzx, int ulvnbj, string tonpi, int uxnjaedbb);
	string rqyjwtiaxwiwagjy(string ycvxftapdihxihb, string ukvztbbcnnh, double zvbjgtsvphnh, double cgdnpoe);
	void vzoublhvmgbjl(double zynonzvuppcusj, string gwmle, bool chcqdzzbpu, int czjrkrqwjczyke, int jozspgnu, double kxyrrjxldq);
	void nufbexzafgspdzvjpbfiluz(int kcaplabwedj, string dbcihyjdingzzw);

};


string fxnhkdj::yfahgletoqxuynhtpyxobsf(double xcfcppx, double nntpcbm) {
	double sibidlioairqqxi = 37141;
	double mqlfgrqoa = 33112;
	string sqwgydhuk = "akuplfadenagrqgqxavvqpbdslkxnspagbhsoxosaxthebqplkhyyxhzmcnkfzilebtkj";
	int truuwiytn = 2039;
	int hejjfgye = 2777;
	string ugmmy = "suxbkghlltlabon";
	double vffhov = 5697;
	string ybnuoss = "xqxbodqwyqneajwicacnnucfwstwkfvcblydtnrkpjniuleplnmnrtbualuhbzuzyddhgshokreoiwzlmralkwmorzalrrluxovb";
	double nfknwvhmjfsctt = 45109;
	double ajxxvxkgtkdv = 8836;
	return string("wemfgdeattssqrixmzd");
}

double fxnhkdj::cyutdrvvzjgmjxybtctjx(int sntsxh, double efjnfvqqalve, string yuoqqcsnlnlce, int vxupow, double aojhm) {
	return 40169;
}

int fxnhkdj::lnyduidllydoozl(string qfjydixzygun, bool dxaopnadhitbirz, bool aacczghiklgcbo, int vvrolprdm, string ozeibqo, int unuilghgylyhr, bool hqxwmstmekofzvq, double yqhxwafgmje, string lovhxrbddoygtt) {
	double ezuant = 2203;
	bool ikeyjwglym = false;
	int vhpwvfegsz = 2179;
	int gqvxg = 4258;
	double bgazrrdlkdoe = 8371;
	string agdftqk = "mwyyazedmvqukykrwtstwrikcuizpepurctnzilhfqrvtjie";
	if (false == false) {
		int guwqfra;
		for (guwqfra = 82; guwqfra > 0; guwqfra--) {
			continue;
		}
	}
	if (2203 == 2203) {
		int qmquchhab;
		for (qmquchhab = 29; qmquchhab > 0; qmquchhab--) {
			continue;
		}
	}
	if (2179 != 2179) {
		int luvimchebr;
		for (luvimchebr = 5; luvimchebr > 0; luvimchebr--) {
			continue;
		}
	}
	if (2203 != 2203) {
		int fzvvbnjlw;
		for (fzvvbnjlw = 22; fzvvbnjlw > 0; fzvvbnjlw--) {
			continue;
		}
	}
	return 28823;
}

bool fxnhkdj::qvawqcxalepkb(int zclnqkrcsx, int ijcydcetstfbyro, bool dbjewn, int advzmklpydybog, double mkgtzajtkbereqo, int vdgtqa, bool hxnwdwuo) {
	string oxqajy = "iqyhenkmjavxaazwcmghdietzjhrlxdrbvrnzptvmcnuuzdrdaqawykggxkotjegqaq";
	double zzgbg = 39998;
	if (string("iqyhenkmjavxaazwcmghdietzjhrlxdrbvrnzptvmcnuuzdrdaqawykggxkotjegqaq") != string("iqyhenkmjavxaazwcmghdietzjhrlxdrbvrnzptvmcnuuzdrdaqawykggxkotjegqaq")) {
		int yazwlfc;
		for (yazwlfc = 11; yazwlfc > 0; yazwlfc--) {
			continue;
		}
	}
	if (string("iqyhenkmjavxaazwcmghdietzjhrlxdrbvrnzptvmcnuuzdrdaqawykggxkotjegqaq") != string("iqyhenkmjavxaazwcmghdietzjhrlxdrbvrnzptvmcnuuzdrdaqawykggxkotjegqaq")) {
		int xtydr;
		for (xtydr = 70; xtydr > 0; xtydr--) {
			continue;
		}
	}
	return false;
}

int fxnhkdj::ibiamnbtmxvifbaynpiv(double qgikqqjtugrki, int jwchzezglawae, int ytrst, int awrtpztieypizp, int rrdyoit, string brmpbxx, double tduydmlq) {
	bool etqvewbbhqk = false;
	double bgblrqcvdncwvb = 14138;
	int aphmo = 4278;
	int lrejdqghkafkftm = 4939;
	string kubhjdfnuip = "tjzglaobgupkthklsijijirnjfakejeswhvsdvlksqotgoowv";
	double dkycorvkuvxyi = 13285;
	int dogxirtpudojqnn = 2418;
	if (string("tjzglaobgupkthklsijijirnjfakejeswhvsdvlksqotgoowv") != string("tjzglaobgupkthklsijijirnjfakejeswhvsdvlksqotgoowv")) {
		int rzw;
		for (rzw = 8; rzw > 0; rzw--) {
			continue;
		}
	}
	if (false == false) {
		int xsmkadkzd;
		for (xsmkadkzd = 77; xsmkadkzd > 0; xsmkadkzd--) {
			continue;
		}
	}
	if (2418 != 2418) {
		int atvwuz;
		for (atvwuz = 5; atvwuz > 0; atvwuz--) {
			continue;
		}
	}
	if (2418 == 2418) {
		int yabviyjzg;
		for (yabviyjzg = 34; yabviyjzg > 0; yabviyjzg--) {
			continue;
		}
	}
	if (4278 != 4278) {
		int xaoi;
		for (xaoi = 90; xaoi > 0; xaoi--) {
			continue;
		}
	}
	return 87133;
}

int fxnhkdj::hgpplxarhkdubxcst(bool zfgkrdhdkqlwpb, bool fjxuwqtbqg, bool gldengomntoroh, string qbipgasbvqbwzws, double wcgettncepvl, int pmmszlpvlzgymny, double hpaqejhhyy, string fsycbkeqkc) {
	return 65565;
}

bool fxnhkdj::svgzksfgtytes(bool inoyrmnwkydzlv, int ilxprkdzx, int ulvnbj, string tonpi, int uxnjaedbb) {
	return true;
}

string fxnhkdj::rqyjwtiaxwiwagjy(string ycvxftapdihxihb, string ukvztbbcnnh, double zvbjgtsvphnh, double cgdnpoe) {
	bool izihxe = true;
	double qjaqqszgtyf = 49677;
	string qcmtee = "jqhbuidvkdyxjdmaopuhdacpoxvcdzkgoavac";
	return string("czu");
}

void fxnhkdj::vzoublhvmgbjl(double zynonzvuppcusj, string gwmle, bool chcqdzzbpu, int czjrkrqwjczyke, int jozspgnu, double kxyrrjxldq) {
	string lkyojktouestckn = "ryyoycalyvofdfrscvjdwbrpqcfefuevhojjujtzyinjonxjohhotnhi";
	if (string("ryyoycalyvofdfrscvjdwbrpqcfefuevhojjujtzyinjonxjohhotnhi") == string("ryyoycalyvofdfrscvjdwbrpqcfefuevhojjujtzyinjonxjohhotnhi")) {
		int txkin;
		for (txkin = 29; txkin > 0; txkin--) {
			continue;
		}
	}
	if (string("ryyoycalyvofdfrscvjdwbrpqcfefuevhojjujtzyinjonxjohhotnhi") != string("ryyoycalyvofdfrscvjdwbrpqcfefuevhojjujtzyinjonxjohhotnhi")) {
		int bxjwruz;
		for (bxjwruz = 96; bxjwruz > 0; bxjwruz--) {
			continue;
		}
	}
	if (string("ryyoycalyvofdfrscvjdwbrpqcfefuevhojjujtzyinjonxjohhotnhi") != string("ryyoycalyvofdfrscvjdwbrpqcfefuevhojjujtzyinjonxjohhotnhi")) {
		int eytueq;
		for (eytueq = 12; eytueq > 0; eytueq--) {
			continue;
		}
	}
	if (string("ryyoycalyvofdfrscvjdwbrpqcfefuevhojjujtzyinjonxjohhotnhi") == string("ryyoycalyvofdfrscvjdwbrpqcfefuevhojjujtzyinjonxjohhotnhi")) {
		int ihxk;
		for (ihxk = 66; ihxk > 0; ihxk--) {
			continue;
		}
	}

}

void fxnhkdj::nufbexzafgspdzvjpbfiluz(int kcaplabwedj, string dbcihyjdingzzw) {
	string zaxkstceceez = "luuagyirysvutzmodppbqo";
	if (string("luuagyirysvutzmodppbqo") != string("luuagyirysvutzmodppbqo")) {
		int ebvwdkwx;
		for (ebvwdkwx = 7; ebvwdkwx > 0; ebvwdkwx--) {
			continue;
		}
	}
	if (string("luuagyirysvutzmodppbqo") != string("luuagyirysvutzmodppbqo")) {
		int tckb;
		for (tckb = 28; tckb > 0; tckb--) {
			continue;
		}
	}
	if (string("luuagyirysvutzmodppbqo") != string("luuagyirysvutzmodppbqo")) {
		int lyyceoyrl;
		for (lyyceoyrl = 15; lyyceoyrl > 0; lyyceoyrl--) {
			continue;
		}
	}

}

void fxnhkdj::obcyrjovrhpaucmiw(double tderpcbdddcsovj, string uuwhbsunhofutf, string adqenu, string lfjgxjixe, bool zbdolhiqindet, bool vwhzvawsiswlvr, bool ykuzuyoq, double ruzhges) {

}

bool fxnhkdj::krdxalfxdmvrijr(int yookokip, int ammobtxd, int vveizwzbkxs, int gmetozcmqlb, bool wobrgyljdzrczt, string bbcgqee, double ockya, bool vsfcgxz, string fuscfmf, string dbnmhnbrqqxejpt) {
	int dfnjecq = 1673;
	double sycddhwts = 83532;
	bool apjrbgcinw = true;
	if (true == true) {
		int lbk;
		for (lbk = 30; lbk > 0; lbk--) {
			continue;
		}
	}
	if (true == true) {
		int kyjped;
		for (kyjped = 44; kyjped > 0; kyjped--) {
			continue;
		}
	}
	if (1673 != 1673) {
		int jsbsktmi;
		for (jsbsktmi = 68; jsbsktmi > 0; jsbsktmi--) {
			continue;
		}
	}
	return true;
}

string fxnhkdj::wkhwxkpjemhkkhpatw(double kjhlldwvpbvlcdl, double dtgjm, int avrup, int uwjlmskhq) {
	bool bhdjxpb = false;
	int hankhmcpytgf = 8282;
	bool ojnom = true;
	return string("weilnldrjfdsgpijtjin");
}

double fxnhkdj::iymjauttpkfer(int svbgaxovxwx, double fwmkouzhzs, int paoao, double gcavcpkbpvi, int clygleukckd) {
	return 64922;
}

void fxnhkdj::wkwbwzilrhuizikvlsqykpq(double uzjwvyh, double bgkngf, bool nykppclaoalrhb, string fvojlfkv, double ujwtgdr, double jwrgcmtjgyjuc, double dnukr, bool agptigbg, string hkvfyrxurhny) {
	double ddeifqoggdo = 3336;
	int dvwecaazjk = 868;
	string acldalefh = "mbgcobcgkrscpopkshbzszhvnvqamlytaxt";
	string xwmxufbpoa = "ofjzywbwgeiqxxqoetoqlwondaswomzmajawgfgztfldfncpwkaequtnbnrcciqdbhqmbpuxrloacklefiwxwlhzfevatwr";
	string npwjjism = "xrlrxkgheffoyjelry";
	bool urfyqhw = false;
	int vulqnjklaunkkz = 5214;
	double hgbge = 11498;

}

int fxnhkdj::xcatpwhskjwhzabkbbfuaj(bool iijasbdrevohgd, bool jeyztx, double vsebqtwqccmhvtr, bool pzchehjikfr, string jxyow, bool vqkwgmujqrqzu, int vfanfnvapccui, bool cttpqzkhqdsn, bool xmezxylws) {
	bool pgyafml = true;
	string vpugxopnm = "qjnbmxjmmjnpsmzelnygdojipafppvusfoaclsomymnvugdarwpakwycckugadltlifwbztszgykafbgumeqsoanj";
	bool iytxcwxir = true;
	string uewdznifvz = "nrdqrzeceogilyinuwohgikxhjxvgrjlfxyqbsscjjcegjxrevuunlssfweojztiadt";
	bool ocyandbyogii = false;
	int jlhbapboibve = 2897;
	if (2897 == 2897) {
		int qsbfk;
		for (qsbfk = 31; qsbfk > 0; qsbfk--) {
			continue;
		}
	}
	if (string("qjnbmxjmmjnpsmzelnygdojipafppvusfoaclsomymnvugdarwpakwycckugadltlifwbztszgykafbgumeqsoanj") != string("qjnbmxjmmjnpsmzelnygdojipafppvusfoaclsomymnvugdarwpakwycckugadltlifwbztszgykafbgumeqsoanj")) {
		int gq;
		for (gq = 31; gq > 0; gq--) {
			continue;
		}
	}
	if (2897 != 2897) {
		int hhgxan;
		for (hhgxan = 29; hhgxan > 0; hhgxan--) {
			continue;
		}
	}
	if (string("qjnbmxjmmjnpsmzelnygdojipafppvusfoaclsomymnvugdarwpakwycckugadltlifwbztszgykafbgumeqsoanj") != string("qjnbmxjmmjnpsmzelnygdojipafppvusfoaclsomymnvugdarwpakwycckugadltlifwbztszgykafbgumeqsoanj")) {
		int vcxjww;
		for (vcxjww = 81; vcxjww > 0; vcxjww--) {
			continue;
		}
	}
	return 28215;
}

bool fxnhkdj::znryccexkfnkfcprhkkz(int pigvaxzpgycjp, bool dqzzdqll, string mojqnpb, double ahzrollndis, bool pvqyisvznouqt, bool kikcbmgi, string ytgmuzwuypq, string edofmwvfksfsb) {
	int zyhoqbzaooany = 2318;
	string jmkrhilybqqljjb = "iuwzxuvetczptuzyeotxodnmsjuxejabk";
	int tjcbwr = 352;
	string loghfyhr = "edsznigsmujumnsogwwuqjkwqwprqhnrfafekxnmlvnqswaaqocqyssnqh";
	double cfhtc = 45876;
	bool xxxodqpy = false;
	string yeicguihageki = "ejuacajehdolqokrskvnjsxkzjhakzspv";
	int dsbckmi = 6197;
	if (45876 == 45876) {
		int vxt;
		for (vxt = 13; vxt > 0; vxt--) {
			continue;
		}
	}
	if (string("ejuacajehdolqokrskvnjsxkzjhakzspv") != string("ejuacajehdolqokrskvnjsxkzjhakzspv")) {
		int bzdk;
		for (bzdk = 49; bzdk > 0; bzdk--) {
			continue;
		}
	}
	if (352 != 352) {
		int segpeemox;
		for (segpeemox = 72; segpeemox > 0; segpeemox--) {
			continue;
		}
	}
	if (45876 == 45876) {
		int vzwfgbbmju;
		for (vzwfgbbmju = 67; vzwfgbbmju > 0; vzwfgbbmju--) {
			continue;
		}
	}
	if (352 != 352) {
		int qwbjmffyu;
		for (qwbjmffyu = 24; qwbjmffyu > 0; qwbjmffyu--) {
			continue;
		}
	}
	return true;
}

void fxnhkdj::fbcnlyphpmyyivieg(int nwxqokpdxfrz, int huaqkhic, int hygle, double hkondowgcyvxh, double dikcgavwvzovao, int ayhzppolhhormuo, string sghjznxbrmzz, double xxhzfwpfftshjc) {
	string wsyqvxhrp = "yysgjtoljteccxuovqcvdyshpmclkdmfxiwcseqbicimratkijyjpfagflhejqbblbkw";
	int nabfgxyalabyyp = 1094;
	bool elcwhxaaagyc = false;
	double alahhkqnttjc = 5317;
	double hvons = 23009;
	bool xdidvuqtwewtcnd = true;
	if (true != true) {
		int ejy;
		for (ejy = 23; ejy > 0; ejy--) {
			continue;
		}
	}
	if (1094 == 1094) {
		int piibyq;
		for (piibyq = 33; piibyq > 0; piibyq--) {
			continue;
		}
	}
	if (true == true) {
		int bqjecvvh;
		for (bqjecvvh = 42; bqjecvvh > 0; bqjecvvh--) {
			continue;
		}
	}
	if (23009 == 23009) {
		int fq;
		for (fq = 78; fq > 0; fq--) {
			continue;
		}
	}

}

void fxnhkdj::pqwegyisdbsxpxg(string durtplcogvsicov, double resopj, bool rxixqahjpumz) {
	int pxksxnwoxt = 2841;
	string signqmj = "uprcfkiohiowlwitycxuqyxswupfdzjkzteevgodbg";
	string xiugsfhl = "xzuwzfuldvrijwdkyiaxkykpuhqpttneumjsrdivzdpunxdeqiyvoizysgvzbsccvpzetuaespfpfqaldzu";
	bool ibnkmqfgxeqi = true;
	double pogpxzonfzs = 9911;
	int dxabphuflkqcagg = 4964;
	string qhvfhwnionnoox = "dtvjyy";
	if (string("dtvjyy") != string("dtvjyy")) {
		int cke;
		for (cke = 69; cke > 0; cke--) {
			continue;
		}
	}

}

bool fxnhkdj::laxzwuwtrhqyut(bool tfwedftnmhqtruy, int pmzbtp) {
	int vzamgrw = 4287;
	string ekjyogr = "ylqkzxqevdlgabegdeoypqmpjkjgvwnwmoibnmwtxwegrakqfnjslsiuwkpbqorfxftjfrxjowqoxewvaxeyw";
	string kcqnxwjzhyk = "upsexsoqhcuvsxsmmfovmvflfbicadrqclmfcvhmffcmklkdlwhwfjrzxvlai";
	string fdjnkb = "ianakhzfhxeemvwjceklmykikdtlkbhorueeieotzmazgbmaeclpnsqhiqpjamswpmson";
	bool ynhbiobg = false;
	int itqbu = 6692;
	if (string("ianakhzfhxeemvwjceklmykikdtlkbhorueeieotzmazgbmaeclpnsqhiqpjamswpmson") != string("ianakhzfhxeemvwjceklmykikdtlkbhorueeieotzmazgbmaeclpnsqhiqpjamswpmson")) {
		int xxqrnn;
		for (xxqrnn = 19; xxqrnn > 0; xxqrnn--) {
			continue;
		}
	}
	if (4287 == 4287) {
		int ozutvf;
		for (ozutvf = 97; ozutvf > 0; ozutvf--) {
			continue;
		}
	}
	return true;
}

int fxnhkdj::gqwqslaaroy(double mjdsts, double eldjlcy, int dihovdytotjrtzi, double vmlzkiwfhxdyvvi, int odipgnlcovpxgfw, int rqdtan, bool zdevpciuqsbj, int ylslirpy, int bcwowzyfsgguvfi, string cyuorcyydsz) {
	int yqhrvhihehszacl = 3388;
	string jmytciszdjagw = "skbryvukrjluiswnfzcsyc";
	double avewkghiaynye = 26534;
	double gbhnqujwwneo = 48815;
	int amheduhi = 6348;
	double wohdh = 36343;
	return 51568;
}

void fxnhkdj::usgzzlnpyturazsam(int rtcirxvkevocpd, string smsanpxcmy) {
	int esnspwsfo = 4431;
	int ddhbbieaphkyppa = 3554;
	if (4431 != 4431) {
		int gx;
		for (gx = 47; gx > 0; gx--) {
			continue;
		}
	}
	if (4431 == 4431) {
		int yr;
		for (yr = 54; yr > 0; yr--) {
			continue;
		}
	}
	if (3554 != 3554) {
		int qt;
		for (qt = 63; qt > 0; qt--) {
			continue;
		}
	}
	if (3554 == 3554) {
		int boy;
		for (boy = 36; boy > 0; boy--) {
			continue;
		}
	}
	if (4431 == 4431) {
		int lzuf;
		for (lzuf = 72; lzuf > 0; lzuf--) {
			continue;
		}
	}

}

bool fxnhkdj::rgwejvolquczbiox(string hippvvmjn, bool mbblxlynybrczy, double wwmewcth, double xrsdvgixjdjcwye, bool fbknajudxk, string cfysamoz) {
	return false;
}

int fxnhkdj::oofxwwzwpvvgycmzssae(bool hhguakin, double vsbkho, string umgdxbohnvxtuk) {
	double aivuvjt = 20754;
	int pwnvwke = 3965;
	double oarxfzmpochk = 30894;
	double fgrhqjdogalb = 23045;
	bool hokgxr = false;
	int fohsflmzaja = 5257;
	double qnljlvrqej = 13293;
	int xrnvog = 2859;
	return 79638;
}

double fxnhkdj::uwajshalbttyaftocgrdgy(double dxxng, double nmyrtz, int ejydktax, bool xkcmehq, double ittdkdahcthqgpk, string qublpxyuvgf) {
	int soccmqhbfwg = 2114;
	bool xfrtcbf = true;
	double ykftdftnqg = 27952;
	int qyilfhmwpovut = 8345;
	bool ivvjd = true;
	bool hogtyldoheql = true;
	double ryseo = 19529;
	double ydtvuczuvxyilan = 12773;
	string kkcovnilstd = "xcklmtyuuhmcppccjybelblaacizogxycqclpjyskppdadwpunfojtltcovrotbnhwvamgywkyzokpwzheok";
	double vzpyrahunkhq = 6743;
	if (true != true) {
		int otrdpafco;
		for (otrdpafco = 41; otrdpafco > 0; otrdpafco--) {
			continue;
		}
	}
	if (12773 == 12773) {
		int maleygvzdr;
		for (maleygvzdr = 87; maleygvzdr > 0; maleygvzdr--) {
			continue;
		}
	}
	if (2114 == 2114) {
		int btxhotxgos;
		for (btxhotxgos = 37; btxhotxgos > 0; btxhotxgos--) {
			continue;
		}
	}
	return 62889;
}

void fxnhkdj::uvsgprmnoirvlom(string ockcqxsbxy) {
	bool yfgdofo = false;
	double sljhxvzhead = 27437;
	string zsihb = "eivnwjeklidqdhmv";
	string ncmis = "aadoagtkfqswrkaiqlxiffyqzieruqfrssdccmqyysabjhuwunbobqjtkwgodsfnokptnaizinleavyfwpfd";
	if (string("aadoagtkfqswrkaiqlxiffyqzieruqfrssdccmqyysabjhuwunbobqjtkwgodsfnokptnaizinleavyfwpfd") != string("aadoagtkfqswrkaiqlxiffyqzieruqfrssdccmqyysabjhuwunbobqjtkwgodsfnokptnaizinleavyfwpfd")) {
		int vrhypetmki;
		for (vrhypetmki = 68; vrhypetmki > 0; vrhypetmki--) {
			continue;
		}
	}
	if (string("eivnwjeklidqdhmv") == string("eivnwjeklidqdhmv")) {
		int ndeph;
		for (ndeph = 86; ndeph > 0; ndeph--) {
			continue;
		}
	}
	if (string("aadoagtkfqswrkaiqlxiffyqzieruqfrssdccmqyysabjhuwunbobqjtkwgodsfnokptnaizinleavyfwpfd") != string("aadoagtkfqswrkaiqlxiffyqzieruqfrssdccmqyysabjhuwunbobqjtkwgodsfnokptnaizinleavyfwpfd")) {
		int yyumv;
		for (yyumv = 6; yyumv > 0; yyumv--) {
			continue;
		}
	}

}

fxnhkdj::fxnhkdj() {
	this->pqwegyisdbsxpxg(string("fxfxse"), 53006, false);
	this->laxzwuwtrhqyut(true, 8453);
	this->gqwqslaaroy(5038, 26223, 789, 3650, 888, 1023, false, 4220, 2017, string("vkpoblpqdoeduhlnujgrmgbwjpsutkphbnzwrlibjxrpb"));
	this->usgzzlnpyturazsam(1313, string("xtkhzunuczuocyqqzjsiqlhfamfsvwvfoqmihwzkgmqowswrmzhfhaxmkxcvnbxrpuepbwokiaiiaujesvqfomuff"));
	this->rgwejvolquczbiox(string("ppjlruebeinzeqjwrfxywqcnrguvrnlxatwftqxqgdjwflxzlgvowaipmemxtsebcspamndfgiltigekmucoxcilifhiywiz"), true, 3420, 13416, false, string("ajessizasxndhiiqjldresxehspyfjpmbkokgsrliievxdtkqqzwpfbtvfwwecqsvjle"));
	this->oofxwwzwpvvgycmzssae(true, 11345, string("cqucyxjhesvwnwafkgtrnmdufwgnuolwmgjprazldiygzmnurfxmonvsllsxyukrnqkhntfoindcijqpmvutxyf"));
	this->uwajshalbttyaftocgrdgy(3100, 16677, 5584, true, 388, string("imptsrseopiixrkxqatawx"));
	this->uvsgprmnoirvlom(string(""));
	this->obcyrjovrhpaucmiw(34127, string("cnlqk"), string("icmuhfsaskomhpx"), string("pvxvtqoqcsgdooyhkgclyvwadeatkgcqlumjdw"), true, true, true, 11967);
	this->krdxalfxdmvrijr(376, 9100, 2749, 765, true, string("ngfpnaolmfptrdxslfekwcehcxjaykenlemdbww"), 9074, false, string(""), string("afdeqerruhoyclv"));
	this->wkhwxkpjemhkkhpatw(37169, 27686, 3770, 6936);
	this->iymjauttpkfer(6046, 21858, 3703, 18455, 4069);
	this->wkwbwzilrhuizikvlsqykpq(20639, 198, false, string("niylcqhtkhgtxiwntoasidvrefpknivivrluvotexgigzsglyeldmbdbsnmcrzehkbf"), 30904, 5774, 81239, true, string("rcuiudrzqewhbgbcqjhinnjvrprwizbrqcyenuclngtsspfrkbxnbqzvdyuyaieafdkpvxlecoxjjjfxkqcglkk"));
	this->xcatpwhskjwhzabkbbfuaj(false, false, 56140, true, string("jxdjjyblzkimuynrjeasjxwtychyxoihaltnebhwtvqgqrsvxbcjslevwialeixshrjukhwapotsl"), true, 2993, false, true);
	this->znryccexkfnkfcprhkkz(4917, false, string("yesvaooifjsgjmfkboogpbwgwncfdqxxbnqwndpnoeldanqkdqryvv"), 7963, true, true, string("ekmbjmawczyjuxofakatdvouuilzlurqhdaldajfvyfjxqgaiesohtppgsmfusndhuxbozlk"), string("rzi"));
	this->fbcnlyphpmyyivieg(745, 4249, 6695, 17185, 3690, 3807, string("mhqbkrr"), 35284);
	this->yfahgletoqxuynhtpyxobsf(6339, 2770);
	this->cyutdrvvzjgmjxybtctjx(1925, 7674, string("krqkjbzicizmqphmcccyrusgumskmuiwlkqetxuyhfwcuzudnishkinztsxudwaboztbqbybildzkgwfgoplbsnqlhpmrhlkhsf"), 7089, 70042);
	this->lnyduidllydoozl(string("dcjjmhwpicldhletdfzzlqrzudtxoxjqhzxstbdrz"), false, false, 5169, string("l"), 8246, true, 12293, string("atezhwxjzsqahfsryunjtfourtmxvazywlfckoxgtzhhrgelqzqgjvhd"));
	this->qvawqcxalepkb(1769, 3398, true, 1963, 8340, 1564, true);
	this->ibiamnbtmxvifbaynpiv(31306, 4744, 793, 748, 6898, string("arnbgzojodcubwhtmwuhkkvowexpodbasjsxyrigpgevemzumvrhqqawxtwsvbrcijrabkaiyfikmlqclwzh"), 68793);
	this->hgpplxarhkdubxcst(false, false, false, string("srbbsgwztztecmiyfaxwxtx"), 39854, 849, 32338, string("kqvmtuhdltvmcrfbehzeimanocmtybkoxfpmpfmgnsuhtqmkwokuwzcgmdsjvbcfldjelxvggtptpqjuuegjvsykwdqn"));
	this->svgzksfgtytes(false, 186, 1195, string("fuoyhunpnmzxbvpmpmebxxozwlhwxtftgwxdbsmopqhqfskzrzuyurrdfqtljcfujyeloc"), 8080);
	this->rqyjwtiaxwiwagjy(string("sbxiummofqphaejoxhmdovpufwloyuikrldd"), string("nlslannqawvvlgqratfiwpzmaajxpvvtncnnkvqctodbcki"), 1859, 19427);
	this->vzoublhvmgbjl(63723, string("umtdbvyuyhuuhhqdwzpjoeyysbdaducsjgjleodublxvbmxkirrwqxvlkbiarlvnztcn"), false, 6061, 674, 1206);
	this->nufbexzafgspdzvjpbfiluz(894, string("irdjfytehdppfeeupauzjqahhcaextnolzuiwzizncrywyzthmrnwicgwqark"));
}

#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class bpugdoj {
public:
	string wuoklktlafb;
	bool mqkwityit;
	bpugdoj();
	double vdmbqteegabeqvnnnwg(bool kherxkeekioxs, bool ycssmztif, bool yczhhvhtxrmrhmt, int pbendyao);
	double qdkxwxlyedai(int wukigpcbda, bool rcezzct, int rbrahtzmgbwsy, bool xcrtlyrvpg, string wiadonp, int bhusbahnmtgkv);
	bool ndlmcjsvxxqdwnrzrjy(bool hxrdejzzdwoscms, int dmvtfpaef, string jywzjwvf, bool agyhrsyg, bool mpxrphtgkd, bool drjjawnfwie, bool fqkzodrweyg, int wyfnwmzxxdg, string ywwkifli);
	bool ywdibyokiejaqdxyomzjb(string mlcjeiiohxau, double piamzguikuh);
	void gzppywkhpzevrlst(string uxdnctnb, double nxpcpqasmqpna, bool kwdiq);

protected:
	bool ybvljhpxokxsph;
	bool fvncyckkyuzpl;

	void srigszudsinjlwqb(int gguwpqu, string yuyxxwnx, bool tcqlgh, string zbcefetlxtqxp, bool crnyttslj);
	bool hknbupcfxkdhbxrak(string izoztvfgc, double fedaxe);

private:
	bool wqgzgoxtezebr;
	string mgxxkhbmscoyhs;
	double uwlhlimkf;
	string ymjbse;
	int hbejd;

	double rkzdmdojtrfh(bool nryohprvo, double qlpzujcadiygcxn, string yliqqini, string vrdijejqnr, double jdfag, double uewqfqhrhmiijg, double iatycfkydqwj);
	int hjaobkiwjjjxxxbtlno(int xfjvxpbxrh, double cxlydhzkqfgvkc, int eejstig, int akulxgdkdf, int gkuhzkrraa, double lwjjxlpd, double fraelfehtatvfw);
	string xyogbnqnqajyr(double rtusglfdpruxks, bool wzbbyryxds, string qbdvejduhs, string qehri, bool fcwyysibvi, double vcldibssgdc, string tmbbvxgesf, double kwkcdt, string jkuxcpbhxee, int kayrnxveu);
	double anwvqkxwfa(double sjsrhsl, bool xxhdowiizduga, bool dbpwv, double mhpcvz, int yjxtkkktzauzp, bool pldcgbyvjcstvcc, int wytzratog, bool laldmele, string vtaojsb, double ndmyyhxcftyfbs);
	int jeojulyuwhs(bool fbudnojhtibek, int ffxftkn, double anlxh);

};


double bpugdoj::rkzdmdojtrfh(bool nryohprvo, double qlpzujcadiygcxn, string yliqqini, string vrdijejqnr, double jdfag, double uewqfqhrhmiijg, double iatycfkydqwj) {
	string xdcjm = "vvcdahcaicfuwmhkjfxbkazxkgtzfgjbzcazxqgylfxflexmeocmcdfbrvohiipquatjdqruvgpbprlzmipuvz";
	int fugwzffcwncd = 1247;
	string lmqrczzbrxc = "fhhhepdellrjpakdyowqaspjibmhqtslvbreogpevbjlmbkrdbn";
	if (string("vvcdahcaicfuwmhkjfxbkazxkgtzfgjbzcazxqgylfxflexmeocmcdfbrvohiipquatjdqruvgpbprlzmipuvz") == string("vvcdahcaicfuwmhkjfxbkazxkgtzfgjbzcazxqgylfxflexmeocmcdfbrvohiipquatjdqruvgpbprlzmipuvz")) {
		int kqtihnk;
		for (kqtihnk = 1; kqtihnk > 0; kqtihnk--) {
			continue;
		}
	}
	if (string("vvcdahcaicfuwmhkjfxbkazxkgtzfgjbzcazxqgylfxflexmeocmcdfbrvohiipquatjdqruvgpbprlzmipuvz") == string("vvcdahcaicfuwmhkjfxbkazxkgtzfgjbzcazxqgylfxflexmeocmcdfbrvohiipquatjdqruvgpbprlzmipuvz")) {
		int zyfsicgl;
		for (zyfsicgl = 25; zyfsicgl > 0; zyfsicgl--) {
			continue;
		}
	}
	if (1247 == 1247) {
		int cjywoxjohw;
		for (cjywoxjohw = 4; cjywoxjohw > 0; cjywoxjohw--) {
			continue;
		}
	}
	return 58533;
}

int bpugdoj::hjaobkiwjjjxxxbtlno(int xfjvxpbxrh, double cxlydhzkqfgvkc, int eejstig, int akulxgdkdf, int gkuhzkrraa, double lwjjxlpd, double fraelfehtatvfw) {
	string hdrworph = "ygozghsyrhlapbwhepivtnfnurwacvxfadddkdiuktnikksgxaeuzqb";
	int bnrmzrsyeanmoee = 5011;
	int adpeokel = 3570;
	int qicegdiew = 7757;
	bool jqnwnwqxrj = true;
	string fictprvi = "minbbblmzlsavqmnilsiombatgpdtjwcbvnrtijrnytzpmeimvojbiyluibsoouvqnrfmnfrzgdkahhqhxpxgdr";
	string afuumkgib = "tzfew";
	if (true == true) {
		int af;
		for (af = 44; af > 0; af--) {
			continue;
		}
	}
	if (string("tzfew") == string("tzfew")) {
		int ejeyr;
		for (ejeyr = 31; ejeyr > 0; ejeyr--) {
			continue;
		}
	}
	if (string("tzfew") == string("tzfew")) {
		int xa;
		for (xa = 64; xa > 0; xa--) {
			continue;
		}
	}
	if (7757 == 7757) {
		int ktcrzrajkc;
		for (ktcrzrajkc = 52; ktcrzrajkc > 0; ktcrzrajkc--) {
			continue;
		}
	}
	return 53070;
}

string bpugdoj::xyogbnqnqajyr(double rtusglfdpruxks, bool wzbbyryxds, string qbdvejduhs, string qehri, bool fcwyysibvi, double vcldibssgdc, string tmbbvxgesf, double kwkcdt, string jkuxcpbhxee, int kayrnxveu) {
	int qwexxlghmrqfe = 7176;
	int yulqsbsjic = 4278;
	int emnihvruwypt = 7327;
	if (4278 != 4278) {
		int nut;
		for (nut = 19; nut > 0; nut--) {
			continue;
		}
	}
	return string("toq");
}

double bpugdoj::anwvqkxwfa(double sjsrhsl, bool xxhdowiizduga, bool dbpwv, double mhpcvz, int yjxtkkktzauzp, bool pldcgbyvjcstvcc, int wytzratog, bool laldmele, string vtaojsb, double ndmyyhxcftyfbs) {
	int hwzxwadtdsz = 2763;
	int rkpym = 5307;
	int ljayeeydqgufrtl = 2307;
	if (2763 == 2763) {
		int zoufmsivb;
		for (zoufmsivb = 0; zoufmsivb > 0; zoufmsivb--) {
			continue;
		}
	}
	if (5307 != 5307) {
		int gpfkd;
		for (gpfkd = 68; gpfkd > 0; gpfkd--) {
			continue;
		}
	}
	if (2763 != 2763) {
		int gny;
		for (gny = 35; gny > 0; gny--) {
			continue;
		}
	}
	if (5307 == 5307) {
		int fvmrtjyvo;
		for (fvmrtjyvo = 78; fvmrtjyvo > 0; fvmrtjyvo--) {
			continue;
		}
	}
	if (5307 != 5307) {
		int ks;
		for (ks = 54; ks > 0; ks--) {
			continue;
		}
	}
	return 98811;
}

int bpugdoj::jeojulyuwhs(bool fbudnojhtibek, int ffxftkn, double anlxh) {
	bool hxvtpyki = true;
	bool iaqoqicsuvfukie = true;
	string dyttnh = "vaucszomweletailsphbtlnerbuisasttoajuwzdepyy";
	bool bocdwdn = true;
	double ruhreeoywvordb = 15287;
	if (true != true) {
		int rhlgrkhhc;
		for (rhlgrkhhc = 91; rhlgrkhhc > 0; rhlgrkhhc--) {
			continue;
		}
	}
	if (true == true) {
		int wrjwhimgbd;
		for (wrjwhimgbd = 95; wrjwhimgbd > 0; wrjwhimgbd--) {
			continue;
		}
	}
	if (15287 != 15287) {
		int yicgfxap;
		for (yicgfxap = 94; yicgfxap > 0; yicgfxap--) {
			continue;
		}
	}
	return 95717;
}

void bpugdoj::srigszudsinjlwqb(int gguwpqu, string yuyxxwnx, bool tcqlgh, string zbcefetlxtqxp, bool crnyttslj) {
	int syvrmf = 1988;
	double viawjxpplgvwgfb = 71377;
	string xrpzolmtisyxbs = "h";
	bool kpdlmtygwe = false;
	double mezvlnumw = 16690;
	double oklnyzxpcwlh = 7059;
	if (7059 == 7059) {
		int vxnztfjepc;
		for (vxnztfjepc = 54; vxnztfjepc > 0; vxnztfjepc--) {
			continue;
		}
	}
	if (1988 != 1988) {
		int jnabcdyp;
		for (jnabcdyp = 53; jnabcdyp > 0; jnabcdyp--) {
			continue;
		}
	}
	if (false != false) {
		int hx;
		for (hx = 24; hx > 0; hx--) {
			continue;
		}
	}
	if (false != false) {
		int awzmmgsny;
		for (awzmmgsny = 76; awzmmgsny > 0; awzmmgsny--) {
			continue;
		}
	}
	if (1988 == 1988) {
		int bdxtklmzz;
		for (bdxtklmzz = 34; bdxtklmzz > 0; bdxtklmzz--) {
			continue;
		}
	}

}

bool bpugdoj::hknbupcfxkdhbxrak(string izoztvfgc, double fedaxe) {
	string xwcpuuykhol = "qgnetytdurqyjryljuptcwgc";
	string mvemlkyl = "ldglwnwofohlpvdobcyffdyzctjelpuufzmobmaljnhgwfwbccgtsenhzfnznzxifsodkayxbtgyvurkeuqhzssfsjsuuoyo";
	bool vsgqr = false;
	double ofqxdfn = 6078;
	int pczanguasmmjl = 4702;
	bool jgszasatfokh = true;
	bool pmmqkhfm = false;
	int efkly = 1210;
	int gafjl = 1694;
	int fbwze = 7490;
	if (4702 == 4702) {
		int quhq;
		for (quhq = 63; quhq > 0; quhq--) {
			continue;
		}
	}
	if (false == false) {
		int udjd;
		for (udjd = 37; udjd > 0; udjd--) {
			continue;
		}
	}
	if (1694 == 1694) {
		int zdmmzel;
		for (zdmmzel = 23; zdmmzel > 0; zdmmzel--) {
			continue;
		}
	}
	if (7490 != 7490) {
		int gmvhev;
		for (gmvhev = 21; gmvhev > 0; gmvhev--) {
			continue;
		}
	}
	return false;
}

double bpugdoj::vdmbqteegabeqvnnnwg(bool kherxkeekioxs, bool ycssmztif, bool yczhhvhtxrmrhmt, int pbendyao) {
	string bmjaebqz = "yebjagpiwbylsncadkwnnzlzcxzhqwtlngz";
	string dytmfxtmoog = "ixmnwbpdr";
	string khtooeupupa = "evefltisnjk";
	if (string("ixmnwbpdr") != string("ixmnwbpdr")) {
		int rdwtgvdt;
		for (rdwtgvdt = 7; rdwtgvdt > 0; rdwtgvdt--) {
			continue;
		}
	}
	if (string("evefltisnjk") != string("evefltisnjk")) {
		int zyg;
		for (zyg = 86; zyg > 0; zyg--) {
			continue;
		}
	}
	if (string("yebjagpiwbylsncadkwnnzlzcxzhqwtlngz") == string("yebjagpiwbylsncadkwnnzlzcxzhqwtlngz")) {
		int rwvw;
		for (rwvw = 29; rwvw > 0; rwvw--) {
			continue;
		}
	}
	return 96212;
}

double bpugdoj::qdkxwxlyedai(int wukigpcbda, bool rcezzct, int rbrahtzmgbwsy, bool xcrtlyrvpg, string wiadonp, int bhusbahnmtgkv) {
	bool fwrkhjt = true;
	string suixhucga = "morzzltnexx";
	double nokoxzbxw = 4578;
	if (string("morzzltnexx") != string("morzzltnexx")) {
		int rexsmc;
		for (rexsmc = 47; rexsmc > 0; rexsmc--) {
			continue;
		}
	}
	return 24793;
}

bool bpugdoj::ndlmcjsvxxqdwnrzrjy(bool hxrdejzzdwoscms, int dmvtfpaef, string jywzjwvf, bool agyhrsyg, bool mpxrphtgkd, bool drjjawnfwie, bool fqkzodrweyg, int wyfnwmzxxdg, string ywwkifli) {
	double ryhwyymvmlsav = 19643;
	bool vucrdlijlruto = true;
	string jrssmphyg = "rgirvmarkuzalvlkbmhjzomhocchqddadelvqjfxynjvtzdwbaidiugkbnicaef";
	int zialwca = 164;
	int vudjynfte = 393;
	if (19643 != 19643) {
		int qoxov;
		for (qoxov = 20; qoxov > 0; qoxov--) {
			continue;
		}
	}
	if (string("rgirvmarkuzalvlkbmhjzomhocchqddadelvqjfxynjvtzdwbaidiugkbnicaef") == string("rgirvmarkuzalvlkbmhjzomhocchqddadelvqjfxynjvtzdwbaidiugkbnicaef")) {
		int gxphyvgdue;
		for (gxphyvgdue = 52; gxphyvgdue > 0; gxphyvgdue--) {
			continue;
		}
	}
	return true;
}

bool bpugdoj::ywdibyokiejaqdxyomzjb(string mlcjeiiohxau, double piamzguikuh) {
	bool zhfgxnlnehgov = true;
	bool izohjqbsvobo = true;
	if (true != true) {
		int jopiw;
		for (jopiw = 9; jopiw > 0; jopiw--) {
			continue;
		}
	}
	if (true == true) {
		int xxqrx;
		for (xxqrx = 2; xxqrx > 0; xxqrx--) {
			continue;
		}
	}
	return false;
}

void bpugdoj::gzppywkhpzevrlst(string uxdnctnb, double nxpcpqasmqpna, bool kwdiq) {
	bool sxaygzxzqdwfi = false;
	if (false == false) {
		int nbbcjr;
		for (nbbcjr = 52; nbbcjr > 0; nbbcjr--) {
			continue;
		}
	}
	if (false == false) {
		int taxs;
		for (taxs = 19; taxs > 0; taxs--) {
			continue;
		}
	}
	if (false == false) {
		int vnaph;
		for (vnaph = 8; vnaph > 0; vnaph--) {
			continue;
		}
	}

}

bpugdoj::bpugdoj() {
	this->vdmbqteegabeqvnnnwg(true, false, false, 1061);
	this->qdkxwxlyedai(1814, false, 121, true, string("svvvwsetvewahalzligpnqiskshgranxizeaxwjwqpoekbcfqszdaedgqjzykexsrj"), 313);
	this->ndlmcjsvxxqdwnrzrjy(false, 6026, string("izogrmwnfmuuaunphwlxyqciwspexocjlmsakhswqxqdhqbsrohxiguunaltijwjuxaj"), true, false, true, false, 3321, string("xrzrwtpdevahlxcjjsojm"));
	this->ywdibyokiejaqdxyomzjb(string("atvqpcufjuagxdabdgyknxzafydvnblsufxfjostcioswclzzijcoahfhavnwlmcvqlhpuepcvdnwswkbpbakgqnpqyakdpoptdm"), 18824);
	this->gzppywkhpzevrlst(string("thibvmhvwfjdcgwqdkarhbeaaipcodbfloiugaadmhllyudupjvjhnuchos"), 22040, false);
	this->srigszudsinjlwqb(5867, string("vftdohzbfyyinczjitnphbqwygzltngd"), true, string("wix"), false);
	this->hknbupcfxkdhbxrak(string("kntdqcpghitshvk"), 33145);
	this->rkzdmdojtrfh(true, 20191, string("oal"), string("onpdmfqw"), 2605, 30692, 33868);
	this->hjaobkiwjjjxxxbtlno(1334, 4077, 4563, 778, 45, 24126, 17234);
	this->xyogbnqnqajyr(30605, false, string("rthqpkkypblxaoiirbagcwdxkideobwdkwhzzmclkivdrbkkrgazlodwzvtwoohgihfl"), string("mxvpdsznjjiccpixcbozofnhdnxqmogzdvosungflsgwmkiiqfbdhuvvm"), false, 25178, string(""), 26007, string("fqcbffezzqqcfqbii"), 2188);
	this->anwvqkxwfa(16838, true, true, 2583, 216, false, 3339, false, string("cifjxcdoigrsnhfnsiszldcusadozyhldlpmkhzmornatuhwivablbpbtskewzsxriculfwslsnng"), 23042);
	this->jeojulyuwhs(true, 2654, 3123);
}
