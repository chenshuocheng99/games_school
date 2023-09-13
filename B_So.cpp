#include "Impulse.h"

GAME_BEGIN( B_So );

int _set_button[ 12 ][ 6 ] = { 0 };
int _num_countdown = 5;
int _number = 0;
int _mark_on_button[ 12 ][ 6 ] = { 0 };
int _mark_match = 0;
int _mark_only = 0;
int _number_plate = 0;
int _number_frame = 0;


int randMark( ) {
	int mark_rand = 0;
	switch ( rand( ) % 4 ) {
	case 0:
		mark_rand = MARK_DRINK;
		break;
	case 1:
		mark_rand = MARK_SHOES;
		break;
	case 2:
		mark_rand = MARK_WATCH;
		break;
	case 3:
		mark_rand = MARK_MISANGA;
		break;
	}
	return mark_rand;
}

void setButton( ) {
	for ( int i = 0; i < 12; i++ ) {
		for ( int j = 0; j < 6; j++ ) {
			int button_x = i * 135 + 225;
			int button_y = j * 135 + 275;
			_set_button[ i ][ j ] = createAsset( ASSET::BUTTON );
			setAsset( _set_button[ i ][ j ], CMD::POS_X, button_x );
			setAsset( _set_button[ i ][ j ], CMD::POS_Y, button_y );
		}
	}
}

void setupMark( ) {
	_mark_match = randMark( );
	for ( int i = 0; i < 12; i++ ) {
		for ( int j = 0; j < 6; j++ ) {
			_mark_on_button[ i ][ j ] = createAsset( ASSET::MARK );
			setAsset( _mark_on_button[ i ][ j ], CMD::POS_X, i * 135 + 225 );
			setAsset( _mark_on_button[ i ][ j ], CMD::POS_Y, j * 135 + 275 );
			setAsset( _mark_on_button[ i ][ j ], CMD::STATUS, _mark_match );
		}
	}
	_mark_only = randMark( );
	while ( _mark_only == _mark_match ) {
		_mark_only = randMark( );
	}
	setAsset( _mark_on_button[ rand( ) % 12 ][ rand( ) % 6 ], CMD::STATUS, _mark_only );
}

void changeMark( ) {
	_mark_match = randMark( );
	for ( int i = 0; i < 12; i++ ) {
		for ( int j = 0; j < 6; j++ ) {
			int button_x = i * 135 + 225;
			int button_y = j * 135 + 275;
			setAsset( _mark_on_button[ i ][ j ], CMD::POS_X, button_x );
			setAsset( _mark_on_button[ i ][ j ], CMD::POS_Y, button_y );
			setAsset( _mark_on_button[ i ][ j ], CMD::STATUS, _mark_match );
		}
	}
	_mark_only = randMark( );
	while ( _mark_only == _mark_match ) {
		_mark_only = randMark( );
	}
	setAsset( _mark_on_button[ rand( ) % 12 ][ rand( ) % 6 ], CMD::STATUS, _mark_only );
}

void countNumber( ) {
	const int NUM_POS_X = 1750;
	const int NUM_POS_Y = 125;
	stopwatch( STOPWATCH::START );
	_number = createAsset( ASSET::NUMBER );
	setAsset( _number, CMD::STATUS, _num_countdown );
	setAsset( _number, CMD::POS_X, NUM_POS_X );
	setAsset( _number, CMD::POS_Y, NUM_POS_Y );
	_number_frame = createAsset( ASSET::FRAME );
	setAsset( _number_frame, CMD::POS_X, NUM_POS_X );
	setAsset( _number_frame, CMD::POS_Y, NUM_POS_Y );
	_number_plate = createAsset( ASSET::PLATE );
	setAsset( _number_plate, CMD::STATUS, PLATE_REMAINING );
	setAsset( _number_plate, CMD::POS_X, NUM_POS_X );
	setAsset( _number_plate, CMD::POS_Y, NUM_POS_Y - 60 );
}

void setup( ) {
	_num_countdown = 5;
	_number = 0;
	_mark_match = 0;
	_mark_only = 0;
	setButton( );
	countNumber( );
	setupMark( );
	creator( "ゲームプログラマー専攻 １年", "ソウ ゲイカ" );
}


void update( ) {
	int touch = 0;
	int mark_touch = 0;
	int mark_left_up = 0;
	int mark_right_up = 0;
	int mark_left_down = 0;
	int mark_right_down = 0;
	for ( int i = 0; i < 12; i++ ) {
		for ( int j = 0; j < 6; j++ ) {
			touch = getAsset( _set_button[ i ][ j ], CMD::TOUCH );
			if ( touch == 1 ) {
				mark_touch = getAsset( _mark_on_button[ i ][ j ], CMD::STATUS );
				mark_left_up = getAsset( _mark_on_button[ 0 ][ 0 ], CMD::STATUS );
				mark_right_up = getAsset( _mark_on_button[ 11 ][ 0 ], CMD::STATUS );
				mark_left_down = getAsset( _mark_on_button[ 0 ][ 5 ], CMD::STATUS );
				mark_right_down = getAsset( _mark_on_button[ 11 ][ 5 ], CMD::STATUS );
				if ( ( mark_touch != mark_left_up && mark_touch != mark_left_down ) ||
					 ( mark_touch != mark_left_up && mark_touch != mark_right_down ) ||
					 ( mark_touch != mark_left_up && mark_touch != mark_right_up ) ||
					 ( mark_touch != mark_left_down && mark_touch != mark_right_down ) ||
					 ( mark_touch != mark_right_up && mark_touch != mark_left_down ) ||
					 ( mark_touch != mark_right_up && mark_touch != mark_right_down ) ) {
					_num_countdown--;
					setAsset( _number, CMD::STATUS, _num_countdown );
					changeMark( );
				}
			}
			if ( _num_countdown == 0 ) {
				finish( );
			}
		}
	}
}

STRING info( ) { // パフォーマンスモードでゲーム開始前に表示されるゲーム説明を登録します。１行１４文字で４行までです。
	return "違うマークのボタンをタッチしよう！";
}

GAME_END;