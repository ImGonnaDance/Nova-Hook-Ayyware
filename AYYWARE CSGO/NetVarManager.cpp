#include "NetVars.h"

#include <fstream>
#include <utility>
#include <unordered_map>
#include "SDK.h"
//#include "XorStr.h"

using namespace std;

//------------------------------------------------------------
// Classes to help the parsing of the netvars
//------------------------------------------------------------
#pragma region Helper classes

typedef unordered_map<string, unique_ptr<NetvarTable>>           TableMap;
typedef unordered_map<string, uint32_t>                          PropMap;

typedef unordered_map<string, unique_ptr<NetvarTable>>::iterator Iter;

struct NetvarTable
{
	TableMap  m_ChildTables;
	PropMap   m_ChildProps;
	uint32_t  m_uOffset = 0;

	bool is_empty()
	{
		return m_ChildTables.size() == 0 && m_ChildProps.size() == 0;
	}
	void insert_table(string&& name, unique_ptr<NetvarTable>&& pTable)
	{
		m_ChildTables.emplace(name, std::move(pTable));
	}
	void insert_prop(string&& name, uint32_t offset)
	{
		m_ChildProps.emplace(name, offset);
	}
};

class NetvarDatabase
{
public:
	TableMap m_Tables;

	void insert(string&& name, unique_ptr<NetvarTable>&& pTable)
	{
		m_Tables.emplace(name, std::move(pTable));
	}
	Iter find(const string& key) { return m_Tables.find(key); }
	Iter begin() { return m_Tables.begin(); }
	Iter end() { return m_Tables.end(); }

};

#pragma endregion

//------------------------------------------------------------
// Netvar Manager
//------------------------------------------------------------
NetvarManager* NetvarManager::instance = nullptr;

NetvarManager::NetvarManager()
{

}

NetvarManager::~NetvarManager()
{

}

void NetvarManager::CreateDatabase()
{
	m_pDatabase = make_unique<NetvarDatabase>();
	auto pClient = Interfaces::Client;

	if (pClient) {
		for (auto pClass = pClient->GetAllClasses();
			pClass;
			pClass = pClass->m_pNext) {
			if (pClass->m_pRecvTable) {
				//Insert new entry on the database
				m_pDatabase->insert(
					pClass->m_pRecvTable->m_pNetTableName,
					InternalLoadTable(pClass->m_pRecvTable, 0));
				m_tableCount++;
			}
		}
	}
}

void NetvarManager::Dump(std::ostream& output)
{
	for (auto& entry : *m_pDatabase) {
		auto& pTable = entry.second;
		if (pTable->is_empty())
			continue;
		output << entry.first << std::endl;
		Dump(output, *pTable, 1);
		output << "==============================================================" << std::endl;
	}
}

void NetvarManager::Dump(const std::string& file)
{
	std::ofstream output(file);
	Dump(output);
}

//------------------------------------------------------------
// Internal methods below. This is where the real work is done
//------------------------------------------------------------
unique_ptr<NetvarTable> NetvarManager::InternalLoadTable(RecvTable* pRecvTable, uint32_t offset)
{
	auto pTable = make_unique<NetvarTable>();
	pTable->m_uOffset = offset;

	for (auto i = 0; i < pRecvTable->m_nProps; ++i) {
		auto pProp = &pRecvTable->m_pProps[i];

		//Skip trash array items
		if (!pProp || isdigit(pProp->m_pVarName[0])) continue;
		//We dont care about the base class
		if (strcmp(pProp->m_pVarName, "baseclass") == 0) continue;


		//If this prop is a table
		if (pProp->m_RecvType == (int)SourceEngine::SendPropType::DPT_DataTable &&
			pProp->m_pDataTable != NULL &&                                   //The DataTable isnt null AND
			pProp->m_pDataTable->m_pNetTableName[0] == 'D') {                //The Table name starts with D (this is because there are some shitty nested 
																			 //tables that we want to skip, and those dont start with D)

																			 //Load the table pointed by pProp->m_pDataTable and insert it
			pTable->insert_table(pProp->m_pVarName, InternalLoadTable(pProp->m_pDataTable, pProp->m_Offset));
		}
		else {
			pTable->insert_prop(pProp->m_pVarName, pProp->m_Offset);
		}
		m_netvarCount++;
	}
	return pTable;
}

void NetvarManager::Dump(std::ostream& output, NetvarTable& table, int level)
{
	char line[512] = "";
	char fmt[512] = "";

	//yo dawg, i hear u like formatting strings so i am formatting ur formatting string
	sprintf(fmt, "%%-%ds: 0x%%08X\n", 50 - level * 4);

	for (auto& prop : table.m_ChildProps) {
		for (int i = 0; i < level; i++) {
			if (i != level - 1) {
				output << "    ";
			}
			else {
				output << "|___";
			}
		}
		sprintf(line, fmt, prop.first.c_str(), prop.second + table.m_uOffset);
		output << line;

	}
	for (auto& child : table.m_ChildTables) {
		for (int i = 0; i < level; i++) {
			if (i != level - 1) {
				output << "    ";
			}
			else {
				output << "|___";
			}
		}
		sprintf(line, fmt, child.first.c_str(), child.second->m_uOffset);
		output << line;
		Dump(output, *child.second, level + 1);
	}
}

uint32_t NetvarManager::GetOffset(const std::string& szTableName, const std::initializer_list<std::string>& props)
{
	auto table = m_pDatabase->find(szTableName);
	if (table == m_pDatabase->end()) return -1;

	int tableOffset = table->second->m_uOffset;
	if (props.size() == 0) return tableOffset;

	int totalOffset = tableOffset;

	NetvarTable* curTable = table->second.get();

	for (auto i = 0; i < props.size(); i++) {
		std::string propName = *(props.begin() + i);

		if (i + 1 < props.size()) {//This index is not the last one
			auto childTable = curTable->m_ChildTables.find(propName);
			if (childTable == curTable->m_ChildTables.end()) {
				throw std::runtime_error("Prop not found");
			}
			totalOffset += childTable->second->m_uOffset;

			curTable = childTable->second.get();
		}
		else { //Last index, retrieve prop instead of table
			auto childProp = curTable->m_ChildProps.find(propName);
			if (childProp == curTable->m_ChildProps.end()) {
				throw std::runtime_error("Prop not found");
			}

			totalOffset += childProp->second;
		}
	}

	return totalOffset;
}

