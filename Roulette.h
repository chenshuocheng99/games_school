#pragma once
#include "Image.h"

PTR( Status );

class Roulette {
public:
	Roulette( StatusConstPtr status );
	virtual ~Roulette( );
public:
	void draw( );
	void hide( );
	void show( );
private:
	void orderRoulette( );
	void drawRoulette( );
private:
	ImagePtr _element_action;
	ImagePtr _explore_action;
	ImagePtr _atk_action;
	ImagePtr _heal_action;
	ImagePtr _action_part[ 6 ];
private:
	StatusConstPtr _status;
private:
	bool _is_roulette;
	bool _is_explore;
	bool _is_atk;
	bool _is_heal;
	bool _is_element;
	int _hide_y;
};