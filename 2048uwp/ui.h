#pragma once
#include "pch.h"
#include "MainPage.xaml.h"

using namespace _2048uwp;
using namespace Platform;
using namespace Windows::Foundation;
using namespace Windows::Foundation::Collections;
using namespace Windows::Storage;
using namespace Windows::UI::ViewManagement;
using namespace Windows::UI::Xaml;
using namespace Windows::UI::Xaml::Controls;
using namespace Windows::UI::Xaml::Controls::Primitives;
using namespace Windows::UI::Xaml::Data;
using namespace Windows::UI::Xaml::Input;
using namespace Windows::UI::Xaml::Media;
using namespace Windows::UI::Xaml::Navigation;;

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

#define DX			e->Delta.Translation.X
#define DY			e->Delta.Translation.Y

static short arr2[6][6];
#define savestage() memcpy(arr2, arr, sizeof(arr))
#define merge(x, y) FLAG3 = !x(); FLAG4 = !y(); NOMOV = FLAG3 && FLAG4
#define mmerge(x, y) FLAG3 = !x(); FLAG4 = !y(); MUMOV = FLAG3 && FLAG4

static Button^ btngrp[4][4];
#define drawb(color) setbtncolor(btngrp[i][j], color)
#define drawt(color, text) setbtntxt(btngrp[i][j], text); setbtntxtcolor(btngrp[i][j], color)

static FILE fbuf;
static String^ path;
static const wchar_t* filepath;
#define localpath ApplicationData::Current->LocalFolder->Path
#define getfilepath() path = localpath; path += "\\savedata"; filepath = path->Begin()

int activeblocks();
void brushinit();
//void btninit();
short calScore(short arr[6][6]);
int downadd();
int downmove();
//void drawblocks();
//void gamedisplay();
//havempty();
int isbglight();
int leftadd();
int leftmove();
errno_t loadfromfile();
void newblock();
//printmyscore();
int rightadd();
int rightmove();
int save2file();
int upadd();
int upmove();

int activeblocks() {
	char c = 0;
	for (int i = 1; i <= 4; i++) {
		for (int j = 1; j <= 4; j++) {
			if (arr[i][j]) c++;
		}
	}
	return c;
}
void brushinit() {
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
void _2048uwp::MainPage::btninit() {
	btngrp[0][0] = b11; btngrp[0][1] = b12; btngrp[0][2] = b13; btngrp[0][3] = b14;
	btngrp[1][0] = b21; btngrp[1][1] = b22; btngrp[1][2] = b23; btngrp[1][3] = b24;
	btngrp[2][0] = b31; btngrp[2][1] = b32; btngrp[2][2] = b33; btngrp[2][3] = b34;
	btngrp[3][0] = b41; btngrp[3][1] = b42; btngrp[3][2] = b43; btngrp[3][3] = b44;
}
short calScore(short arr[6][6]) {
	SCORE = 0;
	for (int i = 1; i <= 4; i++) {
		for (int j = 1; j <= 4; j++) {
			SCORE += arr[i][j];
		}
	}
	return SCORE % 9999 + 1;
}
int downadd() {
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
int downmove() {
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
void _2048uwp::MainPage::drawblocks() {
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
				break;
			case 4096:
				drawb(COLOR4096);
				drawt(WHITE, "4096");
				break;
			case 8192:
				drawb(COLOR8192);
				drawt(WHITE, "8192");
				break;
			default:
				drawb(COLOR8192);
				drawt(WHITE, arr[i+1][j+1]);
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
	if(loadfromfile() || !activeblocks()) newblock();
	if (!~CH) {
		if ((CH = isbglight())) basegrid->Opacity = 1.0;
		else basegrid->Opacity = 0.7;
	}
	drawblocks();
	printmyscore();
	havempty();
}
int _2048uwp::MainPage::havempty() {
	char c = 0;
	ISWIN = 0;
	for (int i = 1; i <= 4; i++) for (int j = 1; j <= 4; j++) {
		if (arr[i][j]) c++;
		if (arr[i][j] >= 2048) {
			bs->Content = "WIN!";
			return ISWIN = 1;
			break;
		}
	}
	if (c >= 16) {
		if (!ISFAIL) {
			bs->Content = "FAILED!";
			ISFAIL = 1;
		}
	}
	else ISFAIL = 0;
	return !ISFAIL;
}
int isbglight() {
	return (0.299 * UISettings().GetColorValue(UIColorType::Background).R +
		0.587 * UISettings().GetColorValue(UIColorType::Background).G +
		0.114 * UISettings().GetColorValue(UIColorType::Background).B) / 255.0 > 0.5;
}
int leftadd() {
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
int leftmove() {
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
errno_t loadfromfile() {
	FILE* fp = &fbuf;
	errno_t e = _wfopen_s(&fp, filepath, L"rb");
	if (!e) {
		fread(arr, sizeof(short), 36, fp);
		fread(arr2, sizeof(short), 36, fp);
		fclose(fp);
	}
	return e;
}
void newblock() {
	int x = rand() % 4 + 1;
	int y = rand() % 4 + 1;
	while (arr[x][y]) {
		x = rand() % 4 + 1;
		y = rand() % 4 + 1;
	}
	if (!(rand() % 10)) arr[x][y] = 4;
	else arr[x][y] = 2;
}
void _2048uwp::MainPage::printmyscore() {
	bs->Content = calScore(arr);
}
int rightadd() {
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
			else FLAG++;
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
int rightmove() {
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
int save2file() {
	FILE* fp = &fbuf;
		if (!_wfopen_s(&fp, filepath, L"wb")) {
			fwrite(arr, sizeof(short), 36, fp);
			fwrite(arr2, sizeof(short), 36, fp);
			fclose(fp);
		}
		return 0;
}
int upadd() {
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
int upmove() {
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