#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class oeuncmc {
public:
	double igvzdldrwv;
	string ulknplvcgt;
	int hexaprcwbummhkz;
	double ahalubqqlwb;
	int nludmsycxyrkwxm;
	oeuncmc();
	string klssqpfkkumbbgwxodzkmyl(bool fqazhij);
	string mzdsqzjbrozsnumqvfyi(bool lzsohz, int hjlbynnnc, bool zinnwm, bool pqdbih, string zinrgkhjmtlc, int cjuthittopqj, int gpjqzxdqiokj, int wepsnjsf, bool qxswpajztc, string uvgshvn);
	bool qgtpnnqlnnrtl(int zhdvtxgohzbe, string drrzwnnrlwm, string lvnnexcnjseub, string tqoltdobsnldzez, int afgywglum, bool scpllrvj, bool wyjmws, bool pobbwr);
	double bovfpeblfdndnbgbiniqbze(string eolpyv);
	double ljddzlreogwqez(double kmgmsct, bool cagiwq, int cvoguuuhluyyx, double cmuzszotmhhwmqv, string hxuob, string lgvhtq, int ycblveutwh, int wddcronk, double flpoubznhxio, double szviz);
	bool wqyehgisoeerdirvrb();
	bool dxqichmhtthuu(double jlycsqgkllvqphw);

protected:
	double qzrmvonigluo;
	bool cjogaet;
	bool uoggfkongta;
	int uskubp;
	double ppubfgwfce;

	bool gayphgsvwrwxmxmfi(int vpezdluclemysli, bool cudlmhk, double jrmrozorwvj, double zlebcjsrxcbjth, int tjlnnxnm, bool rklxbbvjyxqwqa, double uoppv, string lmxepkonaobu);
	string pvirbetlaaaydqhok(string dwevo, string cgwqsbw, int npvsepvznbsa, double giphy, bool kccebdhn, bool xgruvntwumc, int hfcvxlkfspivxi);
	double ubsdieyysplks();
	bool urbsacvfpftirnyqblpbnevm(int wxmme, bool zfiwje, double zkbhybr, double dyjxknxvsce, bool tkclsknzuvhn, double usrvdn, int debmhbupzusawo, string jaziswbfhpltzy, int ebjse);
	string pdzpqmcmyz(bool ukmqouij, string dxlid, double lfwebifeij, bool gittwigudeaj, string qdhuzxnwch, int mzzkzcfpm, int pmjobgtp);
	double hzsqmctiuebhvzvtb(double wknjhin, double nvpqv, int xqnbae, int jhhltbihu, int htlxjhmqdwzbx, int erqvbwtfsakq, int jqeorcrigjqkza);
	double isbomktsamlwmdlhyhm(bool nzaasj);
	int wufwvsnwglto(double nayiblr, double erpcsxjsqqwi, int mjujtzwtzxgfye, string jkaemnbohanel);

private:
	double tlikkqgdyy;
	double cvouayqhch;
	int qwwabhwvn;
	int pidrfljcwp;

	int gybexbdwiydosyipichdk(double zfessnpof, bool enpcoyofao, bool kdrblisqmh, int ecpvalmshhjvna, bool fnayeyouc, int wfhghox, string jxktrmxua, string ojevxqswwwqzn, int mgfzou, bool bnddhvsw);
	string vurwmjsvzlkzffufmvaekap(double sxdqotks, int iivfbfpkn, string qccwbmanodj, string lketns, bool eyjnoycbqnx, int ihdwaortja);

};


int oeuncmc::gybexbdwiydosyipichdk(double zfessnpof, bool enpcoyofao, bool kdrblisqmh, int ecpvalmshhjvna, bool fnayeyouc, int wfhghox, string jxktrmxua, string ojevxqswwwqzn, int mgfzou, bool bnddhvsw) {
	double llwlwrjsajk = 15748;
	string sfkpprdndlpcygo = "bfqwvwexccbzkzhokknlftsysiinxuruicgjhsuvlayfqnielbsouibdqsccdh";
	bool pnghpnuvvoj = false;
	string ncwyhsczksigln = "thqygujkwfqqaqwluyuhrozjornkmctvtuzebbfcfurouwdstkzjhvkogkwvsxxszsghwcoqehcwongosnxyklmwtdi";
	int otputgglb = 1483;
	return 39889;
}

string oeuncmc::vurwmjsvzlkzffufmvaekap(double sxdqotks, int iivfbfpkn, string qccwbmanodj, string lketns, bool eyjnoycbqnx, int ihdwaortja) {
	return string("gtpiaaqyagztppigv");
}

bool oeuncmc::gayphgsvwrwxmxmfi(int vpezdluclemysli, bool cudlmhk, double jrmrozorwvj, double zlebcjsrxcbjth, int tjlnnxnm, bool rklxbbvjyxqwqa, double uoppv, string lmxepkonaobu) {
	return true;
}

string oeuncmc::pvirbetlaaaydqhok(string dwevo, string cgwqsbw, int npvsepvznbsa, double giphy, bool kccebdhn, bool xgruvntwumc, int hfcvxlkfspivxi) {
	string kxyazaxpjukgmod = "nyb";
	string xezklkcfghuuq = "qggywbtyyhncdxzqmeafhadyqxnkzxjabymkrtfvvdpuwoxdxnsxkhrzzw";
	if (string("qggywbtyyhncdxzqmeafhadyqxnkzxjabymkrtfvvdpuwoxdxnsxkhrzzw") == string("qggywbtyyhncdxzqmeafhadyqxnkzxjabymkrtfvvdpuwoxdxnsxkhrzzw")) {
		int blj;
		for (blj = 6; blj > 0; blj--) {
			continue;
		}
	}
	if (string("nyb") == string("nyb")) {
		int rwojajllg;
		for (rwojajllg = 14; rwojajllg > 0; rwojajllg--) {
			continue;
		}
	}
	return string("mfdk");
}

double oeuncmc::ubsdieyysplks() {
	double mhyszvgvv = 8321;
	double msigobaedmwwyh = 24620;
	return 38685;
}

bool oeuncmc::urbsacvfpftirnyqblpbnevm(int wxmme, bool zfiwje, double zkbhybr, double dyjxknxvsce, bool tkclsknzuvhn, double usrvdn, int debmhbupzusawo, string jaziswbfhpltzy, int ebjse) {
	int svxqwwtsqekmlng = 2395;
	double oyawt = 6105;
	string vagjhsftedyu = "tmcxzltmygbsdacfemlmh";
	if (6105 != 6105) {
		int obhu;
		for (obhu = 50; obhu > 0; obhu--) {
			continue;
		}
	}
	if (6105 == 6105) {
		int tdvfi;
		for (tdvfi = 51; tdvfi > 0; tdvfi--) {
			continue;
		}
	}
	if (6105 != 6105) {
		int mm;
		for (mm = 62; mm > 0; mm--) {
			continue;
		}
	}
	return false;
}

string oeuncmc::pdzpqmcmyz(bool ukmqouij, string dxlid, double lfwebifeij, bool gittwigudeaj, string qdhuzxnwch, int mzzkzcfpm, int pmjobgtp) {
	bool nparmmenjburvx = false;
	int wqnadlirpwhy = 781;
	int nmlggbgfwa = 3036;
	bool gzclde = false;
	int tpcrqbyujwru = 4811;
	bool irrsomaogv = true;
	int wnumhi = 3636;
	int plehqsyctnxhi = 4726;
	if (4811 != 4811) {
		int owkbchrvkc;
		for (owkbchrvkc = 45; owkbchrvkc > 0; owkbchrvkc--) {
			continue;
		}
	}
	return string("aat");
}

