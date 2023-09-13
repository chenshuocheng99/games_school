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


//���j�^�[�i�Q�[����ʁj�S�̕`��
//�t�F�[�Y���̃V�[���؂�ւ��i���V�[���j

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

	//��
	_se->draw( );

	//�e�t�F�[�Y�`��
	switch ( _status->getPhaseType( ) ) {
	case PHASE::TITLE://�^�C�g�����
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
	case PHASE::OPENING://�I�[�v�j���O���o
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
	case PHASE::APPEAR://�G�z�u����
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
	case PHASE::APPEAR_ENEMIES://�G�z�u
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
	case PHASE::APPEAR_BOSS://�{�X�z�u
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
	case PHASE::READY_PLACE://��z�u����
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
	case PHASE::PLACE://��z�u
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
	case PHASE::PASS_PLAYER://�v���C���[�����s���̏ꍇ
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
	case PHASE::CURRENT_PLAYER://�v���C���[�������
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
	case PHASE::ENEMY_ACTION://�G�U��
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
	case PHASE::SPECIAL_ATTACK://�{�X���[���b�g�U��
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
	case PHASE::ROULETTE_START://���[���b�g�n�܂�
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
	case PHASE::ROULETTE_STOP://���[���b�g��~
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
	case PHASE::ROULETTE_CONFIRM://���[���b�g���ʊm�F
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
	case PHASE::ATTACK_SUCCESS://�v���C���[�U������
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
	case PHASE::ATTACK_FAILURE://�v���C���[�U�����s
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
	case PHASE::REVIVAL_SUCCESS://�v���C���[��������
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
	case PHASE::REVIVAL_FAILURE://�v���C���[�������s
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
	case PHASE::EXPLORE_SUCCESS://�v���C���[�T������
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
	case PHASE::EXPLORE_FAILURE://�v���C���[�T�����s
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
	case PHASE::RELEASE_SUCCESS://�G�������g�������
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
	case PHASE::ENCOUNTER://�ʏ�{�X���[���b�g
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
	case PHASE::RECOVERY://������
		_player->show( );
		_roulette->hide( );
		_text->hide( );
		_title->hide( );
		_map->show( );
		_enemy->show( );
		_result->hide( );
		break;
	case PHASE::ITEM_USE://�A�C�e���g�p
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
	//������
	if ( _diorama ) {
		_diorama->draw( );
	}
	_title->drawPrepare( );
}