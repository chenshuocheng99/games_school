#include "Diorama.h"

#include "Status.h"
#include "define.h"

#include "Drawer.h"
#include "Model.h"
#include "Image.h"

const int STAGE_Z = 0;
const double PLAYER_SIZE = 0.15;
const double ANGLE = 1.57;

const double ENEMY_GOBLIN_SIZE = 0.15;
const double ENEMY_ZOMBIE_SIZE = 0.15;
const double ENEMY_MINOTAUR_SIZE = 0.15;
const double ENEMY_BOSS_SIZE = 0.0135;

const int HP_POS_X = 40;
const int HP_POS_Y = 20;

const int POWER_POS_X = 50;
const int POWER_POS_Y = 20;

const int ITEM_POS_Z = 0;
const int ITEM_SIZE_X = 20;
const int ITEM_SIZE_Y = 20;
const int ITEM_SIZE_Z = 20;
const int ITEM_AXIS_X = 1;
const int ITEM_AXIS_Y = 0;
const int ITEM_AXIS_Z = 0;

const int BACKGROND_POS_X = 50;
const int BACKGROND_POS_Y = 0;
const int BACKGROND_POS_Z = -1;
const double BACKGROND_SIZE = 0.1;
const int  BACKGROND_AXIS_X = 1;
const int  BACKGROND_AXIS_Y = 0;
const int  BACKGROND_AXIS_Z = 0;

const int  CHARACTER_AXIS_X = 1;
const double  CHARACTER_AXIS_Y = 0;
const int  CHARACTER_AXIS_Z = 0;

const double PICTURE_LEFT_UP_X = 0.0;
const double PICTURE_LEFT_UP_Y = 1.0;
const double PICTURE_LEFT_DOWN_X = 0.0;
const double PICTURE_LEFT_DOWN_Y = 0.0;
const double PICTURE_RIGHT_UP_X = 1.0;
const double PICTURE_RIGHT_UP_Y = 1.0;
const double PICTURE_RIGHT_DOWN_X = 1.0;
const double PICTURE_RIGHT_DOWN_Y = 0.0;

const int ENEMY_AXIS_X = 1;
const int ENEMY_AXIS_Y = 0;
const int ENEMY_AXIS_Z = 0;

const double R_ANGLE = 1.57;
const double R_SIZE_X = 0.25;
const double R_SIZE_Y = 0.25;
const double R_SIZE_Z = 0.25;
const double R_AXIS_X = 2.4;
const int R_AXIS_Y = 1;
const int R_AXIS_Z = -1;

const double G_ANGLE = 1.57;
const double G_SIZE_X = 0.25;
const double G_SIZE_Y = 0.25;
const double G_SIZE_Z = 0.25;
const int G_AXIS_X = 1;
const int G_AXIS_Y = -1;
const int G_AXIS_Z = 1;

const double B_ANGLE = 2.5;
const double B_SIZE_X = 0.35;
const double B_SIZE_Y = 0.35;
const double B_SIZE_Z = 0.35;
const int B_AXIS_X = 1;
const int B_AXIS_Y = 3;
const double B_AXIS_Z = -3.8;

const double W_ANGLE = 4.0;
const double W_SIZE_X = 0.55;
const double W_SIZE_Y = 0.55;
const double W_SIZE_Z = 0.55;
const double W_AXIS_X = -0.5;
const double W_AXIS_Y = 1.2;
const double W_AXIS_Z = -2.5;

