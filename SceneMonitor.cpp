#include "SceneMonitor.h"
#include "Player.h"
#include "Enemy.h"
#include "Roulette.h"
#include "Diorama.h"
#include "Title.h"
#include "Text.h"
#include "Result.h"
#include "Se.h"

#include "Status.h"
#include "Map.h"
#include "define.h"

#include "Client.h"


//モニター（ゲーム画面）全体描画
//フェーズよるのシーン切り替え（仮シーン）

const int MAP_POS_X = Define::SCREEN_WIDTH / 2;
const int MAP_POS_Y = Define::MAP_HEIGHT / 2 + 100;

SCENE( SceneMonitor );

SceneMonitor::SceneMonitor( ) {
	StatusPtr status( new Status );
	ClientPtr client = Task::get< Client >( );
	client->setRecvDataUdp( status );
	_status = status;
}

SceneMonitor::~SceneMonitor( ) {

}

void SceneMonitor::initialize( ) {
	StatusPtr status( new Status );
	ClientPtr client = Task::get< Client >( );
	client->setRecvDataUdp( status );
	_status = status;
	_map = MapPtr( new Map( _status ) );
	_player = PlayerPtr( new Player( _status ) );
	_enemy = EnemyPtr( new Enemy( _status ) );
	_roulette = RoulettePtr( new Roulette( _status ) );
	_diorama = DioramaPtr( new Diorama( _status ) );
	_title = TitlePtr( new Title( ) );
	_text = TextPtr( new Text( _status ) );
	_result = ResultPtr( new Result( _status ) );
	_se = SePtr( new Se( _status ) );
}

