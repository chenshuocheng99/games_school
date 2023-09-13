#include "Player.h"

#include "Status.h"

#include "Log.h"
#include "Drawer.h"

#include <array>

//キャラクターフレーム座標
const Vector2 FRAME[ Define::MAX_PLAYER ] = {
	Vector2( 40, 10 ),
	Vector2( 1365, 10 ),
	Vector2( 40, 360 ),
	Vector2( 1365, 360 ),
};

//ユーザー確認ボタン配置座標
const Vector2 BUTTON[ Define::MAX_PLAYER ] = {
	Vector2( 475, 50 ),
	Vector2( 1245, 50 ),
	Vector2( 475, 400 ),
	Vector2( 1245, 400 ),
};

//画面配置微調整用座標
const Vector2 FRAME_SIZE( 20, 20 );

const Vector2 LIFE_POS( 20, 40 );
const int LIFE_PITCH = 70;

const Vector2 POWER_POS( 10, 125 );
const int POWER_PITCH = 64;

//行動予測ui座標
const Vector2 ACTION_POS( -30, 210 );

const int FARST_ENEMY_POS = 128;
const int ENEMY_PITCH = 64;

Player::Player( StatusConstPtr status )
	: _status( status ) {
	DrawerPtr drawer = Task::get<Drawer>( );

	//hpと攻撃力ui実装
	_image[ RES::ICON_LIFE ] = drawer->createImage( "resource/ui/life.png" );
	_image[ RES::ICON_LIFE_INVISIBLE ] = drawer->createImage( "resource/ui/life_invisible.png" );
	_image[ RES::ICON_POWER ] = drawer->createImage( "resource/ui/power.png" );
	_image[ RES::ICON_LIFE_GREEN ] = drawer->createImage( "resource/ui/green_buff_life.png" );
	_image[ RES::ICON_LIFE_WHITE ] = drawer->createImage( "resource/ui/white_buff_life.png" );
	_image[ RES::ICON_POWER_BLUE ] = drawer->createImage( "resource/ui/blue_buff_power.png" );
	_image[ RES::ICON_POWER_RED ] = drawer->createImage( "resource/ui/red_buff_power.png" );

	//プレイヤーフレーム実装
	_image[ RES::FRAME_R ] = drawer->createImage( "resource/ui/player_red.png" );
	_image[ RES::FRAME_G ] = drawer->createImage( "resource/ui/player_green.png" );
	_image[ RES::FRAME_B ] = drawer->createImage( "resource/ui/player_blue.png" );
	_image[ RES::FRAME_W ] = drawer->createImage( "resource/ui/player_white.png" );

	//ユーザー確認ボタン実装
	_image[ RES::BUTTON_READY ] = drawer->createImage( "resource/ui/ready_ui.png" );
	_image[ RES::BUTTON_OK ] = drawer->createImage( "resource/ui/ok_ui.png" );

	//行動予測アイコン実装
	_image[ RES::ACTION_ATTACK ] = drawer->createImage( "resource/icon/icon_roulette_attack.png" );
	_image[ RES::ACTION_EXPLORE ] = drawer->createImage( "resource/icon/icon_roulette_item.png" );
	_image[ RES::ACTION_RELEASE ] = drawer->createImage( "resource/icon/icon_roulette_elemental.png" );
	_image[ RES::ACTION_REVIVAL ] = drawer->createImage( "resource/icon/icon_roulette_heal.png" );

	//フレーム内の敵キャラアイコンと番号実装
	_image_icon_enemy[ Define::ENEMY_TYPE_GOBLIN ] = drawer->createImage( "resource/icon/icon_goblin.png" );
	_image_icon_enemy[ Define::ENEMY_TYPE_ZOMBIE ] = drawer->createImage( "resource/icon/icon_zombie.png" );
	_image_icon_enemy[ Define::ENEMY_TYPE_MINOTAUR ] = drawer->createImage( "resource/icon/icon_minotaur.png" );
	_image_icon_enemy[ Define::ENEMY_TYPE_BOSS ] = drawer->createImage( "resource/icon/icon_boss.png" );
	_image_number_enemy[ 0 ] = drawer->createImage( "resource/icon/icon_A.png" );
	_image_number_enemy[ 1 ] = drawer->createImage( "resource/icon/icon_B.png" );
	_image_number_enemy[ 2 ] = drawer->createImage( "resource/icon/icon_C.png" );
	_image_number_enemy[ 3 ] = drawer->createImage( "resource/icon/icon_D.png" );
	_image_number_enemy[ 4 ] = drawer->createImage( "resource/icon/icon_E.png" );
	_image_number_enemy[ 5 ] = drawer->createImage( "resource/icon/icon_F.png" );
	_image_number_enemy[ 6 ] = drawer->createImage( "resource/icon/icon_G.png" );
	_image_number_enemy[ 7 ] = drawer->createImage( "resource/icon/icon_h.png" );
	_image_number_enemy[ 8 ] = drawer->createImage( "resource/icon/icon_i.png" );
	_image_number_enemy[ 9 ] = drawer->createImage( "resource/icon/icon_j.png" );
	_image_number_enemy[ 10 ] = drawer->createImage( "resource/icon/icon_k.png" );
	_image_number_enemy[ 11 ] = drawer->createImage( "resource/icon/icon_l.png" );
	_image_number_enemy[ 12 ] = drawer->createImage( "resource/icon/icon_m.png" );
	_image_number_enemy[ 13 ] = drawer->createImage( "resource/icon/icon_n.png" );
	_image_number_enemy[ 14 ] = drawer->createImage( "resource/icon/icon_o.png" );
	_image_number_enemy[ 15 ] = drawer->createImage( "resource/icon/icon_p.png" );
	_image_number_enemy[ 16 ] = drawer->createImage( "resource/icon/icon_q.png" );
	_image_number_enemy[ 17 ] = drawer->createImage( "resource/icon/icon_r.png" );
	_image_number_enemy[ 18 ] = drawer->createImage( "resource/icon/icon_s.png" );
	_image_number_enemy[ 19 ] = drawer->createImage( "resource/icon/icon_t.png" );

	//シーン切り替え時のアニメーション用座標
	_hide = Vector2( 1200, 0 );
}