Diorama::Diorama( StatusConstPtr status )
	: _status( status ) {
	DrawerPtr drawer = Task::get< Drawer >( );
	drawer->setNearFar( 100, 2000 );
	drawer->setCameraUp( Vector( 0, 0, -1 ) );

	//敵キャラ
	_model_enemy[ 0 ] = ModelPtr( new Model );
	_model_enemy[ 0 ]->load( "resource/enemy/goblin/enemy0_attack.mv1" );
	_model_enemy[ 1 ] = ModelPtr( new Model );
	_model_enemy[ 1 ]->load( "resource/enemy/red_goblin/enemy1_attack.mv1" );
	_model_enemy[ 2 ] = ModelPtr( new Model );
	_model_enemy[ 2 ]->load( "resource/enemy/minotaur/enemy_minotaur_cleave.mv1" );
	_model_enemy[ 3 ] = ModelPtr( new Model );
	_model_enemy[ 3 ]->load( "resource/enemy/boss/boss_attack_cleave.mv1" );

	_transform_scaling_enemy[ 0 ] = ENEMY_GOBLIN_SIZE;
	_transform_scaling_enemy[ 1 ] = ENEMY_ZOMBIE_SIZE;
	_transform_scaling_enemy[ 2 ] = ENEMY_MINOTAUR_SIZE;
	_transform_scaling_enemy[ 3 ] = ENEMY_BOSS_SIZE;

	//エレメント
	_model_elemental_air = ModelPtr( new Model );
	_model_elemental_air->load( "resource/elemental/AirElemental/AirElemental_wind.mv1" );
	_model_elemental_earth = ModelPtr( new Model );
	_model_elemental_earth->load( "resource/elemental/EarthElemental/EarthElemental_blue.mv1" );
	_model_elemental_lava = ModelPtr( new Model );
	_model_elemental_lava->load( "resource/elemental/LavaElemental/LavaElemental_red.mv1" );
	_model_elemental_water = ModelPtr( new Model );
	_model_elemental_water->load( "resource/elemental/WaterElemental/WaterElemental_blue.mv1" );

	//プレイヤーキャラ
	_model_character[ 0 ] = ModelPtr( new Model );
	_model_character[ 0 ]->load( "resource/player/red/player_knight_red.mv1" );
	_model_character[ 1 ] = ModelPtr( new Model );
	_model_character[ 1 ]->load( "resource/player/green/player_knight_green.mv1" );
	_model_character[ 2 ] = ModelPtr( new Model );
	_model_character[ 2 ]->load( "resource/player/blue/player_knight_blue.mv1" );
	_model_character[ 3 ] = ModelPtr( new Model );
	_model_character[ 3 ]->load( "resource/player/white/player_knight_white.mv1" );

	//背景
	_model_background = ModelPtr( new Model );
	_model_background->load( "resource/background/GameBackGround_ver1_1114.mv1" );

	//アイテム
	_model_item_stop = ModelPtr( new Model );
	_model_item_stop->load( "resource/item_model/item_stop/item_enemy_stop.mv1" );

	//攻撃範囲画像
	_image_red_circle = drawer->createImage( "resource/ui/enemy_circle.png" );
	_image_blue_circle = drawer->createImage( "resource/ui/player_circle.png" );

	_rot = 0;
}

Diorama::~Diorama( ) {

}

void Diorama::focus( int player_idx ) {
	//カメラ移動→プレイヤー
	_rot += 0.001;
	Matrix mat = Matrix::makeTransformRotation( Vector( 0, 0, -1 ), _rot );

	if ( player_idx == Define::PLAYER_IDX_INVALID ) {
		_camera_target_pos = mat.multiply( Vector( 1000, 0, -1000 ) );
		_camera_target_focus = Vector( );
		return;
	}
	if ( !_status->isValidPlayerPos( player_idx ) ) {
		_camera_target_pos = mat.multiply( Vector( 1000, 0, -1000 ) );
		_camera_target_focus = Vector( );
		return;
	}

	Vector2 pos = _status->getPlayerPos( player_idx );
	Vector camera_pos(
		pos.x - Define::MAP_WIDTH / 2,
		Define::MAP_HEIGHT - pos.y - Define::MAP_HEIGHT / 2,
		STAGE_Z + 10 );
	_camera_target_pos = mat.multiply( Vector( 500, 0, -300 ) ) + camera_pos;
	_camera_target_focus = camera_pos;
}

void Diorama::focusEnemy( ) {
	//カメラ移動→敵
	_rot += 0.001;
	Matrix mat = Matrix::makeTransformRotation( Vector( 0, 0, -1 ), _rot );

	int enemy_num = _status->getEnemyNum( );
	if ( enemy_num < 0 ) {
		_camera_target_pos = mat.multiply( Vector( 1000, 0, -1000 ) );
		_camera_target_focus = Vector( );
		return;
	}

	int focus_enemy_idx = 0;
	int adjust_focus_height = 10;
	for ( int i = 0; i < enemy_num; i++ ) {
		int enemy_idx = i;
		Status::Enemy enemy = _status->getEnemy( enemy_idx );
		if ( enemy.type == Define::ENEMY_TYPE_BOSS ) {
			focus_enemy_idx = enemy_idx;
			adjust_focus_height = -10;
			break;
		}
	}
	Vector2 pos = _status->getEnemyTargetPos( focus_enemy_idx );
	Vector camera_pos(
		pos.x - Define::MAP_WIDTH / 2,
		Define::MAP_HEIGHT - pos.y - Define::MAP_HEIGHT / 2,
		STAGE_Z + adjust_focus_height );
	_camera_target_pos = mat.multiply( Vector( 500, 0, -300 ) ) + camera_pos;
	_camera_target_focus = camera_pos;
}

