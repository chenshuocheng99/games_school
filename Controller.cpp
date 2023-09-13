#pragma once
#include "PPAP.h"
#include <array>

using namespace ppap;

const int FIELD_ORIGIN_X = 1;
const int FIELD_ORIGIN_Y = 1;
const int MAX_LEVEL = 9;
const int MAX_CRYSTAL_NUM = 16;
const int MAP_SIZE = 4;
const double CRYSTAL_SPEED = 16.0;
const int _TITLE_MAP_SCALE_X = 30;
const int _TITLE_MAP_SCALE_Y = 3;
const int _INIT_TITLE_CAMERA_POS_X = 6;
const int _INIT_TITLE_CAMERA_POS_Y = 2;
const double _TITLE_CAMERA_SPEED = 1.5;

const int JEWEL_LEVEL[10 + 1] = {
	0, // STAGE 0
	3, // STAGE 1
	4, // STAGE 2
	4, // STAGE 3
	5, // STAGE 4
	5, // STAGE 5
	6, // STAGE 6
	6, // STAGE 7
	7, // STAGE 8
	8, // STAGE 9
	9, // STAGE 10
};

const int KEY_HAVE[10 + 1] = {
	0, // STAGE 0
	1, // STAGE 1
	1, // STAGE 2
	2, // STAGE 3
	1, // STAGE 4
	2, // STAGE 5
	1, // STAGE 6
	2, // STAGE 7
	1, // STAGE 8
	1, // STAGE 9
	1, // STAGE 10
};

const Sprite::TYPE LEVEL_TO_TYPE[MAX_LEVEL] = {
	Sprite::TYPE::CRYSTAL_001,
	Sprite::TYPE::CRYSTAL_044,
	Sprite::TYPE::CRYSTAL_005,
	Sprite::TYPE::CRYSTAL_091,
	Sprite::TYPE::CRYSTAL_100,
	Sprite::TYPE::CRYSTAL_103,
	Sprite::TYPE::CRYSTAL_105,
	Sprite::TYPE::CRYSTAL_010,
	Sprite::TYPE::CRYSTAL_063,
};

PPAP* g_ppap;

PTR( Jewel );
class Jewel {
public:
	Jewel( int x, int y, int level ) :
		LEVEL( level ),
		_first( true ) {
		createCrystal( x, y );
	}

	virtual ~Jewel( ) {
		g_ppap->deleteSprite( _crystal );
	}

	void setPos( int x, int y ) {
		_crystal->setPos( ( double )x + FIELD_ORIGIN_X, ( double )y + FIELD_ORIGIN_Y, true );
	}

	int getLevel( ) {
		return LEVEL;
	}

	bool isFirst( ) {
		return _first;
	}

	void reset( ) {
		_first = false;
	}

private:

	void createCrystal( double x, double y ) {
		Sprite::TYPE type = Sprite::TYPE::KEY;
		if ( LEVEL < MAX_LEVEL ) {
			type = LEVEL_TO_TYPE[LEVEL];
		}
		_crystal = g_ppap->createSprite( type );
		_crystal->setSpeed( CRYSTAL_SPEED );
		_crystal->setPos( ( double )x + FIELD_ORIGIN_X, ( double )y + FIELD_ORIGIN_Y, false );
	}

private:
	const int LEVEL;
	SpritePtr _crystal;
	bool _first;
};

class Controller : public PPAP {

	enum class STATE {
		SCENE_TRANSITION,
		RESET_JEWEL,
		INPUT,
		MOVE_LEFT,
		MOVE_RIGHT,
		MOVE_UP,
		MOVE_DOWN,
		GET_KEY,
		CREATE_NEW_JEWEL,
		RESET_STAGE,
	};

	enum class ARROW {
		NONE,
		LEFT,
		RIGHT,
		UP,
		DOWN,
	};

