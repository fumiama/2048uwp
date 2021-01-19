//
// MainPage.xaml.h
// MainPage 类的声明。
//

#pragma once
#include "MainPage.g.h"

namespace _2048uwp
{
	/// <summary>
	/// 可用于自身或导航至 Frame 内部的空白页。
	/// </summary>
	public ref class MainPage sealed
	{
	public:
		MainPage();

	private:
		int activeblocks();
		void brushinit();
		void btninit();
		short calScore(short arr[6][6]);
		int downadd();
		int downmove();
		void drawblocks();
		void gamedisplay();
		void goBack();
		int havempty();
		int isbglight();
		int issingle(short i, short j);
		int judgeWin();
		int leftadd();
		int leftmove();
		errno_t loadfromfile();
		void newblock();
		void printmyscore();
		int rightadd();
		int rightmove();
		int save2file();
		int upadd();
		int upmove();

		void stage_SizeChanged(Platform::Object^ sender, Windows::UI::Xaml::SizeChangedEventArgs^ e);
		void Page_SizeChanged(Platform::Object^ sender, Windows::UI::Xaml::SizeChangedEventArgs^ e);
		void bbk_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
		void bre_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
		void bs_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
		void bpage_KeyUp(Platform::Object^ sender, Windows::UI::Xaml::Input::KeyRoutedEventArgs^ e);
		void basegrid_ManipulationCompleted(Platform::Object^ sender, Windows::UI::Xaml::Input::ManipulationCompletedRoutedEventArgs^ e);
		void bpage_RightTapped(Platform::Object^ sender, Windows::UI::Xaml::Input::RightTappedRoutedEventArgs^ e);
		void basegrid_ManipulationDelta(Platform::Object^ sender, Windows::UI::Xaml::Input::ManipulationDeltaRoutedEventArgs^ e);
		void bhide_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
	};
}
