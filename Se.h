#pragma once
#include "Sound.h"
#include "smart_ptr.h"
#include <string>

PTR( Status );

class Se {
public:
	Se( StatusConstPtr status );
	virtual ~Se( );
public:
	void draw( );
private:
	SoundPtr _bgm = Task::get< Sound >( );
	SoundPtr _se_enemy_action = Task::get<Sound>( );
	SoundPtr _se_enemy_appear = Task::get<Sound>( );
	SoundPtr _se_player_action = Task::get<Sound>( );
	SoundPtr _se_boss_appear = Task::get<Sound>( );
	SoundPtr _se_boss_action = Task::get<Sound>( );
	SoundPtr _se_item_get = Task::get<Sound>( );
	SoundPtr _se_release = Task::get<Sound>( );

	StatusConstPtr _status;
};

