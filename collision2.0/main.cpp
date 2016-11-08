#include "LodePNG.h"
#include "CollisionGenerator.h"
#include "Collision.h"

int main()
{
	//Start 600mb
	CollisionGenerator* cg = new CollisionGenerator("extremeTest.png");
	cg->Initialize();
	Collision* c = new Collision("extremeTest.ecd");
	//c->DebugPrint();
	std::cout << "Testing collision checktime." << std::endl;
	double startTime, endTime;
	Coordinate* coord = new Coordinate();
	//startTime = getCPUTime();
	coord = c->Collided(1728, 4, 0, 0);
	//endTime = getCPUTime();
	//fprintf(stderr, "CPU Time used to check Collision: ", (endTime - startTime));
	//cout << "StartTime: " << startTime << endl;
	//cout << "EndTime: " << endTime << endl;
	cout << "Coord data: " << coord->x << " " << coord->y << endl;
	cout << std::endl;
	//c->DebugPrint();
	int a;
	cin >> a;
	return 0;
}