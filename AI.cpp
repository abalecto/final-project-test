/*
 * Copyright 2023 University of Michigan EECS183
 *
 * AI.cpp
 * Project UID 848fee0125dbb5eb53ed294f20dbef81
 *
 * <#Names#>
 * <#Uniqnames#>
 *
 * Final Project - Elevators
 */

#include "AI.h"
#include <cassert>
#include <vector>


const int ANGER_THRESHOLD = 7;
const int FLOOR_THRESHOLD = 5;
const int AREA_A[] = {0, 1, 2, 3};
const int AREA_B[] = {4, 5, 6};
const int AREA_C[] = {7, 8, 9};



// Function declarations
void copyPeople(Floor floor, Person people[MAX_PEOPLE_PER_FLOOR]);
int getMaxAngerPerFloor(const _Floor& floor);
string indicesUpDirection(const Floor& floor, const Person people[]);
string indicesDownDirection(const Floor& floor, const Person people[]);
int floorUrgentScore(const _Floor& floor);
char findTargetArea(int targetFloor);
string indicesOfHighAnger(const Person people[], string indices);
double averageDistance(string indices, Person people[]);
int farthestFloor(string indices, const Person people[]);
string indicesUnderFloor(string indices, const Person people[], int farthestFloor, bool up);
int findFarthestTravel(string indices, const Person people[]);

// This file is used only in the Reach, not the Core.
// You do not need to make any changes to this file for the Core

