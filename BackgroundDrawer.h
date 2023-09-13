#pragma once
#include "smart_ptr.h"
#include "Model.h"

PTR( Image );
PTR( Primitive );

class BackgroundDrawer {
public:
	BackgroundDrawer( );
	virtual ~BackgroundDrawer( );
public:
	void draw( );
private:
	void makeFloorStage01( );
	void makeCeilingStage01( );
	void makeWallStage01( );
	void makeBoxStage02( );
	void makeFloorStage03( );

private:
	PrimitivePtr _model;
	double _width;
	double _depth;
	double _height;

	ModelPtr _model_floor_stage01;
	ModelPtr _model_ceiling_stage01;
	ModelPtr _model_wall_stage01;
	ModelPtr _model_box_stage02;
	ModelPtr _model_floor_stage03;
	ModelPtr _model_stairs_stage03;
	ModelPtr _model_pillar_stage03;
};

