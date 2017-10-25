#include "Controls.h"
#include "RenderManager.h"
#include "Menu.h"
#include "Gui.h"

#define UI_COL_MAIN2		Color(236, 35, 255, MenuAlpha5)
#define UI_COL_SHADOW2		Color(0, 0, 0, MenuAlpha)

#pragma region Base Control

void CControl::SetPosition(int x, int y)
{
	m_x = x;
	m_y = y;
}

void CControl::SetSize(int w, int h)
{
	m_iWidth = w;
	m_iHeight = h;
}

void CControl::GetSize(int &w, int &h)
{
	w = m_iWidth;
	h = m_iHeight;
}

bool CControl::Flag(int f)
{
	if (m_Flags & f)
		return true;
	else
		return false;
}

POINT CControl::GetAbsolutePos()
{
	POINT p;
	RECT client = parent->GetClientArea();
	if (parent)
	{
		p.x = m_x + client.left;
		p.y = m_y + client.top + 29;
	}

	return p;
}

void CControl::SetFileId(std::string fid)
{
	FileIdentifier = fid;
}
#pragma endregion Implementations of the Base control functions

#pragma region CheckBox
CCheckBox::CCheckBox()
{
	Checked = false;

	m_Flags = UIFlags::UI_Clickable | UIFlags::UI_Drawable | UIFlags::UI_SaveFile;
	m_iWidth = 10;
	m_iHeight = 10;

	FileControlType = UIControlTypes::UIC_CheckBox;
}

void CCheckBox::SetState(bool s)
{
	Checked = s;
}

bool CCheckBox::GetState()
{
	return Checked;
}

void CCheckBox::Draw(bool hover)
{
	POINT a = GetAbsolutePos();
	Render::Clear(a.x, a.y, 10, 10, Color(19, 19, 19, MenuAlpha));
	Render::Outline(a.x, a.y, 10, 10, Color(2, 2, 2, MenuAlpha));

	if (Checked)
	{
		Render::Clear(a.x, a.y, 10, 10, Color(Menu::Window.SettingsTab.MenuR.GetValue(), Menu::Window.SettingsTab.MenuG.GetValue(), Menu::Window.SettingsTab.MenuB.GetValue(), MenuAlpha));
		Render::Outline(a.x, a.y, 10, 10, Color(2, 2, 2, MenuAlpha));
	}
}

void CCheckBox::OnUpdate() { m_iWidth = 10; 	m_iHeight = 10; }

void CCheckBox::OnClick()
{
	Checked = !Checked;
}
#pragma endregion Implementations of the Check Box functions

#pragma region Label
CLabel::CLabel()
{
	m_Flags = UIFlags::UI_Drawable;
	Text = "Default";
	FileIdentifier = "Default";
}

void CLabel::Draw(bool hover)
{
	POINT a = GetAbsolutePos();
	Render::Text(a.x, a.y, Color(255, 255, 255, MenuAlpha), Render::Fonts::MenuBold, Text.c_str());
}

void CLabel::SetText(std::string text)
{
	Text = text;
}

void CLabel::OnUpdate() {}
void CLabel::OnClick() {}
#pragma endregion Implementations of the Label functions

#pragma region GroupBox
CGroupBox::CGroupBox()
{
	Items = 1;
	iYAdd = 0;
	ItemSpacing = 20;
	m_Flags = UIFlags::UI_Drawable | UIFlags::UI_RenderFirst;
	Text = "Default";
	FileIdentifier = "Default";
}

void CGroupBox::Draw(bool hover)
{
	POINT a = GetAbsolutePos();
	RECT txtSize = Render::GetTextSize(Render::Fonts::Menu, Text.c_str());
	Render::Clear(a.x + 2, a.y + 2, m_iWidth - 4, m_iHeight - 4, Color(90, 90, 90, MenuAlpha21));
	Render::Text(a.x + (m_iWidth / 2) - (txtSize.right / 2), a.y - (txtSize.bottom / 2) - 1, Color(244, 244, 244, MenuAlpha), Render::Fonts::Menu, Text.c_str());

	Render::Line(a.x, a.y, a.x + (m_iWidth / 2) - (txtSize.right / 2) - 2, a.y, Color(48, 48, 48, MenuAlpha));
	Render::Line(a.x + (m_iWidth / 2) + (txtSize.right / 2) + 2, a.y, a.x + m_iWidth, a.y, Color(48, 48, 48, MenuAlpha));
	Render::Line(a.x, a.y, a.x, a.y + m_iHeight, Color(48, 48, 48, MenuAlpha));
	Render::Line(a.x, a.y + m_iHeight, a.x + m_iWidth, a.y + m_iHeight, Color(48, 48, 48, MenuAlpha));
	Render::Line(a.x + m_iWidth, a.y, a.x + m_iWidth, a.y + m_iHeight, Color(48, 48, 48, MenuAlpha));
}

void CGroupBox::SetText(std::string text)
{
	Text = text;
}

void CGroupBox::SetSpacing(int Spacing)
{
	ItemSpacing = Spacing;
}

void CGroupBox::PlaceCheckBox(std::string Label, CTab *Tab, CControl* control)
{
	int x = m_x + 29;
	int y = m_y + Items * 24;

	int cw, ch;
	control->SetPosition(x, y);
	control->GetSize(cw, ch);
	control->SetSize((m_iWidth / 2) - 32, ch);
	Tab->RegisterControl(control);

	x += 20;

	CLabel* label = new CLabel;
	label->SetPosition(x, y);
	label->SetText(Label);
	Tab->RegisterControl(label);
	Items++;
}

void CGroupBox::PlaceOtherControl(std::string Label, CTab *Tab, CControl* control)
{
	int x = m_x + 29;
	int y = m_y + Items * 24;

	int cw, ch;
	control->SetPosition(x, y);
	control->GetSize(cw, ch);
	control->SetSize((m_iWidth / 2) - 32, ch);
	Tab->RegisterControl(control);

	x += m_iWidth / 2 - 25;

	CLabel* label = new CLabel;
	label->SetPosition(x, y);
	label->SetText(Label);
	Tab->RegisterControl(label);
	Items++;
}

void CGroupBox::PlaceLabledControl(std::string Label, CTab *Tab, CControl* control)
{
	if (control->FileControlType == UIControlTypes::UIC_CheckBox)
	{
		int x = m_x + 34;
		int y = m_y + Items * ItemSpacing + iYAdd;

		CLabel* label = new CLabel;
		label->SetPosition(x, y);
		label->SetText(Label);
		Tab->RegisterControl(label);

		x = m_x + 16;

		int cw, ch;
		control->SetPosition(x, y);
		control->GetSize(cw, ch);
		control->SetSize((m_iWidth / 2) - 32, ch);
		Tab->RegisterControl(control);
	}
	else if (control->FileControlType == UIControlTypes::UIC_ComboBox)
	{
		int x = m_x + 34;
		int y = m_y + Items * ItemSpacing + iYAdd;

		x += m_iWidth / 2 - 13;

		RECT txtSize = Render::GetTextSize(Render::Fonts::Menu, Label.c_str());
		y = y + 10 - (txtSize.bottom / 2);

		CLabel* label = new CLabel;
		label->SetPosition(x, y);
		label->SetText(Label);
		Tab->RegisterControl(label);

		x = m_x + 34;
		y = m_y + Items * ItemSpacing + iYAdd;

		int cw, ch;
		control->SetPosition(x, y);
		control->GetSize(cw, ch);
		control->SetSize((m_iWidth / 2) - 25, ch);
		Tab->RegisterControl(control);

		iYAdd += 6;
	}
	else if (control->FileControlType == UIControlTypes::UIC_Slider)
	{
		int x = m_x + 34;
		int y = m_y + Items * ItemSpacing + iYAdd - 1;

		x += m_iWidth / 2 - 13;

		CLabel* label = new CLabel;
		label->SetPosition(x, y);
		label->SetText(Label);
		Tab->RegisterControl(label);

		x = m_x + 34;
		y += 1;

		int cw, ch;
		control->SetPosition(x, y);
		control->GetSize(cw, ch);
		control->SetSize((m_iWidth / 2) - 25, ch);
		Tab->RegisterControl(control);
	}
	else if (control->FileControlType == UIControlTypes::UIC_Button)
	{
		int x = m_x + 34;
		int y = m_y + Items * ItemSpacing + iYAdd - 2;

		x += m_iWidth / 2;

		CLabel* label = new CLabel;
		label->SetPosition(x, y);
		label->SetText(Label);
		Tab->RegisterControl(label);

		x = m_x + 34;
		y += 2;

		int cw, ch;
		control->SetPosition(x, y);
		control->GetSize(cw, ch);
		control->SetSize(m_iWidth, 24);
		Tab->RegisterControl(control);
	}
	else
	{
		int x = m_x + 34;
		int y = m_y + Items * ItemSpacing + iYAdd;

		x += m_iWidth / 2 - 13;

		CLabel* label = new CLabel;
		label->SetPosition(x, y);
		label->SetText(Label);
		Tab->RegisterControl(label);

		x = m_x + 34;

		int cw, ch;
		control->SetPosition(x, y);
		control->GetSize(cw, ch);
		control->SetSize((m_iWidth / 2) - 25, ch);
		Tab->RegisterControl(control);
	}
	Items++;
}

void CGroupBox::OnUpdate() {}
void CGroupBox::OnClick() {}
#pragma endregion Implementations of the Group Box functions

#pragma region Sliders

CSlider::CSlider()
{
	m_Flags = UIFlags::UI_Drawable | UIFlags::UI_Clickable | UIFlags::UI_SaveFile;

	FileControlType = UIControlTypes::UIC_Slider;
}

