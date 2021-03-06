﻿//Copyright © 2019 (´･@･)
//[License]GNU Affero General Public License, version 3
//[Contact]http://tacoika.blog87.fc2.com/
#pragma once

namespace SDX_ADE
{
	using namespace SDX;

	/*汎用２択ポップアップウィンドウ*/
	class W_Popup : public UIWindow
	{
	private:
	public:

		UITextFrame 文章;

		UIButton 確定;
		UIButton キャンセル;


		void Init()
		{
			Set(WindowType::Popup, IconType::情報);

			SetPos(L基本::ポップアップ_ウィンドウ, true, false, true);
			is閉じるボタン = false;
			isスクロールバー表示 = false;

			//●初期化
			文章.SetUI(L基本::ポップアップ_説明, "テキスト未設定");
			確定.SetUI(L基本::ポップアップ_はい, "はい");
			キャンセル.SetUI(L基本::ポップアップ_いいえ, "いいえ");
			//●イベント
			確定.clickEvent = [&]()
			{
				is表示 = false;
				ポップアップリザルト = 1;
			};

			キャンセル.clickEvent = [&]()
			{
				is表示 = false;
				ポップアップリザルト = 0;
			};


			//●登録
			AddItem(文章);
			AddItem(確定);
			AddItem(キャンセル);
		}

		void Init(WindowType 種類)
		{
			Init();
			SetText(種類);
		}

		void Update()
		{
			SetPos(L基本::ポップアップ_ウィンドウ, true, false, true);
		}

		void SetText( WindowType 種類 )
		{
			文章.テキスト = TX::Window_ヘルプ[種類];
		}

	};
}