double oeuncmc::hzsqmctiuebhvzvtb(double wknjhin, double nvpqv, int xqnbae, int jhhltbihu, int htlxjhmqdwzbx, int erqvbwtfsakq, int jqeorcrigjqkza) {
	bool yyxrikdxcz = false;
	string blkoenl = "uonhpzeimvwbxbjjjtdjajkfjykmuhohmkrnisbchahvaiifzeppwbqddjwlskftldh";
	string fwxytuls = "ocdcytcxcpyelmpaqkgctngdunjhfejiurnktqfplwwrcnyqlwzvpdialcrmqsepolfpmirwqnsyzitxetpfftgkmj";
	double flarlclryf = 28268;
	double mggtgxcuam = 78341;
	int mstohay = 1056;
	if (string("ocdcytcxcpyelmpaqkgctngdunjhfejiurnktqfplwwrcnyqlwzvpdialcrmqsepolfpmirwqnsyzitxetpfftgkmj") != string("ocdcytcxcpyelmpaqkgctngdunjhfejiurnktqfplwwrcnyqlwzvpdialcrmqsepolfpmirwqnsyzitxetpfftgkmj")) {
		int nxls;
		for (nxls = 12; nxls > 0; nxls--) {
			continue;
		}
	}
	if (string("uonhpzeimvwbxbjjjtdjajkfjykmuhohmkrnisbchahvaiifzeppwbqddjwlskftldh") == string("uonhpzeimvwbxbjjjtdjajkfjykmuhohmkrnisbchahvaiifzeppwbqddjwlskftldh")) {
		int bnjrsm;
		for (bnjrsm = 80; bnjrsm > 0; bnjrsm--) {
			continue;
		}
	}
	if (28268 == 28268) {
		int yyyizqnjm;
		for (yyyizqnjm = 95; yyyizqnjm > 0; yyyizqnjm--) {
			continue;
		}
	}
	if (78341 != 78341) {
		int azlrqklmc;
		for (azlrqklmc = 32; azlrqklmc > 0; azlrqklmc--) {
			continue;
		}
	}
	return 39946;
}

double oeuncmc::isbomktsamlwmdlhyhm(bool nzaasj) {
	bool ozhgpgrepjpbw = true;
	double mzgiwvxqhxvp = 24835;
	double wgwiffwb = 2406;
	if (24835 != 24835) {
		int zso;
		for (zso = 0; zso > 0; zso--) {
			continue;
		}
	}
	return 28285;
}

int oeuncmc::wufwvsnwglto(double nayiblr, double erpcsxjsqqwi, int mjujtzwtzxgfye, string jkaemnbohanel) {
	return 72754;
}

string oeuncmc::klssqpfkkumbbgwxodzkmyl(bool fqazhij) {
	bool erpmyubed = true;
	bool uhctqvpnpgb = true;
	bool ifkveklm = true;
	int zrgrzkf = 2930;
	double bmmnqkhqosjpjvr = 10400;
	bool srvcc = false;
	bool adzutltaac = true;
	if (true == true) {
		int walyrftc;
		for (walyrftc = 40; walyrftc > 0; walyrftc--) {
			continue;
		}
	}
	if (true != true) {
		int mmoqlkr;
		for (mmoqlkr = 45; mmoqlkr > 0; mmoqlkr--) {
			continue;
		}
	}
	return string("mjwfaxwbquwrhc");
}

string oeuncmc::mzdsqzjbrozsnumqvfyi(bool lzsohz, int hjlbynnnc, bool zinnwm, bool pqdbih, string zinrgkhjmtlc, int cjuthittopqj, int gpjqzxdqiokj, int wepsnjsf, bool qxswpajztc, string uvgshvn) {
	double awdsfugiajt = 18727;
	int vrqfwlvtacucmvi = 1761;
	int fyxiziiey = 2156;
	string rpwgzb = "laeknlepuescoqemvwohobkhjqpmcpxysxjjhlszuzqlissfrtcakxgobov";
	bool akkcciy = false;
	bool ijskrwbmb = false;
	if (false != false) {
		int dakhyrq;
		for (dakhyrq = 90; dakhyrq > 0; dakhyrq--) {
			continue;
		}
	}
	if (false == false) {
		int mdmuy;
		for (mdmuy = 66; mdmuy > 0; mdmuy--) {
			continue;
		}
	}
	if (18727 != 18727) {
		int ytmesldokl;
		for (ytmesldokl = 45; ytmesldokl > 0; ytmesldokl--) {
			continue;
		}
	}
	return string("jussx");
}

bool oeuncmc::qgtpnnqlnnrtl(int zhdvtxgohzbe, string drrzwnnrlwm, string lvnnexcnjseub, string tqoltdobsnldzez, int afgywglum, bool scpllrvj, bool wyjmws, bool pobbwr) {
	bool vbjdtnownqo = true;
	string dtwjkrxhenajhhc = "ybifrfkffpefezzmoyrbvqjdeprgthsxejoaiksrsvtjppxovqsiavjlfqsfdptuzclfvgsuabuhkqqtpittlzsxtttsdb";
	bool okralpawdjjpo = false;
	string tuqnchlbshahwot = "beoaavjvglhyfofolfcgnkvujsqfjggscynuqgntnwgzcqtcuyazfgbihlcpmzknuqjxxqgxlknrfv";
	if (false == false) {
		int gm;
		for (gm = 75; gm > 0; gm--) {
			continue;
		}
	}
	if (true == true) {
		int vixpko;
		for (vixpko = 47; vixpko > 0; vixpko--) {
			continue;
		}
	}
	if (string("beoaavjvglhyfofolfcgnkvujsqfjggscynuqgntnwgzcqtcuyazfgbihlcpmzknuqjxxqgxlknrfv") != string("beoaavjvglhyfofolfcgnkvujsqfjggscynuqgntnwgzcqtcuyazfgbihlcpmzknuqjxxqgxlknrfv")) {
		int zl;
		for (zl = 66; zl > 0; zl--) {
			continue;
		}
	}
	if (false != false) {
		int derm;
		for (derm = 33; derm > 0; derm--) {
			continue;
		}
	}
	if (string("beoaavjvglhyfofolfcgnkvujsqfjggscynuqgntnwgzcqtcuyazfgbihlcpmzknuqjxxqgxlknrfv") != string("beoaavjvglhyfofolfcgnkvujsqfjggscynuqgntnwgzcqtcuyazfgbihlcpmzknuqjxxqgxlknrfv")) {
		int sygqijsp;
		for (sygqijsp = 85; sygqijsp > 0; sygqijsp--) {
			continue;
		}
	}
	return false;
}

double oeuncmc::bovfpeblfdndnbgbiniqbze(string eolpyv) {
	int aifsomehoux = 4523;
	if (4523 == 4523) {
		int dewv;
		for (dewv = 0; dewv > 0; dewv--) {
			continue;
		}
	}
	return 75991;
}

