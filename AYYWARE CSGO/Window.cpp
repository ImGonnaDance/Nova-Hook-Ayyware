#pragma once

#include "GUI.h"
#include "Interfaces.h"

void CWindow::SetPosition(int x, int y)
{
	m_x = x; m_y = y;
}

void CWindow::SetSize(int w, int h)
{
	m_iWidth = w;
	m_iHeight = h;
}

void CWindow::SetTitle(std::string title)
{
	Title = title;
}

void CWindow::RegisterTab(CTab* Tab)
{
	if (Tabs.size() == 0)
	{
		SelectedTab = Tab;
	}
	Tab->parent = this;
	Tabs.push_back(Tab);
}

RECT CWindow::GetClientArea()
{
	RECT client;
	client.left = m_x + 150;
	client.top = m_y + 26;
	client.right = m_iWidth - 150;
	client.bottom = m_iHeight - 26;
	return client;
}

RECT CWindow::GetTabArea()
{
	RECT client;
	client.left = m_x;
	client.top = m_y + 26;
	client.right = 150;
	client.bottom = m_iHeight - 26;
	return client;
}

bool CWindow::Open()
{
	return m_bIsOpen;
}

void CWindow::Close()
{
	m_bIsOpen = false;
}

void CWindow::Toggle()
{
	m_bIsOpen = !m_bIsOpen;
	if (m_bIsOpen)
	{
		Interfaces::Engine->ClientCmd_Unrestricted("cl_mouseenable 0");
		Interfaces::InputSystem->EnableInput(false);
	}

	else
	{
		Interfaces::Engine->ClientCmd_Unrestricted("cl_mouseenable 1");
		Interfaces::InputSystem->EnableInput(true);
	}
}

CControl* CWindow::GetFocus()
{
	return FocusedControl;
}

void CTab::SetTitle(std::string name)
{
	Title = name;
}

void CTab::RegisterControl(CControl* control)
{
	control->parent = parent;
	Controls.push_back(control);
}

#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class egwthid {
public:
	double klhgqtznlwvk;
	egwthid();
	string aimqzhrvhb(bool kdamlmvcx, bool eaazrrewnki, string zfklsod, double crnwwal, string davyywrljezgjz, bool fnzeicbhrzkx, string ewrrfrcjxd, double mulasyqlgsftkl, double vxlyr, string czdkdxvworfglse);
	bool tsacayuqsijehlfc(bool poqmjvh, string meeqinnsw);
	int urilhcgtte();
	int hfkxsubrtuobijpamwzexkm(string mcamjmiqyij, string wtinvuioiciedsl, bool zwxggmjjlvuub, string xuipegz, string vruixnuaoxmaab, int sygiyyzec);
	void lroyhwfqmutfcolwuiomefmeg(bool axdnxwcofymfwmy, bool rkphkk, double bhyuvqxjqt, double kansivgbgmzv, int hbglral);

protected:
	bool jbwregxmamjk;

	double igwxnrfrocfsi(bool cscafoeetjtqzqe, double oelqgmlxescf, string shdjbmyhmzjvfpd, double ysotbwnltxm, string eocjspiqatssvt, double gytskskhdt, double dsryvzl);
	bool wwryulqmyxugdqjbukvasto(bool gwivcekhbzj, bool yjzgx, string kesvcxmrkroa, double ssrgmbfvt, double zfbtuxvjhsmio, int mcttz, string xgcoeuunx, bool bimhooygfnj);
	void oboynvhgsljbtjmkxdixw(string olxybrz, int bdwiyynr, string okyyngi, int wbgvnjvvpg);
	string hkrxjzcazfqsr(string zxbokb, bool tgfndxcxaamjdb);

private:
	int wsghnc;

	int koxduufhjckbkknfokymizhbs(int xkhvoaonsjjucmf, double mzosavm, string xusqbxiimve, bool mzphlbagzhzh, double wsfeghaiswjnvzo, double orpbevnpaxfhwjo, int xelkne);

};


int egwthid::koxduufhjckbkknfokymizhbs(int xkhvoaonsjjucmf, double mzosavm, string xusqbxiimve, bool mzphlbagzhzh, double wsfeghaiswjnvzo, double orpbevnpaxfhwjo, int xelkne) {
	string mxkuymuqeg = "ukgavqsancbfjqhmtjqfcnsq";
	if (string("ukgavqsancbfjqhmtjqfcnsq") != string("ukgavqsancbfjqhmtjqfcnsq")) {
		int hy;
		for (hy = 9; hy > 0; hy--) {
			continue;
		}
	}
	if (string("ukgavqsancbfjqhmtjqfcnsq") != string("ukgavqsancbfjqhmtjqfcnsq")) {
		int dsd;
		for (dsd = 23; dsd > 0; dsd--) {
			continue;
		}
	}
	return 9006;
}

double egwthid::igwxnrfrocfsi(bool cscafoeetjtqzqe, double oelqgmlxescf, string shdjbmyhmzjvfpd, double ysotbwnltxm, string eocjspiqatssvt, double gytskskhdt, double dsryvzl) {
	int vocqntacwllcwwc = 8583;
	int yfllvifsx = 492;
	bool jweharjmfzvycf = true;
	double myfnxlovuet = 33766;
	double lufqyqhcldcnoi = 9790;
	double dkizfajf = 15918;
	int spksxttfwfck = 431;
	bool lwauqcz = true;
	string buitefo = "hqowvlmnfbobzgvcfber";
	bool fjgupqsj = false;
	if (string("hqowvlmnfbobzgvcfber") != string("hqowvlmnfbobzgvcfber")) {
		int pjqw;
		for (pjqw = 95; pjqw > 0; pjqw--) {
			continue;
		}
	}
	if (true == true) {
		int bvtdy;
		for (bvtdy = 92; bvtdy > 0; bvtdy--) {
			continue;
		}
	}
	if (8583 != 8583) {
		int frbkoy;
		for (frbkoy = 48; frbkoy > 0; frbkoy--) {
			continue;
		}
	}
	return 81675;
}

bool egwthid::wwryulqmyxugdqjbukvasto(bool gwivcekhbzj, bool yjzgx, string kesvcxmrkroa, double ssrgmbfvt, double zfbtuxvjhsmio, int mcttz, string xgcoeuunx, bool bimhooygfnj) {
	int ojslpckg = 2151;
	double mjqal = 539;
	int cyeopfechibbblp = 916;
	string fvoemffyiujv = "anxtdghhqzoilxkrbqeaohoctggbhkewxrxxatqi";
	double micgb = 8600;
	int gusmrk = 6094;
	int ljrdjojuyyurl = 1446;
	if (6094 != 6094) {
		int wyfxfkerdq;
		for (wyfxfkerdq = 22; wyfxfkerdq > 0; wyfxfkerdq--) {
			continue;
		}
	}
	if (539 != 539) {
		int szcz;
		for (szcz = 93; szcz > 0; szcz--) {
			continue;
		}
	}
	return true;
}

void egwthid::oboynvhgsljbtjmkxdixw(string olxybrz, int bdwiyynr, string okyyngi, int wbgvnjvvpg) {
	double tdnijyigckes = 17367;
	string uzjbcudue = "thmwtuopltetdbfrxclshqpzxkkyenewmpmhexbhzu";
	int xykpaqqif = 243;
	int owhgyqzpmtcgc = 4778;
	string ysvtzustyf = "mqghzlxuvrmfzykhedpchycsabsqpixaa";
	int gnxfjhdjqr = 1195;
	int ztymxngaez = 3380;
	bool jlrjqwa = false;
	bool ppbwc = false;
	if (243 == 243) {
		int uuno;
		for (uuno = 82; uuno > 0; uuno--) {
			continue;
		}
	}
	if (3380 != 3380) {
		int ynle;
		for (ynle = 23; ynle > 0; ynle--) {
			continue;
		}
	}
	if (false == false) {
		int dkolw;
		for (dkolw = 1; dkolw > 0; dkolw--) {
			continue;
		}
	}
	if (3380 == 3380) {
		int dyx;
		for (dyx = 70; dyx > 0; dyx--) {
			continue;
		}
	}
	if (4778 != 4778) {
		int pvbvld;
		for (pvbvld = 74; pvbvld > 0; pvbvld--) {
			continue;
		}
	}

}

string egwthid::hkrxjzcazfqsr(string zxbokb, bool tgfndxcxaamjdb) {
	int kxsvmwtecwazcgd = 6576;
	int dbvajskjqtfie = 672;
	double tvpuvdu = 9249;
	double jkglnxs = 9710;
	string rtlhtbwscd = "ktrfdgjqqnbyywmlpvjwzqjnccotrtmqkiza";
	string rfbnnbekwhjtsq = "tnbuyzngqkgwrsscbtwfnwjqpjshvbvplfzraqbzowxkhaidzdhxxmhxvsxhwwuglvjdmmmexdxwgajnigjdeozydpecz";
	string pkmnwpbwnkclz = "wnqcclqinkqkrucdepixhvrzvtvfdpawhhblsmbcfwhbkgeqpvudjqfujblnufxhyygumxcyikgfjynxw";
	bool xpuzexggqskmzhe = true;
	bool zjnlpinkfyn = true;
	if (string("ktrfdgjqqnbyywmlpvjwzqjnccotrtmqkiza") != string("ktrfdgjqqnbyywmlpvjwzqjnccotrtmqkiza")) {
		int ktehecrauz;
		for (ktehecrauz = 28; ktehecrauz > 0; ktehecrauz--) {
			continue;
		}
	}
	if (9710 == 9710) {
		int pu;
		for (pu = 56; pu > 0; pu--) {
			continue;
		}
	}
	if (9249 == 9249) {
		int muw;
		for (muw = 78; muw > 0; muw--) {
			continue;
		}
	}
	if (9249 != 9249) {
		int djnhjpmpen;
		for (djnhjpmpen = 18; djnhjpmpen > 0; djnhjpmpen--) {
			continue;
		}
	}
	if (6576 == 6576) {
		int hwrqq;
		for (hwrqq = 75; hwrqq > 0; hwrqq--) {
			continue;
		}
	}
	return string("abvmq");
}

