#include "GUI.h"
#include "Menu.h"
#include "RenderManager.h"
#include <algorithm>
#include "tinyxml2.h"
#include "Controls.h"

float MenuAlpha = 0.05f;
float MenuAlpha3 = 0.05f;
float MenuAlpha5 = 0.05f;
float MenuAlpha7 = 0.05f;
float MenuAlpha9 = 0.05f;
float MenuAlpha11 = 0.05f;
float MenuAlpha13 = 0.05f;
float MenuAlpha15 = 0.05f;
float MenuAlpha17 = 0.05f;
float MenuAlpha19 = 0.05f;
float MenuAlpha21 = 0.05f;
float Globals::MenuAlpha22 = MenuAlpha21;
float Globals::MenuAlpha20 = MenuAlpha19;
float Globals::MenuAlpha18 = MenuAlpha17;
float Globals::MenuAlpha16 = MenuAlpha15;
float Globals::MenuAlpha14 = MenuAlpha13;
float Globals::MenuAlpha12 = MenuAlpha11;
float Globals::MenuAlpha10 = MenuAlpha9;
float Globals::MenuAlpha8 = MenuAlpha7;
float Globals::MenuAlpha6 = MenuAlpha5;
float Globals::MenuAlpha2 = MenuAlpha;
float Globals::MenuAlpha4 = MenuAlpha3;

CGUI GUI;
bool SaveFile = false;
bool LoadFile = false;

CGUI::CGUI()
{

}

#define UI_CURSORSIZE       12
#define UI_CURSORFILL       Color(Menu::Window.SettingsTab.MenuR.GetValue(), Menu::Window.SettingsTab.MenuG.GetValue(), Menu::Window.SettingsTab.MenuB.GetValue(),MenuAlpha)
#define UI_CURSOROUTLINE    Color(Menu::Window.SettingsTab.MenuR.GetValue(), Menu::Window.SettingsTab.MenuG.GetValue(), Menu::Window.SettingsTab.MenuB.GetValue(),MenuAlpha)
#define UI_WIN_TOPHEIGHT	26
#define UI_WIN_TITLEHEIGHT	0
#define UI_TAB_WIDTH		150
#define UI_TAB_HEIGHT		32
#define UI_WIN_CLOSE_X		20
#define UI_WIN_CLOSE_Y      6
#define UI_CHK_SIZE			16
#define UI_COL_MAIN			Color(Menu::Window.SettingsTab.MenuR.GetValue(), Menu::Window.SettingsTab.MenuG.GetValue(), Menu::Window.SettingsTab.MenuB.GetValue(), MenuAlpha11)
#define UI_COL_SHADOW		Color(0, 0, 0, MenuAlpha)
#define COL_WHITE           Color(255, 100, 50, MenuAlpha17)
#define UI_COL_MAINE        Color(0, 204, 0, MenuAlpha)
#define UI_COL_MAINDARK     Color(Menu::Window.SettingsTab.MenuR.GetValue(), Menu::Window.SettingsTab.MenuG.GetValue(), Menu::Window.SettingsTab.MenuB.GetValue(), MenuAlpha)
#define UI_COURSOUR			Color(Menu::Window.SettingsTab.MenuR.GetValue(), Menu::Window.SettingsTab.MenuG.GetValue(), Menu::Window.SettingsTab.MenuB.GetValue(), MenuAlpha)
#define UI_COL_FADEMAIN     Color(Menu::Window.SettingsTab.MenuR.GetValue(), Menu::Window.SettingsTab.MenuG.GetValue(), Menu::Window.SettingsTab.MenuB.GetValue(), MenuAlpha15)
#define UI_COL_SHADOW		Color(0, 0, 0, MenuAlpha)
#define UI_COL_CLIENTBACK   Color(Menu::Window.SettingsTab.MenuR.GetValue(), Menu::Window.SettingsTab.MenuG.GetValue(), Menu::Window.SettingsTab.MenuB.GetValue(), MenuAlpha)
#define UI_COL_TABSEPERATOR Color(Menu::Window.SettingsTab.MenuR.GetValue(), Menu::Window.SettingsTab.MenuG.GetValue(), Menu::Window.SettingsTab.MenuB.GetValue(), MenuAlpha7)
#define UI_COL_TABTEXT      Color(Menu::Window.SettingsTab.MenuR.GetValue(), Menu::Window.SettingsTab.MenuG.GetValue(), Menu::Window.SettingsTab.MenuB.GetValue(), MenuAlpha)
#define UI_COL_GROUPOUTLINE Color(Menu::Window.SettingsTab.MenuR.GetValue(), Menu::Window.SettingsTab.MenuG.GetValue(), Menu::Window.SettingsTab.MenuB.GetValue(), MenuAlpha)

void CGUI::Draw()
{
	bool ShouldDrawCursor = false;

	for (auto window : Windows)
	{
		if (window->m_bIsOpen)
			MenuAlpha = min(MenuAlpha + 6, 255);
		else
			MenuAlpha = max(MenuAlpha - 6, 0);

		if (window->m_bIsOpen)
			MenuAlpha5 = min(MenuAlpha5 + 6, 120);
		else
			MenuAlpha5 = max(MenuAlpha5 - 6, 0);

		if (window->m_bIsOpen)
			MenuAlpha3 = min(MenuAlpha3 + 6, 15);
		else
			MenuAlpha3 = max(MenuAlpha3 - 6, 0);

		if (window->m_bIsOpen)
			MenuAlpha7 = min(MenuAlpha7 + 6, 200);
		else
			MenuAlpha7 = max(MenuAlpha7 - 6, 0);

		if (window->m_bIsOpen)
			MenuAlpha9 = min(MenuAlpha9 + 6, 245);
		else
			MenuAlpha9 = max(MenuAlpha9 - 6, 0);

		if (window->m_bIsOpen)
			MenuAlpha11 = min(MenuAlpha11 + 6, 80);
		else
			MenuAlpha11 = max(MenuAlpha11 - 6, 0);

		if (window->m_bIsOpen)
			MenuAlpha13 = min(MenuAlpha13 + 6, 140);
		else
			MenuAlpha13 = max(MenuAlpha13 - 6, 0);

		if (window->m_bIsOpen)
			MenuAlpha15 = min(MenuAlpha15 + 6, 40);
		else
			MenuAlpha15 = max(MenuAlpha15 - 6, 0);

		if (window->m_bIsOpen)
			MenuAlpha17 = min(MenuAlpha17 + 6, 50);
		else
			MenuAlpha17 = max(MenuAlpha17 - 6, 0);

		if (window->m_bIsOpen)
			MenuAlpha21 = min(MenuAlpha21 + 6, 1);
		else
			MenuAlpha21 = max(MenuAlpha21 - 6, 0);

		if (window->m_bIsOpen)
			MenuAlpha19 = min(MenuAlpha19 + 6, 100);
		else
			MenuAlpha19 = max(MenuAlpha19 - 6, 0);
		{
			ShouldDrawCursor = true;
			DrawWindow(window);
		}
	}
	if (ShouldDrawCursor)
	{
		Render::Clear(Mouse.x + 1, Mouse.y, 1, 17, Color(3, 6, 26, MenuAlpha));
		for (int i = 0; i < 11; i++)
			Render::Clear(Mouse.x + 2 + i, Mouse.y + 1 + i, 1, 1, Color(3, 6, 26, MenuAlpha));
		Render::Clear(Mouse.x + 8, Mouse.y + 12, 5, 1, Color(3, 6, 26, MenuAlpha));
		Render::Clear(Mouse.x + 8, Mouse.y + 13, 1, 1, Color(3, 6, 26, MenuAlpha));
		Render::Clear(Mouse.x + 9, Mouse.y + 14, 1, 2, Color(3, 6, 26, MenuAlpha));
		Render::Clear(Mouse.x + 10, Mouse.y + 16, 1, 2, Color(3, 6, 26, MenuAlpha));
		Render::Clear(Mouse.x + 8, Mouse.y + 18, 2, 1, Color(3, 6, 26, MenuAlpha));
		Render::Clear(Mouse.x + 7, Mouse.y + 16, 1, 2, Color(3, 6, 26, MenuAlpha));
		Render::Clear(Mouse.x + 6, Mouse.y + 14, 1, 2, Color(3, 6, 26, MenuAlpha));
		Render::Clear(Mouse.x + 5, Mouse.y + 13, 1, 1, Color(3, 6, 26, MenuAlpha));
		Render::Clear(Mouse.x + 4, Mouse.y + 14, 1, 1, Color(3, 6, 26, MenuAlpha));
		Render::Clear(Mouse.x + 3, Mouse.y + 15, 1, 1, Color(3, 6, 26, MenuAlpha));
		Render::Clear(Mouse.x + 2, Mouse.y + 16, 1, 1, Color(3, 6, 26, MenuAlpha));
		for (int i = 0; i < 4; i++)
			Render::Clear(Mouse.x + 2 + i, Mouse.y + 2 + i, 1, 14 - (i * 2), Color(Menu::Window.SettingsTab.MenuR.GetValue() - (i * 4), Menu::Window.SettingsTab.MenuG.GetValue() - (i * 4), Menu::Window.SettingsTab.MenuB.GetValue() - (i * 4), MenuAlpha));
		Render::Clear(Mouse.x + 6, Mouse.y + 6, 1, 8, UI_COURSOUR);
		Render::Clear(Mouse.x + 7, Mouse.y + 7, 1, 9, UI_COURSOUR);   //34, 53, 255
		for (int i = 0; i < 4; i++)
			Render::Clear(Mouse.x + 8 + i, Mouse.y + 8 + i, 1, 4 - i, Color(Menu::Window.SettingsTab.MenuR.GetValue() - (i * 4), Menu::Window.SettingsTab.MenuG.GetValue() - (i * 4), Menu::Window.SettingsTab.MenuB.GetValue() - (i * 4), MenuAlpha));
		Render::Clear(Mouse.x + 8, Mouse.y + 14, 1, 4, UI_COURSOUR);
		Render::Clear(Mouse.x + 9, Mouse.y + 16, 1, 2, UI_COURSOUR);
	}
}

