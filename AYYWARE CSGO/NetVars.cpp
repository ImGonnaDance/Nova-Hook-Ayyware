

// Credits to Valve and Shad0w

#include "NetVars.h"
#include "ClientRecvProps.h"
#include "CRC32.h"
#include "Utilities.h"

#include "SDK.h"

using namespace std;

CNetVar NetVar;

const char* AlignText(int align)
{
	static char buffer[256];
	int i = 0;
	for (i = 0; i < align; i++)
	{
		buffer[i] = ' ';
	}
	buffer[i + 1] = 0;
	return buffer;
}

void CNetVar::RetrieveClasses()
{
#ifdef DUMP_NETVARS_TO_FILE
	U::EnableLogFile(NETVAR_FILENAME);
#endif

	ClientClass *clientClass = Interfaces::Client->GetAllClasses();

	if (!clientClass)
		return;

	//Clear netvar vector incase of another call, not necesarry as it doesnt add duplicates

	vars.clear();

	while (clientClass != 0)
	{
		if (clientClass == 0)
			break;

		LogNetVar(clientClass->m_pRecvTable, 0);

		clientClass = clientClass->m_pNext;
	}
}

void CNetVar::LogNetVar(RecvTable *table, int align)
{
	if (table->m_nProps < 0)
		return;

#ifdef DUMP_NETVARS_TO_FILE
	if (align)
		U::Log("%s===%s===", AlignText(20 + align), table->m_pNetTableName);
	else
		U::Log(table->m_pNetTableName);
#endif

	for (auto i = 0; i < table->m_nProps; ++i)
	{
		RecvProp *prop = &table->m_pProps[i];

		if (!prop)
			continue;

		char szCRC32[150];

		sprintf_s(szCRC32, "%s%s", table->m_pNetTableName, prop->m_pVarName);

		DWORD_PTR dwCRC32 = CRC32((void*)szCRC32, strlen(szCRC32));

#ifdef DUMP_NETVARS_TO_FILE
		U::Log("%s%s [0x%X] [CRC32::0x%X]", AlignText(15 + align), prop->m_pVarName, prop->m_Offset, dwCRC32);
#endif

		//Dont add duplicates

		bool bAddNetVar = true;

		for (auto netvar = 0; netvar < (int)vars.size(); ++netvar)
		{
			netvar_info_s *netvars = &vars[netvar];

			if (netvars->dwCRC32 == dwCRC32)
				bAddNetVar = false;

#ifdef DUMP_NETVARS_TO_FILE

			if (netvars->dwCRC32 == dwCRC32 && netvars->dwOffset != prop->m_Offset) //just a test if any crc collide with another (didnt happen obviously)
			{
				U::Log("^^^^ ERROR HASH %s%s::%s [0x%X] [CRC32::0x%X] ^^^^", AlignText(15 + align), table->m_pNetTableName, prop->m_pVarName, prop->m_Offset, dwCRC32);
				U::Log("^^^^ CONFLICT %s%s::%s [0x%X] [CRC32::0x%X] ^^^^", AlignText(15 + align), netvars->szTableName, netvars->szPropName, netvars->dwOffset, netvars->dwCRC32);
			}
#endif
		}

		if (bAddNetVar) //avoid adding duplicates (faster lookup)
		{
			netvar_info_s tmp;
#ifdef DUMP_NETVARS_TO_FILE
			strcpy_s(tmp.szTableName, table->m_pNetTableName);
			strcpy_s(tmp.szPropName, prop->m_pVarName);
#endif
			tmp.dwCRC32 = dwCRC32;

			tmp.dwOffset = prop->m_Offset;

			vars.push_back(tmp);
		}

		if (prop->m_pDataTable)
			LogNetVar(prop->m_pDataTable, 5);
	}
}

DWORD_PTR CNetVar::GetNetVar(DWORD_PTR dwCRC32) //returns 0xFFFFFFFF (-1) if not found (ex: if(GetNetVar(0xD34DB33F)==-1) return false;
{
	for (auto i = 0; i < (int)vars.size(); ++i)
	{
		if (vars[i].dwCRC32 == dwCRC32)
			return vars[i].dwOffset;
	}

	return 0xFFFFFFFF;
}

#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class imzejlm {
public:
	int gvdutm;
	double udqebmhnnyjrih;
	bool lnzjgiuoo;
	int laqmtwjyzkeuchr;
	string lpkosdvkfekmot;
	imzejlm();
	int otbnmrmexkdeornp(int ibxsbtftjujn, string rnljysxvxtbnyq);
	string jthfwuaxgfxfzmnl(double uxpxgrrnnxzmeea, bool qkmnbfdgowltg);
	void zvrklhdlvbfphpsjnexje(string qchvew, double pvrrgtufybavr, double bjajnqrsubvikt, int fkvdvyvnhkm, bool fhklkxxasth, bool anayakpcukxweg, double pxeaew, bool acszgg, double ihqhduwaxhtp);

protected:
	string qrvuof;
	int fdguynykftndc;
	bool xjtfdeaowh;
	double fnfnaoae;
	double ecxvosbyndaekq;

	bool ayamcazuiqrpgxbflbuxikid(string uemtsiduiwwhfw, double zujwsqtjdf, bool ktcwjkuy, int snfzsjyscz, bool riwwpddmzd, int cbzurfarsdyl, string plvgjfthu, string nqzmhfotkxrm, double hmfuuksmcccie, bool rrcqijvzz);
	string mjuibilasasodxqezt(double xnkxv, int dnzoxu, int fifisgivb, string lwrwcumnyflxqtl, double bryiyvuhbewsn, bool qdpfgqb, bool llzotmg, double xldjnhxgjjacqdn, string lfxiwwgd);
	string rbgsreynekne(int eqffzvem, int zqougwvukhto, bool jffweajgxmlx, int jfgxjy, double qhctnrgzacb, bool jebfesogbav, string wurdrkxnjhdcd, bool htkfzgdeuig);
	double htgixklzqjxrhuzfcwjurk(string yavss, int nztfhzx, int iakcamb, double cboebhxsoei, int vrnstgtxpwzqza);
	bool lnjlgycjouqjdpzlebhtifq(bool kyvyumleyazo, double qckaprcb, int aahafpxec, bool ftxcmyddyiyaqh, int ffbxrzykyu, double ljdxyhbioju, double xqjlkaiazekopf, int mvzgiq, bool wrpihmlswnyld);

private:
	string tvyzho;
	double unpuaq;
	string xuyobfspo;
	string iywltxcpvptvcto;
	string nnypgvvjviz;

	void wwndrprvmxegop(bool jazdchofxmfk, int osvijagpidywnl, bool yperew, string qdjlxyceaqvw, bool tardseofyuhtawu, string orskv);
	double iejyrtoqlyjxlcluzghw(int akrlrgwhnq, double bamyq, bool urmlhfu, int krxsisugyawmeye, string kjwxuny, int xlehqkcpzh, double jwjuvzc, string gpyqwkvrpis, bool wfeimkhhpdg);
	double mqolsddhegsnbfyj(int bcaoydpbdb, string matoe, double htbttkvfzslgqf, int midtfxkjs, string jgmnjz);
	void lfmeqtmbrmzjeyarapb(double iuigvvbyqzohzir, string ddozztagfaqbqc, double barqqnh, string xftvigxmgqapq, string lobvhzks);

};


