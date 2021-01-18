//
// MainPage.xaml.cpp
// MainPage 类的实现。
//

#include "pch.h"
#include "MainPage.xaml.h"
#include "ui.h"
using namespace Concurrency;

using namespace _2048uwp;
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

static wchar_t buff[1024];
static float rbh = 0;

MainPage::MainPage()
{
	InitializeComponent();
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
void _2048uwp::MainPage::bbk_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
	memcpy(arr, arr2, sizeof(arr)); drawblocks(); printmyscore(); save2file();
}

void _2048uwp::MainPage::bre_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
	ContentDialog^ refreshGame = ref new ContentDialog;
	refreshGame->Title = "重置游戏";
	refreshGame->Content = "您将失去现有进度";
	refreshGame->PrimaryButtonText = "重置";
	refreshGame->CloseButtonText = "取消";
	refreshGame->DefaultButton = ContentDialogButton::Close;
	IAsyncOperation<ContentDialogResult>^ refreshTask = refreshGame->ShowAsync();
	create_task(refreshTask).then([this](ContentDialogResult result) {
		if (result == ContentDialogResult::Primary){
			_wremove(filepath); gamedisplay(); savestage();
		}
	});
}
void _2048uwp::MainPage::bs_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
	wchar_t* buf = buff;
	char c = 0;

	memset(buff, 0, sizeof(buff));
	swprintf_s(buf, 1024, L"方文社©2020 by 源文雨.\n\t\tNow\n");
	for (int i = 0; i <= 5; i++) {
		for (int j = 0; j <= 5; j++) {
			swprintf_s(buf + wcslen(buf), 1024 - wcslen(buf), L"%05d ", arr[i][j]);
			if (arr[i][j]) c++;
		}
		swprintf_s(buf + wcslen(buf), 1024 - wcslen(buf), L"\n");
	}
	swprintf_s(buf + wcslen(buf), 1024 - wcslen(buf), L"\t\tPrev\n");
	for (int i = 0; i <= 5; i++) {
		for (int j = 0; j <= 5; j++) {
			swprintf_s(buf + wcslen(buf), 1024 - wcslen(buf), L"%05d ", arr[i][j]);
			if (arr2[i][j]) c++;
		}
		swprintf_s(buf + wcslen(buf), 1024 - wcslen(buf), L"\n");
	}
	swprintf_s(buf + wcslen(buf), 1024 - wcslen(buf), L"Manipulation: (%d, %d). ", XPOS, YPOS);
	swprintf_s(buf + wcslen(buf), 1024 - wcslen(buf), L"Win status: %d. Fail status: %d.\n", ISWIN, ISFAIL);
	//swprintf_s(buf + wcslen(buf), 1024 - wcslen(buf), L"Active blocks in all data: %d. ", c);
	//swprintf_s(buf + wcslen(buf), 1024 - wcslen(buf), L"Active blocks in stage: %d.\n", activeblocks());
	swprintf_s(buf + wcslen(buf), 1024 - wcslen(buf), L"Background color is #%02X%02X%02X%02X, so the system is in ", UISettings().GetColorValue(UIColorType::Background).A, UISettings().GetColorValue(UIColorType::Background).R, UISettings().GetColorValue(UIColorType::Background).G, UISettings().GetColorValue(UIColorType::Background).B);
	swprintf_s(buf + wcslen(buf), 1024 - wcslen(buf), L"%s mode.\n", isbglight()?L"light":L"dark");
	//String^ s = ref new String(buf);
	//s += L"Data folder: " + ApplicationData::Current->LocalFolder->Path;
	ContentDialog^ message = ref new ContentDialog;
	message->Title = "Info";
	message->Content = ref new String(buf);
	message->PrimaryButtonText = "确定";
	//MessageDialog^ msg = ref new MessageDialog(ref new String(buf));
	//msg->Title = "Info";
	//msg->ShowAsync();
	message->ShowAsync();
	//delete[] s;
	//if(!er) fclose(fp);
}
void _2048uwp::MainPage::bpage_KeyUp(Platform::Object^ sender, Windows::UI::Xaml::Input::KeyRoutedEventArgs^ e)
{
	NOMOV = 1;

	switch (e->Key.ToString()->Begin()[0]) {
	case 'L': savestage(); merge(leftmove, leftadd);  break;
	case 'R':  savestage(); merge(rightmove, rightadd); break;
	case 'U':  savestage(); merge(upmove, upadd); break;
	case 'D':  savestage(); merge(downmove, downadd); break;
	case 'B': memcpy(arr, arr2, sizeof(arr)); drawblocks(); printmyscore(); save2file(); break;
	case 'E': Windows::ApplicationModel::Core::CoreApplication::Exit();
	default: return; break;
	}
	if (NOMOV) { loadfromfile(); havempty(); return; }
	else {
		if (havempty()) newblock();
		if (!ISWIN && !ISFAIL) printmyscore();
		if (!ISFAIL) { drawblocks(); save2file(); }
	}
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
		savestage();
		MVTAG = 1;
		if (AXPOS > AYPOS) {
			if (XPOS < 0) { mmerge(leftmove, leftadd); }
			else { mmerge(rightmove, rightadd); }
		}
		else if (YPOS > 0) { mmerge(downmove, downadd); }
		else { mmerge(upmove, upadd); }
		if (MUMOV) { loadfromfile(); havempty(); return; }
		else {
			if (havempty()) newblock();
			if (!ISWIN && !ISFAIL) printmyscore();
			if (!ISFAIL) { drawblocks(); save2file(); }
		}
	}
}
void _2048uwp::MainPage::bhide_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
	if (CH++ > 6) { arr[3][4] = 2048; CH = 0; }
}
