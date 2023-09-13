#pragma once
#include <map>
#include "smart_ptr.h"
#include "UI.h"

PTR( Image );
PTR( Status );

class Enemy : public UI {
public:
	Enemy( StatusConstPtr status );
	virtual ~Enemy( );
public:
	void draw( );
private:
	void drawEnemyFrame( int num )const;
	void drawEnemyNumber( int number, int type, int fream )const;
	void drawEnemyPower( int power, int pos_y, int type, int fream )const;
	void drawEnemyLife( int life, int type, int fream )const;
	void drawBossGauge( ) const;
	void drawBuff( int x, int y )const;
private:
	enum class TYPE {
		FRAME_GOBLIN,
		FRAME_ZOMBIE,
		FRAME_MINOTAUR,
		FRAME_BOSS,
		ICON_GOBLIN,
		ICON_ZOMBIE,
		ICON_MINOTAUR,
		ICON_BOSS,
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
		ICON_T,
		ICON_LIFE,
		ICON_POWER,
		ICON_BUFF,
		GAUGE_BOSS_0,
		GAUGE_BOSS_1,
		GAUGE_BOSS_2,
		GAUGE_BOSS_3,
		GAUGE_BOSS_4,
		GAUGE_BOSS_5,
		GAUGE_BOSS_6,
		GAUGE_BOSS_7,
		GAUGE_BOSS_8,
	};
private:
	std::map< TYPE, ImagePtr > _image;
private:
	int  _enemy_frame_x[ 20 ] = { 0,235,470,705,940,1175,1410,1645,1880,2115,2350,2585,2820,3055,3290,3525,3760,3995,4230,4465 };
	int _enemy_frame_y[ 4 ] = { 800,780,740,680 };
private:
	StatusConstPtr _status;
	bool _is_display_boss_gauge;
};