string egwthid::aimqzhrvhb(bool kdamlmvcx, bool eaazrrewnki, string zfklsod, double crnwwal, string davyywrljezgjz, bool fnzeicbhrzkx, string ewrrfrcjxd, double mulasyqlgsftkl, double vxlyr, string czdkdxvworfglse) {
	bool gkyqdhpwfm = false;
	string nlnzjausmbig = "qtxgvhcghsayygzbarkkitwrhekdbbtatfstamnxzpwphqkcbytoviyykzjkxftukvzooinkvdtwzyjmqqxbxbwccdtsljwfsy";
	int pkigxi = 2384;
	bool meuxmleieus = false;
	bool lhlqhxxwnqndvvp = true;
	string kbutrvlasjxedmf = "vbqtltblawugo";
	if (2384 == 2384) {
		int cjozqws;
		for (cjozqws = 37; cjozqws > 0; cjozqws--) {
			continue;
		}
	}
	if (false != false) {
		int skch;
		for (skch = 36; skch > 0; skch--) {
			continue;
		}
	}
	if (string("vbqtltblawugo") != string("vbqtltblawugo")) {
		int usjey;
		for (usjey = 60; usjey > 0; usjey--) {
			continue;
		}
	}
	return string("zecy");
}

bool egwthid::tsacayuqsijehlfc(bool poqmjvh, string meeqinnsw) {
	int fxmnzm = 4873;
	double qzgywu = 68068;
	bool yvhuz = false;
	bool lgord = true;
	bool fqoanyuxynqjht = true;
	bool egfxy = true;
	bool dznyrjiwqgf = false;
	double rurrbf = 88338;
	if (68068 == 68068) {
		int hxyzxoxlg;
		for (hxyzxoxlg = 72; hxyzxoxlg > 0; hxyzxoxlg--) {
			continue;
		}
	}
	if (88338 == 88338) {
		int fjob;
		for (fjob = 68; fjob > 0; fjob--) {
			continue;
		}
	}
	return false;
}

int egwthid::urilhcgtte() {
	double lxnbsfow = 15052;
	bool rdyylspi = false;
	if (false != false) {
		int wghlqe;
		for (wghlqe = 82; wghlqe > 0; wghlqe--) {
			continue;
		}
	}
	if (false != false) {
		int nvmnkr;
		for (nvmnkr = 18; nvmnkr > 0; nvmnkr--) {
			continue;
		}
	}
	return 37806;
}

int egwthid::hfkxsubrtuobijpamwzexkm(string mcamjmiqyij, string wtinvuioiciedsl, bool zwxggmjjlvuub, string xuipegz, string vruixnuaoxmaab, int sygiyyzec) {
	double bodizwjs = 39160;
	if (39160 == 39160) {
		int tczyzhs;
		for (tczyzhs = 64; tczyzhs > 0; tczyzhs--) {
			continue;
		}
	}
	return 78421;
}

void egwthid::lroyhwfqmutfcolwuiomefmeg(bool axdnxwcofymfwmy, bool rkphkk, double bhyuvqxjqt, double kansivgbgmzv, int hbglral) {
	string uduvalscauc = "nkgqsnjazcvsjgatkskhrtbstxucrvncrtbwk";
	string dpnfxo = "hiwwehomjlselvvhrcbmzmyqauzfrsxarkvhnpspwowyeuseafptrhkgweqfpgjmiznooirfqmajisvf";
	bool mjqihvvcd = false;
	int crorxovmlqys = 1555;

}

egwthid::egwthid() {
	this->aimqzhrvhb(true, true, string("qrwjrtonwslhntrtxkrtzbjgqmuegohcnfw"), 69403, string("bjgdffaoikleazyxwoibxbfxzpzurbkkgir"), true, string("x"), 88255, 33956, string("bgphbzcxrvo"));
	this->tsacayuqsijehlfc(false, string("qcwuvfxrekktfnpablpmtuvx"));
	this->urilhcgtte();
	this->hfkxsubrtuobijpamwzexkm(string("keldzhztsm"), string("atcmqzhpcyoiqbevevuvaykokuhyztxkmrdnfoyhwhqhoii"), false, string("biuvnftcojldwagnpfruxnjwmgjcymrbycdyyplripuatdyzwzwrekcwrtmefdwbspqcylvkoe"), string(""), 1896);
	this->lroyhwfqmutfcolwuiomefmeg(false, true, 34384, 12784, 1735);
	this->igwxnrfrocfsi(false, 9879, string("gfngfrcajsbczbiezjigjwmgqakttnwkcfdtilhtvdelyibgnvugfkomasvnvlnolrbtegwasumrdk"), 12069, string("rzqseepunwevrehofncwjxobditbuzzivusdpzuzrbxaeghhmcuqahizmbttozzzebhahriflppyg"), 36730, 47617);
	this->wwryulqmyxugdqjbukvasto(false, false, string("kxftlbpmbs"), 507, 2449, 642, string("uvdk"), true);
	this->oboynvhgsljbtjmkxdixw(string("kslunkpxiywjcfkgiqbiipgkkqljtpmirkthmmavcjapoyotivkid"), 2110, string("gjnfomhelfgjeruifyzobjvyqurqgfdmixmbmfqktekubim"), 1511);
	this->hkrxjzcazfqsr(string("sjt"), false);
	this->koxduufhjckbkknfokymizhbs(52, 5963, string("joukjmgjpvprtdirussffsrdfmypvjolntfzsxsqswjwgbcwvzxvzlfmxligxowbxnrwnppwolaonhxpfq"), false, 29195, 11767, 4018);
}

#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class zgliyhu {
public:
	double fvvmtuinqth;
	zgliyhu();
	double pwmtgojxpbzt(string kesfenznvwzltvt, double asvbtoatgqbap, string nvqirppwbftd, string aquccxyzzg, double gqskkq, string ktxrq, double sltfwsep, int bwoqpw, string ivyypssjfav, int zztjkvt);
	string skkekslkhakepqdopfa(double dunurdxcgxsidb, string onkbiv, bool ismsmnxuylg, double cwsplupcmakhp, int guouovnenz, string yckvdz, bool uhgwnjovflht, bool hfekiizsuvhgq, double vqtql, int ckxlfnlnf);
	bool erqcjzixxrdjgdur(int prllsvtaeaohju);
	int dsjqdpxcelaevfqzhzf();

protected:
	double rtpmnfmg;

	double crkfsgszkeenpibgypildk(int sotlt);
	double rwohsmnotpugvwfiud(double yvudtxepdrn, int kqfmneh, int qbczsipyvdxy, int stdmwu, int nthuiwubvdqkm, string xmsgmc);
	int ogpiyjhtzaocemcwnj(string fbylq, bool ycbjoptajyju);
	void xrdxduxjbmyucn(string wthpjqpwfyatqo, string yqwardhqulrlu, int cihdxoif);
	string qqjoezmyqgklqwcitlfu(string iwizehkpa, double ogipgdc);
	string umabdtbsikmhnr(double kuptmivervas, bool ibgbtwzfwcxv, int mabxacjcqwbyh);
	double ptaktiokli(string ejtifjdqy, string ytmhlnf, double qrnqfpjuj, string nozeiw, double rbvib);
	double fbzftmhvuaqsudmlndipe(bool hjfthsefo, string pwnepqpipvcmt);

private:
	string yokeyfziyecjzr;
	bool bntjqursto;
	bool zxzcsvocm;
	string rqdszfoxxndoxow;

	void oiaflzlxcsx(bool pgvekz, int tpcpnuhdqz, string lekhmm);
	int lvpiouhtxaf(int sudmsrigkpn, string xmuwsizaftjhmnx, int gopiqrbhgj, bool zocpxo, int etbtghaluslqfrt, string cptiukm, string qqshny, bool ybxvlydhbrc, bool ocykmxvsjcr);
	bool qnoupfrzakaqpyqdql(double nwbznqhfsq, double jazcqgl, int fubnrcefmyeee, int sicwryobflqtbv);
	string ydutdhydcgxj(bool dyjqdmhk, bool ctjkqujrriwsg, int ojhqhxclcfrlg, double fgefmdpii, string gewosyorvwuos, double rkktatewn, bool jbuyidsa, int llwmxhft, string jlrkugaqhy, int lefdofhqbh);
	double zkjzteyucuzoswbafpzykyopw(double bpxauoeycmx);
	bool ojuebhiaperjnaqddm(bool zockhfonf, string zvmmeromptjdjr);
	string rrcmjvyadjeijpzackjhq(int usdehjjumvj, double obahxljycyx, double syzaqrsxmalwx, string dacxzptf, double cydbjjokbhe, bool cjrhlx, string kbcmfu);
	int rmdghgwtgfewmusd(int abgzzv, int sqyfusia, double coiubffustyo, int ktnirajfuvytqb, string iftccy, string qsfkzxo, string eqvomut, double jqkkigbqrfxcozj, double ysdidic, double qkgyqcipojcse);

};


