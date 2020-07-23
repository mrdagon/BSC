﻿//Copyright © 2019 (´･@･)
//[License]GNU Affero General Public License, version 3
//[Contact]http://tacoika.blog87.fc2.com/
#pragma once

namespace SDX_BSC
{
	using namespace SDX;

	/*パーティウィンドウ*/
	class W_Party: public WindowBox
	{
	private:

		class GUI_探索先 : public GUI_Object
		{
		public:
			int パーティID;
			Guild::Party* 所属;
			W_Party* 親ウィンドウ;

			void Init(int パーティID, W_Party* 親ウィンドウ)
			{
				this->パーティID = パーティID;
				所属 = &Guild::P->探索パーティ[パーティID];
				this->親ウィンドウ = 親ウィンドウ;
			}

			void Draw派生(double px, double py)
			{
				MSystem::DrawWindow({ px,py }, (int)位置.GetW(), (int)位置.GetH(), 11);

				auto dun = Guild::P->探索パーティ[パーティID].探索先;

				if (所属->探索状態 == ExplorerType::編成中 || 所属->探索状態 == ExplorerType::リザルト中)
				{
					MSystem::DrawWindow({ px + Lp(70) ,py + Lp(71) }, Lp(72), Lp(73), 0, 1);
					MIcon::アイコン[IconType::三角].Draw({ px + Lp(74),py + Lp(76) });
					MIcon::アイコン[IconType::三角].Draw({ px + Lp(75),py + Lp(76) }, true);
				} else {
					MSystem::DrawWindow({ px + Lp(70) ,py + Lp(71) }, Lp(72), Lp(73), 1, 0);
				}

				//探索先アイコン、レベル
				MIcon::アイコン[dun->アイコン].Draw({ px + Lp(12),py + Lp(13) });
				MFont::BSSize.DrawBold({ px + Lp(14) ,py + Lp(15) }, Color::White, Color::Black, { "Lv ", dun->Lv });
				//探索度ゲージと探索率
				MSystem::DrawBar({ px + Lp(18) , py + Lp(19) }, Lp(20), Lp(21), dun->探索率, 1 , Color::Blue, Color::White, Color::White, true);				
				MFont::BSSize.DrawBold({ px + Lp(16) ,py + Lp(17) }, Color::White, Color::Black, { (int)(dun->探索率 * 100) , "%" }, true);				
				//ボス状態
				MIcon::アイコン[IconType::ボス].DrawRotate({ px + Lp(22),py + Lp(23) }, 1, 0);

				std::string sボス状態;
				if (dun->isボス生存 == false)
				{
					sボス状態 = TX::Dungeon_討伐;
				}
				else if(dun->isボス発見 == true)
				{
					sボス状態 = TX::Dungeon_発見;
				}
				else
				{
					sボス状態 = TX::Dungeon_捜索;
				}

				MFont::BSSize.DrawBold({ px + Lp(22) + 50 ,py + Lp(23) - 9 }, Color::White, Color::Black, sボス状態, true);
				//地図状態
				MIcon::アイコン[IconType::地図].DrawRotate({ px + Lp(22),py + Lp(24) }, 1, 0);
				MFont::BSSize.DrawBold({ px + Lp(22) + 50 ,py + Lp(24) - 9 }, Color::White, Color::Black, { dun->発見地図 , " / " , dun->最大地図}, true);
				//財宝状態
				MIcon::アイコン[IconType::宝箱].DrawRotate({ px + Lp(22),py + Lp(25) }, 1, 0);
				MFont::BSSize.DrawBold({ px + Lp(22) + 50 ,py + Lp(25) - 9 }, Color::White, Color::Black, { dun->発見財宝 , " / " , dun->最大財宝 }, true);


				//探索指示-冒険中は三角を非表示
				if (所属->探索状態 == ExplorerType::編成中 || 所属->探索状態 == ExplorerType::リザルト中)
				{
					MSystem::DrawWindow({ px + Lp(31) ,py + Lp(32) }, Lp(33), Lp(34), 0, 1);
					MIcon::アイコン[IconType::三角].Draw({ px + Lp(26),py + Lp(28) });
					MIcon::アイコン[IconType::三角].Draw({ px + Lp(27),py + Lp(28) }, true);

				} else {
					MSystem::DrawWindow({ px + Lp(31) ,py + Lp(32) }, Lp(33), Lp(34), 1, 0);
				}

				std::string siji;
				switch (Guild::P->探索パーティ[パーティID].探索指示)
				{
				case OrderType::探索: siji = TX::Party_ボス回避; break;
				case OrderType::ボス: siji = TX::Party_ボス討伐; break;
				}

				MFont::BSSize.DrawBold({ px + Lp(29) ,py + Lp(30) }, Color::White, Color::Black, siji, true);
			}

