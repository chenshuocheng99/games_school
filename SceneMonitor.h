#pragma once
#include "Scene.h"
#include "Model.h"
#include "Primitive.h"
#include "define.h"
#include "Image.h"

#include <array>

PTR( Status );
PTR( Map );
PTR( Player );
PTR( Enemy );
PTR( Roulette );
PTR( Diorama );
PTR( Title );
PTR( Text );
PTR( Result );
PTR( Se );

class SceneMonitor : public Scene {
public:
	SceneMonitor( );
	virtual ~SceneMonitor( );
private:
	void initialize( ) override;
	void update( ) override;
private:
	void updatePhaseReady( );
private:
	StatusConstPtr _status;
	MapPtr _map;
	PlayerPtr _player;
	EnemyPtr _enemy;
	RoulettePtr _roulette;
	DioramaPtr _diorama;
	TitlePtr _title;
	TextPtr _text;
	ResultPtr _result;
	SePtr _se;
};