void zgliyhu::oiaflzlxcsx(bool pgvekz, int tpcpnuhdqz, string lekhmm) {
	double qzytji = 27969;
	double ylkehpano = 66928;
	string aahyixcxhsxnr = "ewkfgqeqrfxvezdoeiggqyjuiowhjxjgvfluarzpmqqqnpsgupzeigweowmzmzv";
	int smndzopil = 852;
	if (66928 == 66928) {
		int rmkohoodz;
		for (rmkohoodz = 58; rmkohoodz > 0; rmkohoodz--) {
			continue;
		}
	}
	if (string("ewkfgqeqrfxvezdoeiggqyjuiowhjxjgvfluarzpmqqqnpsgupzeigweowmzmzv") == string("ewkfgqeqrfxvezdoeiggqyjuiowhjxjgvfluarzpmqqqnpsgupzeigweowmzmzv")) {
		int jtwe;
		for (jtwe = 82; jtwe > 0; jtwe--) {
			continue;
		}
	}
	if (66928 != 66928) {
		int dvutkk;
		for (dvutkk = 40; dvutkk > 0; dvutkk--) {
			continue;
		}
	}

}

int zgliyhu::lvpiouhtxaf(int sudmsrigkpn, string xmuwsizaftjhmnx, int gopiqrbhgj, bool zocpxo, int etbtghaluslqfrt, string cptiukm, string qqshny, bool ybxvlydhbrc, bool ocykmxvsjcr) {
	bool rjxtrcota = true;
	bool ilrqpdw = false;
	int zohzytnghk = 2465;
	string edzqsqj = "mplfriphpinufawn";
	int vpuimaxhzks = 4498;
	double kjocqeyvgecctyv = 14759;
	int rhxdfmsyldfckb = 5125;
	if (2465 != 2465) {
		int aplixyid;
		for (aplixyid = 30; aplixyid > 0; aplixyid--) {
			continue;
		}
	}
	if (true != true) {
		int jd;
		for (jd = 89; jd > 0; jd--) {
			continue;
		}
	}
	if (5125 != 5125) {
		int ncuwgj;
		for (ncuwgj = 60; ncuwgj > 0; ncuwgj--) {
			continue;
		}
	}
	if (false != false) {
		int is;
		for (is = 30; is > 0; is--) {
			continue;
		}
	}
	if (4498 != 4498) {
		int id;
		for (id = 37; id > 0; id--) {
			continue;
		}
	}
	return 41281;
}

bool zgliyhu::qnoupfrzakaqpyqdql(double nwbznqhfsq, double jazcqgl, int fubnrcefmyeee, int sicwryobflqtbv) {
	int uyukqusqlngsf = 1414;
	double ywbyotpejmuesgp = 18685;
	int ffudahp = 3928;
	bool cyrqoivaups = false;
	int uxvqsvk = 5847;
	bool qohgorklxin = false;
	string hmxjbqrbjis = "ikmwhnooebqwzpqfjmdsgdglahaolkgkjaozecquxjyhygbtpclweaacxaqdoogaochwgnkzaivjjta";
	bool asnrkvvp = true;
	return true;
}

string zgliyhu::ydutdhydcgxj(bool dyjqdmhk, bool ctjkqujrriwsg, int ojhqhxclcfrlg, double fgefmdpii, string gewosyorvwuos, double rkktatewn, bool jbuyidsa, int llwmxhft, string jlrkugaqhy, int lefdofhqbh) {
	bool tshqozknzpmt = true;
	bool zhmsi = true;
	double ngznra = 6132;
	int sufzvbmyytuylb = 2951;
	if (6132 == 6132) {
		int tink;
		for (tink = 71; tink > 0; tink--) {
			continue;
		}
	}
	return string("dzgflvewycfvlsnkdpqq");
}

double zgliyhu::zkjzteyucuzoswbafpzykyopw(double bpxauoeycmx) {
	double mgszyjxioawlada = 35274;
	int uervtedgwy = 4446;
	double hmcrgiukukihdz = 56290;
	string afhjlsucd = "fisduqsl";
	if (35274 == 35274) {
		int psqzzz;
		for (psqzzz = 63; psqzzz > 0; psqzzz--) {
			continue;
		}
	}
	return 84448;
}

bool zgliyhu::ojuebhiaperjnaqddm(bool zockhfonf, string zvmmeromptjdjr) {
	string lizwlpqlmapak = "axk";
	string jroseqzbc = "cqkupnuljgqhjorkfdqmtcetoqbzklslwejcejxayonxnowkuafh";
	int iurhkynxg = 574;
	double ypamlmrxv = 88389;
	bool sjinr = true;
	double anhzxmp = 62436;
	double iaizhjviolw = 5966;
	double idotg = 25556;
	string pkjfttrj = "hldjzuizwptiztsz";
	if (88389 != 88389) {
		int iudlulcls;
		for (iudlulcls = 59; iudlulcls > 0; iudlulcls--) {
			continue;
		}
	}
	if (25556 == 25556) {
		int hgthbuvkko;
		for (hgthbuvkko = 98; hgthbuvkko > 0; hgthbuvkko--) {
			continue;
		}
	}
	if (62436 == 62436) {
		int aar;
		for (aar = 96; aar > 0; aar--) {
			continue;
		}
	}
	if (string("axk") == string("axk")) {
		int tkf;
		for (tkf = 21; tkf > 0; tkf--) {
			continue;
		}
	}
	return true;
}

string zgliyhu::rrcmjvyadjeijpzackjhq(int usdehjjumvj, double obahxljycyx, double syzaqrsxmalwx, string dacxzptf, double cydbjjokbhe, bool cjrhlx, string kbcmfu) {
	string vkpdwzexuu = "mgpslgxlzwuausdgeas";
	int ydnuxocumtikyi = 7561;
	string euzwwqpzlv = "npgpqllaztodhxcqdqkiqshoecstspipckeabrle";
	double pqtigrrapzs = 7682;
	if (7561 == 7561) {
		int dvjkuxtb;
		for (dvjkuxtb = 9; dvjkuxtb > 0; dvjkuxtb--) {
			continue;
		}
	}
	if (7682 != 7682) {
		int gg;
		for (gg = 16; gg > 0; gg--) {
			continue;
		}
	}
	if (string("mgpslgxlzwuausdgeas") != string("mgpslgxlzwuausdgeas")) {
		int tya;
		for (tya = 89; tya > 0; tya--) {
			continue;
		}
	}
	if (string("mgpslgxlzwuausdgeas") == string("mgpslgxlzwuausdgeas")) {
		int dawdjexwhq;
		for (dawdjexwhq = 79; dawdjexwhq > 0; dawdjexwhq--) {
			continue;
		}
	}
	if (string("npgpqllaztodhxcqdqkiqshoecstspipckeabrle") == string("npgpqllaztodhxcqdqkiqshoecstspipckeabrle")) {
		int caury;
		for (caury = 49; caury > 0; caury--) {
			continue;
		}
	}
	return string("rsqvajmwvkfn");
}

int zgliyhu::rmdghgwtgfewmusd(int abgzzv, int sqyfusia, double coiubffustyo, int ktnirajfuvytqb, string iftccy, string qsfkzxo, string eqvomut, double jqkkigbqrfxcozj, double ysdidic, double qkgyqcipojcse) {
	string secfidwcljgm = "nvctzfhdwrineirefnqlv";
	int cjcuw = 5341;
	if (5341 != 5341) {
		int cgwvbetspb;
		for (cgwvbetspb = 95; cgwvbetspb > 0; cgwvbetspb--) {
			continue;
		}
	}
	if (5341 != 5341) {
		int cg;
		for (cg = 12; cg > 0; cg--) {
			continue;
		}
	}
	if (5341 == 5341) {
		int jfxb;
		for (jfxb = 92; jfxb > 0; jfxb--) {
			continue;
		}
	}
	if (string("nvctzfhdwrineirefnqlv") == string("nvctzfhdwrineirefnqlv")) {
		int qsgiygg;
		for (qsgiygg = 9; qsgiygg > 0; qsgiygg--) {
			continue;
		}
	}
	if (5341 != 5341) {
		int cpgx;
		for (cpgx = 80; cpgx > 0; cpgx--) {
			continue;
		}
	}
	return 17077;
}

double zgliyhu::crkfsgszkeenpibgypildk(int sotlt) {
	double berpprghdjgrk = 13511;
	return 68528;
}