			void Click(double px, double py)
			{
				if (所属->探索状態 != ExplorerType::編成中 && 所属->探索状態 != ExplorerType::リザルト中) { return; }

				//探索指示変更
				if (Point(px, py).Hit(&Rect(Lp(31), Lp(32), Lp(33), Lp(34))) == true)
				{
					int n = int(Guild::P->探索パーティ[パーティID].探索指示);

					if (px < Lp(31) + Lp(33) / 2) { n--; }
					else { n++; }


					if (n == (int)OrderType::COUNT) { n = 0; }
					if (n <  0) { n = (int)OrderType::COUNT - 1; }

					Guild::P->探索パーティ[パーティID].探索指示 = OrderType(n);
					MSound::効果音[SE::ボタンクリック].Play();
				}
				//探索先変更
				if (Point(px, py).Hit(&Rect(Lp(70), Lp(71), Lp(72), Lp(73))) == true)
				{
					bool result = false;

					if (px < Lp(70) + Lp(72) / 2) { result = 所属->探索先前後(false); }
					else { result = 所属->探索先前後(true); }

					if (result) { MSound::効果音[SE::ボタンクリック].Play(); }
				}
			}

			void Drop(double px, double py)
			{
				if (W_Drag_Drop::ダンジョン == nullptr) { return; }
				if (所属->探索状態 != ExplorerType::編成中 && 所属->探索状態 != ExplorerType::リザルト中)
				{ 
					所属->探索先予約 = W_Drag_Drop::ダンジョン;
					MSound::効果音[SE::配置換え].Play();
					return;
				}

				//探索先変更
				所属->探索先 = W_Drag_Drop::ダンジョン;
				MSound::効果音[SE::配置換え].Play();
			}

			void Info派生(Point 座標) override
			{
				Point 補正座標;
				補正座標.x = 座標.x - 親ウィンドウ->相対座標.x - 位置.x;
				補正座標.y = 座標.y - 親ウィンドウ->相対座標.y - 位置.y;


				if (補正座標.Hit(&Rect(Lp(31), Lp(32), Lp(33), Lp(34))) == true)
				{
					//探索指示や入れ替え等の操作方法
					SetHelp( TX::Party_探索方針);
					Info座標補正(座標);
					MSystem::DrawWindow({ 座標.x , 座標.y }, ヘルプ横幅, ヘルプ縦幅, 4);
					MFont::MSize.DrawBold({ 座標.x + 10,座標.y + 10 }, Color::White, Color::Black, ヘルプメッセージ);
				}
				else 
				{
					//探索方針
					InfoDungeon(Guild::P->探索パーティ[パーティID].探索先, 座標);
				}
			}

		};

		class GUI_パーティメンバー : public GUI_Object
		{
		public:
			W_Skilltree* スキルツリー;
			Warker* ギルメン;
			Guild::Party* 所属;
			W_Party* 親ウィンドウ;
			int 並びID;//1パーティ目 0~4、22パーティ目5～9 ...

			void Draw派生(double px, double py)
			{
				//未配置時の表示
				if (ギルメン == nullptr)
				{
					//MSystem::DrawBox({ px,py }, (int)位置.GetW(), (int)位置.GetH(), Color::Gray);
					MSystem::DrawWindow({ px,py }, (int)位置.GetW(), (int)位置.GetH(), 4);
					return;
				}

				MSystem::DrawWindow({ px,py }, (int)位置.GetW(), (int)位置.GetH(), 1);

				//アイコン、Lv、Expゲージ
				MUnit::ユニット[ギルメン->見た目][10]->DrawRotate({ px + Lp(35) ,py + Lp(36) }, 2, 0);
				MSystem::DrawBar({ px + Lp(39),py + Lp(40) }, Lp(41), Lp(42), ギルメン->経験値 / ギルメン->Get要求経験値(), 1, Color::Blue, Color::White, Color::White, true);
				MFont::BSSize.DrawBold({ px + Lp(37) ,py + Lp(38) }, Color::White, Color::Black, { "Lv " ,ギルメン->Lv }, true);

				//装備品 x 3と更新ボタン、自動更新設定、装備ランク
				MSystem::DrawWindow({ px + Lp(43) ,py + Lp(46) }, Lp(48), Lp(49), 0);
				MSystem::DrawWindow({ px + Lp(44) ,py + Lp(46) }, Lp(48), Lp(49), 0);
				MSystem::DrawWindow({ px + Lp(45) ,py + Lp(47) }, Lp(48), Lp(49), 0);
				MIcon::アイテム[Item::data[ギルメン->装備[0]].見た目].Draw({ px + Lp(43) + Lp(50) , py + Lp(46) + Lp(51) });
				MIcon::アイテム[Item::data[ギルメン->装備[1]].見た目].Draw({ px + Lp(44) + Lp(50)  , py + Lp(46) + Lp(51) });
				MIcon::アイテム[Item::data[ギルメン->装備[2]].見た目].Draw({ px + Lp(45) + Lp(50) , py + Lp(47) + Lp(51) });

				MFont::BSSize.DrawBold({ px + Lp(52) , py + Lp(54) }, Color::White, Color::Black, { "Lv " , Item::data[ギルメン->装備[0]].Lv });
				MFont::BSSize.DrawBold({ px + Lp(53) , py + Lp(54) }, Color::White, Color::Black, { "Lv " ,Item::data[ギルメン->装備[1]].Lv });

				//装備更新ボタン
				if (ギルメン->is装備更新)
				{
					MSystem::DrawWindow({ px + Lp(55) ,py + Lp(56) }, Lp(57), Lp(58), 2, -1);
				} else {
					MSystem::DrawWindow({ px + Lp(55) ,py + Lp(56) }, Lp(57), Lp(58), 0, 1);
				}

				MFont::BSSize.DrawBold({ px + Lp(59) ,py + Lp(60) }, Color::White, Color::Black, "装備更新");
			}