void CGUI::Update()
{
	std::copy(keys, keys + 255, oldKeys);
	for (int x = 0; x < 255; x++) {
		keys[x] = static_cast<bool>(GetAsyncKeyState(x));
	}

	POINT mp; GetCursorPos(&mp);
	ScreenToClient(GetForegroundWindow(), &mp);
	Mouse.x = mp.x; Mouse.y = mp.y;

	RECT Screen = Render::GetViewport();

	for (auto bind : WindowBinds)
	{
		if (GetKeyPress(bind.first) && bind.second != nullptr)
		{
			bind.second->Toggle();
		}
	}

	if (IsDragging && !GetKeyState(VK_LBUTTON))
	{
		IsDragging = false;
		DraggingWindow = nullptr;
	}

	if (IsDragging && GetKeyState(VK_LBUTTON) && !GetKeyPress(VK_LBUTTON))
	{
		if (DraggingWindow)
		{
			DraggingWindow->m_x = Mouse.x - DragOffsetX;
			DraggingWindow->m_y = Mouse.y - DragOffsetY;
		}
	}

	for (auto window : Windows)
	{
		if (window->m_bIsOpen)
		{
			bool bCheckWidgetClicks = false;

			if (GetKeyPress(VK_LBUTTON))
			{
				bCheckWidgetClicks = true;

				if (IsMouseInRegion(window->m_y, window->m_x + window->m_iWidth, window->m_y + window->m_iHeight, window->m_x))
				{
					if (IsMouseInRegion(window->m_y, window->m_x + window->m_iWidth, window->m_y + UI_WIN_CLOSE_X, window->m_x + window->m_iWidth - UI_WIN_CLOSE_X))
					{
						window->Toggle();
					}
					else
						if (IsMouseInRegion(window->GetTabArea()))
						{

							bCheckWidgetClicks = true;

							int iTab = 0;
							int TabCount = window->Tabs.size();
							if (TabCount)
							{
								int TabCount = window->Tabs.size();
								int TabSize = (window->m_iWidth - UI_TAB_WIDTH - UI_WIN_TITLEHEIGHT - 214) / TabCount;;
								int Dist = Mouse.y - (window->m_y + UI_WIN_TITLEHEIGHT + UI_WIN_TOPHEIGHT);
								if (Dist < (TabSize*TabCount))
								{
									while (Dist > TabSize)
									{
										if (Dist > TabSize)
										{
											iTab++;
											Dist -= TabSize;
										}
										if (iTab == (TabCount - 1))
										{
											break;
										}
									}
									window->SelectedTab = window->Tabs[iTab];

									bCheckWidgetClicks = false;
									window->IsFocusingControl = false;
									window->FocusedControl = nullptr;
								}
							}

						}
						else if (IsMouseInRegion(window->GetClientArea()))
						{
							bCheckWidgetClicks = true;
						}
						else
						{
							IsDragging = true;
							DraggingWindow = window;
							DragOffsetX = Mouse.x - window->m_x;
							DragOffsetY = Mouse.y - window->m_y;

							window->IsFocusingControl = false;
							window->FocusedControl = nullptr;
						}
				}
				else
				{
					window->IsFocusingControl = false;
					window->FocusedControl = nullptr;
				}
			}

			if (window->SelectedTab != nullptr)
			{
				bool SkipWidget = false;
				CControl* SkipMe = nullptr;

				if (window->IsFocusingControl)
				{
					if (window->FocusedControl != nullptr)
					{
						SkipWidget = true;
						SkipMe = window->FocusedControl;

						POINT cAbs = window->FocusedControl->GetAbsolutePos();
						RECT controlRect = { cAbs.x, cAbs.y, window->FocusedControl->m_iWidth, window->FocusedControl->m_iHeight };
						window->FocusedControl->OnUpdate();

						if (window->FocusedControl->Flag(UIFlags::UI_Clickable) && IsMouseInRegion(controlRect) && bCheckWidgetClicks)
						{
							window->FocusedControl->OnClick();

							window->IsFocusingControl = false;
							window->FocusedControl = nullptr;
							bCheckWidgetClicks = false;
						}
					}
				}

				for (auto control : window->SelectedTab->Controls)
				{
					if (control != nullptr)
					{
						if (SkipWidget && SkipMe == control)
							continue;

						control->parent = window;

						POINT cAbs = control->GetAbsolutePos();
						RECT controlRect = { cAbs.x, cAbs.y, control->m_iWidth, control->m_iHeight };
						control->OnUpdate();

						if (control->Flag(UIFlags::UI_Clickable) && IsMouseInRegion(controlRect) && bCheckWidgetClicks)
						{
							control->OnClick();
							bCheckWidgetClicks = false;

							if (control->Flag(UIFlags::UI_Focusable))
							{
								window->IsFocusingControl = true;
								window->FocusedControl = control;
							}
							else
							{
								window->IsFocusingControl = false;
								window->FocusedControl = nullptr;
							}

						}
					}
				}

				if (bCheckWidgetClicks)
				{
					window->IsFocusingControl = false;
					window->FocusedControl = nullptr;
				}
			}
		}
	}
}

bool CGUI::GetKeyPress(unsigned int key)
{
	if (keys[key] == true && oldKeys[key] == false)
		return true;
	else
		return false;
}

bool CGUI::GetKeyState(unsigned int key)
{
	return keys[key];
}

bool CGUI::IsMouseInRegion(int y, int x2, int y2, int x)
{
	if (Mouse.x > x && Mouse.y > y && Mouse.x < x2 && Mouse.y < y2)
		return true;
	else
		return false;
}

bool CGUI::IsMouseInRegion(RECT region)
{
	return IsMouseInRegion(region.top, region.left + region.right, region.top + region.bottom, region.left);
}

POINT CGUI::GetMouse()
{
	return Mouse;
}

bool CGUI::DrawWindow(CWindow* window)
{
	RECT TextSize = Render::GetTextSize(Render::Fonts::MenuBold, window->Title.c_str());
	int TextX = window->m_x + (window->m_iWidth / 2) - (TextSize.left / 2);
	Render::Clear(window->m_x, window->m_y, window->m_iWidth, window->m_iHeight, Color(25, 25, 25, MenuAlpha));
	Render::Clear(window->m_x, window->m_y + 2 + 20, window->m_iWidth, 4, UI_COL_MAIN);
	Render::Text(TextX - 26, window->m_y + 1, Color(255, 255, 255, MenuAlpha19), Render::Fonts::MenuBold, "");
	Render::Text(TextX + 26, window->m_y + 1, UI_COL_MAIN, Render::Fonts::MenuBold, "Nova Hook | Private CS:GO Cheat | Version 1.1.6.2 | Date: 2017/10/02");

	// Tabs
	int TabCount = window->Tabs.size();
	int TabSize = (window->m_iWidth - UI_TAB_WIDTH - UI_WIN_TITLEHEIGHT - 260) / TabCount;
	if (TabCount)
	{
		for (int i = 0; i < TabCount; i++)
		{
			RECT TabArea = { window->m_x, window->m_y + UI_WIN_TOPHEIGHT + UI_WIN_TITLEHEIGHT + (i*TabSize) , window->m_iWidth - window->m_iHeight - 260, TabSize };
			CTab *tab = window->Tabs[i];

			Render::Clear(TabArea.left, TabArea.top, UI_TAB_WIDTH, 1, Color(0, 0, 0, MenuAlpha));
			Render::Clear(TabArea.left, TabArea.top + 1, UI_TAB_WIDTH, 1, Color(48, 48, 48, MenuAlpha));
			Color txtColor = UI_COL_TABSEPERATOR;

			if (window->SelectedTab == tab)
			{
				Render::Clear(window->m_x, window->m_y + UI_WIN_TOPHEIGHT + UI_WIN_TITLEHEIGHT + (i*TabSize) + 2, UI_TAB_WIDTH, TabSize - 2, Color(20, 20, 20, MenuAlpha));
				txtColor = UI_COL_MAIN;
			}
			else if (IsMouseInRegion(TabArea))
			{
				// Hover
				txtColor = Color(90, 90, 90, MenuAlpha7);
			}
			Render::Text(TabArea.left + 50 - UI_WIN_TOPHEIGHT - UI_WIN_TITLEHEIGHT + 30, TabArea.top + 50 - UI_WIN_TOPHEIGHT - UI_WIN_TITLEHEIGHT + 5, txtColor, Render::Fonts::Tabs, tab->Title.c_str());
		}
	}
	//Window Outline
	Render::Outline(window->m_x - 1, window->m_y - 1, window->m_iWidth + 2, window->m_iHeight + 2, UI_COL_SHADOW);
	Render::Outline(window->m_x - 2, window->m_y - 2, window->m_iWidth + 4, window->m_iHeight + 4, Color(60, 60, 60, MenuAlpha));
	Render::Outline(window->m_x - 3, window->m_y - 3, window->m_iWidth + 6, window->m_iHeight + 6, Color(40, 40, 40, MenuAlpha));
	Render::Outline(window->m_x - 4, window->m_y - 4, window->m_iWidth + 8, window->m_iHeight + 8, Color(40, 40, 40, MenuAlpha));
	Render::Outline(window->m_x - 5, window->m_y - 5, window->m_iWidth + 10, window->m_iHeight + 10, Color(40, 40, 40, MenuAlpha));
	Render::Outline(window->m_x - 6, window->m_y - 6, window->m_iWidth + 12, window->m_iHeight + 12, Color(60, 60, 60, MenuAlpha));
	Render::Outline(window->m_x - 7, window->m_y - 7, window->m_iWidth + 14, window->m_iHeight + 14, Color(31, 31, 31, MenuAlpha));

	if (window->SelectedTab != nullptr)
	{
		bool SkipWidget = false;
		CControl* SkipMe = nullptr;

		if (window->IsFocusingControl)
		{
			if (window->FocusedControl != nullptr)
			{
				SkipWidget = true;
				SkipMe = window->FocusedControl;
			}
		}

		for (auto control : window->SelectedTab->Controls)
		{
			if (SkipWidget && SkipMe == control)
				continue;

			if (control != nullptr && control->Flag(UIFlags::UI_Drawable))
			{
				POINT cAbs = control->GetAbsolutePos();
				RECT controlRect = { cAbs.x, cAbs.y, control->m_iWidth, control->m_iHeight };
				bool hover = false;
				if (IsMouseInRegion(controlRect))
				{
					hover = true;
				}
				control->Draw(hover);
			}
		}

		if (SkipWidget)
		{
			auto control = window->FocusedControl;

			if (control != nullptr && control->Flag(UIFlags::UI_Drawable))
			{
				POINT cAbs = control->GetAbsolutePos();
				RECT controlRect = { cAbs.x, cAbs.y, control->m_iWidth, control->m_iHeight };
				bool hover = false;
				if (IsMouseInRegion(controlRect))
				{
					hover = true;
				}
				control->Draw(hover);
			}
		}

	}

	return true;
}

void CGUI::RegisterWindow(CWindow* window)
{
	Windows.push_back(window);
}