	STATE _state = STATE::RESET_JEWEL;
	ARROW _arrow = ARROW::NONE;
	MapPtr _game_stage_map;
	std::array< JewelPtr, MAP_SIZE* MAP_SIZE > _field;
	std::array< SpritePtr, MAP_SIZE* MAP_SIZE > _grid;
	std::array< PlatePtr, MAX_LEVEL + 1 > _jewel_list;
	std::array< PlatePtr, MAX_LEVEL + 1 > _jewel_level;
	PlatePtr _key_num;
	PlatePtr _how_to_move1;
	PlatePtr _how_to_move2;
	PlatePtr _hidden_plate;
	PlatePtr _target_keys;
	int _key_count = 0;
	PlatePtr _clear_plate;
	PlatePtr _game_over_plate;
	PlatePtr _stage_num;
	PlatePtr _all_clear_plate;
	bool _is_moving_jewel = false;
	CameraPtr _title_camera;
	PlatePtr _brinking_plate;
	int _frame_count = 0;
	bool _title_scene = true;
	MapPtr _title_map;
	std::array< SpritePtr, 20 >_title_crystal;

	int _time_out_count = 0;
	int _stage_count = 1;

	void setup( ) override {
		getCamera( )->setArea( -9999, -9999, 9999, 9999 );
		#if NDEBUG
		_stage_count = 1;
		#endif // NDEBUG
	}

	void updateAsync( ) {
		if ( _title_scene ) {
			if ( _frame_count == 120 ) {
				deletePlate( _all_clear_plate );
				fade( 0.0 );
			}
			if ( _frame_count == 0 ) {
				makeTitleScenePlate( );
				putTitleSceneCrystal( );
				makeTitleSceneMap( );
				ajustTitleSceneCamara( );
			}
			brinkTitleText( );
			moveTitleCamera( );
		}

		if ( !_title_scene ) {
			_time_out_count++;
			if ( _time_out_count > 1500 ) {
				_time_out_count = 0;
				_title_scene = true;
				deletePlates( );
				deleteSprites( );
				deletePlate( _how_to_move1 );
				deletePlate( _how_to_move2 );
				_game_stage_map->clear( );
				deletePlate( _key_num );
				for ( int i = 0; i < MAP_SIZE; i++ ) {
					for ( int j = 0; j < MAP_SIZE; j++ ) {
						deleteSprite( _grid[( size_t )j + ( size_t )i * 4] );
						setField( j, i, JewelPtr( ) );
					}
				}
				for ( int i = 0; i < MAX_LEVEL + 1; i++ ) {
					deletePlate( _jewel_list[i] );
					deletePlate( _jewel_level[i] );
				}
				_stage_count = 1;
				_state = STATE::RESET_JEWEL;
			}
		}
	}

	void makeTitleSceneMap( ) {
		_title_map = getMap( );
		for ( int i = 0; i < _TITLE_MAP_SCALE_Y; i++ ) {
			for ( int j = 0; j < _TITLE_MAP_SCALE_X; j++ ) {
				_title_map->setStreet( j + FIELD_ORIGIN_X, i + FIELD_ORIGIN_Y, true );
			}
		}
	}

	void ajustTitleSceneCamara( ) {
		_title_camera = getCamera( );
		_title_camera->setPos( _INIT_TITLE_CAMERA_POS_X, _INIT_TITLE_CAMERA_POS_Y, false );
		_title_camera->setSpeed( _TITLE_CAMERA_SPEED );
		_title_camera->setRatio( 1.5 );
	}

	void createTitleSceneCrystal( ) {
		_title_crystal[0] = createSprite( Sprite::TYPE::CRYSTAL_001 );
		_title_crystal[1] = createSprite( Sprite::TYPE::CRYSTAL_083 );
		_title_crystal[2] = createSprite( Sprite::TYPE::CRYSTAL_003 );
		_title_crystal[3] = createSprite( Sprite::TYPE::CRYSTAL_036 );
		_title_crystal[4] = createSprite( Sprite::TYPE::CRYSTAL_052 );
		_title_crystal[5] = createSprite( Sprite::TYPE::CRYSTAL_002 );
		_title_crystal[6] = createSprite( Sprite::TYPE::CRYSTAL_056 );
		_title_crystal[7] = createSprite( Sprite::TYPE::CRYSTAL_022 );
		_title_crystal[8] = createSprite( Sprite::TYPE::CRYSTAL_041 );
		_title_crystal[9] = createSprite( Sprite::TYPE::CRYSTAL_058 );
		_title_crystal[10] = createSprite( Sprite::TYPE::CRYSTAL_073 );
		_title_crystal[11] = createSprite( Sprite::TYPE::CRYSTAL_001 );
		_title_crystal[12] = createSprite( Sprite::TYPE::CRYSTAL_023 );
		_title_crystal[13] = createSprite( Sprite::TYPE::CRYSTAL_049 );
		_title_crystal[14] = createSprite( Sprite::TYPE::CRYSTAL_006 );
		_title_crystal[15] = createSprite( Sprite::TYPE::CRYSTAL_051 );
		_title_crystal[16] = createSprite( Sprite::TYPE::CRYSTAL_081 );
		_title_crystal[17] = createSprite( Sprite::TYPE::CRYSTAL_002 );
		_title_crystal[18] = createSprite( Sprite::TYPE::CRYSTAL_044 );
		_title_crystal[19] = createSprite( Sprite::TYPE::CRYSTAL_036 );
	}