double oeuncmc::ljddzlreogwqez(double kmgmsct, bool cagiwq, int cvoguuuhluyyx, double cmuzszotmhhwmqv, string hxuob, string lgvhtq, int ycblveutwh, int wddcronk, double flpoubznhxio, double szviz) {
	double qkvfqfliur = 651;
	double jaydm = 6246;
	bool oinpwfdoh = true;
	int mdpiejhvxgqurro = 5981;
	string xsatide = "efjfvzlpohvvarsxefksxfzojaonqeflsuuqpqqyfhpusugtxlcpvrvtgexsaqbwezl";
	double xyjtjtv = 28881;
	if (28881 != 28881) {
		int fm;
		for (fm = 90; fm > 0; fm--) {
			continue;
		}
	}
	if (6246 != 6246) {
		int dtpmuzj;
		for (dtpmuzj = 44; dtpmuzj > 0; dtpmuzj--) {
			continue;
		}
	}
	if (string("efjfvzlpohvvarsxefksxfzojaonqeflsuuqpqqyfhpusugtxlcpvrvtgexsaqbwezl") != string("efjfvzlpohvvarsxefksxfzojaonqeflsuuqpqqyfhpusugtxlcpvrvtgexsaqbwezl")) {
		int yedlaa;
		for (yedlaa = 82; yedlaa > 0; yedlaa--) {
			continue;
		}
	}
	return 52286;
}

bool oeuncmc::wqyehgisoeerdirvrb() {
	int npyivksxx = 1888;
	double wgjozlj = 7827;
	bool anzpgumbocme = true;
	if (1888 == 1888) {
		int mapajp;
		for (mapajp = 52; mapajp > 0; mapajp--) {
			continue;
		}
	}
	if (true == true) {
		int jpgdrg;
		for (jpgdrg = 80; jpgdrg > 0; jpgdrg--) {
			continue;
		}
	}
	if (true == true) {
		int eldtqzlq;
		for (eldtqzlq = 64; eldtqzlq > 0; eldtqzlq--) {
			continue;
		}
	}
	if (true != true) {
		int iewkmj;
		for (iewkmj = 31; iewkmj > 0; iewkmj--) {
			continue;
		}
	}
	return false;
}

bool oeuncmc::dxqichmhtthuu(double jlycsqgkllvqphw) {
	bool sbnkcingvg = false;
	bool ddzretaxzgh = false;
	string xqcjc = "vhxnwgkfinjdoawvoyapbdnbauhyjwpvntsgryqgkkuqdqiunbpokdiwrrudddmhnrmydbh";
	string hxwlaeistphlpid = "nhkfoowahnuwtgcgkowlibmdsmamvcanlnftfjjrmzhkpx";
	int gjxnlvvjalfeic = 708;
	if (708 == 708) {
		int nmj;
		for (nmj = 23; nmj > 0; nmj--) {
			continue;
		}
	}
	return true;
}

oeuncmc::oeuncmc() {
	this->klssqpfkkumbbgwxodzkmyl(false);
	this->mzdsqzjbrozsnumqvfyi(false, 1709, true, false, string("fyvkmpovmfjfzmixrxrfglrwbohtusjqxhdqvdcbqb"), 575, 2199, 2180, false, string("zxaoipcoodzsnikxvpbcwgiobfjoytgpvoamhnveqceiddjewvdoybjvlqyylccmz"));
	this->qgtpnnqlnnrtl(6630, string("yjrnfykjbkfmymiyajvswooxiaopftctexqqljntnhdxutidwhqdejixbddvjmkswcvgnytvfuu"), string("eekax"), string("ikqvtujicxklqyxpjhrgjmigpszveywjcpiopsmkyxcytxtzasikkebmzjuuokxdwtjhf"), 1609, false, true, true);
	this->bovfpeblfdndnbgbiniqbze(string("kkjejqpzjzsggtzofydufugzl"));
	this->ljddzlreogwqez(3510, true, 428, 10716, string("awjjtrbuv"), string("misgjvnxxnatxvwwqnlfmljblnsevtyuigmzvpsjhcopvsuuthmtctnzrjwdfihksadvrukvspnauz"), 6892, 708, 16270, 33925);
	this->wqyehgisoeerdirvrb();
	this->dxqichmhtthuu(38151);
	this->gayphgsvwrwxmxmfi(1194, false, 2785, 65456, 3864, true, 42172, string("zfftmzpgjvymnsnqsabaubmlmbkibcdikzdxmqdkudaqftmtdfhjmhnaqniqcllzlkpqygswbkfnjqxwmhuwltnvokjxznahof"));
	this->pvirbetlaaaydqhok(string("tgnhoiqinvlfknbqcycqjtyqnznofltladrorqvb"), string("dnma"), 2381, 81779, true, true, 1118);
	this->ubsdieyysplks();
	this->urbsacvfpftirnyqblpbnevm(2087, true, 39166, 3598, false, 9064, 2896, string("lpalqvembbfinvcodyaardoudmzwqtwjswdbufls"), 3451);
	this->pdzpqmcmyz(true, string("jyrrcosawgobnpfagvwybjemqfgtapjuytgccxijfpijkxicpcgsoedxcryoobipclkvexabcapssnrzlflvydssgsjcjnvqtatn"), 39222, true, string("zwgsxtytdlzzpvacjurykrctmjbjzaqvhttzvagptejbjuxuwkyhdjempvlgyfkfcwiskjhaxjvtpvnnajipxwwdtsg"), 209, 1986);
	this->hzsqmctiuebhvzvtb(48121, 6212, 6676, 409, 7063, 845, 650);
	this->isbomktsamlwmdlhyhm(true);
	this->wufwvsnwglto(5132, 9081, 4334, string("ylbvhxccckrwzcwtjsmqpqtg"));
	this->gybexbdwiydosyipichdk(12838, false, true, 796, true, 354, string("gcimlyvpwfoxeedtvxjysbvdoqidgphdqzhxfkhcxpfbmepgibfcwqhromivmrvfpiecozouod"), string("ukvspzvsetjbvzuasxxdbadsasocimucmjkjcbltlcvegpsufeyfmxgxukwpvzziduofkmozbfqetttzicokgjmhzwmfchds"), 770, true);
	this->vurwmjsvzlkzffufmvaekap(18896, 2630, string("oermwhddvtrvcxejsunaoqonlyaktcwicrcvphradbgkiniophqvtmnvndstbadfrhucazwchxlrcvyusudqgkea"), string("zhxujayctqicaglaoeaxhdnftuajqppdfjmhltddvsrhzrrlrylyagzvhqydrhgegbxdhxcgadbpfygzmmzyofwnuty"), true, 1404);
}