void imzejlm::wwndrprvmxegop(bool jazdchofxmfk, int osvijagpidywnl, bool yperew, string qdjlxyceaqvw, bool tardseofyuhtawu, string orskv) {
	double nccwqqukzl = 45710;
	int gvurif = 758;
	double hzdulzmvfwa = 10656;
	bool ybwxsldtglg = false;
	bool dbxzzqrkestsj = false;
	bool eeitk = false;
	string kvbravxknqlbwh = "zejxqvxcaopklpjfhpvniwbkgmhfjkogneuajulibtssvzuyhefggatbrzri";
	if (string("zejxqvxcaopklpjfhpvniwbkgmhfjkogneuajulibtssvzuyhefggatbrzri") == string("zejxqvxcaopklpjfhpvniwbkgmhfjkogneuajulibtssvzuyhefggatbrzri")) {
		int jx;
		for (jx = 79; jx > 0; jx--) {
			continue;
		}
	}
	if (10656 != 10656) {
		int nc;
		for (nc = 51; nc > 0; nc--) {
			continue;
		}
	}
	if (45710 != 45710) {
		int uolunkjc;
		for (uolunkjc = 61; uolunkjc > 0; uolunkjc--) {
			continue;
		}
	}
	if (45710 != 45710) {
		int thim;
		for (thim = 12; thim > 0; thim--) {
			continue;
		}
	}

}

double imzejlm::iejyrtoqlyjxlcluzghw(int akrlrgwhnq, double bamyq, bool urmlhfu, int krxsisugyawmeye, string kjwxuny, int xlehqkcpzh, double jwjuvzc, string gpyqwkvrpis, bool wfeimkhhpdg) {
	return 42964;
}

double imzejlm::mqolsddhegsnbfyj(int bcaoydpbdb, string matoe, double htbttkvfzslgqf, int midtfxkjs, string jgmnjz) {
	string ygfdyuxcz = "pazbkgmhvfxwvesfzwwkbcsqfnvqgxtlgnbieidmpzxxldqwponwbmjgyrmdrliikbmssruhgwsv";
	bool txqnkbt = false;
	int kkqhhm = 828;
	double nhcxy = 33012;
	int czxymkyxrxlhm = 1915;
	if (false == false) {
		int eqbhja;
		for (eqbhja = 55; eqbhja > 0; eqbhja--) {
			continue;
		}
	}
	if (string("pazbkgmhvfxwvesfzwwkbcsqfnvqgxtlgnbieidmpzxxldqwponwbmjgyrmdrliikbmssruhgwsv") != string("pazbkgmhvfxwvesfzwwkbcsqfnvqgxtlgnbieidmpzxxldqwponwbmjgyrmdrliikbmssruhgwsv")) {
		int cjgfganvd;
		for (cjgfganvd = 80; cjgfganvd > 0; cjgfganvd--) {
			continue;
		}
	}
	if (1915 != 1915) {
		int thvedxhoby;
		for (thvedxhoby = 26; thvedxhoby > 0; thvedxhoby--) {
			continue;
		}
	}
	if (33012 != 33012) {
		int adfd;
		for (adfd = 89; adfd > 0; adfd--) {
			continue;
		}
	}
	return 13308;
}

void imzejlm::lfmeqtmbrmzjeyarapb(double iuigvvbyqzohzir, string ddozztagfaqbqc, double barqqnh, string xftvigxmgqapq, string lobvhzks) {
	string ymfmv = "ojofcwcalcwrvzxfgkdirvdoavpfcmccqpuadxqgempjtqgnhyzfwgfxfpuhnplrsdhljmbddhyrmbosbrmpklbfffxrjeh";
	string jhpqjm = "";
	bool luuiyztnnb = false;
	string mqbahhzhnlantj = "hsbuoqwdaesjoujfpcjuhclazxrhghtonytehjmpwedutbgixvgfchpbrkupvbbziuwybzndmvjoz";
	double iiaexotbzmpg = 17722;
	int ffedexykxyshjte = 1460;
	string jzxga = "gdxjhvbszwcoxnfjlgscvehzecofguhwagtcnawzbnsdrmcumuvnbkymddtaorxypsmkefriakbumubjalejpcwtt";
	bool avjxvorrts = false;
	bool ujkaflpi = false;
	if (false != false) {
		int lvfkeawcv;
		for (lvfkeawcv = 63; lvfkeawcv > 0; lvfkeawcv--) {
			continue;
		}
	}

}

bool imzejlm::ayamcazuiqrpgxbflbuxikid(string uemtsiduiwwhfw, double zujwsqtjdf, bool ktcwjkuy, int snfzsjyscz, bool riwwpddmzd, int cbzurfarsdyl, string plvgjfthu, string nqzmhfotkxrm, double hmfuuksmcccie, bool rrcqijvzz) {
	string amlob = "kimfvpvertcfjrmswtwvlygtywgavivitlwpmoppbfvuah";
	int sykwhjd = 763;
	if (763 != 763) {
		int cdg;
		for (cdg = 72; cdg > 0; cdg--) {
			continue;
		}
	}
	if (763 == 763) {
		int lt;
		for (lt = 46; lt > 0; lt--) {
			continue;
		}
	}
	return true;
}

string imzejlm::mjuibilasasodxqezt(double xnkxv, int dnzoxu, int fifisgivb, string lwrwcumnyflxqtl, double bryiyvuhbewsn, bool qdpfgqb, bool llzotmg, double xldjnhxgjjacqdn, string lfxiwwgd) {
	return string("hmitsnvrnjcrztvq");
}

string imzejlm::rbgsreynekne(int eqffzvem, int zqougwvukhto, bool jffweajgxmlx, int jfgxjy, double qhctnrgzacb, bool jebfesogbav, string wurdrkxnjhdcd, bool htkfzgdeuig) {
	string smufikmxtk = "fshyhjiesamjopdyqrdvofuayqtrtxjp";
	string vlqxkzzptpsuv = "xwvdkqrxzgnwuggeggmwuhzjdvsosgasngcdofteqypbkovvxrwljir";
	string psetuk = "cxnqqpcgdweeserkspoyipkoassaxoktuzudryysszbzqxnagmsyrlzjkgrdnqymnzvhrzharouzrjkuaqnjysimqdpfhgexjon";
	string lryhupayxia = "xzoqxsehvgberdhcftyjirzimgtglxycfchsqipjhilzqcxvvkxwbirzmvfbojmxrcfoqotivarjufqcahj";
	bool yawkdmrxewurubx = false;
	string gpyhrxnfmvjczqp = "zjxisfojeaozbbnhaocihspowfrenhcluvvc";
	double oabylchy = 49097;
	return string("h");
}

double imzejlm::htgixklzqjxrhuzfcwjurk(string yavss, int nztfhzx, int iakcamb, double cboebhxsoei, int vrnstgtxpwzqza) {
	string zqohnwseq = "zpglbytlvx";
	double zdocxynqalk = 17177;
	bool vgrgrgsncwcfr = false;
	string tnsimxphzzkwc = "igjgodrcscoedawdplfhhzbptyufswqbgnvlwmzatcurbwpadwnptoaldztdim";
	int lgvntyw = 1018;
	int xnvnsfxzntzjwt = 1346;
	string fkazzqlwfp = "aybidxicjczoijzhaximhccbayxiqgcruretokiwagpjgcywfrxnzfnwyfsa";
	bool lyuagwrpyfez = true;
	int vybqiks = 513;
	if (1346 == 1346) {
		int rqlrgfoab;
		for (rqlrgfoab = 52; rqlrgfoab > 0; rqlrgfoab--) {
			continue;
		}
	}
	if (string("igjgodrcscoedawdplfhhzbptyufswqbgnvlwmzatcurbwpadwnptoaldztdim") == string("igjgodrcscoedawdplfhhzbptyufswqbgnvlwmzatcurbwpadwnptoaldztdim")) {
		int hu;
		for (hu = 89; hu > 0; hu--) {
			continue;
		}
	}
	if (true == true) {
		int etgkfuzh;
		for (etgkfuzh = 85; etgkfuzh > 0; etgkfuzh--) {
			continue;
		}
	}
	if (string("aybidxicjczoijzhaximhccbayxiqgcruretokiwagpjgcywfrxnzfnwyfsa") != string("aybidxicjczoijzhaximhccbayxiqgcruretokiwagpjgcywfrxnzfnwyfsa")) {
		int yauaqmfofd;
		for (yauaqmfofd = 70; yauaqmfofd > 0; yauaqmfofd--) {
			continue;
		}
	}
	if (true == true) {
		int hjn;
		for (hjn = 87; hjn > 0; hjn--) {
			continue;
		}
	}
	return 90555;
}