			void Click(double px, double py)
			{
				if (所属->探索状態 != ExplorerType::編成中) { return; }
				if (ギルメン == nullptr) { return; }

				//装備掴み
				if (Point(px, py).Hit(&Rect(Lp(43), Lp(46), Lp(48), Lp(49))) == true)
				{
					W_Drag_Drop::ギルメン装備.メンバー = ギルメン;
					W_Drag_Drop::ギルメン装備.部位 = 0;
					return;
				}

				if (Point(px, py).Hit(&Rect(Lp(44), Lp(46), Lp(48), Lp(49))) == true)
				{
					W_Drag_Drop::ギルメン装備.メンバー = ギルメン;
					W_Drag_Drop::ギルメン装備.部位 = 1;
					return;
				}

				if (Point(px, py).Hit(&Rect(Lp(45), Lp(47), Lp(48), Lp(49))) == true)
				{
					W_Drag_Drop::ギルメン装備.メンバー = ギルメン;
					W_Drag_Drop::ギルメン装備.部位 = 2;
					return;
				}

				//自動更新切り替え
				if (Point(px, py).Hit(&Rect(Lp(55), Lp(56), Lp(57), Lp(58))) == true)
				{
					//ギルメン->is装備更新 = !ギルメン->is装備更新;
					//MSound::効果音[SE::ボタンクリック].Play();
					スキルツリー->ギルメン = ギルメン;
					スキルツリー->配置id = 並びID;
					スキルツリー->init();
					スキルツリー->ポップアップ呼び出し();
					return;
				}

				//ギルメン掴む
				W_Drag_Drop::探索メン = ギルメン;
				W_Drag_Drop::並びID = 並びID;
				MSound::効果音[SE::ドラッグ].Play();
			}

			void Drop(double px, double py)
			{
				if (所属->探索状態 != ExplorerType::編成中) { return; }

				if (W_Drag_Drop::探索メン != nullptr)
				{
					//ギルメン入れ替え
					W_Drag_Drop::パーティ移動( ギルメン , 並びID);
				}
				else if(W_Drag_Drop::アイテム != -1 && ギルメン != nullptr)
				{
					//装備変更
					int 部位 = 0;
					if (Item::data[W_Drag_Drop::アイテム].種類 == ItemType::アクセサリー)
					{
						部位 = 2;
					}else if (Item::data[W_Drag_Drop::アイテム].種類 == ItemType::隠鎧 ||
						Item::data[W_Drag_Drop::アイテム].種類 == ItemType::軽鎧 ||
						Item::data[W_Drag_Drop::アイテム].種類 == ItemType::重鎧 ||
						Item::data[W_Drag_Drop::アイテム].種類 == ItemType::力鎧 ||
						Item::data[W_Drag_Drop::アイテム].種類 == ItemType::技鎧 ||
						Item::data[W_Drag_Drop::アイテム].種類 == ItemType::知鎧 )
					{
						部位 = 1;
					}					

					Guild::P->装備所持数[ギルメン->装備[部位]]++;
					Guild::P->装備所持数[W_Drag_Drop::アイテム]--;
					ギルメン->装備[部位] = W_Drag_Drop::アイテム;
					ギルメン->装備スキル更新();

					所属->基礎ステ再計算();
					MSound::効果音[SE::装備変更].Play();
				}
				else if (W_Drag_Drop::ダンジョン != nullptr) 
				{ 
					所属->探索先 = W_Drag_Drop::ダンジョン;
					MSound::効果音[SE::配置換え].Play();				
				}
				else if (W_Drag_Drop::ギルメン装備.メンバー != nullptr && ギルメン != nullptr)
				{
					int n = W_Drag_Drop::ギルメン装備.部位;
					int buf = W_Drag_Drop::ギルメン装備.メンバー->装備[n];
					W_Drag_Drop::ギルメン装備.メンバー->装備[n] = ギルメン->装備[n];
					ギルメン->装備[n] = buf;

					ギルメン->装備スキル更新();
					W_Drag_Drop::ギルメン装備.メンバー->装備スキル更新();

					for (auto& it : Guild::P->探索パーティ)
					{
						it.基礎ステ再計算();
					}

					MSound::効果音[SE::装備変更].Play();
				}

			}