#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class yngridm {
public:
	double njwefrwgk;
	int zgthpj;
	int edhkgefwaymxj;
	yngridm();
	int ngmbdrcoonadvppjbrqnqg(int nsjwuaasloczn, double gcjmdwcmrxvbci, string baqwsle, int uxjukrwpbc, int naknys);
	void rrdjmtlqeqltqnxgmpkeiqq(double xokdgdzpb, double qupsl, string tiljzel, double kgrzbnwowqma);
	string dizlltbvmafldfgvvxixkov(double nhfknszmhxva, int vreylhlfkrdsr);
	string iwnubqmfmph(int uwmuej, double amopbdbwpcdc);
	string ycscbcdato(int efiieauhvnjseuc, string fmjgqpyip, string xkcqunf, int ygzdcaracj, int xmzdtjncgqkaf, int iuqejexzvqtqv, string tudniudj, double tmxhwgrqvo, bool cosccyhqbmuakbi);

protected:
	int xtizdtltwxf;
	double lntoyalxr;
	string zpzkkggpwschn;
	bool uwcieebpteamun;

	string vcwhhlwbygmvjd(double ckkioirffzuyyc, double shrpqj, string nlulrjndkxxsec, int vuoliinvnfg, int ardekwowtx, int ulhuubqhdt);
	double pjxpvlqsrx(string alwzjettyuckl, double qosqutnsewfpmet, int utcvdcsm, int dwimetkbd, bool rksdkttma, int dhlumxsig, bool auhzyrqwno, int ekdzihqpcqi);
	string ttcfkdhnnttfhavgivo(double uhfkxebepcd, bool ydbybtxlzzugqs, double xwjnmghvnzguvuu, bool fxwwdfybx, bool tajyqkvgi);
	int oadvlrabmepffzprilmvcli(bool gmpsfq, bool uepyyranxrepxz, string plfaomwlljiay, bool cgvkpghv, int kakzvkdcntdk, string upuktehrfzj, int hpyjlnutxdk);

private:
	double wwybvdhgjbrcdhi;
	int eeelhztgei;
	bool pgqni;

	bool gyezogieboadydaipra();
	bool fulahtkyvatparlppajdgqci(int wtkexrb, int niiams, bool dnpbozttzgwujk, string xeyelqgra);
	double iwapzjsbwwvlvukdvvhvvb(bool ouapuzcvl, int ttrvzhqinop, string nptcajt, string hsommwcwsalhg, int yfjvv);

};


bool yngridm::gyezogieboadydaipra() {
	double jqvnehqti = 32832;
	bool owrgdrzvmcf = true;
	bool krfpcsredifbgzq = false;
	return true;
}

bool yngridm::fulahtkyvatparlppajdgqci(int wtkexrb, int niiams, bool dnpbozttzgwujk, string xeyelqgra) {
	double eagrfihupysb = 51425;
	double opmvfqccxegvv = 16487;
	string nnlqmv = "qaoemqhgbqfmzqjihrxxnrqyeslfsjweuhcvueumvdweachxeckmtkczevlrnrxhddhabsvcqzhpprfqsstftmckqtdttbkexcau";
	string xxcwzun = "kpsykelxbmymjujdvyhlxdpyyynjkdcimxdaymlhhyrxbjlusdfqazieiivefokkfhqoipwgpaxighlsvsqbjcsggarkpizmwu";
	int nboltsgapv = 620;
	int cgwozcau = 1710;
	if (string("kpsykelxbmymjujdvyhlxdpyyynjkdcimxdaymlhhyrxbjlusdfqazieiivefokkfhqoipwgpaxighlsvsqbjcsggarkpizmwu") != string("kpsykelxbmymjujdvyhlxdpyyynjkdcimxdaymlhhyrxbjlusdfqazieiivefokkfhqoipwgpaxighlsvsqbjcsggarkpizmwu")) {
		int xpcotw;
		for (xpcotw = 74; xpcotw > 0; xpcotw--) {
			continue;
		}
	}
	if (1710 == 1710) {
		int hc;
		for (hc = 93; hc > 0; hc--) {
			continue;
		}
	}
	return false;
}

double yngridm::iwapzjsbwwvlvukdvvhvvb(bool ouapuzcvl, int ttrvzhqinop, string nptcajt, string hsommwcwsalhg, int yfjvv) {
	bool vibkfwaktwfrce = true;
	double rpoyo = 32700;
	string zzxfixqcfv = "hybetcwuttglcbsztmdtzkinyshhaqiufuidxeoktepzjfzyvgaqrzwsbrfmgvwdveanimknsxvxqzurljbd";
	return 87278;
}

string yngridm::vcwhhlwbygmvjd(double ckkioirffzuyyc, double shrpqj, string nlulrjndkxxsec, int vuoliinvnfg, int ardekwowtx, int ulhuubqhdt) {
	bool yjhvfzkn = false;
	double azxliudvsnru = 28023;
	string oghaqfjz = "alpsjleujdsfsnrmzkzbooaweaubwwoswidgfjzyulpghhybqkrunsetzcicreuvahossvmigehvomqtshevxovtap";
	string irwmzleq = "nrsbtetvutrfdwzhovuatylvbxcckpdrzbjdaqjpnvtmmgzhodmsepezunskmnvnpskxcoqaqnljud";
	string xjjyjwqzljajwx = "topmlevduetnadgzetuclemlnsuwcvylerehdlpbhntcjsgrlypnnmuvpdmbpvnr";
	double tfqig = 57908;
	bool rugacaxxz = true;
	double ynboblkv = 15721;
	bool nhdkwudzhrgrm = true;
	if (28023 != 28023) {
		int uqmizarbp;
		for (uqmizarbp = 72; uqmizarbp > 0; uqmizarbp--) {
			continue;
		}
	}
	return string("udbgf");
}

double yngridm::pjxpvlqsrx(string alwzjettyuckl, double qosqutnsewfpmet, int utcvdcsm, int dwimetkbd, bool rksdkttma, int dhlumxsig, bool auhzyrqwno, int ekdzihqpcqi) {
	bool tbfsiwkpmxxp = true;
	int pmphvaz = 5762;
	string mtohnlutqisdh = "bvprldnyduoatwrsvqhjexrajtltuteamknhmbcgnfogfnftwvelgzzwhvkcotdpozpqad";
	string fffpbwpynwxmlwb = "eeczmnuifmiwnyykknpeiwfjwrhhbbclenkrjwnxfrrnhioraztoyqemreattqylpvbrqqrqrhhapgcdnswn";
	bool iabalyj = true;
	int iutrxdzbboythk = 3458;
	bool hksitpibmioq = true;
	string cjvtrmhvsak = "pkiltbjcyejjavnfftvjpnrgqoeaaltvkrdzuuihehfmpfaqrujgsvmiykvqriqpfgshxquoaljvuhtumgaretsewkzoipjvzb";
	if (true != true) {
		int qgrqpgouxm;
		for (qgrqpgouxm = 56; qgrqpgouxm > 0; qgrqpgouxm--) {
			continue;
		}
	}
	return 87719;
}

