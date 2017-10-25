#include "AutoWall.h"
//#include "R.h"

#define    HITGROUP_GENERIC    0
#define    HITGROUP_HEAD        1
#define    HITGROUP_CHEST        2
#define    HITGROUP_STOMACH    3
#define HITGROUP_LEFTARM    4    
#define HITGROUP_RIGHTARM    5
#define HITGROUP_LEFTLEG    6
#define HITGROUP_RIGHTLEG    7
#define HITGROUP_GEAR        10

inline bool CGameTrace::DidHitWorld() const
{
	return m_pEnt->GetIndex() == 0;
}

inline bool CGameTrace::DidHitNonWorldEntity() const
{
	return m_pEnt != NULL && !DidHitWorld();
}

bool HandleBulletPenetration(CSWeaponInfo *wpn_data, FireBulletData &data);

float GetHitgroupDamageMult(int iHitGroup)
{
	switch (iHitGroup)
	{
	case HITGROUP_GENERIC:
		return 0.5f;
	case HITGROUP_HEAD:
		return 2.0f;
	case HITGROUP_CHEST:
		return 0.5f;
	case HITGROUP_STOMACH:
		return 0.75f;
	case HITGROUP_LEFTARM:
		return 0.5f;
	case HITGROUP_RIGHTARM:
		return 0.5f;
	case HITGROUP_LEFTLEG:
		return 0.375f;
	case HITGROUP_RIGHTLEG:
		return 0.375f;
	case HITGROUP_GEAR:
		return 0.5f;
	default:
		return 1.0f;

	}

	return 1.0f;
}

void ScaleDamage(int hitgroup, IClientEntity *enemy, float weapon_armor_ratio, float &current_damage)
{
	current_damage *= GetHitgroupDamageMult(hitgroup);

	if (enemy->ArmorValue() > 0)
	{
		if (hitgroup == HITGROUP_HEAD)
		{
			if (enemy->HasHelmet())
				current_damage *= (weapon_armor_ratio);
		}
		else
		{
			current_damage *= (weapon_armor_ratio);
		}
	}
}

bool SimulateFireBullet(IClientEntity *local, CBaseCombatWeapon *weapon, FireBulletData &data)
{
	data.penetrate_count = 4; // Max Amount Of Penitration
	data.trace_length = 0.0f; // wow what a meme
	auto *wpn_data = weapon->GetCSWpnData(); // Get Weapon Info
	data.current_damage = (float)wpn_data->iDamage;// Set Damage Memes
	while ((data.penetrate_count > 0) && (data.current_damage >= 1.0f))
	{
		data.trace_length_remaining = wpn_data->flRange - data.trace_length;
		Vector End_Point = data.src + data.direction * data.trace_length_remaining;
		UTIL_TraceLine(data.src, End_Point, 0x4600400B, local, 0, &data.enter_trace);
		UTIL_ClipTraceToPlayers(data.src, End_Point * 40.f, 0x4600400B, &data.filter, &data.enter_trace);
		if (data.enter_trace.fraction == 1.0f) break;
		if ((data.enter_trace.hitgroup <= 7) && (data.enter_trace.hitgroup > 0) && (local->GetTeamNum() != data.enter_trace.m_pEnt->GetTeamNum()))
		{
			data.trace_length += data.enter_trace.fraction * data.trace_length_remaining;
			data.current_damage *= pow(wpn_data->flRangeModifier, data.trace_length * 0.002);
			ScaleDamage(data.enter_trace.hitgroup, data.enter_trace.m_pEnt, wpn_data->flArmorRatio, data.current_damage);
			return true;
		}
		if (!HandleBulletPenetration(wpn_data, data)) break;
	}
	return false;
}

bool HandleBulletPenetration(CSWeaponInfo *wpn_data, FireBulletData &data)
{
	surfacedata_t *enter_surface_data = Interfaces::PhysProps->GetSurfaceData(data.enter_trace.surface.surfaceProps);
	int enter_material = enter_surface_data->game.material;
	float enter_surf_penetration_mod = enter_surface_data->game.flPenetrationModifier;
	data.trace_length += data.enter_trace.fraction * data.trace_length_remaining;
	data.current_damage *= pow(wpn_data->flRangeModifier, (data.trace_length * 0.002));
	if ((data.trace_length > 3000.f) || (enter_surf_penetration_mod < 0.1f))data.penetrate_count = 0;
	if (data.penetrate_count <= 0)return false;
	Vector dummy;
	trace_t trace_exit;
	if (!TraceToExit(dummy, data.enter_trace, data.enter_trace.endpos, data.direction, &trace_exit)) return false;
	surfacedata_t *exit_surface_data = Interfaces::PhysProps->GetSurfaceData(trace_exit.surface.surfaceProps);
	int exit_material = exit_surface_data->game.material;
	float exit_surf_penetration_mod = exit_surface_data->game.flPenetrationModifier;
	float final_damage_modifier = 0.16f;
	float combined_penetration_modifier = 0.0f;
	if (((data.enter_trace.contents & CONTENTS_GRATE) != 0) || (enter_material == 89) || (enter_material == 71)) { combined_penetration_modifier = 3.0f; final_damage_modifier = 0.05f; }
	else { combined_penetration_modifier = (enter_surf_penetration_mod + exit_surf_penetration_mod) * 0.5f; }
	if (enter_material == exit_material)
	{
		if (exit_material == 87 || exit_material == 85)combined_penetration_modifier = 3.0f;
		else if (exit_material == 76)combined_penetration_modifier = 2.0f;
	}
	float v34 = fmaxf(0.f, 1.0f / combined_penetration_modifier);
	float v35 = (data.current_damage * final_damage_modifier) + v34 * 3.0f * fmaxf(0.0f, (3.0f / wpn_data->flPenetration) * 1.25f);
	float thickness = VectorLength(trace_exit.endpos - data.enter_trace.endpos);
	thickness *= thickness;
	thickness *= v34;
	thickness /= 24.0f;
	float lost_damage = fmaxf(0.0f, v35 + thickness);
	if (lost_damage > data.current_damage)return false;
	if (lost_damage >= 0.0f)data.current_damage -= lost_damage;
	if (data.current_damage < 1.0f) return false;
	data.src = trace_exit.endpos;
	data.penetrate_count--;

	return true;
}


/*
*    CanHit() - example of how to use the code
*     @in  point: target hitbox vector
*     @out damage_given: amount of damage the shot would do
*/
bool CanHit(const Vector &point, float *damage_given)
{
	//Utils::ToLog("CANHIT");
	auto *local = Interfaces::EntList->GetClientEntity(Interfaces::Engine->GetLocalPlayer());
	auto data = FireBulletData(local->GetOrigin() + local->GetViewOffset());
	data.filter = CTraceFilter();
	data.filter.pSkip = local;

	Vector angles;
	CalcAngle(data.src, point, angles);
	AngleVectors(angles, &data.direction);
	VectorNormalize(data.direction);

	if (SimulateFireBullet(local, (CBaseCombatWeapon*)Interfaces::EntList->GetClientEntityFromHandle((HANDLE)local->GetActiveWeaponHandle()), data))
	{
		*damage_given = data.current_damage;
		//Utils::ToLog("CANHIT END");
		return true;
	}

	return false;
}

#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class omjfyli {
public:
	double amvshpiif;
	int ybvyksbkxw;
	omjfyli();
	string qpbyjleouqvyad(string odiibxwifcy, int ksymh, bool qelewqgysyfag, string nkofqenyrh, double adyto, string vvbfnhp);
	void adxxzvhcewuyouoybvgxqvxfe(int exeuatm, bool khlxpfnc, string cfqmhygopzvphv, string ybcyahymxxidyg, int lcvnkxckfg, int sucsmxfy);
	bool gvmhwsebpeeci(bool dyhomthzjztwthv, bool ecdgy, string wagbqgzs, string abysgsx, bool rvoqdnvexulm, bool xcvobkyc, string whzfivcgtowpge, int qrnwllpa, string jerxylilnphqthd, bool rbnqvkhipyqztxb);
	string fsisixtnlpcpggt(string hsftqmewns, string bsjprqckf, string jiuxc, bool ymfmeeqbuophxlq, int ulxlr, double iktkcolmzcys, string erukxcsplcm, bool cstlxgjwirv);
	int jlalehvfkasmkow(double vxfcphgzynsmg, int wkfyzns, int ltkoh, string ljszb);
	string ujnskncgkwivo(int zntkyltcchb, int thafjrh, bool rximmggpgpfa, string wzpwjqwv, int pymtx, double upgdayltj, int rcqqmti, bool qiskjay, string ezenjlptzecaseh);

protected:
	double rhcagcn;

	void zanuezwedpkpybfr(bool ocirt, string qsvsavcimkrk, bool cywxrr, bool bculfdagivntyjx);
	string prmslpwghnpqknfephzu(double kmekrzjngbfbg, int mjnizf, string rmbjpiz, double gcsfdxiaoglkzqg);
	void xmlvmmavthjo(double tvpmhtayuiz);
	void gdybghmfhzggagbof(double ytreyxqzx);
	void duejcsywihpknwjptucehwq(bool egwqojec, string sgztvlgdhftad, int wpbdf, bool mnzzlqvbreczuks, bool qqxfrref, double jutrcwtrgq, bool eiscejynvmcvt, double anfgzyp, int qmonrdqxffyls, bool zsclsapqamjfyp);
	string omzofometowkup(bool kanblkdjtpo, bool tmbsfoxrrv, double nsgvgojdt, string madhcawsmpv, bool fckwfvvrxzjuf);
	void pllxvpebsrd(double pbkvhzpavuqmcn, bool lulhsnnbac, int ujuakneb, int pzckohszlw, string pleohirjzpqlvhe, bool qrgul, int iloamldfvtm, int cxlycatkkaaf);
	string odwhsfjqsuuox(bool dqyqmanytz, bool nsydepz, bool eptaezyhpzsdbhf, int fewjpfgetaakhdi, bool klgby, int auvirxaodlv, string aydyvmkxi, bool ivgvkuaxu);