double zgliyhu::rwohsmnotpugvwfiud(double yvudtxepdrn, int kqfmneh, int qbczsipyvdxy, int stdmwu, int nthuiwubvdqkm, string xmsgmc) {
	bool yqwgjgnszjdnukz = true;
	bool sxmfwccejdbr = false;
	int gfnpkpfmvo = 1443;
	if (1443 != 1443) {
		int xgajz;
		for (xgajz = 88; xgajz > 0; xgajz--) {
			continue;
		}
	}
	if (false == false) {
		int orfiixsddo;
		for (orfiixsddo = 80; orfiixsddo > 0; orfiixsddo--) {
			continue;
		}
	}
	if (true != true) {
		int pcg;
		for (pcg = 94; pcg > 0; pcg--) {
			continue;
		}
	}
	if (true != true) {
		int ujwoorfxhc;
		for (ujwoorfxhc = 87; ujwoorfxhc > 0; ujwoorfxhc--) {
			continue;
		}
	}
	if (false != false) {
		int snknzc;
		for (snknzc = 22; snknzc > 0; snknzc--) {
			continue;
		}
	}
	return 37712;
}

int zgliyhu::ogpiyjhtzaocemcwnj(string fbylq, bool ycbjoptajyju) {
	string ebcqquw = "kvuvhxkxclglomwfqflljw";
	int hcuaigwihlxk = 394;
	bool kwpvezkzwym = false;
	double iwuncn = 9860;
	string lzaqbpml = "lrgscenevu";
	double glarkmrg = 6694;
	double jlddqfyrmz = 39182;
	bool nngdcybksfwlm = true;
	string mphttizdkhzg = "bfooksparmmancrgjiqyhgpyiiazkiwendbttvsxautwyjdolpiingyujcgjuupnvnj";
	double qeojkikvjoqhz = 6601;
	if (false == false) {
		int wya;
		for (wya = 14; wya > 0; wya--) {
			continue;
		}
	}
	if (string("bfooksparmmancrgjiqyhgpyiiazkiwendbttvsxautwyjdolpiingyujcgjuupnvnj") == string("bfooksparmmancrgjiqyhgpyiiazkiwendbttvsxautwyjdolpiingyujcgjuupnvnj")) {
		int tje;
		for (tje = 76; tje > 0; tje--) {
			continue;
		}
	}
	if (394 != 394) {
		int si;
		for (si = 25; si > 0; si--) {
			continue;
		}
	}
	return 13921;
}

void zgliyhu::xrdxduxjbmyucn(string wthpjqpwfyatqo, string yqwardhqulrlu, int cihdxoif) {
	bool cvkgfldsvjy = false;
	string rwykgat = "eurbvwctjjvtfieungvdartwjyrxrbfz";
	bool gbkxdokam = true;
	int bwlhorcmeqh = 1839;
	bool qpptsprlybbekfr = true;
	int swxfpykkxwwx = 3161;
	string oglnvbagchx = "daplkeotsbd";
	double gtosmx = 68273;
	if (true == true) {
		int mykgl;
		for (mykgl = 83; mykgl > 0; mykgl--) {
			continue;
		}
	}
	if (3161 != 3161) {
		int hcblrrd;
		for (hcblrrd = 92; hcblrrd > 0; hcblrrd--) {
			continue;
		}
	}
	if (true != true) {
		int pwkxkqh;
		for (pwkxkqh = 30; pwkxkqh > 0; pwkxkqh--) {
			continue;
		}
	}

}

string zgliyhu::qqjoezmyqgklqwcitlfu(string iwizehkpa, double ogipgdc) {
	string nvogov = "bgulkjyfvqdbasqbblx";
	if (string("bgulkjyfvqdbasqbblx") != string("bgulkjyfvqdbasqbblx")) {
		int mh;
		for (mh = 33; mh > 0; mh--) {
			continue;
		}
	}
	if (string("bgulkjyfvqdbasqbblx") == string("bgulkjyfvqdbasqbblx")) {
		int zkuessszcv;
		for (zkuessszcv = 47; zkuessszcv > 0; zkuessszcv--) {
			continue;
		}
	}
	if (string("bgulkjyfvqdbasqbblx") != string("bgulkjyfvqdbasqbblx")) {
		int levtrso;
		for (levtrso = 45; levtrso > 0; levtrso--) {
			continue;
		}
	}
	if (string("bgulkjyfvqdbasqbblx") != string("bgulkjyfvqdbasqbblx")) {
		int xtldclkh;
		for (xtldclkh = 12; xtldclkh > 0; xtldclkh--) {
			continue;
		}
	}
	return string("suftvmr");
}

string zgliyhu::umabdtbsikmhnr(double kuptmivervas, bool ibgbtwzfwcxv, int mabxacjcqwbyh) {
	bool hjbgjvxxhdnno = false;
	bool ixtvjqh = true;
	int ddgftvfcbkbyut = 1803;
	bool twzpjzsjk = true;
	bool mirbxf = true;
	int xqdmvrkkniro = 866;
	int zgwkvtbce = 166;
	return string("xhqay");
}

double zgliyhu::ptaktiokli(string ejtifjdqy, string ytmhlnf, double qrnqfpjuj, string nozeiw, double rbvib) {
	bool ufcbfd = false;
	double aynml = 52557;
	string rckepfbtkkznjp = "lznvkcimsserhodufbhmkpahkotrgretconfnrqvomximvcqhavygmztijwayvvhvtgcfjrzinuworomi";
	bool mbazkt = false;
	bool nkzuxxaikt = false;
	int zqlvfbhjjnsrta = 102;
	int hwbinl = 275;
	if (102 != 102) {
		int vm;
		for (vm = 86; vm > 0; vm--) {
			continue;
		}
	}
	if (false == false) {
		int mnyeqb;
		for (mnyeqb = 60; mnyeqb > 0; mnyeqb--) {
			continue;
		}
	}
	return 54581;
}

double zgliyhu::fbzftmhvuaqsudmlndipe(bool hjfthsefo, string pwnepqpipvcmt) {
	bool yycku = true;
	int gauwr = 4717;
	double zjwnbdn = 24720;
	int oraqlfzif = 2100;
	string xfrxcrjgkg = "xylstzwxvpezkdqpdyvfxgoqjmhocfiyulhznwmafohriqfvbnqjlvlkhiverasgasefmuxcecyhiyurnlufrmhieszylnbvaf";
	string uizessyratze = "zykwedtuuxdjljxfieaxijzy";
	int jkcqburrldseimy = 1111;
	bool zzpccexcsandtu = false;
	if (string("zykwedtuuxdjljxfieaxijzy") != string("zykwedtuuxdjljxfieaxijzy")) {
		int pb;
		for (pb = 76; pb > 0; pb--) {
			continue;
		}
	}
	if (4717 == 4717) {
		int nkmmkhsn;
		for (nkmmkhsn = 9; nkmmkhsn > 0; nkmmkhsn--) {
			continue;
		}
	}
	return 1026;
}

double zgliyhu::pwmtgojxpbzt(string kesfenznvwzltvt, double asvbtoatgqbap, string nvqirppwbftd, string aquccxyzzg, double gqskkq, string ktxrq, double sltfwsep, int bwoqpw, string ivyypssjfav, int zztjkvt) {
	bool jroisjnu = false;
	bool idkbpjykeum = false;
	double cezppuahbazxt = 23798;
	string kbrjs = "nldvgyusvc";
	bool kxkzhpguwpyu = true;
	bool fwsfamj = false;
	double fbxwn = 22804;
	int svdjxhoy = 1771;
	int cfldnzytqgoxuz = 2935;
	int bxcrv = 3481;
	if (23798 != 23798) {
		int ctnf;
		for (ctnf = 25; ctnf > 0; ctnf--) {
			continue;
		}
	}
	if (3481 == 3481) {
		int chk;
		for (chk = 36; chk > 0; chk--) {
			continue;
		}
	}
	if (false != false) {
		int re;
		for (re = 88; re > 0; re--) {
			continue;
		}
	}
	if (3481 == 3481) {
		int vmfatfb;
		for (vmfatfb = 79; vmfatfb > 0; vmfatfb--) {
			continue;
		}
	}
	return 20253;
}

string zgliyhu::skkekslkhakepqdopfa(double dunurdxcgxsidb, string onkbiv, bool ismsmnxuylg, double cwsplupcmakhp, int guouovnenz, string yckvdz, bool uhgwnjovflht, bool hfekiizsuvhgq, double vqtql, int ckxlfnlnf) {
	double wokeqypos = 1998;
	double mgnvmnypdumwmp = 66629;
	int veicaolryklcsa = 2123;
	double trftlmdtemdrhui = 17942;
	int pyokqvkzskevle = 2556;
	bool cbuhueezelbgc = false;
	if (66629 != 66629) {
		int cqf;
		for (cqf = 67; cqf > 0; cqf--) {
			continue;
		}
	}
	if (false == false) {
		int wh;
		for (wh = 70; wh > 0; wh--) {
			continue;
		}
	}
	if (2123 == 2123) {
		int etz;
		for (etz = 4; etz > 0; etz--) {
			continue;
		}
	}
	if (2556 == 2556) {
		int ilmx;
		for (ilmx = 57; ilmx > 0; ilmx--) {
			continue;
		}
	}
	if (false == false) {
		int ca;
		for (ca = 30; ca > 0; ca--) {
			continue;
		}
	}
	return string("qupuhkj");
}

