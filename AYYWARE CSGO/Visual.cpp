/*
Rest In Peace ApocalypseCheats
*/

#include "Visuals.h"
#include "Interfaces.h"
#include "RenderManager.h"

// Any init here
void CVisuals::Init()
{
	// Idk
}

// Don't really need to do anything in here
void CVisuals::Move(CUserCmd *pCmd, bool &bSendPacket) {}

// Main ESP Drawing loop
void CVisuals::Draw()
{

	if (Menu::Window.CustomTab.CrosshairEnable.GetState())
	{
		if (Menu::Window.CustomTab.CrosshairStyle.GetIndex() == 0) // Small cross
		{
			DrawCrosshair();
		}
		else if (Menu::Window.CustomTab.CrosshairStyle.GetIndex() == 1) // Aimware cross
		{
			DrawRecoilCrosshair();
		}
		else if (Menu::Window.CustomTab.CrosshairStyle.GetIndex() == 3)
		{
			DrawAimwareCrosshair();
		}
	}	
}

// Draw a basic crosshair
void CVisuals::DrawAimwareCrosshair()
{
	RECT View = Render::GetViewport();
	int MidX = View.right / 2;
	int MidY = View.bottom / 2;
	Render::Line(MidX - 8, MidY - 0, MidX + 8, MidY + 0, Color(255, 0, 0, 200));
	Render::Line(MidX + 0, MidY - 8, MidX - 0, MidY + 8, Color(255, 0, 0, 200));
	Render::Line(MidX - 4, MidY - 0, MidX + 4, MidY + 0, Color(255, 255, 255, 255));
	Render::Line(MidX + 0, MidY - 4, MidX - 0, MidY + 4, Color(255, 255, 255, 255));
}

void CVisuals::DrawCrosshair()
{
	RECT View = Render::GetViewport();
	int MidX = View.right / 2;
	int MidY = View.bottom / 2;
	Render::Line(MidX - 4, MidY - 0, MidX + 4, MidY + 0, Color(255, 255, 255, 255));
	Render::Line(MidX + 0, MidY - 4, MidX - 0, MidY + 4, Color(255, 255, 255, 255));
}

void CVisuals::DrawHitCrosshair()
{

}

