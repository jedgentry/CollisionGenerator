#ifndef COLLISION_H
#define COLLISION_H

#include <fstream>
#include "DoublyLinkedList.h"

//Structure of coordinates.
struct Coordinate
{
	//X value of coordinate.
	unsigned int x;
	//Y value of coordinate.
	unsigned int y;
};

class Collision
{
public:
	//Default Constructor.
	Collision();
	//Overlaoded Constructor
	//@param string - name of the file to open and parse.
	Collision(std::string);
	//Overloaded Constructor
	//@param string - name of the file to open and parse.
	//@param int - value of "knockback" when colliding with the object.
	Collision(std::string, int);
	//Default Copy Constructor.
	Collision(const Collision&);
	//Use for arbitrary pixel values on scalable rendering.
	//Otherwise will knockback '1' pixel
	//SetKnockBack - Sets the knockback value of the object.
	//@param int - value to set knockback to.
	void SetKnockback(int knockback) {collisionKnockback = knockback;};
	//Collided - Returns the coordinate of the object and parses collision. Later can do physics as well!
	//@param int - x value of the collider.
	//@param int - y value of the collider.
	//@param int - x value of the second collider.
	//@param int - y value of the second collider.
	//@return Coordinate* - Coordinate value of where the collider should be based on knockback
	//and soon to be physics is returned.
	Coordinate* Collided(int, int, int, int);
	//Default Destructor.
	~Collision();
	//Debug
	void DebugPrint();
private:
	//List of the coordinates to test collision on.
	DoublyLinkedList* collisionData;
	//Name of the file to open.
	std::string filename;
	//Height of the image.
	unsigned int height;
	//Width of the image.
	unsigned int width;
	//Amount to knockback on collision.
	int collisionKnockback;
};

#endif