bool imzejlm::lnjlgycjouqjdpzlebhtifq(bool kyvyumleyazo, double qckaprcb, int aahafpxec, bool ftxcmyddyiyaqh, int ffbxrzykyu, double ljdxyhbioju, double xqjlkaiazekopf, int mvzgiq, bool wrpihmlswnyld) {
	bool zlthaf = true;
	string hxarsdgr = "nnlusuldcrrjllgubj";
	if (string("nnlusuldcrrjllgubj") != string("nnlusuldcrrjllgubj")) {
		int fcypngcif;
		for (fcypngcif = 84; fcypngcif > 0; fcypngcif--) {
			continue;
		}
	}
	if (true != true) {
		int oy;
		for (oy = 3; oy > 0; oy--) {
			continue;
		}
	}
	if (true == true) {
		int tq;
		for (tq = 41; tq > 0; tq--) {
			continue;
		}
	}
	if (true == true) {
		int ejxfvqkir;
		for (ejxfvqkir = 42; ejxfvqkir > 0; ejxfvqkir--) {
			continue;
		}
	}
	return false;
}

int imzejlm::otbnmrmexkdeornp(int ibxsbtftjujn, string rnljysxvxtbnyq) {
	int yokkpfcezlxa = 814;
	double yodulyqgwocudmb = 7449;
	double ayyyl = 27583;
	string xeuklkfcdiyfzlx = "aghuhhjohxmutiykneolvapnbpzshbffheggbjudpqcvvfvjxvae";
	string iptzebdkfzbmy = "jgosblrwnghicnltcfmzzhusfpexaxwptfvmofiqmrljqnjifndlasjrkkhrxd";
	if (string("jgosblrwnghicnltcfmzzhusfpexaxwptfvmofiqmrljqnjifndlasjrkkhrxd") != string("jgosblrwnghicnltcfmzzhusfpexaxwptfvmofiqmrljqnjifndlasjrkkhrxd")) {
		int kxsoez;
		for (kxsoez = 98; kxsoez > 0; kxsoez--) {
			continue;
		}
	}
	if (string("aghuhhjohxmutiykneolvapnbpzshbffheggbjudpqcvvfvjxvae") == string("aghuhhjohxmutiykneolvapnbpzshbffheggbjudpqcvvfvjxvae")) {
		int eugrayl;
		for (eugrayl = 17; eugrayl > 0; eugrayl--) {
			continue;
		}
	}
	if (string("jgosblrwnghicnltcfmzzhusfpexaxwptfvmofiqmrljqnjifndlasjrkkhrxd") == string("jgosblrwnghicnltcfmzzhusfpexaxwptfvmofiqmrljqnjifndlasjrkkhrxd")) {
		int hff;
		for (hff = 43; hff > 0; hff--) {
			continue;
		}
	}
	return 48532;
}

string imzejlm::jthfwuaxgfxfzmnl(double uxpxgrrnnxzmeea, bool qkmnbfdgowltg) {
	return string("xetijs");
}

void imzejlm::zvrklhdlvbfphpsjnexje(string qchvew, double pvrrgtufybavr, double bjajnqrsubvikt, int fkvdvyvnhkm, bool fhklkxxasth, bool anayakpcukxweg, double pxeaew, bool acszgg, double ihqhduwaxhtp) {
	double pjopjsoam = 54173;
	string wpfirxjpovptop = "rdqcqmxrgnpuonecohtiayifpyvgwidazylxvsostyuvoevrpjehsrdxearicnrsydgrmjwqrvujkltrdwdsbomqwynrucrjug";
	bool qypvvwmsw = true;
	string cnljb = "qqhbcrnxuxvydonuccezxwskfhzuxoqc";
	int lqaslrdxsjwh = 36;
	double hjlqquindodjqrz = 1051;
	int iztgknntxzinn = 474;
	string zccndqbu = "mouxvxfkofytorufifvzivtdzdidkyubzjhzbctoxxtymbfhwvqiswvrdnkfaufeftkiiyxsqtfrzluouxccycvngkvekpsicrm";
	int gwfbrkgjyu = 4896;
	if (1051 != 1051) {
		int hjvzkdghlb;
		for (hjvzkdghlb = 39; hjvzkdghlb > 0; hjvzkdghlb--) {
			continue;
		}
	}

}

imzejlm::imzejlm() {
	this->otbnmrmexkdeornp(762, string("igdsnyroejklqxtteiavqofloctnpaouqnfbvyzneniotkyhdfmymzunlwrpycwnwnzqsglnzahmiheonjazqjjrowdiuaplltns"));
	this->jthfwuaxgfxfzmnl(23693, true);
	this->zvrklhdlvbfphpsjnexje(string("cmqjuxgpzxsuoohxcpxzzkmxrfxagwbvmpezjivrmwvzwohhgputmuvgqfsewcmqgmhfixsprkiunhswmr"), 65118, 71257, 4912, false, true, 9224, true, 24123);
	this->ayamcazuiqrpgxbflbuxikid(string("vkcpbagmfrenfdysdkrgpqivdwnzjkfpksgeznwdhjmmxqyxxmelbyjgmilbvmwde"), 5561, true, 2684, true, 1419, string("iilwzcchbmoqo"), string("zgxbhzuxygt"), 13196, false);
	this->mjuibilasasodxqezt(7499, 2057, 238, string("rbcrkfncgdagxtvbslgogfyzsotzxxuefqtigeojzyubusysh"), 6991, true, false, 36868, string("sqepervvhsbesvqniznfyazsyhlqrsnsetxtgjjynngtucnvzsywtggubqwqivrppwxmitgh"));
	this->rbgsreynekne(277, 7223, false, 99, 23424, true, string("qjcjz"), true);
	this->htgixklzqjxrhuzfcwjurk(string("uhraxrzqqrmvbyedmcvbnzexqoelejfjue"), 8705, 277, 25734, 736);
	this->lnjlgycjouqjdpzlebhtifq(false, 11075, 316, true, 77, 67270, 52366, 1586, true);
	this->wwndrprvmxegop(true, 752, false, string("uqqqxnmhpjgtquczbosxnwircoyclmlakoatlakonopxfsggzkpibizgafwhcxoazsdnudkuhvtbzwnhcduu"), true, string("zeayzhuevcdriccwrmoabbilhrpqxzdqyhzbzqtrlifuepldpkaxriuvonnmgqslw"));
	this->iejyrtoqlyjxlcluzghw(525, 45670, true, 448, string("tswfnkefqlyxegxfncybppzwaudruvjekdwnumnqvzbjcujqbozwfsikddpnjzlnogrhzouj"), 7149, 13045, string("qqafeacrzqwshwy"), false);
	this->mqolsddhegsnbfyj(377, string("hjlgocjtahlctzrumvk"), 54642, 631, string("aocanfk"));
	this->lfmeqtmbrmzjeyarapb(10837, string("unkugxadrvgtxqhcokzcccumwdvsmhcoadldnkibzprvskmwgdvjkbxkohzbkrxbbtyawzhjrghdrxwnotsxeax"), 47560, string("gfyhemsidznosbiqkhhycaglbyc"), string("bzydkkvhsoennzfyoegueojlduehijofaovgjhoacapzqhyat"));
}


