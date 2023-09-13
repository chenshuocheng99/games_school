#include "Enemy.h"

#include "Status.h"

#include "Drawer.h"
#include "Log.h"
#include "Keyboard.h"
#include "Image.h"

#include <array>

const int FRAME_SIZE = 35;
const int MAX_LINE = 4;
const int GAUGE_SIZE = 60;

const Vector2 GAUGE = Vector2( 1550, 720 );

Enemy::Enemy( StatusConstPtr status )
	: _status( status ) {
	DrawerPtr drawer = Task::get< Drawer >( );
	_image[ TYPE::FRAME_GOBLIN ] = drawer->createImage( "resource/ui/enemy_flame_1.png" );
	_image[ TYPE::FRAME_ZOMBIE ] = drawer->createImage( "resource/ui/enemy_flame_2.png" );
	_image[ TYPE::FRAME_MINOTAUR ] = drawer->createImage( "resource/ui/enemy_flame_3.png" );
	_image[ TYPE::FRAME_BOSS ] = drawer->createImage( "resource/ui/enemy_flame_boss.png" );

	_image[ TYPE::ICON_BOSS ] = drawer->createImage( "resource/icon/icon_boss.png" );
	_image[ TYPE::ICON_MINOTAUR ] = drawer->createImage( "resource/icon/icon_minotaur.png" );
	_image[ TYPE::ICON_ZOMBIE ] = drawer->createImage( "resource/icon/icon_zombie.png" );
	_image[ TYPE::ICON_GOBLIN ] = drawer->createImage( "resource/icon/icon_goblin.png" );
	_image[ TYPE::ICON_A ] = drawer->createImage( "resource/icon/icon_a.png" );
	_image[ TYPE::ICON_B ] = drawer->createImage( "resource/icon/icon_b.png" );
	_image[ TYPE::ICON_C ] = drawer->createImage( "resource/icon/icon_c.png" );
	_image[ TYPE::ICON_D ] = drawer->createImage( "resource/icon/icon_d.png" );
	_image[ TYPE::ICON_E ] = drawer->createImage( "resource/icon/icon_e.png" );
	_image[ TYPE::ICON_F ] = drawer->createImage( "resource/icon/icon_f.png" );
	_image[ TYPE::ICON_G ] = drawer->createImage( "resource/icon/icon_g.png" );
	_image[ TYPE::ICON_H ] = drawer->createImage( "resource/icon/icon_h.png" );
	_image[ TYPE::ICON_I ] = drawer->createImage( "resource/icon/icon_i.png" );
	_image[ TYPE::ICON_J ] = drawer->createImage( "resource/icon/icon_j.png" );
	_image[ TYPE::ICON_K ] = drawer->createImage( "resource/icon/icon_k.png" );
	_image[ TYPE::ICON_L ] = drawer->createImage( "resource/icon/icon_l.png" );
	_image[ TYPE::ICON_M ] = drawer->createImage( "resource/icon/icon_m.png" );
	_image[ TYPE::ICON_N ] = drawer->createImage( "resource/icon/icon_n.png" );
	_image[ TYPE::ICON_O ] = drawer->createImage( "resource/icon/icon_o.png" );
	_image[ TYPE::ICON_P ] = drawer->createImage( "resource/icon/icon_p.png" );
	_image[ TYPE::ICON_Q ] = drawer->createImage( "resource/icon/icon_q.png" );
	_image[ TYPE::ICON_R ] = drawer->createImage( "resource/icon/icon_r.png" );
	_image[ TYPE::ICON_S ] = drawer->createImage( "resource/icon/icon_s.png" );
	_image[ TYPE::ICON_T ] = drawer->createImage( "resource/icon/icon_t.png" );
	_image[ TYPE::ICON_LIFE ] = drawer->createImage( "resource/ui/life.png" );
	_image[ TYPE::ICON_POWER ] = drawer->createImage( "resource/ui/power.png" );
	_image[ TYPE::ICON_BUFF ] = drawer->createImage( "resource/ui/enemy_buff_item.png" );
	_image[ TYPE::GAUGE_BOSS_0 ] = drawer->createImage( "resource/gauge/gauge_boss_0.png" );
	_image[ TYPE::GAUGE_BOSS_1 ] = drawer->createImage( "resource/gauge/gauge_boss_1.png" );
	_image[ TYPE::GAUGE_BOSS_2 ] = drawer->createImage( "resource/gauge/gauge_boss_2.png" );
	_image[ TYPE::GAUGE_BOSS_3 ] = drawer->createImage( "resource/gauge/gauge_boss_3.png" );
	_image[ TYPE::GAUGE_BOSS_4 ] = drawer->createImage( "resource/gauge/gauge_boss_4.png" );
	_image[ TYPE::GAUGE_BOSS_5 ] = drawer->createImage( "resource/gauge/gauge_boss_5.png" );
	_image[ TYPE::GAUGE_BOSS_6 ] = drawer->createImage( "resource/gauge/gauge_boss_6.png" );
	_image[ TYPE::GAUGE_BOSS_7 ] = drawer->createImage( "resource/gauge/gauge_boss_7.png" );
	_image[ TYPE::GAUGE_BOSS_8 ] = drawer->createImage( "resource/gauge/gauge_boss_8.png" );

	_is_display_boss_gauge = true;
}