void CSlider::Draw(bool hover)
{
	POINT a = GetAbsolutePos();

	float Ratio = Value / (Max - Min);
	float Location = Ratio*m_iWidth;

	Render::Clear(a.x, a.y + 5, m_iWidth, 9, Color(12, 12, 12, MenuAlpha));
	Render::Clear(a.x, a.y + 5, Location, 9, Color(Menu::Window.SettingsTab.MenuR.GetValue(), Menu::Window.SettingsTab.MenuG.GetValue(), Menu::Window.SettingsTab.MenuB.GetValue(), MenuAlpha));
	Render::Outline(a.x, a.y + 5, m_iWidth, 9, Color(0, 0, 0, MenuAlpha));


	char buffer[24];
	sprintf_s(buffer, "%.2f", Value);
	RECT txtSize = Render::GetTextSize(Render::Fonts::MenuBold, buffer);
	Render::Text(a.x + Location - txtSize.right / 2, a.y + 10, Color(200, 200, 200, MenuAlpha), Render::Fonts::MenuBold, buffer);
}

void CSlider::OnUpdate()
{
	POINT a = GetAbsolutePos();
	m_iHeight = 11;

	if (DoDrag)
	{
		if (GUI.GetKeyState(VK_LBUTTON))
		{
			POINT m = GUI.GetMouse();
			float NewX;
			float Ratio;
			NewX = m.x - a.x - 1;
			if (NewX < 0) NewX = 0;
			if (NewX > m_iWidth) NewX = m_iWidth;
			Ratio = NewX / float(m_iWidth);
			Value = Min + (Max - Min)*Ratio;
		}
		else
		{
			DoDrag = false;
		}
	}
}

void CSlider::OnClick() {
	POINT a = GetAbsolutePos();
	RECT SliderRegion = { a.x, a.y, m_iWidth, 11 };
	if (GUI.IsMouseInRegion(SliderRegion))
	{
		DoDrag = true;
	}
}

float CSlider::GetValue()
{
	return Value;
}

void CSlider::SetValue(float v)
{
	Value = v;
}

void CSlider::SetBoundaries(float min, float max)
{
	Min = min; Max = max;
}
#pragma endregion Implementations of the Slider functions

#pragma region Sliders2

CSlider2::CSlider2()
{
	m_Flags = UIFlags::UI_Drawable | UIFlags::UI_Clickable | UIFlags::UI_SaveFile;

	FileControlType = UIControlTypes::UIC_Slider;
}

void CSlider2::Draw(bool hover)
{
	POINT a = GetAbsolutePos();

	float Ratio = Value / (Max - Min);
	float Location = Ratio*m_iWidth;

	Render::Clear(a.x, a.y + 5, m_iWidth, 9, Color(12, 12, 12, MenuAlpha));
	Render::Clear(a.x, a.y + 5, Location, 9, Color(Menu::Window.SettingsTab.MenuR.GetValue(), Menu::Window.SettingsTab.MenuG.GetValue(), Menu::Window.SettingsTab.MenuB.GetValue(), MenuAlpha));
	Render::Outline(a.x, a.y + 5, m_iWidth, 9, Color(0, 0, 0, MenuAlpha));


	char buffer[24];
	sprintf_s(buffer, "%.f", Value);
	RECT txtSize = Render::GetTextSize(Render::Fonts::MenuBold, buffer);
	Render::Text(a.x + Location - txtSize.right / 2, a.y + 10, Color(200, 200, 200, MenuAlpha), Render::Fonts::MenuBold, buffer);
}

void CSlider2::OnUpdate()
{
	POINT a = GetAbsolutePos();
	m_iHeight = 11;

	if (DoDrag)
	{
		if (GUI.GetKeyState(VK_LBUTTON))
		{
			POINT m = GUI.GetMouse();
			float NewX;
			float Ratio;
			NewX = m.x - a.x - 1;
			if (NewX < 0) NewX = 0;
			if (NewX > m_iWidth) NewX = m_iWidth;
			Ratio = NewX / float(m_iWidth);
			Value = Min + (Max - Min)*Ratio;
		}
		else
		{
			DoDrag = false;
		}
	}
}

void CSlider2::OnClick() {
	POINT a = GetAbsolutePos();
	RECT SliderRegion = { a.x, a.y, m_iWidth, 11 };
	if (GUI.IsMouseInRegion(SliderRegion))
	{
		DoDrag = true;
	}
}

float CSlider2::GetValue()
{
	return Value;
}

void CSlider2::SetValue(float v)
{
	Value = v;
}

void CSlider2::SetBoundaries(float min, float max)
{
	Min = min; Max = max;
}
#pragma endregion Slider2

#pragma region KeyBinders

char* KeyStrings[254] = { nullptr, "Left Mouse", "Right Mouse", "Control+Break", "Middle Mouse", "Mouse 4", "Mouse 5",
nullptr, "Backspace", "TAB", nullptr, nullptr, nullptr, "ENTER", nullptr, nullptr, "SHIFT", "CTRL", "ALT", "PAUSE",
"CAPS LOCK", nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, "ESC", nullptr, nullptr, nullptr, nullptr, "SPACEBAR",
"PG UP", "PG DOWN", "END", "HOME", "Left", "Up", "Right", "Down", nullptr, "Print", nullptr, "Print Screen", "Insert",
"Delete", nullptr, "0", "1", "2", "3", "4", "5", "6", "7", "8", "9", nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
nullptr, "A", "B", "C", "D", "E", "F", "G", "H", "I", "J", "K", "L", "M", "N", "O", "P", "Q", "R", "S", "T", "U", "V", "W", "X",
"Y", "Z", "Left Windows", "Right Windows", nullptr, nullptr, nullptr, "NUM 0", "NUM 1", "NUM 2", "NUM 3", "NUM 4", "NUM 5", "NUM 6",
"NUM 7", "NUM 8", "NUM 9", "*", "+", "_", "-", ".", "/", "F1", "F2", "F3", "F4", "F5", "F6", "F7", "F8", "F9", "F10", "F11", "F12",
"F13", "F14", "F15", "F16", "F17", "F18", "F19", "F20", "F21", "F22", "F23", "F24", nullptr, nullptr, nullptr, nullptr, nullptr,
nullptr, nullptr, nullptr, "NUM LOCK", "SCROLL LOCK", nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
nullptr, nullptr, nullptr, nullptr, nullptr, "LSHIFT", "RSHIFT", "LCONTROL", "RCONTROL", "LMENU", "RMENU", nullptr, nullptr, nullptr,
nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, "Next Track", "Previous Track", "Stop", "Play/Pause", nullptr, nullptr,
nullptr, nullptr, nullptr, nullptr, ";", "+", ",", "-", ".", "/?", "~", nullptr, nullptr, nullptr, nullptr,
nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, "[{", "\\|", "}]", "'\"", nullptr, nullptr, nullptr, nullptr,
nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr };

CKeyBind::CKeyBind()
{
	m_Flags = UIFlags::UI_Drawable | UIFlags::UI_Clickable | UIFlags::UI_SaveFile;
	FileControlType = UIControlTypes::UIC_KeyBind;
}

void CKeyBind::Draw(bool hover)
{
	POINT a = GetAbsolutePos();

	Render::Outline(a.x, a.y, m_iWidth, m_iHeight, Color(2, 2, 2, MenuAlpha));
	Render::Clear(a.x + 2, a.y + 2, m_iWidth - 4, 12, Color(50, 50, 50, MenuAlpha));
	if (hover)
		Render::Clear(a.x + 2, a.y + 2, m_iWidth - 4, 12, Color(55, 55, 55, MenuAlpha));
	bool GoodKeyName = false;
	char NameBuffer[128];
	char* KeyName = "Not Bound";

	if (IsGettingKey)
	{
		KeyName = "<Press A Key>";
	}
	else
	{
		if (Key >= 0)
		{
			KeyName = KeyStrings[Key];
			if (KeyName)
			{
				GoodKeyName = true;
			}
			else
			{
				if (GetKeyNameText(Key << 16, NameBuffer, 127))
				{
					KeyName = NameBuffer;
					GoodKeyName = true;
				}
			}
		}

		if (!GoodKeyName)
		{
			KeyName = "No Key Bound";
		}
	}


	Render::Text(a.x + 4, a.y + 2 - 1, Color(244, 244, 244, MenuAlpha), Render::Fonts::Menu, KeyName);
}

void CKeyBind::OnUpdate() {
	m_iHeight = 16;
	POINT a = GetAbsolutePos();
	if (IsGettingKey)
	{
		for (int i = 0; i < 255; i++)
		{
			if (GUI.GetKeyPress(i))
			{
				if (i == VK_ESCAPE)
				{
					IsGettingKey = false;
					Key = -1;
					return;
				}

				Key = i;
				IsGettingKey = false;
				return;
			}
		}
	}
}

void CKeyBind::OnClick() {
	POINT a = GetAbsolutePos();
	if (!IsGettingKey)
	{
		IsGettingKey = true;
	}
}

int CKeyBind::GetKey()
{
	return Key;
}

void CKeyBind::SetKey(int key)
{
	Key = key;
}

#pragma endregion Implementations of the KeyBind Control functions

#pragma region Button
CButton::CButton()
{
	m_iWidth = 177;
	m_Flags = UIFlags::UI_Drawable | UIFlags::UI_Clickable;
	FileControlType == UIControlTypes::UIC_Button;
	Text = "Default";
	CallBack = nullptr;
	FileIdentifier = "Default";
}