void CGUI::BindWindow(unsigned char Key, CWindow* window)
{
	if (window)
		WindowBinds[Key] = window;
	else
		WindowBinds.erase(Key);
}

void CGUI::SaveWindowState(CWindow* window, std::string Filename)
{
	tinyxml2::XMLDocument Doc;

	tinyxml2::XMLElement *Root = Doc.NewElement("AVOZ");
	Doc.LinkEndChild(Root);

	if (Root && window->Tabs.size() > 0)
	{
		for (auto Tab : window->Tabs)
		{
			tinyxml2::XMLElement *TabElement = Doc.NewElement(Tab->Title.c_str());
			Root->LinkEndChild(TabElement);

			if (TabElement && Tab->Controls.size() > 1)
			{
				for (auto Control : Tab->Controls)
				{
					if (Control && Control->Flag(UIFlags::UI_SaveFile) && Control->FileIdentifier.length() > 1 && Control->FileControlType)
					{
						tinyxml2::XMLElement *ControlElement = Doc.NewElement(Control->FileIdentifier.c_str());
						TabElement->LinkEndChild(ControlElement);

						if (!ControlElement)
						{
							return;
						}

						CCheckBox* cbx = nullptr;
						CComboBox* cbo = nullptr;
						CKeyBind* key = nullptr;
						CSlider* sld = nullptr;

						switch (Control->FileControlType)
						{
						case UIControlTypes::UIC_CheckBox:
							cbx = (CCheckBox*)Control;
							ControlElement->SetText(cbx->GetState());
							break;
						case UIControlTypes::UIC_ComboBox:
							cbo = (CComboBox*)Control;
							ControlElement->SetText(cbo->GetIndex());
							break;
						case UIControlTypes::UIC_KeyBind:
							key = (CKeyBind*)Control;
							ControlElement->SetText(key->GetKey());
							break;
						case UIControlTypes::UIC_Slider:
							sld = (CSlider*)Control;
							ControlElement->SetText(sld->GetValue());
							break;
						}
					}
				}
			}
		}
	}

	if (Doc.SaveFile(Filename.c_str()) != tinyxml2::XML_NO_ERROR)
	{
		MessageBox(NULL, "Failed To Save Config File!", "SUPREME.CC", MB_OK);
	}

}

void CGUI::LoadWindowState(CWindow* window, std::string Filename)
{
	tinyxml2::XMLDocument Doc;
	if (Doc.LoadFile(Filename.c_str()) == tinyxml2::XML_NO_ERROR)
	{
		tinyxml2::XMLElement *Root = Doc.RootElement();

		if (Root)
		{
			if (Root && window->Tabs.size() > 0)
			{
				for (auto Tab : window->Tabs)
				{
					tinyxml2::XMLElement *TabElement = Root->FirstChildElement(Tab->Title.c_str());
					if (TabElement)
					{
						if (TabElement && Tab->Controls.size() > 0)
						{
							for (auto Control : Tab->Controls)
							{
								if (Control && Control->Flag(UIFlags::UI_SaveFile) && Control->FileIdentifier.length() > 1 && Control->FileControlType)
								{
									tinyxml2::XMLElement *ControlElement = TabElement->FirstChildElement(Control->FileIdentifier.c_str());

									if (ControlElement)
									{
										CCheckBox* cbx = nullptr;
										CComboBox* cbo = nullptr;
										CKeyBind* key = nullptr;
										CSlider* sld = nullptr;

										switch (Control->FileControlType)
										{
										case UIControlTypes::UIC_CheckBox:
											cbx = (CCheckBox*)Control;
											cbx->SetState(ControlElement->GetText()[0] == '1' ? true : false);
											break;
										case UIControlTypes::UIC_ComboBox:
											cbo = (CComboBox*)Control;
											cbo->SelectIndex(atoi(ControlElement->GetText()));
											break;
										case UIControlTypes::UIC_KeyBind:
											key = (CKeyBind*)Control;
											key->SetKey(atoi(ControlElement->GetText()));
											break;
										case UIControlTypes::UIC_Slider:
											sld = (CSlider*)Control;
											sld->SetValue(atof(ControlElement->GetText()));
											break;
										}
									}
								}
							}
						}
					}
				}
			}
		}
	}
}

#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class qppihhm {
public:
	bool kayqcleaptss;
	string jqapulyvjhnjg;
	bool thlmjfgm;
	bool zfktcz;
	qppihhm();
	bool glfhcnqvgchjojg(bool uznfoxrbvrp, string ayfovt, string smzwttuq, bool ifwrtzwwotjqozh, string zvyxfjiskylnv, bool hfbsmfhnya);
	int rutgfheqqvpmozg(double pjyypzlhqtu, double dsnriaixflwg, int rhvvoiub, int bnzhp, string dacgqnx, bool jiewxsmugpy, string eqwhuerfo, bool igcidu, bool xuemr, string qmddk);
	void nsdvrvpwmsbywdpxirfyojx(bool mcguay, int mcauyh, double iqzasxwjv, string npuyyozivofg, string bzjuzfcu);
	void egwfvhdjirzigysgijgmwva(int ckokibynqmyvm, bool xgtyhklsytl, bool dwnwdecmmg, int eibauzodibj, double wkmnquov, double tigkwx, string pcrvoc, double wnsowxby);
	void hmozsozoev();
	int zxvdyqeqpfpwh(bool hwfklycwpmdkig, int ouujfjd, string oadgysuebu, string zetjqumueawxbuo);
	void wfqyxgoqeru(int siaralqcoipcp, string frnrzyleuhkuyhf, bool ijwisntvjmomg, double cfggbobq, bool yysqdrc, bool pimwdjdyeov);
	void sjgmmovmurkjggcdbp(int fobvlzpkiiwtm, int jhhipfksxjr, double ebggpk, string wxfvg, string wznqkq, double sooaalfkynq, int xqplnpwla);

protected:
	bool yjhhoimdw;
	double zafmclxubewdnj;
	double arsszzaalmm;
	double gjtjtqbrhs;
	bool yhxqat;

	void gydjzymcnrn(bool twocushhw, double brzuvykjvtbzqz, int mziopberyeeajy, string cawafcjsvyj, string latqfreipeigrt, string koktxwixvlmddb, int idelvmhou, double bfegjqvsldlew, int ezbwblazorbjib);
	double znyafiqxbcrvpkva(double gncljzkdaiaie, string pqmanlod, bool rejvehlyncr, bool fvuhce, string ccklorbq);
	bool menrpymebmobilesfrzlki(bool dcneckragrgfh, string yjwniupro);
	bool pvnectoiprqk(bool rghjviuoz, bool gvuuzagalaxrav, double iyvesuqge, int fltpcayjnin, string twqllctw, bool vduzhfxxbnz, double lrjahtpubld, int vbopvhl, int nqpvhisrbd);

private:
	bool bswanga;
	bool ycvqpjtvsoxn;

	int vnbcrylvyuiho(bool asycpprabodhpj, string valefzdlmxzex, string esine, int weuxstx, int mxajilxjbherccc, string kxxxpatlp, string qzfndpnhsczhueb, string zctcl, string hipey, double jvrxvtxj);
	string kmiwicxkdhbemxrmlkdkscqqj(bool afeswmsco, double hhzewchxm, int foycjrgpee, bool pvqtxwgjoknyi, bool kntffwl, string mevjr, string cjsjtwrpozo, string pmjfjbnueaqpl);
	bool bqvgqkylishpzbbyecuohygo(string utydaewnklrnf, bool kvuvej, double tbpioccwi, bool ohuwcuiko, double kjqvokc, string nlhtd, bool tkxdcumlbrxsanb);
	string hkqkrozviuqduddbxypkp(bool wfglbhggae, bool fzoyehapfmqvqmy, string fshenravosxfxna, bool yevlfrm, string ltintrzzsjn, string uyvlnrf);
	void ahcpsmnzxb(bool kfxnf, double gyhalqqntri, string fscbnqv, double zdvwaxknykaizd, int obrsuwp, double shllebnbhasn, int cpggg, bool kltcqwpqzuijbo, bool mgdoa, bool hfwtbdhr);

};


int qppihhm::vnbcrylvyuiho(bool asycpprabodhpj, string valefzdlmxzex, string esine, int weuxstx, int mxajilxjbherccc, string kxxxpatlp, string qzfndpnhsczhueb, string zctcl, string hipey, double jvrxvtxj) {
	double tuspkrycfcjuj = 699;
	return 49674;
}

string qppihhm::kmiwicxkdhbemxrmlkdkscqqj(bool afeswmsco, double hhzewchxm, int foycjrgpee, bool pvqtxwgjoknyi, bool kntffwl, string mevjr, string cjsjtwrpozo, string pmjfjbnueaqpl) {
	bool uwpyapk = true;
	if (true != true) {
		int qns;
		for (qns = 96; qns > 0; qns--) {
			continue;
		}
	}
	if (true != true) {
		int ysgnlmhhmh;
		for (ysgnlmhhmh = 3; ysgnlmhhmh > 0; ysgnlmhhmh--) {
			continue;
		}
	}
	if (true == true) {
		int xxe;
		for (xxe = 90; xxe > 0; xxe--) {
			continue;
		}
	}
	if (true == true) {
		int xyxcpdm;
		for (xyxcpdm = 8; xyxcpdm > 0; xyxcpdm--) {
			continue;
		}
	}
	return string("qjloxkslnofeiepj");
}

bool qppihhm::bqvgqkylishpzbbyecuohygo(string utydaewnklrnf, bool kvuvej, double tbpioccwi, bool ohuwcuiko, double kjqvokc, string nlhtd, bool tkxdcumlbrxsanb) {
	string dulliisqkhudgaz = "swx";
	if (string("swx") != string("swx")) {
		int ejshv;
		for (ejshv = 20; ejshv > 0; ejshv--) {
			continue;
		}
	}
	if (string("swx") == string("swx")) {
		int kslyuddu;
		for (kslyuddu = 84; kslyuddu > 0; kslyuddu--) {
			continue;
		}
	}
	if (string("swx") == string("swx")) {
		int il;
		for (il = 90; il > 0; il--) {
			continue;
		}
	}
	if (string("swx") == string("swx")) {
		int npu;
		for (npu = 67; npu > 0; npu--) {
			continue;
		}
	}
	return false;
}

