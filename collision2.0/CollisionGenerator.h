#ifndef COLLISIONGENERATOR_H
#define COLLISIONGENERATOR_H

#include "LodePNG.h"
#include <iostream>
#include <fstream>
#include <string>
#include <winsock.h>
#include <vector>
#include "Benchmark.h"

#pragma comment(lib, "Ws2_32.lib")

using namespace std;

//Pixel - Pixel object to convert to.
struct Pixel
{
	//Red value.
	UINT8 r;
	//Green value.
	UINT8 g;
	//Blue Value.
	UINT8 b;
	//Alpha value.
	UINT8 a;
	//Location of it on the X.
	int x;
	//Location of it on the Y.
	int y;
	//If the pixel has been culled out due to color algorithm.
	bool colorCull;
	//If the pixel has been culled out due to location algorithm.
	bool locationCull;
};

//Object that can be collided.
struct Collideable
{
	//X location of the collideable object.
	int x;
	//Y location of the collideable object.
	int y;
	//If the object has been culled.
	bool culled;
	//If the object has been culled due to the color algorithm.
	bool colorCulled;
};


class CollisionGenerator
{
public:
	//Default Constructor
	//@param string - name of the file to generate collision off of.
	CollisionGenerator(string filename);
	//Intialize - Initializes the class.
	void Initialize();
	//Shutdown - shutsdown the class.
	void Shutdown();
	//Default Destructor.
	~CollisionGenerator();
private:
	//Decode - Decodes the iamge.
	void Decode();
	//GenerateCollisionVector - Generates the vector of collideable objects.
	void GenerateCollisionVector();
	//Parse - Parses the objects to see whats collideable and converts them
	//to pixel struct format.
	void Parse();
	//ColorCulling - Culls based on the set color.
	void ColorCulling();
	//LocationCulling - Culls based on location.
	void LocationCulling();
	//WriteData - Writes the data out to a file.
	void WriteData();
	//Debug
	//InvertColors - Inverts the colors so they're visible upon viewing the result.
	void InvertColors();
	//Encode - Encodes the iamge back into png format.
	void Encode();
private:
	//Raw image in RGBA format
	vector<unsigned char> image;
	//Height of the image.
	unsigned int imgHeight;
	//Width of the image.
	unsigned int imgWidth;
	//ReserveBuffer - Used for memory's sake.
	unsigned int reserveBuffer;
	//Vector of the Pixel Structs in their conveted format.
	vector<Pixel*> pixelVector;
	//2d vector of collideable objects based on their x and y location.
	std::vector<std::vector<Collideable*>> collisionVector;
	//red value to filter out.
	UINT8 r;
	//green value to filter out.
	UINT8 g;
	//blue value to filter out.
	UINT8 b;
	//alpha value to filter out.
	UINT8 a;
	//Leeway for color filtering.
	UINT8 cushion;
	//Name of the file to open.
	string filename;
	//Name of the file that will be outputted.
	string outputFile;
	//Debug
	//Debug file that will be outputted.
	string debugFilename;
	//Raw image to be put out in debug format.
	vector<unsigned char> debugImage;
	//Collision debugging for verification.
	vector<Collideable*> debugCollision;
	//getCPUTime - used to benchmark functions.
	//@return double - milliseconds of time passed.
	double CollisionGenerator::getCPUTime();
};

#endif