			void Info派生(Point 座標) override
			{
				if (所属->探索状態 != ExplorerType::編成中) { return; }
				if (ギルメン == nullptr) { return; }

				Point 補正座標;
				補正座標.x = 座標.x - 親ウィンドウ->相対座標.x - 位置.x;
				補正座標.y = 座標.y - 親ウィンドウ->相対座標.y - 位置.y;

				if (補正座標.Hit(&Rect(Lp(43), Lp(46), Lp(48), Lp(49))) )
				{
					//武器
					InfoItem(ギルメン->装備[0], 座標);
				}
				else if (補正座標.Hit(&Rect(Lp(44), Lp(46), Lp(48), Lp(49))))
				{
					//防具
					InfoItem(ギルメン->装備[1], 座標);
				}
				else if (補正座標.Hit(&Rect(Lp(45), Lp(47), Lp(48), Lp(49))))
				{
					//アクセ
					SetHelp("アクセサリー(未実装)");
					InfoMessage(座標);
				}
				else if (補正座標.Hit(&Rect(Lp(55), Lp(56), Lp(57), Lp(58))))
				{
					//Auto
					SetHelp("装備自動更新のON/OFF");
					InfoMessage(座標);
				}
				else
				{
					//ギルメン情報
					InfoHunter(ギルメン, 座標);
				}
			}
		};

		class GUI_パーティ : public GUI_Object
		{
		public:
			int パーティID;
			Guild::Party* 所属;
			W_Party* 親ウィンドウ;


			void Init(int パーティID, W_Party* 親ウィンドウ)
			{
				this->パーティID = パーティID;
				所属 = &Guild::P->探索パーティ[パーティID];
				this->親ウィンドウ = 親ウィンドウ;
			}

			void Draw派生(double px, double py)
			{
				MSystem::DrawWindow({ px,py }, (int)位置.GetW(), (int)位置.GetH(), 12);
				
				if (所属->探索状態 == ExplorerType::編成中){ return; }

				if (所属->探索状態 == ExplorerType::リザルト中)
				{
					Draw背景(px, py);
					Drawリザルト(px, py);
					return;
				}

				Screen::SetBright(Color(所属->暗転, 所属->暗転, 所属->暗転));

				//探索中のダンジョン背景
				Draw背景(px, py);

				//パーティメンバー
				for(int a = 0; a < CV::パーティ人数; a++)
				{
					if (所属->メンバー[a] == nullptr) { continue; }

					Drawギルメン(所属->メンバー[a], px + Lp(0) - Lp(1) * a, py + Lp(2 + a));
				}

				if (所属->発見物 != nullptr)
				{
					所属->発見物->DrawRotate({ Lp(14) + px + 所属->発見物X座標 ,Lp(15) + py }, 2, 0);
				}

				//探索状態により描画処理
				switch( 所属->探索状態 )
				{
					case ExplorerType::移動中://パーティメンバーのみ、歩くモーション
						break;
					case ExplorerType::収集中://地図、宝箱、素材に対して収集アニメーション
						break;
					case ExplorerType::全滅中:
					case ExplorerType::戦闘中:
					case ExplorerType::撤退中:
						//敵
						for (int a = 0; a < (int)所属->魔物.size(); a++)
						{
							Draw敵(所属->魔物[a], px - Lp(0) + Lp(8) + Lp(1) * a, py + Lp(2 + a));
						}
						break;
				}

				Screen::SetBright(Color::White);

				MSystem::DrawWindow({ px - 10 ,py - 10 }, 10 , (int)位置.GetH() + 20 , 11);

			}

			void Draw背景(double px, double py)
			{
				//背景スクロール描画
				const int 手前W = 2, 奥W = 2;

				int w幅 = MSystem::ダンジョン背景[1].GetWidth() - 所属->移動量 + 手前W + 奥W;


				if (w幅 <= 0 ) 所属->移動量 = 0;

				if (w幅 < 位置.GetW() - 手前W - 奥W)
				{
					MSystem::ダンジョン背景[1].DrawPart({ px + 手前W + w幅 , py + 2 }, { 0    , 0 , 位置.GetW() - w幅 - 手前W - 奥W ,位置.GetH() - 4 });
					MSystem::ダンジョン背景[1].DrawPart({ px + 手前W       , py + 2 }, { 所属->移動量, 0 , w幅 , 位置.GetH() - 4 });
				} else {
					//一枚目だけ
					MSystem::ダンジョン背景[1].DrawPart({ px + 手前W , py + 2 }, { 所属->移動量, 0 , 位置.GetW() - 手前W - 奥W ,位置.GetH() - 4 });
				}
				
				//各種表示
				switch (所属->探索先->部屋[所属->部屋ID].種類)
				{
				case RoomType::ボス:
					break;
				}
			}

