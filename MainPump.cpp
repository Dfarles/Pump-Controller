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
#include <chrono>

using namespace std;

void primaryPump();
void secondaryPump(int);
void cycle();
bool pumpOn(int);
bool pumpOff(int);
bool checkTime();

auto previousCycle = std::chrono::high_resolution_clock::now(); //Time stamp of last time water has been cycled
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

    // If water hasn't cycled for 3 days, cycle it now
    else if(checkTime()){
        cycle();
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

void cycle(){
    auto minutes = 0;
    auto baseClock = std::chrono::high_resolution_clock::now();
    double halfHour = 1800000.0;
    while(minutes < halfHour){
        
    // If water level is too low, cycle is not necessary
        if(tankLevel < TANKLOW){
            return;
        }

    // If water level is too high, cycle is not necessary
        else if(tankLevel > TANKHIGH){
            return;
        }

    // Check if any of the pumps are on then check if it has been three days
        else if((pumpStatus[0]+pumpStatus[1]+pumpStatus[2]+pumpStatus[3]) == 1){    
            auto diff = std::chrono::high_resolution_clock::now() - baseClock;
            auto minutes = std::chrono::duration<double, std::milli>(diff);
        }
        else if (minutes >= halfHour){ //If 30 minute timer expires, turn off main pump and exit cycle sequence
            pumpOff(PUMPORDER[0]);
            return;
        }

        else{ //If conditions are met, turn on main pump and start 30 minute timer
            pumpOn(PUMPORDER[0]);
            auto baseClock = chrono::high_resolution_clock::now();
        }

    } 
}

bool pumpOn(int pumpNumber){
    pumpStatus[pumpNumber] = 1;
    
    // Update last time water was cycled
    previousCycle = std::chrono::high_resolution_clock::now();
    return pumpStatus[pumpNumber];
}

bool pumpOff(int pumpNumber){
    pumpStatus[pumpNumber] = 0;

    // Update last time water was cycled
    previousCycle = std::chrono::high_resolution_clock::now();
    pumpIndex--;
    return pumpStatus[pumpNumber];    
}

bool checkTime() //Check if water has been cycled in last three days
{
    auto diff = std::chrono::high_resolution_clock::now() - previousCycle; //Compute difference between current time stamp and last time a pump was turned on
    auto hours = std::chrono::duration_cast<chrono::hours>(diff); //Change units to hours
    
    if(hours.count() > 72.0){ //If more than three days have passed, return a true. Otherwise, return false
        return true;
    }
    else {
        return false;
    }
}