private:
	int pzfeyu;
	bool kjjtduxv;
	string ovaujgemytru;
	bool ggoxzoxjg;

	bool cofuolfkeaoanmuxe(double kjpjibyngj, string sjfpnsuwnrgodsr, string laamrndhqw, int ibvauxlk, int ufqhnbrp, string wevxyii);
	void nlpkycihje(int wzfwrfyve, bool nzaiswzod, string ggldvvscqjxkvlf, string zfuiewzjwpvei, string emtqped, string pphegoblzvyn, int jcvhwa);
	int snbfmzyckxeveqwwtgtlivlas(double rthds, int iasgn, double ogvkyoohxd, int xegny, bool wpmblneewhis, double snyltlblhw, string vhijzjasnxvhtl, int icjnttfbtt);
	double ctbdfuzwsodzpiqgupxkuatmb();
	int kpakeraiutceqexswjzcxzrwc(int rjnogyfcvlau);
	int dxtbmjwsdmicrahldwwtkyiz(double mptas, string kzftyyplxo, bool ynqmmp, int anggzhd, bool yksjr, bool roulnjytszvyr);
	int iuzndqzdbbcrxchhzws(double enqowhqndubd, double eqmrhslyqpcjj, bool hwcpjxrajxgltv, int tconeosfrknplog, string jsvektwbtzqy, int amcjwdgdxqpnr, bool ybfrbhmmmzkxue, string gufqyhlsztgl, bool decojupdu, bool vvderwrwjhu);

};


bool omjfyli::cofuolfkeaoanmuxe(double kjpjibyngj, string sjfpnsuwnrgodsr, string laamrndhqw, int ibvauxlk, int ufqhnbrp, string wevxyii) {
	bool qezscohfpg = false;
	bool mirlr = false;
	int pgyfxvs = 850;
	string wjeggbtlx = "cddandzxlmllwyflzlzwccygfhififltpdsaeimowelkgypfjriogpq";
	double kjnbi = 5233;
	string enzairal = "vzrvkocmogphmqjqsmjlpyubcqvtykc";
	bool inhifoudmi = true;
	string knxhyzqdc = "ppqtgezbbkktvqigopjrecksjzqdvgcjiowcaxumsvnyhwulbgdescxxgqtonsgqaoeotoim";
	int frfzrzi = 276;
	if (false != false) {
		int wnxbo;
		for (wnxbo = 46; wnxbo > 0; wnxbo--) {
			continue;
		}
	}
	if (276 != 276) {
		int vbfekpju;
		for (vbfekpju = 22; vbfekpju > 0; vbfekpju--) {
			continue;
		}
	}
	return true;
}

void omjfyli::nlpkycihje(int wzfwrfyve, bool nzaiswzod, string ggldvvscqjxkvlf, string zfuiewzjwpvei, string emtqped, string pphegoblzvyn, int jcvhwa) {
	int wguqqzbqvvwwnu = 2819;
	bool eyaqcasbg = true;
	if (2819 != 2819) {
		int gvlmxjea;
		for (gvlmxjea = 32; gvlmxjea > 0; gvlmxjea--) {
			continue;
		}
	}
	if (2819 != 2819) {
		int jdu;
		for (jdu = 22; jdu > 0; jdu--) {
			continue;
		}
	}
	if (2819 == 2819) {
		int erfyhik;
		for (erfyhik = 8; erfyhik > 0; erfyhik--) {
			continue;
		}
	}
	if (2819 != 2819) {
		int zmhapequl;
		for (zmhapequl = 78; zmhapequl > 0; zmhapequl--) {
			continue;
		}
	}

}

int omjfyli::snbfmzyckxeveqwwtgtlivlas(double rthds, int iasgn, double ogvkyoohxd, int xegny, bool wpmblneewhis, double snyltlblhw, string vhijzjasnxvhtl, int icjnttfbtt) {
	string zuvrnbvys = "ccgxpkvyrxthsjeuhcwfkmzycqxgqkickkafa";
	string sfrifgxnvak = "jiobjutsehlkeacrwwvlvxzktluypgduwpgkhhlshgxpjqznkpsinucstzpjfeukqgvukdawnqzzldjevixoqakt";
	double fefsfpmkjxr = 9047;
	if (string("jiobjutsehlkeacrwwvlvxzktluypgduwpgkhhlshgxpjqznkpsinucstzpjfeukqgvukdawnqzzldjevixoqakt") == string("jiobjutsehlkeacrwwvlvxzktluypgduwpgkhhlshgxpjqznkpsinucstzpjfeukqgvukdawnqzzldjevixoqakt")) {
		int mhwxt;
		for (mhwxt = 21; mhwxt > 0; mhwxt--) {
			continue;
		}
	}
	if (string("jiobjutsehlkeacrwwvlvxzktluypgduwpgkhhlshgxpjqznkpsinucstzpjfeukqgvukdawnqzzldjevixoqakt") == string("jiobjutsehlkeacrwwvlvxzktluypgduwpgkhhlshgxpjqznkpsinucstzpjfeukqgvukdawnqzzldjevixoqakt")) {
		int fohucgh;
		for (fohucgh = 75; fohucgh > 0; fohucgh--) {
			continue;
		}
	}
	if (string("ccgxpkvyrxthsjeuhcwfkmzycqxgqkickkafa") == string("ccgxpkvyrxthsjeuhcwfkmzycqxgqkickkafa")) {
		int hpsdcwlze;
		for (hpsdcwlze = 36; hpsdcwlze > 0; hpsdcwlze--) {
			continue;
		}
	}
	if (string("ccgxpkvyrxthsjeuhcwfkmzycqxgqkickkafa") != string("ccgxpkvyrxthsjeuhcwfkmzycqxgqkickkafa")) {
		int whe;
		for (whe = 35; whe > 0; whe--) {
			continue;
		}
	}
	return 58383;
}

double omjfyli::ctbdfuzwsodzpiqgupxkuatmb() {
	string flqoeabh = "btbljajbrvfcorrghemaqczhndaogqvqtqepgnupxhwyoytdlrhzoclljtvgbaahbiuccshtjhdotfjngletpwiqagwtgdnos";
	double slbcwplwcm = 18102;
	string dbdyswicfil = "";
	int tkfflzcjd = 805;
	double gfpnnhguahgoq = 67036;
	if (805 != 805) {
		int hfzabwpzl;
		for (hfzabwpzl = 82; hfzabwpzl > 0; hfzabwpzl--) {
			continue;
		}
	}
	if (string("btbljajbrvfcorrghemaqczhndaogqvqtqepgnupxhwyoytdlrhzoclljtvgbaahbiuccshtjhdotfjngletpwiqagwtgdnos") == string("btbljajbrvfcorrghemaqczhndaogqvqtqepgnupxhwyoytdlrhzoclljtvgbaahbiuccshtjhdotfjngletpwiqagwtgdnos")) {
		int gphbx;
		for (gphbx = 10; gphbx > 0; gphbx--) {
			continue;
		}
	}
	return 58703;
}

int omjfyli::kpakeraiutceqexswjzcxzrwc(int rjnogyfcvlau) {
	bool kwkrxvp = true;
	double owmioxgzjttd = 8538;
	string rdansbj = "zuiwyqyoplrgembaoeqilakrdylleomykyoprqzpvipjaptnaseejnvmqnmjawiiffkvqcfwebtkzvlbnuyja";
	double tmahmhn = 44342;
	int ufwvrm = 5978;
	if (true != true) {
		int anqindtxs;
		for (anqindtxs = 27; anqindtxs > 0; anqindtxs--) {
			continue;
		}
	}
	if (string("zuiwyqyoplrgembaoeqilakrdylleomykyoprqzpvipjaptnaseejnvmqnmjawiiffkvqcfwebtkzvlbnuyja") != string("zuiwyqyoplrgembaoeqilakrdylleomykyoprqzpvipjaptnaseejnvmqnmjawiiffkvqcfwebtkzvlbnuyja")) {
		int zwqx;
		for (zwqx = 81; zwqx > 0; zwqx--) {
			continue;
		}
	}
	if (5978 != 5978) {
		int rvt;
		for (rvt = 48; rvt > 0; rvt--) {
			continue;
		}
	}
	if (true == true) {
		int facfm;
		for (facfm = 25; facfm > 0; facfm--) {
			continue;
		}
	}
	if (44342 != 44342) {
		int uhxpovrbz;
		for (uhxpovrbz = 43; uhxpovrbz > 0; uhxpovrbz--) {
			continue;
		}
	}
	return 43909;
}

int omjfyli::dxtbmjwsdmicrahldwwtkyiz(double mptas, string kzftyyplxo, bool ynqmmp, int anggzhd, bool yksjr, bool roulnjytszvyr) {
	bool vkqvmwsc = true;
	string riymqf = "f";
	double ykwkxrmfviyvmbb = 8210;
	string dqfrnyk = "nrueiyzolmashnjwsezczkrsnicgbtkaoqihnzyrffplqvicmzoqxbfczqovkjhfizriijhvffrzdsmmin";
	bool qyhesqujtf = false;
	double ixwlmoljywwgn = 23930;
	bool yhoylpgdldhnafb = true;
	if (true != true) {
		int sertqgktx;
		for (sertqgktx = 50; sertqgktx > 0; sertqgktx--) {
			continue;
		}
	}
	if (true == true) {
		int mtx;
		for (mtx = 75; mtx > 0; mtx--) {
			continue;
		}
	}
	return 56465;
}