	void putTitleSceneCrystal( ) {
		createTitleSceneCrystal( );
		_title_crystal[0]->setPos( 2, 1, false );
		_title_crystal[1]->setPos( 4, 1, false );
		_title_crystal[2]->setPos( 7, 1, false );
		_title_crystal[3]->setPos( 11, 1, false );
		_title_crystal[4]->setPos( 17, 1, false );
		_title_crystal[5]->setPos( 21, 1, false );
		_title_crystal[6]->setPos( 25, 1, false );
		_title_crystal[7]->setPos( 3, 2, false );
		_title_crystal[8]->setPos( 8, 2, false );
		_title_crystal[9]->setPos( 12, 2, false );
		_title_crystal[10]->setPos( 14, 2, false );
		_title_crystal[11]->setPos( 19, 2, false );
		_title_crystal[12]->setPos( 22, 2, false );
		_title_crystal[13]->setPos( 27, 2, false );
		_title_crystal[14]->setPos( 10, 3, false );
		_title_crystal[15]->setPos( 15, 3, false );
		_title_crystal[16]->setPos( 18, 3, false );
		_title_crystal[17]->setPos( 24, 3, false );
		_title_crystal[18]->setPos( 26, 3, false );
		_title_crystal[19]->setPos( 29, 3, false );
	}

	void makeTitleScenePlate( ) {
		PlatePtr up_title_plate = createPlate( );
		up_title_plate->setAnchor( Plate::ANCHOR::CENTER_HALFTOP );
		up_title_plate->setText( "Stray            " );
		up_title_plate->setTextExtend( 3.0 );
		up_title_plate->offFrame( );
		PlatePtr center_title_plate = createPlate( );
		center_title_plate->setAnchor( Plate::ANCHOR::CENTER_HALFTOP );
		center_title_plate->setText( "      into       " );
		center_title_plate->setTextExtend( 1.5 );
		center_title_plate->offFrame( );
		PlatePtr down_title_plate = createPlate( );
		down_title_plate->setAnchor( Plate::ANCHOR::CENTER_HALFTOP );
		down_title_plate->setText( "           Jewels" );
		down_title_plate->setTextExtend( 3.0 );
		down_title_plate->offFrame( );
		_brinking_plate = createPlate( );
		_brinking_plate->setAnchor( Plate::ANCHOR::CENTER_HALFBOTTOM );
		_brinking_plate->offFrame( );
	}

	void brinkTitleText( ) {
		if ( _frame_count % 60 > 0 && _frame_count % 60 < 15 ) {
			_brinking_plate->setText( "                    " );
		}
		else {
			_brinking_plate->setText( "plese press A button" );
		}
		_frame_count++;
	}

	void moveTitleCamera( ) {
		_title_camera->setPos( _TITLE_MAP_SCALE_X, _INIT_TITLE_CAMERA_POS_Y, true );
		int camera_pos_x = ( int )_title_camera->getPosX( );
		if ( _TITLE_MAP_SCALE_X - 5 <= camera_pos_x ) {
			_title_camera->setPos( _INIT_TITLE_CAMERA_POS_X, _INIT_TITLE_CAMERA_POS_Y, false );
		}
	}

	void onPushButtonAAsync( ) {
		if ( _title_scene ) {
			fade( 1.0 );
			_title_camera->stop( );
			_frame_count = 0;
			_title_scene = false;
			_state = STATE::SCENE_TRANSITION;
		}
		_time_out_count = 0;
	}

