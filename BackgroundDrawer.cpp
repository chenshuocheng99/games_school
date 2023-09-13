#include "BackgroundDrawer.h"
#include "Drawer.h"
#include "Primitive.h"
#include "StagePlaying.h"
#include "Player.h"
#include "BackgroundManager.h"

extern unsigned char getPcType( );

BackgroundDrawer::BackgroundDrawer( ) {

	_model_floor_stage01 = ModelPtr( new Model );
	_model_floor_stage01->load( "Resources/Model/Stage/Ground_Plate2.mv1" );
	_model_ceiling_stage01 = ModelPtr( new Model );
	_model_ceiling_stage01->load( "Resources/Model/Stage/Ramp1_2m_L0.mv1" );
	_model_wall_stage01 = ModelPtr( new Model );
	_model_wall_stage01->load( "Resources/Model/Stage/Platform_Wall_04_L1.mv1" );
	_model_box_stage02 = ModelPtr( new Model );
	_model_box_stage02->load( "Resources/Model/Stage/Wall_grid_Int.mv1" );
	_model_floor_stage03 = ModelPtr( new Model );
	_model_floor_stage03->load( "Resources/Model/Stage/Concrete_wall1.mv1" );
	_model_stairs_stage03 = ModelPtr( new Model );
	_model_stairs_stage03->load( "Resources/Model/Stage/Concrete_Wall1_Stairs1.mv1" );
	_model_pillar_stage03 = ModelPtr( new Model );
	_model_pillar_stage03->load( "Resources/Model/Stage/Concrete_wall1_Courner.mv1" );

	_width = 0;
	_depth = 0;
	_height = 0;
}

BackgroundDrawer::~BackgroundDrawer( ) {
}

void BackgroundDrawer::draw( ) {
	DrawerPtr  drawer = Task::get< Drawer >( );
	drawer->setUseLight( true );
	drawer->setLight( Vector( 0, -1, 0 ), Vector( 1, 1, 1 ) );
	drawer->setLight( Vector( 0, -1, 0 ), Vector( -1, -1, -1 ) );

	BackgroundManagerPtr manager = Task::get< BackgroundManager >( );
	BackgroundManager::STAGE stage = manager->getStage( );
	stage = BackgroundManager::STAGE::STAGE02;
	switch ( stage ) {
	case BackgroundManager::STAGE::STAGE01:
		makeFloorStage01( );//下二つの関数含めステージ案１
		makeCeilingStage01( );
		makeWallStage01( );
		break;
	case BackgroundManager::STAGE::STAGE02:
		makeBoxStage02( );//ステージ案２
		break;
	case BackgroundManager::STAGE::STAGE03:
		makeBoxStage02( );//ステージ案２
		break;
	case BackgroundManager::STAGE::STAGE04:
		makeBoxStage02( );//ステージ案２
		break;
	}
	drawer->setUseLight( false );
}

void BackgroundDrawer::makeFloorStage01( ) {
	StagePlayingPtr stage_playing = Task::get< StagePlaying >( );
	double height = 1.0;
	Matrix mat_scale = Matrix::makeTransformScaling( Vector( 0.013, 0.007, 0.03 ) );
	OculusPtr oculus = Task::get< Oculus >( );
	if ( oculus->isCreated( ) ) {
		PlayerPtr player = Task::get< Player >( );
		height = player->getHeight( );// console
	} else if ( getPcType( ) == CONNECT_DATA_PCTYPE_CONSOLE ) {
		height = stage_playing->getHeight( ) / 2;//simulator
	}
	Matrix mat_pos = Matrix::makeTransformTranslation( Vector( 0, -height - 0.22, 0 ) );
	_model_floor_stage01->setTrans( mat_pos );
	_model_floor_stage01->setScale( mat_scale );
	_model_floor_stage01->draw( );
}

void BackgroundDrawer::makeCeilingStage01( ) {
	StagePlayingPtr stage_playing = Task::get< StagePlaying >( );

	double height = 1.0;
	Matrix mat_scale = Matrix::makeTransformScaling( Vector( 0.05, 0.05, 0.05 ) );
	OculusPtr oculus = Task::get< Oculus >( );
	if ( oculus->isCreated( ) ) {
		PlayerPtr player = Task::get< Player >( );
		height = player->getHeight( );// console
	} else if ( getPcType( ) == CONNECT_DATA_PCTYPE_CONSOLE ) {
		height = stage_playing->getHeight( ) / 2;//simulator
	}
	Matrix mat_pos = Matrix::makeTransformTranslation( Vector( 3, -height + 4.5, 0 ) );
	_model_ceiling_stage01->setTrans( mat_pos );
	_model_ceiling_stage01->setScale( mat_scale );
	_model_ceiling_stage01->draw( );
}