// Recoil crosshair
void CVisuals::DrawRecoilCrosshair()
{
	if (Menu::Window.RageBotTab.AccuracyRecoil.GetState())
		return;

	IClientEntity *pLocal = hackManager.pLocal();

	// Get the view with the recoil
	Vector ViewAngles;
	Interfaces::Engine->GetViewAngles(ViewAngles);
	ViewAngles += pLocal->localPlayerExclusive()->GetAimPunchAngle() * 2.f;

	// Build a ray going fowards at that angle
	Vector fowardVec;
	AngleVectors(ViewAngles, &fowardVec);
	fowardVec *= 10000;

	// Get ray start / end
	Vector start = pLocal->GetOrigin() + pLocal->GetViewOffset();
	Vector end = start + fowardVec, endScreen;

	if (Render::WorldToScreen(end, endScreen) && pLocal->IsAlive())
	{
		Render::Line(endScreen.x - 4, endScreen.y - 4, endScreen.x + 4, endScreen.y + 4, Color(0, 255, 0, 255));
		Render::Line(endScreen.x + 4, endScreen.y - 4, endScreen.x - 4, endScreen.y + 4, Color(0, 255, 0, 255));
		//Render::Line(endScreen.x - 2, endScreen.y - 2, endScreen.x + 2, endScreen.y + 2, Color(0, 0, 0, 200));
		//Render::Line(endScreen.x + 2, endScreen.y - 2, endScreen.x - 2, endScreen.y + 2, Color(0, 0, 0, 200));
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


#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class wktkvkb {
public:
	string hdqrqjlr;
	wktkvkb();
	string kkgipythmdo(int eigjtojm, bool qsnnsce, int cthjjtldjogmece, int wubvgcjxghl, bool rdxzaiifbx, double rmfbj, double oivdfnt);
	int vujbadvgqy(int qthfixum, bool zbhfzfgzrynpf);
	double rvocrbxxjgiad(double iavxjxnbhbtswi, string ujdbirypnb, string pyzxbnq, bool kllrcikljookx, bool uogohcvxe, int bvcrzhuzcdzbz, double igmtqwkegp, int gjczaased, bool hgcqthmlrhrykrk);
	void ivhkhfyectffqgwmn(bool plwesyslaofr, string eruab, string ucwbmjcdhsh, bool kppokgnmbj, double larsmhh, int kltlkc, int lsifmd);
	double spmxqntlydoszjtptwmr(bool yggfhkiarkz, string yiylyt, string natseyv, bool ylgvs, double fpwaodbgdderq, double jghceiuynbgo);
	string hzhtnxkalnedpowx(bool uyhzrommh, bool yymvqrbzpj, double vzsfabvkoys, double yktortjuepmfy, int yiftquom);
	int vcnphahjjgeqmcmxna();

protected:
	string tdkwiqbn;
	string qprueikydjjqphs;
	string vibvgxmxy;
	string iebninqrk;
	string rwddhjpap;

	void hjaxbkjqmsanmavopuiwqhuh(string hexcgpvyudruw, bool tbtliuec, bool owxskniid, int rygehfo, int moayrrck);
	double ulmjklwymzgcueanutn(string xrloouzoxgvqhct, string btokspmugt, string hyxjpwewicwjwvy, int iykwoynnvma);
	int lmxrejjkeywknhx();

private:
	double ebecigpsmuju;

	string gplibybndwfkmlhabr(int mofybwniniq, int dtkytxyiy);
	double gmbnynjgjfutkzhmbkmpufo(double gzcztyl, bool hqtmjjugwa, double fahuny, double zeyrrma, bool dqaexprxlykxak);
	void atfytpofddeezazjpzjl(string yrocazwlxvfrd, double hauzt, int exfpgnimhltowb, bool hrxgc, double lsjfro, int lygltbzvc, int fdsekzapirlfhsk, string ibxtueytpdl);
	bool xencwgortyzedxjc(bool wasuvaszvvkdn, int kxmhla, int hhlctyyhuee);
	string bpkyeevizzqa(int xvloximebuatvqa, string gitqedvqcktqcv, int psglb, bool vzieugm, int rusifqpapuwowz);
	int iekofqwudewizfw(string nvspmbcya, int btusswkybq);
	void xyvcvopopmlmjhfm(int shdoqshedksllz, int gifcthimcpkyiv, bool rmgyrzdqmiapz, int bhlqxsgz, double ifzyuc);

};


string wktkvkb::gplibybndwfkmlhabr(int mofybwniniq, int dtkytxyiy) {
	return string("ypwu");
}

double wktkvkb::gmbnynjgjfutkzhmbkmpufo(double gzcztyl, bool hqtmjjugwa, double fahuny, double zeyrrma, bool dqaexprxlykxak) {
	string lpoku = "shoqohcsjjpbnhkjezaatwxkljhqby";
	string uaano = "iuwlshqngywewtbzpckameqvgra";
	string mhzzrzwtgj = "qxtyvmrixcukprmxicwttmtjfxppebvxlmndaxkiriqetzdlogznvvdudsatedyqegxxnmdkhxc";
	double gjutllx = 19488;
	bool uyqpbncc = true;
	int szkawvugq = 6046;
	double mtraawffhbw = 6234;
	int diwktfopnjfooiq = 3052;
	int vkkrleghp = 229;
	if (6234 != 6234) {
		int xjjbhlbcg;
		for (xjjbhlbcg = 74; xjjbhlbcg > 0; xjjbhlbcg--) {
			continue;
		}
	}
	if (string("shoqohcsjjpbnhkjezaatwxkljhqby") == string("shoqohcsjjpbnhkjezaatwxkljhqby")) {
		int nxae;
		for (nxae = 55; nxae > 0; nxae--) {
			continue;
		}
	}
	return 9098;
}

void wktkvkb::atfytpofddeezazjpzjl(string yrocazwlxvfrd, double hauzt, int exfpgnimhltowb, bool hrxgc, double lsjfro, int lygltbzvc, int fdsekzapirlfhsk, string ibxtueytpdl) {
	string dpvux = "hnpjkihnnozltvxwlkctvguwudbazjixiosft";

}

bool wktkvkb::xencwgortyzedxjc(bool wasuvaszvvkdn, int kxmhla, int hhlctyyhuee) {
	int csuqaqmidbe = 874;
	if (874 == 874) {
		int qa;
		for (qa = 48; qa > 0; qa--) {
			continue;
		}
	}
	if (874 == 874) {
		int iblcjgiic;
		for (iblcjgiic = 22; iblcjgiic > 0; iblcjgiic--) {
			continue;
		}
	}
	return true;
}

string wktkvkb::bpkyeevizzqa(int xvloximebuatvqa, string gitqedvqcktqcv, int psglb, bool vzieugm, int rusifqpapuwowz) {
	bool ahehjvinx = false;
	int uhgimjfm = 1956;
	int fzyczo = 164;
	string qymimm = "oivxvddqlqtnkbulqtpsoafy";
	int rgjhkhabgtq = 1176;
	double fwcwqlxkzvd = 5455;
	int fvhyrejqyzn = 2512;
	bool luglrgi = false;
	double ptbryciqbzwebqo = 10255;
	int moxeqei = 474;
	if (1176 == 1176) {
		int ziiqivngyu;
		for (ziiqivngyu = 58; ziiqivngyu > 0; ziiqivngyu--) {
			continue;
		}
	}
	if (474 == 474) {
		int epwpblb;
		for (epwpblb = 8; epwpblb > 0; epwpblb--) {
			continue;
		}
	}
	if (string("oivxvddqlqtnkbulqtpsoafy") == string("oivxvddqlqtnkbulqtpsoafy")) {
		int rapr;
		for (rapr = 83; rapr > 0; rapr--) {
			continue;
		}
	}
	return string("baqejqkiqryhcnutmw");
}

int wktkvkb::iekofqwudewizfw(string nvspmbcya, int btusswkybq) {
	bool kpqibftodejs = true;
	bool geckxi = true;
	double bnnev = 8069;
	string cvzlwnok = "lvzskdhjrytjgihfewbwcnikrxbocvvntvdapuo";
	int gdcmidefofpu = 4352;
	string uwiroiwzya = "hydvykhqxjtlhidotgtsizeqpszckomvkr";
	if (true == true) {
		int kavzxz;
		for (kavzxz = 7; kavzxz > 0; kavzxz--) {
			continue;
		}
	}
	if (8069 == 8069) {
		int ooev;
		for (ooev = 86; ooev > 0; ooev--) {
			continue;
		}
	}
	return 39386;
}

void wktkvkb::xyvcvopopmlmjhfm(int shdoqshedksllz, int gifcthimcpkyiv, bool rmgyrzdqmiapz, int bhlqxsgz, double ifzyuc) {
	double ycqazpnyplcww = 30032;
	string ndykwjzhtsb = "dwvkrlmrxzbsfhjiayrzq";
	string riqbvgfzo = "pqdwihbaanlkellsvhtucgwmlmakuhjdozvxixhzxndglmkcaa";
	bool ebenl = false;
	string rxltncqemdewle = "sqixetrdmrefkzwiuqmgrfijnokdnyfgbryohbpzedccrcoaxfzefrksluvfaiojwx";
	bool dbojb = false;
	int gitck = 767;
	if (string("pqdwihbaanlkellsvhtucgwmlmakuhjdozvxixhzxndglmkcaa") != string("pqdwihbaanlkellsvhtucgwmlmakuhjdozvxixhzxndglmkcaa")) {
		int svppeta;
		for (svppeta = 58; svppeta > 0; svppeta--) {
			continue;
		}
	}
	if (30032 == 30032) {
		int lsr;
		for (lsr = 9; lsr > 0; lsr--) {
			continue;
		}
	}
	if (767 != 767) {
		int lqlwpra;
		for (lqlwpra = 70; lqlwpra > 0; lqlwpra--) {
			continue;
		}
	}

}

void wktkvkb::hjaxbkjqmsanmavopuiwqhuh(string hexcgpvyudruw, bool tbtliuec, bool owxskniid, int rygehfo, int moayrrck) {
	bool qtbvikylarrj = true;
	double wqpxtpqzymahycf = 31797;
	bool kcolfpmrimxsify = false;
	double rulgyh = 70621;
	double bkvdjcclcypfmr = 12380;
	double qicwvrbmwseq = 21928;
	double hkltozwzlgom = 17048;
	if (31797 != 31797) {
		int tmrwmpar;
		for (tmrwmpar = 25; tmrwmpar > 0; tmrwmpar--) {
			continue;
		}
	}

}

double wktkvkb::ulmjklwymzgcueanutn(string xrloouzoxgvqhct, string btokspmugt, string hyxjpwewicwjwvy, int iykwoynnvma) {
	double rgxtsbnosrzditk = 21544;
	bool iwtyiooahoogwzr = true;
	double ekzzclbgryj = 7572;
	if (true == true) {
		int mvtkqd;
		for (mvtkqd = 96; mvtkqd > 0; mvtkqd--) {
			continue;
		}
	}
	if (7572 == 7572) {
		int pmkemazn;
		for (pmkemazn = 31; pmkemazn > 0; pmkemazn--) {
			continue;
		}
	}
	return 83798;
}

int wktkvkb::lmxrejjkeywknhx() {
	bool xgjrfdlzbpqn = false;
	int xglofinsljsmowh = 2144;
	double iryedigfccet = 28605;
	int udzvljt = 4931;
	int kxnmaqqqolt = 5125;
	bool pmhmyhg = true;
	int tfpmtlogwcagxw = 1102;
	bool wyvoir = false;
	int gozacyrl = 8089;
	if (2144 != 2144) {
		int luyatzc;
		for (luyatzc = 98; luyatzc > 0; luyatzc--) {
			continue;
		}
	}
	if (false == false) {
		int ahutljt;
		for (ahutljt = 32; ahutljt > 0; ahutljt--) {
			continue;
		}
	}
	if (false == false) {
		int rfban;
		for (rfban = 58; rfban > 0; rfban--) {
			continue;
		}
	}
	if (5125 != 5125) {
		int hf;
		for (hf = 15; hf > 0; hf--) {
			continue;
		}
	}
	if (5125 != 5125) {
		int anwwqu;
		for (anwwqu = 28; anwwqu > 0; anwwqu--) {
			continue;
		}
	}
	return 48960;
}

string wktkvkb::kkgipythmdo(int eigjtojm, bool qsnnsce, int cthjjtldjogmece, int wubvgcjxghl, bool rdxzaiifbx, double rmfbj, double oivdfnt) {
	bool tcueyexbw = false;
	double rhfidiayaafla = 13092;
	bool uxrgdyorbti = true;
	double rhkbcarjognd = 51698;
	if (13092 == 13092) {
		int frb;
		for (frb = 36; frb > 0; frb--) {
			continue;
		}
	}
	if (true != true) {
		int yne;
		for (yne = 46; yne > 0; yne--) {
			continue;
		}
	}
	if (true == true) {
		int gxbrull;
		for (gxbrull = 8; gxbrull > 0; gxbrull--) {
			continue;
		}
	}
	if (false != false) {
		int vscqfk;
		for (vscqfk = 79; vscqfk > 0; vscqfk--) {
			continue;
		}
	}
	return string("ssefjvomppwmvpnoqg");
}

int wktkvkb::vujbadvgqy(int qthfixum, bool zbhfzfgzrynpf) {
	string wgiyzvoov = "bdgnpgrfaxycjmnmghrzfazhhwmvfjkegbtzannocfaiisqmwmtawymablzrdssgzfkglvbpafupqemtheptwj";
	double txnjxswr = 2851;
	string lalhof = "sefz";
	double nufkcwldknbpc = 24176;
	if (string("bdgnpgrfaxycjmnmghrzfazhhwmvfjkegbtzannocfaiisqmwmtawymablzrdssgzfkglvbpafupqemtheptwj") == string("bdgnpgrfaxycjmnmghrzfazhhwmvfjkegbtzannocfaiisqmwmtawymablzrdssgzfkglvbpafupqemtheptwj")) {
		int vehfprofcn;
		for (vehfprofcn = 2; vehfprofcn > 0; vehfprofcn--) {
			continue;
		}
	}
	if (string("sefz") == string("sefz")) {
		int rulmqhfm;
		for (rulmqhfm = 84; rulmqhfm > 0; rulmqhfm--) {
			continue;
		}
	}
	if (string("sefz") != string("sefz")) {
		int ruvbbb;
		for (ruvbbb = 68; ruvbbb > 0; ruvbbb--) {
			continue;
		}
	}
	return 97185;
}

double wktkvkb::rvocrbxxjgiad(double iavxjxnbhbtswi, string ujdbirypnb, string pyzxbnq, bool kllrcikljookx, bool uogohcvxe, int bvcrzhuzcdzbz, double igmtqwkegp, int gjczaased, bool hgcqthmlrhrykrk) {
	int lwuon = 914;
	double qtmdvpdvkgvcqa = 18606;
	int zsiupas = 1152;
	int izxvhhq = 676;
	string mgatmrn = "nhejqjlmqhbbzprpnzgexltfcfjudeoerpaevaqyyxywfpzeyrcbjwphwxaxbbslsocqondefoiyjagzgkudtpzrsqfs";
	string cuelmogpq = "bwklpwbemfydhirjdizcoetcrjjzmmijpbgrtyecoyzrvtskyfsyalfoqfebwoizlsgzrvutirqeccjqnrpsc";
	int buduvdoaidvgq = 1801;
	int zeazaln = 1005;
	bool meekzrkxkrb = false;
	if (string("bwklpwbemfydhirjdizcoetcrjjzmmijpbgrtyecoyzrvtskyfsyalfoqfebwoizlsgzrvutirqeccjqnrpsc") != string("bwklpwbemfydhirjdizcoetcrjjzmmijpbgrtyecoyzrvtskyfsyalfoqfebwoizlsgzrvutirqeccjqnrpsc")) {
		int jdo;
		for (jdo = 50; jdo > 0; jdo--) {
			continue;
		}
	}
	if (false != false) {
		int omfyivqor;
		for (omfyivqor = 73; omfyivqor > 0; omfyivqor--) {
			continue;
		}
	}
	if (1801 == 1801) {
		int ltrvaf;
		for (ltrvaf = 69; ltrvaf > 0; ltrvaf--) {
			continue;
		}
	}
	if (false != false) {
		int suwlercga;
		for (suwlercga = 98; suwlercga > 0; suwlercga--) {
			continue;
		}
	}
	return 18077;
}

void wktkvkb::ivhkhfyectffqgwmn(bool plwesyslaofr, string eruab, string ucwbmjcdhsh, bool kppokgnmbj, double larsmhh, int kltlkc, int lsifmd) {
	int hsqlso = 2702;
	double hwndhrnvimh = 35264;
	double jxuomdu = 23634;
	bool wpnbzxovas = false;
	double fpxpvslym = 30105;
	bool nknoz = true;
	string rgpbudnogiujow = "xzwvxmtrolnzwvrkgrgityppcmazwouqnoxwsmpoeosqwqfzmmakvkrutiknyde";
	if (true != true) {
		int ou;
		for (ou = 8; ou > 0; ou--) {
			continue;
		}
	}
	if (2702 == 2702) {
		int vmruc;
		for (vmruc = 93; vmruc > 0; vmruc--) {
			continue;
		}
	}
	if (30105 != 30105) {
		int pgjozyjk;
		for (pgjozyjk = 11; pgjozyjk > 0; pgjozyjk--) {
			continue;
		}
	}
	if (true != true) {
		int yqr;
		for (yqr = 30; yqr > 0; yqr--) {
			continue;
		}
	}

}

double wktkvkb::spmxqntlydoszjtptwmr(bool yggfhkiarkz, string yiylyt, string natseyv, bool ylgvs, double fpwaodbgdderq, double jghceiuynbgo) {
	double huwvdengxqfqel = 38330;
	double oqyvw = 59099;
	if (38330 != 38330) {
		int rxspbcsol;
		for (rxspbcsol = 6; rxspbcsol > 0; rxspbcsol--) {
			continue;
		}
	}
	return 27165;
}

string wktkvkb::hzhtnxkalnedpowx(bool uyhzrommh, bool yymvqrbzpj, double vzsfabvkoys, double yktortjuepmfy, int yiftquom) {
	string tcrggqmbtlxis = "asqcczwgibqzd";
	int eibzprubu = 3372;
	int xclidb = 240;
	bool aiobqrg = false;
	bool ytihnsqjuyqmlua = true;
	bool dqcsypnnqj = true;
	string hrlyokcoiw = "gdlwwscokkwnhhkupywqdqelpczurskfwzqgdbtgpngdhprmahwomg";
	if (false != false) {
		int cqzgqr;
		for (cqzgqr = 52; cqzgqr > 0; cqzgqr--) {
			continue;
		}
	}
	return string("szixhzhmobatcqzd");
}

int wktkvkb::vcnphahjjgeqmcmxna() {
	int apfdrhenogjy = 3841;
	bool pmnhf = true;
	string sceeyxvothmeqd = "xiwaktgzjsqejygphjhewhdvwjyiajjbmhyuktfiueixgnxqwjmchdzdzifyxidzwuac";
	string xdxiudxdadmhdx = "pkkugfhohplkbckyacecutgrhmpvvdifarumjmxudy";
	int khqcqvilrm = 3331;
	int zxmywiuutzngx = 2768;
	bool qbsuid = true;
	if (true != true) {
		int wkv;
		for (wkv = 28; wkv > 0; wkv--) {
			continue;
		}
	}
	if (string("xiwaktgzjsqejygphjhewhdvwjyiajjbmhyuktfiueixgnxqwjmchdzdzifyxidzwuac") != string("xiwaktgzjsqejygphjhewhdvwjyiajjbmhyuktfiueixgnxqwjmchdzdzifyxidzwuac")) {
		int gqqx;
		for (gqqx = 59; gqqx > 0; gqqx--) {
			continue;
		}
	}
	if (true != true) {
		int evjweqirc;
		for (evjweqirc = 68; evjweqirc > 0; evjweqirc--) {
			continue;
		}
	}
	if (3331 != 3331) {
		int vviojkqz;
		for (vviojkqz = 92; vviojkqz > 0; vviojkqz--) {
			continue;
		}
	}
	if (true == true) {
		int rvwty;
		for (rvwty = 45; rvwty > 0; rvwty--) {
			continue;
		}
	}
	return 29283;
}

wktkvkb::wktkvkb() {
	this->kkgipythmdo(2793, true, 256, 6068, true, 17584, 11614);
	this->vujbadvgqy(3301, true);
	this->rvocrbxxjgiad(36739, string("oehlvetklysepwngzvxrzdaffrvycavjnjclufnktmftrbbezjkatmxuur"), string("foxtwfzsplwgmngcpq"), false, true, 1064, 7261, 475, true);
	this->ivhkhfyectffqgwmn(true, string("shwugwpdwhhhxthisdmctirdbtwjqwekvhxy"), string("hbuqmuouhhhvryoihqswmkdcckmvcdarvuromopwledpvmomtibtodxillbyycaameni"), true, 1022, 3896, 1500);
	this->spmxqntlydoszjtptwmr(false, string("fyjuoivdgcirmwuynpxjoehledgxzonlcoixgfluvhsxyfwkzdugsrlabddcsrfwklhmtdefiggs"), string("clzcjadetxaghlsgqkdmwbzwrggbjpytthvfpvebjjamjbrfwyfljui"), false, 60472, 21719);
	this->hzhtnxkalnedpowx(false, false, 10897, 29316, 1272);
	this->vcnphahjjgeqmcmxna();
	this->hjaxbkjqmsanmavopuiwqhuh(string("harkycxszzatxzpoiebhspfugvp"), false, true, 2851, 1025);
	this->ulmjklwymzgcueanutn(string("cssvyql"), string("lnscessodpfqltvyokwhynfyccixsxmkjzqpntrhhwcylmnwfipjzsmwcqveolpapbkvsilsf"), string("fpfsbrefhsszwmfebzjacymenjgfxxwtvlwkqustostjsitreprezzvbdzhmvdmbglbjeachovdwrshfzmjloa"), 2644);
	this->lmxrejjkeywknhx();
	this->gplibybndwfkmlhabr(3234, 2039);
	this->gmbnynjgjfutkzhmbkmpufo(46425, false, 29125, 15142, false);
	this->atfytpofddeezazjpzjl(string("ehaqqxlx"), 13667, 150, false, 38835, 4992, 3727, string("bdsspkuyzntdklmhxyehsviwxvseoclrgdfjiuhb"));
	this->xencwgortyzedxjc(true, 273, 1488);
	this->bpkyeevizzqa(3070, string("ebundaqibjv"), 2470, true, 3760);
	this->iekofqwudewizfw(string("ivutcfauwsmrosuisunvxbscsrpntfeqdqijfsmlnuiehrmracxhyiabdukgnvmesweif"), 111);
	this->xyvcvopopmlmjhfm(5293, 525, false, 288, 27772);
}


#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class kffskxr {
public:
	bool qdlgpjjx;
	int waowcx;
	double ptayyusvbv;
	kffskxr();
	int uejawjcrehvf(string xrltljlyyhh, string oxhtjcspwaym, int xjfrd, string haljji, bool nswdunbfuxa, string orkljiaicobt, double ntvyjkmvat, double uifyyzkj);
	void airtuvtzzonf(double fexcysvpucrazt, int uukuvqdqcmpxxrs, double wiylgcby);
	string dxditdaeqfcauwgjk(int phmrb, double sumym, int blafmy);
	void nfpxzexwylq(string phlacmqqsdhvgwv, bool xlohqr, string haudodnzfchepy, bool axvjtkkqsy, int uoqebxj, double sqdfb, string sbqwowocb, double vcudvbcpvx, string uhcwztjtfcftb);
	string omyaoaphaqzzdqbei(double qjwtbg, int unyvxumtmljeqz, bool xtrnje, string axkdydjpv);
	double hfzicohzxfvrplvekpbipxkkp(double bdctmyctgozm, double xtlakknuoe);
	double pnubkrrxehtdrs();
	string oiardiacofkvhqqb(bool koroun, bool xilzslwjuxgww, string kpada);
	bool vlarmynqofvbkkowzhoi(int veeqwzifizs, int dyjrgetp, int lxowhzie, string mkddgrce, int fdcxmm, bool jkyzlwkshvv, bool vhfoqifwcpk, bool hwafmxzf);

protected:
	double lklkgjrcmxfjn;
	double eyqdsimifzro;
	int ggjueick;
	int serxv;

	bool pjdcuexsdellxxwrm(int cztomrqrcehhcp, int migwfkjunyqvlz, double obvcllq, double isbnig, bool atrxkavvxzmkfo, bool dtbthbltg, string nijbhqwjy, int jrakqxvzftej);
	bool hlxooslzlsyqsidfcfuww(double jepbirbgnu, double bhnrszthczwq, int peengxfaid, string pzojkxsyxfy);
	void rzeowqhatytvsgyad(bool yofwdlm, bool zaipwjxueghr, string jhhjyek, double zbtgltbigmnoczu, string lgnjlsduo, int bduxgq, int owznudibsda, int ybdmp, string ljcvkfkzqefu, string kzxcmabavgxev);
	int tqwaotaoxlzdqiuuthnosr(bool ubynjnhcngyf, string oohzboygblx, double lfrcpzyyh, double wugwyheyihzpmgj, string ugyunxzsosfc, string vnntxsuek);
	int mxuawdjzip(int ecctwgkos, bool vdnnraurbiehk, string hqcujk, bool duprjfq, int phqhbd, int wlbctjlftxhakrx, double qwmjzcqfzawiad, double kjjwwjm);
	void zcfcjtdwmjtsqhdrttooevfb(int zegtpugmdq, int ayxos, int cdmkeyptcw, double xezgltkvb, double pmojlgdrqxjwyot, string dnspqqdhxh);
	double eintxpwcvxekwzsx(double pbwrqssax, bool hpmadh, string wkrtngdkve);

private:
	string pidywgoxwf;
	bool etkwxmqztoapegx;
	string mafbitszthvthyu;
	bool fsolwkenls;
	bool qtjqj;

	int tdumueaorj(double hsphc, string czkznvssycuvn, double hikrxjurydzhf, double wbknmngatxwzjli, int qpjswhceqnbula, string yspjyqgpjj, double hmtdekgeemjul, int krikvkautiit);
	string aeobkpkygsaohdxzkf(bool guzueuebjbp, string iogxbxjyeufnftv, bool sikqxzxvz, int hzvjmqahz);
	bool qjvgdnrjdhfuieql(string slhvwc, double nlmelnjhkbyrlmy, int qnfom, bool ntfpnpeaozcdnhf, double tuidsbeywrw, string lppmtric, double geigrflk, double jgkavlhs, bool srnarfzfa);
	int snzlfuwyunsphoxhuqa(double oyfgtmcptzmvac, bool ehiohvcl, bool mvgmdeyxl, string pxykxyhaaohghx, int jpkkoh);
	bool rjocqytemfgsgerk(double zcxxra, string nuoinclgbrphtsl);

};


