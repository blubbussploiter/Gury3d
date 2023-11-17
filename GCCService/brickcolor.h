#ifndef BRICKCOLOR_H
#define BRICKCOLOR_H
#include <G3DAll.h>

namespace RBX
{
	class BrickColor
	{
	private:
		int number;
		Color3* color;
	public:
		Color3* getColor();

		int getNumber() { return number; }
		static BrickColor* fromNumber(int number);

		BrickColor()
		{
			color = 0;
			number = 0;
		}
	};
}
#endif