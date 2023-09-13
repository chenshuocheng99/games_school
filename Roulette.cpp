#include "Roulette.h"
#include "Status.h"
#include "Drawer.h"

#include "Keyboard.h"

const int ROULETTE_X = 1280 / 2;
const int ROULETTE_Y = 0;
const int ROULETTE_NUM = 5;

Roulette::Roulette( StatusConstPtr status )
	: _status( status ) {
	//ルーレット実装(ゲーム画面用ルーレットイラスト)

	DrawerPtr drawer = Task::get< Drawer >( );
	//復活
	_heal_action = drawer->createImage( "resource/roulette/roulette_revivle.png" );
	//探索
	_explore_action = drawer->createImage( "resource/roulette/roulette_search.png" );
	//攻撃
	_atk_action = drawer->createImage( "resource/roulette/roulette_attack.png" );
	//エレメント解放
	_element_action = drawer->createImage( "resource/roulette/roulette_release.png" );
	//ルーレット選択
	_action_part[ 0 ] = drawer->createImage( "resource/roulette/roulette_turn/roulette_parts1.png" );
	_action_part[ 1 ] = drawer->createImage( "resource/roulette/roulette_turn/roulette_parts2.png" );
	_action_part[ 2 ] = drawer->createImage( "resource/roulette/roulette_turn/roulette_parts3.png" );
	_action_part[ 3 ] = drawer->createImage( "resource/roulette/roulette_turn/roulette_parts4.png" );
	_action_part[ 4 ] = drawer->createImage( "resource/roulette/roulette_turn/roulette_parts5.png" );
	_action_part[ 5 ] = drawer->createImage( "resource/roulette/roulette_turn/roulette_parts6.png" );

	//フレーム種類判断フラッグ
	_is_roulette = false;
	_is_explore = false;
	_is_atk = false;
	_is_heal = false;
	_is_element = false;

	//フレームパフォーマンス動画
	_hide_y = 1000;
}

Roulette::~Roulette( ) {

}

void Roulette::hide( ) {
	if ( _hide_y < 1000 ) {
		_hide_y += 10;
	}
}

void Roulette::show( ) {
	if ( _hide_y > 0 ) {
		_hide_y -= 10;
	}
}

void Roulette::draw( ) {
	DrawerPtr drawer = Task::get< Drawer >( );
	Drawer::Scope scope;
	const int action_type = _status->getRouletteAction( );

	//ルーレット種類よる個別判断フラッグの設置
	switch ( action_type ) {
	case Define::ACTION_ATTACK:
		_is_atk = true;
		break;
	case Define::ACTION_EXPLORE:
		_is_explore = true;
		break;
	case Define::ACTION_RELEASE:
		_is_element = true;
		break;
	case Define::ACTION_REVIVAL:
		_is_heal = true;
		break;
	}

	//orderRoulette( );	//デバッグ用

	drawRoulette( );
	
	//結果初期化
	_is_roulette = false;
	_is_explore = false;
	_is_atk = false;
	_is_heal = false;
	_is_element = false;
}

void Roulette::drawRoulette( ) {
	//ルーレット描画
	int action = 0;
	action = _status->getRouletteNumber( );
	if ( _is_atk ) {
		_atk_action->draw( ROULETTE_X, ROULETTE_Y - _hide_y );
		_action_part[ action ]->draw( ROULETTE_X, ROULETTE_Y - _hide_y );
	}
	if ( _is_explore ) {
		_explore_action->draw( ROULETTE_X, ROULETTE_Y - _hide_y );
		_action_part[ action ]->draw( ROULETTE_X, ROULETTE_Y - _hide_y );
	}
	if ( _is_heal ) {
		_heal_action->draw( ROULETTE_X, ROULETTE_Y - _hide_y );
		_action_part[ action ]->draw( ROULETTE_X, ROULETTE_Y - _hide_y );
	}
	if ( _is_element ) {
		_element_action->draw( ROULETTE_X, ROULETTE_Y - _hide_y );
		_action_part[ action ]->draw( ROULETTE_X, ROULETTE_Y - _hide_y );
	}
}

void Roulette::orderRoulette( ) {
	//デバッグ用ルーレット機能
	KeyboardPtr keyboard = Task::get< Keyboard >( );
	if ( keyboard->isPushKey( "C" ) ) {
		_is_explore = false;
		_is_atk = false;
		_is_heal = false;
		_is_element = false;
	}
	//
}