Player::~Player( ) {

}

void Player::draw( )const {
	for ( int i = 0; i < Define::MAX_PLAYER; i++ ) {
		draw( i );
		drawButton( i );
	}
}

void Player::show( ) {
	if ( _hide.x > 0 ) {
		_hide.x -= 10;
	}
}

void Player::hide( ) {
	if ( _hide.x <= 1200 ) {
		_hide.x += 10;
	}
}

void Player::drawBuffIcon( int player_idx, int buff_idx )const {
	//プレイヤーバブアイコン表示
	Status::Player player = _status->getPlayer( player_idx );
	Vector2 hide_pos;

	if ( player_idx == Define::PLAYER_IDX_R || player_idx == Define::PLAYER_IDX_B ) {
		hide_pos.x = _hide.x * -1;
		hide_pos.y = _hide.y;
	}

	if ( player_idx == Define::PLAYER_IDX_G || player_idx == Define::PLAYER_IDX_W ) {
		hide_pos.x = _hide.x;
		hide_pos.y = _hide.y;
	}
	PHASE phase_type = _status->getPhaseType( );
	switch ( buff_idx ) {
	case Define::PLAYER_IDX_G:
		if ( player.max_life > player.life && phase_type == PHASE::PLACE ) {
			int life_empty = player.max_life - player.life;
			for ( int i = 0; i < life_empty; i++ ) {
				_image.at( RES::ICON_LIFE_GREEN )->draw( FRAME[ player_idx ] + LIFE_POS + Vector2( LIFE_PITCH * ( player.life + i ), 0 ) + hide_pos );
			}
		}
		break;
	case Define::PLAYER_IDX_W:
		for ( int i = 0; i < player.life; i++ ) {
			_image.at( RES::ICON_LIFE_WHITE )->draw( FRAME[ player_idx ] + LIFE_POS + Vector2( LIFE_PITCH * i - 10, -12 ) + hide_pos );
		}
		break;
	case Define::PLAYER_IDX_R:
		for ( int i = 0; i < Define::BUFF_R; i++ ) {
			_image.at( RES::ICON_POWER_RED )->draw( FRAME[ player_idx ] + POWER_POS + Vector2( POWER_PITCH * ( i + player.power ), 0 ) + hide_pos );
		}
		if ( _status->isReceiveExtraBuff( player_idx ) ) {
			for ( int i = 0; i < Define::BUFF_R_WHEN_RELEASED; i++ ) {
				_image.at( RES::ICON_POWER_RED )->draw( FRAME[ player_idx ] + POWER_POS + Vector2( POWER_PITCH * ( i + player.power + Define::BUFF_R ), 0 ) + hide_pos );
			}
		}
		break;
	case Define::PLAYER_IDX_B:
		for ( int i = 0; i < Define::BUFF_B; i++ ) {
			_image.at( RES::ICON_POWER_BLUE )->draw( FRAME[ player_idx ] + POWER_POS + Vector2( POWER_PITCH * ( i + player.power ), 0 ) + hide_pos );
		}
		break;
	}
}

