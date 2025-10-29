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
    return 0;
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
		hasDownRequest = false;
	}
	// If target floor is below current floor
	else {
		hasUpRequest = false;
		hasDownRequest = true;
	}
}

void Floor::removePeople(const int indicesToRemove[MAX_PEOPLE_PER_FLOOR],
                         int numPeopleToRemove) {
	// Place holder array for indices to Remove
	int targetToRemove[MAX_PEOPLE_PER_FLOOR]; 
	//Copy values into the place holder
	for (int i = 0; i < numPeopleToRemove; ++i) {
		targetToRemove[i] = indicesToRemove[i];
	}
	// Sort the place holder 
	sort(targetToRemove, targetToRemove + numPeopleToRemove);


	for (int i = 0; i < MAX_PEOPLE_PER_FLOOR; i++) {
		if (i == targetToRemove[i]) {
			for (int j = i; j < MAX_PEOPLE_PER_FLOOR - 1; j++) {
				people[j] = people[j + 1];
			}
			for (int k = 0; k < numPeopleToRemove; k++) {
				targetToRemove[k] = targetToRemove[k] - 1;
			}
		}
	}

}

void Floor::resetRequests() {
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