void Diorama::seeWhole( ) {
	//カメラ→全体
	_rot += 0.001;
	Matrix mat = Matrix::makeTransformRotation( Vector( 0, 0, -1 ), _rot );

	_camera_target_pos = mat.multiply( Vector( 1000, 0, -1000 ) );
	_camera_target_focus = Vector( );
}

void Diorama::draw( ) {
	DrawerPtr drawer = Task::get< Drawer >( );

	_camera_current_pos += ( _camera_target_pos - _camera_current_pos ) * 0.01;
	_camera_current_focus += ( _camera_target_focus - _camera_current_focus ) * 0.05;
	drawer->setCamera( _camera_current_pos, _camera_current_focus );

	//背景描画
	drawBackground( );

	//プレイヤーキャラモデル描画
	drawCharacter( );

	//敵キャラモデル描画
	drawEnemy( );

	//エレメント描画
	drawElementalPos( );

	for ( int i = 0; i < _status->getItemNum( ); i++ ) {
		Vector2 pos_item = _status->getItemPos( i );
		//アイテム描画
		drawItemStop(
			( int )pos_item.x - Define::MAP_WIDTH / 2,
			Define::MAP_HEIGHT - ( int )pos_item.y - Define::MAP_HEIGHT / 2 );
	}

}

//ゲーム画面にエレメントの配置座標
void Diorama::drawElementalPos( ) {
	Vector2 pos_w = Vector2( 20, 50 );
	for ( int i = 0; i < 4; i++ ) {
		Vector2 pos = _status->getElementalPos( i );
		switch ( i ) {
		case 0:
			drawElementalR( pos.x - Define::MAP_WIDTH / 2, Define::MAP_HEIGHT - pos.y - Define::MAP_HEIGHT / 2 );
			break;
		case 1:
			drawElementalG( pos.x - Define::MAP_WIDTH / 2, Define::MAP_HEIGHT - pos.y - Define::MAP_HEIGHT / 2 );
			break;
		case 2:
			drawElementalB( pos.x - Define::MAP_WIDTH / 2, Define::MAP_HEIGHT - pos.y - Define::MAP_HEIGHT / 2 );
			break;
		case 3:
			drawElementalW( pos.x - pos_w.x - Define::MAP_WIDTH / 2, Define::MAP_HEIGHT - pos.y + pos_w.y - Define::MAP_HEIGHT / 2 );
			break;
		}
	}
}

//ゲーム画面にアイテムの配置
void Diorama::drawItemStop( int x, int y ) {
	Matrix mat_pos = Matrix::makeTransformTranslation( Vector( x, y, ITEM_POS_Z ) );
	Matrix mat_scale = Matrix::makeTransformScaling( Vector( ITEM_SIZE_X, ITEM_SIZE_Y, ITEM_SIZE_Y ) );
	Matrix mat_rot = Matrix::makeTransformRotation( Vector( ITEM_AXIS_X, ITEM_AXIS_Y, ITEM_AXIS_Z ), ANGLE );
	_model_item_stop->setTrans( mat_pos );
	_model_item_stop->setScale( mat_scale );
	_model_item_stop->setRot( mat_rot );
	_model_item_stop->draw( );
}

//ステージ配置
void Diorama::drawBackground( ) {
	Matrix mat_pos = Matrix::makeTransformTranslation( Vector( BACKGROND_POS_X, BACKGROND_POS_Y, BACKGROND_POS_Z ) );
	Matrix mat_scale = Matrix::makeTransformScaling( Vector( BACKGROND_SIZE, BACKGROND_SIZE, BACKGROND_SIZE ) );
	Matrix mat_rot = Matrix::makeTransformRotation( Vector( BACKGROND_AXIS_X, BACKGROND_AXIS_Y, BACKGROND_AXIS_Z ), ANGLE );
	_model_background->setTrans( mat_pos );
	_model_background->setScale( mat_scale );
	_model_background->setRot( mat_rot );
	_model_background->draw( );
}