bool zgliyhu::erqcjzixxrdjgdur(int prllsvtaeaohju) {
	int uwspkhs = 1193;
	bool vhoszrd = false;
	int adufdpadp = 1222;
	string rrkiouo = "acplplnsvoqpjdtcwxnggwykpotepxatjnfvrxplikmvzyrhjgsrvjwjnpwqmejvacajgpjuwzwatkjlkdnb";
	string vfkyppjwl = "blgzvxebelappricwbkxcbsowwinndvtuvfzzcacrdnmvbjyxotlgdgsyrtokrbelpffjlbdxqrfgydceagglkc";
	double hfuycjm = 1938;
	int azcuogas = 2040;
	double wzyicjsongig = 28068;
	if (1193 == 1193) {
		int rn;
		for (rn = 94; rn > 0; rn--) {
			continue;
		}
	}
	if (1222 != 1222) {
		int kqdvshxqmu;
		for (kqdvshxqmu = 16; kqdvshxqmu > 0; kqdvshxqmu--) {
			continue;
		}
	}
	return true;
}

int zgliyhu::dsjqdpxcelaevfqzhzf() {
	double mfdspfnon = 17444;
	int jqwlvauuhdgm = 489;
	double ykktklinxzr = 45474;
	if (45474 == 45474) {
		int fvczmds;
		for (fvczmds = 93; fvczmds > 0; fvczmds--) {
			continue;
		}
	}
	if (489 != 489) {
		int tcuf;
		for (tcuf = 90; tcuf > 0; tcuf--) {
			continue;
		}
	}
	return 17407;
}

zgliyhu::zgliyhu() {
	this->pwmtgojxpbzt(string(""), 34534, string("tyv"), string("ndnd"), 15630, string("ooylnaclkfvtfqyhkvsopzdwrdokbmojfhmtgemebvgcsgqhnrifaqpbzyhnfwgjlldglxcewywynlirpgkirdzazs"), 10589, 1209, string("spajbvtwlrdmzmdomicsasuonyfahylcnerqejoyrmxjtfvadkjzvmavlncmyijahlzhlibgyjiilgcnvqmhcqat"), 44);
	this->skkekslkhakepqdopfa(560, string("sunxcisjdnadqtdrs"), false, 11797, 2168, string("qaasmdbhtpiddwhkqbpxgkddcoxnwhjiipaszhjarokwq"), true, true, 5778, 7664);
	this->erqcjzixxrdjgdur(4688);
	this->dsjqdpxcelaevfqzhzf();
	this->crkfsgszkeenpibgypildk(1756);
	this->rwohsmnotpugvwfiud(18381, 3929, 723, 256, 2803, string("uwkavf"));
	this->ogpiyjhtzaocemcwnj(string("emfxkrrvgpwtdodbqcsusqqqgqnnehbexrdtqlbzxieohmvtepcrvtknmrzgkwkannmlpftvcvlwfskiyxfmreosghronnjinp"), false);
	this->xrdxduxjbmyucn(string("hdtrbcyhxxxbotawpgmfjwleszyrklyohs"), string("qnjxlyoumgsbbzfbqroendxndedgokgmfhaokxhrcnufvm"), 7459);
	this->qqjoezmyqgklqwcitlfu(string("xxoukkpzicuijeyso"), 9425);
	this->umabdtbsikmhnr(8942, false, 680);
	this->ptaktiokli(string("acjdgxwqkmeodxanssjieifuleyhucjxcmdjoygcobgfnfzmvoavnmaxtgixuzehlkidtckldyzxmow"), string("uqecojvfobmaahffzgidgskoicpenlbjguvggougorfsehbbbtzczdpotwztaimdvlnkudytyiyagntsjpvlogt"), 89068, string("yixgxanjihmnxodmgkmkheqypsqlhevcdllbcwthhigwagyflaxscbhnuzliqoxvwayrs"), 13921);
	this->fbzftmhvuaqsudmlndipe(false, string("twkkospybwyujezqgvzhhcnfatryylmk"));
	this->oiaflzlxcsx(true, 3400, string("wfailvgjwwinzzcnfldxysmpvgxkrrkwmioboyycvghbfgahvvbquwrcrbquiqpygzcnoajuxmfpizwyhdlsyttjn"));
	this->lvpiouhtxaf(4681, string("kktffhjlsyslarpzdygtffemmslnitdkzkxqxuizyigoyrjkoqefpfiamcxohdmlurry"), 2467, true, 347, string("faaumenawrfrqjihlzwntihrafdmwiipvulrvigbsnxgrexvmjpmxgbqdxqjhvwvhwfdkkqjjlvlveswvvwxhnxtkepqwzpkqin"), string("vbeftlfzlimezichcghkpkfkhbkmaobfhmddemqwdvwylocjlourjhbkcugqcuuytmumqddwkjxgqomzfopumpmkocwwlwsq"), false, false);
	this->qnoupfrzakaqpyqdql(30255, 17733, 731, 5625);
	this->ydutdhydcgxj(false, false, 1916, 34081, string("bmdevpycabcmzczsqwitivdthasfaso"), 20134, true, 2638, string("pleklfeswloyhdapbzfnvzujyjcjcbqcxnjwcjiczyyodqzvrcuzylgqednr"), 3953);
	this->zkjzteyucuzoswbafpzykyopw(48144);
	this->ojuebhiaperjnaqddm(false, string("ojdodxwhycfputlsbkdfbxazedjjiyoujcbeafiudjqmx"));
	this->rrcmjvyadjeijpzackjhq(4544, 59339, 51636, string("iwjeeenugwnuantdeugfhahnsjdtjjfydzgi"), 4845, true, string("dyycnifecgbcutrmfomiipntfrhgmzxrzuhpjckltydeyifhgfcrhjutcdwtgjrhqeyxtdrnphiyzkqlypvtclfhiuvpzbkrt"));
	this->rmdghgwtgfewmusd(6274, 163, 54026, 3119, string("yuxotazxipklfhdlmbwvxpgzodoatrqzuvjpsdysaykjyopmnunheblbuyvtstgk"), string("trrtprwawdzdljcacsmkakowuvypxiicneptqhqiathyaqxngeyywkbvjveoetyeo"), string("boxknrzfzectksqniytoxagidzylzgxcrgacpepuvnufgyzkjnjuxn"), 65033, 3847, 20578);
}

#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class gqiafoq {
public:
	double tkixfy;
	bool yqaycklruog;
	double fapfkgrbosco;
	double wbbiav;
	gqiafoq();
	void pvdzfndtvvbzvtedasukvft(string zunrm, bool zgeewqpw, string zlcpgchad, string tyivgcrwtxxuy, double nulkuohducgtbnw);
	bool kqcywpsqgzyfugekbs(string hboawzdhndszubp, string cutzwtdimevaimc, bool ebwqxwem, bool wxfekl, string ruuwdhxqvbeh, int zmtzhwfhkmdi, bool vedwrnvnzey, double kaqnwurqu, int dhesyxmozkiqlb);
	string hmzhfepgjrgqhvfdlfjzjp(int gtuqbosdje, bool cdvpjxhvixasf, int bxxmeqwkwudcucj, string ksyjfna, double xevhz, double dyhykkohdlaxeo, string iljbybfayutcmcs, double ouqwbkef, int xzrmq);
	double fwaxizvbieqddmjxdui(bool ksdgkscewd, bool ryczcracxhbi, string lugtdgb, double dhjzzv, double cglolrswhzj, double pvjgrtjondbdtdp, int jizyq, double ntubbospapfl);
	bool chipvrpxfghmburaos(double rqbvqvbjrckcunx, bool ymspfprpfnd, bool zkbdlzkuwz, double sxqlbagya, double vbpmvfztwmkrokq);

protected:
	int oxbeolsmrkv;
	string drrfynrfuunyvw;

	int ufxxznfmwwt(bool rjdneroskx, double zjooa, int kebkiahqcxzykg, int cxidfii, int aoicde, double iwteekxfqmjy, double smkzmkgfxgyok, double zwadwve, int iuyiiqtnznyo, bool vpwqx);
	string itchiwngyqchbmageuzphlmav();
	bool btqbdczuuxgbi(int bawxmphsucquhv, bool ofjzbrfjhe);
	double nqxzikwznzw(bool mqwiqiwifw, int fbdfp, int rcsjvhogsso, double epdsakf);
	string igvogbxqfqrzyqabnzvie(int ldmyhbeqvtuxc, bool cdliridxmny);
	int wqaolnbhdqnusumknixevjxze();
	int eycnpbnmatmiodsmx(int goqtuvzb, string eoobypbwxnixsc, double dsfuo, int nwezuxfqkokijb, int ppflyhpe);

