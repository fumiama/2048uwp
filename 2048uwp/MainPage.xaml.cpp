//
// MainPage.xaml.cpp
// MainPage 类的实现。
//

#include "pch.h"
#include "MainPage.xaml.h"
using namespace Windows::Storage;
using namespace _2048uwp;
using namespace Concurrency;
using namespace Platform;
using namespace Windows::Foundation;
using namespace Windows::Foundation::Collections;
using namespace Windows::UI::Popups;
using namespace Windows::UI::ViewManagement;
using namespace Windows::UI::Xaml;
using namespace Windows::UI::Xaml::Controls;
using namespace Windows::UI::Xaml::Controls::Primitives;
using namespace Windows::UI::Xaml::Data;
using namespace Windows::UI::Xaml::Input;
using namespace Windows::UI::Xaml::Media;
using namespace Windows::UI::Xaml::Navigation;

#define C8192				Windows::UI::ColorHelper::FromArgb(0xbb, 0x36, 0x91, 0xff)
#define C4096				Windows::UI::ColorHelper::FromArgb(0xbb, 0x8e, 0xd2, 0xff)
#define C2048				Windows::UI::ColorHelper::FromArgb(0xbb, 0xeb, 0xc7, 0x03)
#define C1024				Windows::UI::ColorHelper::FromArgb(0xbb, 0xe3, 0xb9, 0x15)
#define C512				Windows::UI::ColorHelper::FromArgb(0xbb, 0xec, 0xc8, 0x50)
#define C256				Windows::UI::ColorHelper::FromArgb(0xbb, 0xed, 0xcc, 0x61)
#define C128				 Windows::UI::ColorHelper::FromArgb(0xbb, 0xed, 0xce, 0x71)
#define C64					Windows::UI::ColorHelper::FromArgb(0xbb, 0xf6, 0x5d, 0x3b)
#define C32					Windows::UI::ColorHelper::FromArgb(0xbb, 0xf5, 0x7c, 0x5f)
#define C16					Windows::UI::ColorHelper::FromArgb(0xbb, 0xf5, 0x95, 0x63)
#define C8					 Windows::UI::ColorHelper::FromArgb(0xbb, 0xf2, 0xb1, 0x79)
#define C4					Windows::UI::ColorHelper::FromArgb(0xdd, 0xec, 0xe0, 0xc8)
#define C2					Windows::UI::ColorHelper::FromArgb(0xdd, 0xee, 0xe4, 0xda)
#define C0					Windows::UI::ColorHelper::FromArgb(0x55, 0xff, 0xff, 0xff)
#define DGRAY			Windows::UI::ColorHelper::FromArgb(0xff, 0x55, 0x55, 0x55)
#define WTE				Windows::UI::ColorHelper::FromArgb(0xff, 0xff, 0xff, 0xff)

#define COLOR8192       colors[15]
#define COLOR4096       colors[14]
#define COLOR2048       colors[11]
#define COLOR1024       colors[10]
#define COLOR512         colors[9]
#define COLOR256         colors[8]
#define COLOR128         colors[7]
#define COLOR64           colors[6]
#define COLOR32           colors[5]
#define COLOR16           colors[4]
#define COLOR8             colors[3]
#define COLOR4             colors[2]
#define COLOR2             colors[1]
#define COLOR0				colors[0]
#define DARKGRAY			colors[12]
#define WHITE				colors[13]

#define setbtnsz(x) x->Width = buttonsz; x->Height = buttonsz
#define setbtncolor(btn, color) btn->Background = color
#define setbtntxt(x, t) x->Content = t
#define setbtntxtsz(x) x->FontSize = buttonsz / 3
#define setbtntxtcolor(x, c) x->Foreground = c

static Brush^ colors[16];

static short arr[6][6];

#define CH			arr[0][0]
#define XPOS		arr[0][1]
#define YPOS		arr[0][2]
#define AXPOS	arr[0][3]
#define AYPOS	arr[0][4]
#define FLAG		arr[0][5]

#define FLAG2		arr[5][0]
#define FLAG3		arr[5][1]
#define FLAG4		arr[5][2]
#define ISFAIL		arr[5][3]
#define ISWIN		arr[5][4]
#define NOMOV	arr[5][5]