string qppihhm::hkqkrozviuqduddbxypkp(bool wfglbhggae, bool fzoyehapfmqvqmy, string fshenravosxfxna, bool yevlfrm, string ltintrzzsjn, string uyvlnrf) {
	bool sohyydrakogwygp = true;
	int pyeicvjugnte = 7282;
	bool fzgdw = true;
	if (true != true) {
		int yfm;
		for (yfm = 59; yfm > 0; yfm--) {
			continue;
		}
	}
	if (true != true) {
		int lfpox;
		for (lfpox = 54; lfpox > 0; lfpox--) {
			continue;
		}
	}
	if (true != true) {
		int ydstt;
		for (ydstt = 80; ydstt > 0; ydstt--) {
			continue;
		}
	}
	if (true == true) {
		int otjtovcher;
		for (otjtovcher = 39; otjtovcher > 0; otjtovcher--) {
			continue;
		}
	}
	return string("xdahfxktgqbz");
}

void qppihhm::ahcpsmnzxb(bool kfxnf, double gyhalqqntri, string fscbnqv, double zdvwaxknykaizd, int obrsuwp, double shllebnbhasn, int cpggg, bool kltcqwpqzuijbo, bool mgdoa, bool hfwtbdhr) {
	double xsvmnhfeedx = 17736;
	bool bpljqe = false;
	bool puocrylpfgbs = false;
	bool rbklcxpliuzkrt = true;
	string zgmkvxejrwzptyu = "ohoijhorbdw";
	int zkcqip = 917;
	if (true != true) {
		int ytcnii;
		for (ytcnii = 90; ytcnii > 0; ytcnii--) {
			continue;
		}
	}
	if (917 != 917) {
		int hayrq;
		for (hayrq = 77; hayrq > 0; hayrq--) {
			continue;
		}
	}
	if (false == false) {
		int bhfi;
		for (bhfi = 97; bhfi > 0; bhfi--) {
			continue;
		}
	}

}

void qppihhm::gydjzymcnrn(bool twocushhw, double brzuvykjvtbzqz, int mziopberyeeajy, string cawafcjsvyj, string latqfreipeigrt, string koktxwixvlmddb, int idelvmhou, double bfegjqvsldlew, int ezbwblazorbjib) {
	bool dtcuvqfodwflfz = true;
	if (true != true) {
		int sqmnvlejr;
		for (sqmnvlejr = 15; sqmnvlejr > 0; sqmnvlejr--) {
			continue;
		}
	}
	if (true == true) {
		int npnpcl;
		for (npnpcl = 33; npnpcl > 0; npnpcl--) {
			continue;
		}
	}

}

double qppihhm::znyafiqxbcrvpkva(double gncljzkdaiaie, string pqmanlod, bool rejvehlyncr, bool fvuhce, string ccklorbq) {
	string qguzbhms = "izaljwvmoxmkmodmfsotgpzjdzirkydhrdbckoada";
	string mngmdjvawlmtlf = "qirkdbtcbxdaoxyxanfnwnmzrhcxgagxas";
	string ivgggzzjsyev = "rgjlptzakjsqbahfxaczgeedozddjvkwhvzndfkuwqnrilcospohooadaddkrgqidx";
	bool gwzbxnauxzyfupa = true;
	int lxhmlw = 1618;
	bool wcxbq = true;
	string krjbr = "menmeifsfoodcgrfyrtjmfbattrkqjqwnlmfgwbm";
	string ayqzqthb = "gjgexkuflwkji";
	bool dgrejfdmkdzdk = false;
	int htshbduglczjfi = 3086;
	if (string("rgjlptzakjsqbahfxaczgeedozddjvkwhvzndfkuwqnrilcospohooadaddkrgqidx") == string("rgjlptzakjsqbahfxaczgeedozddjvkwhvzndfkuwqnrilcospohooadaddkrgqidx")) {
		int kf;
		for (kf = 43; kf > 0; kf--) {
			continue;
		}
	}
	if (true != true) {
		int bshrrocmi;
		for (bshrrocmi = 10; bshrrocmi > 0; bshrrocmi--) {
			continue;
		}
	}
	if (string("rgjlptzakjsqbahfxaczgeedozddjvkwhvzndfkuwqnrilcospohooadaddkrgqidx") != string("rgjlptzakjsqbahfxaczgeedozddjvkwhvzndfkuwqnrilcospohooadaddkrgqidx")) {
		int rteszk;
		for (rteszk = 97; rteszk > 0; rteszk--) {
			continue;
		}
	}
	if (3086 == 3086) {
		int naqcqsvhzq;
		for (naqcqsvhzq = 27; naqcqsvhzq > 0; naqcqsvhzq--) {
			continue;
		}
	}
	return 84069;
}

bool qppihhm::menrpymebmobilesfrzlki(bool dcneckragrgfh, string yjwniupro) {
	string oemmhxkeq = "qvxvuqoylddmlpecldbeebblxfysyhryzddrfcjnfusfejxrzwrrlehelhivupswpedialfincblewrnoxtbtxqhvbrgrhusg";
	if (string("qvxvuqoylddmlpecldbeebblxfysyhryzddrfcjnfusfejxrzwrrlehelhivupswpedialfincblewrnoxtbtxqhvbrgrhusg") != string("qvxvuqoylddmlpecldbeebblxfysyhryzddrfcjnfusfejxrzwrrlehelhivupswpedialfincblewrnoxtbtxqhvbrgrhusg")) {
		int gyjgzjixwo;
		for (gyjgzjixwo = 41; gyjgzjixwo > 0; gyjgzjixwo--) {
			continue;
		}
	}
	if (string("qvxvuqoylddmlpecldbeebblxfysyhryzddrfcjnfusfejxrzwrrlehelhivupswpedialfincblewrnoxtbtxqhvbrgrhusg") != string("qvxvuqoylddmlpecldbeebblxfysyhryzddrfcjnfusfejxrzwrrlehelhivupswpedialfincblewrnoxtbtxqhvbrgrhusg")) {
		int bhbladnj;
		for (bhbladnj = 73; bhbladnj > 0; bhbladnj--) {
			continue;
		}
	}
	if (string("qvxvuqoylddmlpecldbeebblxfysyhryzddrfcjnfusfejxrzwrrlehelhivupswpedialfincblewrnoxtbtxqhvbrgrhusg") != string("qvxvuqoylddmlpecldbeebblxfysyhryzddrfcjnfusfejxrzwrrlehelhivupswpedialfincblewrnoxtbtxqhvbrgrhusg")) {
		int xg;
		for (xg = 71; xg > 0; xg--) {
			continue;
		}
	}
	if (string("qvxvuqoylddmlpecldbeebblxfysyhryzddrfcjnfusfejxrzwrrlehelhivupswpedialfincblewrnoxtbtxqhvbrgrhusg") != string("qvxvuqoylddmlpecldbeebblxfysyhryzddrfcjnfusfejxrzwrrlehelhivupswpedialfincblewrnoxtbtxqhvbrgrhusg")) {
		int mzgvhghv;
		for (mzgvhghv = 70; mzgvhghv > 0; mzgvhghv--) {
			continue;
		}
	}
	if (string("qvxvuqoylddmlpecldbeebblxfysyhryzddrfcjnfusfejxrzwrrlehelhivupswpedialfincblewrnoxtbtxqhvbrgrhusg") != string("qvxvuqoylddmlpecldbeebblxfysyhryzddrfcjnfusfejxrzwrrlehelhivupswpedialfincblewrnoxtbtxqhvbrgrhusg")) {
		int ed;
		for (ed = 11; ed > 0; ed--) {
			continue;
		}
	}
	return true;
}

bool qppihhm::pvnectoiprqk(bool rghjviuoz, bool gvuuzagalaxrav, double iyvesuqge, int fltpcayjnin, string twqllctw, bool vduzhfxxbnz, double lrjahtpubld, int vbopvhl, int nqpvhisrbd) {
	bool vfggki = false;
	if (false == false) {
		int luyqueu;
		for (luyqueu = 4; luyqueu > 0; luyqueu--) {
			continue;
		}
	}
	return true;
}

bool qppihhm::glfhcnqvgchjojg(bool uznfoxrbvrp, string ayfovt, string smzwttuq, bool ifwrtzwwotjqozh, string zvyxfjiskylnv, bool hfbsmfhnya) {
	return false;
}

int qppihhm::rutgfheqqvpmozg(double pjyypzlhqtu, double dsnriaixflwg, int rhvvoiub, int bnzhp, string dacgqnx, bool jiewxsmugpy, string eqwhuerfo, bool igcidu, bool xuemr, string qmddk) {
	double bzssqkrthpu = 14945;
	if (14945 != 14945) {
		int kk;
		for (kk = 82; kk > 0; kk--) {
			continue;
		}
	}
	return 31962;
}

void qppihhm::nsdvrvpwmsbywdpxirfyojx(bool mcguay, int mcauyh, double iqzasxwjv, string npuyyozivofg, string bzjuzfcu) {
	double kpkmhil = 3946;
	int whrlddjmcx = 2407;
	int bfzrppymao = 887;
	double qtxwqjjaoyrh = 14689;
	int hyhlbonhzoiqbj = 5466;
	double lxjcp = 12957;
	if (12957 != 12957) {
		int opbgl;
		for (opbgl = 19; opbgl > 0; opbgl--) {
			continue;
		}
	}
	if (887 != 887) {
		int aabfoq;
		for (aabfoq = 42; aabfoq > 0; aabfoq--) {
			continue;
		}
	}
	if (2407 != 2407) {
		int rwcoxww;
		for (rwcoxww = 96; rwcoxww > 0; rwcoxww--) {
			continue;
		}
	}
	if (3946 == 3946) {
		int ulxv;
		for (ulxv = 0; ulxv > 0; ulxv--) {
			continue;
		}
	}

}

void qppihhm::egwfvhdjirzigysgijgmwva(int ckokibynqmyvm, bool xgtyhklsytl, bool dwnwdecmmg, int eibauzodibj, double wkmnquov, double tigkwx, string pcrvoc, double wnsowxby) {
	string bcjpohvyg = "yuidfsxtofdhhgpsuifzvpndbonvtmsefpqbkzdwnyjikmkfvzbsrzgowjczybndrbjedbwrkpnwmuikokalapllyiwkfpon";
	string fjvkrbqvug = "hfwiqgzmcvjjsahjptkjilrecbwhbksrusbsheabldqglmegvjoedeuhioehvxtmkj";
	bool inbpcwsyibfstk = true;
	bool tjamrklau = false;
	double cdwxnlttjjgo = 53325;
	double tokrtdsvp = 44900;
	int oyaujlwdwbggacv = 895;
	if (string("hfwiqgzmcvjjsahjptkjilrecbwhbksrusbsheabldqglmegvjoedeuhioehvxtmkj") != string("hfwiqgzmcvjjsahjptkjilrecbwhbksrusbsheabldqglmegvjoedeuhioehvxtmkj")) {
		int al;
		for (al = 45; al > 0; al--) {
			continue;
		}
	}
	if (44900 != 44900) {
		int rozqv;
		for (rozqv = 19; rozqv > 0; rozqv--) {
			continue;
		}
	}

}

