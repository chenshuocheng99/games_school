#include "Se.h"
#include "Status.h"
#include "Sound.h"

Se::Se( StatusConstPtr status )
	: _status( status ) {
	_bgm->loadSE( "resource/bgm/battle_loop.wav" );
	_se_boss_action->loadSE( "resource/bgm/boss_action.wav" );
	_se_boss_appear->loadSE( "resource/bgm/boss_appear.wav" );
	_se_enemy_action->loadSE( "resource/bgm/enemy_action.wav" );
	_se_enemy_appear->loadSE( "resource/bgm/enemy_appear.wav" );
	_se_player_action->loadSE( "resource/bgm/player_action.wav" );
	_se_item_get->loadSE( "resource/bgm/item.wav" );
	_se_release->loadSE( "resource/bgm/release.wav" );
}

Se::~Se( ) {

}

void Se::draw( ) {
	//BGMを流れ
	if ( !_bgm->isPlayingBGM( ) ) {
		_bgm->playBGM( "resource/bgm/battle_loop.wav" );
	}

	//フェーズによるのSE流れ
	if ( _status->getPhaseType( ) == PHASE::ATTACK_SUCCESS || _status->getPhaseType( ) == PHASE::ATTACK_FAILURE ) {
		if ( _status->getPhaseCount( ) == 10 ) {
			_se_player_action->playSE( "resource/bgm/player_action.wav" );
		}
	}
	if ( _status->getPhaseType( ) == PHASE::ENEMY_ACTION ) {
		int enemy_num = _status->getEnemyNum( );
		if ( _status->getPhaseCount( ) == 30 && enemy_num != 0 ) {
			for ( int i = 0; i < enemy_num; i++ ) {
				Status::Enemy enemy = _status->getEnemy( i );
				if ( enemy.type == Define::ENEMY_TYPE_BOSS ) {
					_se_boss_action->playSE( "resource/bgm/boss_action.wav" );
				} else {
					_se_enemy_action->playSE( "resource/bgm/enemy_action.wav" );
				}
			}
		}
	}

	//SE判断
	if ( _status->getPhaseType( ) == PHASE::APPEAR_ENEMIES ) {
		if ( _status->getPhaseCount( ) == 0 ) {
			_se_enemy_appear->playSE( "resource/bgm/enemy_appear.wav" );
		}
	}
	if ( _status->getPhaseType( ) == PHASE::EXPLORE_SUCCESS ) {
		if ( _status->getPhaseCount( ) == 0 ) {
			_se_item_get->playSE( "resource/bgm/item.wav" );
		}
	}
	if ( _status->getPhaseType( ) == PHASE::RELEASE_SUCCESS ) {
		if ( _status->getPhaseCount( ) == 0 ) {
			_se_release->playSE( "resource/bgm/release.wav" );
		}
	}
	if ( _status->getPhaseType( ) == PHASE::SPECIAL_ATTACK ) {
		if ( _status->getPhaseCount( ) == 0 ) {
			_se_boss_action->playSE( "resource/bgm/boss_action.wav" );
		}
	}
	if ( _status->getPhaseType( ) == PHASE::APPEAR_BOSS ) {
		if ( _status->getPhaseCount( ) == 0 ) {
			_se_boss_appear->playSE( "resource/bgm/boss_appear.wav" );
		}
	}
}