void SceneMonitor::update( ) {
	ClientPtr client = Task::get< Client >( );
	if ( client->getPhase( ) == "PHASE_READY" ) {
		updatePhaseReady( );
		return;
	}

	//音
	_se->draw( );

	//各フェーズ描画
	switch ( _status->getPhaseType( ) ) {
	case PHASE::TITLE://タイトル画面
		if ( _diorama ) {
			_diorama->focus( Define::PLAYER_IDX_INVALID );
		}
		_player->hide( );
		_enemy->hide( );
		_roulette->hide( );
		_text->hide( );
		_map->hide( );
		_title->show( );
		_result->hide( );
		break;
	case PHASE::OPENING://オープニング演出
		if ( _diorama ) {
			_diorama->seeWhole( );
		}
		_player->hide( );
		_roulette->hide( );
		_text->show( );
		_title->hide( );
		_map->hide( );
		_result->hide( );
		break;
	case PHASE::APPEAR://敵配置準備
		if ( _diorama ) {
			_diorama->seeWhole( );
		}
		_player->hide( );
		_roulette->hide( );
		_text->show( );
		_enemy->show( );
		_title->hide( );
		_map->hide( );
		_result->hide( );
		break;
	case PHASE::APPEAR_ENEMIES://敵配置
		if ( _diorama ) {
			_diorama->seeWhole( );
		}
		_roulette->hide( );
		_text->show( );
		_enemy->show( );
		_player->show( );
		_title->hide( );
		_map->show( );
		_result->hide( );
		break;
	case PHASE::APPEAR_BOSS://ボス配置
		if ( _diorama ) {
			_diorama->seeWhole( );
		}
		_roulette->hide( );
		_text->show( );
		_enemy->show( );
		_player->show( );
		_title->hide( );
		_map->show( );
		_result->hide( );
		break;
	case PHASE::READY_PLACE://駒配置準備
		if ( _diorama ) {
			_diorama->seeWhole( );
		}
		_text->show( );
		_enemy->show( );
		_player->show( );
		_map->show( );
		_roulette->hide( );
		_title->hide( );
		_result->hide( );
		break;
	case PHASE::PLACE://駒配置
		if ( _diorama ) {
			_diorama->focus( _status->getCurrentPlayerIdx( ) );
		}
		_player->show( );
		_roulette->hide( );
		_text->hide( );
		_title->hide( );
		_map->show( );
		_enemy->show( );
		_result->hide( );
		break;
	case PHASE::PASS_PLAYER://プレイヤーが無行動の場合
		if ( _diorama ) {
			_diorama->focus( _status->getCurrentPlayerIdx( ) );
		}
		_player->show( );
		_enemy->hide( );
		_text->show( );
		_roulette->hide( );
		_title->hide( );
		_map->hide( );
		_result->hide( );
		break;
	case PHASE::CURRENT_PLAYER://プレイヤーもう一回
		if ( _diorama ) {
			_diorama->focus( _status->getCurrentPlayerIdx( ) );
		}
		_player->show( );
		_enemy->hide( );
		_roulette->show( );
		_text->show( );
		_title->hide( );
		_map->hide( );
		_result->hide( );
		break;
	case PHASE::ENEMY_ACTION://敵攻撃
		if ( _diorama ) {
			_diorama->focusEnemy( );
		}
		_player->show( );
		_roulette->hide( );
		_text->show( );
		_title->hide( );
		_map->hide( );
		_enemy->show( );
		_result->hide( );
		break;
	case PHASE::SPECIAL_ATTACK://ボスルーレット攻撃
		if ( _diorama ) {
			_diorama->focusEnemy( );
		}
		_player->show( );
		_roulette->hide( );
		_text->show( );
		_title->hide( );
		_map->hide( );
		_enemy->show( );
		_result->hide( );
		break;
	case PHASE::ROULETTE_START://ルーレット始まり
		if ( _diorama ) {
			_diorama->focus( _status->getCurrentPlayerIdx( ) );
		}
		_player->show( );
		_enemy->hide( );
		_roulette->show( );
		_text->show( );
		_title->hide( );
		_map->hide( );
		_result->hide( );
		break;
	case PHASE::ROULETTE_STOP://ルーレット停止
		if ( _diorama ) {
			_diorama->focus( _status->getCurrentPlayerIdx( ) );
		}
		_player->show( );
		_enemy->hide( );
		_roulette->show( );
		_text->show( );
		_title->hide( );
		_map->hide( );
		_result->hide( );
		break;
	case PHASE::ROULETTE_CONFIRM://ルーレット結果確認
		if ( _diorama ) {
			_diorama->focus( _status->getCurrentPlayerIdx( ) );
		}
		_player->show( );
		_enemy->hide( );
		_roulette->show( );
		_text->hide( );
		_title->hide( );
		_map->hide( );
		_result->hide( );
		break;
	case PHASE::ATTACK_SUCCESS://プレイヤー攻撃成功
		if ( _diorama ) {
			_diorama->focus( _status->getCurrentPlayerIdx( ) );
		}
		_player->show( );
		_enemy->show( );
		_text->show( );
		_roulette->hide( );
		_title->hide( );
		_map->hide( );
		_result->hide( );
	case PHASE::ATTACK_FAILURE://プレイヤー攻撃失敗
		if ( _diorama ) {
			_diorama->focus( _status->getCurrentPlayerIdx( ) );
		}
		_player->show( );
		_enemy->show( );
		_text->show( );
		_roulette->hide( );
		_title->hide( );
		_map->hide( );
		_result->hide( );
		break;
	case PHASE::REVIVAL_SUCCESS://プレイヤー復活成功
		if ( _diorama ) {
			_diorama->focus( _status->getCurrentPlayerIdx( ) );
		}
		_player->show( );
		_enemy->show( );
		_text->show( );
		_roulette->hide( );
		_title->hide( );
		_map->hide( );
		_result->hide( );
		break;
	case PHASE::REVIVAL_FAILURE://プレイヤー復活失敗
		if ( _diorama ) {
			_diorama->focus( _status->getCurrentPlayerIdx( ) );
		}
		_player->show( );
		_enemy->show( );
		_text->show( );
		_roulette->hide( );
		_title->hide( );
		_map->hide( );
		_result->hide( );
		break;
	case PHASE::EXPLORE_SUCCESS://プレイヤー探索成功
		if ( _diorama ) {
			_diorama->focus( _status->getCurrentPlayerIdx( ) );
		}
		_player->show( );
		_enemy->show( );
		_text->show( );
		_roulette->hide( );
		_title->hide( );
		_map->show( );
		_result->hide( );
		break;
	case PHASE::EXPLORE_FAILURE://プレイヤー探索失敗
		if ( _diorama ) {
			_diorama->focus( _status->getCurrentPlayerIdx( ) );
		}
		_player->show( );
		_enemy->show( );
		_text->show( );
		_roulette->hide( );
		_title->hide( );
		_map->hide( );
		_result->hide( );
		break;
	case PHASE::RELEASE_SUCCESS://エレメント解放成功
		if ( _diorama ) {
			_diorama->focus( _status->getCurrentPlayerIdx( ) );
		}
		_player->show( );
		_enemy->show( );
		_text->show( );
		_roulette->hide( );
		_title->hide( );
		_map->show( );
		_result->hide( );
		break;
	case PHASE::ENCOUNTER://通常ボスルーレット
		if ( _diorama ) {
			_diorama->focus( _status->getCurrentPlayerIdx( ) );
		}
		_player->show( );
		_enemy->show( );
		_text->show( );
		_roulette->hide( );
		_title->hide( );
		_map->hide( );
		_result->hide( );
		break;
	case PHASE::GAME_OVER:
		_player->hide( );
		_enemy->hide( );
		_roulette->hide( );
		_text->hide( );
		_map->hide( );
		_title->hide( );
		_result->show( );
		break;
	case PHASE::GAME_CLEAR:
		_player->hide( );
		_enemy->hide( );
		_roulette->hide( );
		_text->hide( );
		_map->hide( );
		_title->hide( );
		_result->show( );
		break;
	case PHASE::RECOVERY://初期化
		_player->show( );
		_roulette->hide( );
		_text->hide( );
		_title->hide( );
		_map->show( );
		_enemy->show( );
		_result->hide( );
		break;
	case PHASE::ITEM_USE://アイテム使用
		_player->show( );
		_roulette->hide( );
		_text->show( );
		_title->hide( );
		_map->hide( );
		_enemy->show( );
		_result->hide( );
		break;
	}

	if ( _diorama ) {
		_diorama->draw( );
	}
	_map->draw( MAP_POS_X, MAP_POS_Y );
	_player->draw( );
	_enemy->draw( );
	_roulette->draw( );
	_text->draw( );
	_title->draw( );
	_result->draw( );
}

void SceneMonitor::updatePhaseReady( ) {
	//初期化
	if ( _diorama ) {
		_diorama->draw( );
	}
	_title->drawPrepare( );
}