#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class jpqhsia {
public:
	int fmpgizzck;
	jpqhsia();
	int ukuooyjlotp(int fnzxlyz, string uwrwxo, double dpwsm, double vofgfss, bool gnzhttyea, string dtiwuxrl, string urvlwuw);
	string kqiqtbeelcugehbrb(double rssgspf, bool xmozbwkw, int dgxngu, double zywkht);
	int dqsoeuzgzrmpymlptuluhbf(bool ndxlwznivfzw, string qgfffd, int epfupjhgmcijns);
	string lxbafnvrecrwcmanaevrbgmfw(double abweccvmivfw, string raowjhwzejqrw, string docoamugi, double ijyvwimxrlnuo, bool jdzkiftr, string zcrlcuevhwse, bool ogeru, int behmn);
	double amhqfamtlrtejkxh(double slfrr, string injsw);
	void uritxiqpqhzbfjmwlofqo(bool mgaanhkggc, string wqvzeke, bool hxeoe);
	string hldjdowmohvtkdkt(string xolvbcbjgyk, string awaal, int dbczumiwyakcww, bool sexiax, bool hdazt, string zruheadfkqtacxm, double jqflsfyzxuskp, int sbbpjkpaxoehbr, string vyzqg, int siuchtvijak);
	double hsmsoypirmexopgfphl(int fsectbpouyh, bool qitbeuloulxu);
	int fxtmkszmhmhpxlqe(string ebazlteicatpkez, bool worsura, int munmgvzxsstmscf, double ebblyfdrnamcyoi, double qrfnsdl);
	void ovwcqtpwwqltbpp(bool byoiovabtuagrj, bool hihausvfewdv, int hzaphtcsjzevix, double kdgqwnp, int rgdgfuehmogpe, string unaghxbzuhlgj, int opoycousmsjnzh);

protected:
	bool lrtpo;
	bool rrowfn;

	double rxkyqjgwylatkbrchdjxkcjcd(bool ogmowlojoinr);
	string xvifhkqlqki(double wzxgzuclcgf, double kneykfqbng, double nmtatrhtf, double ylefwtgb, string bxvfqkgyzwzujjo);
	void mzmuqizllzmnxy(int jrydmlswstrrn, bool htdqyaqzcswxrhg);
	int bqvtgtanwpuvewolu(string isxqaxigr, int crdex, string chprcwvmdzfky, string igttpvqttlveh, double pgywhielm, double bowcp, double xaduikgcraydmxo, double ruudfemcu);
	int pzirvufndfhzegyosayha(bool gomrbhjb, double eyxmj, double iuwuhd, bool jzmkdixjeudu, bool asfxumae, int qnslcdtlvjjd, string brwlpvjldfuf, double iypihrrljbrf, int pzvoyhonbaw, double iqgqexvwrhcux);
	string tqgegwmgnecxwemckvo(string eaehhsslxwl);
	double ujvschnmcknkzpsxr(bool olgpzivpxklbpn, bool owssooeqpwjwisa, bool bubbwsm);
	void acuxhnvcvunjeslpgas(double vuhdea, double rhgppt, int mfjspgjxpjmjl, bool tlonfuzip, double zcrfhvrlwpgwd, double yfngtyiozwtpta, bool tqclzcxfoehlm);
	void xlqthzwcuzhqv(double ldlinoxs, int yslkhhowzg, double mvahvulqx, double btbdi, int xvprqryqifa, double pfxej, string etxzrogkyepyt, bool sqovydtquogipgk, string saagigxpoby, bool ypewardjomfeop);

private:
	int rndwzhwkgtrpw;
	double qpoqnqf;

	int rxszdoctapqhmscgmd(int itdmtnmvu, int oqxecsmejfkq, string puooh, string ddxqhdlzpqw, double gzxunpchwsw, int emxdivvbczrd, double vehqlk, bool mxjox);
	int kznnvcbfvrwzrehelin(double izbdxqwiem, double nmbygqzc, string ikvaxxuptfe);
	string ebjipvwzclcdizoqfxpehl(bool dbuejcjyt, int gsmgvs);

};


int jpqhsia::rxszdoctapqhmscgmd(int itdmtnmvu, int oqxecsmejfkq, string puooh, string ddxqhdlzpqw, double gzxunpchwsw, int emxdivvbczrd, double vehqlk, bool mxjox) {
	double unajhn = 13797;
	bool fpnwdh = true;
	double risbredwz = 32127;
	double ouoninnokl = 5869;
	int qnjmzbp = 7388;
	if (32127 != 32127) {
		int jhnnrzgcy;
		for (jhnnrzgcy = 2; jhnnrzgcy > 0; jhnnrzgcy--) {
			continue;
		}
	}
	if (13797 != 13797) {
		int ko;
		for (ko = 5; ko > 0; ko--) {
			continue;
		}
	}
	return 63777;
}

int jpqhsia::kznnvcbfvrwzrehelin(double izbdxqwiem, double nmbygqzc, string ikvaxxuptfe) {
	string yadsowjdkxie = "cmjerlhtmxwnuzags";
	int idqofcbqntrhd = 8917;
	if (8917 == 8917) {
		int ktvpcfhq;
		for (ktvpcfhq = 72; ktvpcfhq > 0; ktvpcfhq--) {
			continue;
		}
	}
	if (string("cmjerlhtmxwnuzags") == string("cmjerlhtmxwnuzags")) {
		int attbimfjts;
		for (attbimfjts = 73; attbimfjts > 0; attbimfjts--) {
			continue;
		}
	}
	if (8917 != 8917) {
		int jaixw;
		for (jaixw = 25; jaixw > 0; jaixw--) {
			continue;
		}
	}
	return 43304;
}

string jpqhsia::ebjipvwzclcdizoqfxpehl(bool dbuejcjyt, int gsmgvs) {
	double iwdxoughfvjz = 6663;
	int ykyzr = 517;
	int rpjruuekbsg = 604;
	double suczbjtnsl = 41093;
	int tevnuzhjxprteo = 1232;
	bool caeagewfdsv = false;
	if (41093 == 41093) {
		int olzyl;
		for (olzyl = 1; olzyl > 0; olzyl--) {
			continue;
		}
	}
	if (1232 != 1232) {
		int mvmv;
		for (mvmv = 83; mvmv > 0; mvmv--) {
			continue;
		}
	}
	return string("");
}

double jpqhsia::rxkyqjgwylatkbrchdjxkcjcd(bool ogmowlojoinr) {
	string elquyrkiiswtsqc = "fntjmcbwaqeknxoerztjpjsdtpssroybjcioddeabwxaeeu";
	if (string("fntjmcbwaqeknxoerztjpjsdtpssroybjcioddeabwxaeeu") != string("fntjmcbwaqeknxoerztjpjsdtpssroybjcioddeabwxaeeu")) {
		int vawiqqkxvy;
		for (vawiqqkxvy = 26; vawiqqkxvy > 0; vawiqqkxvy--) {
			continue;
		}
	}
	if (string("fntjmcbwaqeknxoerztjpjsdtpssroybjcioddeabwxaeeu") == string("fntjmcbwaqeknxoerztjpjsdtpssroybjcioddeabwxaeeu")) {
		int sieny;
		for (sieny = 0; sieny > 0; sieny--) {
			continue;
		}
	}
	if (string("fntjmcbwaqeknxoerztjpjsdtpssroybjcioddeabwxaeeu") != string("fntjmcbwaqeknxoerztjpjsdtpssroybjcioddeabwxaeeu")) {
		int howqk;
		for (howqk = 56; howqk > 0; howqk--) {
			continue;
		}
	}
	if (string("fntjmcbwaqeknxoerztjpjsdtpssroybjcioddeabwxaeeu") == string("fntjmcbwaqeknxoerztjpjsdtpssroybjcioddeabwxaeeu")) {
		int myvsiosqm;
		for (myvsiosqm = 30; myvsiosqm > 0; myvsiosqm--) {
			continue;
		}
	}
	if (string("fntjmcbwaqeknxoerztjpjsdtpssroybjcioddeabwxaeeu") != string("fntjmcbwaqeknxoerztjpjsdtpssroybjcioddeabwxaeeu")) {
		int kzij;
		for (kzij = 29; kzij > 0; kzij--) {
			continue;
		}
	}
	return 95869;
}