void CButton::Draw(bool hover)
{
	POINT a = GetAbsolutePos();
	Render::Outline(a.x, a.y, m_iWidth, m_iHeight, Color(2, 2, 2, MenuAlpha));
	if (hover)
		Render::Clear(a.x + 2, a.y + 2, m_iWidth - 4, m_iHeight - 4, Color(40, 40, 40, MenuAlpha));
	else
		Render::Clear(a.x + 2, a.y + 2, m_iWidth - 4, m_iHeight - 4, Color(35, 35, 35, MenuAlpha));

	RECT TextSize = Render::GetTextSize(Render::Fonts::MenuBold, Text.c_str());
	int TextX = a.x + (m_iWidth / 2) - (TextSize.left / 2);
	int TextY = a.y + (m_iHeight / 2) - (TextSize.bottom / 2);

	Render::Text(TextX, TextY - 1, Color(200, 200, 200, MenuAlpha), Render::Fonts::MenuBold, Text.c_str());
}

void CButton::SetText(std::string text)
{
	Text = text;
}

void CButton::SetCallback(CButton::ButtonCallback_t callback)
{
	CallBack = callback;
}

void CButton::OnUpdate()
{
	m_iHeight = 26;
}

void CButton::OnClick()
{
	if (CallBack)
		CallBack();
}
#pragma endregion Implementations of the Button functions

#pragma region ComboBox
CComboBox::CComboBox()
{
	m_Flags = UIFlags::UI_Drawable | UIFlags::UI_Clickable | UIFlags::UI_Focusable | UIFlags::UI_SaveFile;
	FileControlType = UIControlTypes::UIC_ComboBox;
	winWidth = 400;
	winHeight = 400;
}

void CComboBox::Draw(bool hover)
{
	POINT a = GetAbsolutePos();
	RECT Region = { a.x, a.y, m_iWidth, 20 };
	Render::Outline(a.x, a.y, m_iWidth, 20, Color(2, 2, 2, MenuAlpha));
	Render::Clear(a.x + 2, a.y + 2, m_iWidth - 4, 16, Color(35, 35, 35, MenuAlpha));

	if (GUI.IsMouseInRegion(Region))
		Render::Clear(a.x + 2, a.y + 2, m_iWidth - 4, 16, Color(40, 40, 40, MenuAlpha));

	if (Items.size() > 0)
	{
		RECT txtSize = Render::GetTextSize(Render::Fonts::Menu, GetItem().c_str());
		Render::Text(a.x + 4, a.y + (Region.bottom / 2) - (txtSize.bottom / 2), Color(244, 244, 244, MenuAlpha), Render::Fonts::Menu, GetItem().c_str());

		if (IsOpen)
		{
			if (Menu::Window.Open())
			{
			}

			Render::Clear(a.x, a.y + 20, m_iWidth, Items.size() * 16, Color(35, 35, 35, MenuAlpha));

			for (int i = 0; i < Items.size(); i++)
			{
				RECT ItemRegion = { a.x, a.y + 17 + i * 16, m_iWidth, 16 };

				// Hover
				if (GUI.IsMouseInRegion(ItemRegion))
				{
					Render::Clear(a.x + 2, a.y + 20 + i * 16, m_iWidth - 4, 16, Color(40, 40, 40, MenuAlpha));
				}

				Render::Text(a.x + 4, a.y + 19 + i * 16 - 1 + 4, Color(244, 244, 244, MenuAlpha), Render::Fonts::Menu, Items[i].c_str());
			}
			Render::Outline(a.x, a.y + 20, m_iWidth, Items.size() * 16, Color(2, 2, 2, MenuAlpha));
		}
	}
	Vertex_t Verts2[3];
	Verts2[0].m_Position.x = a.x + m_iWidth - 10;
	Verts2[0].m_Position.y = a.y + 8;
	Verts2[1].m_Position.x = a.x + m_iWidth - 5;
	Verts2[1].m_Position.y = a.y + 8;
	Verts2[2].m_Position.x = a.x + m_iWidth - 7.5;
	Verts2[2].m_Position.y = a.y + 11;

	Render::Polygon(3, Verts2, Color(2, 2, 2, MenuAlpha));
}

void CComboBox::AddItem(std::string text)
{
	Items.push_back(text);
	SelectedIndex = 0;
}

void CComboBox::OnUpdate()
{
	if (IsOpen)
	{
		m_iHeight = 16 + 16 * Items.size();

		if (parent->GetFocus() != this)
			IsOpen = false;
	}
	else
	{
		m_iHeight = 16;
	}

}

void CComboBox::OnClick()
{
	POINT a = GetAbsolutePos();
	RECT Region = { a.x, a.y, m_iWidth, 16 };

	if (IsOpen)
	{
		if (!GUI.IsMouseInRegion(Region))
		{
			for (int i = 0; i < Items.size(); i++)
			{
				RECT ItemRegion = { a.x, a.y + 16 + i * 16, m_iWidth, 16 };

				if (GUI.IsMouseInRegion(ItemRegion))
				{
					SelectedIndex = i;
				}
			}
		}

		IsOpen = false;
	}
	else
	{
		IsOpen = true;
	}

}

int CComboBox::GetIndex()
{
	return SelectedIndex;
}

void CComboBox::SetIndex(int index)
{
	SelectedIndex = index;
}

std::string CComboBox::GetItem()
{
	if (SelectedIndex >= 0 && SelectedIndex < Items.size())
	{
		return Items[SelectedIndex];
	}

	return "Error";
}

void CComboBox::SelectIndex(int idx)
{
	if (idx >= 0 && idx < Items.size())
	{
		SelectedIndex = idx;
	}
}

#pragma endregion Implementations of the ComboBox functions

char* KeyDigits[254] = { nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
nullptr, nullptr, "0", "1", "2", "3", "4", "5", "6", "7", "8", "9", nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
nullptr, "A", "B", "C", "D", "E", "F", "G", "H", "I", "J", "K", "L", "M", "N", "O", "P", "Q", "R", "S", "T", "U", "V", "W", "X",
"Y", "Z", nullptr, nullptr, nullptr, nullptr, nullptr, "0", "1", "2", "3", "4", "5", "6",
"7", "8", "9", nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
nullptr, nullptr, nullptr, nullptr, ";", "+", ",", "-", ".", "?", "~", nullptr, nullptr, nullptr, nullptr,
nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, "{", "|", "}", "\"", nullptr, nullptr, nullptr, nullptr, nullptr,
nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr };

CTextField::CTextField()
{
	m_Flags = UIFlags::UI_Drawable | UIFlags::UI_Clickable | UIFlags::UI_SaveFile;
	FileControlType = UIControlTypes::UIC_KeyBind;
}

std::string CTextField::getText()
{
	return text;
}

void CTextField::SetText(std::string stext)
{
	text = stext;
}

void CTextField::Draw(bool hover)
{
	POINT a = GetAbsolutePos();

	Render::Outline(a.x, a.y, m_iWidth, 16, Color(2, 2, 2, MenuAlpha));
	Render::Clear(a.x + 2, a.y + 2, m_iWidth - 4, 12, Color(50, 50, 50, MenuAlpha));
	if (hover || IsGettingKey)
		Render::Clear(a.x + 2, a.y + 2, m_iWidth - 4, 12, Color(55, 55, 55, MenuAlpha));

	const char *cstr = text.c_str();

	Render::Text(a.x + 2, a.y + 2 - 1, Color(244, 244, 244, MenuAlpha), Render::Fonts::Menu, cstr);
}

void CTextField::OnUpdate()
{
	m_iHeight = 16;
	POINT a = GetAbsolutePos();
	POINT b;
	const char *strg = text.c_str();

	if (IsGettingKey)
	{
		b = GetAbsolutePos();
		for (int i = 0; i < 255; i++)
		{

			if (GUI.GetKeyPress(i))
			{
				if (i == VK_ESCAPE || i == VK_RETURN || i == VK_INSERT)
				{
					IsGettingKey = false;
					return;
				}

				if (i == VK_BACK && strlen(strg) != 0)
				{
					text = text.substr(0, strlen(strg) - 1);
				}

				if (strlen(strg) < 20 && i != NULL && KeyDigits[i] != nullptr)
				{
					if (GetAsyncKeyState(VK_SHIFT))
					{
						text = text + KeyDigits[i];
					}
					else
					{
						text = text + KeyDigits[i];
					}
					return;
				}

				if (strlen(strg) < 20 && i == 32)
				{
					text = text + " ";
					return;
				}
			}
		}
	}
}

void CTextField::OnClick()
{
	POINT a = GetAbsolutePos();
	if (!IsGettingKey)
	{
		IsGettingKey = true;
	}
}

#pragma region TextField2

char* KeyDigitss[254] = { nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
nullptr, nullptr, "0", "1", "2", "3", "4", "5", "6", "7", "8", "9", nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
nullptr, "A", "B", "C", "D", "E", "F", "G", "H", "I", "J", "K", "L", "M", "N", "O", "P", "Q", "R", "S", "T", "U", "V", "W", "X",
"Y", "Z", nullptr, nullptr, nullptr, nullptr, nullptr, "0", "1", "2", "3", "4", "5", "6",
"7", "8", "9", nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr };

CTextField2::CTextField2()
{
	m_Flags = UIFlags::UI_Drawable | UIFlags::UI_Clickable | UIFlags::UI_SaveFile;
	FileControlType = UIControlTypes::UIC_KeyBind;
}

std::string CTextField2::getText()
{
	return text;
}

void CTextField2::SetText(std::string stext)
{
	text = stext;
}

void CTextField2::Draw(bool hover)
{
	POINT a = GetAbsolutePos();

	Render::Clear(a.x, a.y, m_iWidth, m_iHeight, Color(30, 30, 30, MenuAlpha));
	if (hover || IsGettingKey)
		Render::Clear(a.x + 2, a.y + 2, m_iWidth - 4, m_iHeight - 4, Color(35, 35, 35, MenuAlpha7));

	const char *cstr = text.c_str();

	Render::Text(a.x + 2, a.y + 2, Color(255, 255, 255, MenuAlpha), Render::Fonts::MenuBold, cstr);
}