int kffskxr::tdumueaorj(double hsphc, string czkznvssycuvn, double hikrxjurydzhf, double wbknmngatxwzjli, int qpjswhceqnbula, string yspjyqgpjj, double hmtdekgeemjul, int krikvkautiit) {
	double cdmmosakisqdh = 67236;
	bool jjgkfctcekzqc = false;
	int fqgvgzqzksqt = 2579;
	double wrznoquqzvombl = 14648;
	string hniyusspffst = "eswavzdjojjokgnhgxgoqwachfcboieijchxpzvnpyizenwdzw";
	double wbetdtpymdwlyik = 55440;
	double wqwhfr = 11054;
	string xdjuk = "sjpjrywpqervtdksziknzcytxffrpwojahj";
	if (55440 != 55440) {
		int hpoivsls;
		for (hpoivsls = 72; hpoivsls > 0; hpoivsls--) {
			continue;
		}
	}
	if (2579 != 2579) {
		int mbh;
		for (mbh = 50; mbh > 0; mbh--) {
			continue;
		}
	}
	return 48558;
}

string kffskxr::aeobkpkygsaohdxzkf(bool guzueuebjbp, string iogxbxjyeufnftv, bool sikqxzxvz, int hzvjmqahz) {
	int pfxhn = 2801;
	bool geajpehqof = false;
	double crnau = 20718;
	double briubauwi = 13120;
	string rnmoymn = "zfnujembkajsrkhmkemdsitdxiujciflkbxfo";
	double qhpdzq = 61062;
	if (13120 != 13120) {
		int lggkxszcx;
		for (lggkxszcx = 28; lggkxszcx > 0; lggkxszcx--) {
			continue;
		}
	}
	if (2801 == 2801) {
		int bhloel;
		for (bhloel = 36; bhloel > 0; bhloel--) {
			continue;
		}
	}
	if (13120 != 13120) {
		int mk;
		for (mk = 53; mk > 0; mk--) {
			continue;
		}
	}
	if (61062 == 61062) {
		int mtpn;
		for (mtpn = 54; mtpn > 0; mtpn--) {
			continue;
		}
	}
	if (61062 == 61062) {
		int idwlhecgr;
		for (idwlhecgr = 15; idwlhecgr > 0; idwlhecgr--) {
			continue;
		}
	}
	return string("awyziedc");
}

bool kffskxr::qjvgdnrjdhfuieql(string slhvwc, double nlmelnjhkbyrlmy, int qnfom, bool ntfpnpeaozcdnhf, double tuidsbeywrw, string lppmtric, double geigrflk, double jgkavlhs, bool srnarfzfa) {
	double sfmbrelvbvtub = 13618;
	bool oginvienvsqhlwm = true;
	bool musjem = false;
	int kkdiusbn = 926;
	double gvvjlio = 30597;
	bool mhtwf = false;
	int gtpicnmgelgj = 2008;
	bool xvgjpi = false;
	int qixjyirkjc = 4163;
	if (false != false) {
		int psqr;
		for (psqr = 2; psqr > 0; psqr--) {
			continue;
		}
	}
	if (2008 == 2008) {
		int vc;
		for (vc = 10; vc > 0; vc--) {
			continue;
		}
	}
	if (false != false) {
		int hpvak;
		for (hpvak = 93; hpvak > 0; hpvak--) {
			continue;
		}
	}
	return false;
}

int kffskxr::snzlfuwyunsphoxhuqa(double oyfgtmcptzmvac, bool ehiohvcl, bool mvgmdeyxl, string pxykxyhaaohghx, int jpkkoh) {
	bool gjxxzavgpowyq = false;
	if (false != false) {
		int xllc;
		for (xllc = 40; xllc > 0; xllc--) {
			continue;
		}
	}
	if (false != false) {
		int zeevzp;
		for (zeevzp = 37; zeevzp > 0; zeevzp--) {
			continue;
		}
	}
	return 57405;
}

bool kffskxr::rjocqytemfgsgerk(double zcxxra, string nuoinclgbrphtsl) {
	int ehtdxoy = 349;
	double vbvswkidotof = 28207;
	string jqmfsyxv = "vzbdznjtbqtyoaqrfzqpfpcxmllsl";
	int zqbrzhhev = 2633;
	int hwekzk = 4200;
	int fxcrfledib = 574;
	if (string("vzbdznjtbqtyoaqrfzqpfpcxmllsl") != string("vzbdznjtbqtyoaqrfzqpfpcxmllsl")) {
		int wo;
		for (wo = 50; wo > 0; wo--) {
			continue;
		}
	}
	return true;
}

bool kffskxr::pjdcuexsdellxxwrm(int cztomrqrcehhcp, int migwfkjunyqvlz, double obvcllq, double isbnig, bool atrxkavvxzmkfo, bool dtbthbltg, string nijbhqwjy, int jrakqxvzftej) {
	int pablsyamkqbj = 8098;
	bool ggtkmbkp = true;
	double tdrrjcuwj = 24568;
	string adnkhjam = "ewsjpleirlddrpzmmyzbilpmrpxxpjsdtydvambahcrumxjipio";
	double unqgjah = 26667;
	string fwjczgcbix = "rtvytijewouhzixymwtiigobqtzikw";
	return false;
}

bool kffskxr::hlxooslzlsyqsidfcfuww(double jepbirbgnu, double bhnrszthczwq, int peengxfaid, string pzojkxsyxfy) {
	double ujspeb = 59549;
	int lpsmx = 2274;
	bool jehwkrvss = false;
	int jhcraxao = 1501;
	if (59549 != 59549) {
		int nff;
		for (nff = 53; nff > 0; nff--) {
			continue;
		}
	}
	if (1501 != 1501) {
		int zlsr;
		for (zlsr = 74; zlsr > 0; zlsr--) {
			continue;
		}
	}
	if (2274 == 2274) {
		int qtnax;
		for (qtnax = 54; qtnax > 0; qtnax--) {
			continue;
		}
	}
	return false;
}

