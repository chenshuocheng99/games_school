#include "Text.h"
#include "Status.h"
#include "Image.h"
#include "Drawer.h"

const int FRAME_X = 0;
const int FRAME_Y = 500;
const int TEXT_X = 50;
const int TEXT_Y = 300;

//テキスト種類
enum class TYPE {
	ATK_TEXT,
	HEAL_TEXT,
	EXP_TEXT,
	ELEMENTAL_TEXT,
	TURN_TEXT,
	INIT
};

//総合的なテキスト内容
static const char* PLAYER_TYPE[ ] =
{ "勇者 赤","勇者 緑","勇者 青","勇者 白","" };

static const char* ELEMENT_EFFECT[ ] =
{ "次のターンから 勇者 赤 の バフ効果量 が上昇します。",
"次のターンから 全員 の 最大LIFE が上昇し、 LIFE が回復します。",
"次のターンから 全員 の 攻撃力 が上昇します。",
"次のターンから 全員 の 影響範囲 が拡大します。" };

Text::Text( StatusConstPtr status )
	: _status( status ) {
	DrawerPtr drawer = Task::get< Drawer >( );
	_frame_text = drawer->createImage( "resource/ui/text_flame.png" );
}

Text::~Text( ) {

}

void Text::drawText( int sx, int sy, std::string text ) const {
	DrawerPtr drawer = Task::get< Drawer >( );
	drawer->setStringSize( 40 );

	drawer->drawString( sx, sy, text.c_str( ) );
}