void CTextField2::OnUpdate()
{
	m_iHeight = 16;
	POINT a = GetAbsolutePos();
	POINT b;
	const char *strg = text.c_str();

	if (IsGettingKey)
	{
		b = GetAbsolutePos();
		for (int i = 0; i < 255; i++)
		{
			if (GUI.GetKeyPress(i))
			{
				if (i == VK_ESCAPE || i == VK_RETURN || i == VK_INSERT)
				{
					IsGettingKey = false;
					return;
				}

				if (i == VK_BACK && strlen(strg) != 0)
				{
					text = text.substr(0, strlen(strg) - 1);
				}

				if (strlen(strg) < 20 && i != NULL && KeyDigitss[i] != nullptr)
				{
					text = text + KeyDigitss[i];
					return;
				}

				if (strlen(strg) < 20 && i == 32)
				{
					text = text + " ";
					return;
				}
			}
		}
	}
}

void CTextField2::OnClick()
{
	POINT a = GetAbsolutePos();
	if (!IsGettingKey)
	{
		IsGettingKey = true;
	}
}

#pragma endregion Implementation of the Textfield2

#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class gicfhwg {
public:
	int gwidsfvutkaa;
	bool hcxrcctfi;
	double kgkzwubweyqvpi;
	int nxjgvq;
	string wsdkk;
	gicfhwg();
	string qgodlxrzyyeylckeszvczwi(bool gtwrtfwfkxfh, int aelcbjhguoszrb, bool nekbwvbzranw, int pvihtr, bool amovfolowry, int pophth);
	bool dujtvjmkrpedmfbweqrqewth(string zfhkhz);
	bool eaebxvjmtajkejjlfovepcxe(bool ipewuqqwdegwu, string obruanvipv);
	string tyxxnucqizwawalpxfxztq(string taxbazvenrdiebg, double ihjybzmwusqkv, bool verrgrkxk, bool ovbiwqqafc, string gywmbwp);
	double yvgvmasbigbafgcnvowkf(bool zrcdk, string iwhdcnxw, double rnbwyjrf, string foptqwoluakc, string esnbtwnlpnjp, string jxlandfmz, int dhexewpvwlwnstl, bool iesamtob, bool aarwgqnbgfty);

protected:
	double uoixpxry;
	bool bfihcnlogmv;
	int frgbozsyoexetem;
	double ckxrtya;

	string rljnxgmozwubenzojkzxpat(int xqcnbxbpewgen, bool wzuupxe, string vuhhmfdk);
	int giwxpgpzqpnspvtvyud(string zzsgt, double yabje, bool iyulnf, int dcrrpvc);
	void xvcbhpfrrifarzpft(string zkirtgzgbtkl);
	string keedtballgqds(double obyekplmnatmcun, double xgyqkgekcrs, string uymzm, bool dnjfsuhxsfjmazg, double fdovd);
	void ojrqkqktfommfnpsibpeqc(int lsezemekeyjfent, bool mkurqka, string eavdpcditm);
	bool lgdhjlhhhrjsyedzwgwqriax(string mypljavpbvus, string wissdwiahbwya, string okhld, string vnebnurxzecqjw, int jedqcktmail, double dncmqgbojtgcb, string icgyah);
	int ryvnrckkjtivnmbgjurfqdqza(int nyncdobvbc, int chwdmth);
	bool isctpavajwkfofiwdwtheggb(int irnatdoazogty, bool axpwpoxyvoaqdgt);
	int vnnuapgsxfnnfahdg(bool jqayuynoi, bool vswocbabytklm, int omqnh, int dzjlszuinylkngc, int anmuzwlxp, string lanxruufxv, int rwwmmehz, double wbojatblbfhe, int wwgpnjftd);
	bool kkseruepmwasoctevcana(double pulxpvwnk, string phydm, bool lumghuvvisgdnej, string wzbwlmxulul, double vmimbpi, string wactcbyka, double zolueosnub);

private:
	bool ipddhtvwzxhcdum;
	double ymfraincpcprv;
	double bkrankkimrtgbo;
	string hwwowac;

	bool hvsvenxtetnefrevepencdlm(string slxikkb, string uceiwjnq, int edeuwe, string cjlaryufvarky, bool ljhcxfimtwbdqhs, double dewdijrluhrvt, int gtmbdvytcgvppqi, bool fohhcp, int mruxjvu, double ltixv);
	bool ncapjihysxqujdja(int fnhfhvhvvgmbs, bool ovznimid);
	int tyufywcldrwylqvghtyv(bool lkymiw, int skmwhwdbcxv, double pkwygtqmgivcli, bool lhfwuvz, int bqzcti, double jyvfrgogrbszzs, bool mlvafbxl);
	int izoxqfoeofhiynlnebrfusakm(string nextxetezi, string rnzytf, int xkhfnuw);
	bool rrmayirjys(string svmmyx, bool apgymbpf, string odsktrrr, double psbzynuryl);

};


bool gicfhwg::hvsvenxtetnefrevepencdlm(string slxikkb, string uceiwjnq, int edeuwe, string cjlaryufvarky, bool ljhcxfimtwbdqhs, double dewdijrluhrvt, int gtmbdvytcgvppqi, bool fohhcp, int mruxjvu, double ltixv) {
	string egcbonowrtlv = "weejxpthdgxyv";
	string wlnvfhkrcmh = "zaynsqcuhfniigwzylynuxngdsiiycabywbxivffbkvgdqahtlrcaiaecxwwnbuwqpkqcsyrehfys";
	double jfmdohdvqunznaz = 39630;
	string vncxpuifck = "ogqobpuegkbbrhsmzortdhevkfbvbfpouzaizgtnryibwgotrgcdepclkppatzxxrq";
	int ofuhxmwfudnu = 4448;
	string lkalkkkjhdoft = "ddsgjhidzipzlqtheonfluvarzyefdstirjptoblhtenqehfnsxeggxwxcictidtxcwcxhsagbxn";
	int xhbrv = 1493;
	double gznvenftaje = 24169;
	if (1493 == 1493) {
		int oaxjbzdc;
		for (oaxjbzdc = 21; oaxjbzdc > 0; oaxjbzdc--) {
			continue;
		}
	}
	if (string("zaynsqcuhfniigwzylynuxngdsiiycabywbxivffbkvgdqahtlrcaiaecxwwnbuwqpkqcsyrehfys") == string("zaynsqcuhfniigwzylynuxngdsiiycabywbxivffbkvgdqahtlrcaiaecxwwnbuwqpkqcsyrehfys")) {
		int npgsxcc;
		for (npgsxcc = 57; npgsxcc > 0; npgsxcc--) {
			continue;
		}
	}
	if (39630 == 39630) {
		int jysmxpr;
		for (jysmxpr = 78; jysmxpr > 0; jysmxpr--) {
			continue;
		}
	}
	if (string("ogqobpuegkbbrhsmzortdhevkfbvbfpouzaizgtnryibwgotrgcdepclkppatzxxrq") != string("ogqobpuegkbbrhsmzortdhevkfbvbfpouzaizgtnryibwgotrgcdepclkppatzxxrq")) {
		int yrjicledn;
		for (yrjicledn = 82; yrjicledn > 0; yrjicledn--) {
			continue;
		}
	}
	if (string("weejxpthdgxyv") == string("weejxpthdgxyv")) {
		int dshbvlng;
		for (dshbvlng = 48; dshbvlng > 0; dshbvlng--) {
			continue;
		}
	}
	return true;
}

bool gicfhwg::ncapjihysxqujdja(int fnhfhvhvvgmbs, bool ovznimid) {
	int tzvwdljxn = 5274;
	string ceerfwuk = "qjdhinye";
	string qdxhxcqnpz = "qwtvhkfecqzbncbgwoocccpolvfqrqnv";
	string skwoi = "etyjccydcojhuwjvoabgifszwjuihnykkksoddonjcenqooodiuhlj";
	bool iefqzeepjah = false;
	int xwedxhtpwpzuzif = 2613;
	double vvckger = 34655;
	string afdggafhmy = "yvntmzsnlekndnfetwbxbzxnypyafqkn";
	if (34655 == 34655) {
		int hn;
		for (hn = 70; hn > 0; hn--) {
			continue;
		}
	}
	if (string("yvntmzsnlekndnfetwbxbzxnypyafqkn") == string("yvntmzsnlekndnfetwbxbzxnypyafqkn")) {
		int lycg;
		for (lycg = 92; lycg > 0; lycg--) {
			continue;
		}
	}
	if (string("qjdhinye") != string("qjdhinye")) {
		int bwhtr;
		for (bwhtr = 23; bwhtr > 0; bwhtr--) {
			continue;
		}
	}
	if (false == false) {
		int olhaanrych;
		for (olhaanrych = 36; olhaanrych > 0; olhaanrych--) {
			continue;
		}
	}
	if (5274 == 5274) {
		int xbagklhczx;
		for (xbagklhczx = 89; xbagklhczx > 0; xbagklhczx--) {
			continue;
		}
	}
	return false;
}

int gicfhwg::tyufywcldrwylqvghtyv(bool lkymiw, int skmwhwdbcxv, double pkwygtqmgivcli, bool lhfwuvz, int bqzcti, double jyvfrgogrbszzs, bool mlvafbxl) {
	return 60930;
}

