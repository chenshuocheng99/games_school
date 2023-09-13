#include "define.h"

#include "Introduction.h"
#include "Drawer.h"
#include "Client.h"
#include "Keyboard.h"
#include "Log.h"
#include "Application.h"
#include "Sound.h"

void Introduction::procedure( ) {
	//�Q�[����ʎ����p�N���X����
	ApplicationPtr app = Application::getInstance( );
	app->setScreenSize( Define::SCREEN_WIDTH, Define::SCREEN_HEIGHT );

	Task::create< Drawer >( );
	Task::create< Client >( );
	Task::create< Keyboard >( );
	Task::create< Log >( );
	Task::create< Sound >( );

	LogPtr log = Task::get< Log >( );
	log->toggleShow( );

	REGIST( SceneMonitor );
	start( "SceneMonitor" );
}
