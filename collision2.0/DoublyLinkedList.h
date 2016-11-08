#ifndef DOUBLYLINKEDLIST_H
#define DOUBLYLINKEDLIST_H

#include <string>
//Optimization.
#include <vector>
//Debug
#include <iostream>
//End Debug

class CoordinateNode
{
public:
	//Default Constructor.
	CoordinateNode()
	{
		previousNode = NULL;
		nextNode = NULL;
	};
	//Copy Constructor.
	//@param CoordinateNode - node to copy.
	CoordinateNode(const CoordinateNode& oldNode)
	{
	};
	//CoordinateNode - Overloaded Constructor.
	//@param int - x value of the coordinate.
	//@param int - y value of the coordinate.
	CoordinateNode(int x, int y)
	{
		previousNode = NULL;
		nextNode = NULL;
		this->x = x;
		this->y = y;
	};
	//GetDataX - Returns the x data.
	//@return int - returns the X coordinate value.
	int GetDataX()
	{
		return x;
	};
	//GetDataY - Returns the Y data.
	//@return int - returns the y coordinate value.
	int GetDataY()
	{
		return y;
	};
	//SetX - Sets the X value of the coordinate.
	//@param int - x value to set the coordiante.
	void SetX(int x)
	{
		this->x = x;
	};
	//SetY - Sets the Y value of the coordinate.
	//@param int - y value to set the coordinate.
	void SetY(int y)
	{
		this->y = y;
	};
	//GetNext - Gets the next node.
	//@return CoordinateNode* - pointer to the next node.
	CoordinateNode* GetNext()
	{
		return nextNode;
	};
	//GetPrevious - Gets the previous Node.
	//@return CoordinateNode* - pointer to the previous node.
	CoordinateNode* GetPrevious()
	{
		return previousNode;
	};
	//SetNext - Sets the next node.
	//@param CoordinateNode* - Value to set the next node to.
	void SetNext(CoordinateNode* n)
	{
		nextNode = n;
	};
	//SetPrevious - Sets the previous Node.
	//@param CoordinateNode* - value to set the previous node to.
	void SetPrevious(CoordinateNode* n)
	{
		previousNode = n;
	};
	//~CoordianteNode - Default Destructor.
	~CoordinateNode()
	{
		previousNode = NULL;
		nextNode = NULL;
		x = -1;
		y = -1;
	};
private:
	//previousNode - value of the previousNode.
	CoordinateNode* previousNode;
	//nextNode - Value of the next node.
	CoordinateNode* nextNode;
	//Value of the x coordinate.
	int x;
	//Value of the Y coordinate.
	int y;
};


class DoublyLinkedList
{
public:
	//DoublyLinkedList - Default Constructor.
	DoublyLinkedList();
	//InsertBack - Inserts the coordinate to the back of the list.
	//@param int - x value of the coordinate.
	//@param int - y value of the coordinate.
	void InsertBack(int, int);
	//GetLength - Gets the length of the list.
	//@return int - number of the length.
	int GetLength(){return count;};
	//Shutdown - Shutsdown the list.
	void Shutdown();
	//GetFirst - Gets the first node in the list.
	//@return CoordinateNode* - returns the first node.
	CoordinateNode* GetFirst() {return firstNode;};
	//GetLast - Gets the last node in the list.
	//@return CoordinateNode* - Returns the last node.
	CoordinateNode* GetLast() {return lastNode;};
	//GetIndexFilesystem - Returns the pointer starting at the value of the index given.
	//@param index - X value where to skip to in the list.
	//@return CoordinateNode* - Pointer that is returned to the start of the index value given.
	CoordinateNode* GetIndexFilesystem(int index) {return fileSystemX[index];};

	//Debug
	void Print();
	void Debug();
	//Default Destructor
	~DoublyLinkedList();
private:
	//Value representing the amount of items in the list.
	unsigned int count;
	//Pointer to the start of the list.
	CoordinateNode* firstNode;
	//Pointer to the end of the list.
	CoordinateNode* lastNode;
	//fileSystemX vector of X coordinates to skip to in ordered list.
	std::vector<CoordinateNode*> fileSystemX;
};

#endif