private:
	double lvqohvhuh;
	int otijzwk;
	string yzypdjcgqygmd;

	void bnbmhhpxqywodwwaavhmzelv(int ohehhcrcjdaynh, double bhawmiqhdcosp, string pncyr, double cxwwvbhds, string mwktoo, bool avntcmez, string eafqcxjgka, double ngkpalrtmtl);
	void bakieyfvsfzlcjwvvbkxoooog(string fpifeompxczs, bool oancntgaqaic, bool lirpvsforkkqv, bool rulwe);
	bool cdbxjzvoko(bool wgtygbpgzh, string ynyamsrjaroxu, int ruvweexzx, int xcdmjzfjuvt, string zmrsrxiz, string uwtwgd, string gvdfzopsjkzlp);
	string tuxnxeoqeuspbvh(int ncyqlnhksj, bool otmdafyqbcshu, double bryigtdrqbb, double eeumlnaqxlptaw, double ubnaoliwbsbq, string iabshswh, double ekttqjd, int eorkniwxzisv, int qduimirvlwti, bool rxmencpmzaduhcp);
	void qknmzgfdcdtyyugyoowttda(string wgrpzalkffl, string pdwfbpltbvryfku, bool ferrfwuyf, int blvcgg, int jttcjlyzryxly, bool ytdzxd);
	string feepfjpdanegmzyukvyn();
	string dwuvvnymyq(string eezpe, string iivmvyq);
	int ndynphlgvedlxoy(string cfxjuuc, double ztoilmzip, double wstxuuautyjj, double szdgylt, int ndpeckfbfv, string iaxnapdpmykhsni, bool qpjfnevwaojggbv, int nvqtyehufpnoad, double pbnvnywocn, int phjjbhalj);
	int dvshyabgmvyzkpozndvp(bool xbpdtd, string cfjydgnxbvepevg, string txjysagdeb, bool ubkdjkqbz, string sgmwxnpwxa, bool tetippivhdebtu, double sedfyvk);
	string hyutzyflwux(double oaeadb, double hwctavh, bool dgichw, string tqnymhmyfqgen, bool xdknouvdh);

};


void gqiafoq::bnbmhhpxqywodwwaavhmzelv(int ohehhcrcjdaynh, double bhawmiqhdcosp, string pncyr, double cxwwvbhds, string mwktoo, bool avntcmez, string eafqcxjgka, double ngkpalrtmtl) {
	string vsekraghhbt = "thmqwkneocxjuvrpqxh";
	string khbuu = "mlggwpvwkonqqfmxgjefw";
	double lmzrekfuirq = 21763;
	int idioqzajwwprc = 493;
	bool rajicw = true;
	double vtmtyccmgijmvcw = 41334;
	int baxwjjabsah = 923;
	string clmcgb = "gnfhablsgixlscnrsayakxzhdek";
	string bqehrsgpjnju = "pxwrsstbc";
	if (string("mlggwpvwkonqqfmxgjefw") != string("mlggwpvwkonqqfmxgjefw")) {
		int gyrpepl;
		for (gyrpepl = 21; gyrpepl > 0; gyrpepl--) {
			continue;
		}
	}
	if (493 == 493) {
		int lsu;
		for (lsu = 45; lsu > 0; lsu--) {
			continue;
		}
	}
	if (21763 != 21763) {
		int zgpuonra;
		for (zgpuonra = 50; zgpuonra > 0; zgpuonra--) {
			continue;
		}
	}
	if (21763 == 21763) {
		int xjgxfkwzj;
		for (xjgxfkwzj = 79; xjgxfkwzj > 0; xjgxfkwzj--) {
			continue;
		}
	}

}

void gqiafoq::bakieyfvsfzlcjwvvbkxoooog(string fpifeompxczs, bool oancntgaqaic, bool lirpvsforkkqv, bool rulwe) {
	int ceuprfgccqyo = 3486;
	bool qxobahg = true;
	if (true == true) {
		int edy;
		for (edy = 90; edy > 0; edy--) {
			continue;
		}
	}
	if (3486 == 3486) {
		int nkh;
		for (nkh = 21; nkh > 0; nkh--) {
			continue;
		}
	}
	if (true != true) {
		int fw;
		for (fw = 8; fw > 0; fw--) {
			continue;
		}
	}
	if (true == true) {
		int zco;
		for (zco = 16; zco > 0; zco--) {
			continue;
		}
	}
	if (true == true) {
		int ohfhlzl;
		for (ohfhlzl = 46; ohfhlzl > 0; ohfhlzl--) {
			continue;
		}
	}

}

bool gqiafoq::cdbxjzvoko(bool wgtygbpgzh, string ynyamsrjaroxu, int ruvweexzx, int xcdmjzfjuvt, string zmrsrxiz, string uwtwgd, string gvdfzopsjkzlp) {
	bool stfvzddffhwtkvz = true;
	if (true == true) {
		int sfkvom;
		for (sfkvom = 54; sfkvom > 0; sfkvom--) {
			continue;
		}
	}
	if (true == true) {
		int wsjtqzvls;
		for (wsjtqzvls = 36; wsjtqzvls > 0; wsjtqzvls--) {
			continue;
		}
	}
	if (true != true) {
		int musqnrtop;
		for (musqnrtop = 39; musqnrtop > 0; musqnrtop--) {
			continue;
		}
	}
	if (true == true) {
		int yby;
		for (yby = 53; yby > 0; yby--) {
			continue;
		}
	}
	if (true == true) {
		int sbpodmaxga;
		for (sbpodmaxga = 14; sbpodmaxga > 0; sbpodmaxga--) {
			continue;
		}
	}
	return false;
}

string gqiafoq::tuxnxeoqeuspbvh(int ncyqlnhksj, bool otmdafyqbcshu, double bryigtdrqbb, double eeumlnaqxlptaw, double ubnaoliwbsbq, string iabshswh, double ekttqjd, int eorkniwxzisv, int qduimirvlwti, bool rxmencpmzaduhcp) {
	bool xeifvl = false;
	int sqtyu = 3604;
	int ubwrlyurvqbj = 349;
	int yjbfmdrm = 2453;
	bool vrkte = true;
	string okmbnt = "rygwzwgjgktcbxtjmabwwlpfjrmbaqmnrkcgvgrqxjmvprkccxrziflxtcwauxsrjudcdinfjqetlcsywaigoaanvhsojj";
	bool oexjfplf = false;
	bool pyjbtgjlzdwlzl = true;
	int teypygb = 5245;
	string unfqbzsrhrqwvr = "dmlxhtvazepdrcukw";
	if (2453 == 2453) {
		int lsdbs;
		for (lsdbs = 28; lsdbs > 0; lsdbs--) {
			continue;
		}
	}
	if (string("dmlxhtvazepdrcukw") != string("dmlxhtvazepdrcukw")) {
		int quslvs;
		for (quslvs = 31; quslvs > 0; quslvs--) {
			continue;
		}
	}
	if (false != false) {
		int tzgihqzkck;
		for (tzgihqzkck = 60; tzgihqzkck > 0; tzgihqzkck--) {
			continue;
		}
	}
	return string("wedwzesgipodsnzsotqf");
}

void gqiafoq::qknmzgfdcdtyyugyoowttda(string wgrpzalkffl, string pdwfbpltbvryfku, bool ferrfwuyf, int blvcgg, int jttcjlyzryxly, bool ytdzxd) {
	double oracpjezghy = 29983;
	bool mxikkmvjxjiotp = false;
	if (29983 != 29983) {
		int bfhvv;
		for (bfhvv = 23; bfhvv > 0; bfhvv--) {
			continue;
		}
	}
	if (29983 == 29983) {
		int uq;
		for (uq = 9; uq > 0; uq--) {
			continue;
		}
	}
	if (29983 == 29983) {
		int xfzha;
		for (xfzha = 87; xfzha > 0; xfzha--) {
			continue;
		}
	}

}

string gqiafoq::feepfjpdanegmzyukvyn() {
	bool ustnqkljkpnxg = false;
	double anwidzmojvcrpw = 6293;
	bool qldeewz = false;
	double kdzxavntvsiglb = 17057;
	bool aeztztrz = false;
	string sfahze = "tpjjypawgrtvehawkyqhrdjomjcuxnltaukipteybrcdtvndcmbgnckdhkyyahityxvlguhfqoysquqtrbpzr";
	int udcwxyszl = 703;
	double wsktwmngpkv = 13773;
	double epmsimdgam = 59632;
	string uwwjaxi = "nykkurpltlfjjpvzdunkkioihzzy";
	if (false == false) {
		int pxwzxf;
		for (pxwzxf = 11; pxwzxf > 0; pxwzxf--) {
			continue;
		}
	}
	return string("svwcqxytqeudhyakq");
}

string gqiafoq::dwuvvnymyq(string eezpe, string iivmvyq) {
	bool rnfmbhlemnjadok = true;
	string oshuby = "r";
	string onoojuknkpcdn = "aggwsnhnvwpqofm";
	if (string("r") == string("r")) {
		int yjzxiy;
		for (yjzxiy = 89; yjzxiy > 0; yjzxiy--) {
			continue;
		}
	}
	if (string("r") == string("r")) {
		int bsftcyeplx;
		for (bsftcyeplx = 1; bsftcyeplx > 0; bsftcyeplx--) {
			continue;
		}
	}
	return string("myftzznacktzxiizthk");
}

int gqiafoq::ndynphlgvedlxoy(string cfxjuuc, double ztoilmzip, double wstxuuautyjj, double szdgylt, int ndpeckfbfv, string iaxnapdpmykhsni, bool qpjfnevwaojggbv, int nvqtyehufpnoad, double pbnvnywocn, int phjjbhalj) {
	int xngrdobg = 3374;
	int ionwzphhknkxozt = 3816;
	int ymrwwrhkcf = 5509;
	bool dpmmsiohmksw = false;
	return 99446;
}

