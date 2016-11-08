#include "CollisionGenerator.h"


/*
 * Author:  David Robert Nadeau
 * Site:    http://NadeauSoftware.com/
 * License: Creative Commons Attribution 3.0 Unported License
 *          http://creativecommons.org/licenses/by/3.0/deed.en_US
 */
#if defined(_WIN32)
#include <Windows.h>

#elif defined(__unix__) || defined(__unix) || defined(unix) || (defined(__APPLE__) && defined(__MACH__))
#include <unistd.h>
#include <sys/resource.h>
#include <sys/times.h>
#include <time.h>

#else
#error "Unable to define getCPUTime( ) for an unknown OS."
#endif





/**
 * Returns the amount of CPU time used by the current process,
 * in seconds, or -1.0 if an error occurred.
 */
double CollisionGenerator::getCPUTime( )
{
#if defined(_WIN32)
	/* Windows -------------------------------------------------- */
	FILETIME createTime;
	FILETIME exitTime;
	FILETIME kernelTime;
	FILETIME userTime;
	if ( GetProcessTimes( GetCurrentProcess( ),
		&createTime, &exitTime, &kernelTime, &userTime ) != -1 )
	{
		SYSTEMTIME userSystemTime;
		if ( FileTimeToSystemTime( &userTime, &userSystemTime ) != -1 )
			return (double)userSystemTime.wHour * 3600.0 +
				(double)userSystemTime.wMinute * 60.0 +
				(double)userSystemTime.wSecond +
				(double)userSystemTime.wMilliseconds / 1000.0;
	}

#elif defined(__unix__) || defined(__unix) || defined(unix) || (defined(__APPLE__) && defined(__MACH__))
	/* AIX, BSD, Cygwin, HP-UX, Linux, OSX, and Solaris --------- */

#if defined(_POSIX_TIMERS) && (_POSIX_TIMERS > 0)
	/* Prefer high-res POSIX timers, when available. */
	{
		clockid_t id;
		struct timespec ts;
#if _POSIX_CPUTIME > 0
		/* Clock ids vary by OS.  Query the id, if possible. */
		if ( clock_getcpuclockid( 0, &id ) == -1 )
#endif
#if defined(CLOCK_PROCESS_CPUTIME_ID)
			/* Use known clock id for AIX, Linux, or Solaris. */
			id = CLOCK_PROCESS_CPUTIME_ID;
#elif defined(CLOCK_VIRTUAL)
			/* Use known clock id for BSD or HP-UX. */
			id = CLOCK_VIRTUAL;
#else
			id = (clockid_t)-1;
#endif
		if ( id != (clockid_t)-1 && clock_gettime( id, &ts ) != -1 )
			return (double)ts.tv_sec +
				(double)ts.tv_nsec / 1000000000.0;
	}
#endif

#if defined(RUSAGE_SELF)
	{
		struct rusage rusage;
		if ( getrusage( RUSAGE_SELF, &rusage ) != -1 )
			return (double)rusage.ru_utime.tv_sec +
				(double)rusage.ru_utime.tv_usec / 1000000.0;
	}
#endif

#if defined(_SC_CLK_TCK)
	{
		const double ticks = (double)sysconf( _SC_CLK_TCK );
		struct tms tms;
		if ( times( &tms ) != (clock_t)-1 )
			return (double)tms.tms_utime / ticks;
	}
#endif

#if defined(CLOCKS_PER_SEC)
	{
		clock_t cl = clock( );
		if ( cl != (clock_t)-1 )
			return (double)cl / (double)CLOCKS_PER_SEC;
	}
#endif

#endif

	return -1;		/* Failed. */
}






CollisionGenerator::CollisionGenerator(string filename)
{
	this->filename = filename;
	debugFilename = (filename.substr(0, filename.length() - 4) + "_ecd-visual.png");
	outputFile = (filename.substr(0, filename.length() - 3) + "ecd");
	r = 255;
	g = 255;
	b = 255;
	a = 255;
	cushion = 15;
	imgHeight = 0;
	imgWidth = 0;
	reserveBuffer = 10;
}

