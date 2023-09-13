#pragma once
#include "smart_ptr.h"
#include "Image.h"

#include <map>
#include <array>

PTR( Status );

class Result {
public:
	Result( StatusConstPtr status );
	virtual ~Result( );
public:
	void draw( ) const;
	void hide( );
	void show( );
private:
	enum class IMG {
		FADE,
		OVER,
		CLEAR
	};
private:
	std::map< IMG, ImagePtr > _image;

	StatusConstPtr _status;
};