//角度調整用関数
Matrix Diorama::changeRotation( Vector2 pos_e, Vector2 pos_player ) {
	Vector2 pos_p = Vector2( pos_player.x, pos_player.y );
	Vector pos_focus = Vector( pos_e.x - pos_p.x, pos_e.y - pos_p.y, 0 );
	Vector pos_mine( CHARACTER_AXIS_X, CHARACTER_AXIS_Y, CHARACTER_AXIS_Z );//回転角度じく
	double angle = 0;//角度の値
	Vector axis = pos_mine.cross( pos_focus );
	if ( pos_p.y > pos_e.y ) {
		angle = pos_focus.angle( pos_mine ) + 1.5;
	} else {
		angle = pos_focus.angle( pos_mine ) - 1.5;
	}
	Matrix mat_rot = Matrix::makeTransformRotation( axis, angle );
	return mat_rot;
}

void Diorama::drawCharacter( ) {
	for ( int i = 0; i < Define::MAX_PLAYER; i++ ) {
		if ( _status->isValidPlayerPos( i ) ) {
			Vector2 pos = _status->getPlayerPos( i );

			Matrix mat_pos = Matrix::makeTransformTranslation( Vector( pos.x - Define::MAP_WIDTH / 2,
																	   Define::MAP_HEIGHT - pos.y - Define::MAP_HEIGHT / 2,
																	   STAGE_Z ) );
			Matrix mat_scale = Matrix::makeTransformScaling( Vector( PLAYER_SIZE, PLAYER_SIZE, PLAYER_SIZE ) );
			Matrix mat_pose = Matrix::makeTransformRotation( Vector( CHARACTER_AXIS_X, CHARACTER_AXIS_Y, CHARACTER_AXIS_Z ), ANGLE );
			_model_character[ i ]->setTrans( mat_pos );
			_model_character[ i ]->setScale( mat_scale );

			//キャラクターモデル角度調整
			Status::Player player = _status->getPlayer( i );
			if ( player.action == Define::ACTION_RELEASE ) {
				Vector2 pos_e = _status->getElementalPos( i );
				_model_character[ i ]->setRot( mat_pose * changeRotation( pos_e, pos ) );
			} else if ( player.action == Define::ACTION_ATTACK ) {
				Vector2 pos_e;
				int enemy_num = _status->getEnemyNum( );
				for ( int j = 0; j < enemy_num; j++ ) {
					Status::Enemy enemy = _status->getEnemy( j );
					if ( enemy.defense[ i ] ) {
						pos_e = _status->getEnemyCurrentPos( j );
					}
				}
				_model_character[ i ]->setRot( mat_pose * changeRotation( pos_e, pos ) );
			} else {
				_model_character[ i ]->setRot( mat_pose );
			}

			//攻撃アニメーション
			double max_anim_time = _model_character[ i ]->getEndAnimTime( );
			int phase_count = _status->getPhaseCount( );
			if ( _status->getCurrentPlayerIdx( ) == i &&
				 ( _status->getPhaseType( ) == PHASE::ATTACK_SUCCESS || _status->getPhaseType( ) == PHASE::ATTACK_FAILURE ) &&
				 phase_count <= max_anim_time ) {
				_model_character[ i ]->setAnimTime( phase_count );
			}

			_model_character[ i ]->draw( );
		}
	}
}

void Diorama::drawEnemy( ) {
	int enemy_num = _status->getEnemyNum( );
	for ( int i = 0; i < enemy_num; i++ ) {
		Status::Enemy enemy = _status->getEnemy( i );

		Matrix mat_pos = Matrix::makeTransformTranslation( Vector( enemy.current_pos.x - Define::MAP_WIDTH / 2,
																   Define::MAP_HEIGHT - enemy.current_pos.y - Define::MAP_HEIGHT / 2,
																   STAGE_Z ) );
		Matrix mat_scale = Matrix::makeTransformScaling( Vector( _transform_scaling_enemy[ enemy.type ],
																 _transform_scaling_enemy[ enemy.type ],
																 _transform_scaling_enemy[ enemy.type ] ) );
		Matrix mat_pose = Matrix::makeTransformRotation( Vector( ENEMY_AXIS_X, ENEMY_AXIS_Y, ENEMY_AXIS_Z ), ANGLE );
		_model_enemy[ enemy.type ]->setTrans( mat_pos );
		_model_enemy[ enemy.type ]->setScale( mat_scale );

		//敵キャラモデル角度調整
		int player_idx = 0;
		if ( enemy.attack[ 0 ] ) {
			player_idx = 0;
		} else if ( enemy.attack[ 1 ] ) {
			player_idx = 1;
		} else if ( enemy.attack[ 2 ] ) {
			player_idx = 2;
		} else if ( enemy.attack[ 3 ] ) {
			player_idx = 3;
		}
		Status::Player player = _status->getPlayer( player_idx );
		Vector2 pos_p = _status->getPlayerPos( player_idx );
		if ( enemy.attack[ player_idx ] ) {
			_model_enemy[ enemy.type ]->setRot( mat_pose * changeRotation( pos_p, enemy.current_pos ) );
		} else {
			_model_enemy[ enemy.type ]->setRot( mat_pose );
		}

		//攻撃アニメーション
		double max_anim_time = _model_enemy[ enemy.type ]->getEndAnimTime( );
		int phase_count = _status->getPhaseCount( ) - Define::ENEMY_START_ATTACKING_TIME;
		if ( _status->getPhaseType( ) == PHASE::ENEMY_ACTION &&
			 phase_count >= 0 && phase_count <= max_anim_time ) {
			if ( enemy.type != Define::ENEMY_TYPE_BOSS ) {
				_model_enemy[ enemy.type ]->setAnimTime( phase_count );
			} else {
				_model_enemy[ enemy.type ]->setAnimTime( phase_count * 2.5 );
			}
		} else if ( _status->getPhaseType( ) == PHASE::SPECIAL_ATTACK &&
					phase_count >= 0 && phase_count <= max_anim_time &&
					enemy.type == Define::ENEMY_TYPE_BOSS ) {
			_model_enemy[ enemy.type ]->setAnimTime( phase_count * 2.5 );
		} else if ( _status->getPhaseType( ) != PHASE::ENEMY_ACTION ) {
			_model_enemy[ enemy.type ]->setAnimTime( 0 );
		}

		_model_enemy[ enemy.type ]->draw( );
	}
}