string getAIMoveString(const BuildingState& buildingState) {
    string AImove = "";

    // Check if elevators are serving 
    
    bool serving0 = buildingState.elevators[0].isServicing;
    bool serving1 = buildingState.elevators[1].isServicing;
    bool serving2 = buildingState.elevators[2].isServicing;

    //First three turns, set elevators to their respective floors
    if (serving0 && serving1 && serving2) {
        return "";
    }

    if (buildingState.turn == 0) {
        return "e2f8";
    }
    else if (buildingState.turn == 1) {
        return "e1f5";
    }
    
    else if (buildingState.turn == 2) {
        return "e0f1";
    }
    
    //current and target positions of elevators

    int elevator0Current = buildingState.elevators[0].currentFloor;
    int elevator1Current = buildingState.elevators[1].currentFloor;
    int elevator2Current = buildingState.elevators[2].currentFloor;

    int elevator0Target = buildingState.elevators[0].targetFloor;
    int elevator1Target = buildingState.elevators[1].targetFloor;
    int elevator2Target = buildingState.elevators[2].targetFloor;
    
    char elevator0TargetArea = findTargetArea(elevator0Target);
    char elevator1TargetArea = findTargetArea(elevator1Target);
    char elevator2TargetArea = findTargetArea(elevator2Target);
    
    // Compute urgency scores for each floor

    int urgencyScores[NUM_FLOORS];
    int urgencyScoresA[4];
    int urgencyScoresB[3];
    int urgencyScoresC[3];
    
    for (int i = 0; i < NUM_FLOORS; i++) {
        urgencyScores[i] = floorUrgentScore(buildingState.floors[i]);
    }
    for (int i = 0; i < 4; i++) {
        urgencyScoresA[i] = floorUrgentScore(buildingState.floors[i]);
    }
    for (int i = 0; i < 3; i++) {
        urgencyScoresB[i] = floorUrgentScore(buildingState.floors[i + 4]);
    }
    for (int i = 0; i < 3; i++) {
        urgencyScoresC[i] = floorUrgentScore(buildingState.floors[i + 7]);
    }
    
    // Find the highest urgent floors of each area
    int highUrgency = -1;
    int urgentFloor = -1;

    int highUrgencyA = -1;
    int urgentFloorA = -1;

    int highUrgencyB = -1;
    int urgentFloorB = -1;

    int urgentFloorC = -1;
    int highUrgencyC = -1;

    for (int i = 0; i < NUM_FLOORS; i++) {
        int currentUrgency = urgencyScores[i];
        if (currentUrgency > highUrgency) {
            highUrgency = currentUrgency;
            urgentFloor = i;
        }
    }


    for (int i = 0; i < 4; i++) {
        int currentUrgencyA = urgencyScoresA[i];
        if (currentUrgencyA > highUrgencyA) {
            highUrgencyA = currentUrgencyA;
            urgentFloorA = i;
        }
    }

    for (int i = 0; i < 3; i++) {
        int currentUrgencyB = urgencyScoresB[i];
        int currentUrgencyC = urgencyScoresC[i];

        if (currentUrgencyB > highUrgencyB) {
            highUrgencyB = currentUrgencyB;
            urgentFloorB = i + 4;
        }
        if (currentUrgencyC > highUrgencyC) {
            highUrgencyC = currentUrgencyC;
            urgentFloorC = i + 7;
        }

    }
    
    string AImove1 = "";
    string AImove2 = "";
    string AImove3 = "";
    if (!serving0) {
        AImove1.push_back('e');
        AImove1.push_back('0');
        if (elevator0TargetArea == 'A' && elevator0Current == urgentFloorA && highUrgencyA != 0) {
            AImove1.push_back('p');
            return AImove1;
  
        }
        else if (elevator0TargetArea == 'A' && elevator0Current != urgentFloorA && highUrgencyA != 0) {
            AImove1.push_back('f');
            AImove1 += to_string(urgentFloorA);
        }
        else if (elevator0TargetArea == 'B' && elevator0Current == urgentFloorB && highUrgencyB != 0) {
            AImove1.push_back('p');
            return AImove1;
        }
        else if (elevator0TargetArea == 'B' && elevator0Current != urgentFloorB && highUrgencyB != 0) {
            AImove1.push_back('f');
            AImove1 += to_string(urgentFloorB);
        }
        else if (elevator0TargetArea == 'C' && elevator0Current == urgentFloorC && highUrgencyC != 0) {
            AImove1.push_back('p');
            return AImove1;
        }
        else if (elevator0TargetArea == 'C' && elevator0Current != urgentFloorC && highUrgencyC != 0) {
            AImove1.push_back('f');
            AImove1 += to_string(urgentFloorC);
        }
        else if (abs(elevator0Current - urgentFloor) <= 5 && highUrgency != 0) {
            AImove1.push_back('f');
            AImove1 += to_string(urgentFloor);
        }
        else if (elevator0Current == urgentFloor && highUrgency != 0) {
            AImove1.push_back('p');
            return AImove1;
        }
        
    } 
    if (!serving1) {
        AImove2.push_back('e');
        AImove2.push_back('1');
        if (elevator1TargetArea == 'A' && elevator1Current == urgentFloorA && highUrgencyA != 0) {
            AImove2.push_back('p');
            return AImove2;
        }
        else if (elevator1TargetArea == 'A' && elevator1Current != urgentFloorA && highUrgencyA != 0) {
            AImove2.push_back('f');
            AImove2 += to_string(urgentFloorA);
        }
        else if (elevator1TargetArea == 'B' && elevator1Current == urgentFloorB && highUrgencyB != 0) {
            AImove2.push_back('p');
            return AImove2;
        }
        else if (elevator1TargetArea == 'B' && elevator1Current != urgentFloorB && highUrgencyB != 0) {
            AImove2.push_back('f');
            AImove2 += to_string(urgentFloorB);
        }
        else if (elevator1TargetArea == 'C' && elevator1Current == urgentFloorC && highUrgencyC != 0) {
            AImove2.push_back('p');
            return AImove2;
        }
        else if (elevator1TargetArea == 'C' && elevator1Current != urgentFloorC && highUrgencyC != 0) {
            AImove2.push_back('f');
            AImove2 += to_string(urgentFloorC);
        }
        else if (abs(elevator1Current - urgentFloor) <= 5 && highUrgency != 0) {
            AImove2.push_back('f');
            AImove2 += to_string(urgentFloor);
        }
        else if (elevator1Current == urgentFloor && highUrgency != 0) {
            AImove2.push_back('p');
            return AImove2;
        }
        else if ((!serving0 && elevator1TargetArea == elevator0TargetArea)) {
            AImove2.push_back('f');
            AImove2 += to_string(urgentFloor);
        }

    }
    if (!serving2) {
        AImove3.push_back('e');
        AImove3.push_back('2');
        if (elevator2TargetArea == 'A' && elevator2Current == urgentFloorA && highUrgencyA != 0) {
            AImove3.push_back('p');
            return AImove3;
        }
        else if (elevator2TargetArea == 'A' && elevator2Current != urgentFloorA && highUrgencyA != 0) {
            AImove3.push_back('f');
            AImove3 += to_string(urgentFloorA);
        }
        else if (elevator2TargetArea == 'B' && elevator2Current == urgentFloorB && highUrgencyB != 0) {
            AImove3.push_back('p');
            return AImove3;
        }
        else if (elevator2TargetArea == 'B' && elevator2Current != urgentFloorB && highUrgencyB != 0) {
            AImove3.push_back('f');
            AImove3 += to_string(urgentFloorB);
        }
        else if (elevator2TargetArea == 'C' && elevator2Current == urgentFloorC && highUrgencyC != 0) {
            AImove3.push_back('p');
            return AImove3;
        }
        else if (elevator2TargetArea == 'C' && elevator2Current != urgentFloorC && highUrgencyC != 0) {
            AImove3.push_back('f');
            AImove3 += to_string(urgentFloorC);
        }
        else if (abs(elevator2Current - urgentFloor) <= 5 && highUrgency != 0) {
            AImove3.push_back('f');
            AImove3 += to_string(urgentFloor);
        }
        else if (elevator2Current == urgentFloor && highUrgency != 0) {
            AImove3.push_back('p');
            return AImove3;
        }
        else if ((!serving0 && elevator2TargetArea == elevator0TargetArea) || (!serving1 && elevator2TargetArea == elevator1TargetArea)) {
            AImove3.push_back('f');
            AImove3 += to_string(urgentFloor);
        }
    }

    if (AImove1.at(2) == 'p') {
        return AImove1;
    }
    else if (AImove2.at(2) == 'p') {
        return AImove2;
    }
    else if (AImove3.at(2) == 'p') {
        return AImove3;
    }
    else {
        if (buildingState.turn % 3 == 1) {
            return AImove1; 
        }
        else if (buildingState.turn % 3 == 2) {
            return AImove2;
        }
        else if (buildingState.turn % 3 == 0) {
            return AImove3;
        }
    }


    return "";
    
}

