#include <fstream>
#include <math.h>

#ifdef __APPLE__
#include <OpenGL/OpenGL.h>
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include "text3d.h"

using namespace std;

T3DLoadException::T3DLoadException(string message1) : message0(message1) {

}

string T3DLoadException::message() const {
	return message0;
}

namespace {
	int toInt(const char* bytes) {
		return int(((unsigned char)bytes[3] << 24) |
				   ((unsigned char)bytes[2] << 16) |
				   ((unsigned char)bytes[1] << 8) |
				   (unsigned char)bytes[0]);
	}


	float toFloat(const char* buffer) {
		char exp = buffer[0];
		int mant = toInt(buffer + 1);
		bool isNegative;
		if (mant < 0) {
			isNegative = true;
			mant = ~mant;
		}
		else
			isNegative = false;
		float a = (2147483648u + (unsigned int)mant) *
					pow(2.0f, exp) / 2147483648.0;
		return isNegative ? -a : a;
	}


	unsigned short toUShort(const char* buffer) {
		return (((unsigned short)((unsigned char)buffer[1])) << 8) +
				(unsigned short)((unsigned char)buffer[0]);
	}

	template<class T>
	class auto_array {
		private:
			T* array;
			mutable bool isReleased;
		public:
			explicit auto_array(T* array_ = NULL) :
				array(array_), isReleased(false) {
			}

			auto_array(const auto_array<T> &aarray) {
				array = aarray.array;
				isReleased = aarray.isReleased;
				aarray.isReleased = true;
			}

			~auto_array() {
				if (!isReleased && array != NULL) {
					delete[] array;
				}
			}

			T* get() const {
				return array;
			}

			T &operator*() const {
				return *array;
			}

			void operator=(const auto_array<T> &aarray) {
				if (!isReleased && array != NULL) {
					delete[] array;
				}
				array = aarray.array;
				isReleased = aarray.isReleased;
				aarray.isReleased = true;
			}

			T* operator->() const {
				return array;
			}

			T* release() {
				isReleased = true;
				return array;
			}

			void reset(T* array_ = NULL) {
				if (!isReleased && array != NULL) {
					delete[] array;
				}
				array = array_;
			}

			T* operator+(int i) {
				return array + i;
			}

			T &operator[](int i) {
				return array[i];
			}
	};

	enum Opcodes {OP_NORMAL = 65532,
				  OP_TRIANGLE_STRIP,
				  OP_TRIANGLES,
				  OP_END_PART};

	const float PI_TIMES_2_OVER_65536 = 2 * 3.1415926535f / 65536.0f;