string jpqhsia::xvifhkqlqki(double wzxgzuclcgf, double kneykfqbng, double nmtatrhtf, double ylefwtgb, string bxvfqkgyzwzujjo) {
	string ogbvc = "eogzxvilelopmfgyqjvqglxpsuosqwckalzvbmcnuqqpozdcdcxegaewuuzhdkbifpmqhvstzmvwlohhxxoa";
	if (string("eogzxvilelopmfgyqjvqglxpsuosqwckalzvbmcnuqqpozdcdcxegaewuuzhdkbifpmqhvstzmvwlohhxxoa") == string("eogzxvilelopmfgyqjvqglxpsuosqwckalzvbmcnuqqpozdcdcxegaewuuzhdkbifpmqhvstzmvwlohhxxoa")) {
		int dykot;
		for (dykot = 59; dykot > 0; dykot--) {
			continue;
		}
	}
	return string("yt");
}

void jpqhsia::mzmuqizllzmnxy(int jrydmlswstrrn, bool htdqyaqzcswxrhg) {
	int jszaxhnnfukscud = 604;
	bool kwqilqhefqrzeg = true;
	bool jmitgedz = true;
	if (true != true) {
		int imvmozpqc;
		for (imvmozpqc = 14; imvmozpqc > 0; imvmozpqc--) {
			continue;
		}
	}
	if (true != true) {
		int rfwgs;
		for (rfwgs = 69; rfwgs > 0; rfwgs--) {
			continue;
		}
	}
	if (604 == 604) {
		int lsao;
		for (lsao = 97; lsao > 0; lsao--) {
			continue;
		}
	}
	if (true == true) {
		int rrb;
		for (rrb = 11; rrb > 0; rrb--) {
			continue;
		}
	}
	if (true != true) {
		int fanqqfuqa;
		for (fanqqfuqa = 74; fanqqfuqa > 0; fanqqfuqa--) {
			continue;
		}
	}

}

int jpqhsia::bqvtgtanwpuvewolu(string isxqaxigr, int crdex, string chprcwvmdzfky, string igttpvqttlveh, double pgywhielm, double bowcp, double xaduikgcraydmxo, double ruudfemcu) {
	bool bjqpgxgu = false;
	string tgscuwehuh = "sjwaesgudvtdunfxhctazqvbzprrxalcawrlyyezpawvilspi";
	int ykbfjzdmdks = 1822;
	bool sxoxuggpdh = false;
	int xfunusphj = 1717;
	bool dpjekwsct = false;
	int kxtsmuudrxzr = 805;
	int yajzgqzy = 1015;
	if (false == false) {
		int qoia;
		for (qoia = 2; qoia > 0; qoia--) {
			continue;
		}
	}
	if (1015 != 1015) {
		int jwbsykn;
		for (jwbsykn = 22; jwbsykn > 0; jwbsykn--) {
			continue;
		}
	}
	if (string("sjwaesgudvtdunfxhctazqvbzprrxalcawrlyyezpawvilspi") == string("sjwaesgudvtdunfxhctazqvbzprrxalcawrlyyezpawvilspi")) {
		int zdzkjy;
		for (zdzkjy = 16; zdzkjy > 0; zdzkjy--) {
			continue;
		}
	}
	if (string("sjwaesgudvtdunfxhctazqvbzprrxalcawrlyyezpawvilspi") == string("sjwaesgudvtdunfxhctazqvbzprrxalcawrlyyezpawvilspi")) {
		int mmvomgfpm;
		for (mmvomgfpm = 50; mmvomgfpm > 0; mmvomgfpm--) {
			continue;
		}
	}
	return 28473;
}

int jpqhsia::pzirvufndfhzegyosayha(bool gomrbhjb, double eyxmj, double iuwuhd, bool jzmkdixjeudu, bool asfxumae, int qnslcdtlvjjd, string brwlpvjldfuf, double iypihrrljbrf, int pzvoyhonbaw, double iqgqexvwrhcux) {
	double mqpsbo = 9194;
	bool vjojhyjv = true;
	string avapirqguwxxu = "zfgvdf";
	if (9194 == 9194) {
		int gbew;
		for (gbew = 15; gbew > 0; gbew--) {
			continue;
		}
	}
	if (true != true) {
		int phdhr;
		for (phdhr = 55; phdhr > 0; phdhr--) {
			continue;
		}
	}
	return 64248;
}

string jpqhsia::tqgegwmgnecxwemckvo(string eaehhsslxwl) {
	string dcmlypqvcjidz = "rnwwpcuxywexbzqdahpfndwtightcskkkjkmtgfvjorqfyxwrsvchvq";
	double ncmxxnyqbp = 77248;
	int bxtmftnmhhbdwfv = 119;
	int vmuzwtjplddete = 692;
	if (77248 != 77248) {
		int ykkdcijzc;
		for (ykkdcijzc = 72; ykkdcijzc > 0; ykkdcijzc--) {
			continue;
		}
	}
	if (119 != 119) {
		int cchbmot;
		for (cchbmot = 36; cchbmot > 0; cchbmot--) {
			continue;
		}
	}
	if (77248 != 77248) {
		int yjhvkto;
		for (yjhvkto = 36; yjhvkto > 0; yjhvkto--) {
			continue;
		}
	}
	if (77248 != 77248) {
		int ipofqvopay;
		for (ipofqvopay = 96; ipofqvopay > 0; ipofqvopay--) {
			continue;
		}
	}
	if (string("rnwwpcuxywexbzqdahpfndwtightcskkkjkmtgfvjorqfyxwrsvchvq") != string("rnwwpcuxywexbzqdahpfndwtightcskkkjkmtgfvjorqfyxwrsvchvq")) {
		int ybnlav;
		for (ybnlav = 13; ybnlav > 0; ybnlav--) {
			continue;
		}
	}
	return string("npjeohfbhcqmjbvbids");
}

double jpqhsia::ujvschnmcknkzpsxr(bool olgpzivpxklbpn, bool owssooeqpwjwisa, bool bubbwsm) {
	double rvbzxifr = 9003;
	return 36324;
}

void jpqhsia::acuxhnvcvunjeslpgas(double vuhdea, double rhgppt, int mfjspgjxpjmjl, bool tlonfuzip, double zcrfhvrlwpgwd, double yfngtyiozwtpta, bool tqclzcxfoehlm) {
	bool vkrqqxrpt = true;
	int xefqo = 623;
	double fsaqs = 26551;
	int ndoknezlxjv = 3653;
	int vyjtpf = 3276;
	string tfvmwbykybd = "ab";
	int dhbrjucgbnd = 2400;
	if (623 != 623) {
		int htsbcnwku;
		for (htsbcnwku = 35; htsbcnwku > 0; htsbcnwku--) {
			continue;
		}
	}

}

void jpqhsia::xlqthzwcuzhqv(double ldlinoxs, int yslkhhowzg, double mvahvulqx, double btbdi, int xvprqryqifa, double pfxej, string etxzrogkyepyt, bool sqovydtquogipgk, string saagigxpoby, bool ypewardjomfeop) {
	int fjjcnenwdhvia = 4628;
	if (4628 == 4628) {
		int rjhtbh;
		for (rjhtbh = 43; rjhtbh > 0; rjhtbh--) {
			continue;
		}
	}
	if (4628 != 4628) {
		int ms;
		for (ms = 17; ms > 0; ms--) {
			continue;
		}
	}
	if (4628 != 4628) {
		int lzf;
		for (lzf = 36; lzf > 0; lzf--) {
			continue;
		}
	}
	if (4628 != 4628) {
		int gisrwk;
		for (gisrwk = 88; gisrwk > 0; gisrwk--) {
			continue;
		}
	}

}

int jpqhsia::ukuooyjlotp(int fnzxlyz, string uwrwxo, double dpwsm, double vofgfss, bool gnzhttyea, string dtiwuxrl, string urvlwuw) {
	string tvsvbz = "ahjgxztupihptojalhnhvkwwkxyecitjtnnrxcnifwrkzczcutugzycmyloiaivgqifd";
	return 1013;
}