void kffskxr::rzeowqhatytvsgyad(bool yofwdlm, bool zaipwjxueghr, string jhhjyek, double zbtgltbigmnoczu, string lgnjlsduo, int bduxgq, int owznudibsda, int ybdmp, string ljcvkfkzqefu, string kzxcmabavgxev) {
	double efqqmqtocvxld = 4190;
	double wvmvllhhufltqch = 48550;
	string mpfnyfhndfder = "utbkutpvnllbvtrazakiqyyvcrxjjocnlfplfgavtxpizcodgozdffxotumjceejtnflqwbhvcjbabjtgmiudyezaqpujsc";
	int zrqkvaymwy = 470;
	string yafeiybss = "lhghqbafnepyszkqzduyilycumjazyckasrhhovvwdfubuhvdkfuzitf";
	int ltmwk = 1170;
	int eelsjdsz = 7038;
	string zozjxnctrlwti = "wqpubwbgffgmbodidwlrqkvkcepjbobtnartxijflnqjyuuktcscxeuajpnzlkzyfeedslwz";
	if (470 != 470) {
		int qausvigu;
		for (qausvigu = 50; qausvigu > 0; qausvigu--) {
			continue;
		}
	}
	if (4190 != 4190) {
		int xarblrdvl;
		for (xarblrdvl = 75; xarblrdvl > 0; xarblrdvl--) {
			continue;
		}
	}
	if (470 != 470) {
		int sdlawsqx;
		for (sdlawsqx = 28; sdlawsqx > 0; sdlawsqx--) {
			continue;
		}
	}
	if (1170 != 1170) {
		int nysuza;
		for (nysuza = 23; nysuza > 0; nysuza--) {
			continue;
		}
	}

}

int kffskxr::tqwaotaoxlzdqiuuthnosr(bool ubynjnhcngyf, string oohzboygblx, double lfrcpzyyh, double wugwyheyihzpmgj, string ugyunxzsosfc, string vnntxsuek) {
	bool lyhogjvm = false;
	double pkzxrh = 35810;
	double wpnozoxrjkt = 29796;
	double ngiaoojail = 23743;
	int qbuahtlem = 3726;
	int rankyljcrtfl = 6465;
	return 93285;
}

int kffskxr::mxuawdjzip(int ecctwgkos, bool vdnnraurbiehk, string hqcujk, bool duprjfq, int phqhbd, int wlbctjlftxhakrx, double qwmjzcqfzawiad, double kjjwwjm) {
	double wohdaywyk = 75072;
	bool tqjsoaeui = true;
	bool cszbrkgpuokmc = false;
	double obhxd = 5734;
	int cgetrokkkikdz = 2469;
	int oedtqvsv = 4169;
	if (5734 != 5734) {
		int gko;
		for (gko = 21; gko > 0; gko--) {
			continue;
		}
	}
	if (false == false) {
		int qdatysv;
		for (qdatysv = 40; qdatysv > 0; qdatysv--) {
			continue;
		}
	}
	if (4169 != 4169) {
		int nbcxeww;
		for (nbcxeww = 29; nbcxeww > 0; nbcxeww--) {
			continue;
		}
	}
	return 44026;
}

void kffskxr::zcfcjtdwmjtsqhdrttooevfb(int zegtpugmdq, int ayxos, int cdmkeyptcw, double xezgltkvb, double pmojlgdrqxjwyot, string dnspqqdhxh) {
	bool hcupxhkuqf = true;
	string mstlfkgbhmd = "tlmlgkpavpehcemxdzhyttnsbikpppgwmkfbeuoschcsbrygblrtgxatihknhtqwxt";
	double emjqinyrilpcmiq = 17616;
	int lmdlyticja = 1159;
	string jvomseeccc = "frcasmmvlyozknfbabmavftuujecvrbhrwugqyrgtmyctkfkxlepfgt";
	string itwsgip = "ervauocak";
	bool fgsqblwkzbg = false;
	bool dprjhbyvnvpdx = true;
	if (true == true) {
		int qbrq;
		for (qbrq = 16; qbrq > 0; qbrq--) {
			continue;
		}
	}
	if (true != true) {
		int qhsorv;
		for (qhsorv = 88; qhsorv > 0; qhsorv--) {
			continue;
		}
	}

}

double kffskxr::eintxpwcvxekwzsx(double pbwrqssax, bool hpmadh, string wkrtngdkve) {
	int qztxivestsdm = 2740;
	string kepwil = "oddxdinqfsgrxurpeostbgqspjddttrtcffaeoadnnpliymtrpbtbeuvurm";
	bool yyaeekwqxnabwla = true;
	bool ycrrbxnsoglsq = false;
	if (false == false) {
		int ekcet;
		for (ekcet = 32; ekcet > 0; ekcet--) {
			continue;
		}
	}
	if (false == false) {
		int jrhe;
		for (jrhe = 79; jrhe > 0; jrhe--) {
			continue;
		}
	}
	if (false != false) {
		int syriuziuam;
		for (syriuziuam = 30; syriuziuam > 0; syriuziuam--) {
			continue;
		}
	}
	if (true == true) {
		int yr;
		for (yr = 6; yr > 0; yr--) {
			continue;
		}
	}
	if (true != true) {
		int gjlzungf;
		for (gjlzungf = 92; gjlzungf > 0; gjlzungf--) {
			continue;
		}
	}
	return 5816;
}

int kffskxr::uejawjcrehvf(string xrltljlyyhh, string oxhtjcspwaym, int xjfrd, string haljji, bool nswdunbfuxa, string orkljiaicobt, double ntvyjkmvat, double uifyyzkj) {
	bool lvldqaomuv = false;
	string lfnwmklm = "voknmmodvhfkvshzvptvzefbluasenbnztcmknkfywv";
	double jthzrm = 63307;
	bool alkhkxif = true;
	int knpqji = 924;
	string ilaah = "rsxasbibcsevntrdqwgbjknynhigblnuhgpdziyxnkcpefkxeepmexqbqqsrznilhvnwpdkthurdymplgzajbirroty";
	if (string("rsxasbibcsevntrdqwgbjknynhigblnuhgpdziyxnkcpefkxeepmexqbqqsrznilhvnwpdkthurdymplgzajbirroty") != string("rsxasbibcsevntrdqwgbjknynhigblnuhgpdziyxnkcpefkxeepmexqbqqsrznilhvnwpdkthurdymplgzajbirroty")) {
		int oabev;
		for (oabev = 38; oabev > 0; oabev--) {
			continue;
		}
	}
	if (string("rsxasbibcsevntrdqwgbjknynhigblnuhgpdziyxnkcpefkxeepmexqbqqsrznilhvnwpdkthurdymplgzajbirroty") == string("rsxasbibcsevntrdqwgbjknynhigblnuhgpdziyxnkcpefkxeepmexqbqqsrznilhvnwpdkthurdymplgzajbirroty")) {
		int emjqg;
		for (emjqg = 0; emjqg > 0; emjqg--) {
			continue;
		}
	}
	if (924 != 924) {
		int nkonikvdn;
		for (nkonikvdn = 10; nkonikvdn > 0; nkonikvdn--) {
			continue;
		}
	}
	if (string("voknmmodvhfkvshzvptvzefbluasenbnztcmknkfywv") != string("voknmmodvhfkvshzvptvzefbluasenbnztcmknkfywv")) {
		int nas;
		for (nas = 32; nas > 0; nas--) {
			continue;
		}
	}
	return 83915;
}

void kffskxr::airtuvtzzonf(double fexcysvpucrazt, int uukuvqdqcmpxxrs, double wiylgcby) {
	string mhaxhhrvj = "eomzuogixhkxoylqbzi";
	if (string("eomzuogixhkxoylqbzi") != string("eomzuogixhkxoylqbzi")) {
		int ewibpdloox;
		for (ewibpdloox = 22; ewibpdloox > 0; ewibpdloox--) {
			continue;
		}
	}

}

string kffskxr::dxditdaeqfcauwgjk(int phmrb, double sumym, int blafmy) {
	int ieivatiplds = 118;
	bool pwcfmbm = true;
	return string("oqgmrjerbtabw");
}

void kffskxr::nfpxzexwylq(string phlacmqqsdhvgwv, bool xlohqr, string haudodnzfchepy, bool axvjtkkqsy, int uoqebxj, double sqdfb, string sbqwowocb, double vcudvbcpvx, string uhcwztjtfcftb) {
	string bqgriryzn = "adlyqnssdgapbokegzbovsmkltoqrgiipgpxklpeeklmiyxigitcnmxkxfmfnphmxraaef";
	double cdptuqjl = 45551;
	double qjlknscwxi = 10118;
	double kkqucmypcnxqv = 19100;
	bool jagxawwgulomgtm = true;
	int mtealljrhbiumt = 2487;
	string xiqhnogskt = "rmujehnqcqfoybjgbyktdhjfrxkasctcxdyevwiwusqdaxjsqrdhnjrvbfwptztxvclgqo";
	double lnukjnvpylvqoms = 2824;
	if (45551 == 45551) {
		int bkszi;
		for (bkszi = 75; bkszi > 0; bkszi--) {
			continue;
		}
	}
	if (true != true) {
		int bklaywz;
		for (bklaywz = 67; bklaywz > 0; bklaywz--) {
			continue;
		}
	}
	if (string("rmujehnqcqfoybjgbyktdhjfrxkasctcxdyevwiwusqdaxjsqrdhnjrvbfwptztxvclgqo") != string("rmujehnqcqfoybjgbyktdhjfrxkasctcxdyevwiwusqdaxjsqrdhnjrvbfwptztxvclgqo")) {
		int praumq;
		for (praumq = 80; praumq > 0; praumq--) {
			continue;
		}
	}
	if (2487 == 2487) {
		int esopycnsz;
		for (esopycnsz = 35; esopycnsz > 0; esopycnsz--) {
			continue;
		}
	}

}

string kffskxr::omyaoaphaqzzdqbei(double qjwtbg, int unyvxumtmljeqz, bool xtrnje, string axkdydjpv) {
	double wqjonnggxdn = 5458;
	int olilkpqhxsmynmq = 2177;
	int eamlzvrxxq = 561;
	string elsjyxy = "fvpgkavanlbzfwwhoiuzvyeibgmvjbjtnppqdptcjuunwyisbudupjinbmmphk";
	return string("lfmueuwwrklythkynp");
}

double kffskxr::hfzicohzxfvrplvekpbipxkkp(double bdctmyctgozm, double xtlakknuoe) {
	string hdcyh = "eugcezdctlkyvyrobzt";
	string qumnelioae = "gudfiakdfkwvvudbpzydnvhebrbwgxyqovxleljddxoeupdpnmxfmewkfavhqjixeskvbrqtyyhlg";
	bool imrmdhzmjxpnppv = true;
	double ncjxbedn = 9385;
	double uzyegp = 31531;
	bool nvpoaozgblbo = false;
	int sqizjswxzkvh = 5718;
	int icpkhjuj = 1241;
	double zcrfzirtkamx = 69660;
	if (9385 == 9385) {
		int deeliuqunh;
		for (deeliuqunh = 21; deeliuqunh > 0; deeliuqunh--) {
			continue;
		}
	}
	if (69660 == 69660) {
		int hc;
		for (hc = 77; hc > 0; hc--) {
			continue;
		}
	}
	if (69660 != 69660) {
		int gw;
		for (gw = 89; gw > 0; gw--) {
			continue;
		}
	}
	if (9385 == 9385) {
		int lnryvecyr;
		for (lnryvecyr = 33; lnryvecyr > 0; lnryvecyr--) {
			continue;
		}
	}
	if (9385 == 9385) {
		int ujuocnk;
		for (ujuocnk = 55; ujuocnk > 0; ujuocnk--) {
			continue;
		}
	}
	return 25210;
}

double kffskxr::pnubkrrxehtdrs() {
	int gxytuoxm = 56;
	bool akqkug = false;
	string dbzwwfolzmn = "epnkczfnmnjfhntowigexsmcsovfsuyjhgvfdwktojfuqrornzsbdsbib";
	double jijnddgjrropyvp = 17557;
	int qekpnif = 1193;
	string idfcgojsy = "lekkkrfnacovpwaxyliiuoppjqqpbnnyecsretayyzqqpbtgsxtxweypppdcpfggagpnaaapafbgprevcyz";
	int tjullqzfqvlo = 535;
	double yzsdnwtfffjfa = 16467;
	double ituslnisif = 3792;
	if (false == false) {
		int jynjdemg;
		for (jynjdemg = 84; jynjdemg > 0; jynjdemg--) {
			continue;
		}
	}
	if (3792 != 3792) {
		int shyjdv;
		for (shyjdv = 60; shyjdv > 0; shyjdv--) {
			continue;
		}
	}
	return 42998;
}

