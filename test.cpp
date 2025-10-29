/*
 * Copyright 2023 University of Michigan EECS183
 *
 * test.cpp
 * Project UID 848fee0125dbb5eb53ed294f20dbef81
 *
 * Final Project - Elevators
 */
 
////////////////////////////////////////////////
// You will submit test.cpp to the autograder //
// FOR LAB 8, but NOT the final project Core  //
////////////////////////////////////////////////

#include <iostream>
#include <fstream>
#include <string>
#include "AI.h"
#include "Building.h"
#include "BuildingState.h"
#include "Elevator.h"
#include "Floor.h"
#include "Game.h"
#include "Move.h"
#include "Person.h"
#include "SatisfactionIndex.h"
#include "Utility.h"

using namespace std;

// declare your test functions here
void file_check();
void test_Person();

void start_tests() {
    //file_check();
    test_Person();

    return;
}
void file_check() {

    // open one of the text files that are part of the starter code
    ifstream ins;
    ins.open("new.in");

    // if the file was not in the correct directory, the stream state is fail
    if (ins.fail()) {
        cout << "Could not open new.in" << endl;
    }
    else {
        cout << "Success! Text files are in the right directory." << endl;
    }

    return;
}

void test_Person() {
    cout << "Now testing Person class" << endl << endl;

    // Testing constructor
    Person p("0f0t5a5");

    cout << "Expected: 0, Actual: " << p.getTurn() << endl;
    cout << "Expected: 0, Actual: " << p.getCurrentFloor() << endl;
    cout << "Expected: 5, Actual: " << p.getTargetFloor() << endl;
    cout << "Expected: 5, Actual: " << p.getAngerLevel() << endl; 

    // Testing print function
    cout << "Expected: f0t5a5, Actual: ";
    p.print(cout);
    cout << endl;

    // Testing tick function
    bool exploded = p.tick(TICKS_PER_ANGER_INCREASE);
    
    cout << "Expected: 0, Actual: " << exploded << endl;

    cout << "Expected: 6, Actual: " << p.getAngerLevel() << endl;

    // Anger = 7
    p.tick(TICKS_PER_ANGER_INCREASE);
    // Anger = 8
    p.tick(TICKS_PER_ANGER_INCREASE);
    // Anger = 9
    p.tick(TICKS_PER_ANGER_INCREASE);

    exploded = p.tick(TICKS_PER_ANGER_INCREASE);

    cout << "Expected: 1, Actual: " << exploded << endl;

   
}
void test_Elevator() {
    cout << "Now testing Elevator class" << endl << endl;
    Elevator elevator; 
    
    cout << "Expected: 0, Actual: " << elevator.getCurrentFloor() << endl;
    cout << "Expected: 0, Actual: " << elevator.getTargetFloor() << endl;
    cout << "Expected: 0, Actual: " << elevator.isServicing() << endl;

    // Testing serviceRequest function

    elevator.serviceRequest(5);
    
    cout << "Expected: 5, Actual: " << elevator.getTargetFloor() << endl;
    cout << "Expected: 1, Actual: " << elevator.isServicing() << endl;


    // Testing tick function
    
    // Case 1: TargetFloor > Current Floor
    elevator.tick(TICKS_PER_ELEVATOR_MOVE);

    cout << "Expected: 1, Actual: " << elevator.getCurrentFloor() << endl;
    
    // Case 2: TargetFloor < Current Floor
    elevator.setCurrentFloor(6);
    elevator.serviceRequest(3);
    
    elevator.tick(TICKS_PER_ELEVATOR_MOVE);
    cout << "Expected: 5, Actual: " << elevator.getCurrentFloor() << endl;
    
    // Case 3: TargetFloor == Current Floor 
    elevator.setCurrentFloor(3);
    elevator.serviceRequest(3);
    elevator.tick(TICKS_PER_ELEVATOR_MOVE);
    
    cout << "Expected: 3, Actual: " << elevator.getCurrentFloor() << endl;
    cout << "Expected: 0, Actual: " << elevator.isServicing() << endl;


}

void test_Floor() {
    
}
// write test functions here