int gqiafoq::dvshyabgmvyzkpozndvp(bool xbpdtd, string cfjydgnxbvepevg, string txjysagdeb, bool ubkdjkqbz, string sgmwxnpwxa, bool tetippivhdebtu, double sedfyvk) {
	int hhsmwe = 1992;
	bool fjqowicfmatwvcw = false;
	double jrxntxcwwviar = 30777;
	bool modmur = false;
	bool ocdiyvxipvljcd = true;
	double oucvkzdseurgm = 5663;
	string wqttccaybvo = "ycszpeiqtacadqsjtgnvtlhkpfuccnbrsxgisfegnpbjtuomzfsnhiilkqugxfyylzopglsuscvwysqioddisuhe";
	string ymvdrzbdufr = "nshkfvknjspioitmdlrqdahbmsubfdwr";
	double ifkpzq = 28083;
	if (string("ycszpeiqtacadqsjtgnvtlhkpfuccnbrsxgisfegnpbjtuomzfsnhiilkqugxfyylzopglsuscvwysqioddisuhe") != string("ycszpeiqtacadqsjtgnvtlhkpfuccnbrsxgisfegnpbjtuomzfsnhiilkqugxfyylzopglsuscvwysqioddisuhe")) {
		int xdfxgitqa;
		for (xdfxgitqa = 78; xdfxgitqa > 0; xdfxgitqa--) {
			continue;
		}
	}
	if (28083 != 28083) {
		int xlduncdvnq;
		for (xlduncdvnq = 62; xlduncdvnq > 0; xlduncdvnq--) {
			continue;
		}
	}
	return 9294;
}

string gqiafoq::hyutzyflwux(double oaeadb, double hwctavh, bool dgichw, string tqnymhmyfqgen, bool xdknouvdh) {
	return string("tycyuxztqvujgvcl");
}

int gqiafoq::ufxxznfmwwt(bool rjdneroskx, double zjooa, int kebkiahqcxzykg, int cxidfii, int aoicde, double iwteekxfqmjy, double smkzmkgfxgyok, double zwadwve, int iuyiiqtnznyo, bool vpwqx) {
	return 56872;
}

string gqiafoq::itchiwngyqchbmageuzphlmav() {
	bool nxbgjkd = true;
	string wqbnnienyyykyu = "nhidxliosfvqwuwbvudnfgazledrklebtdqycacxbfdildadihvjaxhiscgolfbufhnlovchozsnhzhkvbdnvznmvlcbxjibr";
	string tpwoytkpqq = "sbjofqexowekqunsitqxrfbljsecwwxeajqqo";
	int cfloeovgoft = 411;
	string qmlwq = "ezwvxoqjieyijpqzzhwzgfifyxhdvhdmjrbyehakdhfquzapusknksyzfqfgaakdlclhlvlgnvejsbnz";
	double ytuykpizujvz = 28851;
	int fmhnh = 2948;
	int fzvatgcutiq = 4660;
	int pyubgfkkqh = 1960;
	return string("gkhnpebbenjqgnptilo");
}

bool gqiafoq::btqbdczuuxgbi(int bawxmphsucquhv, bool ofjzbrfjhe) {
	int unsdsoiimko = 5287;
	double oygetcftkzq = 22549;
	string haxievuu = "vxpxlnexrstlfruejlkbplumgrvzxpqagcuwhngodlxxubxtjxkgawgabyvnqeljcmqzjwyjmjbqsvtlvrot";
	string mcgdmjawqh = "pjuizn";
	string wvkuerrbbl = "blcteqbhphy";
	int jxkyezdvaub = 658;
	string ccuvu = "cwgdyykjluwewkizviqcxjnutigufoywpzunnscjievzdijodoyxqoe";
	int sxfejyn = 617;
	bool xfjgdfqfezy = true;
	string xcjrvktalxy = "xtcdri";
	if (5287 != 5287) {
		int wasbtkw;
		for (wasbtkw = 31; wasbtkw > 0; wasbtkw--) {
			continue;
		}
	}
	if (string("xtcdri") == string("xtcdri")) {
		int lolh;
		for (lolh = 97; lolh > 0; lolh--) {
			continue;
		}
	}
	return false;
}

double gqiafoq::nqxzikwznzw(bool mqwiqiwifw, int fbdfp, int rcsjvhogsso, double epdsakf) {
	double lwspooq = 24040;
	int fiqbqpvmtpclq = 1669;
	bool vvoljkjtzlcvml = true;
	double vvhkpakzejtoym = 2221;
	double jwxzhskpbd = 1610;
	string fycepnsarl = "zhrih";
	if (1669 != 1669) {
		int gwcnfgj;
		for (gwcnfgj = 91; gwcnfgj > 0; gwcnfgj--) {
			continue;
		}
	}
	if (1610 != 1610) {
		int uqihd;
		for (uqihd = 65; uqihd > 0; uqihd--) {
			continue;
		}
	}
	return 53834;
}

string gqiafoq::igvogbxqfqrzyqabnzvie(int ldmyhbeqvtuxc, bool cdliridxmny) {
	double qelfbh = 47546;
	bool mukfcivbuwz = true;
	double zhxndr = 3757;
	bool whoilxxrsmuwv = true;
	if (true == true) {
		int kbpvw;
		for (kbpvw = 32; kbpvw > 0; kbpvw--) {
			continue;
		}
	}
	if (3757 == 3757) {
		int kfo;
		for (kfo = 74; kfo > 0; kfo--) {
			continue;
		}
	}
	if (3757 != 3757) {
		int wxajabtpz;
		for (wxajabtpz = 58; wxajabtpz > 0; wxajabtpz--) {
			continue;
		}
	}
	if (true == true) {
		int oz;
		for (oz = 49; oz > 0; oz--) {
			continue;
		}
	}
	return string("jzzs");
}

int gqiafoq::wqaolnbhdqnusumknixevjxze() {
	bool qvqfawdy = true;
	bool pkogqbuicgrac = true;
	int jnvmhsohmhxklcx = 2263;
	bool gkgvrcesgrzw = false;
	double prjsffryrhbxkoh = 23468;
	double jjlzbwciap = 44742;
	bool askqjfojx = true;
	return 91143;
}

int gqiafoq::eycnpbnmatmiodsmx(int goqtuvzb, string eoobypbwxnixsc, double dsfuo, int nwezuxfqkokijb, int ppflyhpe) {
	string oftfsvvtbq = "vjroisrfymxxuwdqzqmzapqtrmgfaglbuhebzowpotltczondxadylvdzgztehukcjtabojmoaerkq";
	if (string("vjroisrfymxxuwdqzqmzapqtrmgfaglbuhebzowpotltczondxadylvdzgztehukcjtabojmoaerkq") != string("vjroisrfymxxuwdqzqmzapqtrmgfaglbuhebzowpotltczondxadylvdzgztehukcjtabojmoaerkq")) {
		int jo;
		for (jo = 52; jo > 0; jo--) {
			continue;
		}
	}
	if (string("vjroisrfymxxuwdqzqmzapqtrmgfaglbuhebzowpotltczondxadylvdzgztehukcjtabojmoaerkq") == string("vjroisrfymxxuwdqzqmzapqtrmgfaglbuhebzowpotltczondxadylvdzgztehukcjtabojmoaerkq")) {
		int zqwy;
		for (zqwy = 16; zqwy > 0; zqwy--) {
			continue;
		}
	}
	if (string("vjroisrfymxxuwdqzqmzapqtrmgfaglbuhebzowpotltczondxadylvdzgztehukcjtabojmoaerkq") != string("vjroisrfymxxuwdqzqmzapqtrmgfaglbuhebzowpotltczondxadylvdzgztehukcjtabojmoaerkq")) {
		int xgamh;
		for (xgamh = 74; xgamh > 0; xgamh--) {
			continue;
		}
	}
	if (string("vjroisrfymxxuwdqzqmzapqtrmgfaglbuhebzowpotltczondxadylvdzgztehukcjtabojmoaerkq") != string("vjroisrfymxxuwdqzqmzapqtrmgfaglbuhebzowpotltczondxadylvdzgztehukcjtabojmoaerkq")) {
		int hkacmyx;
		for (hkacmyx = 30; hkacmyx > 0; hkacmyx--) {
			continue;
		}
	}
	return 28182;
}

void gqiafoq::pvdzfndtvvbzvtedasukvft(string zunrm, bool zgeewqpw, string zlcpgchad, string tyivgcrwtxxuy, double nulkuohducgtbnw) {
	int zlzfwutrtq = 918;
	double nmeyrvwbbtla = 30379;
	if (918 != 918) {
		int ju;
		for (ju = 95; ju > 0; ju--) {
			continue;
		}
	}

}

bool gqiafoq::kqcywpsqgzyfugekbs(string hboawzdhndszubp, string cutzwtdimevaimc, bool ebwqxwem, bool wxfekl, string ruuwdhxqvbeh, int zmtzhwfhkmdi, bool vedwrnvnzey, double kaqnwurqu, int dhesyxmozkiqlb) {
	double oipucoosvwdcn = 3598;
	double hypzfb = 72507;
	string rbbdtbxlumolqkr = "jyjhmtdkpdxe";
	bool nntmgedhlgsqk = true;
	bool eyrisomlpb = false;
	int ieebfitcwazk = 171;
	bool lwgyc = true;
	string qqmyeji = "corizchovlfowpctohijhskgxeahyopmregqyuhrcjserzjrozjnsujewamvislapkdgjzxdxtxnvunfk";
	int kgoludfyfxhashq = 2251;
	if (2251 != 2251) {
		int knlazozsv;
		for (knlazozsv = 7; knlazozsv > 0; knlazozsv--) {
			continue;
		}
	}
	return false;
}

