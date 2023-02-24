#pragma once

/* RGBA Colours */
#define RGBA_BLACK 0.0f, 0.0f, 0.0f, 1.0f
#define RGBA_WHITE 1.0f, 1.0f, 1.0f, 1.0f

#define RGBA_RED 1.0f, 0.0f, 0.0f, 1.0f
#define RGBA_GREEN 0.0f, 1.0f, 0.0f, 1.0f
#define RGBA_BLUE 0.0f, 0.0f, 1.0f, 1.0f

#define RGBA_CYAN 0.0f, 1.0f, 1.0f, 1.0f
#define RGBA_YELLOW 1.0f, 1.0f, 0.0f, 1.0f
#define RGBA_MAGENTA 1.0f, 0.0f, 0.0f, 1.0f

#define RGBA_TRANSPARENT 1.0f, 1.0f, 1.0f, 0.0f
#define RGBA_TRANSLUCENT 1.0f, 1.0f, 1.0f, 0.5f

/* ARGB Colours */
#define ARGB_BLACK 1.0f, 0.0f, 0.0f, 0.0f
#define ARGB_WHITE 1.0f, 1.0f, 1.0f, 1.0f

#define ARGB_RED 1.0f, 1.0f, 0.0f, 0.0f
#define ARGB_GREEN 1.0f, 0.0f, 1.0f, 0.0f
#define ARGB_BLUE 1.0f, 0.0f, 0.0f, 1.0f

#define ARGB_CYAN 1.0f, 0.0f, 1.0f, 1.0f
#define ARGB_YELLOW 1.0f, 1.0f, 1.0f, 0.0f
#define ARGB_MAGENTA 1.0f, 1.0f, 0.0f, 0.0f

#define ARGB_TRANSPARENT 0.0f, 1.0f, 1.0f, 1.0f
#define ARGB_TRANSLUCENT 0.5f, 1.0f, 1.0f, 1.0f

/* RGB Colours*/
#define RGB_BLACK 0.0f, 0.0f, 0.0f
#define RGB_WHITE 1.0f, 1.0f, 1.0f

#define RGB_RED 1.0f, 0.0f, 0.0f
#define RGB_GREEN 0.0f, 1.0f, 0.0f
#define RGB_BLUE 0.0f, 0.0f, 1.0f

#define RGB_CYAN 0.0f, 1.0f, 1.0f
#define RGB_YELLOW 1.0f, 1.0f, 0.0f
#define RGB_MAGENTA 1.0f, 0.0f, 0.0f

/* Vertex Data */
class SimpleMesh {
	public:
	static constexpr const float verticiesRectangle[24] = {
		//	Position (x, y, z)		Colour (R, G, B)	Name
			-0.5f,	0.5f, 0.0f,		1.0f, 0.0f, 0.0f,	// Top-Left
			0.5f, 0.5f, 0.0f,		0.0f, 1.0f, 0.0f,	// Top-Right
			0.5f, -0.5f, 0.0f,		0.0f, 0.0f, 1.0f,	// Bottom-Right
			-0.5f, -0.5f, 0.0f,		1.0f, 1.0f, 1.0f	// Bottom-Left
	};

	static constexpr const float verticiesCube[48] = {
		//	Position (x, y, z)		Colour (R, G, B)	//	Colour
			-0.5f, -0.5f, -0.5f,	1.0f, 1.0f, 1.0f,	//	White		1
			-0.5f, -0.5f, 0.5f,		0.0f, 0.0f, 1.0f,	//	Blue		2
			0.5f, -0.5f, 0.5f,		0.0f, 1.0f, 0.0f,	//	Green		3
			0.5f, -0.5f, -0.5f,		0.0f, 1.0f, 1.0f,	//	Cyan		4
			-0.5f, 0.5f, -0.5f,		1.0f, 0.0f, 1.0f,	//	Magenta		5
			-0.5f, 0.5f, 0.5f,		1.0f, 1.0f, 0.0f,	//	Yellow		6
			0.5f, 0.5f, 0.5f,		0.5f, 0.5f, 0.5f,	//  Grey		7
			0.5f, 0.5f, -0.5f,		1.0f, 0.0f, 0.0f,	//	Red			8
	};