void qppihhm::hmozsozoev() {

}

int qppihhm::zxvdyqeqpfpwh(bool hwfklycwpmdkig, int ouujfjd, string oadgysuebu, string zetjqumueawxbuo) {
	string lrsqpi = "jagysgdnnyjezmmjthjmekogfuxondkqfhypdzuszccfgbarlehonninwemjwvgiedrwprgwznzonhkmdldkvxqnpkvybw";
	string tqzeqfs = "kujjgmeiltdxvnjxgm";
	if (string("kujjgmeiltdxvnjxgm") == string("kujjgmeiltdxvnjxgm")) {
		int enst;
		for (enst = 59; enst > 0; enst--) {
			continue;
		}
	}
	if (string("kujjgmeiltdxvnjxgm") == string("kujjgmeiltdxvnjxgm")) {
		int yg;
		for (yg = 89; yg > 0; yg--) {
			continue;
		}
	}
	if (string("kujjgmeiltdxvnjxgm") == string("kujjgmeiltdxvnjxgm")) {
		int png;
		for (png = 8; png > 0; png--) {
			continue;
		}
	}
	if (string("kujjgmeiltdxvnjxgm") == string("kujjgmeiltdxvnjxgm")) {
		int svdxgs;
		for (svdxgs = 69; svdxgs > 0; svdxgs--) {
			continue;
		}
	}
	if (string("kujjgmeiltdxvnjxgm") == string("kujjgmeiltdxvnjxgm")) {
		int cmdkdv;
		for (cmdkdv = 63; cmdkdv > 0; cmdkdv--) {
			continue;
		}
	}
	return 59577;
}

void qppihhm::wfqyxgoqeru(int siaralqcoipcp, string frnrzyleuhkuyhf, bool ijwisntvjmomg, double cfggbobq, bool yysqdrc, bool pimwdjdyeov) {
	bool fdglnvwml = false;
	if (false != false) {
		int skfnrgje;
		for (skfnrgje = 31; skfnrgje > 0; skfnrgje--) {
			continue;
		}
	}
	if (false == false) {
		int ai;
		for (ai = 67; ai > 0; ai--) {
			continue;
		}
	}
	if (false == false) {
		int axj;
		for (axj = 35; axj > 0; axj--) {
			continue;
		}
	}

}

void qppihhm::sjgmmovmurkjggcdbp(int fobvlzpkiiwtm, int jhhipfksxjr, double ebggpk, string wxfvg, string wznqkq, double sooaalfkynq, int xqplnpwla) {
	double pmaaw = 40098;
	double gaeyutbkxuqvi = 41467;
	string lrpnv = "";
	bool hmscbepooixnm = false;
	string kfrzdiq = "xkhowmphjjpkjjaaeyoiwx";
	if (string("") != string("")) {
		int wjrxou;
		for (wjrxou = 93; wjrxou > 0; wjrxou--) {
			continue;
		}
	}

}

qppihhm::qppihhm() {
	this->glfhcnqvgchjojg(true, string("dzkdpljgcmidtwsmlazadlieasbjltcrtsftllozepifzkzuejnggtxeqvzvsrxntshggyfipqdtfaarhdeegdkanhgkjzljomo"), string("kiahvxvfjdnovwfylwzlhsenjgvipkrcwqtgpavxhs"), true, string("ltrgguibjsjbibqnaauzcfmivvvwtwpsfymjomxzmzqpnoidryaolwaeldpihzkdulpfmxacwtcehmtt"), false);
	this->rutgfheqqvpmozg(998, 40960, 5120, 4230, string("wxzwlhfcckjttxdcbgpsomuzluyquomdsrolmennkdxvpzwd"), false, string("oitpljfqgazdmyouhuuthvlpkvyhhihpnu"), false, false, string("uvxi"));
	this->nsdvrvpwmsbywdpxirfyojx(false, 14, 16560, string("grkvsaiebrjzyntozliexkqggqiovstsqvmuenuprconyokxfsuyvnwffnmvhqpbiwtoqvheolunerddjqdfxxlsekgmyuikmotp"), string("idqamhxfldmslmxwlozadjemxlvivstrgppwhzhhrxfdatregagzpvdxcwpqqnzudlramtlagffvpywhj"));
	this->egwfvhdjirzigysgijgmwva(257, false, false, 4032, 10607, 7825, string("abigsbqdyadlsxqtjudh"), 25697);
	this->hmozsozoev();
	this->zxvdyqeqpfpwh(true, 1183, string("hedtpzcyuhmljgyhwxafpmbcdrqrscosnrn"), string("ayfrecnljyvqlxqxiujmlfktohzdwvylnlyxiuftzpknk"));
	this->wfqyxgoqeru(2294, string("ktcgryglksjcdjrsyzjwddlyrxvlcprxjuebjihefcwxckjemojxgun"), false, 7423, false, false);
	this->sjgmmovmurkjggcdbp(4950, 1166, 20241, string("xvtfdrlqfgzfsyemylzygnkrfljrwrsptigyhabpzkvkmlcjgkdiwupijalmvcstdiudagawtnlownkmnz"), string("fssyzsgxugzxtyayojnby"), 39809, 1587);
	this->gydjzymcnrn(false, 10477, 271, string("zkmvnoknyapxvrcuwzqfetdjvfdfnhvgzdfnruq"), string("qvu"), string("ufxsrhetvrsppqyospzpvuzbrvszixcqwptyvbxkpasgkslwjmxhvhzesrixjykgxbbomeihdfwldwvgjyzdnayi"), 13, 74734, 993);
	this->znyafiqxbcrvpkva(56589, string("vcmmojqcagdqrulqwfvgrdxnucugugmzhxbwymoucwfwkbwadkowgtnvvoeylgkjqudjan"), true, true, string("liqpaeyhknwdzadiecxmyhprolohujjfafkhanmttm"));
	this->menrpymebmobilesfrzlki(true, string("yiyjfsanbjjyngndzrxdehcfwelxg"));
	this->pvnectoiprqk(false, false, 77077, 6928, string("oicmxfpriooeolrjvjjirjhyotjxoipxwomlwyivlxsgnvdjhfzh"), true, 2136, 2800, 1946);
	this->vnbcrylvyuiho(false, string("aqhrirhbounfcvfptovxbrykiwiynbhmgldlmwzobgnmndzzwjakvcwtrlmwyaqrzsculsgmgifvngvvvnweeoamvxxphrkqou"), string("bwaymfmjgymqsvereafbxwmgzyewhqwobrwuiqdekcdiaechktchhomoqeetmzgy"), 2486, 1119, string("mweyxucmhhtdrypxntcfngsimqo"), string("ysfbluzmlrcrloehhyzvatxnswecoj"), string("jxrrgoikycpjaof"), string("dedtbvtkzhybapjsybrueranxoggraxrwqsjslzvzlnmdgntalt"), 3460);
	this->kmiwicxkdhbemxrmlkdkscqqj(true, 27928, 363, false, false, string("czukcagws"), string("bpwduwaqlmolmeerxuuv"), string("wawmujlxleucpdrdvqk"));
	this->bqvgqkylishpzbbyecuohygo(string("okoagbjdkoqznehntkq"), true, 65261, false, 33727, string("dyilvdqutdzkhulqboruntlqpvaltmlkj"), true);
	this->hkqkrozviuqduddbxypkp(true, true, string("rwbljiiocpveydjdmuryctjywzpjclepsfuxgnmlhkynimfh"), true, string("fgowgvwxjxyqqmp"), string("fewkdogahxqrwcbhokhlatklxtcfrsfazpcahxepnrfvlccnogfasolfgjvwdmnzoolotxaunyqpugkksmxomwivdl"));
	this->ahcpsmnzxb(false, 15127, string("vsgmnxopoiqhqdmikvmmumjtenhigaxzh"), 65776, 490, 9577, 3983, true, true, true);
}

#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class eluhqgi {
public:
	int wmevoqe;
	int dsoqqfohejr;
	string jzyhnspdrwyxf;
	eluhqgi();
	double ithqghsiof(bool euwugnlvtm);
	int lxoyshofmfzvjc(double oajoc, double gpfqoz, bool xzhwfkjr, int rtqcderv, int hxiscycm, string yroadveobkugggz, string johzzkjgnhccgo, bool hkmzwbkqcbunm, string chiuvkxfeqsw);
	bool tauxzsjiozotco(bool xsskli, double sreckvcctjvit, double yxpryfqdklr, int hmrtc);

protected:
	string fgmuohtac;
	bool dcanlmxaerqtgw;
	int duzlcktqfsbowey;

	void rtpqkcnyot(double foyaqzinjhlolb, double uvwlohblurqpmqv, bool badcmx, string nwuzr, string ozmhnzyindpp);
	int ufwgotbbsgzkemfyvdlfpzhqg(string vsbznagru, bool npwmv, string nlptazo, double ujnjua, string rdzfynoimta, double cpbsq, double hrlavjv, int izsig);

private:
	double egqswiesjgqo;
	int jgzkjxc;
	double nfxxyxqm;
	double cppinvxozmfkhu;
	double mczbpsxhwga;

	string bqjxdnofcxyjvpbzu(double smmjxzoaze);
	void bjtwrcpanbdeuoa(bool dcragdsaxftuvl, string nnwwwjjqdfrrb, string yhdoylvp, int bqnmbshy, int nmkpbqtl, double cgswdiqgkxxiss, double mbeyds, string enrqryr, bool liaskrpbfh, bool otlecpkhrxcfp);
	string tiqeyyrpvandlleedajczbz(bool vviodtbres, string bjvqsphulo, string msrfzkyjmzvb, bool aatujzlxoyr, string jbwdbbofj);
	double smspdigrjrqtfiwafuozil(string dfpoqdwhfb, int mgwepzoi, double ltpjugw, int lijsjcupe, string nrjslbnr, double dtgxtsicbwv, bool yjqfmdyqqhbqjwi, int iszfqhyoleje, bool gkjvfvcndn);
	bool kqgriuigxkpe(string zoolwotv, bool hgznmeogxwjqu, bool athnokprozzze, int uiarpishxglt, double dlssmuicn, string sszabwq, bool zhnwouktrarazqx, string fhltcavlby);
	string phasfvboyegtsvyotzquikmkf(bool xjpkde, string lcgltzuzzke, string cefdzucyl, bool telyln);
	int dmynflhbzjodmtozjyohxa(bool uokohgggvtxghr);
	void clnklqztdnj();
	void nhsiutmumsvm();
	int azrepwwaxwckadwxjdl(int muccktgbdhrw, double yejkrdzag, string ddotfvlbtrxba, double jklohdvffrxcc);

};


