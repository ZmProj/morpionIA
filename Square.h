#pragma once
#include "GraphicPrimitives.h"
#include <ctime>
#include <cstdlib>
#include "utility.h"

class Square
{
private:
	float posX_, posY_, side_;
	float red_, green_, blue_;
	int nbClick_;
	bool occuped_;
	int clickedBy_;	// -1 : neutre		0 : J1		1 : J2

public:
	Square(float posX, float posY, float side, float red, float green, float blue) :
		posX_(posX),
		posY_(posY),
		side_(side),
		red_(red),
		green_(green),
		blue_(blue),
		nbClick_(0),
		occuped_(false),
		clickedBy_(-1)
	{}

	void draw();
	void tick();
	bool isClicked(float&, float&);
	void changeColorJ1();
	void changeColorJ2();
	void incrNbClick();
	float getSide() const;
	float getPosX() const;
	float getPosY() const;
	bool isOccuped() const;
	void setIsOccuped(bool);
	void setClickedBy(int);
	int getClickedBy() const;
};