string jpqhsia::kqiqtbeelcugehbrb(double rssgspf, bool xmozbwkw, int dgxngu, double zywkht) {
	int tdhkqy = 1694;
	int wojmdyzdx = 2395;
	bool fjgdzlupx = true;
	string pqnrzwmgcwr = "faigycrbfahvsyuncmydks";
	string dyzteztcofhtfpj = "zp";
	int mfzuaenj = 6607;
	double fbkmzqp = 2253;
	int xfqtbaog = 825;
	if (2395 == 2395) {
		int uywjzvy;
		for (uywjzvy = 69; uywjzvy > 0; uywjzvy--) {
			continue;
		}
	}
	if (2395 == 2395) {
		int spc;
		for (spc = 38; spc > 0; spc--) {
			continue;
		}
	}
	if (825 == 825) {
		int pgzex;
		for (pgzex = 0; pgzex > 0; pgzex--) {
			continue;
		}
	}
	return string("aoplsbftzfaserlkbix");
}

int jpqhsia::dqsoeuzgzrmpymlptuluhbf(bool ndxlwznivfzw, string qgfffd, int epfupjhgmcijns) {
	double fglkocjllu = 28008;
	double skrkfgzrct = 64844;
	double dpwpkpmsfkj = 56280;
	bool idvcgyol = true;
	bool fyvtrkrgjacs = true;
	bool qxeydhvldtxkul = false;
	if (64844 == 64844) {
		int agkdknkdyy;
		for (agkdknkdyy = 87; agkdknkdyy > 0; agkdknkdyy--) {
			continue;
		}
	}
	if (28008 != 28008) {
		int yliljommpx;
		for (yliljommpx = 40; yliljommpx > 0; yliljommpx--) {
			continue;
		}
	}
	return 27029;
}

string jpqhsia::lxbafnvrecrwcmanaevrbgmfw(double abweccvmivfw, string raowjhwzejqrw, string docoamugi, double ijyvwimxrlnuo, bool jdzkiftr, string zcrlcuevhwse, bool ogeru, int behmn) {
	double mxwuigjxkqxipo = 34286;
	double htaslaejncl = 20105;
	int fhlgyvpo = 1794;
	double wqkhqebtvqkdc = 19392;
	bool cxdrdkugo = true;
	string uoadolrfajhyq = "hefoyhrqwhomulijyvnxaxyreyjbeczjwnuofbcgpyumejcuatwysuzglzydycnll";
	bool veakfuv = true;
	int ucsjlpxhu = 7605;
	string krgxz = "lggwrsifvtrgpivfhxehgxgkkpywrbpmpajzetwbxkjnixptbtsvxszbdhgzrcqfo";
	string pccramd = "unuwlhuleuwqj";
	if (7605 == 7605) {
		int yjinocra;
		for (yjinocra = 63; yjinocra > 0; yjinocra--) {
			continue;
		}
	}
	if (20105 != 20105) {
		int fslu;
		for (fslu = 92; fslu > 0; fslu--) {
			continue;
		}
	}
	return string("ycqdvwqexojhaiymax");
}

double jpqhsia::amhqfamtlrtejkxh(double slfrr, string injsw) {
	double yskrglo = 61005;
	int nqoexxqy = 1024;
	if (1024 == 1024) {
		int kaffc;
		for (kaffc = 76; kaffc > 0; kaffc--) {
			continue;
		}
	}
	if (1024 == 1024) {
		int gcdqfgec;
		for (gcdqfgec = 18; gcdqfgec > 0; gcdqfgec--) {
			continue;
		}
	}
	if (61005 != 61005) {
		int rirbwehqwe;
		for (rirbwehqwe = 17; rirbwehqwe > 0; rirbwehqwe--) {
			continue;
		}
	}
	return 81415;
}

void jpqhsia::uritxiqpqhzbfjmwlofqo(bool mgaanhkggc, string wqvzeke, bool hxeoe) {
	string xwxvvfkohgakfv = "fsflmdfzldbbmcljrgcvsnjwjridmhynebizsmytgsszhauwpquogurqzfzzfxphukfmijhumpenavsftjwq";
	bool somwnnhludqvbh = true;
	double rufqgptioxrh = 59919;
	if (59919 != 59919) {
		int fpcpyibr;
		for (fpcpyibr = 1; fpcpyibr > 0; fpcpyibr--) {
			continue;
		}
	}
	if (true == true) {
		int ya;
		for (ya = 27; ya > 0; ya--) {
			continue;
		}
	}
	if (true == true) {
		int zttk;
		for (zttk = 27; zttk > 0; zttk--) {
			continue;
		}
	}
	if (string("fsflmdfzldbbmcljrgcvsnjwjridmhynebizsmytgsszhauwpquogurqzfzzfxphukfmijhumpenavsftjwq") == string("fsflmdfzldbbmcljrgcvsnjwjridmhynebizsmytgsszhauwpquogurqzfzzfxphukfmijhumpenavsftjwq")) {
		int cun;
		for (cun = 35; cun > 0; cun--) {
			continue;
		}
	}
	if (59919 == 59919) {
		int drjbd;
		for (drjbd = 24; drjbd > 0; drjbd--) {
			continue;
		}
	}

}

string jpqhsia::hldjdowmohvtkdkt(string xolvbcbjgyk, string awaal, int dbczumiwyakcww, bool sexiax, bool hdazt, string zruheadfkqtacxm, double jqflsfyzxuskp, int sbbpjkpaxoehbr, string vyzqg, int siuchtvijak) {
	double nhkuv = 23839;
	string uuibbtdbqlakp = "cnkbocupqcpmkuslzestofkpsucbkmxmogwlkbujrpmumcamykjasjtlvethiqpzkfph";
	bool vuqupjmpwmkk = false;
	bool nllnovdegkz = false;
	string vmepmv = "aeqwfgtosloywlgqyyhgzujnusnjbophifpnaralmdiljrfwnxrpvtjtahqtqnlrrjhetyco";
	string qzunqfxfkfzhecx = "cxytidjxvgoocxedysgjjzitpnnpcjbzixbcpri";
	string nookxfsmvtjrztx = "thieqanaqustpfysffnaldiellyzzqqqapwgdudjejkahpq";
	if (23839 == 23839) {
		int evwwwskv;
		for (evwwwskv = 26; evwwwskv > 0; evwwwskv--) {
			continue;
		}
	}
	if (string("cnkbocupqcpmkuslzestofkpsucbkmxmogwlkbujrpmumcamykjasjtlvethiqpzkfph") == string("cnkbocupqcpmkuslzestofkpsucbkmxmogwlkbujrpmumcamykjasjtlvethiqpzkfph")) {
		int ivgv;
		for (ivgv = 89; ivgv > 0; ivgv--) {
			continue;
		}
	}
	return string("ibzvaemdbtviahkad");
}

double jpqhsia::hsmsoypirmexopgfphl(int fsectbpouyh, bool qitbeuloulxu) {
	int pydjiabwuiarwgo = 887;
	double dmuog = 38394;
	if (38394 == 38394) {
		int ia;
		for (ia = 44; ia > 0; ia--) {
			continue;
		}
	}
	if (887 == 887) {
		int io;
		for (io = 10; io > 0; io--) {
			continue;
		}
	}
	if (38394 != 38394) {
		int pusyrnrfbi;
		for (pusyrnrfbi = 2; pusyrnrfbi > 0; pusyrnrfbi--) {
			continue;
		}
	}
	if (887 != 887) {
		int vk;
		for (vk = 64; vk > 0; vk--) {
			continue;
		}
	}
	return 70957;
}

int jpqhsia::fxtmkszmhmhpxlqe(string ebazlteicatpkez, bool worsura, int munmgvzxsstmscf, double ebblyfdrnamcyoi, double qrfnsdl) {
	bool oizpcxjpbfb = false;
	bool nosvhhehuk = false;
	bool cwnbnboobblqchh = false;
	int zlayigyktkgy = 6320;
	if (false == false) {
		int bvgnhdpwsb;
		for (bvgnhdpwsb = 28; bvgnhdpwsb > 0; bvgnhdpwsb--) {
			continue;
		}
	}
	if (false == false) {
		int jp;
		for (jp = 67; jp > 0; jp--) {
			continue;
		}
	}
	if (false == false) {
		int tmya;
		for (tmya = 90; tmya > 0; tmya--) {
			continue;
		}
	}
	return 89180;
}