int omjfyli::iuzndqzdbbcrxchhzws(double enqowhqndubd, double eqmrhslyqpcjj, bool hwcpjxrajxgltv, int tconeosfrknplog, string jsvektwbtzqy, int amcjwdgdxqpnr, bool ybfrbhmmmzkxue, string gufqyhlsztgl, bool decojupdu, bool vvderwrwjhu) {
	double vkbhewea = 30142;
	int eaivnruma = 154;
	bool qaass = true;
	double bzxthu = 26753;
	if (30142 != 30142) {
		int seo;
		for (seo = 6; seo > 0; seo--) {
			continue;
		}
	}
	if (26753 == 26753) {
		int bezgcs;
		for (bezgcs = 2; bezgcs > 0; bezgcs--) {
			continue;
		}
	}
	return 35138;
}

void omjfyli::zanuezwedpkpybfr(bool ocirt, string qsvsavcimkrk, bool cywxrr, bool bculfdagivntyjx) {
	bool qymlipcaevien = true;
	int wkjlq = 1255;
	int fgtjdt = 2102;
	string ydzpzr = "oaffuqxbq";
	int kzqpgwuv = 6883;
	double koffpi = 5961;
	if (1255 == 1255) {
		int nlyyzoux;
		for (nlyyzoux = 19; nlyyzoux > 0; nlyyzoux--) {
			continue;
		}
	}
	if (1255 == 1255) {
		int qu;
		for (qu = 100; qu > 0; qu--) {
			continue;
		}
	}

}

string omjfyli::prmslpwghnpqknfephzu(double kmekrzjngbfbg, int mjnizf, string rmbjpiz, double gcsfdxiaoglkzqg) {
	int qbtskxnczzqqc = 4433;
	int rfibrkzbnmg = 2280;
	int skmdczlzjqretr = 7135;
	string jjedufwmbb = "czbloejahjqsbkznlmrdpvuxwoktjxfirsbokqnvtise";
	string lilfo = "xqahnmwfquotzmugylkvgrnopqmuodkanrgtbdoqjnnmnzxnkglq";
	double uhbyvmedehtdsyi = 13578;
	if (string("czbloejahjqsbkznlmrdpvuxwoktjxfirsbokqnvtise") == string("czbloejahjqsbkznlmrdpvuxwoktjxfirsbokqnvtise")) {
		int bntlyz;
		for (bntlyz = 83; bntlyz > 0; bntlyz--) {
			continue;
		}
	}
	if (2280 == 2280) {
		int nnejv;
		for (nnejv = 18; nnejv > 0; nnejv--) {
			continue;
		}
	}
	return string("");
}

void omjfyli::xmlvmmavthjo(double tvpmhtayuiz) {
	bool oazjhcxcqciol = false;
	string qdfnie = "jyiffcsadlegzvvylnaurccdpfkkofmqfqblqqibdyocyylzfgfqlfzyxmsdsezbpmbnrhqenowbfg";
	bool zdaepdrtxumfxs = true;
	double pasxsbkh = 11949;

}

void omjfyli::gdybghmfhzggagbof(double ytreyxqzx) {
	int azqttgi = 2526;
	bool docvptwelpz = false;
	string mpemydmyrbgyp = "xekqmhlpnsdhtebnhbdnmfabzywepurmqqwlplfuavywysovcjpyyqpegfcznyzjnoaszgfgldyhzrrudrv";
	double nehtmzwj = 19340;
	string leqjcxfthpohw = "cuxlumokwwjnkluqxssoqwannldk";
	bool jlrla = true;
	int ygevxgkst = 4050;
	if (true != true) {
		int zfthigr;
		for (zfthigr = 31; zfthigr > 0; zfthigr--) {
			continue;
		}
	}
	if (4050 != 4050) {
		int cf;
		for (cf = 1; cf > 0; cf--) {
			continue;
		}
	}
	if (19340 != 19340) {
		int jlwyjoyhfm;
		for (jlwyjoyhfm = 8; jlwyjoyhfm > 0; jlwyjoyhfm--) {
			continue;
		}
	}
	if (2526 == 2526) {
		int evwdk;
		for (evwdk = 66; evwdk > 0; evwdk--) {
			continue;
		}
	}
	if (2526 == 2526) {
		int uzoyrdfuhg;
		for (uzoyrdfuhg = 17; uzoyrdfuhg > 0; uzoyrdfuhg--) {
			continue;
		}
	}

}

void omjfyli::duejcsywihpknwjptucehwq(bool egwqojec, string sgztvlgdhftad, int wpbdf, bool mnzzlqvbreczuks, bool qqxfrref, double jutrcwtrgq, bool eiscejynvmcvt, double anfgzyp, int qmonrdqxffyls, bool zsclsapqamjfyp) {
	string fyaozvywcmi = "qcbxfxaphubufaowfukmmjvjdyemyxfdxnbposgvinyjykalhtaaunfcdkltuknrqvrwtbppzerrjunoevxvdslp";
	int nmwwqlac = 6014;
	int xobjymsk = 4641;
	int gtlpap = 790;

}

string omjfyli::omzofometowkup(bool kanblkdjtpo, bool tmbsfoxrrv, double nsgvgojdt, string madhcawsmpv, bool fckwfvvrxzjuf) {
	return string("bzthdxdh");
}

void omjfyli::pllxvpebsrd(double pbkvhzpavuqmcn, bool lulhsnnbac, int ujuakneb, int pzckohszlw, string pleohirjzpqlvhe, bool qrgul, int iloamldfvtm, int cxlycatkkaaf) {
	double mbdng = 2514;
	double xkmxm = 40854;
	int wtfdtlwbscq = 3001;
	double wvxfvopltwv = 81251;
	int rdgfl = 3144;
	bool lgxbjfpqfgusdto = false;
	double vcorgkwkrudhk = 13263;
	bool avyomfkz = false;
	int vaqjaxva = 685;
	if (3001 == 3001) {
		int rzuxhktvl;
		for (rzuxhktvl = 70; rzuxhktvl > 0; rzuxhktvl--) {
			continue;
		}
	}

}

string omjfyli::odwhsfjqsuuox(bool dqyqmanytz, bool nsydepz, bool eptaezyhpzsdbhf, int fewjpfgetaakhdi, bool klgby, int auvirxaodlv, string aydyvmkxi, bool ivgvkuaxu) {
	string erattxvrq = "gfumhojibahfsmmcjitbewbyctuooliaxqiwjydtdnjstdartvmzbhqf";
	string gaafzfvpri = "gyrbgikniexnovorxvwrxqblqhhsoysdpbpkshulltpsnfwbptssqgcbppacqhvtxbsvhgnqdeqsexndhnxwzyhijzee";
	bool yqsfl = true;
	string asvabyve = "nzqnwdvyvwapjdnchbjrwprutpihlevolptdroemxshjmvfydwoneluxjntdklxgqwkst";
	if (string("nzqnwdvyvwapjdnchbjrwprutpihlevolptdroemxshjmvfydwoneluxjntdklxgqwkst") == string("nzqnwdvyvwapjdnchbjrwprutpihlevolptdroemxshjmvfydwoneluxjntdklxgqwkst")) {
		int rbymv;
		for (rbymv = 91; rbymv > 0; rbymv--) {
			continue;
		}
	}
	if (string("gfumhojibahfsmmcjitbewbyctuooliaxqiwjydtdnjstdartvmzbhqf") == string("gfumhojibahfsmmcjitbewbyctuooliaxqiwjydtdnjstdartvmzbhqf")) {
		int kpgkr;
		for (kpgkr = 68; kpgkr > 0; kpgkr--) {
			continue;
		}
	}
	if (string("nzqnwdvyvwapjdnchbjrwprutpihlevolptdroemxshjmvfydwoneluxjntdklxgqwkst") == string("nzqnwdvyvwapjdnchbjrwprutpihlevolptdroemxshjmvfydwoneluxjntdklxgqwkst")) {
		int ihg;
		for (ihg = 22; ihg > 0; ihg--) {
			continue;
		}
	}
	return string("hhiyyj");
}

string omjfyli::qpbyjleouqvyad(string odiibxwifcy, int ksymh, bool qelewqgysyfag, string nkofqenyrh, double adyto, string vvbfnhp) {
	bool cxlry = false;
	if (false != false) {
		int hwbplak;
		for (hwbplak = 48; hwbplak > 0; hwbplak--) {
			continue;
		}
	}
	if (false == false) {
		int ohqjf;
		for (ohqjf = 78; ohqjf > 0; ohqjf--) {
			continue;
		}
	}
	if (false == false) {
		int jss;
		for (jss = 6; jss > 0; jss--) {
			continue;
		}
	}
	if (false == false) {
		int upphnqn;
		for (upphnqn = 32; upphnqn > 0; upphnqn--) {
			continue;
		}
	}
	if (false == false) {
		int icxryxm;
		for (icxryxm = 70; icxryxm > 0; icxryxm--) {
			continue;
		}
	}
	return string("a");
}

void omjfyli::adxxzvhcewuyouoybvgxqvxfe(int exeuatm, bool khlxpfnc, string cfqmhygopzvphv, string ybcyahymxxidyg, int lcvnkxckfg, int sucsmxfy) {

}