			void Drawギルメン(Warker* it,double px, double py)
			{
				int 向き = 0;
				double サイズ = 2;

				switch (所属->探索状態)
				{
				case ExplorerType::移動中://パーティメンバーのみ、歩くモーション
					向き = 9 + (Game::時間 / 5 % 4);
					if (向き == 12) { 向き = 11; }
					break;
				case ExplorerType::リザルト中://枠表示、LvUP、探索成果、活躍表示
					向き = 9;
					break;
				case ExplorerType::戦闘中://木 or 岩に対して収集アニメーション
				case ExplorerType::収集中://木 or 岩に対して収集アニメーション
					向き = 9;
					break;
				case ExplorerType::全滅中://全滅表示
					向き = 4;
					break;
				case ExplorerType::撤退中://モンスターそのままで後ろ向きにギルメン逃走
					向き = 6 + (Game::時間 / 3 % 4);
					if (向き == 9) { 向き = 8; }
					break;
				}

				//→向き
				double 角度 = 0;
				if (it->現在HP <= 0)
				{
					角度 = PAI / 2;
					向き = 4;
				}


				MUnit::ユニット[it->見た目][向き]->DrawRotate({ px + (int)it->E座標,py }, サイズ, 角度);
				if (it->E光強さ > 0)
				{
					Screen::SetBlendMode(BlendMode::Add, int(it->E光強さ * 255));
					MUnit::ユニット[it->見た目][向き]->SetColor(it->E光色);
					MUnit::ユニット[it->見た目][向き]->DrawRotate({ px + (int)it->E座標,py }, サイズ, 角度);
					MUnit::ユニット[it->見た目][向き]->SetColor(Color::White);
					Screen::SetBlendMode();
				}

				if (所属->探索状態 == ExplorerType::撤退中) { return; }

				//ライフバー
				int バー幅 = int(Lp(12) * サイズ);

				MSystem::DrawBar({ px + Lp(10) - バー幅 / 2,py + Lp(11) }, バー幅, Lp(13), (double)it->現在HP / it->最大HP, 1, Color::Blue, Color::White, Color::White, true);

				if ( it->Eダメージ時間 > 0)
				{
					if (it->Eダメージ == 0)
					{
						//0ならmiss
						MFont::BSSize.DrawBold({ px-15 , py }, Color::White, Color::Black, "miss");
					} else if(it->Eダメージ > 0) {
						//+はダメージ
						MFont::BSSize.DrawBold({ px , py }, Color::White, Color::Black, it->Eダメージ);
					} else if(it->Eダメージ < 0){
						//-は回復
						MFont::BSSize.DrawBold({ px , py }, Color(128,255,128), Color::Black, -it->Eダメージ);
					}
				}

			}

			void Draw敵(Monster& it, double px, double py)
			{					
				//←向き
				if (it.現在HP <= 0) { return; }
				UnitImageType 見た目 = it.種族->見た目;
				int 向き = 7;
				double サイズ = 2;

				if (it.isボス)
				{
					サイズ = 4;
					px += Lp(16);
					py += Lp(17);
				}


				MUnit::ユニット[見た目][向き]->DrawRotate({ px - (int)it.E座標,py }, サイズ, 0);
				if (it.E光強さ > 0)
				{
					Screen::SetBlendMode(BlendMode::Add, int(it.E光強さ * 255));
					MUnit::ユニット[見た目][向き]->SetColor(it.E光色);
					MUnit::ユニット[見た目][向き]->DrawRotate({ px - (int)it.E座標,py }, サイズ, 0);
					MUnit::ユニット[見た目][向き]->SetColor(Color::White);
					Screen::SetBlendMode();
				}

				int バー幅 = int(Lp(12) * サイズ);
				int ボスY = (it.isボス) ? Lp(18) : 0;
				//ライフバー
				MSystem::DrawBar({ px + Lp(10) - バー幅/2 ,py + Lp(11) + ボスY }, バー幅 , Lp(13), (double)it.現在HP / it.最大HP, 1, Color::Blue, Color::White, Color::White, true);

				if (it.Eダメージ時間 > 0)
				{
					if (it.Eダメージ == 0)
					{
						//0ならmiss
						MFont::BSSize.DrawBold({ px - 15 , py }, Color::White, Color::Black, "miss");
					}
					else if (it.Eダメージ > 0)
					{
						//+はダメージ
						MFont::BSSize.DrawBold({ px , py }, Color::White, Color::Black, it.Eダメージ);
					}
					else if (it.Eダメージ < 0)
					{
						//-は回復
						MFont::BSSize.DrawBold({ px , py }, Color(128, 255, 128), Color::Black, -it.Eダメージ);
					}
				}

			}