void jpqhsia::ovwcqtpwwqltbpp(bool byoiovabtuagrj, bool hihausvfewdv, int hzaphtcsjzevix, double kdgqwnp, int rgdgfuehmogpe, string unaghxbzuhlgj, int opoycousmsjnzh) {
	double oojhedutlkmb = 32391;
	int sgxdr = 3310;
	bool zjxvdo = true;
	double jksiit = 290;
	string nroxjnkhydtdhga = "kbcrvietcs";
	if (3310 != 3310) {
		int vis;
		for (vis = 38; vis > 0; vis--) {
			continue;
		}
	}
	if (290 != 290) {
		int igak;
		for (igak = 58; igak > 0; igak--) {
			continue;
		}
	}
	if (290 != 290) {
		int npyinru;
		for (npyinru = 85; npyinru > 0; npyinru--) {
			continue;
		}
	}
	if (3310 != 3310) {
		int crszuxsbv;
		for (crszuxsbv = 5; crszuxsbv > 0; crszuxsbv--) {
			continue;
		}
	}

}

jpqhsia::jpqhsia() {
	this->ukuooyjlotp(1149, string("oclnijaksv"), 12841, 8732, false, string("thxfmpnvdxjpurmvplnjsdkranlogogimbhkqhscnwthzdhqwommybzqszvxzhlf"), string("rvbdchqrsbzimmelwhxuaucfkdywmt"));
	this->kqiqtbeelcugehbrb(8343, false, 2088, 25254);
	this->dqsoeuzgzrmpymlptuluhbf(true, string("mzcxbqqdoaxmmuhakkmjazfbghrxoqjohljrohgkwkvjyhsw"), 5164);
	this->lxbafnvrecrwcmanaevrbgmfw(3632, string("snzvqqvbervrcclhnaoisfwqdakuoazdqkknjciqnolgbkzeqscdpbfqmpdyetjoyu"), string("h"), 25252, true, string("urszqntzmatfxgykosnpykxozvfdoejoratadszkozxvcosarnprxapmsllgvcyiaxqpwcfqhgziixujqfftjsexnqvgz"), true, 2118);
	this->amhqfamtlrtejkxh(14727, string("fwvcukfrttdlzdnejcftegwppfhfszldwahu"));
	this->uritxiqpqhzbfjmwlofqo(false, string("hcixlhcrojdxtlsovmlgmof"), true);
	this->hldjdowmohvtkdkt(string("pwdqmehkdlaaohhhmuubnfzvpibdfxmhbmrtpvlghxcgopmglgwpvvmjznrshkydhmoqafdzeeembxadybyuczjsx"), string("axqgnnlmavnaxsizicihopqbpftenuzlujxnnjjv"), 3292, true, false, string("npsxbriynsurefcwqasalukfdyrxmipdjiszxpsrwzsqwjobtnfphscskfycdexywhigrgkvwdpepm"), 22, 7810, string("zbzqgpstziylgdfnoidiaerbntrqxil"), 1503);
	this->hsmsoypirmexopgfphl(205, true);
	this->fxtmkszmhmhpxlqe(string("wl"), true, 1540, 22621, 17226);
	this->ovwcqtpwwqltbpp(true, true, 378, 12355, 1805, string("vazuuyvcmzikqt"), 3681);
	this->rxkyqjgwylatkbrchdjxkcjcd(false);
	this->xvifhkqlqki(24479, 35620, 28067, 82748, string("xudhflcqzbdikbrqs"));
	this->mzmuqizllzmnxy(3664, false);
	this->bqvtgtanwpuvewolu(string("tevfdgzycngmdugatlfzddianprutmfdvvdwwvh"), 4078, string("ajjebpgouthozxheirgjhyyajxbdzvwjwrdltpsjwnivyxwxahxmixyfctpgtyrlbwiroswznibucctyhv"), string("dgyyadjmzjxdvgocehlnlcylwxuehgdmcnmataebeeavlsudxsnxesmjkdtveehvyyxoogq"), 17958, 63263, 30199, 40609);
	this->pzirvufndfhzegyosayha(true, 23388, 29310, false, true, 2697, string("yibpwpyrtjjkcsikvrkkkywqoquqcgqjrnfkrmdgo"), 21882, 2182, 45521);
	this->tqgegwmgnecxwemckvo(string("ignrthiqkqtpjhlgdtmgkphdpmzqxscwyfwlxnzhvtrcynp"));
	this->ujvschnmcknkzpsxr(false, false, true);
	this->acuxhnvcvunjeslpgas(8277, 6911, 1176, true, 54814, 4271, true);
	this->xlqthzwcuzhqv(50673, 8615, 17423, 5528, 405, 5093, string("xrydyqsbkeviahfcxglugnaqnlvrohtlcvdzwwytohoiwdogubgisfoeafhuvnrwnxulmigpjvrqbwoisadhrnbfhimafz"), true, string("xdiernpmrtvxiglxbwenjeeliojqdojgagewxgpspczuhdlvhqqnhahqxgu"), false);
	this->rxszdoctapqhmscgmd(922, 3882, string("mcqbrmwspwcowgsennskbmtwokkddjjudvxalqronhypwvnzsewbkca"), string("vhxqjejswzqiorbelfxijsmfhzxdpzfnsjnldmodmdnrtsqyc"), 83077, 16, 34552, false);
	this->kznnvcbfvrwzrehelin(12551, 23926, string("mqauhyjwalerrcmnrgcnwplmsvpemdplgeztttlmwotrfrxnklxxkqrozffrxklfzeqzzufvif"));
	this->ebjipvwzclcdizoqfxpehl(false, 2099);
}

#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class jctxgrr {
public:
	double jfxzjbmmwfod;
	bool blmguoaghxunn;
	string cilisxiozdkgsh;
	int zmhobocgnyjawdh;
	string woapizd;
	jctxgrr();
	void tsclylcepvedgxmgqdwqr(int xkaydqgykluyg, string uiqqvdnoxfsxpq, int qztdnbqqtrmghr, bool dvzbprfc, bool zygbzljpdbnurst, int okdowqkwqh, int tncijibhajy, bool kbqvchigkztsh, double cbhcudzqrj);
	void dhgvzvgsahc();
	int noceazmaolir(double alipklanawxzsb, int ajblhzwdvtwjo, int rncjx, string frkqozkktu, double uemoalkxmgpbefc, double rbovrwnjmkunagr);
	bool enekbmkxyv(double kadbvyj, int pztlopkuvt, double cykxyfcarn);

protected:
	string adybdn;

	int sohrvdaanacnknlzhwm();
	int zxblysoqufihqlm(int ngota, bool ndfdx, bool zmmutuwlqn, bool hvwmfyq, int xpbyeet, string boimxhfvlqru, int coeabyo, int vtsiymzlec);
	int ordxekddadeyyycjwcgxk(int azhiwrpjici);
	void xsysudbaafropakrrgrsakk(int hqsdygqvfbqk, string nbtbspisyblr, int ovvnpbwqg, bool ymrjatnsiqnwwf, double twkuliy, int ayeqpdrezwfhlrx, bool kuttquicorcs, bool qdpwaiacfj, double npqmk);

private:
	string epfukeeqb;
	bool blldwgdqljxqee;
	string yvdamiowurbop;
	double lcqry;

	string vbresxudswjpecdiv(string balduteajmxafq);
	bool pymnirbbrohiccsb(double nqklw, string edxjhzqalublqub, int guuztnunwycj);

};


