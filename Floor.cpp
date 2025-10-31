/*
 * Copyright 2023 University of Michigan EECS183
 *
 * Floor.cpp
 * Project UID 848fee0125dbb5eb53ed294f20dbef81
 *
 * <#Names#>
 * <#Uniqnames#>
 *
 * Final Project - Elevators
 */


#include "Floor.h"
#include <algorithm>
using namespace std;

int Floor::tick(int currentTime) {
	// Record who exploded with index of array people
	int indicesToRemove[MAX_PEOPLE_PER_FLOOR];
	// Keep track of number of people explode
	int count = 0;
	// Access each person in the list People
	for (int i = 0; i < numPeople; i++) {
		// Increment each person's tick and check if person exploded
		if (people[i].tick(currentTime)) {
			indicesToRemove[count] = i;
			count += 1;
		}

	}
	// Remove people who exploded with anger
	removePeople(indicesToRemove, count);
    return count;
}

void Floor::addPerson(Person newPerson, int request) {
	// Add new person to people if there is still room
	if (numPeople < MAX_PEOPLE_PER_FLOOR) {
		people[numPeople] = newPerson;
		numPeople += 1;
	}
	// If target floor is above current floor
	if (request > 0) {
		hasUpRequest = true;
	}
	// If target floor is below current floor
	else {
		hasDownRequest = true;
	}
}

void Floor::removePeople(const int indicesToRemove[MAX_PEOPLE_PER_FLOOR],
                         int numPeopleToRemove) {

	Person newPeople[MAX_PEOPLE_PER_FLOOR];
	int h = 0;
	for (int i = 0; i < numPeople; i++) {
		bool remove = false;
		for (int j = 0; j < numPeopleToRemove; j++) {
			if (indicesToRemove[j] == i) {
				remove = true;
			}
		}
		if (!remove) {
			newPeople[h] = people[i];
			h++;
		}
	}
	for (int k = 0; k < MAX_PEOPLE_PER_FLOOR; k++) {
		people[k] = newPeople[k];
	}
	numPeople = h;
	resetRequests();
	
}

void Floor::resetRequests() {
	hasUpRequest = false;
	hasDownRequest = false;
	for (int i = 0; i < numPeople; i++) {
		if (people[i].getTargetFloor() > people[i].getCurrentFloor()) {
			hasUpRequest = true;
		}
		else if (people[i].getTargetFloor() < people[i].getCurrentFloor()) {
			hasDownRequest = true;
		}
	}
}

//////////////////////////////////////////////////////
////// DO NOT MODIFY ANY CODE BENEATH THIS LINE //////
//////////////////////////////////////////////////////

Floor::Floor() {
    hasDownRequest = false;
    hasUpRequest = false;
    numPeople = 0;
}

void Floor::prettyPrintFloorLine1(ostream& outs) const {
	string up = "U";
	outs << (hasUpRequest ? up : " ") << " ";
	for (int i = 0; i < numPeople; ++i){
		outs << people[i].getAngerLevel();
		outs << ((people[i].getAngerLevel() < MAX_ANGER) ? "   " : "  ");
	}
	outs << endl;
}

void Floor::prettyPrintFloorLine2(ostream& outs) const {
	string down = "D";
	outs << (hasDownRequest ? down : " ") << " ";
	for (int i = 0; i < numPeople; ++i) {
		outs << "o   ";
	}
	outs << endl;
}

void Floor::printFloorPickupMenu(ostream& outs) const {
	cout << endl;
	outs << "Select People to Load by Index" << endl;
	outs << "All people must be going in same direction!";
	/*  O   O
	// -|- -|-
	//  |   |
	// / \ / \  */
	outs << endl << "              ";
	for (int i = 0; i < numPeople; ++i) {
		outs << " O   ";
	}
	outs << endl << "              ";
	for (int i = 0; i < numPeople; ++i) {
		outs << "-|-  ";
	}
	outs << endl << "              ";
	for (int i = 0; i < numPeople; ++i) {
		outs << " |   ";
	}
	outs << endl << "              ";
	for (int i = 0; i < numPeople; ++i) {
		outs << "/ \\  ";
	}
	outs << endl << "INDEX:        ";
	for (int i = 0; i < numPeople; ++i) {
		outs << " " << i << "   ";
	}
	outs << endl << "ANGER:        ";
	for (int i = 0; i < numPeople; ++i) {
		outs << " " << people[i].getAngerLevel() << "   ";
	}
	outs << endl << "TARGET FLOOR: ";
	for (int i = 0; i < numPeople; ++i) {
		outs << " " << people[i].getTargetFloor() << "   ";
	}
}

void Floor::setHasUpRequest(bool hasRequest) {
    hasUpRequest = hasRequest;
}

bool Floor::getHasUpRequest() const {
	return hasUpRequest;
}

void Floor::setHasDownRequest(bool hasRequest) {
    hasDownRequest = hasRequest;
}

bool Floor::getHasDownRequest() const {
	return hasDownRequest;
}

int Floor::getNumPeople() const {
    return numPeople;
}

Person Floor::getPersonByIndex(int index) const {
    return people[index];
}
