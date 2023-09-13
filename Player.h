#pragma once
#include "smart_ptr.h"
#include "Image.h"

#include <map>
#include <array>

PTR( Status );

class Player {
public:
	Player( StatusConstPtr status );
	virtual ~Player( );
public:
	void draw( )const ;
	void hide( );
	void show( );
private:
	enum class RES {
		FRAME_R,
		FRAME_G,
		FRAME_B,
		FRAME_W,
		ICON_LIFE,
		ICON_LIFE_INVISIBLE,
		ICON_LIFE_GREEN,
		ICON_LIFE_WHITE,
		ICON_POWER_BLUE,
		ICON_POWER_RED,
		ICON_POWER,
		BUTTON_READY,
		BUTTON_OK,
		ACTION_ATTACK,
		ACTION_EXPLORE,
		ACTION_RELEASE,
		ACTION_REVIVAL,
		ICON_BOSS,
		ICON_MINOTAUR,
		ICON_ZOMBIE,
		ICON_GOBLIN
	};

	enum class TYPE_NUM {
		ICON_A,
		ICON_B,
		ICON_C,
		ICON_D,
		ICON_E,
		ICON_F,
		ICON_G,
		ICON_H,
		ICON_I,
		ICON_J,
		ICON_K,
		ICON_L,
		ICON_M,
		ICON_N,
		ICON_O,
		ICON_P,
		ICON_Q,
		ICON_R,
		ICON_S,
		ICON_T
	};
private:
	void draw( int player_idx ) const;
	void drawButton( int player_idx )const;
	void drawActionIcon( int player_idx ) const;
	void drawTargetEnemy( int player_idx ) const;
	void drawBuffIcon( int player_idx, int buff_idx )const;
private:
	StatusConstPtr _status;
	std::map< RES, ImagePtr > _image;
	std::array< ImagePtr, 4 > _image_icon_enemy;
	std::array< ImagePtr, 20 > _image_number_enemy;

	Vector2 _hide;
};