void Text::draw( ) {
	Status::Encounter encounter = _status->getEncounter( );
	int player_type_code = _status->getCurrentPlayerIdx( );
	int action = _status->getRouletteNumber( );
	int frame_x = FRAME_X;
	int frame_y = FRAME_Y + ( int )getHideRatio( ) * Define::SCREEN_HEIGHT / 2;
	_frame_text->draw( frame_x, frame_y );

	int text_x = frame_x + TEXT_X;
	int text_y = frame_y + TEXT_Y;

	//フェーズによってテキスト内容判断
	if ( _status->getPhaseType( ) == PHASE::OPENING ) {
		drawText( text_x, text_y, "エレメンタル ガーディアンへようこそ" );
	}
	if ( _status->getPhaseType( ) == PHASE::APPEAR ) {
		drawText( text_x, text_y, "敵が出現しています。" );
	}
	if ( _status->getPhaseType( ) == PHASE::APPEAR_ENEMIES ) {
		drawText( text_x, text_y, "敵の増援が来ます！！" );
	}
	if ( _status->getPhaseType( ) == PHASE::APPEAR_BOSS ) {
		drawText( text_x, text_y, "ボスが現れます！！" );
	}

	//敵キャラ行動テキスト
	int enemy_num = _status->getEnemyNum( );
	if ( _status->getPhaseType( ) == PHASE::ENEMY_ACTION ) {
		if ( enemy_num != 0 ) {
			if ( !_status->isItemUsed( ) ) {
				drawText( text_x, text_y, "敵が攻撃しています。" );
			}
		} else {
			drawText( text_x, text_y, "敵がいない！　エレメント解放のチャンスです！" );
		}
	}

	//アイテム使用テキスト
	if ( _status->getPhaseType( ) == PHASE::ITEM_USE ) {
		drawText( text_x, text_y, "アイテムの効果で敵は動けない！" );
	}

	//駒配置提示
	if ( _status->getPhaseType( ) == PHASE::READY_PLACE ) {
		drawText( text_x, text_y, "駒を配置する準備をしてください。" );
	}

	//ユーザー行動提示
	std::string str_player = PLAYER_TYPE[ player_type_code ];
	if ( _status->getPhaseType( ) == PHASE::ROULETTE_START ) {
		drawText( text_x, text_y, str_player + " の番です。\n手をボタンの上に「OK」になるまで置いて、ルーレットを回してください。" );
	}
	if ( _status->getPhaseType( ) == PHASE::ROULETTE_STOP ) {
		drawText( text_x, text_y, str_player + " の番です。\n手をボタンの上に「OK」になるまで置いて、ルーレットを止めてください。" );
	}

	if ( _status->getPhaseType( ) == PHASE::PASS_PLAYER ) {
		drawText( text_x, text_y, str_player + " は 行動をパスします。" );
	}

	//ユーザー行動よるのテキスト提示
	if ( _status->getPhaseType( ) == PHASE::CURRENT_PLAYER ) {
		const int action_type = _status->getRouletteAction( );
		switch ( action_type ) {
		case Define::ACTION_ATTACK:
			drawText( text_x, text_y, str_player + " が 攻撃します。" );
			break;
		case Define::ACTION_EXPLORE:
			drawText( text_x, text_y, str_player + " が 探索します。" );
			break;
		case Define::ACTION_REVIVAL:
			drawText( text_x, text_y, str_player + " が 復活します。" );
			break;
		case Define::ACTION_RELEASE:
			drawText( text_x, text_y, str_player + " が エレメントを解放します。" );
			break;
		}
	}

	//ユーザー攻撃行動テキスト
	if ( _status->getPhaseType( ) == PHASE::ATTACK_SUCCESS ) {
		if ( action == 0 ) {
			drawText( text_x, text_y,
					  str_player +
					  " は 攻撃を 当てた！ もう一度ルーレットを回せます！" );
		} else {
			drawText( text_x, text_y,
					  str_player +
					  " は 攻撃を 当てた！" );
		}
	}

	//bossに当たる時の差分表現
	if ( _status->getPhaseType( ) == PHASE::ENCOUNTER ) {
		drawText( text_x, text_y, str_player + " は 行動を 失敗した！ BOSS召喚ゲージが上昇します！" );
	}

	if ( _status->getPhaseType( ) == PHASE::SPECIAL_ATTACK ) {
		drawText( text_x, text_y, str_player + " は 行動を 失敗した！ BOSSが勇者全員に特殊攻撃をします！！" );
	}

	if ( _status->getPhaseType( ) == PHASE::ATTACK_FAILURE ) {
		drawText( text_x, text_y, str_player + " は 攻撃を はずした！！" );
	}

	//アイテム（探索）相関テキスト
	if ( _status->getPhaseType( ) == PHASE::EXPLORE_SUCCESS ) {
		if ( _status->getItemNum( ) == Define::MAX_ITEM_NUM ) {
			drawText( text_x, text_y, " アイテムの数が 上限 に達しました " );
		} else {
			drawText( text_x, text_y, str_player + " は 探索を 成功した！ アイテムを発見した！ " );
		}
	}

	if ( _status->getPhaseType( ) == PHASE::EXPLORE_FAILURE ) {
		drawText( text_x, text_y, str_player + " は 探索を 失敗した！ " );
	}

	//ユーザー復活行動テキスト
	if ( _status->getPhaseType( ) == PHASE::REVIVAL_SUCCESS ) {
		drawText( text_x, text_y, str_player + " は 復活を 成功した！  " );
	}

	if ( _status->getPhaseType( ) == PHASE::REVIVAL_FAILURE ) {
		drawText( text_x, text_y, str_player + " は 復活を 失敗した！ " );
	}

	//エレメント解放テキスト
	std::string ele_effect = ELEMENT_EFFECT[ player_type_code ];

	if ( _status->getPhaseType( ) == PHASE::RELEASE_SUCCESS ) {
		int release_count = _status->getElementalRelease( player_type_code );
		if ( release_count < 3 ) {
			if ( action == 0 ) {
				drawText( text_x, text_y,
						  str_player +
						  " は エレメント を一回解放した！ もう一度ルーレットを回せます！" );
			} else {
				drawText( text_x, text_y,
						  str_player +
						  " は エレメント を一回解放した！" );
			}
		} else {
			drawText( text_x, text_y,
					  str_player +
					  " は エレメント を完全解放しました！\n" + ele_effect );
		}
	}
}