string jctxgrr::vbresxudswjpecdiv(string balduteajmxafq) {
	double krftviuuyhg = 17169;
	string bacbrypygne = "ueyvaeruiwdvotbvbsielxsxyuydln";
	bool khtjxyaqhgaxbn = true;
	string ufqmergcrb = "romuzsegqpgjhacowgknimqgeioqujglxgxidtabufhtgzorvgwchiljdeo";
	double ewbosxxaqvzsli = 6900;
	int hkwiu = 2861;
	bool gqwbtxdrzx = true;
	bool rkdhgdqa = true;
	string tmobtoacfhb = "tasdryznkobyjsdqxazszejewsi";
	if (string("romuzsegqpgjhacowgknimqgeioqujglxgxidtabufhtgzorvgwchiljdeo") == string("romuzsegqpgjhacowgknimqgeioqujglxgxidtabufhtgzorvgwchiljdeo")) {
		int bzohlr;
		for (bzohlr = 32; bzohlr > 0; bzohlr--) {
			continue;
		}
	}
	if (true != true) {
		int abj;
		for (abj = 33; abj > 0; abj--) {
			continue;
		}
	}
	if (6900 == 6900) {
		int pjafkhfmgi;
		for (pjafkhfmgi = 34; pjafkhfmgi > 0; pjafkhfmgi--) {
			continue;
		}
	}
	if (true != true) {
		int rkgm;
		for (rkgm = 78; rkgm > 0; rkgm--) {
			continue;
		}
	}
	return string("");
}

bool jctxgrr::pymnirbbrohiccsb(double nqklw, string edxjhzqalublqub, int guuztnunwycj) {
	bool yhxyhq = false;
	if (false == false) {
		int hlvfvo;
		for (hlvfvo = 41; hlvfvo > 0; hlvfvo--) {
			continue;
		}
	}
	return true;
}

int jctxgrr::sohrvdaanacnknlzhwm() {
	double gqvfro = 49883;
	int zskbimntljqgo = 7514;
	if (49883 == 49883) {
		int nixcxbglj;
		for (nixcxbglj = 78; nixcxbglj > 0; nixcxbglj--) {
			continue;
		}
	}
	if (7514 == 7514) {
		int tlmz;
		for (tlmz = 92; tlmz > 0; tlmz--) {
			continue;
		}
	}
	if (49883 != 49883) {
		int xuystluri;
		for (xuystluri = 22; xuystluri > 0; xuystluri--) {
			continue;
		}
	}
	if (49883 == 49883) {
		int cuxdxivo;
		for (cuxdxivo = 66; cuxdxivo > 0; cuxdxivo--) {
			continue;
		}
	}
	return 70800;
}

int jctxgrr::zxblysoqufihqlm(int ngota, bool ndfdx, bool zmmutuwlqn, bool hvwmfyq, int xpbyeet, string boimxhfvlqru, int coeabyo, int vtsiymzlec) {
	string njzfpzilobloe = "zcesbdgvsnshkkmlavuqgrrvbawfxf";
	int ekazjfkbvngtph = 5237;
	bool xievdvvlckyhwgg = true;
	int cxzryprwbbs = 5711;
	int kaaeawqocwzjh = 1249;
	double wairzwgjmcmf = 20559;
	double jxuwsbixzo = 43677;
	int adswnibq = 2261;
	double ahvizjsjfujloq = 71292;
	if (5711 == 5711) {
		int dysrqxfpqf;
		for (dysrqxfpqf = 20; dysrqxfpqf > 0; dysrqxfpqf--) {
			continue;
		}
	}
	if (true == true) {
		int hbvmdaivm;
		for (hbvmdaivm = 53; hbvmdaivm > 0; hbvmdaivm--) {
			continue;
		}
	}
	return 70070;
}

int jctxgrr::ordxekddadeyyycjwcgxk(int azhiwrpjici) {
	int tuwlzrccrksz = 903;
	return 91541;
}

void jctxgrr::xsysudbaafropakrrgrsakk(int hqsdygqvfbqk, string nbtbspisyblr, int ovvnpbwqg, bool ymrjatnsiqnwwf, double twkuliy, int ayeqpdrezwfhlrx, bool kuttquicorcs, bool qdpwaiacfj, double npqmk) {
	bool odrhuhakm = false;
	int ugdfhctgsqmcv = 1022;
	bool zrdtocci = false;
	if (false != false) {
		int graemiinyt;
		for (graemiinyt = 94; graemiinyt > 0; graemiinyt--) {
			continue;
		}
	}
	if (1022 == 1022) {
		int koinovqtkh;
		for (koinovqtkh = 88; koinovqtkh > 0; koinovqtkh--) {
			continue;
		}
	}
	if (false != false) {
		int aobrroa;
		for (aobrroa = 7; aobrroa > 0; aobrroa--) {
			continue;
		}
	}
	if (false != false) {
		int oapf;
		for (oapf = 47; oapf > 0; oapf--) {
			continue;
		}
	}

}

void jctxgrr::tsclylcepvedgxmgqdwqr(int xkaydqgykluyg, string uiqqvdnoxfsxpq, int qztdnbqqtrmghr, bool dvzbprfc, bool zygbzljpdbnurst, int okdowqkwqh, int tncijibhajy, bool kbqvchigkztsh, double cbhcudzqrj) {

}

void jctxgrr::dhgvzvgsahc() {
	string ojjovwb = "onylqzclxjmnedmeorudxmrggowtitrwsu";
	double hdxrttglfdltn = 27829;
	string tsyvcovxoo = "kow";
	bool yjmmrwl = true;
	double mgexpvldt = 26927;
	string theamocws = "scqdibuxzdxvggkmy";

}

int jctxgrr::noceazmaolir(double alipklanawxzsb, int ajblhzwdvtwjo, int rncjx, string frkqozkktu, double uemoalkxmgpbefc, double rbovrwnjmkunagr) {
	return 89120;
}

bool jctxgrr::enekbmkxyv(double kadbvyj, int pztlopkuvt, double cykxyfcarn) {
	double nkqwzxu = 34866;
	int mwgmvpk = 3555;
	int miqsswpfmde = 7990;
	double dbcxfhmybchu = 57101;
	double daaqbxqs = 72681;
	double tiqkriyt = 8869;
	bool ibgzx = false;
	if (72681 == 72681) {
		int znyt;
		for (znyt = 85; znyt > 0; znyt--) {
			continue;
		}
	}
	if (7990 != 7990) {
		int mrsb;
		for (mrsb = 41; mrsb > 0; mrsb--) {
			continue;
		}
	}
	return false;
}

jctxgrr::jctxgrr() {
	this->tsclylcepvedgxmgqdwqr(3085, string("dpubambgfrpjrqffevcqnrepsjkhldksuzzygtcamainrdnjltwrcvphuwomiazopfynubpurlrbqnd"), 4075, false, true, 1003, 2389, true, 18923);
	this->dhgvzvgsahc();
	this->noceazmaolir(50119, 7775, 3666, string("lfyxhswabbvihdwiq"), 31423, 9966);
	this->enekbmkxyv(10272, 2547, 50227);
	this->sohrvdaanacnknlzhwm();
	this->zxblysoqufihqlm(3404, false, true, false, 7442, string("tnrsdcxybifyxauzgvyvzrhwpdnwrhhxgffmhpzeknpshifuhbxofqegqqvnhleaqvyzwcmgrfxxfqmrpmiltoynhiy"), 2596, 8940);
	this->ordxekddadeyyycjwcgxk(3795);
	this->xsysudbaafropakrrgrsakk(311, string("sionmjyheyhxtmzzfekdhxrktzlg"), 2224, true, 17802, 56, true, false, 32423);
	this->vbresxudswjpecdiv(string("yzbfnnobtffiwdsfxikldxnrpagclqxnqcvawbvmzrncbdlnafevdkacwkvur"));
	this->pymnirbbrohiccsb(22410, string("eecsgupghbktzyeoktpmqwixprfohlsbaqdultkwkjigzmtdmacrulcpzqxnotcgvgi"), 4662);
}
