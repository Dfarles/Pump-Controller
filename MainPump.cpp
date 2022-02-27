/**
 * @file MainPump.cpp
 * @author Theophania Labay, Daniel Kar, Dylan Farley, and Hayden Saalfeld
 * @brief 
 * @version 0.1
 * @date 2022-02-14
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#include <iostream>


using namespace std;

void primaryPump();
void secondaryPump(int);
bool pumpOn(int);
bool pumpOff(int);

double electricDemand = 0; // Instantaneous electric demand in KW
const double ELECTHRESHOLD = 1200; // Value in KW. Below this value, a pump will turn on if tank isn't full
double tankLevel = 17.047; // Level of tank in meters
const double PUMPSLOW [4] = {14.67, 14.37, 14.06, 13.76}; // Array of values in meters at which each pump comes on
const double PUMPSHIGH [4] = {18.02, 17.72, 17.41, 17.11}; // Array of values in meters at which each pump turns off
const double TANKHIGH = PUMPSHIGH[0];
const double TANKLOW = PUMPSLOW[3];
const int PUMPORDER [4] = {2, 1, 0, 3}; // Order that pumps come on

bool pumpStatus [4] = {0, 0, 0, 0}; // Array of pump on/off statuses
int pumpIndex = 0; // Value that gives the number of a given pump

int main()
{



while (true){

    cin >> electricDemand; //Get  instantaneous electrical demand
    cin >> tankLevel; // Get instantaneous tank level from model/simulator
    primaryPump();
    while (pumpIndex > 0){
        cin >> electricDemand; //Get  instantaneous electrical demand
        cin >> tankLevel; // Get instantaneous tank level from model/simulator

        secondaryPump(pumpIndex); // Call secondary Pump function for pumpIndex number  
    }

}
return 0;
}


void primaryPump(){
    // Cascade of conditional statements to determine whether primary pump should be on or off
    if(tankLevel >= PUMPSHIGH[PUMPORDER[0]]){
        pumpOff(PUMPORDER[0]);
        return;
    }
    else if(tankLevel <= PUMPSLOW[PUMPORDER[0]]){
        pumpOn(PUMPORDER[0]);
        return;
    }
    // If electric demand is low, start filling the tank
    else if(electricDemand < ELECTHRESHOLD){
        pumpOn(PUMPORDER[0]);
        return;
    }
    else {// Turn pump off if no conditionals are true
    pumpOff(PUMPORDER[0]);
    return;
    }
}


void secondaryPump(int pumpIndex){
    // Cascade of conditional statements to determine whether primary pump should be on or off
    if(tankLevel >= PUMPSHIGH[PUMPORDER[pumpIndex]]){
        pumpOff(PUMPORDER[pumpIndex]);
        return;
    }
    else if(tankLevel <= PUMPSLOW[PUMPORDER[pumpIndex]]){
        pumpOn(PUMPORDER[pumpIndex]);
        return;
    }
    else if (pumpStatus[pumpIndex]){
        if (pumpIndex < 3){
            pumpIndex++;
        }
    }
    else{ 
        pumpIndex--; // If pump does not turn off or on
    }
}

bool pumpOn(int pumpNumber){
    pumpStatus[pumpNumber] = 1;
    return pumpStatus[pumpNumber];
}

bool pumpOff(int pumpNumber){
    pumpStatus[pumpNumber] = 0;
    pumpIndex--;
    return pumpStatus[pumpNumber];    
}
