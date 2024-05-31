#ifndef TEXT_3D_H_INCLUDED
#define TEXT_3D_H_INCLUDED

#include <string>

void t3dInit();

void t3dCleanup();

void t3dDraw2D(std::string str,
			   int hAlign, int vAlign,
			   float lineHeight = 1.5f);
void t3dDraw3D(std::string str,
			   int hAlign, int vAlign,
			   float depth,
			   float lineHeight = 1.5f);

float t3dDrawWidth(std::string str);

float t3dDrawHeight(std::string str, float lineHeight = 1.5f);

class T3DLoadException {
	private:
		std::string message0;
	public:
		T3DLoadException(std::string message1);
		std::string message() const;
};

#endif