string yngridm::ttcfkdhnnttfhavgivo(double uhfkxebepcd, bool ydbybtxlzzugqs, double xwjnmghvnzguvuu, bool fxwwdfybx, bool tajyqkvgi) {
	string oavnecsgm = "pxafnspemavqawkcvlpoeaesldjztfkfvvxftlkvohqvilxsezhgkvdokmeyklepglrbqmwpfgtauep";
	string lffupeczfcswxur = "tpmof";
	double wxonttfa = 16439;
	int kedfftvuwoou = 2117;
	string sllvrrzslf = "xxpkjnxgltcdeqfkwcmcalrbqtdllrbjjaqwcujkqjnply";
	return string("zsqtxfrzmogiwkfaozo");
}

int yngridm::oadvlrabmepffzprilmvcli(bool gmpsfq, bool uepyyranxrepxz, string plfaomwlljiay, bool cgvkpghv, int kakzvkdcntdk, string upuktehrfzj, int hpyjlnutxdk) {
	int hfdzvtw = 3765;
	double tkqulhqssm = 18962;
	bool mtczrgniomrmgop = false;
	bool zxfncq = true;
	bool obqdwdbp = false;
	int ixqpcxs = 3778;
	string ztlkjlgp = "ltdg";
	int tsbyenpnhlink = 1721;
	if (1721 == 1721) {
		int dvqem;
		for (dvqem = 78; dvqem > 0; dvqem--) {
			continue;
		}
	}
	if (1721 == 1721) {
		int dabynd;
		for (dabynd = 50; dabynd > 0; dabynd--) {
			continue;
		}
	}
	if (18962 == 18962) {
		int ejj;
		for (ejj = 32; ejj > 0; ejj--) {
			continue;
		}
	}
	if (true != true) {
		int wgiwrpfhf;
		for (wgiwrpfhf = 46; wgiwrpfhf > 0; wgiwrpfhf--) {
			continue;
		}
	}
	return 75793;
}

int yngridm::ngmbdrcoonadvppjbrqnqg(int nsjwuaasloczn, double gcjmdwcmrxvbci, string baqwsle, int uxjukrwpbc, int naknys) {
	return 71357;
}

void yngridm::rrdjmtlqeqltqnxgmpkeiqq(double xokdgdzpb, double qupsl, string tiljzel, double kgrzbnwowqma) {
	bool kfyyhrphetans = false;
	string jokvythumee = "xpgotztvxkbaoz";
	if (false == false) {
		int zevcynxdi;
		for (zevcynxdi = 0; zevcynxdi > 0; zevcynxdi--) {
			continue;
		}
	}
	if (false != false) {
		int bxeii;
		for (bxeii = 67; bxeii > 0; bxeii--) {
			continue;
		}
	}
	if (string("xpgotztvxkbaoz") != string("xpgotztvxkbaoz")) {
		int jnpbhnbxu;
		for (jnpbhnbxu = 92; jnpbhnbxu > 0; jnpbhnbxu--) {
			continue;
		}
	}
	if (string("xpgotztvxkbaoz") != string("xpgotztvxkbaoz")) {
		int uxf;
		for (uxf = 86; uxf > 0; uxf--) {
			continue;
		}
	}

}

string yngridm::dizlltbvmafldfgvvxixkov(double nhfknszmhxva, int vreylhlfkrdsr) {
	int dwieecrnwdftu = 560;
	int czzicam = 7038;
	bool nvaxl = false;
	if (false == false) {
		int pwcsjymxes;
		for (pwcsjymxes = 2; pwcsjymxes > 0; pwcsjymxes--) {
			continue;
		}
	}
	if (false != false) {
		int ocfdp;
		for (ocfdp = 94; ocfdp > 0; ocfdp--) {
			continue;
		}
	}
	if (false == false) {
		int tj;
		for (tj = 78; tj > 0; tj--) {
			continue;
		}
	}
	if (7038 == 7038) {
		int kjuin;
		for (kjuin = 85; kjuin > 0; kjuin--) {
			continue;
		}
	}
	if (false == false) {
		int iu;
		for (iu = 41; iu > 0; iu--) {
			continue;
		}
	}
	return string("onojxxmjephrzw");
}

string yngridm::iwnubqmfmph(int uwmuej, double amopbdbwpcdc) {
	double yfuazyaoxawmfv = 19601;
	double fqwjeqaarnbyhq = 61802;
	double ypkzdugczlitr = 51236;
	double uvupaipfnvb = 41437;
	if (61802 == 61802) {
		int amxsalp;
		for (amxsalp = 24; amxsalp > 0; amxsalp--) {
			continue;
		}
	}
	return string("nihhkuwsnywydawas");
}

string yngridm::ycscbcdato(int efiieauhvnjseuc, string fmjgqpyip, string xkcqunf, int ygzdcaracj, int xmzdtjncgqkaf, int iuqejexzvqtqv, string tudniudj, double tmxhwgrqvo, bool cosccyhqbmuakbi) {
	bool ulsjrhzv = true;
	double hookvmf = 1254;
	string hmlvc = "hvdcsbyqtlbszvtolivdlilcbjjevoemyzgrpaizupqauxqkoxbpsdolzlyiqqyxwuttymowecbmoj";
	string iqlwvgin = "xnmthqyyrvlrkdregfimzsoehxdkgboqqrquizcatquhwuglmxvhldkiynyznzipyhesmdnmmepqiraoupwzyujbtukaexeoxmz";
	bool kodaxbopfvjh = true;
	int mkltre = 1084;
	double ebemcjqfvkes = 9952;
	bool ieoom = true;
	string hgdpgmwpvgalgw = "cmnxukjlpiddvkzvmenyzhyrgvmqneuhkmvvljwjgosnqlzcjsdawbkdizgmliqyfwiaxwc";
	if (string("xnmthqyyrvlrkdregfimzsoehxdkgboqqrquizcatquhwuglmxvhldkiynyznzipyhesmdnmmepqiraoupwzyujbtukaexeoxmz") == string("xnmthqyyrvlrkdregfimzsoehxdkgboqqrquizcatquhwuglmxvhldkiynyznzipyhesmdnmmepqiraoupwzyujbtukaexeoxmz")) {
		int omh;
		for (omh = 93; omh > 0; omh--) {
			continue;
		}
	}
	if (true == true) {
		int guualce;
		for (guualce = 42; guualce > 0; guualce--) {
			continue;
		}
	}
	if (1254 != 1254) {
		int kav;
		for (kav = 86; kav > 0; kav--) {
			continue;
		}
	}
	if (1084 == 1084) {
		int ocdysebje;
		for (ocdysebje = 31; ocdysebje > 0; ocdysebje--) {
			continue;
		}
	}
	return string("vyhncoeagotntkgpnzfh");
}

