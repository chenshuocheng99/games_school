#pragma once
#include "define.h"

#include "Model.h"
#include "Primitive.h"
#include "Image.h"

#include <array>

PTR( Status );

class Diorama {
public:
	Diorama( StatusConstPtr status );
	virtual ~Diorama( );
public:
	void draw( );
	void focus( int player_idx );
	void focusEnemy( );
	void seeWhole( );
private:
	void drawCharacter( );
	void drawEnemy( );
	void drawElementalR( double x, double y );
	void drawElementalG( double x, double y );
	void drawElementalB( double x, double y );
	void drawElementalW( double x, double y );
	void drawBackground( );
	void drawItemStop( int x, int y );
	void drawElementalPos( );
	Matrix changeRotation( Vector2 pos_e, Vector2 pos_player );
private:
	std::array< ModelPtr, Define::MAX_PLAYER > _model_character;
	std::array< ModelPtr, 4 > _model_enemy;
	std::array< double, 4 > _transform_scaling_enemy;
	ModelPtr _model_elemental_air;
	ModelPtr _model_elemental_earth;
	ModelPtr _model_elemental_lava;
	ModelPtr _model_elemental_water;
	ModelPtr _model_background;
	ModelPtr _model_item_stop;
	PrimitivePtr _atk_circle;
	ImagePtr _image_red_circle;
	ImagePtr _image_blue_circle;
private:
	StatusConstPtr _status;
	Vector _camera_target_pos;
	Vector _camera_target_focus;
	Vector _camera_current_pos;
	Vector _camera_current_focus;
	double _rot;
};