#define SCORE	arr[1][0]
#define MUMOV	arr[2][0]
#define MVTAG	arr[3][0]
#define SINGLE	arr[4][0]

#define INIT			arr[1][5]

#define DX			e->Delta.Translation.X
#define DY			e->Delta.Translation.Y

static short arr2[6][6];
#define savestage() memcpy(arr2, arr, sizeof(arr))
#define savestagefromtmp() memcpy(arr2, arrtmp, sizeof(arr))
#define merge(x, y, nomov) FLAG3 = !x(); FLAG4 = !y(); nomov = FLAG3 && FLAG4

static short arrtmp[6][6];
#define savetmpstage() memcpy(arrtmp, arr, sizeof(arr))

static Button^ btngrp[4][4];
#define drawb(color) setbtncolor(btngrp[i][j], color)
#define drawt(color, text) setbtntxt(btngrp[i][j], text); setbtntxtcolor(btngrp[i][j], color)

static FILE fbuf;
static String^ path;
static const wchar_t* filepath;
#define localpath ApplicationData::Current->LocalFolder->Path
#define getfilepath() path = localpath; path += "\\savedata"; filepath = path->Begin()

#define loadBlk2Stage() drawblocks();printmyscore();havempty();judgeWin()

static wchar_t buff[1024];
static float rbh = 0;
static ContentDialog^ message;
static ContentDialog^ refreshGame;

MainPage::MainPage()
{
	InitializeComponent();
	INIT = 0;
	brushinit();
	btninit();
	getfilepath();
	gamedisplay();
}
void _2048uwp::MainPage::stage_SizeChanged(Platform::Object^ sender, Windows::UI::Xaml::SizeChangedEventArgs^ e)
{
	float buttonsz = (e->NewSize.Width - 50.0) / 4.0;
	float rate = pow((float)buttonsz / (float)e->NewSize.Width - 1.0, 3);

	if (rate > 1.0) rate = 1.0;
	bs->Width = buttonsz * 2;
	bs->Height = buttonsz * (asin(rate) / 3.14 * 2.2 + 1.0);
}
void _2048uwp::MainPage::Page_SizeChanged(Platform::Object^ sender, Windows::UI::Xaml::SizeChangedEventArgs^ e)
{
	float rate = pow((float)e->NewSize.Height / (float)e->NewSize.Width - 1.0, 3);

	if (rate > 1.0) rate = 1.0;
	rbase->Height = rbh =  127 * (asin(rate) / 3.14 * 2 + 1.0);
	float newsize = ((e->NewSize.Width > e->NewSize.Height) ? e->NewSize.Height : e->NewSize.Width) - 128;
	float buttonsz = (newsize - 50.0) / 4.0;

	stage->Width = newsize;
	stage->Height = newsize;
	setbtnsz(b11); setbtnsz(b12); setbtnsz(b13); setbtnsz(b14);
	setbtnsz(b21); setbtnsz(b22); setbtnsz(b23); setbtnsz(b24);
	setbtnsz(b31); setbtnsz(b32); setbtnsz(b33); setbtnsz(b34);
	setbtnsz(b41); setbtnsz(b42); setbtnsz(b43); setbtnsz(b44);
	setbtntxtsz(b11); setbtntxtsz(b12); setbtntxtsz(b13); setbtntxtsz(b14);
	setbtntxtsz(b21); setbtntxtsz(b22); setbtntxtsz(b23); setbtntxtsz(b24);
	setbtntxtsz(b31); setbtntxtsz(b32); setbtntxtsz(b33); setbtntxtsz(b34);
	setbtntxtsz(b41); setbtntxtsz(b42); setbtntxtsz(b43); setbtntxtsz(b44);
	setbtntxtsz(bs);
	r2->Width = 10 + buttonsz;
	r1->Width = 30 + 2 * buttonsz;
	c2->Height = 10 + buttonsz;
	c1->Height = 30 + 2 * buttonsz;
	bbk->Height = buttonsz / 2.5; bbk->Width = buttonsz / 2.5;
	bre->Height = buttonsz / 2.5; bre->Width = buttonsz / 2.5;
	bbk->FontSize = buttonsz / 6; bre->FontSize = buttonsz / 6;
}