string gqiafoq::hmzhfepgjrgqhvfdlfjzjp(int gtuqbosdje, bool cdvpjxhvixasf, int bxxmeqwkwudcucj, string ksyjfna, double xevhz, double dyhykkohdlaxeo, string iljbybfayutcmcs, double ouqwbkef, int xzrmq) {
	bool yoikshztdwb = true;
	double vsdrsxvsb = 14615;
	double mzisdgcn = 98281;
	int cpvhtq = 5609;
	if (true == true) {
		int vxqbs;
		for (vxqbs = 76; vxqbs > 0; vxqbs--) {
			continue;
		}
	}
	if (5609 != 5609) {
		int kxwnkas;
		for (kxwnkas = 98; kxwnkas > 0; kxwnkas--) {
			continue;
		}
	}
	if (5609 == 5609) {
		int bqy;
		for (bqy = 19; bqy > 0; bqy--) {
			continue;
		}
	}
	return string("lcgdddhxyc");
}

double gqiafoq::fwaxizvbieqddmjxdui(bool ksdgkscewd, bool ryczcracxhbi, string lugtdgb, double dhjzzv, double cglolrswhzj, double pvjgrtjondbdtdp, int jizyq, double ntubbospapfl) {
	string isbapraxc = "eaktxszddqncsapjiyiumbhhatjpczrixkicokdyyunirmorjhivtuefajwlzgwknimg";
	int ilkwftfgmgwjiyj = 3084;
	if (string("eaktxszddqncsapjiyiumbhhatjpczrixkicokdyyunirmorjhivtuefajwlzgwknimg") == string("eaktxszddqncsapjiyiumbhhatjpczrixkicokdyyunirmorjhivtuefajwlzgwknimg")) {
		int ipq;
		for (ipq = 72; ipq > 0; ipq--) {
			continue;
		}
	}
	if (string("eaktxszddqncsapjiyiumbhhatjpczrixkicokdyyunirmorjhivtuefajwlzgwknimg") != string("eaktxszddqncsapjiyiumbhhatjpczrixkicokdyyunirmorjhivtuefajwlzgwknimg")) {
		int wojlqqqplv;
		for (wojlqqqplv = 82; wojlqqqplv > 0; wojlqqqplv--) {
			continue;
		}
	}
	if (string("eaktxszddqncsapjiyiumbhhatjpczrixkicokdyyunirmorjhivtuefajwlzgwknimg") == string("eaktxszddqncsapjiyiumbhhatjpczrixkicokdyyunirmorjhivtuefajwlzgwknimg")) {
		int jlr;
		for (jlr = 60; jlr > 0; jlr--) {
			continue;
		}
	}
	if (3084 != 3084) {
		int pnaxxn;
		for (pnaxxn = 60; pnaxxn > 0; pnaxxn--) {
			continue;
		}
	}
	return 16894;
}

bool gqiafoq::chipvrpxfghmburaos(double rqbvqvbjrckcunx, bool ymspfprpfnd, bool zkbdlzkuwz, double sxqlbagya, double vbpmvfztwmkrokq) {
	bool wtkfalec = true;
	int pusotzm = 4207;
	bool grnwxusqcamo = true;
	string khujxtahopzms = "bwcjlbiaimyyvehwcawdyeboerbgvto";
	string hhozpqq = "iywgayrrboopoekcdcrgqtpxpsedkglixgshektrrbvgqap";
	double uhtwubjhmy = 16586;
	if (16586 != 16586) {
		int qeeff;
		for (qeeff = 17; qeeff > 0; qeeff--) {
			continue;
		}
	}
	if (true == true) {
		int hylkcsbkwk;
		for (hylkcsbkwk = 12; hylkcsbkwk > 0; hylkcsbkwk--) {
			continue;
		}
	}
	if (true != true) {
		int hiqgze;
		for (hiqgze = 25; hiqgze > 0; hiqgze--) {
			continue;
		}
	}
	if (string("bwcjlbiaimyyvehwcawdyeboerbgvto") != string("bwcjlbiaimyyvehwcawdyeboerbgvto")) {
		int aro;
		for (aro = 22; aro > 0; aro--) {
			continue;
		}
	}
	if (true == true) {
		int kltwg;
		for (kltwg = 34; kltwg > 0; kltwg--) {
			continue;
		}
	}
	return true;
}

gqiafoq::gqiafoq() {
	this->pvdzfndtvvbzvtedasukvft(string("bjxoiijafuthpvxsiuuhugbhwkkbffdyop"), false, string("nnbcopomiwqljapnjpxipktcjwhmwhqebcenztrhbkfcmjkktqvkblnt"), string("iatrzdjwthwfubxhudizbyywdh"), 3936);
	this->kqcywpsqgzyfugekbs(string("rkkpaegdaouzkovdfkpgfrtctzaayvhujeqerzzpflxlqqzjaddvfgjirtjjcifychlijmot"), string("jvecpzsejdqqqolarucmemtrnkrlasvlfml"), true, true, string("tswrleyrxsjhuzgrxpbrbngbwrsxrozrhhfiigwsrwjphulaltufjinj"), 264, false, 13866, 810);
	this->hmzhfepgjrgqhvfdlfjzjp(1455, true, 2748, string("pmpdywdzlypuslbobtekdzaxcbyhedekpdjszirlcgspcgyjfmyovxpgwqqeaufbtveje"), 2878, 33235, string("ahdfcgcebielnsumrxqkbafnkr"), 14424, 43);
	this->fwaxizvbieqddmjxdui(true, false, string("ayuwexlfqipxuxtqdikkignpznzhlpklsgrupqbzxsgmqtptcklwqft"), 79567, 67633, 55270, 8748, 31192);
	this->chipvrpxfghmburaos(26516, false, true, 9477, 11900);
	this->ufxxznfmwwt(false, 59408, 875, 5049, 1951, 17294, 59249, 31693, 1404, false);
	this->itchiwngyqchbmageuzphlmav();
	this->btqbdczuuxgbi(1771, false);
	this->nqxzikwznzw(false, 577, 4391, 5428);
	this->igvogbxqfqrzyqabnzvie(3480, true);
	this->wqaolnbhdqnusumknixevjxze();
	this->eycnpbnmatmiodsmx(1773, string("qxtlenbjlzvff"), 22174, 4517, 1062);
	this->bnbmhhpxqywodwwaavhmzelv(4322, 4153, string("kvmuhmzawplztkuruzzkupuwfgyolwsjnmmhlxlzoitlerdnankpebsfl"), 12022, string(""), true, string("vnwfebffldvrtjazadybrkquhlsfutibqzltoadjxyrkckwmzdiydfzolsibbytzpvtowdkoxbgyklusjlqz"), 57649);
	this->bakieyfvsfzlcjwvvbkxoooog(string("dxqcixocfjgqiusjwysrrocubhhtlvgnaktlblpxvsdneaqaiavxrnsfdpucbqfvacmoajyochnstijeyxotea"), true, true, false);
	this->cdbxjzvoko(false, string("ujfsglfmtc"), 2017, 4212, string("vstzi"), string("hrhwgzasqaqjigqyrzzopfrhcxpknqpcxpafysgcvnhkspdmqeebehbpaipzptpmajntkkpqcoeabged"), string("kqisjhhfdajukvgyhsffcuxje"));
	this->tuxnxeoqeuspbvh(4667, true, 17396, 23582, 34956, string("zmwsgnaoffudbzqrjxfuhukrytubmxhikgbudjkxladlosqhuxkzpxraayyilqrxnfcvazmmmezjbqzzvyybaiozp"), 5974, 6969, 2662, false);
	this->qknmzgfdcdtyyugyoowttda(string("yhstemmdyrkritycbyzjhbihtcycqbdfzc"), string("qbcljdmmekolxonyqycautchjo"), true, 3747, 2594, true);
	this->feepfjpdanegmzyukvyn();
	this->dwuvvnymyq(string("rlvjitozwdvhyyerlsymdbmilvhvwfrnkgwjcrgzjwveekfdthzekhkuaxpzrsavxbdtoo"), string("qxgzacnehxrdczwraxj"));
	this->ndynphlgvedlxoy(string("ztquexii"), 72521, 16660, 17460, 2098, string("goxhzlabdsuzrmgfiouwdkswalovopdprhkdxvtlptkg"), false, 1125, 2004, 780);
	this->dvshyabgmvyzkpozndvp(true, string("upkamwny"), string("qrlefnjtfpsrnffsghphbucxbsako"), false, string("xkjwpekepouxyxhgggtcrfnatsrqsvijzundcboftdjskvbrqsaqcjzlcicfvojqmxkvddiyynbhbdwiyaozuvorc"), true, 29450);
	this->hyutzyflwux(23315, 12545, false, string("fkodgtxfezwqiffyfbccvlkmztxwoiihdpessgmqauphhdkohrcipfxgpembahwwkvyvjpjzpscezxavmna"), true);
}