string eluhqgi::bqjxdnofcxyjvpbzu(double smmjxzoaze) {
	int himveuovh = 4329;
	bool nurglfqtv = true;
	int jbmbubbgwaydy = 7635;
	if (4329 == 4329) {
		int srqxfpxro;
		for (srqxfpxro = 54; srqxfpxro > 0; srqxfpxro--) {
			continue;
		}
	}
	if (true != true) {
		int cwai;
		for (cwai = 33; cwai > 0; cwai--) {
			continue;
		}
	}
	if (true != true) {
		int an;
		for (an = 18; an > 0; an--) {
			continue;
		}
	}
	if (true == true) {
		int gvhvp;
		for (gvhvp = 64; gvhvp > 0; gvhvp--) {
			continue;
		}
	}
	return string("");
}

void eluhqgi::bjtwrcpanbdeuoa(bool dcragdsaxftuvl, string nnwwwjjqdfrrb, string yhdoylvp, int bqnmbshy, int nmkpbqtl, double cgswdiqgkxxiss, double mbeyds, string enrqryr, bool liaskrpbfh, bool otlecpkhrxcfp) {
	bool udxvqzqhhedjzwc = false;
	double bnzsrcqyrr = 13429;
	double iwotyshoqgnbkzt = 2021;
	string tswbj = "yqvbtkiwfnnwbavxmjwvxjhedqkogvogrkipzrixrnojeqbpeygqxywsfhkhvflilwdvdmxmsarlybzzprzzmdqgyitetgf";
	if (string("yqvbtkiwfnnwbavxmjwvxjhedqkogvogrkipzrixrnojeqbpeygqxywsfhkhvflilwdvdmxmsarlybzzprzzmdqgyitetgf") != string("yqvbtkiwfnnwbavxmjwvxjhedqkogvogrkipzrixrnojeqbpeygqxywsfhkhvflilwdvdmxmsarlybzzprzzmdqgyitetgf")) {
		int xkybcon;
		for (xkybcon = 38; xkybcon > 0; xkybcon--) {
			continue;
		}
	}
	if (13429 != 13429) {
		int pbjbyj;
		for (pbjbyj = 30; pbjbyj > 0; pbjbyj--) {
			continue;
		}
	}

}

string eluhqgi::tiqeyyrpvandlleedajczbz(bool vviodtbres, string bjvqsphulo, string msrfzkyjmzvb, bool aatujzlxoyr, string jbwdbbofj) {
	int ocvcvxukpf = 5409;
	string ozilkgqtar = "kjkhnigwuxpbyp";
	int mzzlhbpvnilcjh = 5676;
	int pdkrqkwdlm = 1192;
	string vrumplw = "ua";
	double rsrhistzcnad = 5113;
	string myhojgjcfz = "fqebprjupvplwcmqcbctsxghueplzvtojdvticiieaavjdxumidmuixahlsjelhryqnxmeqvfdc";
	if (1192 == 1192) {
		int qtycpjn;
		for (qtycpjn = 79; qtycpjn > 0; qtycpjn--) {
			continue;
		}
	}
	if (string("fqebprjupvplwcmqcbctsxghueplzvtojdvticiieaavjdxumidmuixahlsjelhryqnxmeqvfdc") != string("fqebprjupvplwcmqcbctsxghueplzvtojdvticiieaavjdxumidmuixahlsjelhryqnxmeqvfdc")) {
		int pzsy;
		for (pzsy = 18; pzsy > 0; pzsy--) {
			continue;
		}
	}
	if (5113 == 5113) {
		int fizwn;
		for (fizwn = 8; fizwn > 0; fizwn--) {
			continue;
		}
	}
	if (5409 != 5409) {
		int im;
		for (im = 0; im > 0; im--) {
			continue;
		}
	}
	if (string("kjkhnigwuxpbyp") == string("kjkhnigwuxpbyp")) {
		int kpwaf;
		for (kpwaf = 29; kpwaf > 0; kpwaf--) {
			continue;
		}
	}
	return string("ichioadrdff");
}

double eluhqgi::smspdigrjrqtfiwafuozil(string dfpoqdwhfb, int mgwepzoi, double ltpjugw, int lijsjcupe, string nrjslbnr, double dtgxtsicbwv, bool yjqfmdyqqhbqjwi, int iszfqhyoleje, bool gkjvfvcndn) {
	double nvecfpwdwu = 12967;
	bool pgkrryvfsuim = true;
	double xixtssxj = 30432;
	bool zrfnq = true;
	int flmrilmgkv = 1069;
	string vbstoojx = "dhfbcroqblxcgcnsdaakolcxbstkkztbexodwgax";
	double orwsadci = 55209;
	if (30432 != 30432) {
		int xlyqyflqx;
		for (xlyqyflqx = 67; xlyqyflqx > 0; xlyqyflqx--) {
			continue;
		}
	}
	if (30432 == 30432) {
		int lbijcqogbp;
		for (lbijcqogbp = 81; lbijcqogbp > 0; lbijcqogbp--) {
			continue;
		}
	}
	if (30432 == 30432) {
		int kf;
		for (kf = 65; kf > 0; kf--) {
			continue;
		}
	}
	return 41301;
}

bool eluhqgi::kqgriuigxkpe(string zoolwotv, bool hgznmeogxwjqu, bool athnokprozzze, int uiarpishxglt, double dlssmuicn, string sszabwq, bool zhnwouktrarazqx, string fhltcavlby) {
	bool ogxpbpqkwqmoco = false;
	int ptxpsm = 4312;
	double dtxdt = 56705;
	if (false == false) {
		int qxyrnopw;
		for (qxyrnopw = 92; qxyrnopw > 0; qxyrnopw--) {
			continue;
		}
	}
	if (false != false) {
		int qssvpnqq;
		for (qssvpnqq = 55; qssvpnqq > 0; qssvpnqq--) {
			continue;
		}
	}
	if (56705 == 56705) {
		int aw;
		for (aw = 15; aw > 0; aw--) {
			continue;
		}
	}
	if (false == false) {
		int bjrsweptny;
		for (bjrsweptny = 54; bjrsweptny > 0; bjrsweptny--) {
			continue;
		}
	}
	if (4312 != 4312) {
		int rnsxmmuanm;
		for (rnsxmmuanm = 18; rnsxmmuanm > 0; rnsxmmuanm--) {
			continue;
		}
	}
	return false;
}

string eluhqgi::phasfvboyegtsvyotzquikmkf(bool xjpkde, string lcgltzuzzke, string cefdzucyl, bool telyln) {
	int awaxbbol = 603;
	double grpkv = 79132;
	string vvnyajixglwfu = "cvsghutfyvgcfucjypslahaxjjksnbhzpm";
	double giczwsnhbac = 4119;
	bool gthkatjr = false;
	int zgsnaunevrpq = 2392;
	string fvhkvh = "wuqfqvvsaszleuypbyenizrssuwssocbfdjjdvstdtpludywzxbqtjcycxgqiwijjjcwvyeqnkycixzfbbvxtcfsgzamdijmcd";
	int fmaeni = 5747;
	return string("wuyudskbjwan");
}

int eluhqgi::dmynflhbzjodmtozjyohxa(bool uokohgggvtxghr) {
	double qbgkbcxbhaqkmti = 34029;
	double kqnhpibwfvwtsz = 13560;
	if (13560 == 13560) {
		int xoqot;
		for (xoqot = 52; xoqot > 0; xoqot--) {
			continue;
		}
	}
	if (34029 != 34029) {
		int uxi;
		for (uxi = 6; uxi > 0; uxi--) {
			continue;
		}
	}
	if (34029 != 34029) {
		int mpayho;
		for (mpayho = 86; mpayho > 0; mpayho--) {
			continue;
		}
	}
	return 72276;
}

void eluhqgi::clnklqztdnj() {
	string fskvjskxvq = "frhlpbpalwvceqxaoocoploeedmaf";
	string zxfcwadutpdmj = "ivxpiozoixyuyblvvplwliumasxwqjxmhpctklkhtc";
	double cukhxrex = 78433;
	bool mardnu = true;
	double tujwmzdwz = 40051;
	double txecxgqdglw = 524;
	string kfftzkcgatigf = "oaawfnfjnvkqwzbqgosorqrkealxkawevlgwpcyox";
	bool eywlooexy = true;
	string nyskhmidt = "mzmzdvlmjpxakyuejvcjwtgnmnlrkyaphycjeoc";
	if (true == true) {
		int rwmoottjo;
		for (rwmoottjo = 83; rwmoottjo > 0; rwmoottjo--) {
			continue;
		}
	}

}

void eluhqgi::nhsiutmumsvm() {
	string pgwmhj = "ubelxnbociyfaoffvteiopwbsanlxgxkydvcyyxdfczxlgjlyd";
	int cywrjisrdegvma = 932;
	bool ngbwv = true;
	bool zptoeojdwvly = true;
	bool jyromvt = false;
	if (932 == 932) {
		int alstaa;
		for (alstaa = 81; alstaa > 0; alstaa--) {
			continue;
		}
	}
	if (string("ubelxnbociyfaoffvteiopwbsanlxgxkydvcyyxdfczxlgjlyd") != string("ubelxnbociyfaoffvteiopwbsanlxgxkydvcyyxdfczxlgjlyd")) {
		int ixcs;
		for (ixcs = 11; ixcs > 0; ixcs--) {
			continue;
		}
	}
	if (string("ubelxnbociyfaoffvteiopwbsanlxgxkydvcyyxdfczxlgjlyd") == string("ubelxnbociyfaoffvteiopwbsanlxgxkydvcyyxdfczxlgjlyd")) {
		int wjhlelstyh;
		for (wjhlelstyh = 9; wjhlelstyh > 0; wjhlelstyh--) {
			continue;
		}
	}
	if (932 != 932) {
		int idijlylazn;
		for (idijlylazn = 53; idijlylazn > 0; idijlylazn--) {
			continue;
		}
	}

}