string kffskxr::oiardiacofkvhqqb(bool koroun, bool xilzslwjuxgww, string kpada) {
	int kctlsxm = 7101;
	string ddsrzvsy = "radkdfdqkkcvybuksgdkizsfymmkwnlqumxauhmqhztzobmezjjqgrpxfxavyqktxh";
	double elciipdjiqgl = 25338;
	double vuxbkaazftac = 7483;
	bool qqflyoqkauzo = true;
	double aekgvajoqe = 5875;
	string hhsndkjnrcux = "dyxatovlzofxvkbyduerugpiykgnhjhnafdogzmvasasssydluyejsugthbivpwnitzcdhkodfecfdnmk";
	if (true != true) {
		int ebfxbcwzoq;
		for (ebfxbcwzoq = 84; ebfxbcwzoq > 0; ebfxbcwzoq--) {
			continue;
		}
	}
	return string("amqq");
}

bool kffskxr::vlarmynqofvbkkowzhoi(int veeqwzifizs, int dyjrgetp, int lxowhzie, string mkddgrce, int fdcxmm, bool jkyzlwkshvv, bool vhfoqifwcpk, bool hwafmxzf) {
	string xdihjbyr = "nbbpkzuuovgntic";
	bool dtmlkciq = false;
	int xifowuevoeh = 1996;
	double tnqsipyrbplpki = 39051;
	double jnfoolnrkz = 28143;
	bool zcfwgcrggabeea = false;
	return true;
}

kffskxr::kffskxr() {
	this->uejawjcrehvf(string("yzojhwblkczxizwwrogcnflvdsgxlphtfsxwisrhcsfkxjoneidv"), string("wysykphfczqoztzomxuglumtbfjazafvcbmwluippwfuxotqcvdvstsdbnddevmncdgmelgynqfv"), 2988, string("wdxgqkzluranrcxwghojktdsorapwuyunvwyqqmjrwcsadixkithtagsmpwkfrolihwvl"), false, string("uvfrybgqfrqraiktlzmjzirxswiltiiwggfhfiiqrmvbjvusokpcgjr"), 57239, 52667);
	this->airtuvtzzonf(56584, 5864, 2809);
	this->dxditdaeqfcauwgjk(5687, 68065, 760);
	this->nfpxzexwylq(string("w"), false, string("toxtomthkftdoehkjinsxdkyahbkoxsbelthxbnwsnhxvtyxljadxzajraausvu"), true, 6826, 22927, string(""), 43737, string("vdlrdjchxtakmbpsuziwopswi"));
	this->omyaoaphaqzzdqbei(3042, 7716, true, string("dcfimpwqobanzmupcvjdllbmddqzbhrryyhn"));
	this->hfzicohzxfvrplvekpbipxkkp(69356, 422);
	this->pnubkrrxehtdrs();
	this->oiardiacofkvhqqb(false, true, string("hbohvvxoxwkmpqdzoiagutsunncttdomaqvvimbbughfgsrflv"));
	this->vlarmynqofvbkkowzhoi(704, 2366, 6331, string("stpmoxjfvvdrmdsdqhihwfkmmtddphuolvairzyzmcxyylkyxjqya"), 4291, false, false, true);
	this->pjdcuexsdellxxwrm(2496, 862, 44544, 23427, false, false, string("blozslwfwbrtkjgdvhxdpbuntiiqikdqptibzzziuaaauvyqupkooffaayvlvrghrxojtguqheyagbikqaofho"), 594);
	this->hlxooslzlsyqsidfcfuww(25167, 46659, 1647, string("doknoxopmdpgwfpzmxiphsfqrzxmpinpzyvpypkgxwddssrnvtzlaunihwvsbureweneyqrgppofdkwunzejhvsabcjushcemum"));
	this->rzeowqhatytvsgyad(false, false, string("kaaaurwdwpsdyu"), 92324, string("pgfyozwesprcnxfnctpabgambwcbpvhzuzatayjorllafdsflaodusltrzik"), 1737, 2981, 5323, string("liakpsihdwdwatqodvdiosvwdkwgvmutdjgnisgvhzszzzyhqiovsdvqtzejlejonryyimowp"), string("ysgvcntabjlhmmedufnzatokczgspsfcbkiqduptwmanpwplphwdqu"));
	this->tqwaotaoxlzdqiuuthnosr(false, string("eiqvkgzzqkrbctmtpldydxpjeslkhqitmsceizbqvbxidzyllt"), 52284, 8335, string("wjodbspxdjfsnetevoinulinqmrxzryrnlnodaqhzdwlfcjxevjsvcbnijupsinioddpqktg"), string("sfoniwjheqhhmnjmvemfsxbvcqiqjoiysycdtuhuykgmrbyiyldgpaszqymtooluusfriybeecqklfob"));
	this->mxuawdjzip(454, false, string("dvqqlsdgubcpawtvcqqharmldnfyatuvvwrxzpqdoocklpcskruzhjjvmbfgxxmqjcrzkqjotbjvko"), true, 5916, 8742, 19909, 16930);
	this->zcfcjtdwmjtsqhdrttooevfb(148, 1639, 1424, 28373, 85206, string("bywcqqqlrahmwgtofguawsufodfilzahnzmdlgiicobfuyyrbzzmrikdotomm"));
	this->eintxpwcvxekwzsx(39767, false, string("rnfnd"));
	this->tdumueaorj(8297, string("hbdgrviahrtagmwaqqvhhyfzobvoflxovxusqzonzhotedtifeonwnkfznfkaiqlgdb"), 1591, 22448, 5790, string("jfbtbrziavtfdbwfjydduypkkeyabwlhhjoawlogwtivzhwzjeiepmpgxwlpswabftpnbarnvcyso"), 31003, 2138);
	this->aeobkpkygsaohdxzkf(true, string("xihudapmwllrlbmdikfbnxohkotxlseyoegbyozwihfsvcxsrgpggjucxcznsakwmaenfqswxlgev"), true, 2737);
	this->qjvgdnrjdhfuieql(string("umjgyfveebxnxypwkxgyztdqzxazpoqjrcgmquxzdgeeloovabfjexglxqnwtrybnujbdgiyipjy"), 36535, 2905, true, 11238, string("roblxqllpelvocirxpructcwmnrvcajbknyeozmbywnqnliwkhkchrucurssojroblkpbsloroosxufcmmozqau"), 3336, 8185, false);
	this->snzlfuwyunsphoxhuqa(77668, true, false, string("ndmjijqymerjysmlqylsdmjntgwoozbubukdtusibkgqkbrhgjrsaivhst"), 3527);
	this->rjocqytemfgsgerk(8333, string("ldfsysrquzxvgqlclehwyghkoguglxmwabgwwuzzdigryqqctoyxbcblkpznyrsdnvokgquaclxcpwfljybvhfsalzkao"));
}

#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class ljqpafd {
public:
	string qlgrieyznspqfu;
	string mdaixgwkci;
	ljqpafd();
	int pcslhudgxgfchihdjtjtxzp(double jzpkebluzijpy, string iebxqfeodilmoo, int joissoqnllpefsy, bool nseyswlhcssotkh, int rrkwzphgebo, double mnytplrtekh, string xboavxznxkagxrm, bool sfcywupqmsyiqr, int tegnpgbazmyxze);

protected:
	bool perpdkfpsit;
	string agixrh;
	bool zkrxc;

	bool bpzppxfzcpxinjazfdaoeaxuw(string seocomqt, int swxsoamumhg, double xocolzmop, double wkhxcrwcu, double aigyzseubuotxjl, int qzkaahcj, string dzbzb, double yotzb, bool wxcmnowsvmdhn, int lulde);
	int vgtdqwxxnpqt(bool osnqox, int avglmtlvs, double ogbgelkru, double oaowjmeopsltys, bool cnfvuagyalml, double yjxgvoqby, double knhccksmxj, int acwyjph, string qicznkmzwgloe);
	void krgoittlryilan(bool ooocilo, string vtyyxxenfbncoou, string vzaqpmvlumj, bool qpstb, bool plpsnvmzqtl, double bqcczvmsd, int uveufrfjoi, bool clpeagxkikjuhc, bool vfidvwicjowep);
	bool pjtlwyxokkqijenmtozmosg();
	void vaoexzqezlpzrjqdkmhp(bool geqrmsppxkjal, string zzhjmcu);
	double yioiyrcmcfadeqbjjqo(double nacoll, string zuqvvki, int brnjcx, bool wqzfwtyjwzjnuej, bool hyhllanxf, string xqazwzjgtrfniyw, int jmyuhr);
	string ndmgshuzabanagssfbqjcve(int caomjzhkfj, double blimqvytxcdmq, bool smejzr, int gprqykf, double ugqwoar, string waupulrquojxlp, string qwmqbocatartv, bool lyfjyhyvjlt, double kbtfcgubqrbjppe, string cigvtiyjlmbw);
	int yjdgtmifhiom(bool jblgpexjosf, bool qsguvomsn);
	double msepbduuxb(string zddkisdtdknvf);
	bool mtjcqszkjanytcjsiolvve(bool pjsplwima);

private:
	string tymiywm;
	string hjwdtzgnrpz;

	string yeyvmgmdbqzrgudg(bool ysvljpttbsz, string loflnucy, string gkfxdruwr, bool baqrbeclso, int jowelpa, double tfhgl, int zgtgmxhx, int oknuffcfvbx, bool fbcqwdridedftt, string ivpfd);
	bool rnobvjjzhxiznjef(bool htcizcycpmgs, bool uourweq, string bwravxevebydlye, double mwzjkxpdiooo, bool afmkmuvjwwh, bool wkisfsokntjx, double sdigkhoyvb, int yoswstbf);
	int ybalwicbpmywvouoqfxpxq(bool svrplcqmljjjtjw, string mzjqjjckkz, int hbyjatotpvib, int deovwvbdhzs, double ugrnh, bool fptxgpjqygkwy);
	double gmxyjnnnuevcrkehdbrnajlm(int eksutqiv, bool ahgwokrvut, int dhajeabj, int wswlkeqxaa, bool bfabcamkertxuq);
	double nnoaecrgjgpp(string yonbs, bool kodfhpqyz);
	string ukoyskpmsyeurparwvcgyy(double adwartjvvd, bool wblswcgkewnqw, double tldpyqdxzdnrl, bool itoewccou, double gurrec, int sfsiowmuxpod, bool kndzkbhyesx, bool pyytchoesa);
	int aympjxnlfceryatkwgz(double xmprtufney, double lqqppzbcaau, bool jfuigxf, int mypvczxpp, string dfhquqeppyusiv, int abnqxxmffdlmz, int bwcgeyjjaepzyfb, bool deqsk, bool mlqpbyeymk, double rvycipss);
	string yhnfqhwxbuyoaflr(string rpcnbkddmzn);
	void kgzuqtosmnqm(string xsoqolwfoih, bool suuoldeisb, double oebxcucchbasdm, double qfdkbxca, bool gbkakjpegpr, int fjtfbmvs, string jbumysi, int fhktu, double ehebcjkxcvy);
	string pkujzlsqipvpvcbsrcpk();

};


string ljqpafd::yeyvmgmdbqzrgudg(bool ysvljpttbsz, string loflnucy, string gkfxdruwr, bool baqrbeclso, int jowelpa, double tfhgl, int zgtgmxhx, int oknuffcfvbx, bool fbcqwdridedftt, string ivpfd) {
	int thdgkjxqvty = 2545;
	double nawvy = 33473;
	double yrarsfzcnkkbb = 38686;
	int acaskishochmj = 4280;
	bool mtrcqd = false;
	string fmqmubl = "hhythzblmiyjjazuoohntqmtszyypccdbgggvciotsnyrhars";
	double szoeijshepah = 8872;
	int ysvaosranahxen = 950;
	int zcujulmkrkjrg = 8014;
	if (950 == 950) {
		int eefak;
		for (eefak = 41; eefak > 0; eefak--) {
			continue;
		}
	}
	if (string("hhythzblmiyjjazuoohntqmtszyypccdbgggvciotsnyrhars") != string("hhythzblmiyjjazuoohntqmtszyypccdbgggvciotsnyrhars")) {
		int paju;
		for (paju = 0; paju > 0; paju--) {
			continue;
		}
	}
	if (38686 != 38686) {
		int ikiq;
		for (ikiq = 0; ikiq > 0; ikiq--) {
			continue;
		}
	}
	return string("lsylgqfbjjngdpwek");
}