	void update( ) override {
		switch ( _state ) {
		case STATE::SCENE_TRANSITION:
			moveGameScene( );
			break;
		case STATE::RESET_JEWEL:
			updateStateReset( );
			break;
		case STATE::INPUT:
			updateStateInput( );
			break;
		case STATE::MOVE_LEFT:
			updateStateMove( ARROW::LEFT );
			break;
		case STATE::MOVE_RIGHT:
			updateStateMove( ARROW::RIGHT );
			break;
		case STATE::MOVE_UP:
			updateStateMove( ARROW::UP );
			break;
		case STATE::MOVE_DOWN:
			updateStateMove( ARROW::DOWN );
			break;
		case STATE::GET_KEY:
			updateGetKey( );
			break;
		case STATE::CREATE_NEW_JEWEL:
			updateStateCreateNewJewel( );
			break;
		case STATE::RESET_STAGE:
			updateResetStage( );
			break;
		}

		_arrow = ARROW::NONE;
	}

	void moveGameScene( ) {
		deleteSprites( );
		deletePlates( );
		_title_map->clear( );
		fade( 0.0 );
		g_ppap = this;
		makeGameStage( );
		initJewel( );
		_state = STATE::RESET_JEWEL;
	}

	void makeGameStage( ) {
		createJewelList( );
		createStageText( );
		createHowToMoveText( );
		createTargetKeyText( );
		adjustCamera( );
		makeGameStageMap( );
		indicateKeyNum( );
	}

	void createJewelList( ) {
		std::array< SpritePtr, MAX_LEVEL + 1 > jewel;
		SpritePtr key = createSprite( Sprite::TYPE::KEY );
		key->setView( false );
		for ( int i = 0; i < MAX_LEVEL + 1; i++ ) {
			if ( i == 9 ) {
				jewel[i] = key;
			}
			else {
				jewel[i] = createSprite( LEVEL_TO_TYPE[i] );
			}
			jewel[i]->setView( false );
			_jewel_list[i] = createPlate( );
			_jewel_list[i]->setSprite( jewel[i] );
			_jewel_list[i]->setAnchor( Plate::ANCHOR::LEFT_CENTER );
			_jewel_level[i] = createPlate( );
			char buf[7] = "level0";
			buf[5] = '0' + i + 1;
			if ( i == 9 ) {
				_jewel_level[i]->setText( "level10" );
				_jewel_list[i]->setSpriteExtend( 1.0 );
				_jewel_level[i]->setTextExtend( 1.0 );
			}
			else {
				_jewel_level[i]->setText( buf );
				_jewel_list[i]->setSpriteExtend( 0.5 );
				_jewel_level[i]->setTextExtend( 0.7 );
			}
			_jewel_level[i]->setAnchor( Plate::ANCHOR::LEFT_CENTER );
		}
		if ( _stage_count < 10 ) {
			_jewel_list[JEWEL_LEVEL[_stage_count]]->setSprite( key );
			_jewel_list[JEWEL_LEVEL[_stage_count]]->setSpriteExtend( 1.0 );
			_jewel_level[JEWEL_LEVEL[_stage_count]]->setTextExtend( 1.0 );
			_jewel_list[9]->hide( false );
			_jewel_level[9]->hide( false );
		}
	}

	void createStageText( ) {
		_stage_num = createPlate( );
		_stage_num->setAnchor( Plate::ANCHOR::CENTER_TOP );
		_stage_num->setTextExtend( 2.0 );
		char buf[7] = "stage0";
		if ( _stage_count == 10 ) {
			_stage_num->setText( "stage10" );
		}
		else {
			buf[5] = '0' + _stage_count;
			_stage_num->setText( buf );
		}
	}

	void createHowToMoveText( ) {
		_how_to_move1 = createPlate( );
		_how_to_move1->setAnchor( Plate::ANCHOR::RIGHT_TOP );
		_how_to_move1->setText( "how to move :    " );
		_how_to_move1->setTextExtend( 1.1 );
		_how_to_move1->offFrame( );
		_how_to_move2 = createPlate( );
		_how_to_move2->setAnchor( Plate::ANCHOR::RIGHT_TOP );
		_how_to_move2->setText( "     ËÀÞØ½Ã¨¯¸" );
		_how_to_move2->setTextExtend( 1.1 );
		_how_to_move2->offFrame( );
	}

	void createTargetKeyText( ) {
		_hidden_plate = createPlate( );
		_hidden_plate->setAnchor( Plate::ANCHOR::RIGHT_TOP );
		_hidden_plate->setText( "  " );
		_hidden_plate->offFrame( );
		_target_keys = createPlate( );
		_target_keys->setAnchor( Plate::ANCHOR::RIGHT_TOP );
		char buf[14] = "target keys:1";
		buf[12] = '0' + KEY_HAVE[_stage_count];
		_target_keys->setText( buf );
		_target_keys->show( false );
	}