string getAIPickupList(const Move& move, const BuildingState& buildingState, 
                       const Floor& floorToPickup) {
    string AImove = "";
    bool serving0 = buildingState.elevators[0].isServicing;
    bool serving1 = buildingState.elevators[1].isServicing;
    bool serving2 = buildingState.elevators[2].isServicing;

    Person people[MAX_PEOPLE_PER_FLOOR];
    copyPeople(floorToPickup, people);
    int numPeople = floorToPickup.getNumPeople();

    // Separate people who are going up and down
    string upDirection = indicesUpDirection(floorToPickup, people);
    string downDirection = indicesDownDirection(floorToPickup, people);

    // Look for people with high anger (above 7) within each directions 
   
    string upHighAnger = indicesOfHighAnger(people, upDirection);
    string downHighAnger = indicesOfHighAnger(people, downDirection);

    cout << downHighAnger << endl;


    // Return people with high anger, with direction with people with more high anger
    if (upHighAnger.length() > downHighAnger.length()) {
        int farthestTarget = farthestFloor(upHighAnger, people);
        AImove = indicesUnderFloor(upDirection, people, farthestTarget, true);
        return AImove;
    }
    else if (upHighAnger.length() < downHighAnger.length()) {
        int farthestTarget = farthestFloor(downHighAnger, people);
        AImove = indicesUnderFloor(downDirection, people, farthestTarget, false);
        return AImove;
    }
    else if (upHighAnger.length() == downHighAnger.length() && upHighAnger != "") {
        if (averageDistance(upHighAnger, people) > averageDistance(downHighAnger, people)) {
            int farthestTarget = farthestFloor(upHighAnger, people);
            AImove = indicesUnderFloor(upDirection, people, farthestTarget, true);
            return AImove;
        }
        else {
            int farthestTarget = farthestFloor(downHighAnger, people);
            AImove = indicesUnderFloor(downDirection, people, farthestTarget, false);
            return AImove;
        }
    }

    // If a floor doesn't have anyone with high anger
    if (upDirection.length() > downDirection.length()) {
        AImove = upDirection;
        return AImove;
    }
    else if (upDirection.length() < downDirection.length()) {
        AImove = downDirection;
        return AImove;
    }
    else if (upDirection.length() == downDirection.length() && upDirection.length() != 0) {
        int distanceUp = findFarthestTravel(upDirection, people);
        int distanceDown = findFarthestTravel(downDirection, people);
        if (distanceUp > distanceDown) {
            AImove = upDirection;
            return AImove;
        }
        else if (distanceUp < distanceDown) {
            AImove = downDirection;
            return AImove;
        }
    }
    
    return "0";
    
}