			int itemX(int cnt)
			{
				return Lp(29) + (cnt % Lp(32)) * Lp(31);
			}

			int itemY(int cnt)
			{
				return Lp(30) + (cnt / Lp(32)) * Lp(31);
			}

			void Drawリザルト(double px, double py)
			{
				//Lp(18)から
				//リザルト枠
				Screen::SetBlendMode(BlendMode::Alpha, 128);
				MSystem::DrawWindow({ px + Lp(19) ,py + Lp(19) }, (int)位置.GetW() - Lp(19)*2 - Lp(33), (int)位置.GetH() - Lp(19)*2, 1);
				Screen::SetBlendMode();

				//味方5人
				for (int a = 0; a < CV::パーティ人数;a++)
				{
					auto it = 所属->メンバー[a];
					if ( it == nullptr) { continue; }
					//キャラ
					MUnit::ユニット[it->見た目][2]->DrawRotate({ px + Lp(20) + Lp(21) * a , py + Lp(22) }, 2, 0);
					//経験値バー-獲得分の表示
					int バー幅 = Lp(23);
					double 経験値割合 = it->経験値 / it->Get要求経験値();
					MSystem::DrawBar({ px + Lp(24) - バー幅 / 2 + Lp(21) * a ,py + Lp(25) }, バー幅, Lp(26), 経験値割合, 1, Color::Blue, Color::White, Color::White, true);

					//if ( !it->isレベルアップ演出)
					//{
					//	経験値割合 = (double)it->探検前経験値 / it->Get要求経験値();
					//	MSystem::DrawBar({ px + Lp(94) - バー幅 / 2 + Lp(91) * a + 1 ,py + Lp(95)+1 }, バー幅-2, Lp(96), 経験値割合, 0, Color::Blue, Color::White, Color(0,0,0,0), true);
					//}

					//LvUPとスキル習得の文字
					if (it->isスキル習得演出)
					{
						MFont::BSSize.DrawBold({ px + Lp(27) + Lp(21) * a  , py + Lp(28) }, Color::White, Color::Black, "New Skill");
					} else if (it->isレベルアップ演出) {
						MFont::BSSize.DrawBold({ px + Lp(27) + Lp(21) * a  , py + Lp(28) }, Color::White, Color::Black, "  Lv UP");
					}
				}
				int cnt = 0;

				//ボス
				for(int a = 0; a < 所属->撃破ボス数; a++)
				{
					MIcon::アイコン[IconType::ボス].DrawRotate({ px + itemX(cnt) ,py + itemY(cnt) }, 2, 0);
					cnt++;
				}
				//地図
				for (int a = 0; a < 所属->獲得地図数; a++)
				{
					MIcon::アイコン[IconType::地図].DrawRotate({ px + itemX(cnt) ,py + itemY(cnt) }, 2, 0);
					cnt++;
				}
				//宝箱
				for (int a = 0; a < 所属->獲得財宝数; a++)
				{
					MIcon::アイコン[IconType::宝箱].DrawRotate({ px + itemX(cnt) ,py + itemY(cnt) }, 2, 0);
					cnt++;
				}
				//素材

				for (int a = 0; a < CV::最大素材種; a++)
				{
					if (所属->獲得素材[a] <= 0) { continue; }

					auto& it = Material::data[a];

					MIcon::アイコン[it.アイコン].DrawRotate({ px + itemX(cnt) ,py + itemY(cnt) }, 1, 0);
					//Lv表示
					MFont::BSSize.DrawBold({ px + itemX(cnt) + Lp(34) ,py + itemY(cnt) + Lp(35) }, Color::White, Color::Black, { "Lv " , it.Lv });

					//素材数表示
					MFont::BSSize.DrawBold({ px + itemX(cnt) + Lp(36) ,py + itemY(cnt) + Lp(37) }, Color::White, Color::Black, { "x", 所属->獲得素材[a] });
					cnt++;					
				}
			}

			void Click(double px, double py)
			{
				if (所属->探索状態 == ExplorerType::リザルト中)
				{
					所属->探索状態 = ExplorerType::編成中;
				}
			}