	void adjustCamera( ) {
		CameraPtr camera = getCamera( );
		camera->setPos( ( MAP_SIZE + 1 ) / 2.0, ( MAP_SIZE + 1 ) / 2.0 - 0.5, false );
		camera->setRatio( 1.3 );
	}

	void makeGameStageMap( ) {
		_game_stage_map = getMap( );
		for ( int i = 0; i < MAP_SIZE; i++ ) {
			for ( int j = 0; j < MAP_SIZE; j++ ) {
				_game_stage_map->setStreet( j + FIELD_ORIGIN_X, i + FIELD_ORIGIN_Y, true );
				_grid[( size_t )j + ( size_t )i * 4] = createSprite( Sprite::TYPE::CURSOR_2 );
				_grid[( size_t )j + ( size_t )i * 4]->setExtend( 1.18 );
				_grid[( size_t )j + ( size_t )i * 4]->setPos( ( double )j + FIELD_ORIGIN_X, ( double )i + FIELD_ORIGIN_Y, false );
			}
		}
	}

	void indicateKeyNum( ) {
		SpritePtr key = createSprite( Sprite::TYPE::KEY );
		key->setView( false );
		_key_num = createPlate( );
		_key_num->setSprite( key );
		countKeyNum( 0 );
		_key_num->setAnchor( Plate::ANCHOR::RIGHT_HALFTOP );
	}

	void initJewel( ) {
		#if 1
		spawnJewel( );
		spawnJewel( );
		#else
		createJewel( 0, 0, 8 );
		createJewel( 0, 1, 8 );
		#endif
	}

	void createJewel( int x, int y, int level ) {
		JewelPtr jewel( new Jewel( x, y, level ) );
		setField( x, y, jewel );
	}

	void setField( int x, int y, JewelPtr jewel ) {
		size_t idx = ( size_t )x + ( size_t )y * MAP_SIZE;
		_field[idx] = jewel;
	}

	JewelPtr getField( int x, int y ) const {
		size_t idx = ( size_t )x + ( size_t )y * MAP_SIZE;
		return _field[idx];
	}

	void updateStateReset( ) {
		for ( int j = 0; j < MAP_SIZE; j++ ) {
			for ( int i = 0; i < MAP_SIZE; i++ ) {
				JewelPtr jewel = getField( i, j );
				if ( !jewel ) {
					continue;
				}
				jewel->reset( );
			}
		}
		_state = STATE::INPUT;
	}

	void updateStateInput( ) {
		switch ( _arrow ) {
		case ARROW::LEFT:
			_state = STATE::MOVE_LEFT;
			break;
		case ARROW::RIGHT:
			_state = STATE::MOVE_RIGHT;
			break;
		case ARROW::UP:
			_state = STATE::MOVE_UP;
			break;
		case ARROW::DOWN:
			_state = STATE::MOVE_DOWN;
			break;
		}
	}

	void updateStateMove( ARROW move_arrow ) {
		bool moving = moveJewels( move_arrow );
		if ( !moving ) {
			for ( int j = 0; j < MAP_SIZE; j++ ) {
				for ( int i = 0; i < MAP_SIZE; i++ ) {
					JewelPtr jewel = getField( i, j );
					if ( jewel ) {
						if ( jewel->getLevel( ) == 99 ) {
							jewel->setPos( 7, 0 );
						}
					}
				}
			}
			if ( isGameOver( ) ) {
				becomeGameOver( );
			}
			else {
				_state = STATE::GET_KEY;
			}
		}
	}

	bool moveJewels( ARROW move_arrow ) {
		bool moving = false;
		for ( int j = 0; j < MAP_SIZE; j++ ) {
			for ( int i = 0; i < MAP_SIZE - 1; i++ ) {
				int dst_x = 0;
				int dst_y = 0;
				int src_x = 0;
				int src_y = 0;
				switch ( move_arrow ) {
				case ARROW::LEFT:
					dst_x = i;
					dst_y = j;
					src_x = dst_x + 1;
					src_y = dst_y;
					break;
				case ARROW::RIGHT:
					dst_x = MAP_SIZE - 1 - i;
					dst_y = j;
					src_x = dst_x - 1;
					src_y = dst_y;
					break;
				case ARROW::UP:
					dst_x = j;
					dst_y = i;
					src_x = dst_x;
					src_y = dst_y + 1;
					break;
				case ARROW::DOWN:
					dst_x = j;
					dst_y = MAP_SIZE - 1 - i;
					src_x = dst_x;
					src_y = dst_y - 1;
					break;
				}
				if ( moveJewel( src_x, src_y, dst_x, dst_y ) ) {
					moving = true;
					_is_moving_jewel = true;
				}
			}
		}
		return moving;
	}