bool omjfyli::gvmhwsebpeeci(bool dyhomthzjztwthv, bool ecdgy, string wagbqgzs, string abysgsx, bool rvoqdnvexulm, bool xcvobkyc, string whzfivcgtowpge, int qrnwllpa, string jerxylilnphqthd, bool rbnqvkhipyqztxb) {
	double jvxmbjrgshbcm = 39335;
	string egrrla = "gqawrgtoujvjprpqwuxkm";
	int zrqzqjejc = 1020;
	double nhafyhsfsdibs = 48127;
	if (string("gqawrgtoujvjprpqwuxkm") == string("gqawrgtoujvjprpqwuxkm")) {
		int wknfmm;
		for (wknfmm = 4; wknfmm > 0; wknfmm--) {
			continue;
		}
	}
	if (string("gqawrgtoujvjprpqwuxkm") == string("gqawrgtoujvjprpqwuxkm")) {
		int cucipkr;
		for (cucipkr = 33; cucipkr > 0; cucipkr--) {
			continue;
		}
	}
	if (48127 == 48127) {
		int iealyqjt;
		for (iealyqjt = 5; iealyqjt > 0; iealyqjt--) {
			continue;
		}
	}
	return true;
}

string omjfyli::fsisixtnlpcpggt(string hsftqmewns, string bsjprqckf, string jiuxc, bool ymfmeeqbuophxlq, int ulxlr, double iktkcolmzcys, string erukxcsplcm, bool cstlxgjwirv) {
	string rnywycocssgfshp = "zbgsbfmwuauktmviugwswkycrhvhhwonzfoaajnilcqrsrqphjyputmdnwuoa";
	double oialzpazqxj = 88067;
	if (string("zbgsbfmwuauktmviugwswkycrhvhhwonzfoaajnilcqrsrqphjyputmdnwuoa") == string("zbgsbfmwuauktmviugwswkycrhvhhwonzfoaajnilcqrsrqphjyputmdnwuoa")) {
		int jrbzwxoypa;
		for (jrbzwxoypa = 38; jrbzwxoypa > 0; jrbzwxoypa--) {
			continue;
		}
	}
	if (string("zbgsbfmwuauktmviugwswkycrhvhhwonzfoaajnilcqrsrqphjyputmdnwuoa") != string("zbgsbfmwuauktmviugwswkycrhvhhwonzfoaajnilcqrsrqphjyputmdnwuoa")) {
		int tidrvsvp;
		for (tidrvsvp = 51; tidrvsvp > 0; tidrvsvp--) {
			continue;
		}
	}
	return string("vgryrliopoaakqzpwu");
}

int omjfyli::jlalehvfkasmkow(double vxfcphgzynsmg, int wkfyzns, int ltkoh, string ljszb) {
	bool qigciyjkzzieglr = true;
	string solljfdutzgd = "imcdchywkhyvpbjjfwevqzwtftxrcawkrtzdrfbewaaoubsnsukctproetprfqwekfuolqspfalevx";
	bool ritvvwkrblbiiw = false;
	if (true != true) {
		int dsmldj;
		for (dsmldj = 56; dsmldj > 0; dsmldj--) {
			continue;
		}
	}
	if (string("imcdchywkhyvpbjjfwevqzwtftxrcawkrtzdrfbewaaoubsnsukctproetprfqwekfuolqspfalevx") != string("imcdchywkhyvpbjjfwevqzwtftxrcawkrtzdrfbewaaoubsnsukctproetprfqwekfuolqspfalevx")) {
		int zyrbpteqj;
		for (zyrbpteqj = 82; zyrbpteqj > 0; zyrbpteqj--) {
			continue;
		}
	}
	if (false == false) {
		int clvjdvy;
		for (clvjdvy = 73; clvjdvy > 0; clvjdvy--) {
			continue;
		}
	}
	return 85542;
}

string omjfyli::ujnskncgkwivo(int zntkyltcchb, int thafjrh, bool rximmggpgpfa, string wzpwjqwv, int pymtx, double upgdayltj, int rcqqmti, bool qiskjay, string ezenjlptzecaseh) {
	double jvcqsenc = 34417;
	bool mhpiirrpmgklm = false;
	bool pshlwemaxp = true;
	if (34417 == 34417) {
		int qoozqw;
		for (qoozqw = 92; qoozqw > 0; qoozqw--) {
			continue;
		}
	}
	return string("sunznyegv");
}

omjfyli::omjfyli() {
	this->qpbyjleouqvyad(string("xptgbuyfxu"), 233, false, string("hxylkxfryfpkeofhvhysujnickjhrodhfksqimfbwfrwrkuzluaajekihofgiwf"), 10815, string("ckizjeocwiaszqybgghqdsmw"));
	this->adxxzvhcewuyouoybvgxqvxfe(4654, false, string("vrnpxgyuipoqhriybkqstkotacevapnorxmdoyjkmnvkxitjxmovlq"), string("zrxkvieynczyqgq"), 1944, 2394);
	this->gvmhwsebpeeci(false, true, string("gxkqojr"), string("djmyctohgxxffrftlrwszhgnksgdeyimkg"), true, true, string("kxwmmcfcpr"), 1963, string("lhgnacfojrehowqwivvfqeoorqdnwhakxprqrqmijebuwgfkhrwvjxkrsttxcqbzzzzpcpivgkmzubrfz"), false);
	this->fsisixtnlpcpggt(string("dzwxhfgaixetpevmvbicybcgeul"), string("yqvrxevizvqfdtaqbiefchbfggmtibufyaxhcsbfxhzrsovcvmelsompgiacbnqnte"), string("rylduwjmudkdzbqyxrfk"), false, 1828, 24013, string(""), true);
	this->jlalehvfkasmkow(31888, 3379, 94, string("rcgwxfhffheochrwkqlcvggrxhbcunxjinsivmqvcppboclqcqdyapsrnctmrhqelylvjwxxotthurewjkxnydcaqhjadewcckd"));
	this->ujnskncgkwivo(649, 1403, false, string("mojyvpsqopknjsqpdaedhedgqprhvwmlgbwktsbhtjudqmwdfapwavyxwyvbuvwpngqxdlyabsvmxyawwkbjtcfyhvz"), 4132, 61078, 5840, false, string("zafbbsdcossice"));
	this->zanuezwedpkpybfr(true, string("kl"), false, false);
	this->prmslpwghnpqknfephzu(85494, 8094, string("pihihmp"), 64377);
	this->xmlvmmavthjo(60188);
	this->gdybghmfhzggagbof(51306);
	this->duejcsywihpknwjptucehwq(false, string("rgjfverhdtcttolaggbtqfpkxnxfauomxlxsxwqzabsgxsclhamfbacwfhsiaexz"), 610, true, false, 66005, true, 41951, 5424, true);
	this->omzofometowkup(true, false, 11896, string("srdkgvfdmjnwjfydmkmtxuoqlobclzmahjcxkzwxhxc"), true);
	this->pllxvpebsrd(1520, true, 1455, 2860, string("oihwtihxydsirgitvexuuitzqsrmqwavwljqpjozkhhwtrec"), false, 6206, 5244);
	this->odwhsfjqsuuox(false, true, false, 1009, false, 4603, string("tizjzusxoufegootgeti"), false);
	this->cofuolfkeaoanmuxe(4738, string("quycegjurowl"), string("pdxiiothhjvpyjevnphatufnrynekbzyyruryqeghkilbvfxheskcjuhjeyujciz"), 162, 6304, string("kicdmfjyo"));
	this->nlpkycihje(2722, true, string("esikpvkgntacpwvxhwgpncidzsfjtrirqhfemwnaabispnixaoergnfwmvkpzex"), string("mtgdtipqjpeeiapsvmcqoonkvwnbynlssytxnxtgkbcdgvystyoypocogdbbnslddivok"), string("xmvprsaytnjwgvazvetxufmomuqlbdzusgagpzjtmkorsqqniegzkhwsnlf"), string("brqhjmncqixerrrnopdrryltdjpzdpbgs"), 2203);
	this->snbfmzyckxeveqwwtgtlivlas(27895, 1547, 16142, 3810, true, 26860, string("pisdmcmveeamvzistsyhdnpfqdznvjbavfwx"), 3848);
	this->ctbdfuzwsodzpiqgupxkuatmb();
	this->kpakeraiutceqexswjzcxzrwc(7430);
	this->dxtbmjwsdmicrahldwwtkyiz(3577, string("ymtwfzygpeupjnuspeyafjaklojkfscrxycwwlbdleqjwojtrbzjnjccckseyozulaoulslxyodswehazyimicdjhgbb"), true, 3252, false, true);
	this->iuzndqzdbbcrxchhzws(3618, 4595, true, 7724, string("elqgqx"), 4969, true, string("cxxqddyadqjyswusnytxmkoebnerszakzbtpbqerplylivsaewlgsjyjugnyvgcatrgesnlmwqeyedoliubfqaid"), true, false);
}

#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class igdzojy {
public:
	bool wfloqslfu;
	int vjiju;
	double muqizgsvscqvh;
	bool sfrckaobcvttdn;
	igdzojy();
	bool cepmozbmsyi(bool ykucxjrnxt, bool twrlrnrvgqtjpe, int bgnnxdsp, int gaakxhp, int ukprixccrvcxed);
	double txzrudiyhqclwsgozhinb();
	string iehcvygotgdktpvtyqw(double rfhvjbxeio, int atnuvv, bool tlyaqbyynizezf, string ctaksosfv, bool qfradxpmahka, bool pyhoodmsgka, double exvitvcuwkajhzc, bool wtppjwaodzpcby, int bhtnc);
	bool etxpetxrilzuvadzwhi(double khzkqumdxbldnls, int plewacexg, int cbrdyuq, int puwmdoh, double jycdkvekzny, int coqfydql, int otuzk, int nlybgvttijxex);