void Player::draw( int player_idx ) const {
	// frame内容描画
	RES res_frame;
	switch ( player_idx ) {
	case Define::PLAYER_IDX_R:
		res_frame = RES::FRAME_R;
		break;
	case Define::PLAYER_IDX_G:
		res_frame = RES::FRAME_G;
		break;
	case Define::PLAYER_IDX_B:
		res_frame = RES::FRAME_B;
		break;
	case Define::PLAYER_IDX_W:
		res_frame = RES::FRAME_W;
		break;
	}
	if ( player_idx == Define::PLAYER_IDX_R || player_idx == Define::PLAYER_IDX_B ) {
		_image.at( res_frame )->draw( FRAME[ player_idx ] - _hide );
	}
	if ( player_idx == Define::PLAYER_IDX_G || player_idx == Define::PLAYER_IDX_W ) {
		_image.at( res_frame )->draw( FRAME[ player_idx ] + _hide );
	}

	Status::Player player = _status->getPlayer( player_idx );

	//Life
	for ( int i = 0; i < player.max_life; i++ ) {
		Vector2 hide_pos;
		if ( player_idx == Define::PLAYER_IDX_R || player_idx == Define::PLAYER_IDX_B ) {
			hide_pos.x = _hide.x * -1;
			hide_pos.y = _hide.y;
		}
		if ( player_idx == Define::PLAYER_IDX_G || player_idx == Define::PLAYER_IDX_W ) {
			hide_pos.x = _hide.x;
			hide_pos.y = _hide.y;
		}
		if ( i < player.life ) {
			_image.at( RES::ICON_LIFE )->draw( FRAME[ player_idx ] + LIFE_POS + Vector2( LIFE_PITCH * i, 0 ) + hide_pos );
		} else {
			_image.at( RES::ICON_LIFE_INVISIBLE )->draw( FRAME[ player_idx ] + LIFE_POS + Vector2( LIFE_PITCH * i, 0 ) + hide_pos );
		}
	}

	//Power
	for ( int i = 0; i < player.power; i++ ) {
		Vector2 hide_pos;
		if ( player_idx == Define::PLAYER_IDX_R || player_idx == Define::PLAYER_IDX_B ) {
			hide_pos.x = _hide.x * -1;
			hide_pos.y = _hide.y;
		}
		if ( player_idx == Define::PLAYER_IDX_G || player_idx == Define::PLAYER_IDX_W ) {
			hide_pos.x = _hide.x;
			hide_pos.y = _hide.y;
		}
		_image.at( RES::ICON_POWER )->draw( FRAME[ player_idx ] + POWER_POS + Vector2( POWER_PITCH * i, 0 ) + hide_pos );
	}

	//Buff
	for ( int j = 0; j < Define::MAX_PLAYER; j++ ) {
		if ( player.buff[ j ] ) {
			drawBuffIcon( player_idx, j );
		}
	}

	//Action
	switch ( player_idx ) {
	case 0:
		drawActionIcon( player_idx );
		break;
	case 1:
		drawActionIcon( player_idx );
		break;
	case 2:
		drawActionIcon( player_idx );
		break;
	case 3:
		drawActionIcon( player_idx );
		break;
	}
}