Enemy::~Enemy( ) {

}

void Enemy::draw( ) {
	DrawerPtr drawer = Task::get< Drawer >( );
	Drawer::Scope scope;
	const int num = _status->getEnemyNum( );
	drawEnemyFrame( num );
	//ƒ{ƒXƒQ[ƒW•\Ž¦”»’f
	if ( !_status->isBossAppeared( ) ) {
		drawBossGauge( );
	}
}

void Enemy::drawBuff( int x, int y )const {
	//ƒoƒtƒAƒCƒRƒ“•\Ž¦
	int pos_x = 90;
	int pos_y = -50;
	if ( _status->isItemUsed( ) ) {
		_image.at( TYPE::ICON_BUFF )->draw( x + pos_x, ( int )( y + pos_y + getHideRatio( ) * 1000 ) );
	}
}

void Enemy::drawEnemyFrame( int enemy_num )const {
	//“GƒtƒŒ[ƒ€•\Ž¦
	int life_height = _image.at( TYPE::ICON_LIFE )->getHeight( );
	int life_y_goblin = life_height;
	int life_y_zombie = life_height;
	int life_y_minotaur = life_height * 2;
	int life_y_boss = life_height * 3;
	int add_y = ( int )( getHideRatio( ) * 1000 );
	for ( int i = 0; i < enemy_num; i++ ) {
		int enemy_idx = i;
		Status::Enemy enemy = _status->getEnemy( enemy_idx );
		if ( _status->isBossAppeared( ) ) {
			enemy_idx += 1;
		}

		//“GƒLƒƒƒ‰Ží—Þ‚É‚æ‚éŒÂ•Ê•`‰æ
		switch ( enemy.type ) {
		case Define::ENEMY_TYPE_GOBLIN:
			_image.at( TYPE::FRAME_GOBLIN )->draw( _enemy_frame_x[ enemy_idx ], _enemy_frame_y[ Define::ENEMY_TYPE_GOBLIN ] + add_y );
			_image.at( TYPE::ICON_GOBLIN )->draw( _enemy_frame_x[ enemy_idx ] + FRAME_SIZE, _enemy_frame_y[ Define::ENEMY_TYPE_GOBLIN ] + FRAME_SIZE + add_y );
			drawEnemyNumber( enemy.number, enemy_idx, Define::ENEMY_TYPE_GOBLIN );
			drawEnemyLife( enemy.life, enemy_idx, Define::ENEMY_TYPE_GOBLIN );
			drawEnemyPower( enemy.power, life_y_goblin, enemy_idx, Define::ENEMY_TYPE_GOBLIN );
			drawBuff( _enemy_frame_x[ enemy_idx ], _enemy_frame_y[ Define::ENEMY_TYPE_GOBLIN ] );
			break;
		case Define::ENEMY_TYPE_ZOMBIE:
			_image.at( TYPE::FRAME_ZOMBIE )->draw( _enemy_frame_x[ enemy_idx ], _enemy_frame_y[ Define::ENEMY_TYPE_ZOMBIE ] + add_y );
			_image.at( TYPE::ICON_ZOMBIE )->draw( _enemy_frame_x[ enemy_idx ] + FRAME_SIZE, _enemy_frame_y[ Define::ENEMY_TYPE_ZOMBIE ] + FRAME_SIZE + add_y );
			drawEnemyNumber( enemy.number, enemy_idx, Define::ENEMY_TYPE_ZOMBIE );
			drawEnemyLife( enemy.life, enemy_idx, Define::ENEMY_TYPE_ZOMBIE );
			drawEnemyPower( enemy.power, life_y_zombie, enemy_idx, Define::ENEMY_TYPE_ZOMBIE );
			drawBuff( _enemy_frame_x[ enemy_idx ], _enemy_frame_y[ Define::ENEMY_TYPE_ZOMBIE ] );
			break;
		case Define::ENEMY_TYPE_MINOTAUR:
			_image.at( TYPE::FRAME_MINOTAUR )->draw( _enemy_frame_x[ enemy_idx ], _enemy_frame_y[ Define::ENEMY_TYPE_MINOTAUR ] + add_y );
			_image.at( TYPE::ICON_MINOTAUR )->draw( _enemy_frame_x[ enemy_idx ] + FRAME_SIZE, _enemy_frame_y[ Define::ENEMY_TYPE_MINOTAUR ] + FRAME_SIZE + add_y );
			drawEnemyNumber( enemy.number, enemy_idx, Define::ENEMY_TYPE_MINOTAUR );
			drawEnemyLife( enemy.life, enemy_idx, Define::ENEMY_TYPE_MINOTAUR );
			drawEnemyPower( enemy.power, life_y_minotaur, enemy_idx, Define::ENEMY_TYPE_MINOTAUR );
			drawBuff( _enemy_frame_x[ enemy_idx ], _enemy_frame_y[ Define::ENEMY_TYPE_MINOTAUR ] );
			break;
		case Define::ENEMY_TYPE_BOSS:
			_image.at( TYPE::FRAME_BOSS )->draw( _enemy_frame_x[ 0 ], _enemy_frame_y[ Define::ENEMY_TYPE_BOSS ] + add_y );
			_image.at( TYPE::ICON_BOSS )->draw( _enemy_frame_x[ 0 ] + FRAME_SIZE, _enemy_frame_y[ Define::ENEMY_TYPE_BOSS ] + FRAME_SIZE + add_y );
			drawEnemyLife( enemy.life, 0, Define::ENEMY_TYPE_BOSS );
			drawEnemyPower( enemy.power, life_y_boss, 0, Define::ENEMY_TYPE_BOSS );
			drawBuff( _enemy_frame_x[ 0 ], _enemy_frame_y[ Define::ENEMY_TYPE_BOSS ] );
			break;
		}
	}
}

