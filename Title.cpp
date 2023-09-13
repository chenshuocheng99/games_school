#include "Title.h"

#include "define.h"

#include "Drawer.h"
#include "Image.h"

//タイトル画面配置とアニメーション演出

Title::Title( ) {
	DrawerPtr drawer = Task::get< Drawer >( );
	_image_title = drawer->createImage( "resource/title/attraction_logo.png" );
	_image_prepare = drawer->createImage( "resource/title/title_backgroung.png" );
}

Title::~Title( ) {

}

void Title::draw( ) const {
	double ratio = getHideRatio( );
	int hide_y = ratio * Define::SCREEN_HEIGHT;

	int title_width;
	int title_height;
	_image_title->getImageSize( title_width, title_height );
	_image_title->draw(
		( Define::SCREEN_WIDTH - title_width ) / 2,
		( Define::SCREEN_HEIGHT - title_height ) / 2 - hide_y );
}

void Title::drawPrepare( ) const {
	int title_width;
	int title_height;
	_image_title->getImageSize( title_width, title_height );
	_image_title->draw(
		( Define::SCREEN_WIDTH - title_width ) / 2,
		( Define::SCREEN_HEIGHT - title_height ) / 2 );

	int prepare_width;
	int prepare_height;
	_image_prepare->getImageSize( prepare_width, prepare_height );
	_image_prepare->draw(
		Define::SCREEN_WIDTH - prepare_width,
		Define::SCREEN_HEIGHT - prepare_height );
}