protected:
	bool bsaaqd;

	bool iuhhcfjizxncmmsujr(int rajpnkvpzzo, double esjmwbpv, string spzziyjzs, double rvxefpazkhq, bool cwznnxzzlxwu);
	string yxmpkmkujvqcmldzzpdjpa(double bystpct, bool wpsnbzfmw, bool plqptrtamyagovt, int dqaezxdqhucvwpf);
	double ggkldobofjagbadppolzhju(string gpfuvzrgdrma, string nvwgnfqpjhg, int cnpmqdyxpu);
	int fhhybuugvzjndxnbwimg(string hrgeouim, string dvcralnhcw);
	string geftdtgkbdq(string vlsopacnul, string nkzctiunckvyf, int zlpmghhstdtzef);
	bool fonvhzzprico(bool ufwyagh, double qxcsthbkzwksb);
	double nyhtwmyzcbifjnse();

private:
	int zavxfyin;
	double yuanluhfxlqft;

	int rqnicwwzlcmfbw(double usscmk, bool wkcydzdused, int ujorosikqrgh, double ldbncgukh, bool vluiczxyajwcmbq, string whhnoyub, int qhtqbsnxgoqjalx);
	bool nlsettesrsducetdfqi(string volsixqozkiqyzo, int mjewpxgberayqr, int gkzsgsxhmuyhdx, bool xiysbpddn);
	void rlpojcxuohqntenomkgde(int ltnxljaee, int bdqojqkp, bool eegnjvmpc, double mijjk, int fujwnuo, bool gvaiyiujwuxh, bool reatwdnjjcz);
	bool hpkkyhylrq(double gwtpknui, int czqdfgilgmh, double fsowncurhyr);

};


int igdzojy::rqnicwwzlcmfbw(double usscmk, bool wkcydzdused, int ujorosikqrgh, double ldbncgukh, bool vluiczxyajwcmbq, string whhnoyub, int qhtqbsnxgoqjalx) {
	double utxubj = 7835;
	string wuexut = "nlmmwilvkuzkxtszocopwdlqivruspkiihitstnotlhhmirnivtjranxultlykhz";
	int uusqzb = 2881;
	int wivigpforgnwjed = 2808;
	int fxbwycgyzb = 6486;
	bool vbeedb = true;
	int hhljpanckkdqtz = 545;
	string monsc = "obrvzhoefgzittzzbwzhzyytagwgbqxyaqdmrnapsosytgafrfdgyzgbhibdldyzhrkvoufomvlcoelogrhddqogwua";
	if (6486 != 6486) {
		int rir;
		for (rir = 16; rir > 0; rir--) {
			continue;
		}
	}
	if (6486 != 6486) {
		int gtxpjj;
		for (gtxpjj = 45; gtxpjj > 0; gtxpjj--) {
			continue;
		}
	}
	if (2808 != 2808) {
		int nq;
		for (nq = 56; nq > 0; nq--) {
			continue;
		}
	}
	if (true == true) {
		int va;
		for (va = 63; va > 0; va--) {
			continue;
		}
	}
	if (545 != 545) {
		int cxxon;
		for (cxxon = 56; cxxon > 0; cxxon--) {
			continue;
		}
	}
	return 68987;
}

bool igdzojy::nlsettesrsducetdfqi(string volsixqozkiqyzo, int mjewpxgberayqr, int gkzsgsxhmuyhdx, bool xiysbpddn) {
	int tnmhawermqyfe = 1296;
	bool jkqmpx = false;
	double kyfjlvostmo = 41910;
	bool xhasjuyojzxh = false;
	bool ukbzwphvov = true;
	if (1296 != 1296) {
		int yf;
		for (yf = 15; yf > 0; yf--) {
			continue;
		}
	}
	if (false != false) {
		int kjpoobbi;
		for (kjpoobbi = 48; kjpoobbi > 0; kjpoobbi--) {
			continue;
		}
	}
	return true;
}

void igdzojy::rlpojcxuohqntenomkgde(int ltnxljaee, int bdqojqkp, bool eegnjvmpc, double mijjk, int fujwnuo, bool gvaiyiujwuxh, bool reatwdnjjcz) {
	string xkcjtzbsbatrkos = "tawxermsfuegrvekmpwghcvpezaayxtucafupdjecvwmfwhamxuyozcthsylhfymchuykhcrjipkvqggztiggpuxpnwdv";
	string adwhqbklzkshcyp = "ogfygdtxslhyxacqqesdnwqbf";
	int dvndqsbtucuvzef = 4032;
	string zdvqoanudahpxpe = "xfihz";
	bool hvjmpur = false;
	bool wbbqbaqvxa = true;
	double qilgebzybj = 231;
	double fpyybldvfw = 3181;
	bool euwwtyurs = true;
	if (false == false) {
		int lqyqresnvb;
		for (lqyqresnvb = 28; lqyqresnvb > 0; lqyqresnvb--) {
			continue;
		}
	}
	if (true != true) {
		int zpt;
		for (zpt = 83; zpt > 0; zpt--) {
			continue;
		}
	}
	if (4032 == 4032) {
		int kdcnbmpw;
		for (kdcnbmpw = 41; kdcnbmpw > 0; kdcnbmpw--) {
			continue;
		}
	}
	if (true == true) {
		int ujqzpqau;
		for (ujqzpqau = 13; ujqzpqau > 0; ujqzpqau--) {
			continue;
		}
	}

}

bool igdzojy::hpkkyhylrq(double gwtpknui, int czqdfgilgmh, double fsowncurhyr) {
	int rcxellygmb = 256;
	bool zyrkextgr = true;
	bool zzlbgbpcz = false;
	int ucsebgzgnjffvt = 1370;
	int zklfbgwyfprlkr = 4102;
	return false;
}

bool igdzojy::iuhhcfjizxncmmsujr(int rajpnkvpzzo, double esjmwbpv, string spzziyjzs, double rvxefpazkhq, bool cwznnxzzlxwu) {
	int utplptfeqt = 2680;
	bool gqxfoabvarbfmhe = true;
	string eotrro = "iigqcowfjkzc";
	bool emumzydufpjily = false;
	string tvidkgoz = "ioobnshzovuarelehggcic";
	int ejohmc = 381;
	double pmqimfd = 27526;
	bool njklwsjyg = false;
	return true;
}

string igdzojy::yxmpkmkujvqcmldzzpdjpa(double bystpct, bool wpsnbzfmw, bool plqptrtamyagovt, int dqaezxdqhucvwpf) {
	bool mejpk = true;
	if (true == true) {
		int ge;
		for (ge = 58; ge > 0; ge--) {
			continue;
		}
	}
	if (true != true) {
		int oljmc;
		for (oljmc = 35; oljmc > 0; oljmc--) {
			continue;
		}
	}
	return string("civkzmxkzf");
}

double igdzojy::ggkldobofjagbadppolzhju(string gpfuvzrgdrma, string nvwgnfqpjhg, int cnpmqdyxpu) {
	string kilsfzg = "rjzfbkwxyscufujytjfdaddakwzdsaqmabxndkcvvhoyqgrlvuxpvrfkogcwbrhgwkpyq";
	string kyqztpsf = "gcpyeiwsdpbtxbjkfqkekualkatqdawtvihyzdmbsxbtykntvrsuqeotmvbfasvloruhlxfvjhqtoq";
	bool ueatihh = true;
	string jnqbljauqze = "uqojavprbnqrbhetbbttzgrawp";
	int arvywxs = 1113;
	string qocjkqcnewys = "pvkadjqhieigeflydqzsalkkqhmqewbgvdijujveexexc";
	string hpeejxukllmgo = "jxfpzynuvhnzmxjgjnsmiqoanazsntpoudzxqobyngcp";
	int cgxuk = 4279;
	double ulhlsiprn = 30521;
	int edrasvzwn = 2471;
	if (string("gcpyeiwsdpbtxbjkfqkekualkatqdawtvihyzdmbsxbtykntvrsuqeotmvbfasvloruhlxfvjhqtoq") != string("gcpyeiwsdpbtxbjkfqkekualkatqdawtvihyzdmbsxbtykntvrsuqeotmvbfasvloruhlxfvjhqtoq")) {
		int cvdnksz;
		for (cvdnksz = 72; cvdnksz > 0; cvdnksz--) {
			continue;
		}
	}
	return 89669;
}

int igdzojy::fhhybuugvzjndxnbwimg(string hrgeouim, string dvcralnhcw) {
	int yrbzvbysdsppz = 4741;
	string nfqgvnf = "kbklaiouwageykiohhhlksjrkwxmyiefvdljvkoqnfdfxcsogzbqywyjashosjptaccpvfwuqmqxcbvsaitmfglnxukcvsrb";
	double vdsdfrq = 67576;
	string acchopsbhsvphof = "ojzfzvsvcvvppunacmqwa";
	int umfuatcumyspejf = 5566;
	if (5566 == 5566) {
		int rjfsorpjmu;
		for (rjfsorpjmu = 39; rjfsorpjmu > 0; rjfsorpjmu--) {
			continue;
		}
	}
	if (string("kbklaiouwageykiohhhlksjrkwxmyiefvdljvkoqnfdfxcsogzbqywyjashosjptaccpvfwuqmqxcbvsaitmfglnxukcvsrb") == string("kbklaiouwageykiohhhlksjrkwxmyiefvdljvkoqnfdfxcsogzbqywyjashosjptaccpvfwuqmqxcbvsaitmfglnxukcvsrb")) {
		int aabjb;
		for (aabjb = 39; aabjb > 0; aabjb--) {
			continue;
		}
	}
	return 70882;
}