	bool moveJewel( int src_x, int src_y, int dst_x, int dst_y ) {
		JewelPtr src_jewel = getField( src_x, src_y );
		if ( !src_jewel ) {
			return false;
		}
		if ( src_jewel->isFirst( ) ) {
			return false;
		}
		JewelPtr dst_jewel = getField( dst_x, dst_y );
		if ( dst_jewel ) {
			if ( src_jewel->getLevel( ) != dst_jewel->getLevel( ) ) {
				return false;
			}
			if ( dst_jewel->isFirst( ) ) {
				return false;
			}
			int level = src_jewel->getLevel( ) + 1;
			if ( isGetkeyJewelLevel( level ) ) {
				level = 99;
				_key_count++;
			}
			createJewel( src_x, src_y, level );
			src_jewel = getField( src_x, src_y );
			causeEffect( ( double )dst_x + 1, ( double )dst_y + 1 );
		}
		setField( src_x, src_y, JewelPtr( ) );
		src_jewel->setPos( dst_x, dst_y );
		setField( dst_x, dst_y, src_jewel );
		return true;
	}

	bool isSyntheticJewelOneLevel( int col, int row, int src_level ) const {
		JewelPtr jewel = getField( col, row );
		if ( !jewel ) {
			return false;
		}
		if ( jewel->getLevel( ) != src_level ) {
			return false;
		}
		return true;
	}

	bool isSyntheticJewelOne( int col, int row ) const {
		JewelPtr src_jewel = getField( col, row );
		if ( !src_jewel ) {
			return false;
		}
		int src_level = src_jewel->getLevel( );

		if ( col > 0 ) {
			if ( isSyntheticJewelOneLevel( col - 1, row, src_level ) ) {
				return true;
			}
		}
		if ( col < 3 ) {
			if ( isSyntheticJewelOneLevel( col + 1, row, src_level ) ) {
				return true;
			}
		}
		if ( row > 0 ) {
			if ( isSyntheticJewelOneLevel( col, row - 1, src_level ) ) {
				return true;
			}
		}
		if ( row < 3 ) {
			if ( isSyntheticJewelOneLevel( col, row + 1, src_level ) ) {
				return true;
			}
		}

		return false;
	}

	bool isSyntheticJewel( ) const {
		for ( int j = 0; j < MAP_SIZE; j++ ) {
			for ( int i = 0; i < MAP_SIZE; i++ ) {
				if ( isSyntheticJewelOne( i, j ) ) {
					return true;
				}
			}
		}
		return false;
	}

	bool isGameOver( ) {
		int jewel_count = 0;
		for ( int i = 0; i < MAP_SIZE; i++ ) {
			for ( int j = 0; j < MAP_SIZE; j++ ) {
				JewelPtr jewel = getField( j, i );
				if ( jewel ) {
					jewel_count++;
				}
			}
		}
		if ( jewel_count == 16 && !isSyntheticJewel( ) ) {
			return true;
		}
		return false;
	}

	void becomeGameOver( ) {
		_game_over_plate = createPlate( );
		_game_over_plate->setText( "GAME OVER" );
		_game_over_plate->setTextExtend( 1.4 );
		_game_over_plate->setBeforeFade( true );
		setFadeSpeed( 0.015 );
		fade( 1.0 );
		_state = STATE::RESET_STAGE;
	}

	bool isGetkeyJewelLevel( int level ) {
		return JEWEL_LEVEL[_stage_count] == level;
	}

	void updateGetKey( ) {
		for ( int i = 0; i < MAP_SIZE; i++ ) {
			for ( int j = 0; j < MAP_SIZE; j++ ) {
				JewelPtr jewel = getField( j, i );
				if ( !jewel ) {
					continue;
				}
				if ( jewel->getLevel( ) == 99 ) {
					causeEffect( 8.0, 0.5 );
					setField( j, i, JewelPtr( ) );
					countKeyNum( _key_count );
				}
			}
		}
		if ( isStageClear( ) ) {
			clearStage( );
		}
		else {
			if ( _is_moving_jewel ) {
				_state = STATE::CREATE_NEW_JEWEL;
				_is_moving_jewel = false;
			}
			else {
				_state = STATE::RESET_JEWEL;
			}
		}
	}

