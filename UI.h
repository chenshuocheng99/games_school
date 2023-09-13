#pragma once

class UI {
public:
	UI( );
	virtual ~UI( );
public:
	void show( );
	void hide( );
protected:
	double getHideRatio( ) const;
private:
	double _hide_ratio;
};