string igdzojy::geftdtgkbdq(string vlsopacnul, string nkzctiunckvyf, int zlpmghhstdtzef) {
	int zofspbwz = 4948;
	double vwwtewngonfe = 64586;
	double wbxxtkquynp = 64795;
	double wsmyrr = 20336;
	int qmuqn = 3503;
	bool unkiegmzav = false;
	string fdsfdhj = "znsyknemiycqhzpxlilblscby";
	double nuqxgcpf = 49849;
	bool pqwynyeouvn = true;
	if (64586 != 64586) {
		int aklnwogyej;
		for (aklnwogyej = 7; aklnwogyej > 0; aklnwogyej--) {
			continue;
		}
	}
	if (4948 != 4948) {
		int bdlz;
		for (bdlz = 97; bdlz > 0; bdlz--) {
			continue;
		}
	}
	if (49849 != 49849) {
		int bbfuaoqqk;
		for (bbfuaoqqk = 94; bbfuaoqqk > 0; bbfuaoqqk--) {
			continue;
		}
	}
	if (true == true) {
		int uhh;
		for (uhh = 87; uhh > 0; uhh--) {
			continue;
		}
	}
	return string("frlxtqkkfnrtyvljw");
}

bool igdzojy::fonvhzzprico(bool ufwyagh, double qxcsthbkzwksb) {
	string zmuweothpwwi = "ketjbhjjmbvibkfychbimllzvskivxihasfuogiaegwsvjjrbaivvpnygawmxszltmtiueygwggbpzqznhzwfibfxkgbo";
	bool ipvvc = true;
	string apiqu = "rhgvcpzoubrqlwrcxxhmpxcassbymrggahhfkgjijyzjwrgxxynsnurtcpkrsmjxavsgyjpedcnjtricjbkfeuemzqhe";
	double edrtctoyvekrigi = 35537;
	bool njtzua = false;
	int wtjgkrdj = 3083;
	bool iopesytwtbm = false;
	if (35537 != 35537) {
		int gxfm;
		for (gxfm = 97; gxfm > 0; gxfm--) {
			continue;
		}
	}
	if (false != false) {
		int qxbuym;
		for (qxbuym = 88; qxbuym > 0; qxbuym--) {
			continue;
		}
	}
	if (string("rhgvcpzoubrqlwrcxxhmpxcassbymrggahhfkgjijyzjwrgxxynsnurtcpkrsmjxavsgyjpedcnjtricjbkfeuemzqhe") == string("rhgvcpzoubrqlwrcxxhmpxcassbymrggahhfkgjijyzjwrgxxynsnurtcpkrsmjxavsgyjpedcnjtricjbkfeuemzqhe")) {
		int yudzz;
		for (yudzz = 3; yudzz > 0; yudzz--) {
			continue;
		}
	}
	return true;
}

double igdzojy::nyhtwmyzcbifjnse() {
	double lqfchqir = 56001;
	string pzylnyhhz = "hpsrahzumjvhkv";
	int rhsiytmfpmjwnp = 2354;
	double orfmibatgf = 33903;
	bool jkoyutirosaqr = false;
	if (false != false) {
		int ihvsr;
		for (ihvsr = 50; ihvsr > 0; ihvsr--) {
			continue;
		}
	}
	return 76736;
}

bool igdzojy::cepmozbmsyi(bool ykucxjrnxt, bool twrlrnrvgqtjpe, int bgnnxdsp, int gaakxhp, int ukprixccrvcxed) {
	int mtukh = 669;
	double pbszhjatyqdom = 12735;
	if (12735 == 12735) {
		int bofgmepicl;
		for (bofgmepicl = 39; bofgmepicl > 0; bofgmepicl--) {
			continue;
		}
	}
	return true;
}

double igdzojy::txzrudiyhqclwsgozhinb() {
	bool thbdrknxzhxnjs = false;
	int rbnchc = 3874;
	bool hurarhxuvkv = true;
	double fipgffidqwac = 28361;
	bool uwfbfmoaia = false;
	int qgtcmfbztkibde = 2221;
	int lxivoshpcs = 266;
	int tojawdlbpirmd = 864;
	int egclfpukit = 7676;
	int eqkzqzuwttucusw = 1879;
	if (266 == 266) {
		int xcdrlyvrm;
		for (xcdrlyvrm = 65; xcdrlyvrm > 0; xcdrlyvrm--) {
			continue;
		}
	}
	if (false == false) {
		int heoyg;
		for (heoyg = 15; heoyg > 0; heoyg--) {
			continue;
		}
	}
	if (1879 != 1879) {
		int lfetlryx;
		for (lfetlryx = 49; lfetlryx > 0; lfetlryx--) {
			continue;
		}
	}
	if (28361 == 28361) {
		int zrwsslfqt;
		for (zrwsslfqt = 62; zrwsslfqt > 0; zrwsslfqt--) {
			continue;
		}
	}
	if (28361 != 28361) {
		int zvbrrkrv;
		for (zvbrrkrv = 38; zvbrrkrv > 0; zvbrrkrv--) {
			continue;
		}
	}
	return 45531;
}

string igdzojy::iehcvygotgdktpvtyqw(double rfhvjbxeio, int atnuvv, bool tlyaqbyynizezf, string ctaksosfv, bool qfradxpmahka, bool pyhoodmsgka, double exvitvcuwkajhzc, bool wtppjwaodzpcby, int bhtnc) {
	double aewwrauizdql = 11947;
	double knhvndlsj = 8263;
	string usrocfp = "zfjjwqtfhswwsoimnwijmgauxoirybodehvbafzkhbmblcyjasphciwkqdgneppjgzjmumyffgulnwsrsfeurml";
	string ayckrrmklwp = "vdftoniq";
	double xlikeuzcpsdqqug = 59501;
	return string("dhvursctl");
}

bool igdzojy::etxpetxrilzuvadzwhi(double khzkqumdxbldnls, int plewacexg, int cbrdyuq, int puwmdoh, double jycdkvekzny, int coqfydql, int otuzk, int nlybgvttijxex) {
	return false;
}

igdzojy::igdzojy() {
	this->cepmozbmsyi(true, true, 1344, 2752, 724);
	this->txzrudiyhqclwsgozhinb();
	this->iehcvygotgdktpvtyqw(80, 671, true, string("qxkxlysfcascvvwhlowtchkdlxijuxrjkxmpaohwiyrelwmgtarlbfjkuhogycn"), false, true, 30318, true, 513);
	this->etxpetxrilzuvadzwhi(88326, 3244, 67, 2802, 47550, 906, 216, 579);
	this->iuhhcfjizxncmmsujr(2177, 36998, string("larsuiyvbiwdgbappuiupqnhpjmxgyngmthxfhmojusxrakxtgttd"), 4163, true);
	this->yxmpkmkujvqcmldzzpdjpa(53377, false, false, 1708);
	this->ggkldobofjagbadppolzhju(string("ganalcthbdlbczixuxedllmgqlvvmyhmgxiberhwtnjfsiawpyxuegwnvgaugtpytljdgcwmtxjjiwyqoooqsqm"), string("ejssalmyikqop"), 2396);
	this->fhhybuugvzjndxnbwimg(string("roolwnmfovxjbwihsxiphlr"), string("hcxspxslcdodjqklaxneezjjawzz"));
	this->geftdtgkbdq(string("kbrwfsopuamrfmswkrmgqztwupavfixuwyqzjwyykbeo"), string("hkeorfoefqsvgsrelxewelyuitsogxwgycojwlmsngiqiqcebywtncongvedcztayagdudomcvionhfdbpghto"), 4853);
	this->fonvhzzprico(false, 6800);
	this->nyhtwmyzcbifjnse();
	this->rqnicwwzlcmfbw(28013, true, 4413, 18218, true, string("swdzmvahoqcsidladjluendzczgzvzcwjwawlupyfy"), 130);
	this->nlsettesrsducetdfqi(string("fojhpbosybcakaabgplzufbuqztsrmbpzta"), 2961, 5915, false);
	this->rlpojcxuohqntenomkgde(4830, 1869, true, 11914, 431, true, false);
	this->hpkkyhylrq(61316, 567, 46426);
}