	bool isStageClear( ) {
		return KEY_HAVE[_stage_count] <= _key_count;
	}

	void countKeyNum( int count ) {
		switch ( count ) {
		case 0:
			_key_num->setText( "0" );
			break;
		case 1:
			_key_num->setText( "1" );
			break;
		case 2:
			_key_num->setText( "2" );
			break;
		case 3:
			_key_num->setText( "3" );
			break;
		case 4:
			_key_num->setText( "4" );
			break;
		}
	}

	void clearStage( ) {
		_clear_plate = createPlate( );
		_clear_plate->setText( "STAGE CLEAR" );
		_clear_plate->setTextExtend( 1.3 );
		_clear_plate->setBeforeFade( true );
		_is_moving_jewel = false;
		_stage_count++;
		setFadeSpeed( 0.015 );
		fade( 1.0 );
		_state = STATE::RESET_STAGE;
		_time_out_count = 0;
	}

	void updateResetStage( ) {
		for ( int i = 0; i < MAP_SIZE; i++ ) {
			for ( int j = 0; j < MAP_SIZE; j++ ) {
				JewelPtr jewel = getField( j, i );
				if ( jewel ) {
					setField( j, i, JewelPtr( ) );
				}
			}
		}
		deletePlate( _clear_plate );
		deletePlate( _game_over_plate );
		deletePlate( _stage_num );
		deletePlate( _target_keys );
		deletePlate( _hidden_plate );
		for ( int i = 0; i < MAX_LEVEL + 1; i++ ) {
			deletePlate( _jewel_level[i] );
			deletePlate( _jewel_list[i] );
		}
		_key_count = 0;
		if ( _stage_count == 11 ) {
			clearAllStage( );
		}
		else {
			createJewelList( );
			countKeyNum( 0 );
			initJewel( );
			createStageText( );
			createTargetKeyText( );
			fade( 0.0 );
			_state = STATE::RESET_JEWEL;
		}
	}

	void clearAllStage( ) {
		_all_clear_plate = createPlate( );
		_all_clear_plate->setAnchor( Plate::ANCHOR::CENTER_CENTER );
		_all_clear_plate->setText( "All Stage Clear" );
		_all_clear_plate->setTextExtend( 2.0 );
		_all_clear_plate->setBeforeFade( true );
		deletePlate( _how_to_move1 );
		deletePlate( _how_to_move2 );
		_game_stage_map->clear( );
		deletePlate( _key_num );
		for ( int i = 0; i < MAP_SIZE; i++ ) {
			for ( int j = 0; j < MAP_SIZE; j++ ) {
				deleteSprite( _grid[( size_t )j + ( size_t )i * 4] );
			}
		}
		for ( int i = 0; i < MAX_LEVEL + 1; i++ ) {
			deletePlate( _jewel_list[i] );
			deletePlate( _jewel_level[i] );
		}
		_stage_count = 1;
		_title_scene = true;
		_state = STATE::RESET_JEWEL;
	}

	void updateStateCreateNewJewel( ) {
		spawnJewel( );
		_state = STATE::RESET_JEWEL;
	}

	void spawnJewel( ) {
		int rand_pos_x = -1;
		int rand_pos_y = -1;
		do {
			rand_pos_x = random( 3 );
			rand_pos_y = random( 3 );
		} while ( getField( rand_pos_x, rand_pos_y ) != nullptr );
		createJewel( rand_pos_x, rand_pos_y, random( 1 ) );
	}

	void onPushArrowLeft( ) {
		_arrow = ARROW::LEFT;
		_time_out_count = 0;
	}

	void onPushArrowRight( ) {
		_arrow = ARROW::RIGHT;
		_time_out_count = 0;
	}

	void onPushArrowUp( ) {
		_arrow = ARROW::UP;
		_time_out_count = 0;
	}

	void onPushArrowDown( ) {
		_arrow = ARROW::DOWN;
		_time_out_count = 0;
	}
};

PPAP_GENERATOR( Controller, 20210430 );
