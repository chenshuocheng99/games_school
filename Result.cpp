#include "Result.h"
#include "Status.h"

#include "Drawer.h"

#include <array>


//今回のゲームの結果→　over　or clear(結果提示)

//結果出すパフォーマンス用
bool is_fade = false;
double fade_count = 0;

Result::Result( StatusConstPtr status )
	: _status( status ) {
	DrawerPtr drawer = Task::get<Drawer>( );
	_image[ IMG::FADE ] = drawer->createImage( "resource/result/game_fade.png" );
	_image[ IMG::OVER ] = drawer->createImage( "resource/result/game_over.png" );
	_image[ IMG::CLEAR ] = drawer->createImage( "resource/result/game_clear.png" );
}

Result::~Result( ) {

}

void Result::show( ) {
	is_fade = true;
}

void Result::hide( ) {
	is_fade = false;
}

void Result::draw( ) const {
	DrawerPtr drawer = Task::get< Drawer >( );
	Drawer::Scope scope;
	if ( is_fade ) {
		fade_count += 0.01;
	} else {
		fade_count = 0;
	}
	drawer->setBlend( scope, Drawer::BLEND::ALPHA, fade_count );
	_image.at( IMG::FADE )->draw( Vector2( 0, 0 ) );
	
	switch ( _status->getPhaseType( ) ) {
	case PHASE::GAME_OVER:
		drawer->setBlend( scope, Drawer::BLEND::ALPHA, fade_count * 0.5 );
		_image.at( IMG::OVER )->draw( Vector2( 0, Define::SCREEN_HEIGHT / 2 - 100 ) );
		break;
	case PHASE::GAME_CLEAR:
		drawer->setBlend( scope, Drawer::BLEND::ALPHA, fade_count * 0.5 );
		_image.at( IMG::CLEAR )->draw( Vector2( 0, Define::SCREEN_HEIGHT / 2 - 100 ) );
		break;
	}
}