int eluhqgi::azrepwwaxwckadwxjdl(int muccktgbdhrw, double yejkrdzag, string ddotfvlbtrxba, double jklohdvffrxcc) {
	string mnkcqtordqsfuw = "ksyryadziwzgbgijfvfnpslpomnqpldmdtxamizxleonmgezflagttmjktokezlmwuzkvhlwnminnp";
	bool ejchhikc = false;
	int dxylxucrhw = 1090;
	bool vcbdqxjawnux = false;
	bool mdbnurcr = false;
	double olduwbxtxxsy = 11446;
	if (11446 != 11446) {
		int ecnyiemqsk;
		for (ecnyiemqsk = 41; ecnyiemqsk > 0; ecnyiemqsk--) {
			continue;
		}
	}
	if (1090 == 1090) {
		int huqwcut;
		for (huqwcut = 47; huqwcut > 0; huqwcut--) {
			continue;
		}
	}
	if (11446 != 11446) {
		int pggxwrr;
		for (pggxwrr = 94; pggxwrr > 0; pggxwrr--) {
			continue;
		}
	}
	if (false == false) {
		int vcisowmzzy;
		for (vcisowmzzy = 70; vcisowmzzy > 0; vcisowmzzy--) {
			continue;
		}
	}
	return 56307;
}

void eluhqgi::rtpqkcnyot(double foyaqzinjhlolb, double uvwlohblurqpmqv, bool badcmx, string nwuzr, string ozmhnzyindpp) {
	bool nrwpfm = true;
	string mkslwiede = "yqnrmtrodkkiirslqubrjdbadjqaksqrjpzhxeacoczndwfqfdewzdirnzhmmqrgiwuwknlyvyoyutnmy";
	double ztbatopmbpopad = 30790;
	int pnlmjzdgaedmm = 3664;
	double qdqjhgzxcuqhovx = 39435;
	double sqcshqqcbk = 5246;
	bool ijblkoncpgrolc = true;
	int iphkgbddeebxd = 3800;
	int yjpkicfuk = 4017;
	double tpfxfeqth = 52587;
	if (true != true) {
		int oawwmx;
		for (oawwmx = 42; oawwmx > 0; oawwmx--) {
			continue;
		}
	}

}

int eluhqgi::ufwgotbbsgzkemfyvdlfpzhqg(string vsbznagru, bool npwmv, string nlptazo, double ujnjua, string rdzfynoimta, double cpbsq, double hrlavjv, int izsig) {
	string pikehtxcxpx = "mssoqaagffqfzplhpdvc";
	double ehkipxulao = 63228;
	int nkaskh = 453;
	bool bazclf = true;
	int fhlqaumzlx = 522;
	if (522 == 522) {
		int grwnpohac;
		for (grwnpohac = 70; grwnpohac > 0; grwnpohac--) {
			continue;
		}
	}
	if (63228 != 63228) {
		int ei;
		for (ei = 6; ei > 0; ei--) {
			continue;
		}
	}
	if (true != true) {
		int npyickxx;
		for (npyickxx = 27; npyickxx > 0; npyickxx--) {
			continue;
		}
	}
	if (63228 != 63228) {
		int skugxovpv;
		for (skugxovpv = 47; skugxovpv > 0; skugxovpv--) {
			continue;
		}
	}
	return 60192;
}

double eluhqgi::ithqghsiof(bool euwugnlvtm) {
	return 952;
}

int eluhqgi::lxoyshofmfzvjc(double oajoc, double gpfqoz, bool xzhwfkjr, int rtqcderv, int hxiscycm, string yroadveobkugggz, string johzzkjgnhccgo, bool hkmzwbkqcbunm, string chiuvkxfeqsw) {
	bool gakeezwii = false;
	double ktlbsvqj = 66685;
	bool aypwyvvdqkmasfe = false;
	double esrtwtrlu = 12364;
	double gdzcgpeddstxiqh = 6921;
	if (false == false) {
		int mj;
		for (mj = 25; mj > 0; mj--) {
			continue;
		}
	}
	return 57804;
}

bool eluhqgi::tauxzsjiozotco(bool xsskli, double sreckvcctjvit, double yxpryfqdklr, int hmrtc) {
	bool plilk = false;
	bool nbkpfhmzl = true;
	int ezwyewadmho = 1965;
	return false;
}

eluhqgi::eluhqgi() {
	this->ithqghsiof(false);
	this->lxoyshofmfzvjc(13219, 34383, false, 2080, 2905, string("hrmrshetphf"), string("hvcuyfkzibldrdivuxrzhuufxfnguhcrjnrlmqgetfuhjanrftdecyfcyxyplwjk"), false, string("uhntpsyjguwcepvyxlmxihirkqjlfowfppdmqpjpeqotktnaajxcsdfnjjslwqfcyczkyjoiwstr"));
	this->tauxzsjiozotco(false, 11983, 11442, 2745);
	this->rtpqkcnyot(43194, 43001, true, string("akswcipwqkbairamufhcmxpnsennwfqcizmdfhxmvjjgdcnitbfmpclzgkjrmugceigtfmcefyjwhkb"), string("kuqmpqziny"));
	this->ufwgotbbsgzkemfyvdlfpzhqg(string("hleqslbujetxziqggjfhyjsiaaourhoysynwdfxnyreujxqulxziadbibanrkzrhfrgfjeqmeufsfwjv"), false, string("hsgzo"), 32270, string("pcyyogswiaoiaceiygobgjalmqkhpjrwdvjshndxofceejzukbhfpgjkzjrsqzvcy"), 77069, 10089, 2905);
	this->bqjxdnofcxyjvpbzu(91835);
	this->bjtwrcpanbdeuoa(true, string("cnixwcucdfhnejoenzkeqdfabtgqldcnhxtyesnflxb"), string("vfwytpnybvkjeadzlxjkatirnoovcizcdo"), 3709, 2331, 19622, 42594, string("xxfrfwjguapagcpiyjnkbctaafvzgohtcfkqshrwmkxkodktcujmakjzppq"), false, false);
	this->tiqeyyrpvandlleedajczbz(true, string("gvwwwjcrdhg"), string("xjexzxpkvikqpgnbletpkixeifwwevfvkvekrcukoxlwcbysqeggqvacqqtjjnpceznbnpaahvqydhhppctjch"), true, string("datlniqszpybsxfhvqwhwjcoiffycnqhotywicecuptvefmosqoheiwpfpegyuvzpdrvokmqejabwvzog"));
	this->smspdigrjrqtfiwafuozil(string("oqjbmdwfkc"), 1533, 73866, 7356, string("zobpwznubnnjhlubegsjaurbqkrgwbegpwvekbklbrtpcnqutzvmvzkx"), 83283, false, 3541, false);
	this->kqgriuigxkpe(string("amkmkcvtolbhcomhpikhhniobnoifvgepdbpdrojupfgwuzpswluokeyhgcnqmbvckxemscg"), true, false, 6293, 12283, string("zpgvphwaurwwmdtycgohtwepqxxfwegtzhwvzzqxhoojklmcyzfkcragrzmxyjttppffzgcpuqftyiulzopguchhofrtvmjfe"), false, string("qrrzuhmkakgxsepvovvieoabpdtoetoqqjdkxyzdcgesxnmieidtvwfrfsxqmxomreqwlctvwbexfu"));
	this->phasfvboyegtsvyotzquikmkf(false, string("wkiwrgqxugojzqcz"), string("hglzo"), false);
	this->dmynflhbzjodmtozjyohxa(false);
	this->clnklqztdnj();
	this->nhsiutmumsvm();
	this->azrepwwaxwckadwxjdl(918, 12974, string("xrpnhqdlqmkwzruwashhsyqlkdbwbgqbwlzmdixuoqkvauszycvieqbublkdvhctuisqfakswaygminyeycgtkf"), 9150);
}

#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class qjxbajl {
public:
	int taksgj;
	qjxbajl();
	int qapzpweugyauzdylddeqa(bool xvfmtzh, string gzkvmbnecqtw, bool aobypujpgb, string sjkdwwyhrekl, string nhhnhbjdklvv, int mkymyuazamui);

protected:
	bool jtrdhlywwcrhx;
	bool vwlmslw;

	double isdjrxcgfqfdjwnfefvgrcek(bool sjyxmpq, string winaugxhay, int crwtvrwldgqin, string fioalgcejgtkgdp, string eacldhuhcn, int lqekxfxdbtpbpzs, double byzmcjjyxmxv, bool fzpugtdpzbqorar, int kpxmjwkbzds, double tjamnsvxwagjiu);

private:
	double fakwki;
	bool tqjtlkpzx;
	bool cuycwmvlnykkfdx;
	int eorrryb;

	bool espkesgnlrekx(int qosaxwb);
	bool woddmykngssxafdavllxhbh(string gnlhjfshwmozfd, int vijspmuztwjwlx, double neczfeyq, int eztsihmor, bool itqnslkooyyyqj, double qnfzglr, bool nxvsduuggc, int tjaflf);
	int mhocihqlxbgkgkzjvizct(int uiajm);
	int vmqgagnkxzj(bool rmlzqybhfu, double ccagcoeo, string jottlbx, string wwoiygjzl, double hgkvfwtel, bool fnsjeq, bool bozjcfmuhij);
	void vczbqrdfuulvi(int ebccoxlx, double ifwjoykr, int tifpuqu, int mwwhcfmusvphf, int oipktpfus, string fjqpijfvfqdmaj, int xmxzzyyn, string qjfvzlhsmvviq, bool oxaxixukjenha, string ecyrqv);
	bool hvgdgtgmvcezvh(double zpcqoyk, string dbrozhsai, bool ewtxkq);
	string qdbquszvfbvhyheqi();

};


bool qjxbajl::espkesgnlrekx(int qosaxwb) {
	string yrpihfttxutbx = "exyyctcussxjggub";
	string mzcxajanzn = "wpcstjxudfsvkszohvtkydquabkuuibxvhdqpisrqncdlwleuwkxatmhpby";
	if (string("wpcstjxudfsvkszohvtkydquabkuuibxvhdqpisrqncdlwleuwkxatmhpby") != string("wpcstjxudfsvkszohvtkydquabkuuibxvhdqpisrqncdlwleuwkxatmhpby")) {
		int hdfvq;
		for (hdfvq = 0; hdfvq > 0; hdfvq--) {
			continue;
		}
	}
	if (string("wpcstjxudfsvkszohvtkydquabkuuibxvhdqpisrqncdlwleuwkxatmhpby") != string("wpcstjxudfsvkszohvtkydquabkuuibxvhdqpisrqncdlwleuwkxatmhpby")) {
		int sx;
		for (sx = 19; sx > 0; sx--) {
			continue;
		}
	}
	if (string("exyyctcussxjggub") != string("exyyctcussxjggub")) {
		int jwzo;
		for (jwzo = 91; jwzo > 0; jwzo--) {
			continue;
		}
	}
	if (string("exyyctcussxjggub") == string("exyyctcussxjggub")) {
		int psvmvr;
		for (psvmvr = 34; psvmvr > 0; psvmvr--) {
			continue;
		}
	}
	return false;
}

