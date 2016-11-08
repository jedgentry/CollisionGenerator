#include "Collision.h"

Collision::Collision()
{
	collisionData = NULL;
	filename = "";
	height = 0;
	width = 0;
	collisionKnockback = 1;
}

Collision::Collision(std::string filename)
{
	//setup file.
	this->filename = filename;
	//create list
	collisionData = new DoublyLinkedList();
	//Open up file.
	std::ifstream fin;
	fin.open(filename);
	height = 0;
	width = 0;
	//Read in initial width and height.
	fin >> width >> height;
	int tempX = -1;
	int tempY = -1;
	//Read till end of file and load in collision data.
	while(fin >> tempX >> tempY)
	{
		collisionData->InsertBack(tempX, tempY);
	}
	//Collision is done loading.
	fin.close();
	collisionKnockback = 1;
}

Collision::Collision(std::string filename, int knockback)
{
	collisionKnockback = knockback;
	//setup file.
	this->filename = filename;
	//create list
	collisionData = new DoublyLinkedList();
	//Open up file.
	std::ifstream fin;
	fin.open(filename);
	height = 0;
	width = 0;
	//Read in initial width and height.
	fin >> width >> height;
	int tempX = -1;
	int tempY = -1;
	//Read till end of file and load in collision data.
	while(fin >> tempX >> tempY)
	{
		collisionData->InsertBack(tempX, tempY);
	}
	//Collision is done loading.
	fin.close();
}

Collision::Collision(const Collision& c)
{
	collisionData = c.collisionData;
	filename = c.filename;
	height = c.height;
	width = c.width;
	collisionKnockback = c.collisionKnockback;
}

Coordinate* Collision::Collided(int colliderX, int colliderY, int objectX, int objectY)
{
	//Debug
	std::cout << "Collision data size: " << collisionData->GetLength() << std::endl;
	//End Debug
	//Calculate distance to see if we should even check
	Coordinate* result = new Coordinate();
	result->x = -1;
	result->y = -1;
	if((colliderX < objectX) || (colliderY > objectY + static_cast<int>(height)) ||
		(colliderY < objectY) || (colliderX > objectX + static_cast<int>(width)))
		return result;
	else
	{
		//Parse Collision Linked List and check for collision.
		CoordinateNode* tracker = collisionData->GetIndexFilesystem(objectX);
		//Make new coordinate
		while(tracker)
		{
			if((colliderX == (objectX + tracker->GetDataX())))
			{
				result->x = colliderX - collisionKnockback;
				if(result->x < 0)
					result->x = 0;
			}
			if((colliderY == (objectY + tracker->GetDataY())))
			{
				result->y = colliderY - collisionKnockback;
				if(result->y < 0)
					result->y = 0;
			}
			tracker = tracker->GetNext();
		}
		return result;
	}
}

void Collision::DebugPrint()
{
	std::cout << "Image Width, Height: (" << width << "," << height << ")" << std::endl
		<< "Filename & Knockback: " << filename << " " << collisionKnockback << std::endl;
	collisionData->Print();
}