	static constexpr const float verticiesCubeSolidTriangles[324] = {
		//	Position (x, y, z)		Colour (R, G, B)	Tex Coord (x, y)	//	Colour	Face	Vertex
			-0.5f, -0.5f, -0.5f,	1.0f, 0.0f, 1.0f,	0.0f, 1.0f,			//	Magenta	1		1
			-0.5f, 0.5f, -0.5f,		1.0f, 0.0f, 1.0f,	1.0f, 1.0f,			//					2
			0.5f, -0.5f, -0.5f,		1.0f, 0.0f, 1.0f,	1.0f, 0.0f,			//					3

			0.5f, -0.5f, -0.5f,		1.0f, 0.0f, 1.0f,	1.0f, 0.0f,			//					3
			-0.5f, 0.5f, -0.5f,		1.0f, 0.0f, 1.0f,	1.0f, 1.0f,			//					2
			0.5f, 0.5f, -0.5f,		1.0f, 0.0f, 1.0f,	0.0f, 0.0f,			//					4		



			-0.5f, -0.5f, -0.5f,	1.0f, 1.0f, 1.0f,	0.0f, 1.0f,			//	White	2		1		
			0.5f, -0.5f, -0.5f,		1.0f, 1.0f, 1.0f,	1.0f, 1.0f,			//					2		
			-0.5f, -0.5f, 0.5f,		1.0f, 1.0f, 1.0f,	1.0f, 0.0f,			//					3

			-0.5f, -0.5f, 0.5f,		1.0f, 1.0f, 1.0f,	1.0f, 0.0f,			//					3
			0.5f, -0.5f, -0.5f,		1.0f, 1.0f, 1.0f,	1.0f, 1.0f,			//					2
			0.5f, -0.5f, 0.5f,		1.0f, 1.0f, 1.0f,	0.0f, 0.0f,			//					4



			-0.5f, -0.5f, -0.5f,	0.0f, 1.0f, 1.0f,	0.0f, 1.0f,			//	Cyan	3		1
			-0.5f, -0.5f, 0.5f,		0.0f, 1.0f, 1.0f,	1.0f, 1.0f,			//					2
			-0.5f, 0.5f, 0.5f,		0.0f, 1.0f, 1.0f,	1.0f, 0.0f,			//					3

			-0.5f, -0.5f, -0.5f,	0.0f, 1.0f, 1.0f,	0.0f, 1.0f,			//					1
			-0.5f, 0.5f, 0.5f,		0.0f, 1.0f, 1.0f,	1.0f, 0.0f,			//					3
			-0.5f, 0.5f, -0.5f,		0.0f, 1.0f, 1.0f,	0.0f, 0.0f,			//					4


			
			-0.5f, -0.5f, 0.5f,		0.0f, 1.0f, 0.0f,	0.0f, 1.0f,			//	Green	4		1
			0.5f, -0.5f, 0.5f,		0.0f, 1.0f, 0.0f,	1.0f, 1.0f,			//					2
			0.5f, 0.5f, 0.5f,		0.0f, 1.0f, 0.0f,	1.0f, 0.0f,			//					3

			-0.5f, -0.5f, 0.5f,		0.0f, 1.0f, 0.0f,	0.0f, 1.0f,			//					1
			0.5f, 0.5f, 0.5f,		0.0f, 1.0f, 0.0f,	1.0f, 0.0f,			//					3
			-0.5f, 0.5f, 0.5f,		0.0f, 1.0f, 0.0f,	0.0f, 0.0f,			//					4
			


			-0.5f, 0.5f, -0.5f,		0.5f, 0.5f, 0.5f,	0.0f, 1.0f,			//	Grey	5		1
			-0.5f, 0.5f, 0.5f,		0.5f, 0.5f, 0.5f,	1.0f, 1.0f,			//					2
			0.5f, 0.5f, 0.5f,		0.5f, 0.5f, 0.5f,	1.0f, 0.0f,			//					3

			-0.5f, 0.5f, -0.5f,		0.5f, 0.5f, 0.5f,	0.0f, 1.0f,			//					1
			0.5f, 0.5f, 0.5f,		0.5f, 0.5f, 0.5f,	1.0f, 0.0f,			//					3
			0.5f, 0.5f, -0.5f,		0.5f, 0.5f, 0.5f,	0.0f, 0.0f,			//					4



			0.5f, -0.5f, 0.5f,		0.0f, 0.0f, 1.0f,	0.0f, 1.0f,			//	Blue	6		1
			0.5f, 0.5f, -0.5f,		0.0f, 0.0f, 1.0f,	1.0f, 1.0f,			//					2
			0.5f, 0.5f, 0.5f,		0.0f, 0.0f, 1.0f,	1.0f, 0.0f,			//					3
			
			0.5f, -0.5f, 0.5f,		0.0f, 0.0f, 1.0f,	0.0f, 1.0f,			//					1
			0.5f, -0.5f, -0.5f,		0.0f, 0.0f, 1.0f,	0.0f, 0.0f,			//					4
			0.5f, 0.5f, -0.5f,		0.0f, 0.0f, 1.0f,	1.0f, 1.0f			//					2
	};