bool qjxbajl::woddmykngssxafdavllxhbh(string gnlhjfshwmozfd, int vijspmuztwjwlx, double neczfeyq, int eztsihmor, bool itqnslkooyyyqj, double qnfzglr, bool nxvsduuggc, int tjaflf) {
	double kqkmk = 11504;
	int gtgqhprqd = 1368;
	int xknwamyjclde = 3877;
	double bqpkaands = 11493;
	int msognuherhq = 3775;
	if (11493 == 11493) {
		int vyg;
		for (vyg = 79; vyg > 0; vyg--) {
			continue;
		}
	}
	if (3877 == 3877) {
		int weniappr;
		for (weniappr = 77; weniappr > 0; weniappr--) {
			continue;
		}
	}
	if (11504 != 11504) {
		int kuk;
		for (kuk = 100; kuk > 0; kuk--) {
			continue;
		}
	}
	if (1368 != 1368) {
		int ustwrb;
		for (ustwrb = 89; ustwrb > 0; ustwrb--) {
			continue;
		}
	}
	if (11493 == 11493) {
		int lfzswunth;
		for (lfzswunth = 15; lfzswunth > 0; lfzswunth--) {
			continue;
		}
	}
	return true;
}

int qjxbajl::mhocihqlxbgkgkzjvizct(int uiajm) {
	int qrqiwuuffxrawbq = 3987;
	int wmwdpkcqcw = 1012;
	int euiqrnnkioz = 4417;
	bool uqche = false;
	double ujxhfxc = 4468;
	double elgphs = 15529;
	double jmspliuc = 22743;
	return 36543;
}

int qjxbajl::vmqgagnkxzj(bool rmlzqybhfu, double ccagcoeo, string jottlbx, string wwoiygjzl, double hgkvfwtel, bool fnsjeq, bool bozjcfmuhij) {
	string gsbgcmufkidisxl = "vxmgnovq";
	string ndwvie = "spxekonjpqzwuqtvkhwqbjflhpckwt";
	bool ctrdu = true;
	if (string("spxekonjpqzwuqtvkhwqbjflhpckwt") != string("spxekonjpqzwuqtvkhwqbjflhpckwt")) {
		int of;
		for (of = 26; of > 0; of--) {
			continue;
		}
	}
	if (string("vxmgnovq") != string("vxmgnovq")) {
		int aeewj;
		for (aeewj = 24; aeewj > 0; aeewj--) {
			continue;
		}
	}
	if (string("vxmgnovq") == string("vxmgnovq")) {
		int zzh;
		for (zzh = 60; zzh > 0; zzh--) {
			continue;
		}
	}
	return 38378;
}

void qjxbajl::vczbqrdfuulvi(int ebccoxlx, double ifwjoykr, int tifpuqu, int mwwhcfmusvphf, int oipktpfus, string fjqpijfvfqdmaj, int xmxzzyyn, string qjfvzlhsmvviq, bool oxaxixukjenha, string ecyrqv) {
	double bqbotnoqbl = 4245;
	bool ybmmhfxzhrwtu = false;
	bool opkoxjjaj = false;
	if (false == false) {
		int xonreqf;
		for (xonreqf = 37; xonreqf > 0; xonreqf--) {
			continue;
		}
	}

}

bool qjxbajl::hvgdgtgmvcezvh(double zpcqoyk, string dbrozhsai, bool ewtxkq) {
	string rwjdzngxchbloc = "gcknjstspgfitoljgerhudbzbfvpejowotybhvvdfvr";
	bool xdtlwryrij = false;
	string htwoci = "hhdehbgkjdisbjccr";
	double vaxzawpmkbhdr = 33274;
	double vnjju = 7909;
	double zdwebe = 3131;
	if (string("gcknjstspgfitoljgerhudbzbfvpejowotybhvvdfvr") != string("gcknjstspgfitoljgerhudbzbfvpejowotybhvvdfvr")) {
		int oxnzdv;
		for (oxnzdv = 36; oxnzdv > 0; oxnzdv--) {
			continue;
		}
	}
	if (33274 != 33274) {
		int ukt;
		for (ukt = 95; ukt > 0; ukt--) {
			continue;
		}
	}
	if (string("gcknjstspgfitoljgerhudbzbfvpejowotybhvvdfvr") == string("gcknjstspgfitoljgerhudbzbfvpejowotybhvvdfvr")) {
		int nqmkbzfv;
		for (nqmkbzfv = 25; nqmkbzfv > 0; nqmkbzfv--) {
			continue;
		}
	}
	if (3131 != 3131) {
		int nrui;
		for (nrui = 67; nrui > 0; nrui--) {
			continue;
		}
	}
	if (33274 == 33274) {
		int vuksmncfhu;
		for (vuksmncfhu = 51; vuksmncfhu > 0; vuksmncfhu--) {
			continue;
		}
	}
	return true;
}

string qjxbajl::qdbquszvfbvhyheqi() {
	int wlwarawb = 1829;
	bool rmpwu = false;
	string drhcgqltjsyjr = "alsazvdgqpfhkqdupsujgvjpnncoqucazsaexwzhvfzwlagfkjv";
	if (string("alsazvdgqpfhkqdupsujgvjpnncoqucazsaexwzhvfzwlagfkjv") == string("alsazvdgqpfhkqdupsujgvjpnncoqucazsaexwzhvfzwlagfkjv")) {
		int drujwxh;
		for (drujwxh = 82; drujwxh > 0; drujwxh--) {
			continue;
		}
	}
	if (string("alsazvdgqpfhkqdupsujgvjpnncoqucazsaexwzhvfzwlagfkjv") != string("alsazvdgqpfhkqdupsujgvjpnncoqucazsaexwzhvfzwlagfkjv")) {
		int bvgv;
		for (bvgv = 55; bvgv > 0; bvgv--) {
			continue;
		}
	}
	if (false != false) {
		int ijhmaemlt;
		for (ijhmaemlt = 39; ijhmaemlt > 0; ijhmaemlt--) {
			continue;
		}
	}
	if (string("alsazvdgqpfhkqdupsujgvjpnncoqucazsaexwzhvfzwlagfkjv") != string("alsazvdgqpfhkqdupsujgvjpnncoqucazsaexwzhvfzwlagfkjv")) {
		int dpuetazlo;
		for (dpuetazlo = 22; dpuetazlo > 0; dpuetazlo--) {
			continue;
		}
	}
	return string("hqblqxhuvoacfkjcst");
}

double qjxbajl::isdjrxcgfqfdjwnfefvgrcek(bool sjyxmpq, string winaugxhay, int crwtvrwldgqin, string fioalgcejgtkgdp, string eacldhuhcn, int lqekxfxdbtpbpzs, double byzmcjjyxmxv, bool fzpugtdpzbqorar, int kpxmjwkbzds, double tjamnsvxwagjiu) {
	int lrnvlovoab = 2725;
	int qfukjteshou = 1829;
	bool ualfzw = false;
	string cvsuxxj = "pjleuouivrybdhlbgrqsbczre";
	string eftbrwfptpr = "otjohztjkztoxwkeyhjiymqkgriirawyxgrgsqpofrjdkisuwkqpofrxwckjcdhcbjdhbgxfmbjicmd";
	string wnkqfzdis = "zmqpdrsdjagaldjtolpgccjrfucz";
	double sdueiard = 23628;
	bool dnwfnudyvkgo = false;
	int abyaxjsmtotiw = 5803;
	if (false != false) {
		int etwtldhex;
		for (etwtldhex = 93; etwtldhex > 0; etwtldhex--) {
			continue;
		}
	}
	if (false == false) {
		int whljfmolrt;
		for (whljfmolrt = 53; whljfmolrt > 0; whljfmolrt--) {
			continue;
		}
	}
	return 32585;
}

int qjxbajl::qapzpweugyauzdylddeqa(bool xvfmtzh, string gzkvmbnecqtw, bool aobypujpgb, string sjkdwwyhrekl, string nhhnhbjdklvv, int mkymyuazamui) {
	double zzpkqaoxv = 4233;
	bool eefxbtxfxqvl = true;
	return 36473;
}

qjxbajl::qjxbajl() {
	this->qapzpweugyauzdylddeqa(true, string("acfjfidsdictcisfaboblevoquxpimfalvz"), true, string("fhttzzhzelolvdiwjskhrkpxequabbtv"), string("lmyhoowvkwjeqbqtfzhqrqjmwozrvpalcoxbecnjnwluliflvuzanqipheokaeahlmsbfrcspunrlyuxaqzmrikumoiofzevnue"), 8571);
	this->isdjrxcgfqfdjwnfefvgrcek(false, string("unorwcvdczcnacneunwuiafrvp"), 1008, string("ymoswtilqhwosgldstwqvbdobdszkunpvdexocygslrsqwfvckrvemnolotgltvkmlmajl"), string("mhushjnlggxtvqagjaigaxdmjxtaotdysmoqrglsdajejovhitotlb"), 3685, 4003, true, 1514, 29803);
	this->espkesgnlrekx(1003);
	this->woddmykngssxafdavllxhbh(string("halbhlowudtasjwmokmwcdibsndzh"), 1308, 27976, 630, false, 13450, false, 698);
	this->mhocihqlxbgkgkzjvizct(4445);
	this->vmqgagnkxzj(false, 26617, string("vpjtdxrtfyhavhyqwhdgdztpwguzxvvwizwkwyobdolkrfbdgwhfvcohaxchkjpb"), string("nbnlitdwwnyzxgfcbpurvrpjuhsoqhhserpzskbwhsqeoxrqtjxfcfsccgzyqynwlcdegwnioulmrkgtfqvaktpdydveumkceyb"), 45986, true, false);
	this->vczbqrdfuulvi(244, 31374, 7758, 2820, 4028, string("xxowtylnoajluutjmsefsaypteofs"), 2832, string("pddojrrsmwlidushugyhdakxdjktijxsvypdruxhgxqnswjnixvngvzabaphkjnnrifomecgjgz"), true, string("cidykofiaqcgiivztsdsxwrsthgfkfkmlxdsplnlpldtmoeiwzbkwsppc"));
	this->hvgdgtgmvcezvh(10978, string("ptr"), true);
	this->qdbquszvfbvhyheqi();
}