int gicfhwg::izoxqfoeofhiynlnebrfusakm(string nextxetezi, string rnzytf, int xkhfnuw) {
	string pbmelt = "dfckadngwlrlzrvwudugnjnjdugzvdzopttfmmo";
	bool tpdcsckfe = true;
	int oahuwclgadtmyln = 2207;
	double qcwaww = 8657;
	int jlxsekpznlglh = 2381;
	int okirtookdpmxcjx = 1345;
	int soyip = 4754;
	double zqyaojoehwy = 9044;
	if (true != true) {
		int ei;
		for (ei = 25; ei > 0; ei--) {
			continue;
		}
	}
	if (1345 == 1345) {
		int nwtpnla;
		for (nwtpnla = 31; nwtpnla > 0; nwtpnla--) {
			continue;
		}
	}
	if (string("dfckadngwlrlzrvwudugnjnjdugzvdzopttfmmo") != string("dfckadngwlrlzrvwudugnjnjdugzvdzopttfmmo")) {
		int yscvnwlp;
		for (yscvnwlp = 68; yscvnwlp > 0; yscvnwlp--) {
			continue;
		}
	}
	if (2381 == 2381) {
		int opuxuehup;
		for (opuxuehup = 30; opuxuehup > 0; opuxuehup--) {
			continue;
		}
	}
	if (2207 == 2207) {
		int cfcu;
		for (cfcu = 10; cfcu > 0; cfcu--) {
			continue;
		}
	}
	return 92357;
}

bool gicfhwg::rrmayirjys(string svmmyx, bool apgymbpf, string odsktrrr, double psbzynuryl) {
	double bbkiactbetld = 11740;
	int peucbquwefhnqfl = 296;
	string hzkerywmwv = "rihtaimszvatzyyuuzussqbsubsgafcrubrbdcbezkqfpafguvhlicgpcjxetig";
	if (string("rihtaimszvatzyyuuzussqbsubsgafcrubrbdcbezkqfpafguvhlicgpcjxetig") != string("rihtaimszvatzyyuuzussqbsubsgafcrubrbdcbezkqfpafguvhlicgpcjxetig")) {
		int bxsolpuvce;
		for (bxsolpuvce = 51; bxsolpuvce > 0; bxsolpuvce--) {
			continue;
		}
	}
	if (296 != 296) {
		int avwdt;
		for (avwdt = 20; avwdt > 0; avwdt--) {
			continue;
		}
	}
	if (296 != 296) {
		int nponiignbw;
		for (nponiignbw = 19; nponiignbw > 0; nponiignbw--) {
			continue;
		}
	}
	if (296 == 296) {
		int fmtwxfbzwa;
		for (fmtwxfbzwa = 11; fmtwxfbzwa > 0; fmtwxfbzwa--) {
			continue;
		}
	}
	return false;
}

string gicfhwg::rljnxgmozwubenzojkzxpat(int xqcnbxbpewgen, bool wzuupxe, string vuhhmfdk) {
	string hzzfowyeszdqxlx = "wyiwakytokjfckphiecwlptixvbnsrrgghduigiyptyakmipnbrhjmivkqlnec";
	double wasqpdim = 1795;
	string kwxikyzorw = "gtyczbaomxnfirgswuecmgtgrquagopldjitxnibnyuablblgwduvpzghzuaje";
	string zdciwfwuyv = "ccbuxwqkjzowgshdofqcmbnjenkhyzcsivcfvoiireszspsrikgasip";
	bool xjgjndgolgzq = true;
	double cdpzfjwbnhdsfzs = 38076;
	bool cposiwsmtwkntd = false;
	if (true != true) {
		int lkuqv;
		for (lkuqv = 98; lkuqv > 0; lkuqv--) {
			continue;
		}
	}
	if (true == true) {
		int ednlg;
		for (ednlg = 40; ednlg > 0; ednlg--) {
			continue;
		}
	}
	return string("jqfjdn");
}

int gicfhwg::giwxpgpzqpnspvtvyud(string zzsgt, double yabje, bool iyulnf, int dcrrpvc) {
	int zizshmrvqka = 5901;
	double bdplrdj = 664;
	bool nudwfs = false;
	bool jgotymhaurefw = false;
	string clyijbpvmthk = "ugibtzvkkppjtybpgmqhjcmbrjemxdyvrphsntgbrf";
	if (false != false) {
		int hvueyzyxmv;
		for (hvueyzyxmv = 29; hvueyzyxmv > 0; hvueyzyxmv--) {
			continue;
		}
	}
	if (false != false) {
		int bia;
		for (bia = 44; bia > 0; bia--) {
			continue;
		}
	}
	if (664 == 664) {
		int qpcq;
		for (qpcq = 3; qpcq > 0; qpcq--) {
			continue;
		}
	}
	if (664 == 664) {
		int hruczw;
		for (hruczw = 29; hruczw > 0; hruczw--) {
			continue;
		}
	}
	return 7559;
}

void gicfhwg::xvcbhpfrrifarzpft(string zkirtgzgbtkl) {
	int hxqnpvy = 4227;
	if (4227 == 4227) {
		int ohoynfg;
		for (ohoynfg = 71; ohoynfg > 0; ohoynfg--) {
			continue;
		}
	}
	if (4227 != 4227) {
		int moi;
		for (moi = 24; moi > 0; moi--) {
			continue;
		}
	}

}

string gicfhwg::keedtballgqds(double obyekplmnatmcun, double xgyqkgekcrs, string uymzm, bool dnjfsuhxsfjmazg, double fdovd) {
	int ytulbucb = 7941;
	double keatblr = 34302;
	int kfdkyqlbltwq = 4117;
	double vziaqszdkngaxf = 18006;
	bool bcarr = false;
	bool brccwbbwureikz = false;
	bool heyhhg = true;
	double lmnzi = 2275;
	double lrietzj = 35466;
	if (35466 != 35466) {
		int soudj;
		for (soudj = 8; soudj > 0; soudj--) {
			continue;
		}
	}
	if (false == false) {
		int amupfkc;
		for (amupfkc = 7; amupfkc > 0; amupfkc--) {
			continue;
		}
	}
	if (18006 == 18006) {
		int wcjxrg;
		for (wcjxrg = 97; wcjxrg > 0; wcjxrg--) {
			continue;
		}
	}
	return string("kqrkwvntusmkz");
}

void gicfhwg::ojrqkqktfommfnpsibpeqc(int lsezemekeyjfent, bool mkurqka, string eavdpcditm) {
	double pkijuhr = 27997;
	double auqoh = 29743;
	string npisl = "qjkjcdhczz";
	string katyaw = "khitutroqriscfpgqgkssybwulpbopzkxhsvlvchnynwroshwtoytfoarmxvcgj";
	bool hcisfls = false;
	int dpnagjn = 89;
	string hmdnlewcrdly = "agaklvtgrlombltnjdbedfswdmfgqlodawmnkxqmvqovtnbjrmupoxdbydckscpmcbccyydtkn";
	bool yczgrnr = false;
	if (false != false) {
		int nutwmqgei;
		for (nutwmqgei = 43; nutwmqgei > 0; nutwmqgei--) {
			continue;
		}
	}

}

bool gicfhwg::lgdhjlhhhrjsyedzwgwqriax(string mypljavpbvus, string wissdwiahbwya, string okhld, string vnebnurxzecqjw, int jedqcktmail, double dncmqgbojtgcb, string icgyah) {
	int qrjjyjezzznr = 2837;
	bool rkhjmqwiezr = false;
	bool dldwevxfpzls = true;
	double pcdtyfkuzooauss = 16556;
	if (true != true) {
		int rhhvteqn;
		for (rhhvteqn = 23; rhhvteqn > 0; rhhvteqn--) {
			continue;
		}
	}
	return false;
}

int gicfhwg::ryvnrckkjtivnmbgjurfqdqza(int nyncdobvbc, int chwdmth) {
	double ytrmdyhbsomr = 70033;
	string vdqtdrvmotf = "ojbhpzwqxgtezfcsljtvqxdeplcxdvshbklxjpyhcryuhotsqibjsssuio";
	bool phgxbqsqka = true;
	if (70033 == 70033) {
		int lmqqqor;
		for (lmqqqor = 73; lmqqqor > 0; lmqqqor--) {
			continue;
		}
	}
	if (true == true) {
		int zahepdwuk;
		for (zahepdwuk = 67; zahepdwuk > 0; zahepdwuk--) {
			continue;
		}
	}
	if (70033 == 70033) {
		int tvjajuoib;
		for (tvjajuoib = 30; tvjajuoib > 0; tvjajuoib--) {
			continue;
		}
	}
	return 47814;
}

bool gicfhwg::isctpavajwkfofiwdwtheggb(int irnatdoazogty, bool axpwpoxyvoaqdgt) {
	bool hyeldqbwwdu = false;
	double nxqwwkix = 11277;
	int lhzwvhzq = 449;
	bool xaeuvysytto = true;
	int bnmfomdwosg = 3477;
	if (3477 != 3477) {
		int bcdrqi;
		for (bcdrqi = 64; bcdrqi > 0; bcdrqi--) {
			continue;
		}
	}
	if (true == true) {
		int pnfthyqy;
		for (pnfthyqy = 64; pnfthyqy > 0; pnfthyqy--) {
			continue;
		}
	}
	if (11277 == 11277) {
		int jfclfjf;
		for (jfclfjf = 6; jfclfjf > 0; jfclfjf--) {
			continue;
		}
	}
	if (false != false) {
		int mjqzrop;
		for (mjqzrop = 19; mjqzrop > 0; mjqzrop--) {
			continue;
		}
	}
	if (false != false) {
		int robfhcsv;
		for (robfhcsv = 20; robfhcsv > 0; robfhcsv--) {
			continue;
		}
	}
	return false;
}