void Player::drawButton( int player_idx ) const {
	//行動確認ボタン描画
	const double touch = _status->getTouchRatio( player_idx );
	if ( touch == 0.0 ) {
		return;
	}

	DrawerPtr drawer = Task::get< Drawer >( );
	if ( touch == 1.0 ) {
		_image.at( RES::BUTTON_OK )->draw( BUTTON[ player_idx ] );
	} else {
		Drawer::Scope scope;
		drawer->setBlend( scope, Drawer::BLEND::ALPHA, touch * 0.5 );
		double ext = ( 1 - touch * touch ) * 50;
		_image.at( RES::BUTTON_READY )->drawExtend( BUTTON[ player_idx ] - Vector2( ext, ext ), BUTTON[ player_idx ] + Vector2( 256 + ext, 256 + ext ) );
	}
}

void Player::drawActionIcon( int player_idx ) const {
	//プレイヤー行動予測アイコン描画
	Status::Player player = _status->getPlayer( player_idx );
	Vector2 hide_pos;
	if ( player_idx == Define::PLAYER_IDX_R || player_idx == Define::PLAYER_IDX_B ) {
		hide_pos.x = _hide.x * -1;
		hide_pos.y = _hide.y;
	}
	if ( player_idx == Define::PLAYER_IDX_G || player_idx == Define::PLAYER_IDX_W ) {
		hide_pos.x = _hide.x;
		hide_pos.y = _hide.y;
	}
	switch ( player.action ) {
	case Define::ACTION_ATTACK:
		_image.at( RES::ACTION_ATTACK )->draw( FRAME[ player_idx ] + ACTION_POS + hide_pos );
		drawTargetEnemy( player_idx );
		break;
	case Define::ACTION_EXPLORE:
		_image.at( RES::ACTION_EXPLORE )->draw( FRAME[ player_idx ] + ACTION_POS + hide_pos );
		break;
	case Define::ACTION_RELEASE:
		_image.at( RES::ACTION_RELEASE )->draw( FRAME[ player_idx ] + ACTION_POS + hide_pos );
		break;
	case Define::ACTION_REVIVAL:
		_image.at( RES::ACTION_REVIVAL )->draw( FRAME[ player_idx ] + ACTION_POS + hide_pos );
		break;
	}
}

void Player::drawTargetEnemy( int player_idx ) const {
	//プレイヤー攻撃先アイコン（敵アイコン）描画
	int enemy_num = _status->getEnemyNum( );
	int count = 0;
	int line_num = 0;
	Vector2 hide_pos;
	if ( player_idx == Define::PLAYER_IDX_R || player_idx == Define::PLAYER_IDX_B ) {
		hide_pos.x = _hide.x * -1;
		hide_pos.y = _hide.y;
	}
	if ( player_idx == Define::PLAYER_IDX_G || player_idx == Define::PLAYER_IDX_W ) {
		hide_pos.x = _hide.x;
		hide_pos.y = _hide.y;
	}
	for ( int i = 0; i < enemy_num; i++ ) {
		Status::Enemy enemy = _status->getEnemy( i );
		if ( !enemy.defense[ player_idx ] ) {
			continue;
		}

		int icon_width;
		int icon_height;
		_image_icon_enemy[ enemy.type ]->getImageSize( icon_width, icon_height );
		if ( count % 7 == 0 && count > 6 ) {
			line_num++;
			count = 0;
		}
		_image_icon_enemy[ enemy.type ]->draw( FRAME[ player_idx ] + ACTION_POS + Vector2( FARST_ENEMY_POS + ENEMY_PITCH * count, 0 + ENEMY_PITCH * line_num ) + hide_pos );

		if ( enemy.type != Define::ENEMY_TYPE_BOSS ) {
			int number = enemy.number;
			if ( number >= ( int )_image_number_enemy.size( ) ) {
				number = ( int )_image_number_enemy.size( ) - 1;
			}
			int number_width;
			int number_height;
			Vector2 num_pos = Vector2( 5, 5 );
			_image_number_enemy[ number ]->getImageSize( number_width, number_height );
			_image_number_enemy[ number ]->draw( FRAME[ player_idx ] + ACTION_POS + num_pos + Vector2( FARST_ENEMY_POS + ENEMY_PITCH * count, 0 + ENEMY_PITCH * line_num ) + hide_pos );
		}
		count++;
	}
}