////////////////////////////////////
// Helper functions for Move String 
////////////////////////////////////

// Finds the floor with the most people 
int getMostPeopleFloor(const BuildingState& buildingState) {
    int mostNumFloor = 0;
    int mostNumPeople = 0;
    int currentNumPeople;
    for (int i = 0; i < NUM_FLOORS; i++) {
        currentNumPeople = buildingState.floors[i].numPeople;
        if (currentNumPeople >= mostNumPeople) {
            mostNumPeople = currentNumPeople;
            mostNumFloor = i;
        }
    }
    return mostNumFloor;
}

// Get the highest anger of a person given a floor
int getMaxAngerPerFloor(const _Floor& floor) {
    int numPeople = floor.numPeople;
    int maxAnger = 0;
    for (int i = 0; i < numPeople; i++) {
        int currentAnger = floor.people[i].angerLevel;
        if (currentAnger > maxAnger) {
            maxAnger = currentAnger;
        }
    }
    return maxAnger;
}


// return the number of people with anger above threshold given a floor
int numberOfHighAnger(const _Floor& floor) {
    int numPeople = floor.numPeople;
    int highAngerPeople = 0;
    for (int i = 0; i < numPeople; i++) {
        if (floor.people[i].angerLevel > ANGER_THRESHOLD) {
            highAngerPeople += 1;
        }
    }
    return highAngerPeople;
}

// 
int floorUrgentScore(const _Floor& floor) {
    int maxAnger = getMaxAngerPerFloor(floor);
    int highAngerPeople = numberOfHighAnger(floor);
    int numPeople = floor.numPeople;
    if (numPeople < FLOOR_THRESHOLD) {
        return numPeople; 
    }
    return (3 * maxAnger + 6 * highAngerPeople + numPeople);

}

// Set elevator to original positions (e0 -> floor 1, e1 -> floor 5, e2 -> f8)
string setToOriginal(_Elevator elevator) {
    if (elevator.elevatorId == 0) {
        return "e0f1";
    }
    else if (elevator.elevatorId == 1) {
        return "e1f5";
    }
    else {
        return "e2f8";
    }
}

// Finds the elevator closest to some floor given the floor
int closestElevator (int floor, const BuildingState& buildingState) {
    int shortestDistance = 11; 
    int closestElevatorId = -1;
    for (int i = 0; i < NUM_ELEVATORS; i++) {
        _Elevator elevator = buildingState.elevators[i];
        int currentDistance;

        // Compare targetFloor if servicing, if not compare current
        if (elevator.isServicing) {
            currentDistance = abs(floor - elevator.targetFloor);
        }
        else {
            currentDistance = abs(floor - elevator.currentFloor);
        }

        if (currentDistance < shortestDistance) {
            closestElevatorId = i;
            shortestDistance = currentDistance;
        }
    }
    return closestElevatorId;
}


char findTargetArea(int targetFloor) {
    for (int i = 0; i < 4; i++) {
        if (targetFloor == AREA_A[i]) {
            return 'A';
        }
    }
    for (int i = 0; i < 3; i++) {
        if (targetFloor == AREA_B[i]) {
            return'B';
        }
        else if (targetFloor == AREA_C[i]) {
            return 'C';
        }
    }
    return '0';
}




////////////////////////////////////
// Helper functions for pickUp String
////////////////////////////////////


