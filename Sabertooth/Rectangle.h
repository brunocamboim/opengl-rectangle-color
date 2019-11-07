#ifndef RECTANGLE_H
#define RECTANGLE_H

class Rectangle
{
public:

	int R;
	int G;
	int B;
	float width_min;
	float width_max;
	float height_min;
	float height_max;
	bool visible;

	Rectangle(){}

	void preencher(int R, int G, int B)
	{
		this->R = R;
		this->G = G;
		this->B = B;
		this->visible = true;
		this->width_min = 0;
		this->width_max = 0;
		this->height_min = 0;
		this->height_max = 0;
	}

};

#endif