#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class jypqytg {
public:
	bool mwwxsllvciwkeu;
	int dnzjcubbmttwqy;
	int vplpbqdorzebmgy;
	jypqytg();
	string eduogkluuillyb(int pmilpg, string rhtpknkrqyljbv, double ujwqttybfobzlmd, double kptroyxrlbxti, string zgfadkvsif, string kquirchgasjz, string jmezbwlvrpq);
	bool whchhorfhtmecykmmc(double mulsymbdvzuv, string hgulpiozr, string ewldqz, double cfcmomylgvkxf, string oxremfhhmp, double ezlrmt, string pwdolyixlaiqbs, bool vikyngrldp);
	bool fclhmjwxslkgmzh(int iinuqyptcsjeud, string npujipdbjzce, double rqkjugszfta, string botrwernphfnmd, double yrbkokzlogrlglq, bool bhuqygw, double fikysvzzfl, string ugqnvsaycofnv, string fzmazprayahuim, string ydafjglpx);
	int qtiyheasyixvbuebscyq(bool yitssejnahqgmxm, int hxplvmryptivxs, double qyounoqulkargq, string pyyplvmzu, double tnqvcjueudsdqe, bool zhfqhybwa, int wlnutz, bool esvhhiwya);
	int pbenkazngd(int dmtlqeqybwehx);
	bool bgolpomtiqmtgsam(bool scustn);
	bool mrbttxynfsibiajv(string hhnnvstugqdgd, string piheqwhgbspl, string ltjdorevjokjrs, double yrspn);
	bool ygydxtsitss(bool wzxkriirylis, int zdhlzwmlxlyny, double fbmfmhxqicurel, bool okkacheyhfi, bool jfdtmey, double igldn, int esibcbbvrz);
	void lfwbtiwhrdqea(int xzgxr, string owdvplilucffws, bool bbckmkp, string mitqt);
	string zuqcnkoshrv(double zyhvuzp, bool bbjtmueurygpv, bool dxzpfdqwhxox, string svkjyx, string docmzqrswwv, double dyzsbuszmjzndq, string wyfxxrkfmfwz, double vnsiaflcv);

protected:
	int kbcpfjelheuj;

	double lqvfusckntljzseiqnp(bool kqqyjdmjqfah, bool fagcllqefec);

private:
	string owuydedug;
	int gopnj;
	string usmfatbwkz;

	double xvvkulumruqgfr();

};


double jypqytg::xvvkulumruqgfr() {
	bool qduaujbdsgqso = false;
	string eoaeqpaglwpkjmc = "ayybwpqeclindjwgutbqfinmppaflrtqpmwnkxfbidovvnckuaftvnxipzdvavitnmcvfvsjhqxn";
	if (false == false) {
		int qsd;
		for (qsd = 78; qsd > 0; qsd--) {
			continue;
		}
	}
	if (false == false) {
		int urw;
		for (urw = 22; urw > 0; urw--) {
			continue;
		}
	}
	if (string("ayybwpqeclindjwgutbqfinmppaflrtqpmwnkxfbidovvnckuaftvnxipzdvavitnmcvfvsjhqxn") != string("ayybwpqeclindjwgutbqfinmppaflrtqpmwnkxfbidovvnckuaftvnxipzdvavitnmcvfvsjhqxn")) {
		int hlyydfwbsx;
		for (hlyydfwbsx = 67; hlyydfwbsx > 0; hlyydfwbsx--) {
			continue;
		}
	}
	if (string("ayybwpqeclindjwgutbqfinmppaflrtqpmwnkxfbidovvnckuaftvnxipzdvavitnmcvfvsjhqxn") != string("ayybwpqeclindjwgutbqfinmppaflrtqpmwnkxfbidovvnckuaftvnxipzdvavitnmcvfvsjhqxn")) {
		int sjs;
		for (sjs = 92; sjs > 0; sjs--) {
			continue;
		}
	}
	if (false != false) {
		int vzrxa;
		for (vzrxa = 43; vzrxa > 0; vzrxa--) {
			continue;
		}
	}
	return 87060;
}

double jypqytg::lqvfusckntljzseiqnp(bool kqqyjdmjqfah, bool fagcllqefec) {
	bool tcobc = false;
	double cinirqiyog = 17472;
	bool uvytoecuaduej = true;
	string usfwdlqpapqvil = "nxzdpgunxiuvksbbugdcdndkphegshjbptkiqrxqvidgtamqxsrlhumeqbxtaeqnsrzbeikqqjjffk";
	int dfkmfdz = 889;
	bool zkntjusizbdy = true;
	double ralvgz = 64793;
	string aanzfmyp = "tkjebirhrztzymtmfhbrswvkqrphlwfrmlmfcxvkwlivigcrudnptjbdpmvshcgjdqgoohfxbxdfnsgwtuixyi";
	int yhwskodynhbbta = 1035;
	bool lpdsnvrgluj = true;
	if (false == false) {
		int npsishug;
		for (npsishug = 51; npsishug > 0; npsishug--) {
			continue;
		}
	}
	if (64793 == 64793) {
		int ysiws;
		for (ysiws = 33; ysiws > 0; ysiws--) {
			continue;
		}
	}
	return 42271;
}

string jypqytg::eduogkluuillyb(int pmilpg, string rhtpknkrqyljbv, double ujwqttybfobzlmd, double kptroyxrlbxti, string zgfadkvsif, string kquirchgasjz, string jmezbwlvrpq) {
	int xkiechcy = 932;
	bool chrraw = false;
	string dzlrvoexi = "wmtblkyjzgdwmzinwzdraujsfkvk";
	double renkmp = 54480;
	int itpcur = 1562;
	double azjamwjtcnpn = 1659;
	double mptsfanvnafv = 42571;
	bool bgxzmjz = false;
	string hefmowmsvqcf = "sslzlqzmawrifrcafllmrwhwwunsnjnrchzdojgxctsixwhmt";
	double imfzclcxakp = 31474;
	if (false == false) {
		int ac;
		for (ac = 9; ac > 0; ac--) {
			continue;
		}
	}
	if (1659 != 1659) {
		int jzpfwng;
		for (jzpfwng = 45; jzpfwng > 0; jzpfwng--) {
			continue;
		}
	}
	if (42571 == 42571) {
		int eltifhyh;
		for (eltifhyh = 94; eltifhyh > 0; eltifhyh--) {
			continue;
		}
	}
	if (42571 != 42571) {
		int oubrs;
		for (oubrs = 67; oubrs > 0; oubrs--) {
			continue;
		}
	}
	if (string("wmtblkyjzgdwmzinwzdraujsfkvk") == string("wmtblkyjzgdwmzinwzdraujsfkvk")) {
		int tlfpelr;
		for (tlfpelr = 20; tlfpelr > 0; tlfpelr--) {
			continue;
		}
	}
	return string("oblauekvwzbwzxmpqpnh");
}

bool jypqytg::whchhorfhtmecykmmc(double mulsymbdvzuv, string hgulpiozr, string ewldqz, double cfcmomylgvkxf, string oxremfhhmp, double ezlrmt, string pwdolyixlaiqbs, bool vikyngrldp) {
	string xulpqtpdqwhnmc = "kxesy";
	double kbfjgalztv = 34340;
	int przmfjikrgjxqkl = 1742;
	if (string("kxesy") != string("kxesy")) {
		int qssgkxf;
		for (qssgkxf = 63; qssgkxf > 0; qssgkxf--) {
			continue;
		}
	}
	return false;
}

bool jypqytg::fclhmjwxslkgmzh(int iinuqyptcsjeud, string npujipdbjzce, double rqkjugszfta, string botrwernphfnmd, double yrbkokzlogrlglq, bool bhuqygw, double fikysvzzfl, string ugqnvsaycofnv, string fzmazprayahuim, string ydafjglpx) {
	int zqvaqvgcbqyvs = 4562;
	string ezicbpftrr = "qmyryqmnusiwgnfhyncmctcbnxdyznxmmzbymjtsaygjjsedarvvrjqhoybry";
	bool hwnwomifd = true;
	string ptcbzuf = "dfeacethjyvurmjflqvakjldorxfxevqbewfjja";
	double nfnhwyltnml = 45905;
	string pvrktdfqlzyfiik = "zmjudckrdpptfizr";
	double eaeijxogqthbgj = 61923;
	int vvazbd = 4032;
	if (4032 != 4032) {
		int faedjbayp;
		for (faedjbayp = 95; faedjbayp > 0; faedjbayp--) {
			continue;
		}
	}
	if (61923 == 61923) {
		int lwvsqe;
		for (lwvsqe = 64; lwvsqe > 0; lwvsqe--) {
			continue;
		}
	}
	return false;
}

int jypqytg::qtiyheasyixvbuebscyq(bool yitssejnahqgmxm, int hxplvmryptivxs, double qyounoqulkargq, string pyyplvmzu, double tnqvcjueudsdqe, bool zhfqhybwa, int wlnutz, bool esvhhiwya) {
	string xjzwopetadi = "snmtvinoqoyrxisdtqodchmkovrihundavurwhughcuikqcbrogdcrsgbjrjzllxmurydxiqwxwraecz";
	int pdawk = 987;
	double wjuwmwkmxnaenpa = 81127;
	int vdeux = 2387;
	int qezdlfzbpmi = 997;
	bool oftdtmxuauzcf = false;
	double amwlwinncym = 16275;
	bool mhcwvj = false;
	bool bqfxzo = false;
	int xcniv = 1930;
	if (997 != 997) {
		int bagkenydw;
		for (bagkenydw = 63; bagkenydw > 0; bagkenydw--) {
			continue;
		}
	}
	if (997 != 997) {
		int xzbeivmv;
		for (xzbeivmv = 95; xzbeivmv > 0; xzbeivmv--) {
			continue;
		}
	}
	if (16275 == 16275) {
		int sddd;
		for (sddd = 54; sddd > 0; sddd--) {
			continue;
		}
	}
	if (false != false) {
		int pmzhh;
		for (pmzhh = 34; pmzhh > 0; pmzhh--) {
			continue;
		}
	}
	if (987 == 987) {
		int ql;
		for (ql = 13; ql > 0; ql--) {
			continue;
		}
	}
	return 74881;
}

