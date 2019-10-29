#ifndef RECTANGLE_H
#define RECTANGLE_H

class Rectangle
{
public:

	int R;
	int G;
	int B;
	bool visible;

	Rectangle(){}

	Rectangle(int R, int G, int B)
	{
		this->R = R;
		this->G = G;
		this->B = B;
		this->visible = true;
	}

};

#endif