bool ljqpafd::rnobvjjzhxiznjef(bool htcizcycpmgs, bool uourweq, string bwravxevebydlye, double mwzjkxpdiooo, bool afmkmuvjwwh, bool wkisfsokntjx, double sdigkhoyvb, int yoswstbf) {
	bool joxsbyd = false;
	int fgyhq = 1656;
	double qinxbdmfxbejazi = 4759;
	bool uzbvgmcysl = true;
	double iozcxvdovaf = 4447;
	if (1656 == 1656) {
		int sgvjeugap;
		for (sgvjeugap = 91; sgvjeugap > 0; sgvjeugap--) {
			continue;
		}
	}
	if (false == false) {
		int yjgvlfnxm;
		for (yjgvlfnxm = 68; yjgvlfnxm > 0; yjgvlfnxm--) {
			continue;
		}
	}
	if (1656 == 1656) {
		int wmcpvdg;
		for (wmcpvdg = 57; wmcpvdg > 0; wmcpvdg--) {
			continue;
		}
	}
	if (1656 != 1656) {
		int mfqml;
		for (mfqml = 84; mfqml > 0; mfqml--) {
			continue;
		}
	}
	if (1656 == 1656) {
		int lmd;
		for (lmd = 75; lmd > 0; lmd--) {
			continue;
		}
	}
	return true;
}

int ljqpafd::ybalwicbpmywvouoqfxpxq(bool svrplcqmljjjtjw, string mzjqjjckkz, int hbyjatotpvib, int deovwvbdhzs, double ugrnh, bool fptxgpjqygkwy) {
	bool cmfrapjqb = true;
	int dzeftipop = 2977;
	int mktrbgrbbavsq = 3351;
	int eavzohvejdkll = 4011;
	int zuakaqkc = 2063;
	int ykhft = 627;
	string tdjvvzch = "lrxqrrdemhkcmsyevklwjahhgncpyqflojgkvjcjwtxekdxojysiozaqpvbiuuziaryhmtnpq";
	if (2063 == 2063) {
		int akvpnxfa;
		for (akvpnxfa = 36; akvpnxfa > 0; akvpnxfa--) {
			continue;
		}
	}
	return 86552;
}

double ljqpafd::gmxyjnnnuevcrkehdbrnajlm(int eksutqiv, bool ahgwokrvut, int dhajeabj, int wswlkeqxaa, bool bfabcamkertxuq) {
	double oglmd = 9886;
	double umzkaict = 31894;
	int vxzyosyqhyuhysw = 53;
	bool ofbajplgc = true;
	if (true != true) {
		int owjcjurt;
		for (owjcjurt = 64; owjcjurt > 0; owjcjurt--) {
			continue;
		}
	}
	return 7910;
}

double ljqpafd::nnoaecrgjgpp(string yonbs, bool kodfhpqyz) {
	return 5055;
}

string ljqpafd::ukoyskpmsyeurparwvcgyy(double adwartjvvd, bool wblswcgkewnqw, double tldpyqdxzdnrl, bool itoewccou, double gurrec, int sfsiowmuxpod, bool kndzkbhyesx, bool pyytchoesa) {
	int rsrhbzbnjf = 1576;
	string vnfvgpish = "ydmnkqutxlbmfteqr";
	return string("jfwxwkzytqxwuqerw");
}

int ljqpafd::aympjxnlfceryatkwgz(double xmprtufney, double lqqppzbcaau, bool jfuigxf, int mypvczxpp, string dfhquqeppyusiv, int abnqxxmffdlmz, int bwcgeyjjaepzyfb, bool deqsk, bool mlqpbyeymk, double rvycipss) {
	bool nzpsnmbbot = false;
	double fuqlfqdrwqqu = 66872;
	bool fiwaqgsbrecjx = false;
	bool qikapulbjupg = false;
	int nqvglfiz = 7257;
	int wqwgotpudnfk = 2893;
	double gabhjmbcetc = 9700;
	if (false == false) {
		int xwwoz;
		for (xwwoz = 64; xwwoz > 0; xwwoz--) {
			continue;
		}
	}
	if (9700 == 9700) {
		int ntgavxs;
		for (ntgavxs = 75; ntgavxs > 0; ntgavxs--) {
			continue;
		}
	}
	if (false != false) {
		int evgcwma;
		for (evgcwma = 75; evgcwma > 0; evgcwma--) {
			continue;
		}
	}
	if (false == false) {
		int du;
		for (du = 15; du > 0; du--) {
			continue;
		}
	}
	return 62776;
}

string ljqpafd::yhnfqhwxbuyoaflr(string rpcnbkddmzn) {
	int waisenmusttj = 7302;
	double zazmuac = 946;
	if (946 == 946) {
		int whjnuxj;
		for (whjnuxj = 48; whjnuxj > 0; whjnuxj--) {
			continue;
		}
	}
	if (7302 == 7302) {
		int hvfhli;
		for (hvfhli = 22; hvfhli > 0; hvfhli--) {
			continue;
		}
	}
	if (7302 == 7302) {
		int nujrhvrv;
		for (nujrhvrv = 37; nujrhvrv > 0; nujrhvrv--) {
			continue;
		}
	}
	return string("xdceuyysfbfs");
}

void ljqpafd::kgzuqtosmnqm(string xsoqolwfoih, bool suuoldeisb, double oebxcucchbasdm, double qfdkbxca, bool gbkakjpegpr, int fjtfbmvs, string jbumysi, int fhktu, double ehebcjkxcvy) {
	bool hsnygeqhxb = true;
	int haiis = 6112;
	int kzfpjbljnkd = 590;
	if (true != true) {
		int zav;
		for (zav = 21; zav > 0; zav--) {
			continue;
		}
	}

}

string ljqpafd::pkujzlsqipvpvcbsrcpk() {
	int tygwqt = 3956;
	bool fpugqwqsoj = false;
	string phhxizvskds = "";
	if (string("") == string("")) {
		int udoj;
		for (udoj = 15; udoj > 0; udoj--) {
			continue;
		}
	}
	if (3956 != 3956) {
		int xueguxkm;
		for (xueguxkm = 64; xueguxkm > 0; xueguxkm--) {
			continue;
		}
	}
	if (3956 == 3956) {
		int twhfzdjy;
		for (twhfzdjy = 87; twhfzdjy > 0; twhfzdjy--) {
			continue;
		}
	}
	if (false == false) {
		int txyagv;
		for (txyagv = 91; txyagv > 0; txyagv--) {
			continue;
		}
	}
	return string("wwvw");
}

bool ljqpafd::bpzppxfzcpxinjazfdaoeaxuw(string seocomqt, int swxsoamumhg, double xocolzmop, double wkhxcrwcu, double aigyzseubuotxjl, int qzkaahcj, string dzbzb, double yotzb, bool wxcmnowsvmdhn, int lulde) {
	bool aqjxwht = false;
	double teskhrjnildcxw = 30191;
	if (false != false) {
		int lus;
		for (lus = 88; lus > 0; lus--) {
			continue;
		}
	}
	if (false == false) {
		int yx;
		for (yx = 83; yx > 0; yx--) {
			continue;
		}
	}
	return false;
}

int ljqpafd::vgtdqwxxnpqt(bool osnqox, int avglmtlvs, double ogbgelkru, double oaowjmeopsltys, bool cnfvuagyalml, double yjxgvoqby, double knhccksmxj, int acwyjph, string qicznkmzwgloe) {
	string cxqobg = "fflzmxhbpshcwwrlwjweikwveqvodnltwqndnzwepx";
	int uwawvhn = 6324;
	double bqivfglzezkoguf = 17034;
	double suasdvajnaqvkve = 14569;
	double lxskfmqq = 8281;
	double wgyshhltxgmaefa = 30819;
	double jslhdij = 15492;
	string bfxszzgqfuv = "delubfsidvnmqgckprunnotqtkvgllwsdrhojmwvlcnfcyepadmynkxhbgeqnhezsrwdicogcrhtmnbhjaimgyghbuaone";
	double cadpxhptv = 4758;
	if (30819 == 30819) {
		int fwoszxdsye;
		for (fwoszxdsye = 60; fwoszxdsye > 0; fwoszxdsye--) {
			continue;
		}
	}
	return 58494;
}

void ljqpafd::krgoittlryilan(bool ooocilo, string vtyyxxenfbncoou, string vzaqpmvlumj, bool qpstb, bool plpsnvmzqtl, double bqcczvmsd, int uveufrfjoi, bool clpeagxkikjuhc, bool vfidvwicjowep) {
	string gqxqzpsclvr = "vodlrdmtuzjdijskcnzb";
	double upaflyx = 5461;
	string wuoirtwzcd = "bgltzguelbomuxgwmeozxsuko";
	int exzrqyncsynph = 4401;
	bool nywamztfxy = false;
	int dgoifdd = 1937;
	int ptxkplisz = 2891;
	int vhjhmjf = 1286;
	double ycudrbkmu = 27988;
	if (4401 == 4401) {
		int tfnsl;
		for (tfnsl = 80; tfnsl > 0; tfnsl--) {
			continue;
		}
	}
	if (string("vodlrdmtuzjdijskcnzb") == string("vodlrdmtuzjdijskcnzb")) {
		int igmhsuvbk;
		for (igmhsuvbk = 82; igmhsuvbk > 0; igmhsuvbk--) {
			continue;
		}
	}

}

bool ljqpafd::pjtlwyxokkqijenmtozmosg() {
	string pqedlaqpylpns = "qvsojjhzkwglsuxpadukbylldhgrinjdamtcbasigzxsnthapppbcspjuntgqdgaqabkls";
	if (string("qvsojjhzkwglsuxpadukbylldhgrinjdamtcbasigzxsnthapppbcspjuntgqdgaqabkls") != string("qvsojjhzkwglsuxpadukbylldhgrinjdamtcbasigzxsnthapppbcspjuntgqdgaqabkls")) {
		int hm;
		for (hm = 0; hm > 0; hm--) {
			continue;
		}
	}
	if (string("qvsojjhzkwglsuxpadukbylldhgrinjdamtcbasigzxsnthapppbcspjuntgqdgaqabkls") != string("qvsojjhzkwglsuxpadukbylldhgrinjdamtcbasigzxsnthapppbcspjuntgqdgaqabkls")) {
		int uu;
		for (uu = 56; uu > 0; uu--) {
			continue;
		}
	}
	if (string("qvsojjhzkwglsuxpadukbylldhgrinjdamtcbasigzxsnthapppbcspjuntgqdgaqabkls") != string("qvsojjhzkwglsuxpadukbylldhgrinjdamtcbasigzxsnthapppbcspjuntgqdgaqabkls")) {
		int lfdadni;
		for (lfdadni = 72; lfdadni > 0; lfdadni--) {
			continue;
		}
	}
	if (string("qvsojjhzkwglsuxpadukbylldhgrinjdamtcbasigzxsnthapppbcspjuntgqdgaqabkls") != string("qvsojjhzkwglsuxpadukbylldhgrinjdamtcbasigzxsnthapppbcspjuntgqdgaqabkls")) {
		int xwj;
		for (xwj = 16; xwj > 0; xwj--) {
			continue;
		}
	}
	return false;
}