void CollisionGenerator::Initialize()
{
	double startTime, endTime;
	startTime = getCPUTime();
	Decode();
	endTime = getCPUTime();
	fprintf(stderr, "Decode CPU time used = %lf\n", (endTime - startTime));
	startTime = getCPUTime();
	GenerateCollisionVector();
	endTime = getCPUTime();
	fprintf(stderr, "GenerateVector CPU time used = %lf\n", (endTime - startTime));
	startTime = getCPUTime();
	Parse();
	endTime = getCPUTime();
	fprintf(stderr, "Parse CPU time used = %lf\n", (endTime - startTime));
	startTime = getCPUTime();
	ColorCulling();
	endTime = getCPUTime();
	fprintf(stderr, "ColorCulling CPU time used = %lf\n", (endTime - startTime));
	startTime = getCPUTime();
	LocationCulling();
	endTime = getCPUTime();
	fprintf(stderr, "LocationCulling CPU time used = %lf\n", (endTime - startTime));
	startTime = getCPUTime();
	//Debugging.
	InvertColors();
	endTime = getCPUTime();
	fprintf(stderr, "InvertColors CPU time used = %lf\n", (endTime - startTime));
	startTime = getCPUTime();
	Encode();
	endTime = getCPUTime();
	fprintf(stderr, "Encode CPU time used = %lf\n", (endTime - startTime));
	startTime = getCPUTime();
	//Final.
	WriteData();
}

void CollisionGenerator::WriteData()
{
	ofstream fout;
	fout.open(outputFile);
	fout << imgWidth << " " << imgHeight << endl;
	for(unsigned int i = 0; i < imgWidth; i++)
	{
		for(unsigned int j = 0; j < imgHeight; j++)
		{
			if(!collisionVector[i][j]->colorCulled && !collisionVector[i][j]->culled)
			{
				fout << collisionVector[i][j]->x << " " << collisionVector[i][j]->y << endl;
			}
		}
	}
}

void CollisionGenerator::InvertColors()
{
	//depricated.
	return;
	int pixelSize = pixelVector.size();
	for(int i = 0; i < pixelSize; i++)
	{
		if(pixelVector[i]->colorCull)
		{
			//Invert to bright red.
			pixelVector[i]->r = 255;
			pixelVector[i]->g = 0;
			pixelVector[i]->b = 0;
			pixelVector[i]->a = 255;
		}
	}
}

void CollisionGenerator::Decode()
{
	std::ifstream in(filename);
	//Skip file header.
    in.seekg(16);
    in.read((char *)&imgWidth, 4);
    in.read((char *)&imgHeight, 4);

    imgWidth = ntohl(imgWidth);
    imgHeight = ntohl(imgHeight);

    std::cout << filename << " is " << imgWidth << " pixels wide and " << imgHeight << " pixels high.\n";
	in.close();

	unsigned error = lodepng::decode(image, imgWidth, imgHeight, filename);
    //if there's an error, display it
    if(error) std::cout << "decoder error " << error << ": " << lodepng_error_text(error) << std::endl;
}

void CollisionGenerator::Parse()
{
	int imgSize = image.size();
	for(int i = 0; i < imgSize; i += 4)
	{
		Pixel* p = new Pixel();
		p->r = static_cast<UINT8>(image[i]);
		p->g = static_cast<UINT8>(image[i + 1]);
		p->b = static_cast<UINT8>(image[i + 2]);
		p->a = static_cast<UINT8>(image[i + 3]);
		p->x = ((i / 4) % imgWidth);
		p->y = ((i / 4) / imgWidth);
		pixelVector.push_back(p);
		collisionVector[p->x][p->y]->x = p->x;
		collisionVector[p->x][p->y]->y = p->y;
	}
}

void CollisionGenerator::Encode()
{
	/*
	Print sizes of vectors.
	*/
	cout << "Image Vect: " << sizeof(image) << endl
		<< "Pixel Vect: " << sizeof(pixelVector) << endl
		<< "Collision Vect: " << sizeof(collisionVector) << endl
		<< "DebugImage: " << sizeof(debugImage) << endl
		<< "DebugCollision: " << sizeof(debugCollision) << endl
		<< "Total Without Debug: " << sizeof(image) + sizeof(pixelVector)
		+ sizeof(collisionVector) << endl
		<< "Total With Debug: " << sizeof(image) + sizeof(pixelVector)
		+ sizeof(collisionVector) + sizeof(debugImage) + sizeof(debugCollision)
		<< endl;
	//Put back into their format.
	int pixelSize = pixelVector.size();
	for(int i = 0; i < pixelSize; i++)
	{
		debugImage.push_back(pixelVector[i]->r);
		debugImage.push_back(pixelVector[i]->g);
		debugImage.push_back(pixelVector[i]->b);
		debugImage.push_back(pixelVector[i]->a);
	}
	//Encode the image
    unsigned error = lodepng::encode(debugFilename, debugImage, imgWidth, imgHeight);

    //if there's an error, display it
    if(error) std::cout << "encoder error " << error << ": "<< lodepng_error_text(error) << std::endl;
}