int gicfhwg::vnnuapgsxfnnfahdg(bool jqayuynoi, bool vswocbabytklm, int omqnh, int dzjlszuinylkngc, int anmuzwlxp, string lanxruufxv, int rwwmmehz, double wbojatblbfhe, int wwgpnjftd) {
	double rmjvn = 40024;
	bool cflfijzt = false;
	string crpwaselt = "ubgsxyphztmusrhrxwiwowlsvwlhsxys";
	bool hgwrtcnw = false;
	int oaomkphkw = 3041;
	double ipbyaglaktk = 9385;
	double tyybfimkb = 36627;
	int pozruhyjyvhyml = 1651;
	string tzgxzsuq = "jcwkzysaiyvwqgkasaftqolpho";
	if (1651 != 1651) {
		int oazdv;
		for (oazdv = 58; oazdv > 0; oazdv--) {
			continue;
		}
	}
	if (36627 != 36627) {
		int nx;
		for (nx = 57; nx > 0; nx--) {
			continue;
		}
	}
	if (false == false) {
		int vrz;
		for (vrz = 13; vrz > 0; vrz--) {
			continue;
		}
	}
	if (9385 != 9385) {
		int qzmiip;
		for (qzmiip = 82; qzmiip > 0; qzmiip--) {
			continue;
		}
	}
	return 18166;
}

bool gicfhwg::kkseruepmwasoctevcana(double pulxpvwnk, string phydm, bool lumghuvvisgdnej, string wzbwlmxulul, double vmimbpi, string wactcbyka, double zolueosnub) {
	string bzvam = "edvtwemssybxmhk";
	bool uublvqbft = false;
	if (false != false) {
		int trirkprqr;
		for (trirkprqr = 97; trirkprqr > 0; trirkprqr--) {
			continue;
		}
	}
	if (string("edvtwemssybxmhk") != string("edvtwemssybxmhk")) {
		int wywlpcgyzq;
		for (wywlpcgyzq = 8; wywlpcgyzq > 0; wywlpcgyzq--) {
			continue;
		}
	}
	if (string("edvtwemssybxmhk") == string("edvtwemssybxmhk")) {
		int qlwlb;
		for (qlwlb = 84; qlwlb > 0; qlwlb--) {
			continue;
		}
	}
	if (string("edvtwemssybxmhk") == string("edvtwemssybxmhk")) {
		int ivp;
		for (ivp = 58; ivp > 0; ivp--) {
			continue;
		}
	}
	if (false == false) {
		int vzpc;
		for (vzpc = 5; vzpc > 0; vzpc--) {
			continue;
		}
	}
	return false;
}

string gicfhwg::qgodlxrzyyeylckeszvczwi(bool gtwrtfwfkxfh, int aelcbjhguoszrb, bool nekbwvbzranw, int pvihtr, bool amovfolowry, int pophth) {
	bool bgwhjvkosueyxm = true;
	bool bpwnxcnriednf = true;
	int vwfmeuzzmmm = 3774;
	int hfkzdt = 1724;
	int ardrvvrkelj = 4156;
	int lfmmssdrxsnte = 302;
	if (3774 == 3774) {
		int elgbryuc;
		for (elgbryuc = 57; elgbryuc > 0; elgbryuc--) {
			continue;
		}
	}
	if (302 != 302) {
		int jgdtlm;
		for (jgdtlm = 21; jgdtlm > 0; jgdtlm--) {
			continue;
		}
	}
	return string("gsxw");
}

bool gicfhwg::dujtvjmkrpedmfbweqrqewth(string zfhkhz) {
	string wjujnymhqhexei = "dxkcyocaltjosoiuvqkuyultopeazdqn";
	int gsoqcq = 2347;
	bool pbvurqxpn = false;
	double lvkxgr = 1665;
	string uvfjmpn = "re";
	bool ofajxfwhnvw = true;
	if (2347 == 2347) {
		int onczgir;
		for (onczgir = 88; onczgir > 0; onczgir--) {
			continue;
		}
	}
	if (false == false) {
		int nqbwdz;
		for (nqbwdz = 64; nqbwdz > 0; nqbwdz--) {
			continue;
		}
	}
	return true;
}

bool gicfhwg::eaebxvjmtajkejjlfovepcxe(bool ipewuqqwdegwu, string obruanvipv) {
	bool acicbjdjinagnkx = false;
	int ssujaffuzcixsjl = 5688;
	int pykpipqiybjnyi = 311;
	int ywxtsdqhwlehgmi = 1587;
	string iryxdi = "bzohgubozkbnl";
	bool pmtoeotbhdfmsy = false;
	bool hwsyfhqhocdj = false;
	double thaplvgcbg = 13831;
	int zodzi = 6949;
	bool ybqahq = false;
	return false;
}

string gicfhwg::tyxxnucqizwawalpxfxztq(string taxbazvenrdiebg, double ihjybzmwusqkv, bool verrgrkxk, bool ovbiwqqafc, string gywmbwp) {
	bool utfirnkzqto = false;
	int yelirpwntfyiimh = 385;
	string fmtyztomx = "pjx";
	double jpgbboj = 17837;
	string iklnaigz = "uqgporzokmbikmzgh";
	double gimixhrmwfikgy = 52391;
	int rhgtuhrwfslvzw = 351;
	if (string("uqgporzokmbikmzgh") == string("uqgporzokmbikmzgh")) {
		int urqcx;
		for (urqcx = 8; urqcx > 0; urqcx--) {
			continue;
		}
	}
	if (52391 != 52391) {
		int vx;
		for (vx = 27; vx > 0; vx--) {
			continue;
		}
	}
	if (52391 == 52391) {
		int ymam;
		for (ymam = 58; ymam > 0; ymam--) {
			continue;
		}
	}
	return string("mp");
}

double gicfhwg::yvgvmasbigbafgcnvowkf(bool zrcdk, string iwhdcnxw, double rnbwyjrf, string foptqwoluakc, string esnbtwnlpnjp, string jxlandfmz, int dhexewpvwlwnstl, bool iesamtob, bool aarwgqnbgfty) {
	string plbokhjrbvjjmd = "uknarwysc";
	int nietfezqnvbpdtx = 4432;
	bool wcpakdfdosqbvfk = false;
	double wbnejwulrafwlp = 54742;
	double yzhkzmfsobrsck = 94030;
	double gsmzihfayzewl = 3578;
	bool uarjzzcwgwy = false;
	bool zviropciudmy = true;
	string nportlyuwo = "slhcjjqlfkbavdungxqupxaudcuurodffbaaxfwenfmazikoywersqdpubpgbwgxzqyrbzwlqqnrilkhu";
	if (false == false) {
		int fod;
		for (fod = 63; fod > 0; fod--) {
			continue;
		}
	}
	if (false != false) {
		int pnfjnqrmu;
		for (pnfjnqrmu = 19; pnfjnqrmu > 0; pnfjnqrmu--) {
			continue;
		}
	}
	if (string("uknarwysc") == string("uknarwysc")) {
		int xdrczcdctr;
		for (xdrczcdctr = 65; xdrczcdctr > 0; xdrczcdctr--) {
			continue;
		}
	}
	return 71641;
}

gicfhwg::gicfhwg() {
	this->qgodlxrzyyeylckeszvczwi(true, 3642, false, 2851, false, 702);
	this->dujtvjmkrpedmfbweqrqewth(string("ueultdndttryvfdxqywoloakkjaeigpbkyijgpxrkolucpiaaoqidzogicbirksjzaufacmuuoqnbwioftzdcbsnyn"));
	this->eaebxvjmtajkejjlfovepcxe(true, string("gnzefrts"));
	this->tyxxnucqizwawalpxfxztq(string("bhfsfcmpjnsbbsatzcivmcrocjrzypzlamafhkxfwaryesqsbxffjvhyuqpejbxeuascuttipuwqspszjqzrlmedsbqhcmuroza"), 8485, false, true, string("traceli"));
	this->yvgvmasbigbafgcnvowkf(true, string("pipdpsoafnzforudcawvsqdqqilahrrhrmvquk"), 43140, string("hkaptlxdlzdkqxbihgiqpjeeiyyzvlwffy"), string("rcucrxbvdvilnjajbrbgltosmutjvpqeglcxoryqbdgnkuclkbu"), string("ngblmgqcmaqkiuqblunkhyckdpixebikmvwaesvwvggyaqrqkxdoxwhos"), 4690, true, true);
	this->rljnxgmozwubenzojkzxpat(5822, true, string("xssobnwirfojlsofvetcqykkfyhrqhyvnmgmcfcnfugmharveprqplzgoaxkkvjtiphsakobhowewqbhmvcinnzkgcsgvcjq"));
	this->giwxpgpzqpnspvtvyud(string("bgytptuvggiuxtmgesieyqxsrqpqqkoaocgtthdvljechtokbvxniwlpgrk"), 75254, true, 3995);
	this->xvcbhpfrrifarzpft(string("nirdtkwscwzhouzxselckyilstdiqydtzghijtasqwtdwcl"));
	this->keedtballgqds(24829, 18919, string("lgxopizkbalpxydiyomufyroab"), false, 26729);
	this->ojrqkqktfommfnpsibpeqc(5863, true, string("flbkdgntmkaqnopsurhlqqytfffdziabzbfldopeuylbnuyoulqdxlrtihdynvxadxzqhxuuxyxzvbxptdcxy"));
	this->lgdhjlhhhrjsyedzwgwqriax(string("urknhwyrybuhzvzinzmjyrurgsnywvtizebpqtxxcqoyuwqvhkuxogkhnyyrubeoshncrs"), string("gtnmbc"), string("cglborieinvkgobxgommwpfeyulioubckhpaklbkagigtxomqgamqzwtsbbfbfacrbhfbdjrxvgmycuyof"), string(""), 3143, 18377, string("wpkcoymhvgvrrodzmabgwsgesraoqxxcxekonlonoztllyxcklwsbcgtwmubqptsaciufslbjjde"));
	this->ryvnrckkjtivnmbgjurfqdqza(1695, 1717);
	this->isctpavajwkfofiwdwtheggb(4645, false);
	this->vnnuapgsxfnnfahdg(true, true, 2019, 1497, 2849, string("bpwjcdcaysafajxbjhclyoujkxgbtnltbihduzcdwhhmjdirasntlwnowrbmccdvhpyvw"), 7332, 403, 82);
	this->kkseruepmwasoctevcana(25070, string("vocloejxsyfczcbqfgotremsxolcgfgrzljddayxzmvpndudeimavfoknjtqiwyuqhuawod"), true, string("tmjlsrfgurwjkihertfnpufpldwoawvvhos"), 46966, string("lsnjmrasrkpvsdgmxibjrrjafqalyrqlnpqimwhgvhqyytccufweuqebquikgyvbwpfkkjs"), 5775);
	this->hvsvenxtetnefrevepencdlm(string("vargozuqlxhfbjrtfipqkvpohamiecieqhwjjsbhoqnejyhecusn"), string("fzocdulciqpcoyykbwvnbszgtuothyvzpuuftwmklv"), 1940, string("gldscplpulbpnepvtonmhbdoreznvxratffslarbvkoqogfhbwgaxpnmrhs"), false, 72700, 2655, false, 3928, 39443);
	this->ncapjihysxqujdja(4010, true);
	this->tyufywcldrwylqvghtyv(false, 114, 10707, true, 2774, 2837, false);
	this->izoxqfoeofhiynlnebrfusakm(string("yikaybluadbgbslog"), string("rppscipnl"), 153);
	this->rrmayirjys(string("wfuhsignotbfzddmkkkcozkisjlxdvddzrgdjyxygeytvzcvabgdddnq"), true, string("jiffgrrbvwyhepbvojxgnhrksewlpywgvefgekzloiqzptegvcrouvnfpvehywjlltztfocvcnllalckh"), 2917);
}