void ljqpafd::vaoexzqezlpzrjqdkmhp(bool geqrmsppxkjal, string zzhjmcu) {
	double ebybv = 55659;
	double nmezcv = 29095;
	double mnlkmeajrk = 9428;
	double nvnrnscfzldya = 28104;
	bool hfotsxsenvmblui = true;
	int ikcogbkbvo = 9534;
	bool zxutthoftmizs = false;
	string zvaoadrmjzwsswf = "iqxpcksdvxubsvtcaoaroinakxrdxegavhbvxpbhphymhkhdlwofzgnnvjqevvfeytakx";
	double udzvvasgw = 72323;
	string zcvogbqxeqza = "ryigkaaplozmtuzjnh";

}

double ljqpafd::yioiyrcmcfadeqbjjqo(double nacoll, string zuqvvki, int brnjcx, bool wqzfwtyjwzjnuej, bool hyhllanxf, string xqazwzjgtrfniyw, int jmyuhr) {
	double hynazwttetby = 23184;
	bool rtbrvyrbrl = true;
	bool krouapqtidwv = true;
	string fnkpzwervpg = "htsdxdwsobywuvyxqgulzijwyzuxnunyzlpnzlsivnpflebcwblvecdipxbdqgiamxcuvyasmqptuycn";
	string umedvsub = "hvunblnhzmzuwaszz";
	string dfuma = "kttmjetvkfadbvlrnweirwwmircpny";
	bool arexvfgcy = true;
	bool eqkmralrnhtxnz = true;
	bool glmbwdpbbtre = false;
	int tvcviekewv = 606;
	if (23184 != 23184) {
		int snh;
		for (snh = 0; snh > 0; snh--) {
			continue;
		}
	}
	if (606 != 606) {
		int uppi;
		for (uppi = 5; uppi > 0; uppi--) {
			continue;
		}
	}
	return 40572;
}

string ljqpafd::ndmgshuzabanagssfbqjcve(int caomjzhkfj, double blimqvytxcdmq, bool smejzr, int gprqykf, double ugqwoar, string waupulrquojxlp, string qwmqbocatartv, bool lyfjyhyvjlt, double kbtfcgubqrbjppe, string cigvtiyjlmbw) {
	int iasco = 3839;
	int pfjvgurapte = 492;
	string szngzxegvc = "rdayqmbaxhazrtfkkpkkqafyroknllieizyturujjduebvsqcpejkytdcwhvjerfioxuledhdcvr";
	return string("kjscjylzq");
}

int ljqpafd::yjdgtmifhiom(bool jblgpexjosf, bool qsguvomsn) {
	return 36815;
}

double ljqpafd::msepbduuxb(string zddkisdtdknvf) {
	int iduxf = 2946;
	string chrywpivurvxc = "yubetuzrwwraguqmnevbfgqgyuarancvnywuogdkuhcbjmawzyvqakzgpbwmbfrshbimrrnipwbxuaxpkbdbynflubswciifu";
	string tvpprt = "ohijsmzjoqrvvfnujhnzowuianiz";
	double tuybwsssad = 16384;
	double uuwxsxpam = 40229;
	int xazqeetodd = 3008;
	if (16384 == 16384) {
		int mex;
		for (mex = 67; mex > 0; mex--) {
			continue;
		}
	}
	if (40229 == 40229) {
		int umhpdp;
		for (umhpdp = 78; umhpdp > 0; umhpdp--) {
			continue;
		}
	}
	if (2946 != 2946) {
		int ecpzbw;
		for (ecpzbw = 2; ecpzbw > 0; ecpzbw--) {
			continue;
		}
	}
	if (3008 != 3008) {
		int adtunjrmlb;
		for (adtunjrmlb = 7; adtunjrmlb > 0; adtunjrmlb--) {
			continue;
		}
	}
	if (40229 != 40229) {
		int asgs;
		for (asgs = 48; asgs > 0; asgs--) {
			continue;
		}
	}
	return 17713;
}

bool ljqpafd::mtjcqszkjanytcjsiolvve(bool pjsplwima) {
	bool nalollje = false;
	int pgdcpcrzejh = 4268;
	bool ilrquh = true;
	int fjfeg = 3249;
	int rlvpfkoogi = 5799;
	int xmfcgg = 1428;
	double jhxoyevzznu = 55846;
	string tfchczeiond = "memoilqkitdrkqlvwrmq";
	if (true != true) {
		int psooerig;
		for (psooerig = 41; psooerig > 0; psooerig--) {
			continue;
		}
	}
	return true;
}

int ljqpafd::pcslhudgxgfchihdjtjtxzp(double jzpkebluzijpy, string iebxqfeodilmoo, int joissoqnllpefsy, bool nseyswlhcssotkh, int rrkwzphgebo, double mnytplrtekh, string xboavxznxkagxrm, bool sfcywupqmsyiqr, int tegnpgbazmyxze) {
	double mnneftnxe = 3383;
	bool vkhicyaia = false;
	bool akrkfqmwe = true;
	string iwvxpkpndrg = "qdhaharhun";
	string hkmyqovz = "trsgwxdptwawleiuuqyczhldhnxmafkmmhuyqrbb";
	return 58083;
}

ljqpafd::ljqpafd() {
	this->pcslhudgxgfchihdjtjtxzp(2766, string("uyarbygbpqudsrzbugxvibyzdaxgbalgsusxvmwikba"), 297, false, 1447, 317, string("hhduysvzamj"), true, 2043);
	this->bpzppxfzcpxinjazfdaoeaxuw(string("rkibbwflmxivwisn"), 2176, 34459, 23748, 5583, 154, string("boygsvu"), 50304, true, 7539);
	this->vgtdqwxxnpqt(true, 971, 8643, 8473, false, 6077, 17232, 1323, string("wipvxlnrsppbvtuicvytkmwnwxxijuenktjxnenenljsvp"));
	this->krgoittlryilan(true, string("mgplsypoyezudvbafrdpslzgbosyyfdstupiqksvnnxzriouyllwcmxocmpuzhkdrzgkxmmfvbxjzhltrcephsdlkibekgaurfy"), string("jfksixwoixdcngjmooothxzmdldhv"), false, true, 32529, 4361, false, false);
	this->pjtlwyxokkqijenmtozmosg();
	this->vaoexzqezlpzrjqdkmhp(true, string("swhbhzlbxksyxsdeqixtamrsywniqpqiotmiblqqwzqwstuglnjvvbflufnwgxskgocuevwtafdrkzmcxzbr"));
	this->yioiyrcmcfadeqbjjqo(8539, string("yrsif"), 1144, true, false, string("diamrdcgkmsdnlqvxamfzuhiaafcezmtzzvvpmakakineqisofdunapillumdljevyoqe"), 691);
	this->ndmgshuzabanagssfbqjcve(2857, 21304, true, 7341, 9937, string("fitspshpxuzqdfythntfotwrinkujowyaiaqwgl"), string("qowjbjuypthhcyjwwofolksigiqfimelutqdfzzjtgwouspluulyxnlvzauoogzlpvffgybaxj"), false, 11411, string("qijylroghvbqndfqoypmxmmzewqveokysfxethwctzcmyxfoxushrqutysnnhckssllbujgfclxidgzzugfpz"));
	this->yjdgtmifhiom(true, true);
	this->msepbduuxb(string("hbrfbnnwnmcyrfvimqdsbamlbjvbyaqgplwhhrsgmkybhnfzozpyjagtvumlncwmcdbloyqbtay"));
	this->mtjcqszkjanytcjsiolvve(true);
	this->yeyvmgmdbqzrgudg(false, string("rnhdewmchncmgfajtvhwjqucrzjvrtkuihzsbyxedbaprkzcgakyxyirlhcxvryjowjxk"), string("ezcppraoduxxkvdosmdyviuwlymuoumfeijijczotbdwigpso"), false, 142, 18844, 5843, 29, true, string("sngvjqmrzorrgpivxjtdogwrjgqajhensdwpnrhrahyzhwfxpoiaomamjvwqtliikpuoxtogbzdytconfsrsownwrwnplqacfnbt"));
	this->rnobvjjzhxiznjef(false, false, string("kigutznzbewehajugkrrkwhiopbovhlnavheazootkkpfavlijdcdwykkypslhx"), 25, false, false, 31162, 3238);
	this->ybalwicbpmywvouoqfxpxq(true, string("hyhnvcytp"), 5786, 824, 11379, true);
	this->gmxyjnnnuevcrkehdbrnajlm(3141, true, 6799, 3185, false);
	this->nnoaecrgjgpp(string("auxpinxkvmcrncjrqxefavpvl"), true);
	this->ukoyskpmsyeurparwvcgyy(12500, true, 21521, true, 7135, 3503, true, true);
	this->aympjxnlfceryatkwgz(41141, 18766, true, 2665, string("jiopzjpjsygbhbxvnejlcuwkaaghldclzzwximgnkkjbvaxdbdmpmilxkgightqjwgquvjkejvhnttgmbdgnsdbpefmbqtj"), 424, 5577, false, false, 47948);
	this->yhnfqhwxbuyoaflr(string("vuffktaunxmamzgppycfkzaycxxrrqecxuqijetthxeaskgnodvecbypqcoazqpok"));
	this->kgzuqtosmnqm(string("fmzntt"), true, 6033, 19029, false, 746, string("nrddswcswjditmofayzrxypvamyuuxhztfegdqrlacbnzqwnxvxvjtjfdquwsnjyfvefenlvgeialuwrh"), 2799, 44829);
	this->pkujzlsqipvpvcbsrcpk();
}

#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class inprcws {
public:
	double mkukivpr;
	double wdgwslkdqyt;
	inprcws();
	double qtbvvbciztreb(string afjlepzajny, bool bpyogvsyb);

protected:
	bool rrfogueewszvol;
	string gymhxqnqwmkzjzm;

	int rdjalqikcmzkb(string rwlggaqynbr, string clrjkmunqjf, double giokgnc, string dqlsusqzqyj, int lelnplyxaixbe, bool xexpl);
	bool vldpdiyiwe(bool oubeoxpruvwaszg, string iwnhw, string atfdrkkt, double lvdwzbjdbtskf, double oysmcdka, double yoetdkerb, double ejaii, bool orffjwlrrlayd, bool ddtxykoqy);
	string shyexjjmwxallxnfoohal(double jfckemd, int wchvykxrww, string lhgndvcvfvyx, double sldwnwdd, string gctdcaaocmcx, string psrrthwzc, double tnnpowua, string giyyptrrwc, string dfsbmokbxviu);
	void bfuownhsuh(double dcurcpkqzuz, bool dtvzaconspk);
	int icigvogvlyetrylutdn(bool nqvgrrs);
	bool wspjjjxyauwkz(bool losmahs, int oypdwicgtj, string tbevo, double ticvt, string zchvdivt, int vjjeh, bool sxplxayitf);

private:
	double ufgggxu;
	bool rpngc;

	bool fjpopmeizorzfsvewlq(bool lxeqgicnuf, int iiprjofxquxnse, string dugodqzjakx, double aejfkhl, double goddrlbd, int iaxch, bool waynbfg, bool cgibeujmp);
	string lbwmdbalaicejypaqrja(int zpyqcxxjjw, int zhudrxbajkfy, string kjrsok, double vlulleny, int synmpwtufk, string qgzfapdrlcqalm, int wpmxzerto, string bncdc, string idcoqskmiv);
	double gqfhjtkmjdpvzd(string cxpomlhb, string xxchdvirh, bool zgcvkojkflzmq, int fukgpdh, string codqwkmkmdkyb, double laiszhalxhdrimb, int yzrtux, int azixvxmkssej, int iujucyel);

};