void Enemy::drawEnemyNumber( int number, int type, int fream ) const {
	//“G”Ô†•`‰æ
	int number_x = 5;
	int number_y = 5;
	int add_y = ( int )( getHideRatio( ) * 1000 );
	switch ( number ) {
	case 0:
		_image.at( TYPE::ICON_A )->draw( _enemy_frame_x[ type ] + FRAME_SIZE + number_x, _enemy_frame_y[ fream ] + FRAME_SIZE + number_y + add_y );
		break;
	case 1:
		_image.at( TYPE::ICON_B )->draw( _enemy_frame_x[ type ] + FRAME_SIZE + number_x, _enemy_frame_y[ fream ] + FRAME_SIZE + number_y + add_y );
		break;
	case 2:
		_image.at( TYPE::ICON_C )->draw( _enemy_frame_x[ type ] + FRAME_SIZE + number_x, _enemy_frame_y[ fream ] + FRAME_SIZE + number_y + add_y );
		break;
	case 3:
		_image.at( TYPE::ICON_D )->draw( _enemy_frame_x[ type ] + FRAME_SIZE + number_x, _enemy_frame_y[ fream ] + FRAME_SIZE + number_y + add_y );
		break;
	case 4:
		_image.at( TYPE::ICON_E )->draw( _enemy_frame_x[ type ] + FRAME_SIZE + number_x, _enemy_frame_y[ fream ] + FRAME_SIZE + number_y + add_y );
		break;
	case 5:
		_image.at( TYPE::ICON_F )->draw( _enemy_frame_x[ type ] + FRAME_SIZE + number_x, _enemy_frame_y[ fream ] + FRAME_SIZE + number_y + add_y );
		break;
	case 6:
		_image.at( TYPE::ICON_G )->draw( _enemy_frame_x[ type ] + FRAME_SIZE + number_x, _enemy_frame_y[ fream ] + FRAME_SIZE + number_y + add_y );
		break;
	case 7:
		_image.at( TYPE::ICON_H )->draw( _enemy_frame_x[ type ] + FRAME_SIZE + number_x, _enemy_frame_y[ fream ] + FRAME_SIZE + number_y + add_y );
		break;
	case 8:
		_image.at( TYPE::ICON_I )->draw( _enemy_frame_x[ type ] + FRAME_SIZE + number_x, _enemy_frame_y[ fream ] + FRAME_SIZE + number_y + add_y );
		break;
	case 9:
		_image.at( TYPE::ICON_J )->draw( _enemy_frame_x[ type ] + FRAME_SIZE + number_x, _enemy_frame_y[ fream ] + FRAME_SIZE + number_y + add_y );
		break;
	case 10:
		_image.at( TYPE::ICON_K )->draw( _enemy_frame_x[ type ] + FRAME_SIZE + number_x, _enemy_frame_y[ fream ] + FRAME_SIZE + number_y + add_y );
		break;
	case 11:
		_image.at( TYPE::ICON_L )->draw( _enemy_frame_x[ type ] + FRAME_SIZE + number_x, _enemy_frame_y[ fream ] + FRAME_SIZE + number_y + add_y );
		break;
	case 12:
		_image.at( TYPE::ICON_M )->draw( _enemy_frame_x[ type ] + FRAME_SIZE + number_x, _enemy_frame_y[ fream ] + FRAME_SIZE + number_y + add_y );
		break;
	case 13:
		_image.at( TYPE::ICON_N )->draw( _enemy_frame_x[ type ] + FRAME_SIZE + number_x, _enemy_frame_y[ fream ] + FRAME_SIZE + number_y + add_y );
		break;
	case 14:
		_image.at( TYPE::ICON_O )->draw( _enemy_frame_x[ type ] + FRAME_SIZE + number_x, _enemy_frame_y[ fream ] + FRAME_SIZE + number_y + add_y );
		break;
	case 15:
		_image.at( TYPE::ICON_P )->draw( _enemy_frame_x[ type ] + FRAME_SIZE + number_x, _enemy_frame_y[ fream ] + FRAME_SIZE + number_y + add_y );
		break;
	case 16:
		_image.at( TYPE::ICON_Q )->draw( _enemy_frame_x[ type ] + FRAME_SIZE + number_x, _enemy_frame_y[ fream ] + FRAME_SIZE + number_y + add_y );
		break;
	case 17:
		_image.at( TYPE::ICON_R )->draw( _enemy_frame_x[ type ] + FRAME_SIZE + number_x, _enemy_frame_y[ fream ] + FRAME_SIZE + number_y + add_y );
		break;
	case 18:
		_image.at( TYPE::ICON_S )->draw( _enemy_frame_x[ type ] + FRAME_SIZE + number_x, _enemy_frame_y[ fream ] + FRAME_SIZE + number_y + add_y );
		break;
	case 19:
		_image.at( TYPE::ICON_T )->draw( _enemy_frame_x[ type ] + FRAME_SIZE + number_x, _enemy_frame_y[ fream ] + FRAME_SIZE + number_y + add_y );
		break;
	}
}