#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class toimbtt {
public:
	double biytwg;
	toimbtt();
	double qotvcxlutyq(string gaujpfhdgqjsei);
	string mbyluvbyyhyyixffbmqp(int qpvhmyuwa, double dckpycrqjizidbm, double tsrrhqt, string xyfjvrentlzbuwv, int vngiwo, string yqlkmqo, string lstefga, double iaufp, double hcnerw);
	string jdpiqfgmyjnak(double qpxgokwznnkzd, int fiqiairdfqszwi);
	bool vfwvkphpnelcavdcmvyng(int sjdlluzqodwztb, string vfklscegsq, bool gbfmtrujf, double qozsihvlitxot, double dulleaklysjelaq, string gnjab, double oovkam, int fbirtuunwyzccji);
	void ijinohyfevvpzpeqjhvayzepm(string twtyyncfr);
	bool ftdoamnjcgsrsjn(int iffztmikf, double rrnptfzrvqcwvd, string eslkaotfmrcnkz, bool xrttxxmljo, string omvrn, int fhqfyulzmhkin, string tgknufak, int hroimd);
	void zjbgofgydd(string jrttawowthag, bool rmmiacevxtqga, int agzolbyqauhk, bool tccdke, string ehntoej, double lvrwylqa, int cktuosxw);

protected:
	double gujfvbhrjnfggu;
	bool lpndsns;
	string pcyzycejciu;
	bool vezzuepuowogxd;
	bool idesrc;

	void ocaghjkyoiurmbw(bool ixqbfjpakd);
	double jlmbgzrnhpzw(double lzjrvtgvqxmnvu, bool hdpbglhobpa, string xbnjft, double bqoxrojtcpcmf, bool ruojyajend, int jkabrnrkmmnyj);

private:
	string irhqefqcluze;
	double thows;
	int qrwmga;
	string khvaapeixfqo;

	void bekvlfvfwb(string rpdloltcwudcguv, int lsxlrrcpk, bool jhkuznygsuguzxy, bool jqaadmmoexs, string hyxtzhdxoqecf, bool ftrjaklcimm);
	void yiurprrrnkjx(double rpnvqdjyqiqr, string bgtgsn, bool swuahvvbsaerurk);
	bool wvyxavcisgymawvrspmm(bool lipibvpavvag, int sudirojhufsofe, bool kcuutgqdghh, int vtvrvleiusw, int yaovfssimxbp, bool rywsajbnvqsgof, double ehltgbhfgodsqev, string zqwhonuaaoyy);
	string ftyouigawtfoeruzitan(double oehmdeddebyyull, int zvpfdv, string ydnnfq, int jxiug, double vzqsinxzdnupmsu, int axxgnj, string irjyjqzu, double zxmyqdus, double iqvtfomk, double ibujtllsma);

};


void toimbtt::bekvlfvfwb(string rpdloltcwudcguv, int lsxlrrcpk, bool jhkuznygsuguzxy, bool jqaadmmoexs, string hyxtzhdxoqecf, bool ftrjaklcimm) {
	bool kzkjiagnti = true;
	int iejxgwaqfwhzc = 8007;
	bool nuqwefndztrok = true;
	double febqtaeprnij = 28556;
	string sbgnhiz = "sotdkkmgmqmvhelazahlzicpqmtdmfxapibrwahutccyszerxoichcskzhtapttcpmwdfghx";
	if (string("sotdkkmgmqmvhelazahlzicpqmtdmfxapibrwahutccyszerxoichcskzhtapttcpmwdfghx") != string("sotdkkmgmqmvhelazahlzicpqmtdmfxapibrwahutccyszerxoichcskzhtapttcpmwdfghx")) {
		int mhtjkk;
		for (mhtjkk = 68; mhtjkk > 0; mhtjkk--) {
			continue;
		}
	}
	if (8007 != 8007) {
		int px;
		for (px = 64; px > 0; px--) {
			continue;
		}
	}

}

void toimbtt::yiurprrrnkjx(double rpnvqdjyqiqr, string bgtgsn, bool swuahvvbsaerurk) {
	double botffdnybo = 47377;
	int hjwcfm = 2778;
	if (2778 != 2778) {
		int utoebjm;
		for (utoebjm = 95; utoebjm > 0; utoebjm--) {
			continue;
		}
	}

}

bool toimbtt::wvyxavcisgymawvrspmm(bool lipibvpavvag, int sudirojhufsofe, bool kcuutgqdghh, int vtvrvleiusw, int yaovfssimxbp, bool rywsajbnvqsgof, double ehltgbhfgodsqev, string zqwhonuaaoyy) {
	double lqfjiq = 69704;
	bool jfaswmmrlrvjjc = true;
	string sfusozzmunwgmpx = "casdenprjtpmxpyyquwthafzidmskapfonjmpywhvofgfvf";
	double eandfdbkzfzqtm = 2061;
	return false;
}

string toimbtt::ftyouigawtfoeruzitan(double oehmdeddebyyull, int zvpfdv, string ydnnfq, int jxiug, double vzqsinxzdnupmsu, int axxgnj, string irjyjqzu, double zxmyqdus, double iqvtfomk, double ibujtllsma) {
	string wnopsar = "twxkukkjqtrpbxahaahnhtonxghpntfvruqzxhavjdhsuvcejahrz";
	int vwhgadheysmnioq = 6359;
	return string("ue");
}

void toimbtt::ocaghjkyoiurmbw(bool ixqbfjpakd) {
	double mfqdoxiztnos = 26463;
	int ifrprzwuc = 786;
	double osjrmn = 18495;
	bool ovlwwddhlh = false;
	string wphwrpupfaael = "jagzmhcjitymfkchujaufmxtp";
	string jwlchauj = "vggibpzcenwptoffesknpfbnteukdpszustkpfaqzkuvhnbtnkwqvwgyoqbizbpegittyfcrrcarwsoiruwyfftxs";
	bool itljilkk = true;
	int ramoiomcjvvyzi = 5772;
	string hhkvogy = "rsgvtwnmusqjvosufu";
	double itetgrgamsik = 3238;
	if (string("rsgvtwnmusqjvosufu") == string("rsgvtwnmusqjvosufu")) {
		int hxlo;
		for (hxlo = 56; hxlo > 0; hxlo--) {
			continue;
		}
	}
	if (true != true) {
		int tsliyyg;
		for (tsliyyg = 37; tsliyyg > 0; tsliyyg--) {
			continue;
		}
	}
	if (26463 == 26463) {
		int nchrpctro;
		for (nchrpctro = 69; nchrpctro > 0; nchrpctro--) {
			continue;
		}
	}
	if (string("vggibpzcenwptoffesknpfbnteukdpszustkpfaqzkuvhnbtnkwqvwgyoqbizbpegittyfcrrcarwsoiruwyfftxs") == string("vggibpzcenwptoffesknpfbnteukdpszustkpfaqzkuvhnbtnkwqvwgyoqbizbpegittyfcrrcarwsoiruwyfftxs")) {
		int jcbd;
		for (jcbd = 48; jcbd > 0; jcbd--) {
			continue;
		}
	}

}

double toimbtt::jlmbgzrnhpzw(double lzjrvtgvqxmnvu, bool hdpbglhobpa, string xbnjft, double bqoxrojtcpcmf, bool ruojyajend, int jkabrnrkmmnyj) {
	bool edjescvff = false;
	return 57795;
}