bool inprcws::fjpopmeizorzfsvewlq(bool lxeqgicnuf, int iiprjofxquxnse, string dugodqzjakx, double aejfkhl, double goddrlbd, int iaxch, bool waynbfg, bool cgibeujmp) {
	int bzxbbjgnutuzrd = 6952;
	string qxuitrtczbyfhxn = "uzkbouztkgyjezaesspailqllcygebgtycsfgrfownualcuqswpjqnkwkmafwvubthkjtd";
	int wkmkzktgscn = 5583;
	int scqpwnptkqtdjig = 871;
	int nhglwsw = 1898;
	int wcklolulif = 423;
	string sxvftwhbax = "cwgvneooznufjfdoekdowrhnsulbvqdqca";
	bool gsfeia = true;
	if (true == true) {
		int ww;
		for (ww = 5; ww > 0; ww--) {
			continue;
		}
	}
	if (6952 == 6952) {
		int mccbabb;
		for (mccbabb = 52; mccbabb > 0; mccbabb--) {
			continue;
		}
	}
	if (423 != 423) {
		int lquofm;
		for (lquofm = 82; lquofm > 0; lquofm--) {
			continue;
		}
	}
	if (string("cwgvneooznufjfdoekdowrhnsulbvqdqca") != string("cwgvneooznufjfdoekdowrhnsulbvqdqca")) {
		int xjh;
		for (xjh = 38; xjh > 0; xjh--) {
			continue;
		}
	}
	return false;
}

string inprcws::lbwmdbalaicejypaqrja(int zpyqcxxjjw, int zhudrxbajkfy, string kjrsok, double vlulleny, int synmpwtufk, string qgzfapdrlcqalm, int wpmxzerto, string bncdc, string idcoqskmiv) {
	string ktkbrot = "ncmvdum";
	int ubobxsra = 2675;
	string dnjookizfjzh = "pxfbdqunrycpswagstmaxwiirucifgklhirrknnfzwbwzrbivmunrtojxukhhbbpmuemrpvmqxzqiurcccxrnhxa";
	string ugsem = "idxjxbhzdapyczytdkihzibovazclrqzyysfjnknjzuxluwgsngfvzgagdfpcyrbhsqtdpejnxydpazjgfgyjnbywltrkf";
	int ukipgjsok = 855;
	string ilmlrkhhax = "zdpwfuvwxknagnpeowkepxdrvbyyuxcidlpdyahjllzvduljukihvaciwyjhclpbykwvxklkqnipdpmuh";
	int eokfmawotvme = 111;
	if (string("idxjxbhzdapyczytdkihzibovazclrqzyysfjnknjzuxluwgsngfvzgagdfpcyrbhsqtdpejnxydpazjgfgyjnbywltrkf") == string("idxjxbhzdapyczytdkihzibovazclrqzyysfjnknjzuxluwgsngfvzgagdfpcyrbhsqtdpejnxydpazjgfgyjnbywltrkf")) {
		int oisfdddpf;
		for (oisfdddpf = 5; oisfdddpf > 0; oisfdddpf--) {
			continue;
		}
	}
	if (string("ncmvdum") != string("ncmvdum")) {
		int vuudvqdc;
		for (vuudvqdc = 19; vuudvqdc > 0; vuudvqdc--) {
			continue;
		}
	}
	if (111 != 111) {
		int ebdjoukhql;
		for (ebdjoukhql = 23; ebdjoukhql > 0; ebdjoukhql--) {
			continue;
		}
	}
	return string("enjiv");
}

double inprcws::gqfhjtkmjdpvzd(string cxpomlhb, string xxchdvirh, bool zgcvkojkflzmq, int fukgpdh, string codqwkmkmdkyb, double laiszhalxhdrimb, int yzrtux, int azixvxmkssej, int iujucyel) {
	double wihbmgvxeamtn = 31504;
	double udloguhinyg = 7397;
	bool ebgzyvpyyevt = true;
	if (7397 == 7397) {
		int pstegdlz;
		for (pstegdlz = 18; pstegdlz > 0; pstegdlz--) {
			continue;
		}
	}
	if (31504 != 31504) {
		int pvp;
		for (pvp = 82; pvp > 0; pvp--) {
			continue;
		}
	}
	if (true != true) {
		int jknxpci;
		for (jknxpci = 95; jknxpci > 0; jknxpci--) {
			continue;
		}
	}
	return 3459;
}

int inprcws::rdjalqikcmzkb(string rwlggaqynbr, string clrjkmunqjf, double giokgnc, string dqlsusqzqyj, int lelnplyxaixbe, bool xexpl) {
	return 70592;
}

bool inprcws::vldpdiyiwe(bool oubeoxpruvwaszg, string iwnhw, string atfdrkkt, double lvdwzbjdbtskf, double oysmcdka, double yoetdkerb, double ejaii, bool orffjwlrrlayd, bool ddtxykoqy) {
	bool bumdwoxy = false;
	string utdpemwxjia = "qkzkprtymmynrvuokxpwlarwvlexwzshbjtuudrobvtktkgyvrwslxxxbhnewrztwjtrjuontvels";
	int vqjbmofwqmu = 2736;
	string fynmoxwkjel = "oeatkvzbllajtsqngelixjghbnqvsdunrgirrdyuducauhu";
	if (2736 == 2736) {
		int nfrq;
		for (nfrq = 12; nfrq > 0; nfrq--) {
			continue;
		}
	}
	return false;
}

string inprcws::shyexjjmwxallxnfoohal(double jfckemd, int wchvykxrww, string lhgndvcvfvyx, double sldwnwdd, string gctdcaaocmcx, string psrrthwzc, double tnnpowua, string giyyptrrwc, string dfsbmokbxviu) {
	bool vbqbwuwuubc = true;
	if (true == true) {
		int pnfrblelj;
		for (pnfrblelj = 32; pnfrblelj > 0; pnfrblelj--) {
			continue;
		}
	}
	if (true == true) {
		int uqlkkljb;
		for (uqlkkljb = 17; uqlkkljb > 0; uqlkkljb--) {
			continue;
		}
	}
	if (true == true) {
		int pmxyw;
		for (pmxyw = 72; pmxyw > 0; pmxyw--) {
			continue;
		}
	}
	if (true == true) {
		int cawysfibi;
		for (cawysfibi = 72; cawysfibi > 0; cawysfibi--) {
			continue;
		}
	}
	return string("zkfoiv");
}

void inprcws::bfuownhsuh(double dcurcpkqzuz, bool dtvzaconspk) {
	string rcfrqmtdr = "xagifnnxvvji";
	string hhoznkwexhrstmr = "nlxa";
	int lqtxraajwg = 2931;
	if (2931 != 2931) {
		int cjdboar;
		for (cjdboar = 53; cjdboar > 0; cjdboar--) {
			continue;
		}
	}

}

int inprcws::icigvogvlyetrylutdn(bool nqvgrrs) {
	double uykuncww = 14753;
	bool txobjb = false;
	bool qorzebtbxnn = true;
	int mnjxxt = 1850;
	bool zsxrvv = true;
	string vlzhesijufnu = "cdfspszf";
	string zemlbpg = "eqbtunlkvabmwlh";
	bool geuvswp = false;
	double ulzzuu = 48800;
	if (true != true) {
		int fiazdfudw;
		for (fiazdfudw = 95; fiazdfudw > 0; fiazdfudw--) {
			continue;
		}
	}
	if (48800 != 48800) {
		int tai;
		for (tai = 88; tai > 0; tai--) {
			continue;
		}
	}
	if (false == false) {
		int jvjdwfpee;
		for (jvjdwfpee = 60; jvjdwfpee > 0; jvjdwfpee--) {
			continue;
		}
	}
	return 40959;
}

bool inprcws::wspjjjxyauwkz(bool losmahs, int oypdwicgtj, string tbevo, double ticvt, string zchvdivt, int vjjeh, bool sxplxayitf) {
	double ufysxz = 14955;
	bool krqaoxjnbt = true;
	double ccynxltcuybn = 13034;
	int gjvtbjzixxmh = 1360;
	if (1360 != 1360) {
		int dndm;
		for (dndm = 47; dndm > 0; dndm--) {
			continue;
		}
	}
	if (14955 != 14955) {
		int ya;
		for (ya = 74; ya > 0; ya--) {
			continue;
		}
	}
	if (14955 != 14955) {
		int qmifpdid;
		for (qmifpdid = 9; qmifpdid > 0; qmifpdid--) {
			continue;
		}
	}
	if (13034 != 13034) {
		int dh;
		for (dh = 99; dh > 0; dh--) {
			continue;
		}
	}
	return false;
}

double inprcws::qtbvvbciztreb(string afjlepzajny, bool bpyogvsyb) {
	string fzjqbk = "sctlxggkouqbahtfunnmvrrqlo";
	string uwiehzyux = "kvbdcwindojrlrftfuvrnprtnmbanhfkyq";
	string crvxvkqaubbiz = "yeawtpmzilkvoog";
	double puxvqgzrihzd = 27257;
	if (27257 != 27257) {
		int pf;
		for (pf = 1; pf > 0; pf--) {
			continue;
		}
	}
	if (string("kvbdcwindojrlrftfuvrnprtnmbanhfkyq") != string("kvbdcwindojrlrftfuvrnprtnmbanhfkyq")) {
		int qarblcs;
		for (qarblcs = 44; qarblcs > 0; qarblcs--) {
			continue;
		}
	}
	if (string("kvbdcwindojrlrftfuvrnprtnmbanhfkyq") == string("kvbdcwindojrlrftfuvrnprtnmbanhfkyq")) {
		int xzc;
		for (xzc = 49; xzc > 0; xzc--) {
			continue;
		}
	}
	if (27257 != 27257) {
		int cnh;
		for (cnh = 94; cnh > 0; cnh--) {
			continue;
		}
	}
	return 53552;
}

inprcws::inprcws() {
	this->qtbvvbciztreb(string("sugppphmrd"), true);
	this->rdjalqikcmzkb(string("walsjxxnxwyywjqspffkdofisjwrpylbrduzyqqhnqeiyfoanwynnvdhviejchdjxdczlftxkgrttzqhfmdksmviqvnvhnkflxks"), string("kyysusthlwdsdqwtniaiujbcqizvppvaouzesdwcvhiqlsddspgrmuhnvwgfzlgfheplvnn"), 11810, string("nxidczgyozownsxvpwxjukoalwpcuhjmxepkqqjmcrnwspjfghoycnizcfaleachlsy"), 7775, true);
	this->vldpdiyiwe(false, string("bgupyxfmzgdhqutbcuwakstngtna"), string("dbzhgmuqlxgxokalgnzlrsrddlawvzqwhsntftyrabdbajtvdlmduffhzapthekgxhg"), 26139, 83999, 6721, 15545, true, false);
	this->shyexjjmwxallxnfoohal(22313, 1153, string("ixisosuqdluxguhbbdjawbmcjdanxfoafhmpzeeoffuzimgwifefbqutiqmivqxuwdvramkxluwqc"), 19140, string("fxbonuhyifssuzqsmrvqhglwlznlniksicdtlugshnsufpuncwkheyohehdmgnbyuimpxqiylqadetiumyfxytbsrskw"), string("rphnevqmcutxjidwhygxhajgdjgofgfxbcaftztejqfbmsygtafprkscphdjtm"), 59296, string("iyhfogrikpaerwijebwmxadqbnpkaczdtfshxhzqvqrzquhxosmsehseozwjsysdemok"), string("vniaaodepkvnlpkwxeogbkdy"));
	this->bfuownhsuh(6035, false);
	this->icigvogvlyetrylutdn(false);
	this->wspjjjxyauwkz(true, 6213, string("znnnazxtqkctpwvtwyrgektvoewfjd"), 7314, string("iukk"), 2998, false);
	this->fjpopmeizorzfsvewlq(true, 1703, string("texpkoigicvnvtnuotgubiblozjjpsnfpkptjy"), 16535, 11164, 296, true, false);
	this->lbwmdbalaicejypaqrja(1436, 3873, string("udfzttzh"), 9752, 1840, string("jdfsusqykyxcavjmegbdccwkjhdc"), 3364, string("rnwpzuzpwmlsgibgoaowuocfjvvmodptpcomqroghalrirkbyeknpl"), string("bpbpppinebmgdcoedsh"));
	this->gqfhjtkmjdpvzd(string("cmkerlsdkmqc"), string("zeyjhqemgqmhbretedcsiwcnhnolay"), true, 470, string("hzhaqejvvewukvnrvfjkeqntwwccnqsofxcwvfjafzuaklhsxjoghyyfoguc"), 7307, 1790, 1269, 2503);
}
