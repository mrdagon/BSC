﻿//Copyright © 2017 (´･@･)
//[License]GNU Affero General Public License, version 3
//[Contact]http://tacoika.blog87.fc2.com/
#pragma once
namespace SDX_BSC
{
	using namespace SDX;
	//各種列挙型
	//商品種
	//従業員職種
	//経営戦術系統
	//実績


	//ユニット画像
	enum class UnitImageType
	{
		ゼラルー,
		ゴブリン,
		オーガ,
		歩兵,
		竜王,
		重装兵,
		機動兵,
		僧兵,
		魔法兵,
		妖精,
		忍者,
		くノ一,
		女エルフ,
		男エルフ,
		魔族,
		格闘家,
		傭兵,
		魔貴族,
		斧兵,
		勇者,
		王子,
		王,
		王女,
		老兵,
		主人公,
		エルフ王,
		騎士,
		司祭,
		骸骨,
		海人,
		ゴーレム,
		獣人,
		ゴブリン王,
		竜,
		悪魔,
		樹人,
		怪鳥,
		COUNT
	};

	//アイテム画像
	enum class SEType
	{
		COUNT
	};

	//経営戦術
	enum class TacticsType
	{
		空き,//"――――"表示
		全軍前進,
		全軍後退,
		全軍防御,
		全軍突撃,
		全軍退却,
		前列前進,
		後列前進,
		速攻前進,
		強撃前進,
		上翼攻撃,
		下翼攻撃,
		全軍前進･強,
		全軍休息,
		速攻後退,
		後退攻撃,
		後退防御,
		前列防御,
		後列後退,
		前列死守,
		全軍防御･強,
		前列突撃,
		後列突撃,
		上翼突撃,
		下翼突撃,
		指揮官突撃,
		前列交代,
		速攻交代,
		統率回復･弱,
		統率回復･強,
		陣形回復,
		陣形回復･速,
		速攻退却,
		死んだフリ,
		雨ごい,
		冬将軍,
		分断作戦,
		爆裂部隊,
		火攻め,
		おとり作戦,
		穴掘り作戦,
		防柵作戦,
		水際作戦,
		背火の計,
		戦場の風,
		情報操作L1,
		情報操作L2,
		情報操作L3,
		情報操作L4,
		情報操作L5,
		投石部隊,//ここから敵専用
		信じる者達,
		生命回復,
		ｽﾗｲﾑﾌｨｰﾊﾞｰ,
		ナラクの風,
		大熱波,
		大あまごい,
		疾風怒涛,
		大地震,
		COUNT
	};

	//ゲームモード
	enum class GameType
	{
		ノーマル,
		ハード,
		デスマーチ,
		COUNT
	};

	//褒章
	enum class RewardType
	{
		COUNT
	};
}