void _2048uwp::MainPage::goBack() {
	if (INIT) {
		memcpy(arr, arr2, sizeof(arr)); loadBlk2Stage(); save2file();
	}
}

void _2048uwp::MainPage::bbk_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
	goBack();
}

void _2048uwp::MainPage::bre_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
	if (!refreshGame) {
		refreshGame = ref new ContentDialog;
		refreshGame->Title = "重置游戏";
		refreshGame->Content = "您将失去现有进度";
		refreshGame->PrimaryButtonText = "重置";
		refreshGame->CloseButtonText = "取消";
		refreshGame->DefaultButton = ContentDialogButton::Close;
	}
	IAsyncOperation<ContentDialogResult>^ refreshTask = refreshGame->ShowAsync();
	create_task(refreshTask).then([this](ContentDialogResult result) {
		if (result == ContentDialogResult::Primary){
			_wremove(filepath); gamedisplay();
		}
	});
}
void _2048uwp::MainPage::bs_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
	//if (!message) {
		wchar_t* buf = buff;
		char c = 0;

		memset(buff, 0, sizeof(buff));
		swprintf_s(buf, 1024, L"方文社©2020 by 源文雨.\n\tNow\n");
		for (int i = 0; i <= 5; i++) {
			for (int j = 0; j <= 5; j++) {
				swprintf_s(buf + wcslen(buf), 1024 - wcslen(buf), L"%5d ", arr[i][j]);
				if (arr[i][j]) c++;
			}
			swprintf_s(buf + wcslen(buf), 1024 - wcslen(buf), L"\n");
		}
		swprintf_s(buf + wcslen(buf), 1024 - wcslen(buf), L"\tPrev\n");
		for (int i = 0; i <= 5; i++) {
			for (int j = 0; j <= 5; j++) {
				swprintf_s(buf + wcslen(buf), 1024 - wcslen(buf), L"%5d ", arr2[i][j]);
				if (arr2[i][j]) c++;
			}
			swprintf_s(buf + wcslen(buf), 1024 - wcslen(buf), L"\n");
		}
		swprintf_s(buf + wcslen(buf), 1024 - wcslen(buf), L"Manipulation: (%d, %d). ", XPOS, YPOS);
		swprintf_s(buf + wcslen(buf), 1024 - wcslen(buf), L"Win status: %d. Fail status: %d.\n", ISWIN, ISFAIL);
		swprintf_s(buf + wcslen(buf), 1024 - wcslen(buf), L"Background color is #%02X%02X%02X%02X, so the system is in ", UISettings().GetColorValue(UIColorType::Background).A, UISettings().GetColorValue(UIColorType::Background).R, UISettings().GetColorValue(UIColorType::Background).G, UISettings().GetColorValue(UIColorType::Background).B);
		swprintf_s(buf + wcslen(buf), 1024 - wcslen(buf), L"%s mode.\n", isbglight() ? L"light" : L"dark");
		if (message) delete message;
		message = ref new ContentDialog;
		message->Title = "Info";
		message->Content = ref new String(buf);
		message->PrimaryButtonText = "确定";
	//}
	message->ShowAsync();
}

#define manageBlks(nomov) {\
	if (!nomov) {\
		INIT = 1;\
		if(havempty()) newblock();\
		loadBlk2Stage();\
		savestagefromtmp();\
		save2file();\
	}\
}