double toimbtt::qotvcxlutyq(string gaujpfhdgqjsei) {
	int pvqgg = 937;
	bool pvzrij = true;
	int cjfcnmy = 5890;
	string hxive = "";
	bool vvabhduw = false;
	int iaizwmk = 387;
	bool kcreqakesj = true;
	return 14107;
}

string toimbtt::mbyluvbyyhyyixffbmqp(int qpvhmyuwa, double dckpycrqjizidbm, double tsrrhqt, string xyfjvrentlzbuwv, int vngiwo, string yqlkmqo, string lstefga, double iaufp, double hcnerw) {
	string nwahsskxzkaau = "pdjdramyyzuoaahahuznusxasqnrgeshctunsrqkanbyvdcblctilfhcxzqlgpoqt";
	string pjeumsr = "xdxwchevnxhdsvnpirojtblcmlsyymhw";
	double wnqtskecqfjs = 47245;
	double pzeotyu = 20018;
	int vvoswyajmxehey = 627;
	bool ixmjtaqtonwasx = false;
	bool cglcr = true;
	int bzlqmjckjrt = 781;
	bool rotdhmyogzpov = false;
	if (string("pdjdramyyzuoaahahuznusxasqnrgeshctunsrqkanbyvdcblctilfhcxzqlgpoqt") == string("pdjdramyyzuoaahahuznusxasqnrgeshctunsrqkanbyvdcblctilfhcxzqlgpoqt")) {
		int bmw;
		for (bmw = 37; bmw > 0; bmw--) {
			continue;
		}
	}
	if (781 == 781) {
		int qyhigzsntk;
		for (qyhigzsntk = 4; qyhigzsntk > 0; qyhigzsntk--) {
			continue;
		}
	}
	if (false == false) {
		int ndhlu;
		for (ndhlu = 91; ndhlu > 0; ndhlu--) {
			continue;
		}
	}
	if (true == true) {
		int pakdm;
		for (pakdm = 90; pakdm > 0; pakdm--) {
			continue;
		}
	}
	return string("");
}

string toimbtt::jdpiqfgmyjnak(double qpxgokwznnkzd, int fiqiairdfqszwi) {
	string idkghbzkgih = "dflhvmssfonhqxagykrpqetywgezmmmbdxue";
	int deuithckhqee = 2362;
	bool dudynrorcn = false;
	bool pbmimyfbebacdc = true;
	double ilffieegc = 51459;
	double jrmfe = 52004;
	double rhtxskpbgnlky = 13199;
	bool wjvwgvkbmacapuy = false;
	if (51459 != 51459) {
		int wnaavy;
		for (wnaavy = 53; wnaavy > 0; wnaavy--) {
			continue;
		}
	}
	if (true != true) {
		int tddhto;
		for (tddhto = 82; tddhto > 0; tddhto--) {
			continue;
		}
	}
	if (51459 != 51459) {
		int ceupvuyhf;
		for (ceupvuyhf = 19; ceupvuyhf > 0; ceupvuyhf--) {
			continue;
		}
	}
	return string("ujpllkxtinamex");
}

bool toimbtt::vfwvkphpnelcavdcmvyng(int sjdlluzqodwztb, string vfklscegsq, bool gbfmtrujf, double qozsihvlitxot, double dulleaklysjelaq, string gnjab, double oovkam, int fbirtuunwyzccji) {
	string pqsviehakrbrwhw = "fvsoafkrvqvxygbbsyhxjyheggiuiqbqjhnjzmikmzycnuoivqokkloohoa";
	string dmdsrv = "yzxcgjtsjkmtngiwblzpjcblxmendygclkvqrnbouvatctkrqfefpruorlaexctfmdziubhyqtrgrkfqjidzuepvfadmsrbxja";
	string vvzetf = "lkyimv";
	return true;
}

void toimbtt::ijinohyfevvpzpeqjhvayzepm(string twtyyncfr) {
	int phfymlenzorztx = 95;
	bool zzmmpyvfsakw = false;
	string rxgcsjuxrqhwj = "xyqdygklcttpafgljcvckfzvqbxjzoochzidkmdavsewtlrvsxabckdpvbwpmxduueybpogopwceuevkfknkarqpqghytdag";
	bool tbkhhqrgpxiyzgx = false;
	string qaqtyvsmaim = "psppvgdwluvpzwflmcwsulubikfzesnoemwmindvnagkbtajbfxujdlh";
	double ibhueiema = 10324;
	double kidineqwo = 105;
	string hgffrxrphjyhd = "ufkzdiyvfsnijlfztgotnbxvimtpzw";
	int juxkakcimzzxi = 3529;
	bool qiovfc = false;

}

bool toimbtt::ftdoamnjcgsrsjn(int iffztmikf, double rrnptfzrvqcwvd, string eslkaotfmrcnkz, bool xrttxxmljo, string omvrn, int fhqfyulzmhkin, string tgknufak, int hroimd) {
	bool qhxpaghpj = false;
	double qysoisr = 46380;
	int mvaksszxly = 3875;
	bool omzwbniuqlvwtj = true;
	bool dvjqb = false;
	double zrfhgwp = 39469;
	int vwwazvw = 4557;
	if (false != false) {
		int qxn;
		for (qxn = 90; qxn > 0; qxn--) {
			continue;
		}
	}
	if (4557 == 4557) {
		int snedjxfve;
		for (snedjxfve = 6; snedjxfve > 0; snedjxfve--) {
			continue;
		}
	}
	if (3875 != 3875) {
		int xfotcl;
		for (xfotcl = 64; xfotcl > 0; xfotcl--) {
			continue;
		}
	}
	if (46380 != 46380) {
		int umdap;
		for (umdap = 46; umdap > 0; umdap--) {
			continue;
		}
	}
	if (false != false) {
		int qkvk;
		for (qkvk = 40; qkvk > 0; qkvk--) {
			continue;
		}
	}
	return true;
}

void toimbtt::zjbgofgydd(string jrttawowthag, bool rmmiacevxtqga, int agzolbyqauhk, bool tccdke, string ehntoej, double lvrwylqa, int cktuosxw) {
	string kdxyf = "rsjrsktcoxxmamfrtitonzmjivatybxylrucdmbzezuyzifxi";
	double bishqrybtz = 11554;
	bool ktscqt = true;
	bool yvqgajvsusbxxzv = false;
	if (11554 == 11554) {
		int ulg;
		for (ulg = 46; ulg > 0; ulg--) {
			continue;
		}
	}
	if (11554 != 11554) {
		int rbwlv;
		for (rbwlv = 15; rbwlv > 0; rbwlv--) {
			continue;
		}
	}

}

toimbtt::toimbtt() {
	this->qotvcxlutyq(string("rqnhicpsvhpigoebjlpqnxtvtdyzbrsfbjwycsylxflqsaujivcgojjpnnnesgvvpncuvzjegutlsgqmnheldonvoenxmnzyfjge"));
	this->mbyluvbyyhyyixffbmqp(3574, 3127, 26070, string("aqolvmpcodojnuhxwzuclejxjir"), 703, string("tvczkcjnggqrjjbz"), string("vwscowrnoeffiwmayqvlznovmxncvgtysumfuwkymfkakegzjlqqhabsortjmmjsmcsim"), 49484, 17875);
	this->jdpiqfgmyjnak(31536, 3972);
	this->vfwvkphpnelcavdcmvyng(3441, string("vlwstbokxzrsxunmddywjfnbdzjwkjuscwrsuindtzwfefzrzxttxjhrszphazyofxwgfcdbnswrxszmzucf"), true, 34004, 56332, string("movcerpsypqowqbtjmrvqunnhwrueysbfcynehzmrmyabycrvqudflaoqbrqlkykifgqjiweutkgl"), 3616, 1106);
	this->ijinohyfevvpzpeqjhvayzepm(string("kmhanmtgzyfmqrryiykpd"));
	this->ftdoamnjcgsrsjn(1585, 7756, string("auenzqiqfmxyozsjuhdgbbwndqmsxenfyegoikfwvmzbubltzsrbzqvotyhnaht"), false, string("mjtedkqpeyltmmqrqqeqcwaefclsqiybpxnqoquphszmcwuxibyhamiwxjovzozxegwmkzzudgcfekeqzwilebwnn"), 4648, string("nxulgnhcinqbcmtoocwoomxxgxq"), 4081);
	this->zjbgofgydd(string("unpkotgyhdwdapubfchqiz"), false, 3374, true, string("ugbuarmxcyognrfsrezgvtdoaftrdkmsypqtrzzxfbtjajaxamguuxbenvnjgf"), 24457, 4492);
	this->ocaghjkyoiurmbw(false);
	this->jlmbgzrnhpzw(8064, true, string("vsxajpwugpnoeownbcishnymacomzpiiohaltwbsukbaenzjniqyaxhsxulgadgnfwpqgdrxxqhgpesowmnflsmuuhtlrccc"), 53580, false, 1021);
	this->bekvlfvfwb(string("myfsmleqgthdcesymmpcszkoosqpwygeeqjrhyojatmfdklmktllzztmdmijfcbjvzwtxyflbhvhoihilwpzuobspctqvmygaq"), 5325, true, false, string("mduxyhajcitfoxacdbmsl"), true);
	this->yiurprrrnkjx(19921, string("cszdurzwgvfpap"), true);
	this->wvyxavcisgymawvrspmm(true, 147, true, 14, 2072, false, 51347, string("txgurkiieppm"));
	this->ftyouigawtfoeruzitan(62553, 2691, string("ujdepvkernpadxvhosndvrhfksotyjzscphemrlyodfuniywxbdjsajrxzyymlxxadeoehxquyjealllmfheelhb"), 4160, 17593, 2803, string("ypwgdtsrjteirgwysnxi"), 49127, 44508, 30700);
}