yngridm::yngridm() {
	this->ngmbdrcoonadvppjbrqnqg(606, 13285, string("jbztppgablzfqawcbglgguijimomdaklgenwpobzytvicnvjoulmrbkcybefsyzhctzvyvvnroawvgsinykmgxsk"), 2446, 547);
	this->rrdjmtlqeqltqnxgmpkeiqq(41429, 55358, string("yyvqiyipjiafessgykqvlznioyjjyjbchybrdkfvfgaqmwgjslypnlbueaaviurwxnwzygzuxhbedzemltqbibm"), 21445);
	this->dizlltbvmafldfgvvxixkov(22377, 3619);
	this->iwnubqmfmph(1109, 67677);
	this->ycscbcdato(981, string("zdomwcsqejbqibxpy"), string("owqs"), 1181, 2945, 2837, string("tofozbnijucsojjsyacrmjivpgtjf"), 18079, false);
	this->vcwhhlwbygmvjd(57702, 14090, string("haafizeejrcafyapulvwcnyaeypctnmbdirnhcfwqzgaljnztimpugrdahdswryzfrzdnafvoplpekgsubhxamc"), 2205, 1222, 8691);
	this->pjxpvlqsrx(string("vwfcbsvpmhcx"), 37439, 627, 2136, true, 4093, true, 2081);
	this->ttcfkdhnnttfhavgivo(6939, true, 13019, true, true);
	this->oadvlrabmepffzprilmvcli(true, true, string("mjweslatenywritlbfxfqtrggrqmogmbisnlazrojogrmpr"), true, 1856, string("mkdcdauekpcywgiknrpnpjtzjctzuhwzlaisxjmzvebufjbxupeupialoevxhjhrntibzkmflbalkorjhfrnztzfx"), 355);
	this->gyezogieboadydaipra();
	this->fulahtkyvatparlppajdgqci(2458, 282, true, string("heevakimonnxhnuzeccpatpisvnhqgaanynozvqkagwmsdkaaaoof"));
	this->iwapzjsbwwvlvukdvvhvvb(false, 351, string("pvzmcfgydfdkrafwofjxwomzmffisostygvwxrzup"), string("pwxogajnafnnbkhynmebtattbukamkwmhyldgiovqretmikcojfsjmqenozkddprafitfdbuxvzrjtdt"), 7079);
}

#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class dhmihua {
public:
	int pqopzgscnpjj;
	string atrpgo;
	double weavomayu;
	bool egfhfitzxmff;
	dhmihua();
	bool dfvvbaiqzzs(int vnaoxmutkhsnn, double kszheaee);
	bool bwmcbdevrpngq(double qyphceveou, double iqamizyldjlltq, bool yjfbdtya, bool aiqow, bool qzehrwevorfridw, int agedczgrqnvxllp, string zruhajybpjsr, bool ngjwr, string rnoxbglswmhzh);
	void qxqkwpvehv(double adjcug, int gxxhbsrpd, double qehzvh, bool ktgyuoaui, string xyqpnwce);
	void oynvwwvoynhvh(bool ltphcliyatvfkux, double rxegzckgm, string kugmhdoeehsg, double bixwlxmraydhs, bool fzpidzutinkbunk, double kiqzaghgkhkrm, int pgefvdpfssrhut, double muhsjzuptma, string wusdftkhkpdqw, double rwpedo);

protected:
	string bxoarmoikffili;

	string wblwdanpjgqybdcsrofdu(string obnwoyzil, bool xowupbwmmdlcs, string zitcbwk, string tznmscxovrngbz);
	double ufntlpbdodaeddtqct(bool fnusjvdepd, int fahmoiluljptwc, bool jhpivf, string mgpyameg);

private:
	double iffmqxdbafs;
	bool pzhwzcp;
	bool jfrjpknnhgk;
	bool lnhtuveof;

	void laicolmckmzwrpqcuwozgmfdl(bool xvubshkf, string wvpgijixspra, double kgfxwzhc, double btsulrsatfltl, bool skmvme, bool hpnlnrozql, string ajsonqfclsic);
	double cfkpvbtiuzvrbwua(bool tpbmdddnhhju, string fvmkvaekoz, int vhflihwgbgijcic);
	string eputunjkxtvlotiv(int yauswefmuwoea, double cthzbgba, bool yvflspghvcchgfx, double rzfryvjoepnajc, int wtuanptjofky, double zagxt, string ajsqwndcd, double uiidr, double rvftajex, double razawfpqcmsttpf);

};


void dhmihua::laicolmckmzwrpqcuwozgmfdl(bool xvubshkf, string wvpgijixspra, double kgfxwzhc, double btsulrsatfltl, bool skmvme, bool hpnlnrozql, string ajsonqfclsic) {
	double qgeeyuzg = 43082;
	double xglypjwutlkud = 84123;
	bool lnljvjcnrswac = false;
	string vlcqvpbtvhbi = "lehwnumpadzjkkbbhmhqixpjqwsxjhayjrmgzdrjzj";
	string rtkbwkc = "fudrwiwunksgbjniptpaerfinurislivnwrdeuwhwmyf";
	double cgvlu = 53664;
	bool ujkzydecz = true;
	if (false != false) {
		int vgdsevs;
		for (vgdsevs = 16; vgdsevs > 0; vgdsevs--) {
			continue;
		}
	}

}

double dhmihua::cfkpvbtiuzvrbwua(bool tpbmdddnhhju, string fvmkvaekoz, int vhflihwgbgijcic) {
	double hytsfyqdcmo = 20904;
	if (20904 != 20904) {
		int nn;
		for (nn = 27; nn > 0; nn--) {
			continue;
		}
	}
	if (20904 == 20904) {
		int owk;
		for (owk = 63; owk > 0; owk--) {
			continue;
		}
	}
	if (20904 == 20904) {
		int ifda;
		for (ifda = 31; ifda > 0; ifda--) {
			continue;
		}
	}
	return 45883;
}

string dhmihua::eputunjkxtvlotiv(int yauswefmuwoea, double cthzbgba, bool yvflspghvcchgfx, double rzfryvjoepnajc, int wtuanptjofky, double zagxt, string ajsqwndcd, double uiidr, double rvftajex, double razawfpqcmsttpf) {
	int gqeuwmveaxkedh = 2226;
	double lkmncouzrv = 22721;
	bool ohycwzpfkeel = true;
	bool gvaiksfdbkuyw = false;
	string wnjywyyh = "axtoftofqxposii";
	bool oomblrcsv = true;
	double pxedjjuh = 74684;
	int tiynaqbvyii = 4329;
	int dwtdimpasg = 6449;
	if (4329 != 4329) {
		int pxippr;
		for (pxippr = 9; pxippr > 0; pxippr--) {
			continue;
		}
	}
	if (74684 == 74684) {
		int uxlb;
		for (uxlb = 68; uxlb > 0; uxlb--) {
			continue;
		}
	}
	if (false != false) {
		int ebyyyqwxtd;
		for (ebyyyqwxtd = 64; ebyyyqwxtd > 0; ebyyyqwxtd--) {
			continue;
		}
	}
	if (false == false) {
		int zpm;
		for (zpm = 8; zpm > 0; zpm--) {
			continue;
		}
	}
	return string("sez");
}

string dhmihua::wblwdanpjgqybdcsrofdu(string obnwoyzil, bool xowupbwmmdlcs, string zitcbwk, string tznmscxovrngbz) {
	return string("wihsg");
}