void Diorama::drawElementalR( double x, double y ) {
	Matrix mat_pos_lava = Matrix::makeTransformTranslation( Vector( x, y, STAGE_Z ) );
	Matrix mat_scale_lava = Matrix::makeTransformScaling( Vector( R_SIZE_X, R_SIZE_Y, R_SIZE_Z ) );
	Matrix mat_rot_lava = Matrix::makeTransformRotation( Vector( R_AXIS_X, R_AXIS_Y, R_AXIS_Z ), R_ANGLE );
	_model_elemental_lava->setTrans( mat_pos_lava );
	_model_elemental_lava->setScale( mat_scale_lava );
	_model_elemental_lava->setRot( mat_rot_lava );
	_model_elemental_lava->draw( );
}

void Diorama::drawElementalG( double x, double y ) {
	Matrix mat_pos_air = Matrix::makeTransformTranslation( Vector( x, y, STAGE_Z ) );
	Matrix mat_scale_air = Matrix::makeTransformScaling( Vector( G_SIZE_X, G_SIZE_Y, G_SIZE_Z ) );
	Matrix mat_rot_air = Matrix::makeTransformRotation( Vector( G_AXIS_X, G_AXIS_Y, G_AXIS_Z ), G_ANGLE );
	_model_elemental_air->setTrans( mat_pos_air );
	_model_elemental_air->setScale( mat_scale_air );
	_model_elemental_air->setRot( mat_rot_air );
	_model_elemental_air->draw( );
}

void Diorama::drawElementalB( double x, double y ) {
	Matrix mat_pos_water = Matrix::makeTransformTranslation( Vector( x, y, STAGE_Z ) );
	Matrix mat_scale_water = Matrix::makeTransformScaling( Vector( B_SIZE_X, B_SIZE_Y, B_SIZE_Z ) );
	Matrix mat_rot_water = Matrix::makeTransformRotation( Vector( B_AXIS_X, B_AXIS_Y, B_AXIS_Z ), B_ANGLE );
	_model_elemental_water->setTrans( mat_pos_water );
	_model_elemental_water->setScale( mat_scale_water );
	_model_elemental_water->setRot( mat_rot_water );
	_model_elemental_water->draw( );
}

void Diorama::drawElementalW( double x, double y ) {
	Matrix mat_pos_earth = Matrix::makeTransformTranslation( Vector( x, y, STAGE_Z ) );
	Matrix mat_scale_earth = Matrix::makeTransformScaling( Vector( W_SIZE_X, W_SIZE_Y, W_SIZE_Z ) );
	Matrix mat_rot_earth = Matrix::makeTransformRotation( Vector( W_AXIS_X, W_AXIS_Y, W_AXIS_Z ), W_ANGLE );
	_model_elemental_earth->setTrans( mat_pos_earth );
	_model_elemental_earth->setScale( mat_scale_earth );
	_model_elemental_earth->setRot( mat_rot_earth );
	_model_elemental_earth->draw( );
}
