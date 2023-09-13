#include "UI.h"

//UI全体コントローラ
//フェーズ切り替えによるの演出コントローラ

const double SPEED = 0.03;

UI::UI( ) {
	_hide_ratio = 1.0;
}

UI::~UI( ) {

}

void UI::show( ) {
	_hide_ratio -= SPEED;
	if ( _hide_ratio < 0 ) {
		_hide_ratio = 0;
	}
}

void UI::hide( ) {
	_hide_ratio += SPEED;
	if ( _hide_ratio > 1.0 ) {
		_hide_ratio = 1.0;
	}
}

double UI::getHideRatio( ) const {
	return _hide_ratio * _hide_ratio;
}