double dhmihua::ufntlpbdodaeddtqct(bool fnusjvdepd, int fahmoiluljptwc, bool jhpivf, string mgpyameg) {
	double sgcytategfhse = 39065;
	return 84839;
}

bool dhmihua::dfvvbaiqzzs(int vnaoxmutkhsnn, double kszheaee) {
	bool xavwagzlowe = true;
	string echhonycdugtnj = "zlqedhjtzxjbpuwtrrnzjcfadqwjextygibqowawfkblmorcfsmtdugbjzglqxvdsdvdjp";
	bool lxsrqozso = true;
	string yongcxat = "wddexjzkmtrnbvx";
	int ocprjbpissptwz = 6048;
	bool dpvdssdvic = true;
	int zvsahm = 744;
	double hkjvchh = 17004;
	bool edhxe = true;
	if (6048 != 6048) {
		int hdpmn;
		for (hdpmn = 21; hdpmn > 0; hdpmn--) {
			continue;
		}
	}
	if (744 != 744) {
		int yisaklqp;
		for (yisaklqp = 89; yisaklqp > 0; yisaklqp--) {
			continue;
		}
	}
	if (true == true) {
		int jfwcepfnsl;
		for (jfwcepfnsl = 77; jfwcepfnsl > 0; jfwcepfnsl--) {
			continue;
		}
	}
	return true;
}

bool dhmihua::bwmcbdevrpngq(double qyphceveou, double iqamizyldjlltq, bool yjfbdtya, bool aiqow, bool qzehrwevorfridw, int agedczgrqnvxllp, string zruhajybpjsr, bool ngjwr, string rnoxbglswmhzh) {
	int rodach = 2655;
	int xjllirve = 8430;
	bool wiihiechjtue = true;
	string zfiakx = "sfcojgvagcxslklldkgscvalymxayfdywkydsmuregoqappsgqfhgqneogfbokeovalohdppjc";
	bool mghatnnpjyz = true;
	bool sskiiszfdsgd = false;
	string pzsjzbgnbgcvx = "ejubjntqetdljiqbxebufvimroosvmsgnwbdaoymsuglescwxasbjfpyozaaksfdddqduobvakrzprjzfgeotjizratpga";
	int nudawrootqljpnq = 2181;
	if (2181 != 2181) {
		int umtlnxvdwg;
		for (umtlnxvdwg = 26; umtlnxvdwg > 0; umtlnxvdwg--) {
			continue;
		}
	}
	if (true == true) {
		int oivy;
		for (oivy = 52; oivy > 0; oivy--) {
			continue;
		}
	}
	return true;
}

void dhmihua::qxqkwpvehv(double adjcug, int gxxhbsrpd, double qehzvh, bool ktgyuoaui, string xyqpnwce) {
	int adbyzkgw = 4777;
	string pbnto = "tomaqnxlmldukogvldtzopejtkiyvsoaev";
	if (string("tomaqnxlmldukogvldtzopejtkiyvsoaev") != string("tomaqnxlmldukogvldtzopejtkiyvsoaev")) {
		int erwrtrrzw;
		for (erwrtrrzw = 24; erwrtrrzw > 0; erwrtrrzw--) {
			continue;
		}
	}
	if (4777 == 4777) {
		int shz;
		for (shz = 43; shz > 0; shz--) {
			continue;
		}
	}
	if (string("tomaqnxlmldukogvldtzopejtkiyvsoaev") != string("tomaqnxlmldukogvldtzopejtkiyvsoaev")) {
		int angoalmvp;
		for (angoalmvp = 70; angoalmvp > 0; angoalmvp--) {
			continue;
		}
	}
	if (4777 != 4777) {
		int wpafmt;
		for (wpafmt = 24; wpafmt > 0; wpafmt--) {
			continue;
		}
	}

}

void dhmihua::oynvwwvoynhvh(bool ltphcliyatvfkux, double rxegzckgm, string kugmhdoeehsg, double bixwlxmraydhs, bool fzpidzutinkbunk, double kiqzaghgkhkrm, int pgefvdpfssrhut, double muhsjzuptma, string wusdftkhkpdqw, double rwpedo) {
	string digwa = "p";
	string ofwcsixyia = "jicteypvpclvqvlzwjnwyyicrmxwvegcrpkhstxblsqisfahddhiyinojx";
	int nfhwxv = 3622;
	double qthhx = 17464;
	int rqcfztxvko = 1618;
	if (17464 == 17464) {
		int cugflpelot;
		for (cugflpelot = 46; cugflpelot > 0; cugflpelot--) {
			continue;
		}
	}
	if (3622 != 3622) {
		int qdtg;
		for (qdtg = 34; qdtg > 0; qdtg--) {
			continue;
		}
	}
	if (1618 != 1618) {
		int wopvzx;
		for (wopvzx = 4; wopvzx > 0; wopvzx--) {
			continue;
		}
	}

}

dhmihua::dhmihua() {
	this->dfvvbaiqzzs(1771, 24010);
	this->bwmcbdevrpngq(66821, 1885, false, false, true, 1287, string("sjrbtegkohnxvnadpjxghcvdqonxdbitnrzvqcbqucemgfqtowbhama"), true, string("pmcgcdvimjcpkwjgnkbmokjtlquxgsascxuoecldjfjimxakelknjovkvvxdolteuiazssjskohxcpnvnqbc"));
	this->qxqkwpvehv(55242, 3824, 2999, false, string("yexmzzgdmzedmoxhfxlvxnrvwpdtyrmxlbbbqimiohpahjavsunrjhdz"));
	this->oynvwwvoynhvh(false, 8761, string("vkpputykeiocwkyzkkzscivstiyaozzfdxtrlwaoqrvzsybhhurnbrgukhyhpszjemitpygbalszgaanx"), 21891, false, 18036, 2524, 127, string("galcytxanemmvntlkikjikuxjzfaoytgejdwyjnvfpmfbjis"), 2367);
	this->wblwdanpjgqybdcsrofdu(string("rxnorpuwxsmjigcxulmpzxapnkgvojuvfudjstikkqyqbydpwdzxldynboufhxevigoljbcdyrczcysqogeczremqznyyqmkrvs"), false, string("gtbbnktdvkwoyyztehoazrrwjwarfuusvisgn"), string("vvmvevjgmicxg"));
	this->ufntlpbdodaeddtqct(false, 9729, true, string("vaejewewzosxwdemuyqjvavhmpgmlkoxwfrtkupn"));
	this->laicolmckmzwrpqcuwozgmfdl(true, string("rkdshb"), 14616, 21894, false, true, string("mngasfunx"));
	this->cfkpvbtiuzvrbwua(true, string("ilcmypzkexlukatutixsdfyeackwxglrykjrppcwlqysbwsgvunmhjpywbqwlbbpgpecerlwniv"), 4958);
	this->eputunjkxtvlotiv(4292, 78412, true, 20226, 887, 11539, string("vtrg"), 21695, 59908, 48399);
}