void Enemy::drawEnemyLife( int life, int type, int fream ) const {
	//“GHP•\Ž¦
	bool is_newline = false;
	int power_width;
	int power_height;
	_image.at( TYPE::ICON_LIFE )->getImageSize( power_width, power_height );
	const int power_size_x = power_width * 4 / 5;
	int count_line = 1;
	int count_x_plus = 0;
	for ( int i = 0; i < life; i++ ) {
		count_x_plus = i;
		if ( i >= MAX_LINE && i % MAX_LINE == 0 ) {
			count_line++;
			is_newline = true;
		}
		if ( is_newline ) {
			count_x_plus = i - ( count_line - 1 ) * MAX_LINE;
		}
		_image.at( TYPE::ICON_LIFE )->draw(
			( _enemy_frame_x[ type ] + FRAME_SIZE ) + power_size_x * count_x_plus,
			_enemy_frame_y[ fream ] + FRAME_SIZE + power_height * count_line + ( int )( getHideRatio( ) * 1000 ) );
	}
}

void Enemy::drawEnemyPower( int power, int pos_y, int type, int fream ) const {
	//“GUŒ‚—Í•\Ž¦
	bool is_newline = false;
	int life_width;
	int life_height;
	_image.at( TYPE::ICON_LIFE )->getImageSize( life_width, life_height );
	const int life_size_x = life_width * 4 / 5;
	int count_line = 1;
	int count_x_plus = 0;
	for ( int i = 0; i < power; i++ ) {
		count_x_plus = i;
		if ( i >= MAX_LINE && i % MAX_LINE == 0 ) {
			count_line++;
			is_newline = true;
		}
		if ( is_newline ) {
			count_x_plus = i - ( count_line - 1 ) * MAX_LINE;
		}
		_image.at( TYPE::ICON_POWER )->draw(
			( _enemy_frame_x[ type ] + FRAME_SIZE ) + life_size_x * count_x_plus,
			_enemy_frame_y[ fream ] + FRAME_SIZE + pos_y + life_height * count_line + ( int )( getHideRatio( ) * 1000 ) );
	}
}