	class T3DFont {
		private:
			float spaceWidth;
			float widths[94];
			GLuint displayListId2D;
			GLuint displayListId3D;
		public:
			T3DFont(ifstream &input) {
				char buffer[8];
				input.read(buffer, 8);
				if (input.fail()) {
					throw T3DLoadException("Invalid font file");
				}

				const char header[9] = "VTR\0FNT\0";
				for(int i = 0; i < 8; i++) {
					if (buffer[i] != header[i]) {
						throw T3DLoadException("Invalid font file");
					}
				}

				input.read(buffer, 5);
				spaceWidth = toFloat(buffer);

				displayListId2D = glGenLists(94);
				displayListId3D = glGenLists(94);
				for(int i = 0; i < 94; i++) {
					input.read(buffer, 5);
					float scale = toFloat(buffer) / 65536;
					input.read(buffer, 2);
					float width = scale * toUShort(buffer);
					input.read(buffer, 2);
					float height = scale * toUShort(buffer);
					scale /= height;
					widths[i] = width / height;
					input.read(buffer, 2);
					unsigned short numVerts = toUShort(buffer);
					auto_array<float> verts(new float[2 * numVerts]);
					float* verts2 = verts.get();
					for(int j = 0; j < numVerts; j++) {
						input.read(buffer, 2);
						verts2[2 * j] = scale * ((int)toUShort(buffer) - 32768);
						input.read(buffer, 2);
						verts2[2 * j + 1] =
							scale * ((int)toUShort(buffer) - 32768);
					}

					glNewList(displayListId2D + i, GL_COMPILE);

					glNormal3f(0, 0, 1);

					input.read(buffer, 2);
					unsigned short opcode = toUShort(buffer);
					switch(opcode) {
						case OP_TRIANGLES:
							glBegin(GL_TRIANGLES);
							break;
						case OP_TRIANGLE_STRIP:
							glBegin(GL_TRIANGLE_STRIP);
							break;
						default:
							throw T3DLoadException("Invalid font file");
					}

					int limit = 10000;

					while(true) {
						input.read(buffer, 2);
						opcode = toUShort(buffer);
						switch(opcode) {
							case OP_TRIANGLES:
								glEnd();
								glBegin(GL_TRIANGLES);
								break;
							case OP_TRIANGLE_STRIP:
								glEnd();
								glBegin(GL_TRIANGLE_STRIP);
								break;
							case OP_END_PART:
								goto BreakOuter;
							default:
								glVertex3f(verts2[2 * opcode],
										   verts2[2 * opcode + 1],
										   0);
								break;
						}

						if (--limit == 0) {
							glEndList();
							throw T3DLoadException("Invalid font file");
						}
					}
BreakOuter:
					glEnd();
					glEndList();

					glNewList(displayListId3D + i, GL_COMPILE);
					glPushMatrix();
					glTranslatef(0, 0, 0.5f);
					glFrontFace(GL_CW);
					glCallList(displayListId2D + i);
					glTranslatef(0, 0, -1);
					glScalef(1, 1, -1);
					glFrontFace(GL_CCW);
					glCallList(displayListId2D + i);
					glFrontFace(GL_CW);

					input.read(buffer, 2);
					opcode = toUShort(buffer);
					switch(opcode) {
						case OP_TRIANGLES:
							glBegin(GL_TRIANGLES);
							break;
						case OP_TRIANGLE_STRIP:
							glBegin(GL_TRIANGLE_STRIP);
							break;
						default:
							throw T3DLoadException("Invalid font file");
					}

					limit = 10000;
					while(true) {
						input.read(buffer, 2);
						opcode = toUShort(buffer);
						switch(opcode) {
							case OP_TRIANGLES:
								glEnd();
								glBegin(GL_TRIANGLES);
								break;
							case OP_TRIANGLE_STRIP:
								glEnd();
								glBegin(GL_TRIANGLE_STRIP);
								break;
							case OP_NORMAL:
								input.read(buffer, 2);
								float angle;
								angle = toUShort(buffer) *
									PI_TIMES_2_OVER_65536;
								float x, y;
								x = cos(angle);
								y = sin(angle);
								glNormal3f(x, y, 0);
								break;
							case OP_END_PART:
								goto BreakOuter2;
							default:
								if (opcode < numVerts) {
									glVertex3f(verts2[2 * opcode],
											   verts2[2 * opcode + 1],
											   0);
								}
								else {
									glVertex3f(verts2[2 * (opcode - numVerts)],
											   verts2[2 * (opcode - numVerts) +
													  1],
											   -1);
								}
								break;
						}

						if (--limit == 0) {
							glEndList();
							throw T3DLoadException("Invalid font file");
						}
					}
BreakOuter2:
					glEnd();
					glPopMatrix();
					glEndList();
				}

				if (input.fail()) {
					throw T3DLoadException("Invalid font file");
				}
				input.read(buffer, 1);
				if (!input.eof()) {
					throw T3DLoadException("Invalid font file");
				}
			}

			void draw2D(char c) {
				if (c >= 33 && c <= 126) {
					glCallList(displayListId2D + c - '!');
				}
			}

			void draw3D(char c) {
				if (c >= 33 && c <= 126) {
					glCallList(displayListId3D + c - '!');
				}
			}

			float width(char c) {
				if (c >= 33 && c <= 126) {
					return widths[c - 33];
				}
				else {
					return spaceWidth;
				}
			}
	};

	T3DFont* font = NULL;

	void draw2D(char c) {
		font->draw2D(c);
	}

	void draw3D(char c) {
		font->draw3D(c);
	}

