﻿//Copyright © 2019 (´･@･)
//[License]GNU Affero General Public License, version 3
//[Contact]http://tacoika.blog87.fc2.com/
#pragma once

namespace SDX_BSC
{
	//LayoutValue
	#define LV(a) DV::I[0][a]
	
	
	using namespace SDX;

	/*経営戦略ウィンドウ*/
	class W_Management : public WindowBox
	{
	private:
		class GUI_MLv : public GUI_Object
		{
		public:
			ManagementType 部門;

			void Draw派生(double px, double py)
			{
				double rate = (double)Guild::P->投資経験値[部門] / Management::必要経験値[Guild::P->投資Lv[部門]];

				MSystem::DrawBar({ px,py }, (int)位置.GetW(), (int)位置.GetH(), rate, 1, Color::Blue, Color::White, Color::White, true);

				MFont::SSize.DrawBold({ px + LV(30) ,py + LV(31) }, Color::White, Color::Black, { "Lv",Guild::P->投資Lv[部門] });

			}
		};

		class GUI_Gold : public GUI_Object
		{
		public:
			void Draw派生(double px, double py)
			{
				//選択戦術の資金消費を表示、不足している場合赤色
				MSystem::DrawWindow({ px ,py }, 位置.GetW(), 位置.GetH(), 11);

				//現在の資金
				MIcon::アイコン[IconType::資金].Draw({ px + LV(34) , py + LV(35) });
				MFont::BMSize.DrawBold({ px + LV(32) ,py + LV(33) }, Color::White, Color::Black, { (long long)Guild::P->資金 , " G" }, true);
				//消費する資金
				if (GUI_Help::戦術 != nullptr)
				{
					Color fc = { 255,64,64 };
					//if (Guild::P->選択戦術->消費資金 > Guild::P->資金) { fc = Color::Red; }
					MFont::BMSize.DrawBold({ px + LV(36) ,py + LV(37) }, fc, Color::Black, { "- " ,GUI_Help::戦術->消費資金 , " G" }, true);
					GUI_Help::戦術 = nullptr;
				}
				else if (Guild::P->選択戦術 != MSkillType::COUNT )
				{
					Color fc = {255,64,64};
					//if (Guild::P->選択戦術->消費資金 > Guild::P->資金) { fc = Color::Red; }
					MFont::BMSize.DrawBold({ px + LV(36) ,py + LV(37) }, fc, Color::Black, {"- " , Management::data[(int)Guild::P->選択戦術].消費資金 , " G"}, true);
				}

			}
		};
		class GUI_Rank : public GUI_Object
		{
		public:
			int ランク = 0;

			void Draw派生(double px, double py)
			{
				MSystem::DrawWindow({ px ,py }, 位置.GetW(), 位置.GetH(), 11);

				MFont::BMSize.DrawBold({ px + LV(41),py + LV(42) }, Color::White, Color::Black, { "Lv " , ランク });

			}
		};
		class GUI_Skill : public GUI_Object
		{
		public:
			Management* 参照戦術;
			int クリック時間 = 0;

			void Draw派生(double px, double py)
			{
				Color bc = Color::Black;
				bool can使用 = 参照戦術->can使用 && (参照戦術->消費資金 <= Guild::P->資金);
				bool isLv = (参照戦術->Lv <= Guild::P->投資Lv[参照戦術->系統]);
				bool is選択中 = (Guild::P->選択戦術 == 参照戦術->MID);

				int 枠No = 1;
				int 凹凸 = 0;
				if (参照戦術->is永続 && 参照戦術->使用回数 > 0)
				{
					//使用済み、永続
					枠No = 5;
				}
				else if (!isLv)
				{
					//Lv不足は灰色
					枠No = 15;
				}
				else if (is選択中 || クリック時間 > 0)
				{
					//選択中は凹み
					枠No = 4;
					凹凸 = -2;
				}
				else if (can使用)
				{
					//使用可能で盛り上がり
					枠No = 1;
					凹凸 = 2;
				}
				else
				{
					//資金不足
					枠No = 3;
				}

				MSystem::DrawWindow({ px,py }, (int)位置.GetW(), (int)位置.GetH(),枠No,凹凸);
				MIcon::アイコン[参照戦術->アイコン].DrawRotate({px + (int)位置.GetW()/2 - 凹凸,py + (int)位置.GetH()/2 - 凹凸 },2,0);

				クリック時間--;

			}
			void Click(double px, double py)
			{
				//お金足りてたら使用、不足してたら予約状態にする
				if (!参照戦術->can使用)
				{
					return;
				}


				if (Guild::P->選択戦術 != 参照戦術->MID)
				{
					Guild::P->選択戦術 = 参照戦術->MID;
					if (Guild::P->資金 >= 参照戦術->消費資金)
					{
						MSound::効果音[SE::投資実行].Play();
					} else {
						MSound::効果音[SE::投資予約].Play();
					}
				} else {
					Guild::P->選択戦術 = MSkillType::COUNT;
					MSound::効果音[SE::投資解除].Play();
				}

				クリック時間 = 5;
			}

			void Info派生(Point 座標) override
			{
				InfoManagement(参照戦術, 座標);
			}

		};

	public:
		std::vector<GUI_Tab> タブ;
		GUI_Gold gui_gold;//消費資金
		GUI_MLv gui_mlv[4];//部門LVと経験値バー
		//GUI_実行 gui_実行;//実行or予約ボタン

		GUI_Rank gui_rank[10];//ランク毎の区切り-10個？
		GUI_Skill gui_skill[100];//各種戦術アイコン、とりあえず最大100

		//消費資金

		int 現在タブ = 0;
		int 戦術数 = 0;