void Enemy::drawBossGauge( ) const {
	//ƒ{ƒXƒQ[ƒW•\Ž¦
	Vector2 gauge_full = Vector2( -40, -45 );
	Status::Encounter encounter = _status->getEncounter( );
	switch ( encounter.time ) {
	case 0:
		_image.at( TYPE::GAUGE_BOSS_0 )->draw( GAUGE + Vector2( 0.0, getHideRatio( ) * 1000 ) );
		break;
	case 1:
		_image.at( TYPE::GAUGE_BOSS_1 )->draw( GAUGE + Vector2( 0.0, getHideRatio( ) * 1000 ) );
		break;
	case 2:
		_image.at( TYPE::GAUGE_BOSS_2 )->draw( GAUGE + Vector2( 0.0, getHideRatio( ) * 1000 ) );
		break;
	case 3:
		_image.at( TYPE::GAUGE_BOSS_3 )->draw( GAUGE + Vector2( 0.0, getHideRatio( ) * 1000 ) );
		break;
	case 4:
		_image.at( TYPE::GAUGE_BOSS_4 )->draw( GAUGE + Vector2( 0.0, getHideRatio( ) * 1000 ) );
		break;
	case 5:
		_image.at( TYPE::GAUGE_BOSS_5 )->draw( GAUGE + Vector2( 0.0, getHideRatio( ) * 1000 ) );
		break;
	case 6:
		_image.at( TYPE::GAUGE_BOSS_6 )->draw( GAUGE + Vector2( 0.0, getHideRatio( ) * 1000 ) );
		break;
	case 7:
		_image.at( TYPE::GAUGE_BOSS_7 )->draw( GAUGE + Vector2( 0.0, getHideRatio( ) * 1000 ) );
		break;
	case 8:
		_image.at( TYPE::GAUGE_BOSS_8 )->draw( GAUGE + gauge_full + Vector2( 0.0, getHideRatio( ) * 1000 ) );
		break;
	}
}