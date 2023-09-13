#include "Text.h"
#include "Status.h"
#include "Image.h"
#include "Drawer.h"

const int FRAME_X = 0;
const int FRAME_Y = 500;
const int TEXT_X = 50;
const int TEXT_Y = 300;

//�e�L�X�g���
enum class TYPE {
	ATK_TEXT,
	HEAL_TEXT,
	EXP_TEXT,
	ELEMENTAL_TEXT,
	TURN_TEXT,
	INIT
};

//�����I�ȃe�L�X�g���e
static const char* PLAYER_TYPE[ ] =
{ "�E�� ��","�E�� ��","�E�� ��","�E�� ��","" };

static const char* ELEMENT_EFFECT[ ] =
{ "���̃^�[������ �E�� �� �� �o�t���ʗ� ���㏸���܂��B",
"���̃^�[������ �S�� �� �ő�LIFE ���㏸���A LIFE ���񕜂��܂��B",
"���̃^�[������ �S�� �� �U���� ���㏸���܂��B",
"���̃^�[������ �S�� �� �e���͈� ���g�債�܂��B" };

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

	//�t�F�[�Y�ɂ���ăe�L�X�g���e���f
	if ( _status->getPhaseType( ) == PHASE::OPENING ) {
		drawText( text_x, text_y, "�G�������^�� �K�[�f�B�A���ւ悤����" );
	}
	if ( _status->getPhaseType( ) == PHASE::APPEAR ) {
		drawText( text_x, text_y, "�G���o�����Ă��܂��B" );
	}
	if ( _status->getPhaseType( ) == PHASE::APPEAR_ENEMIES ) {
		drawText( text_x, text_y, "�G�̑��������܂��I�I" );
	}
	if ( _status->getPhaseType( ) == PHASE::APPEAR_BOSS ) {
		drawText( text_x, text_y, "�{�X������܂��I�I" );
	}

	//�G�L�����s���e�L�X�g
	int enemy_num = _status->getEnemyNum( );
	if ( _status->getPhaseType( ) == PHASE::ENEMY_ACTION ) {
		if ( enemy_num != 0 ) {
			if ( !_status->isItemUsed( ) ) {
				drawText( text_x, text_y, "�G���U�����Ă��܂��B" );
			}
		} else {
			drawText( text_x, text_y, "�G�����Ȃ��I�@�G�������g����̃`�����X�ł��I" );
		}
	}

	//�A�C�e���g�p�e�L�X�g
	if ( _status->getPhaseType( ) == PHASE::ITEM_USE ) {
		drawText( text_x, text_y, "�A�C�e���̌��ʂœG�͓����Ȃ��I" );
	}

	//��z�u��
	if ( _status->getPhaseType( ) == PHASE::READY_PLACE ) {
		drawText( text_x, text_y, "���z�u���鏀�������Ă��������B" );
	}

	//���[�U�[�s����
	std::string str_player = PLAYER_TYPE[ player_type_code ];
	if ( _status->getPhaseType( ) == PHASE::ROULETTE_START ) {
		drawText( text_x, text_y, str_player + " �̔Ԃł��B\n����{�^���̏�ɁuOK�v�ɂȂ�܂Œu���āA���[���b�g���񂵂Ă��������B" );
	}
	if ( _status->getPhaseType( ) == PHASE::ROULETTE_STOP ) {
		drawText( text_x, text_y, str_player + " �̔Ԃł��B\n����{�^���̏�ɁuOK�v�ɂȂ�܂Œu���āA���[���b�g���~�߂Ă��������B" );
	}

	if ( _status->getPhaseType( ) == PHASE::PASS_PLAYER ) {
		drawText( text_x, text_y, str_player + " �� �s�����p�X���܂��B" );
	}

	//���[�U�[�s�����̃e�L�X�g��
	if ( _status->getPhaseType( ) == PHASE::CURRENT_PLAYER ) {
		const int action_type = _status->getRouletteAction( );
		switch ( action_type ) {
		case Define::ACTION_ATTACK:
			drawText( text_x, text_y, str_player + " �� �U�����܂��B" );
			break;
		case Define::ACTION_EXPLORE:
			drawText( text_x, text_y, str_player + " �� �T�����܂��B" );
			break;
		case Define::ACTION_REVIVAL:
			drawText( text_x, text_y, str_player + " �� �������܂��B" );
			break;
		case Define::ACTION_RELEASE:
			drawText( text_x, text_y, str_player + " �� �G�������g��������܂��B" );
			break;
		}
	}

	//���[�U�[�U���s���e�L�X�g
	if ( _status->getPhaseType( ) == PHASE::ATTACK_SUCCESS ) {
		if ( action == 0 ) {
			drawText( text_x, text_y,
					  str_player +
					  " �� �U���� ���Ă��I ������x���[���b�g���񂹂܂��I" );
		} else {
			drawText( text_x, text_y,
					  str_player +
					  " �� �U���� ���Ă��I" );
		}
	}

	//boss�ɓ����鎞�̍����\��
	if ( _status->getPhaseType( ) == PHASE::ENCOUNTER ) {
		drawText( text_x, text_y, str_player + " �� �s���� ���s�����I BOSS�����Q�[�W���㏸���܂��I" );
	}

	if ( _status->getPhaseType( ) == PHASE::SPECIAL_ATTACK ) {
		drawText( text_x, text_y, str_player + " �� �s���� ���s�����I BOSS���E�ґS���ɓ���U�������܂��I�I" );
	}

	if ( _status->getPhaseType( ) == PHASE::ATTACK_FAILURE ) {
		drawText( text_x, text_y, str_player + " �� �U���� �͂������I�I" );
	}

	//�A�C�e���i�T���j���փe�L�X�g
	if ( _status->getPhaseType( ) == PHASE::EXPLORE_SUCCESS ) {
		if ( _status->getItemNum( ) == Define::MAX_ITEM_NUM ) {
			drawText( text_x, text_y, " �A�C�e���̐��� ��� �ɒB���܂��� " );
		} else {
			drawText( text_x, text_y, str_player + " �� �T���� ���������I �A�C�e���𔭌������I " );
		}
	}

	if ( _status->getPhaseType( ) == PHASE::EXPLORE_FAILURE ) {
		drawText( text_x, text_y, str_player + " �� �T���� ���s�����I " );
	}

	//���[�U�[�����s���e�L�X�g
	if ( _status->getPhaseType( ) == PHASE::REVIVAL_SUCCESS ) {
		drawText( text_x, text_y, str_player + " �� ������ ���������I  " );
	}

	if ( _status->getPhaseType( ) == PHASE::REVIVAL_FAILURE ) {
		drawText( text_x, text_y, str_player + " �� ������ ���s�����I " );
	}

	//�G�������g����e�L�X�g
	std::string ele_effect = ELEMENT_EFFECT[ player_type_code ];

	if ( _status->getPhaseType( ) == PHASE::RELEASE_SUCCESS ) {
		int release_count = _status->getElementalRelease( player_type_code );
		if ( release_count < 3 ) {
			if ( action == 0 ) {
				drawText( text_x, text_y,
						  str_player +
						  " �� �G�������g ������������I ������x���[���b�g���񂹂܂��I" );
			} else {
				drawText( text_x, text_y,
						  str_player +
						  " �� �G�������g ������������I" );
			}
		} else {
			drawText( text_x, text_y,
					  str_player +
					  " �� �G�������g �����S������܂����I\n" + ele_effect );
		}
	}
}