	void drawLine(const char* str, int hAlign, void (*drawFunc)(char)) {
		glPushMatrix();
		if (hAlign >= 0) {
			float width = 0;
			for(int i = 0; str[i] != '\n' && str[i] != '\0'; i++) {
				width += font->width(str[i]);
			}
			glTranslatef(hAlign > 0 ? -width : -width / 2, 0, 0);
		}

		for(int i = 0; str[i] != '\n' && str[i] != '\0'; i++) {
			float width = font->width(str[i]);
			glTranslatef(width / 2, 0, 0);
			drawFunc(str[i]);
			glTranslatef(width / 2, 0, 0);
		}

		glPopMatrix();
	}

	void draw(const char* str,
			  int hAlign, int vAlign,
			  float lineHeight,
			  void (*drawFunc)(char)) {
		GLint shadeModel;
		glGetIntegerv(GL_SHADE_MODEL, &shadeModel);
		glShadeModel(GL_SMOOTH);
		GLboolean lightsEnabled;
		glGetBooleanv(GL_LIGHTING, &lightsEnabled);
		GLboolean normalsWereNormalized;
		glGetBooleanv(GL_NORMALIZE, &normalsWereNormalized);
		if (lightsEnabled) {
			glEnable(GL_NORMALIZE);
		}
		else {
			glDisable(GL_NORMALIZE);
		}

		glPushMatrix();
		if (vAlign >= 0) {
			int numLines = 1;
			for(int i = 0; str[i] != '\0'; i++) {
				if (str[i] == '\n') {
					numLines++;
				}
			}

			float height = lineHeight * (numLines - 1) + 1;
			glTranslatef(0, vAlign > 0 ? height : height / 2, 0);
		}

		glTranslatef(0, -0.5f, 0);
		drawLine(str, hAlign, drawFunc);
		for(int i = 0; str[i] != '\0'; i++) {
			if (str[i] == '\n') {
				glTranslatef(0, -lineHeight, 0);
				drawLine(str + i + 1, hAlign, drawFunc);
			}
		}

		glPopMatrix();

		glShadeModel(shadeModel);
		if (normalsWereNormalized) {
			glEnable(GL_NORMALIZE);
		}
		else {
			glDisable(GL_NORMALIZE);
		}
	}
}

void t3dInit() {
	if (font == NULL) {
		ifstream input;
		input.open("charset", istream::binary);
		font = new T3DFont(input);
		input.close();
	}
}

void t3dCleanup() {
	delete font;
}

void t3dDraw2D(string str, int hAlign, int vAlign, float lineHeight) {
	GLboolean wasCulling;
	glGetBooleanv(GL_CULL_FACE, &wasCulling);
	glDisable(GL_CULL_FACE);

	draw(str.c_str(), hAlign, vAlign, lineHeight, draw2D);

	if (wasCulling) {
		glEnable(GL_CULL_FACE);
	}
}

void t3dDraw3D(string str,
			   int hAlign, int vAlign,
			   float depth,
			   float lineHeight) {
	GLboolean wasCulling;
	glGetBooleanv(GL_CULL_FACE, &wasCulling);
	glEnable(GL_CULL_FACE);
	GLint frontFace;
	glGetIntegerv(GL_FRONT_FACE, &frontFace);

	glPushMatrix();
	glScalef(1, 1, depth);
	draw(str.c_str(), hAlign, vAlign, lineHeight, draw3D);
	glPopMatrix();

	if (!wasCulling) {
		glDisable(GL_CULL_FACE);
	}
	glFrontFace(frontFace);
}

float t3dDrawWidth(string str) {
	float bestWidth = 0;
	int i = 0;
	while (str[i] != '\0') {
		float width = 0;
		while (str[i] != '\n' && str[i] != '\0') {
			width += font->width(str[i]);
			i++;
		}
		if (width > bestWidth) {
			bestWidth = width;
		}
		if (str[i] != '\0') {
			i++;
		}
	}
	return bestWidth;
}

float t3dDrawHeight(string str, float lineHeight) {
	int numLines = 1;
	for(int i = 0; str[i] != '\0'; i++) {
		if (str[i] == '\n') {
			numLines++;
		}
	}

	return (numLines - 1) * lineHeight + 1;
}
