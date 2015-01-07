#include "Square.h"
#include <cstdlib>
#include "MyGraphicEngine.h"

void Square::draw(){
	
	GraphicPrimitives::drawFillRect2D(
		posX_,
		posY_,
		side_,
		side_,
		0.5f,
		0.5f,
		0.5f);

	GraphicPrimitives::drawFillRect2D(
		posX_ + 0.01f,
		posY_ + 0.01f,
		side_ - 0.01f,
		side_ - 0.01f,
		red_,
		green_,
		blue_);
}

void Square::tick(){}

bool Square::isClicked(float& x, float& y){
	if (x < posX_) return false;
	if (y < posY_) return false;
	if (x > posX_ + side_) return false;
	if (y > posY_ + side_) return false;
	return true;
}

void Square::changeColorJ1(){
	red_ = 0.0f;
	green_ = 1.0f;
	blue_ = 0.0f;
}

void Square::changeColorJ2(){
	red_ = 1.0f;
	green_ = 0.0f;
	blue_ = 0.0f;
}

void Square::incrNbClick(){
	nbClick_++;
}

float Square::getSide() const{
	return side_;
}

float Square::getPosX() const{
	return posX_;
}

float Square::getPosY() const{
	return posY_;
}

bool Square::isOccuped() const{
	return occuped_;
}

void Square::setIsOccuped(bool b){
	occuped_ = b;
}

void Square::setClickedBy(int x){
	clickedBy_ = x;
}

int Square::getClickedBy() const{
	return clickedBy_;
}