#include "DoublyLinkedList.h"

DoublyLinkedList::DoublyLinkedList()
{
	firstNode = NULL;
	lastNode = NULL;
	count = 0;
}

DoublyLinkedList::~DoublyLinkedList()
{
	CoordinateNode* trackerNode = firstNode;
	CoordinateNode* currentNode = firstNode;

	while(currentNode != NULL)
	{
		trackerNode = currentNode;
		currentNode = currentNode->GetNext();
		delete trackerNode;
		count--;
	}
}

void DoublyLinkedList::Shutdown()
{
	DoublyLinkedList::~DoublyLinkedList();
}

void DoublyLinkedList::InsertBack(int x, int y)
{
	CoordinateNode* newNode = new CoordinateNode(x, y);
	newNode->SetNext(NULL);
	newNode->SetPrevious(NULL);
	if(count == 0)
	{
		firstNode = newNode;
	}
	else if(count == 1)
	{
		lastNode = newNode;
		firstNode->SetNext(newNode);
		newNode->SetPrevious(firstNode);
	}
	else
	{
		lastNode->SetNext(newNode);
		newNode->SetPrevious(lastNode);
		lastNode = newNode;
	}
	//Increment count
	count++;
	//Index
	if(static_cast<unsigned>(x) == fileSystemX.size())
	{
		fileSystemX.push_back(newNode);
	}
}

void DoublyLinkedList::Debug()
{
	//Debug
	std::cout << "File Size: " << fileSystemX.size() << std::endl;
	for(unsigned int i = 0; i < fileSystemX.size(); i++)
	{
		std::cout << "FileSystemX: " << fileSystemX[i]->GetDataX() << std::endl;
	}
}

void DoublyLinkedList::Print()
{
	CoordinateNode* current = firstNode;
	std::cout << std::endl << "Count: " << count << std::endl;
	//while(current)
	//{
	//	std::cout << "Current (X,Y): (" << current->GetDataX() << "," <<
	//		current->GetDataY() << ")" << std::endl;
	//	current = current->GetNext();
	//}
	//Debug
	Debug();
}