void BackgroundDrawer::makeWallStage01( ) {
	StagePlayingPtr stage_playing = Task::get< StagePlaying >( );
	double height = 1.0;
	Matrix mat_scale = Matrix::makeTransformScaling( Vector( 0.01, 0.012, 0.014 ) );
	OculusPtr oculus = Task::get< Oculus >( );
	if ( oculus->isCreated( ) ) {
		PlayerPtr player = Task::get< Player >( );
		height = player->getHeight( );// console
	} else if ( getPcType( ) == CONNECT_DATA_PCTYPE_CONSOLE ) {
		height = stage_playing->getHeight( ) / 2;//simulator
	}
	Matrix mat_pos[ 4 ] = {
		Matrix::makeTransformTranslation( Vector( 2.5, -height - 0.2, 0 ) ) ,
		Matrix::makeTransformTranslation( Vector( -2.5, -height - 0.2, 0 ) ),
		Matrix::makeTransformTranslation( Vector( 0, -height - 0.2, 5.3 ) ) ,
		Matrix::makeTransformTranslation( Vector( 0, -height - 0.2, -5.3 ) )
	};
	Matrix mat_rot[ 4 ] = {
		Matrix::makeTransformRotation( Vector( 0, 0, 0 ), 0 ) ,
		Matrix::makeTransformRotation( Vector( 0, 0, 0 ), 0 ) ,
		Matrix::makeTransformRotation( Vector( 0, 1, 0 ), -1.57 ) ,
		Matrix::makeTransformRotation( Vector( 0, 1, 0 ), -1.57 )
	};
	for ( int i = 0; i < 4; i++ ) {
		_model_wall_stage01->setTrans( mat_pos[ i ] );
		_model_wall_stage01->setScale( mat_scale );
		_model_wall_stage01->setRot( mat_rot[ i ] );
		_model_wall_stage01->draw( );
	}
}

void BackgroundDrawer::makeBoxStage02( ) {
	StagePlayingPtr stage_playing = Task::get< StagePlaying >( );
	double height = 1.0;
	Matrix mat_scale = Matrix::makeTransformScaling( Vector( 0.02, 0.02, 0.04 ) );
	Matrix mat_rot[ 2 ] = {
		Matrix::makeTransformRotation( Vector( 0, 0, 0 ), 0 ),
		Matrix::makeTransformRotation( Vector( 0, 1, 0 ), -3.14 )
	};
	OculusPtr oculus = Task::get< Oculus >( );
	if ( oculus->isCreated( ) ) {
		PlayerPtr player = Task::get< Player >( );
		height = player->getHeight( );// console
	} else if ( getPcType( ) == CONNECT_DATA_PCTYPE_CONSOLE ) {
		height = stage_playing->getHeight( ) / 2;//simulator
	}
	Matrix mat_pos = Matrix::makeTransformTranslation( Vector( 0, -height - 0.1, 0 ) );
	for ( int i = 0; i < 2; i++ ) {
		_model_box_stage02->setTrans( mat_pos );
		_model_box_stage02->setScale( mat_scale );
		_model_box_stage02->setRot( mat_rot[ i ] );
		_model_box_stage02->draw( );
	}
}

void BackgroundDrawer::makeFloorStage03( ) {
	StagePlayingPtr stage_playing = Task::get< StagePlaying >( );
	double height = 1;
	Matrix mat_scale = Matrix::makeTransformScaling( Vector( 0.035, 0.006, 0.065 ) );
	Matrix mat_rot = Matrix::makeTransformRotation( Vector( 1, 0, 0 ), -1.57 );
	OculusPtr oculus = Task::get< Oculus >( );
	if ( oculus->isCreated( ) ) {
		PlayerPtr player = Task::get< Player >( );
		height = player->getHeight( );// console
	} else if ( getPcType( ) == CONNECT_DATA_PCTYPE_CONSOLE ) {
		height = stage_playing->getHeight( ) / 2;//simulator
	}
	Matrix mat_pos = Matrix::makeTransformTranslation( Vector( 0, -height - 0.5, -9.8 ) );
	_model_floor_stage03->setTrans( mat_pos );
	_model_floor_stage03->setScale( mat_scale );
	_model_floor_stage03->setRot( mat_rot );
	_model_floor_stage03->draw( );
}