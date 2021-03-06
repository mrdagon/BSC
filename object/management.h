﻿//Copyright © 2019 (´･@･)
//[License]GNU Affero General Public License, version 3
//[Contact]http://tacoika.blog87.fc2.com/
#pragma once

namespace SDX_ADE
{
	using namespace SDX;

	/*経営戦術*/
	class Management
	{
	private:
	public:
		inline static std::vector<Management> data;
		inline static const int 必要経験値[CV::最大投資Lv] =
		{
			500000,
			1000000,
			2000000,
			4000000,
			8000000,
		   15000000,
		   25000000,
		   35000000,
		   50000000,
		   70000000,
		};

		Management()
		{}

		Image* image;
		int ID;
		int ランク;//習得レベル

		std::string 名前;
		std::string 説明;

		int 消費資金;
		bool is使用済み = false;
		bool is使用可 = true;

		bool is永続;//単発効果 or 永続効果

		/*戦術実行効果*/
		void Active(Guild* guild)
		{
			//資金消費
			guild->資金 -= 消費資金;
			is使用済み = true;
			//ログ
			EventLog::Add(0, Game::日付, LogDetailType::経営戦術使用, ID);

			if (is永続) { is使用可 = false; }

			//部門Lv上昇判定
			if (!Lv上昇判定()){

				MSound::効果音[SE::投資実行].Play();
			}

			switch( MSkillType(ID) )
			{
				case MSkillType::ビラ配り:
					guild->集客力 += 5;
					break;
				case MSkillType::薄利多売:
					break;
				case MSkillType::新人発掘:
					break;
				case MSkillType::OJT:
					guild->戦闘経験補正 += 0.1;
					break;
				case MSkillType::技術研究:
					break;
				case MSkillType::低コスト化:
					guild->素材節約 += 0.1;
					break;
				case MSkillType::探索術:
					guild->未開探索 += 0.05;
					break;
				case MSkillType::探索許可証:
					if (guild->最大パーティ数 < CV::最大パーティ数){guild->最大パーティ数++;}
					break;
				case MSkillType::探索許可証2:
					if (guild->最大パーティ数 < CV::最大パーティ数) { guild->最大パーティ数++; }
					break;
			}


		}

		bool Lv上昇判定()
		{

			MSound::効果音[SE::投資実行].Play();
			return false;
		}

		static void LoadData()
		{
			File file_data("file/data/invest.dat", FileMode::Read, true);
			File file_csv("file/data/invest.csv", FileMode::Read, false);
			auto strs = file_csv.GetCsvToString2();//空の場合、Vectorのサイズが1になる

			int data_count = 0;
			file_data.Read(data_count);

			for (int i = 0; i < data_count; i++)
			{
				int dummy;

				data.emplace_back();
				auto& it = data.back();

				it.名前 = strs[i][0];
				if (strs[i].size() == 2)
				{
					it.説明 = strs[i][1];
				}

				it.ID = i;
			}
		}

		//投資状況を初期化
		static void ResetData()
		{
			for (auto& it : data)
			{
				it.is使用済み = false;
				it.is使用可 = false;
			}
		}

		static bool SaveLoad(File& ファイル, FileMode 読み書きモード)
		{
			//増加資金と使用回数
		}
	};

}