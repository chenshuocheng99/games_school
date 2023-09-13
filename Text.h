#pragma once
#include "UI.h"
#include "smart_ptr.h"
#include <string>

PTR( Image );
PTR( Status );

class Text : public UI {
public:
	Text( StatusConstPtr status );
	virtual ~Text( );
public:
	void draw( );
private:
	void drawText( int sx, int sy, std::string text ) const;
private:
	ImagePtr _frame_text;
	StatusConstPtr _status;
};