void _2048uwp::MainPage::bpage_KeyUp(Platform::Object^ sender, Windows::UI::Xaml::Input::KeyRoutedEventArgs^ e)
{
	NOMOV = 1;

	switch (e->Key.ToString()->Begin()[0]) {
	case 'L': savetmpstage(); merge(leftmove, leftadd, NOMOV);  break;
	case 'R':  savetmpstage(); merge(rightmove, rightadd, NOMOV); break;
	case 'U':  savetmpstage(); merge(upmove, upadd, NOMOV); break;
	case 'D':  savetmpstage(); merge(downmove, downadd, NOMOV); break;
	case 'B': goBack(); break;
	case 'E': Windows::ApplicationModel::Core::CoreApplication::Exit();
	default: return; break;
	}
	manageBlks(NOMOV);
}
void _2048uwp::MainPage::basegrid_ManipulationCompleted(Platform::Object^ sender, Windows::UI::Xaml::Input::ManipulationCompletedRoutedEventArgs^ e)
{
	MVTAG = 0;
	rbase->Height = rbh;
	cbase1->Width = GridLength((double)241, GridUnitType::Star);
	cbase2->Width = GridLength((double)25, GridUnitType::Star);
}
void _2048uwp::MainPage::bpage_RightTapped(Platform::Object^ sender, Windows::UI::Xaml::Input::RightTappedRoutedEventArgs^ e)
{
	if (CH) { CH = 0; basegrid->Opacity = 0.7; }
	else { CH = 1; basegrid->Opacity = 1.0; }
	save2file();
}
void _2048uwp::MainPage::basegrid_ManipulationDelta(Platform::Object^ sender, Windows::UI::Xaml::Input::ManipulationDeltaRoutedEventArgs^ e)
{
	if (MVTAG) {
		rbase->Height = rbh;
		cbase1->Width = GridLength((double)241, GridUnitType::Star);
		cbase2->Width = GridLength((double)25, GridUnitType::Star);
		return;
	}
	rbase->Height = abs(rbh + DY / 16);
	cbase1->Width = GridLength((double)abs(241 - DX / 16), GridUnitType::Star);
	cbase2->Width = GridLength((double)abs(25 - DX / 16), GridUnitType::Star);
	MUMOV = 1;

	AXPOS = abs(e->Cumulative.Translation.X);
	AYPOS = abs(e->Cumulative.Translation.Y);
	XPOS = e->Cumulative.Translation.X;
	YPOS = e->Cumulative.Translation.Y;
	if (abs(AXPOS - AYPOS) < 100) return;
	else {
		savetmpstage();
		MVTAG = 1;
		if (AXPOS > AYPOS) {
			if (XPOS < 0) { merge(leftmove, leftadd, MUMOV); }
			else { merge(rightmove, rightadd, MUMOV); }
		}
		else if (YPOS > 0) { merge(downmove, downadd, MUMOV); }
		else { merge(upmove, upadd, MUMOV); }
		manageBlks(MUMOV);
	}
}
void _2048uwp::MainPage::bhide_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
	if (CH++ > 6) { arr[3][4] = 2048; CH = 0; }
}

void _2048uwp::MainPage::btninit() {
	btngrp[0][0] = b11; btngrp[0][1] = b12; btngrp[0][2] = b13; btngrp[0][3] = b14;
	btngrp[1][0] = b21; btngrp[1][1] = b22; btngrp[1][2] = b23; btngrp[1][3] = b24;
	btngrp[2][0] = b31; btngrp[2][1] = b32; btngrp[2][2] = b33; btngrp[2][3] = b34;
	btngrp[3][0] = b41; btngrp[3][1] = b42; btngrp[3][2] = b43; btngrp[3][3] = b44;
}
void _2048uwp::MainPage::drawblocks() {
	ISWIN = 0;
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			switch (arr[i + 1][j + 1]) {
			case 0:
				drawb(COLOR0);
				drawt(DARKGRAY, "");
				break;
			case 2:
				drawb(COLOR2);
				drawt(DARKGRAY, "2");
				break;
			case 4:
				drawb(COLOR4);
				drawt(DARKGRAY, "4");
				break;
			case 8:
				drawb(COLOR8);
				drawt(WHITE, "8");
				break;
			case 16:
				drawb(COLOR16);
				drawt(WHITE, "16");
				break;
			case 32:
				drawb(COLOR32);
				drawt(WHITE, "32");
				break;
			case 64:
				drawb(COLOR64);
				drawt(WHITE, "64");
				break;
			case 128:
				drawb(COLOR128);
				drawt(WHITE, "128");
				break;
			case 256:
				drawb(COLOR256);
				drawt(WHITE, "256");
				break;
			case 512:
				drawb(COLOR512);
				drawt(WHITE, "512");
				break;
			case 1024:
				drawb(COLOR1024);
				drawt(WHITE, "1024");
				break;
			case 2048:
				drawb(COLOR2048);
				drawt(WHITE, "2048");
				ISWIN = 1;
				break;
			case 4096:
				drawb(COLOR4096);
				drawt(WHITE, "4096");
				ISWIN = 1;
				break;
			case 8192:
				drawb(COLOR8192);
				drawt(WHITE, "8192");
				ISWIN = 1;
				break;
			default:
				drawb(COLOR8192);
				drawt(WHITE, arr[i + 1][j + 1]);
				ISWIN = 1;
				break;
			}
		}
	}
}
void _2048uwp::MainPage::gamedisplay() {
	srand(time(0));
	memset(arr, 0, sizeof(arr));
	memset(arr2, 0, sizeof(arr));
	CH = -1;
	if (loadfromfile() || !activeblocks()) newblock();
	if (!~CH) {
		if ((CH = isbglight())) basegrid->Opacity = 1.0;
		else basegrid->Opacity = 0.7;
	}
	loadBlk2Stage();
}
int _2048uwp::MainPage::judgeWin() {
	if (ISWIN) bs->Background = COLOR2048;
	else if (ISFAIL) bs->Background = COLOR64;
	else bs->Background = COLOR2;
}
void _2048uwp::MainPage::printmyscore() {
	bs->Content = calScore(arr);
}