void CollisionGenerator::ColorCulling()
{
	int pixelSize = pixelVector.size();
	for(int i = 0; i < pixelSize; i++)
	{
		//Todo, Add ColorCushioning.
		if((pixelVector[i]->r > r - cushion) && (pixelVector[i]->g > g - cushion) &&
			(pixelVector[i]->b > b - cushion) && (pixelVector[i]->a > a - cushion))
		{
			pixelVector[i]->colorCull = true;
			collisionVector[pixelVector[i]->x][pixelVector[i]->y]->colorCulled = true;
			//DEBUG
			pixelVector[i]->r = 255;
			pixelVector[i]->g = 0;
			pixelVector[i]->b = 0;
			pixelVector[i]->a = 255;
		}
	}
}

void CollisionGenerator::GenerateCollisionVector()
{
	collisionVector.reserve(imgWidth + reserveBuffer);
	for(unsigned int j = 0; j < imgWidth; j++)
	{
		std::vector<Collideable*> cVect;
		cVect.reserve(imgHeight + reserveBuffer);
		for(unsigned int i = 0; i < imgHeight; i++)
		{
			Collideable* c = new Collideable;
			c->colorCulled = false;
			c->culled = false;
			c->x = -5;
			c->y = -5;
			cVect.push_back(c);
		}
		collisionVector.push_back(cVect);
	}
}

void CollisionGenerator::LocationCulling()
{
	bool left = false;
	bool right = false;
	bool top = false;
	bool bottom = false;
	for(unsigned int i = 1; i < imgWidth - 1; i++)
	{
		for(unsigned int j = 1; j < imgHeight - 1; j++)
		{
			//std::cout << "---------------CULLING---------------" << std::endl;
			//std::cout << "X:" << i << " Y:" << j << std::endl;
			if(collisionVector[i - 1][j]->x != -5 && collisionVector[i - 1][j]->colorCulled == false)
				left = true;
			if(collisionVector[i + 1][j]->x != -5 && collisionVector[i + 1][j]->colorCulled == false)
				right = true;
			if(collisionVector[i][j - 1]->x != -5 && collisionVector[i][j - 1]->colorCulled == false)
				bottom = true;
			if(collisionVector[i][j + 1]->x != -5 && collisionVector[i][j + 1]->colorCulled == false)
				top = true;
			if(top && bottom && left && right)
			{
				collisionVector[i][j]->x = -1;
				collisionVector[i][j]->y = -1;
				collisionVector[i][j]->culled = true;
				//Debug
				Collideable* debug = new Collideable;
				debug->x = i;
				debug->y = j;
				debug->culled = true;
				debugCollision.push_back(debug);
				//TEMP DEBUG
				pixelVector[((j * imgWidth) + i)]->r = 0;
				pixelVector[((j * imgWidth) + i)]->g = 255;
				pixelVector[((j * imgWidth) + i)]->b = 0;
				pixelVector[((j * imgWidth) + i)]->a = 255;
			}
			left = false;
			right = false;
			top = false;
			bottom = false;
		}
	}

	/*std::cout << std::endl << "Culled coordinates ------------" << std::endl;
	for(unsigned int i = 0; i < imgWidth; i++)
	{
		for(unsigned int j = 0; j < imgHeight; j++)
		{
			std::cout << "SLOT: [" << i << "," << j << "]: (" <<
				collisionVector[i][j]->x << "," << collisionVector[i][j]->y << ")\t" << std::endl;
		}
	}*/
}

void CollisionGenerator::Shutdown()
{
	CollisionGenerator::~CollisionGenerator();
}

CollisionGenerator::~CollisionGenerator()
{
}