			void Drop(double px, double py)
			{
				if (所属->探索状態 != ExplorerType::編成中 && 所属->探索状態 != ExplorerType::リザルト中)
				{
					所属->探索先予約 = W_Drag_Drop::ダンジョン;
					MSound::効果音[SE::配置換え].Play();
					return;
				}
				if (W_Drag_Drop::ダンジョン == nullptr) { return; }

				所属->探索先 = W_Drag_Drop::ダンジョン;
				MSound::効果音[SE::配置換え].Play();
			}

			void Info派生(Point 座標) override
			{
				//探索先情報
				if (所属->探索状態 != ExplorerType::編成中)
				{
					InfoResult(所属,座標);
					return;
				}

				SetHelp("ダンジョンドラッグ＆ドロップで探索先変更\nギルメンドラッグ＆ドロップで編成変更\n方針ボタンで探索方針変更");
				Info座標補正(座標);
				MSystem::DrawWindow({ 座標.x , 座標.y }, ヘルプ横幅, ヘルプ縦幅, 4);
				MFont::MSize.DrawBold({ 座標.x + 10,座標.y + 10 }, Color::White, Color::Black, ヘルプメッセージ);
			}

		};

		class GUI_控え枠 : public GUI_Object
		{
		public:
			void Draw派生(double px, double py)
			{
				MSystem::DrawWindow({ px,py }, (int)位置.GetW(), (int)位置.GetH(), 12);
			}

			void Drop(double px, double py)
			{
				if (W_Drag_Drop::探索メン != nullptr)
				{
					//ギルメン入れ替え
					W_Drag_Drop::パーティ移動(nullptr, -100);
				}
			}

		};

		class GUI_控え : public GUI_Object
		{
		public:
			int ID;
			Warker* ギルメン;

			void Draw派生(double px, double py)
			{
				//未配置時の表示
				if (ギルメン == nullptr)
				{
					//MSystem::DrawWindow({ px,py }, (int)位置.GetW(), (int)位置.GetH(), 4);
					return;
				}

				MSystem::DrawWindow({ px,py }, (int)位置.GetW(), (int)位置.GetH(), 1);

				//アイコン、Lv
				MFont::BSSize.DrawBold({ px + Lp(84) ,py + Lp(85) }, Color::White, Color::Black, { "Lv " ,ギルメン->Lv }, true);
				MUnit::ユニット[ギルメン->見た目][1]->DrawRotate({ px + (int)位置.GetW()/2 ,py + Lp(86) }, 2, 0);
			}

			void Click(double px, double py)
			{
				if (ギルメン == nullptr) { return; }

				//ギルメン掴む
				W_Drag_Drop::探索メン = ギルメン;
				W_Drag_Drop::並びID = -ID - 1;
				MSound::効果音[SE::ドラッグ].Play();
			}

			void Drop(double px, double py)
			{
				if (ギルメン == nullptr) { return; }

				if (W_Drag_Drop::探索メン != nullptr)
				{
					//ギルメン入れ替え
					W_Drag_Drop::パーティ移動(ギルメン, -ID-1);
				}
				else if (W_Drag_Drop::アイテム != -1)
				{
					//装備変更
					int 部位 = 0;
					if (Item::data[W_Drag_Drop::アイテム].種類 == ItemType::アクセサリー)
					{
						部位 = 2;
					}
					else if (Item::data[W_Drag_Drop::アイテム].種類 == ItemType::隠鎧 ||
						Item::data[W_Drag_Drop::アイテム].種類 == ItemType::軽鎧 ||
						Item::data[W_Drag_Drop::アイテム].種類 == ItemType::重鎧 ||
						Item::data[W_Drag_Drop::アイテム].種類 == ItemType::力鎧 ||
						Item::data[W_Drag_Drop::アイテム].種類 == ItemType::技鎧 ||
						Item::data[W_Drag_Drop::アイテム].種類 == ItemType::知鎧)
					{
						部位 = 1;
					}

					Guild::P->装備所持数[ギルメン->装備[部位]]++;
					Guild::P->装備所持数[W_Drag_Drop::アイテム]--;
					ギルメン->装備[部位] = W_Drag_Drop::アイテム;
					ギルメン->装備スキル更新();
					ギルメン->基礎ステータス計算();
					MSound::効果音[SE::装備変更].Play();
				}
				else if (W_Drag_Drop::ギルメン装備.メンバー != nullptr )
				{
					int n = W_Drag_Drop::ギルメン装備.部位;
					int buf = W_Drag_Drop::ギルメン装備.メンバー->装備[n];
					W_Drag_Drop::ギルメン装備.メンバー->装備[n] = ギルメン->装備[n];
					ギルメン->装備[n] = buf;

					ギルメン->装備スキル更新();
					W_Drag_Drop::ギルメン装備.メンバー->装備スキル更新();

					for (auto& it : Guild::P->探索パーティ)
					{
						it.基礎ステ再計算();
					}

					MSound::効果音[SE::装備変更].Play();
				}

			}