int jypqytg::pbenkazngd(int dmtlqeqybwehx) {
	double ostlrqbd = 22451;
	string kghbuvxvrvc = "pzazuulnbioscduthhzlixpxjcawktmcpumthyxosbzsbxvcytgkzxqhgxtu";
	double izpmabvwkl = 30503;
	int lgdvfpbwssxq = 4412;
	double umvbljxud = 19445;
	int dbrgru = 6370;
	int vppkbfgrjfn = 290;
	int eexyffrniv = 585;
	bool wcdur = false;
	bool hoyjatqxjldlkp = true;
	if (585 != 585) {
		int jxadpvn;
		for (jxadpvn = 23; jxadpvn > 0; jxadpvn--) {
			continue;
		}
	}
	if (6370 == 6370) {
		int qbmdalrbke;
		for (qbmdalrbke = 68; qbmdalrbke > 0; qbmdalrbke--) {
			continue;
		}
	}
	if (string("pzazuulnbioscduthhzlixpxjcawktmcpumthyxosbzsbxvcytgkzxqhgxtu") != string("pzazuulnbioscduthhzlixpxjcawktmcpumthyxosbzsbxvcytgkzxqhgxtu")) {
		int vgm;
		for (vgm = 60; vgm > 0; vgm--) {
			continue;
		}
	}
	if (22451 == 22451) {
		int hdoruplhh;
		for (hdoruplhh = 7; hdoruplhh > 0; hdoruplhh--) {
			continue;
		}
	}
	if (false != false) {
		int dsqdao;
		for (dsqdao = 54; dsqdao > 0; dsqdao--) {
			continue;
		}
	}
	return 83070;
}

bool jypqytg::bgolpomtiqmtgsam(bool scustn) {
	bool adtyy = false;
	if (false != false) {
		int fbfy;
		for (fbfy = 41; fbfy > 0; fbfy--) {
			continue;
		}
	}
	if (false == false) {
		int ajpa;
		for (ajpa = 78; ajpa > 0; ajpa--) {
			continue;
		}
	}
	if (false != false) {
		int nuoacgu;
		for (nuoacgu = 23; nuoacgu > 0; nuoacgu--) {
			continue;
		}
	}
	if (false != false) {
		int mdtqu;
		for (mdtqu = 0; mdtqu > 0; mdtqu--) {
			continue;
		}
	}
	if (false == false) {
		int qortlv;
		for (qortlv = 88; qortlv > 0; qortlv--) {
			continue;
		}
	}
	return true;
}

bool jypqytg::mrbttxynfsibiajv(string hhnnvstugqdgd, string piheqwhgbspl, string ltjdorevjokjrs, double yrspn) {
	int icugiyr = 6480;
	double dkwadrpkcf = 9096;
	if (9096 == 9096) {
		int xatger;
		for (xatger = 91; xatger > 0; xatger--) {
			continue;
		}
	}
	return false;
}

bool jypqytg::ygydxtsitss(bool wzxkriirylis, int zdhlzwmlxlyny, double fbmfmhxqicurel, bool okkacheyhfi, bool jfdtmey, double igldn, int esibcbbvrz) {
	string qktnzfhic = "awgnuhqwvqzczmuvevxgvxrqdccmjysjlesxjfxruzcpyhligauoyfeeefxnozzvoyzrgxmxddexlkpvwyisxeitubrl";
	double zgpkazcutlxueyh = 10974;
	int mjvnle = 912;
	string gjcjr = "qyerfilzdebmduqppslaldymtxsmczewozvityolqxlhkzhyvrrsbxofopzdpkhgkfahk";
	if (string("qyerfilzdebmduqppslaldymtxsmczewozvityolqxlhkzhyvrrsbxofopzdpkhgkfahk") == string("qyerfilzdebmduqppslaldymtxsmczewozvityolqxlhkzhyvrrsbxofopzdpkhgkfahk")) {
		int axybtkxyo;
		for (axybtkxyo = 7; axybtkxyo > 0; axybtkxyo--) {
			continue;
		}
	}
	return true;
}

void jypqytg::lfwbtiwhrdqea(int xzgxr, string owdvplilucffws, bool bbckmkp, string mitqt) {
	int umwnfevzlhe = 1315;
	string ymcteblr = "nactu";
	string nverpynvbf = "nqlxzkhmdjtxrwgkzldavlnwjbhltvzkyjpqckrehromxpflhltmhndlqngpqfcueilbcftherkwvavagtuyenl";
	int kcuduzudgrpvqo = 1040;
	double dybumouvgtvl = 16410;
	if (string("nqlxzkhmdjtxrwgkzldavlnwjbhltvzkyjpqckrehromxpflhltmhndlqngpqfcueilbcftherkwvavagtuyenl") != string("nqlxzkhmdjtxrwgkzldavlnwjbhltvzkyjpqckrehromxpflhltmhndlqngpqfcueilbcftherkwvavagtuyenl")) {
		int nnculiepzu;
		for (nnculiepzu = 36; nnculiepzu > 0; nnculiepzu--) {
			continue;
		}
	}
	if (16410 != 16410) {
		int pxablh;
		for (pxablh = 55; pxablh > 0; pxablh--) {
			continue;
		}
	}

}

string jypqytg::zuqcnkoshrv(double zyhvuzp, bool bbjtmueurygpv, bool dxzpfdqwhxox, string svkjyx, string docmzqrswwv, double dyzsbuszmjzndq, string wyfxxrkfmfwz, double vnsiaflcv) {
	bool igiztrwzoudpdl = true;
	string mbexlol = "qsnxqmfcmgptydgwiemjjrjenuntubmfropgdpcannjmutb";
	string xhkzqbvy = "moryubugbhbaqlacskniurgesxwdfxrpkvotswmiqxykwaqbfzvytztexivfynifyllpjdqztasxwoiiy";
	if (string("moryubugbhbaqlacskniurgesxwdfxrpkvotswmiqxykwaqbfzvytztexivfynifyllpjdqztasxwoiiy") != string("moryubugbhbaqlacskniurgesxwdfxrpkvotswmiqxykwaqbfzvytztexivfynifyllpjdqztasxwoiiy")) {
		int xkahrf;
		for (xkahrf = 48; xkahrf > 0; xkahrf--) {
			continue;
		}
	}
	if (true == true) {
		int qqrbcbizi;
		for (qqrbcbizi = 59; qqrbcbizi > 0; qqrbcbizi--) {
			continue;
		}
	}
	return string("oyg");
}

jypqytg::jypqytg() {
	this->eduogkluuillyb(193, string("bpifrbqohsdopdthsqgslzcrxdxnznqwgrlljxmamyhrdsejddjjqfpplcenmw"), 18140, 7757, string("ymarbrucktqrrrfhmwkwtsgabrpu"), string("awozbqdwvbud"), string("vtdlqhhhgewkkymzlacfirbqxw"));
	this->whchhorfhtmecykmmc(31536, string("gurqklkzxrsbypscoqlsfvirrtmsebkkpfophcsuhfalfzfqgxbofmyszjbckydqrrlafjwke"), string("plyjruibarvdfqeovoqovrbwmusrbfwpbkadbjxttpyalpngfvxnxtwdmrfeuiuybibahjyiyrtsrdqzgt"), 61082, string("doqnydosgbaucahrzvbicamghavkizhupywfpldmicrkhoobakfqeo"), 19199, string("fmqucxcqqbrgfxbtpphrovwxsborufvbqoietwkldxmdai"), true);
	this->fclhmjwxslkgmzh(3223, string("mixkzbedwojuxxrdcqjufwydeibgjbcn"), 39439, string("yftvxtxkngsszmqfjllkgrfgidkocjqentgnsgqwsppzuvujeanzf"), 457, false, 13850, string("bbmkvckgrogqbmfcrcjxabxqpfvlgleudbzvxnssgyhvgqy"), string("kbejqkaxjdaupcelnonwfxmiaqaazniawabkzwiozfestkjxjvqnurkcsmm"), string("xojgwwszmclngsqhrauxutivdixqyywkdvmtckgonfrhnheiperei"));
	this->qtiyheasyixvbuebscyq(false, 244, 33305, string("ejqwaupjrhrfqhyjpokcvihspinuatipqzsbmuqxonqgtopazwnndmukrxzccqzgdawiqwlodcrvawnojzzkecawlqhtsdd"), 16638, true, 123, false);
	this->pbenkazngd(717);
	this->bgolpomtiqmtgsam(false);
	this->mrbttxynfsibiajv(string("vijqrut"), string("gxhkbcugxegpkefdzbexlvfkrtahyijnmlazwqedzyjv"), string("jgaekjtzcfsyogpokxulgibjlnfaotapqqcwumznamrkqqpmtywcnqcrzaouvfxuu"), 19132);
	this->ygydxtsitss(true, 1041, 17856, false, false, 1499, 3686);
	this->lfwbtiwhrdqea(49, string("blbglmpujigebqestawutqzabkmubkpqtnknawdmgrbyhswgdobzghyrivodlnrajdwgtmhirhifyp"), true, string("fiymnfpcvqhkwjyrhvfjamyykxfhrekfxztsqxsmrhgxadkm"));
	this->zuqcnkoshrv(79885, true, true, string("uzapoqjggyqgdqhayooamozypueugjsrmpiiwmkutjaizkccdfpmkvwgbkdoearbynyd"), string("sqtjevejgzdsyfnulehrxmvzremseomthffkylkfoiqezcpwwoxougojdz"), 36440, string("rqwgtmdqvxvtioeabfnrhzeddatcqdhkhcdpznlomhtrdoyajuoutuhmimfnpsjzaosqsssm"), 4271);
	this->lqvfusckntljzseiqnp(true, true);
	this->xvvkulumruqgfr();
}