		void init()
		{
			種類 = WindowType::Management;
			名前 = TX::Window_名前[種類];
			略記 = TX::Window_略記[種類];
			SetHelp(TX::Window_ヘルプ[種類]);

			アイコン = IconType::戦略;
			横幅 = 320;
			縦幅 = 300;
			最小縦 = 200;
			最大縦 = 600;
			縦内部幅 = 600;//変動する
			固定縦 = 85;
			スクロール位置 = 0;

			タブ.emplace_back(現在タブ, 0, IconType::情報, "");//経営
			タブ.emplace_back(現在タブ, 1, IconType::求人, "");//人事
			タブ.emplace_back(現在タブ, 2, IconType::製造, "");//製造
			タブ.emplace_back(現在タブ, 3, IconType::迷宮, "");//探索

			for (int a = 0; a < 4; a++)
			{
				タブ[a].SetHelp(TX::Manage_タブヘルプ[a]);
			}

			gui_mlv[0].部門 = ManagementType::経営;
			gui_mlv[1].部門 = ManagementType::人事;
			gui_mlv[2].部門 = ManagementType::製造;
			gui_mlv[3].部門 = ManagementType::探索;

			gui_objects.push_back(&gui_gold);
			for (int a = 0; a < 4; a++) { gui_objects.push_back(&タブ[a]); }
			for (int a = 0; a < 4; a++) { gui_objects.push_back(&gui_mlv[a]); }
			for (int a = 0; a < 10; a++) { gui_objects.push_back(&gui_rank[a]); }
			for (int a = 0; a < 100; a++) { gui_objects.push_back(&gui_skill[a]); }

			gui_gold.SetHelp( TX::Manage_お金 );

			gui_gold.isヘルプ表示 = true;
			for (int a = 0; a < 4; a++) { タブ[a].isヘルプ表示 = true; }
			for (int a = 0; a < 4; a++) { gui_mlv[a].isヘルプ表示 = true; }

			gui_gold.is固定 = true;
			for (int a = 0; a < 4; a++) { タブ[a].is固定 = true; }
			for (int a = 0; a < 4; a++) { gui_mlv[a].is固定 = true; }


			GUI_Init();
			Tub_Change();
		}

		void GUI_Init()
		{
			タブ[0].位置 = { LV(0) ,         LV(1) ,LV(2) ,LV(3) };
			タブ[1].位置 = { LV(0) + LV(4)  ,LV(1) ,LV(2) ,LV(3) };
			タブ[2].位置 = { LV(0) + LV(4) * 2,LV(1) ,LV(2) ,LV(3) };
			タブ[3].位置 = { LV(0) + LV(4) * 3,LV(1) ,LV(2) ,LV(3) };

			for (auto&it : タブ)
			{
				it.文字オフセット = -18;
			}

			gui_gold.位置 = { LV(5),LV(9),LV(7),LV(10) };
			gui_mlv[0].位置 = { LV(5),LV(11),LV(12),LV(13) };
			gui_mlv[1].位置 = { LV(5)+LV(14),LV(11),LV(12),LV(13) };
			gui_mlv[2].位置 = { LV(5) + LV(14)*2,LV(11),LV(12),LV(13) };
			gui_mlv[3].位置 = { LV(5) + LV(14)*3,LV(11),LV(12),LV(13) };

			//gui_実行.位置 = { LV(15) , LV(16) , LV(17) , LV(18) };
		}

		void Tub_Change()
		{
			int n = 0;
			int r = -1;
			int x = LV(20);
			int y = LV(21);

			for (int a = 0; a < 10; a++)
			{
				gui_rank[a].ランク = a + 1;
				gui_rank[a].位置.x = -1000;
			}

			for (int a = 0; a < 100; a++)
			{
				gui_skill[a].位置.x = -1000;
			}


			for (auto& it : Management::data)
			{
				if ((int)it.系統 == 現在タブ)
				{
					if (it.Lv != r)
					{
						if (r != -1) { y += LV(25); }
						r = it.Lv;
						gui_rank[r-1].位置 = { LV(20) , y , LV(26),LV(27) };
						y += LV(28);
						x = LV(20);
					}

					if (x >= LV(20) + LV(24) * 6)
					{
						x = LV(20);
						y += LV(25);
					}

					gui_skill[n].参照戦術 = &it;
					gui_skill[n].位置 = { x , y , LV(22),LV(23) };
					x += LV(24);

					n++;
				}
			}

			戦術数 = n;
			縦内部幅 = y + 50 + 固定縦;
		}


		void 派生Draw()
		{			
			GUI_Init();

			for (auto&it : タブ)
			{
				it.Draw();
			}
			MSystem::DrawWindow({LV(0),LV(1)+45},282,700, 12, 0);

			//固定描画
			gui_gold.Draw();
			for (auto &it : gui_mlv)
			{
				it.Draw();
			}


			//スクロールする
			描画範囲(true);

			for (auto &it : gui_rank)
			{
				it.Draw();
			}

			for (int a = 0; a < 戦術数; a++)
			{
				gui_skill[a].Draw();
			}
		}

		bool 派生操作()
		{
			int tubcheck = 現在タブ;

			for (auto& it : タブ)
			{
				it.操作チェック(座標.x, 座標.y + タイトル枠高さ);
			}

			for (auto& it : gui_skill)
			{
				if (Input::mouse.y - 座標.y - タイトル枠高さ < 固定縦){break;}

				it.操作チェック(相対座標.x, 相対座標.y);
			}

			if (tubcheck != 現在タブ)
			{
				Tub_Change();
			}

			return false;
		}
	};

	#undef LV
	#undef LV2
	#undef LV4
}