	static constexpr const float verticiesCubeWireFrame[144] = {
		//	Position (x, y, z)		Colour (R, G, B)	//	Colour	
			-0.5f, -0.5f, -0.5f,	1.0f, 1.0f, 1.0f,	//	White		1
			-0.5f, -0.5f, 0.5f,		0.0f, 0.0f, 1.0f,	//	Blue		2

			-0.5f, -0.5f, 0.5f,		0.0f, 0.0f, 1.0f,	//	Blue		2
			0.5f, -0.5f, 0.5f,		0.0f, 1.0f, 0.0f,	//	Green		3

			0.5f, -0.5f, 0.5f,		0.0f, 1.0f, 0.0f,	//	Green		3
			0.5f, -0.5f, -0.5f,		0.0f, 1.0f, 1.0f,	//	Cyan		4

			0.5f, -0.5f, -0.5f,		0.0f, 1.0f, 1.0f,	//	Cyan		4
			-0.5f, -0.5f, -0.5f,	1.0f, 1.0f, 1.0f,	//	White		1



			-0.5f, 0.5f, -0.5f,		1.0f, 0.0f, 1.0f,	//	Magenta		5
			-0.5f, 0.5f, 0.5f,		1.0f, 1.0f, 0.0f,	//	Yellow		6

			-0.5f, 0.5f, 0.5f,		1.0f, 1.0f, 0.0f,	//  Yellow		6
			0.5f, 0.5f, 0.5f,		0.5f, 0.5f, 0.5f,	//  Grey		7

			0.5f, 0.5f, 0.5f,		0.5f, 0.5f, 0.5f,	//  Grey		7
			0.5f, 0.5f, -0.5f,		1.0f, 0.0f, 0.0f,	//	Red			8

			0.5f, 0.5f, -0.5f,		1.0f, 0.0f, 0.0f,	//	Red			8
			-0.5f, 0.5f, -0.5f,		1.0f, 0.0f, 1.0f,	//	Magenta		5



			-0.5f, -0.5f, -0.5f,	1.0f, 1.0f, 1.0f,	//	White		1
			-0.5f, 0.5f, -0.5f,		1.0f, 0.0f, 1.0f,	//	Magenta		5

			-0.5f, -0.5f, 0.5f,		0.0f, 0.0f, 1.0f,	//	Blue		2
			-0.5f, 0.5f, 0.5f,		1.0f, 1.0f, 0.0f,	//	Yellow		6

			0.5f, -0.5f, 0.5f,		0.0f, 1.0f, 0.0f,	//	Green		3
			0.5f, 0.5f, 0.5f,		0.5f, 0.5f, 0.5f,	//  Grey		7

			0.5f, -0.5f, -0.5f,		0.0f, 1.0f, 1.0f,	//	Cyan		4
			0.5f, 0.5f, -0.5f,		1.0f, 0.0f, 0.0f,	//	Red			8
	};

}