			void Info派生(Point 座標) override
			{
				if (ギルメン == nullptr) { return; }

				//ギルメン情報
				InfoHunter(ギルメン, 座標);
			}
		};

	public:
		//パーティ
		W_Skilltree スキルツリー;

		GUI_パーティ パーティ[CV::最大パーティ数];
		GUI_探索先 探索先[CV::最大パーティ数];
		GUI_パーティメンバー パーティメンバー[CV::最大パーティ数 * CV::パーティ人数];
		GUI_控え 控え[CV::最大控え人数];
		GUI_控え枠 控え枠;

		void init()
		{
			種類 = WindowType::Party;
			名前 = TX::Window_名前[種類];
			略記 = TX::Window_略記[種類];
			SetHelp(TX::Window_ヘルプ[種類]);

			アイコン = IconType::編成;
			横幅 = 550;
			縦幅 = 125;
			最小縦 = 125;
			最大縦 = 800;
			縦内部幅 = 800;//120 x ランク数
			スクロール位置 = 0;

			csv_page = 8;

			スキルツリー.init();

			for (int a = 0; a < Guild::P->最大パーティ数 * CV::パーティ人数; a++)
			{
				gui_objects.push_back(&パーティメンバー[a]);
				パーティメンバー[a].親ウィンドウ = this;
				パーティメンバー[a].所属 = &Guild::P->探索パーティ[a/5];
				パーティメンバー[a].並びID = a;
				パーティメンバー[a].csv_page = 8;
				パーティメンバー[a].スキルツリー = &スキルツリー;
			}

			for (int a = 0; a < Guild::P->最大パーティ数; a++)
			{
				gui_objects.push_back(&探索先[a]);
				探索先[a].csv_page = 8;
			}

			for (int a = 0; a < Guild::P->最大パーティ数; a++)
			{
				gui_objects.push_back(&パーティ[a]);
				パーティ[a].csv_page = 22;
			}

			for (int a = 0; a < CV::最大控え人数; a++)
			{
				gui_objects.push_back(&控え[a]);
				控え[a].ID = a;
				控え[a].csv_page = 8;
			}

			gui_objects.push_back(&控え枠);
			控え枠.csv_page = 8;

			GUI_Init();
		}

		void GUI_Init()
		{
			//オブジェクト初期化
			int cnt = 0;

			for (int a = 0; a < CV::最大パーティ数; a++)
			{
				パーティ[a].位置 = { Lp(0) , Lp(1) + (Lp(3) + Lp(4)) * a , Lp(2) , Lp(3) };
				探索先[a].位置 = { Lp(10) , Lp(6) + (Lp(3) + Lp(4)) * a , Lp(11) , Lp(8) };

				パーティ[a].Init(a, this);
				探索先[a].Init(a, this);

				for (int b = 0; b < CV::パーティ人数; b++)
				{
					int no = a * CV::パーティ人数 + b;
					パーティメンバー[no].ギルメン = Guild::P->探索パーティ[a].メンバー[b];
					パーティメンバー[no].位置 = { Lp(5) - (Lp(7) + Lp(9)) * b , Lp(6) + (Lp(3) + Lp(4)) * a , Lp(7) , Lp(8) };

					if (a < Guild::P->最大パーティ数 && パーティメンバー[no].所属->探索状態 == ExplorerType::編成中)
					{
						パーティメンバー[no].is表示 = true;
					}
					else {
						パーティメンバー[no].is表示 = false;
					}
				}
			}

			for (int a = 0; a < CV::最大控え人数; a++)
			{
				if (a < Guild::P->ギルメン控え.size())
				{
					控え[a].ギルメン = Guild::P->ギルメン控え[a];
					控え[a].位置 = { Lp(77) + Lp(81) * (a % Lp(83) ) , Lp(82) * (a / Lp(83)) + Lp(78) + Guild::P->最大パーティ数 * Lp(62) , Lp(79) , Lp(80) };
					控え[a].is表示 = true;
				} else {
					控え[a].ギルメン = nullptr;
					控え[a].is表示 = false;
				}
			}

			控え枠.位置 = { Lp(0) , Lp(61) + Guild::P->最大パーティ数 * Lp(62) , Lp(2) , Lp(63) };
		}

		void 派生Draw()
		{
			GUI_Init();

			for (int a = 0; a < Guild::P->最大パーティ数; a++)
			{
				パーティ[a].Draw();
				探索先[a].Draw();
			}

			for (auto& it : パーティメンバー)
			{
				if (it.is表示 && it.所属 != nullptr && it.所属->探索状態 == ExplorerType::編成中) { it.Draw(); }
			}

			控え枠.Draw();

			for (auto& it : 控え)
			{
				it.Draw();
			}
		}
	};
}