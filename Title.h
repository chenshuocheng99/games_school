#pragma once
#include "UI.h"
#include "smart_ptr.h"

PTR( Image );

class Title : public UI {
public:
	Title( );
	virtual ~Title( );
public:
	void draw( ) const;
	void drawPrepare( ) const;
private:
	ImagePtr _image_title;
	ImagePtr _image_prepare;
};