int MainPage::activeblocks() {
	char c = 0;
	for (int i = 1; i <= 4; i++) {
		for (int j = 1; j <= 4; j++) {
			if (arr[i][j]) c++;
		}
	}
	return c;
}
void MainPage::brushinit() {
	colors[0] = ref new SolidColorBrush(C0);
	colors[1] = ref new SolidColorBrush(C2);
	colors[2] = ref new SolidColorBrush(C4);
	colors[3] = ref new SolidColorBrush(C8);
	colors[4] = ref new SolidColorBrush(C16);
	colors[5] = ref new SolidColorBrush(C32);
	colors[6] = ref new SolidColorBrush(C64);
	colors[7] = ref new SolidColorBrush(C128);
	colors[8] = ref new SolidColorBrush(C256);
	colors[9] = ref new SolidColorBrush(C512);
	colors[10] = ref new SolidColorBrush(C1024);
	colors[11] = ref new SolidColorBrush(C2048);
	colors[12] = ref new SolidColorBrush(DGRAY);
	colors[13] = ref new SolidColorBrush(WTE);
	colors[14] = ref new SolidColorBrush(C4096);
	colors[15] = ref new SolidColorBrush(C8192);
}
short MainPage::calScore(short arr[6][6]) {
	SCORE = 0;
	for (int i = 1; i <= 4; i++) {
		for (int j = 1; j <= 4; j++) {
			SCORE += arr[i][j];
		}
	}
	return SCORE % 9999 + 1;
}
int MainPage::downadd() {
	FLAG = 0;
	for (int j = 1; j <= 4; j++)
		switch ((arr[1][j] ? 8 : 0) + (arr[2][j] ? 4 : 0) + (arr[3][j] ? 2 : 0) + (arr[4][j] ? 1 : 0)) {
		case 0: case 1: break;
		case 3:
			if (arr[3][j] == arr[4][j]) {
				arr[4][j] *= 2;
				FLAG++;
				arr[3][j] = 0;
			}
			break;
		case 7:
			if (arr[3][j] == arr[4][j]) {
				arr[4][j] *= 2;
				FLAG++;
				arr[3][j] = arr[2][j];
				arr[2][j] = 0;
			}
			else if (arr[2][j] == arr[3][j]) {
				arr[3][j] *= 2;
				FLAG++;
				arr[2][j] = 0;
			}
			break;
		case 15:
			if (arr[3][j] == arr[4][j]) {
				arr[4][j] *= 2;
				FLAG++;
				arr[3][j] = 0;
			}
			else if (arr[2][j] == arr[3][j]) {
				arr[3][j] *= 2;
				FLAG++;
				arr[2][j] = 0;
			}
			if (arr[1][j] == arr[2][j]) {
				arr[2][j] *= 2;
				FLAG++;
				arr[1][j] = 0;
			}
			downmove();
			break;
		}
	return FLAG;
}
int MainPage::downmove() {
	FLAG2 = 0;
	for (int j = 1; j <= 4; j++)
		switch ((arr[1][j] ? 8 : 0) + (arr[2][j] ? 4 : 0) + (arr[3][j] ? 2 : 0) + (arr[4][j] ? 1 : 0)) {
		case 0: case 1: case 3: case 7: case 15: FLAG2++; break;
		case 2: arr[4][j] = arr[3][j]; arr[3][j] = 0; break;
		case 4: arr[4][j] = arr[2][j]; arr[2][j] = 0; break;
		case 5: arr[3][j] = arr[2][j]; arr[2][j] = 0; break;
		case 6: arr[4][j] = arr[3][j]; arr[3][j] = arr[2][j]; arr[2][j] = 0; break;
		case 8: arr[4][j] = arr[1][j]; arr[1][j] = 0; break;
		case 9: arr[3][j] = arr[1][j]; arr[1][j] = 0; break;
		case 10: arr[4][j] = arr[3][j]; arr[3][j] = arr[1][j]; arr[1][j] = 0; break;
		case 11: arr[2][j] = arr[1][j]; arr[1][j] = 0; break;
		case 12: arr[4][j] = arr[2][j]; arr[3][j] = arr[1][j]; arr[1][j] = arr[2][j] = 0; break;
		case 13: arr[3][j] = arr[2][j]; arr[2][j] = arr[1][j]; arr[1][j] = 0; break;
		case 14: arr[4][j] = arr[3][j]; arr[3][j] = arr[2][j]; arr[2][j] = arr[1][j]; arr[1][j] = 0; break;
		default: break;
		}
	return (FLAG2 - 4);
}
int MainPage::havempty() {
	SINGLE = 0;
	char nonZeroCnt = 0;
	ISFAIL = 0;
	for (int i = 1; i <= 4; i++) for (int j = 1; j <= 4; j++) {
		if (arr[i][j]) {
			nonZeroCnt++;
			if (issingle(i, j)) SINGLE++;
		}
	}
	ISFAIL = SINGLE == 16;
	return nonZeroCnt < 16;
}
int MainPage::isbglight() {
	return (0.299 * UISettings().GetColorValue(UIColorType::Background).R +
		0.587 * UISettings().GetColorValue(UIColorType::Background).G +
		0.114 * UISettings().GetColorValue(UIColorType::Background).B) / 255.0 > 0.5;
}
int MainPage::issingle(short i, short j) {
	short cmp, l, r, t, b;
	cmp = arr[i][j];
	l = i - 1;	r = i + 1;	t = j - 1;	b = j + 1;
	return !(
		(l >= 1 && cmp == arr[l][j])
		|| (r <= 4 && cmp == arr[r][j])
		|| (t >= 1 && cmp == arr[i][t])
		|| (b <= 4 && cmp == arr[i][b])
		);
}
int MainPage::leftadd() {
	FLAG = 0;
	for (int j = 1; j <= 4; j++)
		switch ((arr[j][1] ? 8 : 0) + (arr[j][2] ? 4 : 0) + (arr[j][3] ? 2 : 0) + (arr[j][4] ? 1 : 0)) {
		case 0: case 8: break;
		case 12:
			if (arr[j][1] == arr[j][2]) {
				arr[j][1] *= 2;
				FLAG++;
				arr[j][2] = 0;
			}
			break;
		case 14:
			if (arr[j][1] == arr[j][2]) {
				arr[j][1] *= 2;
				FLAG++;
				arr[j][2] = arr[j][3];
				arr[j][3] = 0;
			}
			else if (arr[j][2] == arr[j][3]) {
				arr[j][2] *= 2;
				FLAG++;
				arr[j][3] = 0;
			}
			break;
		case 15:
			if (arr[j][1] == arr[j][2]) {
				arr[j][1] *= 2;
				FLAG++;
				arr[j][2] = 0;
			}
			else if (arr[j][2] == arr[j][3]) {
				arr[j][2] *= 2;
				FLAG++;
				arr[j][3] = 0;
			}
			if (arr[j][3] == arr[j][4]) {
				arr[j][3] *= 2;
				FLAG++;
				arr[j][4] = 0;
			}
			leftmove();
			break;
		}
	return FLAG;
}
int MainPage::leftmove() {
	FLAG2 = 0;
	for (int j = 1; j <= 4; j++)
		switch ((arr[j][1] ? 8 : 0) + (arr[j][2] ? 4 : 0) + (arr[j][3] ? 2 : 0) + (arr[j][4] ? 1 : 0)) {
		case 0: case 8: case 12: case 14: case 15: FLAG2++; break;
		case 1: arr[j][1] = arr[j][4]; arr[j][4] = 0; break;
		case 2: arr[j][1] = arr[j][3]; arr[j][3] = 0; break;
		case 3: arr[j][1] = arr[j][3]; arr[j][2] = arr[j][4]; arr[j][3] = arr[j][4] = 0; break;
		case 4: arr[j][1] = arr[j][2]; arr[j][2] = 0; break;
		case 5: arr[j][1] = arr[j][2]; arr[j][2] = arr[j][4]; arr[j][4] = 0; break;
		case 6: arr[j][1] = arr[j][2]; arr[j][2] = arr[j][3]; arr[j][3] = 0; break;
		case 7: arr[j][1] = arr[j][2]; arr[j][2] = arr[j][3]; arr[j][3] = arr[j][4]; arr[j][4] = 0; break;
		case 9: arr[j][2] = arr[j][4]; arr[j][4] = 0; break;
		case 10: arr[j][2] = arr[j][3]; arr[j][3] = 0; break;
		case 11: arr[j][2] = arr[j][3]; arr[j][3] = arr[j][4]; arr[j][4] = 0; break;
		case 13: arr[j][3] = arr[j][4]; arr[j][4] = 0; break;
		default: break;
		}
	return (FLAG2 - 4);
}
errno_t MainPage::loadfromfile() {
	FILE* fp = &fbuf;
	errno_t e = _wfopen_s(&fp, filepath, L"rb");
	if (!e) {
		fread(arr, sizeof(short), 36, fp);
		fread(arr2, sizeof(short), 36, fp);
		fclose(fp);
	}
	return e;
}
void MainPage::newblock() {
	int x = rand() % 4 + 1;
	int y = rand() % 4 + 1;
	while (arr[x][y]) {
		x = rand() % 4 + 1;
		y = rand() % 4 + 1;
	}
	if (!(rand() % 10)) arr[x][y] = 4;
	else arr[x][y] = 2;
}
int MainPage::rightadd() {
	FLAG = 0;
	for (int j = 1; j <= 4; j++)
		switch ((arr[j][1] ? 8 : 0) + (arr[j][2] ? 4 : 0) + (arr[j][3] ? 2 : 0) + (arr[j][4] ? 1 : 0)) {
		case 0: case 1: break;
		case 3:
			if (arr[j][3] == arr[j][4]) {
				arr[j][4] *= 2;
				FLAG++;
				arr[j][3] = 0;
			}
			break;
		case 7:
			if (arr[j][3] == arr[j][4]) {
				arr[j][4] *= 2;
				FLAG++;
				arr[j][3] = arr[j][2];
				arr[j][2] = 0;
			}
			else if (arr[j][2] == arr[j][3]) {
				arr[j][3] *= 2;
				FLAG++;
				arr[j][2] = 0;
			}
			//else FLAG++;
			break;
		case 15:
			if (arr[j][3] == arr[j][4]) {
				arr[j][4] *= 2;
				FLAG++;
				arr[j][3] = 0;
			}
			else if (arr[j][2] == arr[j][3]) {
				arr[j][3] *= 2;
				FLAG++;
				arr[j][2] = 0;
			}
			if (arr[j][1] == arr[j][2]) {
				arr[j][2] *= 2;
				FLAG++;
				arr[j][1] = 0;
			}
			rightmove();
			break;
		}
	return FLAG;
}
int MainPage::rightmove() {
	FLAG2 = 0;
	for (int j = 1; j <= 4; j++)
		switch ((arr[j][1] ? 8 : 0) + (arr[j][2] ? 4 : 0) + (arr[j][3] ? 2 : 0) + (arr[j][4] ? 1 : 0)) {
		case 0: case 1: case 3: case 7: case 15: FLAG2++; break;
		case 2: arr[j][4] = arr[j][3]; arr[j][3] = 0; break;
		case 4: arr[j][4] = arr[j][2]; arr[j][2] = 0; break;
		case 5: arr[j][3] = arr[j][2]; arr[j][2] = 0; break;
		case 6: arr[j][4] = arr[j][3]; arr[j][3] = arr[j][2]; arr[j][2] = 0; break;
		case 8: arr[j][4] = arr[j][1]; arr[j][1] = 0; break;
		case 9: arr[j][3] = arr[j][1]; arr[j][1] = 0; break;
		case 10: arr[j][4] = arr[j][3]; arr[j][3] = arr[j][1]; arr[j][1] = 0; break;
		case 11: arr[j][2] = arr[j][1]; arr[j][1] = 0; break;
		case 12: arr[j][4] = arr[j][2]; arr[j][3] = arr[j][1]; arr[j][1] = arr[j][2] = 0; break;
		case 13: arr[j][3] = arr[j][2]; arr[j][2] = arr[j][1]; arr[j][1] = 0; break;
		case 14: arr[j][4] = arr[j][3]; arr[j][3] = arr[j][2]; arr[j][2] = arr[j][1]; arr[j][1] = 0; break;
		default: break;
		}
	return (FLAG2 - 4);
}
int MainPage::save2file() {
	FILE* fp = &fbuf;
	if (!_wfopen_s(&fp, filepath, L"wb")) {
		fwrite(arr, sizeof(short), 36, fp);
		fwrite(arr2, sizeof(short), 36, fp);
		fclose(fp);
	}
	return 0;
}
int MainPage::upadd() {
	FLAG = 0;
	for (int j = 1; j <= 4; j++)
		switch ((arr[1][j] ? 8 : 0) + (arr[2][j] ? 4 : 0) + (arr[3][j] ? 2 : 0) + (arr[4][j] ? 1 : 0)) {
		case 0: case 8: break;
		case 12:
			if (arr[1][j] == arr[2][j]) {
				arr[1][j] *= 2;
				FLAG++;
				arr[2][j] = 0;
			}
			break;
		case 14:
			if (arr[1][j] == arr[2][j]) {
				arr[1][j] *= 2;
				FLAG++;
				arr[2][j] = arr[3][j];
				arr[3][j] = 0;
			}
			else if (arr[2][j] == arr[3][j]) {
				arr[2][j] *= 2;
				FLAG++;
				arr[3][j] = 0;
			}
			break;
		case 15:
			if (arr[1][j] == arr[2][j]) {
				arr[1][j] *= 2;
				FLAG++;
				arr[2][j] = 0;
			}
			else if (arr[2][j] == arr[3][j]) {
				arr[2][j] *= 2;
				FLAG++;
				arr[3][j] = 0;
			}
			if (arr[3][j] == arr[4][j]) {
				arr[3][j] *= 2;
				FLAG++;
				arr[4][j] = 0;
			}
			upmove();
			break;
		}
	return FLAG;
}
int MainPage::upmove() {
	FLAG2 = 0;
	for (int j = 1; j <= 4; j++)
		switch ((arr[1][j] ? 8 : 0) + (arr[2][j] ? 4 : 0) + (arr[3][j] ? 2 : 0) + (arr[4][j] ? 1 : 0)) {
		case 0: case 8: case 12: case 14: case 15: FLAG2++; break;
		case 1: arr[1][j] = arr[4][j]; arr[4][j] = 0; break;
		case 2: arr[1][j] = arr[3][j]; arr[3][j] = 0; break;
		case 3: arr[1][j] = arr[3][j]; arr[2][j] = arr[4][j]; arr[3][j] = arr[4][j] = 0; break;
		case 4: arr[1][j] = arr[2][j]; arr[2][j] = 0; break;
		case 5: arr[1][j] = arr[2][j]; arr[2][j] = arr[4][j]; arr[4][j] = 0; break;
		case 6: arr[1][j] = arr[2][j]; arr[2][j] = arr[3][j]; arr[3][j] = 0; break;
		case 7: arr[1][j] = arr[2][j]; arr[2][j] = arr[3][j]; arr[3][j] = arr[4][j]; arr[4][j] = 0; break;
		case 9: arr[2][j] = arr[4][j]; arr[4][j] = 0; break;
		case 10: arr[2][j] = arr[3][j]; arr[3][j] = 0; break;
		case 11: arr[2][j] = arr[3][j]; arr[3][j] = arr[4][j]; arr[4][j] = 0; break;
		case 13: arr[3][j] = arr[4][j]; arr[4][j] = 0; break;
		default: break;
		}
	return (FLAG2 - 4);
}