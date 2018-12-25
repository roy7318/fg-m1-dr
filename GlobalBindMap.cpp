#include "GlobalBindMap.h"

// Initialize the variable path map for the simulator.
map<string, double> pathMap = {{"/instrumentation/airspeed-indicator/indicated-speed-kt",0},
                               {"/instrumentation/altimeter/indicated-altitude-ft",0},
                               {"/instrumentation/altimeter/pressure-alt-ft",0},
                               {"/instrumentation/attitude-indicator/indicated-pitch-deg",0},
                               {"/instrumentation/attitude-indicator/indicated-roll-deg",0},
                               {"/instrumentation/attitude-indicator/internal-pitch-deg",0},
                               {"/instrumentation/attitude-indicator/internal-roll-deg",0},
                               {"/instrumentation/encoder/indicated-altitude-ft",0},
                               {"/instrumentation/encoder/pressure-alt-ft",0},
                               {"/instrumentation/gps/indicated-altitude-ft",0},
                                  {"/instrumentation/gps/indicated-ground-speed-kt",0},
                                  {"/instrumentation/gps/indicated-vertical-speed",0},
                                  {"/instrumentation/heading-indicator/indicated-heading-deg",0},
                                  {"/instrumentation/magnetic-compass/indicated-heading-deg",0},
                                  {"/instrumentation/slip-skid-ball/indicated-slip-skid",0},
                                  {"/instrumentation/turn-indicator/indicated-turn-rate",0},
                                  {"/instrumentation/vertical-speed-indicator/indicated-speed-fpm",0},
                                  {"/controls/flight/aileron",0},
                                  {"/controls/flight/elevator",0},
                                  {"/controls/flight/rudder",0},
                                  {"/controls/flight/flaps",0},
                                  {"/controls/engines/current-engine/throttle",0},
                                  {"/engines/engine/rpm", 0}};

// The global map that contains the connection between a variable name and a path.
map<string,string> vartopath = {};

// This will be used to start the client after the server has been opened.
bool timer =  false;

// This will continue the main after the client is connected.
bool mainTimer = false;