// copies people of floor so information is accessible, 
// since people[] is private within Floor class
void copyPeople(Floor floor, Person people[MAX_PEOPLE_PER_FLOOR]) {
    int numPeople = floor.getNumPeople();
    for (int i = 0; i < numPeople; i++) {
        people[i] = floor.getPersonByIndex(i);
    }
    return;
}

// Finds the farthest distance of travel with in a list of indices
int findFarthestTravel(string indices, const Person people[]) {
    int numPeople = indices.length();
    int farthestDistance = 0;
    for (int i = 0; i < numPeople; i++) {
        int index = indices.at(i) - '0';
        int currentFloor = people[index].getCurrentFloor();
        int targetFloor = people[index].getTargetFloor();
        if (abs(targetFloor - currentFloor) > farthestDistance) {
            farthestDistance = abs(targetFloor - currentFloor);
        }
    }
   
    return farthestDistance;
}



// Finding an element within a list
bool withinList(const int list[], int size, int lookFor) {
    bool found = false;
    for (int i = 0; i < size; i++) {
        if (list[i] == lookFor) {
            found = true;
        }
    }
    return found;
}

// give indices of people with anger above 7 
// given a floor
string indicesOfHighAnger(const Person people[], string indices) {
    string newIndices = "";
    int numPeople = indices.size();
    for (int i = 0; i < numPeople; i++) {
        int index = indices.at(i) - '0';
        int angerLevel = people[index].getAngerLevel();
        if (angerLevel >= ANGER_THRESHOLD) {
            newIndices.push_back(indices.at(i));
        }
    }
    
    return newIndices;
}


// indices of people who wants to go up
string indicesUpDirection(const Floor& floor, const Person people[]) {
    string upDirection = "";
    int numPeople = floor.getNumPeople();
    int j = 0;
    for (int i = 0; i < numPeople; i++) {
        if (people[i].getTargetFloor() > people[i].getCurrentFloor()) {
             upDirection += to_string(i);
            j++;
        }
    }
    return upDirection;
}

// indices of people who wants to go up
string indicesDownDirection(const Floor& floor, const Person people[]) {
    string downDirection = "";
    int numPeople = floor.getNumPeople();
    for (int i = 0; i < numPeople; i++) {
        if (people[i].getTargetFloor() < people[i].getCurrentFloor()) {
            downDirection += to_string(i);
        }
    }
    return downDirection;
}

 double averageDistance(string indices, Person people[]) {
    int numPeople = indices.length();
    int sum = 0;
    for (int i = 0; i < numPeople; i++) {
        int index = indices.at(i) - '0';
        int distance = abs(people[index].getTargetFloor() - people[index].getCurrentFloor());
        sum += distance;
    }
    return (static_cast<double>(sum)) / numPeople;
}
 
string sameTargetIndices(const Floor& floor, const Person people[], int targetFloor) {
    string toReturn = "";
    int numPeople = floor.getNumPeople();
    for (int i = 0; i < numPeople; i++) {
        if (people[i].getTargetFloor() == targetFloor) {
            toReturn += to_string(i); 
        }
    }
    return toReturn;
}

// Computes the farthest floor given a string of indices
int farthestFloor(string indices, const Person people[]) {
    int numPeople = indices.length();
    int longestDistance = 0;
    int longestTravelIndex = -1;
    for (int i = 0; i < numPeople; i++) {
        int index = indices.at(i) - '0';
        int distance = abs(people[index].getTargetFloor() - people[index].getCurrentFloor());
        if (distance > longestDistance) {
            longestTravelIndex = indices.at(i) - '0';
            longestDistance = distance;
        }
    }
    return people[longestTravelIndex].getTargetFloor();
}

string indicesUnderFloor(string indices, const Person people[], int farthestFloor, bool up) {
    string newIndices = "";
    int numPeople = indices.length();
    for (int i = 0; i < numPeople; i++) {
        int index = indices.at(i) - '0';
        if (up) {
            if (people[index].getTargetFloor() <= farthestFloor) {
                newIndices += indices.at(i);
            }
        }
        else {
            if (people[index].getTargetFloor() >= farthestFloor) {
                newIndices += indices.at(i);
            }
        }
        
    }
    return newIndices;
}






