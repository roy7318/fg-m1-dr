#pragma once
#include <string>
#include <map>
#include <vector>
#include <iostream>
#include "GlobalBindMap.h"
using namespace std;

// Class parser the buffer from the server and creates a map of binding locations and values.
class BufferParser {
public:
    // Splits the expression into both sides, based on the appropriate sign of the expression.
    void updatBindMap(string buffer) {
        // Split the string.
        vector<string> values = splitExpression(buffer);
        // Create a map iterator.
        map<string, double>::iterator it;
        // All the values are in a constant order, we will apply the correct value by index in the splitted vector.
        for (int i = 0; i < values.size(); i++) {
            if (i == 0) {
                // Find the correct place in the map based on command order from the xml file.
                it = pathMap.find("/instrumentation/airspeed-indicator/indicated-speed-kt"); //////////// 0
                // Change the value to a double by converting the string in the correct vector to a double.
                it->second = stod(values[i]);
            }
            // Rinse and repeat for all lines.
            if (i == 1) {
                it = pathMap.find("/instrumentation/altimeter/indicated-altitude-ft"); ///////////// 1
                it->second = stod(values[i]);
            }
            if (i == 2) {
                it = pathMap.find("/instrumentation/altimeter/pressure-alt-ft"); ////////////////// 2
                it->second = stod(values[i]);
            }
            if (i == 3) {
                it = pathMap.find("/instrumentation/attitude-indicator/indicated-pitch-deg"); //////////////// 3
                it->second = stod(values[i]);
            }
            if (i == 4) {
                it = pathMap.find("/instrumentation/attitude-indicator/indicated-roll-deg"); /////////////////// 4
                it->second = stod(values[i]);
            }
            if (i == 5) {
                it = pathMap.find("/instrumentation/attitude-indicator/internal-pitch-deg"); //////////////////// 5
                it->second = stod(values[i]);
            }
            if (i == 6) {
                it = pathMap.find("/instrumentation/attitude-indicator/internal-roll-deg"); //////////////// 6
                it->second = stod(values[i]);
            }
            if (i == 7) {
                it = pathMap.find("/instrumentation/encoder/indicated-altitude-ft"); /////////// 7
                it->second = stod(values[i]);
            }
            if (i == 8) {
                it = pathMap.find("/instrumentation/encoder/pressure-alt-ft"); //////// 8
                it->second = stod(values[i]);
            }
            if (i == 9) {
                it = pathMap.find("/instrumentation/gps/indicated-altitude-ft"); /////// 9
                it->second = stod(values[i]);
            }
            if (i == 10) {
                it = pathMap.find("/instrumentation/gps/indicated-ground-speed-kt"); //// 10
                it->second = stod(values[i]);
            }
            if (i == 11) {
                it = pathMap.find("/instrumentation/gps/indicated-vertical-speed"); ///// 11
                it->second = stod(values[i]);
            }
            if (i == 12) {
                it = pathMap.find("/instrumentation/heading-indicator/indicated-heading-deg"); ///// 12
                it->second = stod(values[i]);
            }
            if (i == 13) {
                it = pathMap.find("/instrumentation/magnetic-compass/indicated-heading-deg"); /////// 13
                it->second = stod(values[i]);
            }
            if (i == 14) {
                it = pathMap.find("/instrumentation/slip-skid-ball/indicated-slip-skid"); ///// 14
                it->second = stod(values[i]);
            }
            if (i == 15) {
                it = pathMap.find("/instrumentation/turn-indicator/indicated-turn-rate"); ///// 15
                it->second = stod(values[i]);
            }
            if (i == 16) {
                it = pathMap.find("/instrumentation/vertical-speed-indicator/indicated-speed-fpm"); /// 16
                it->second = stod(values[i]);
            }
            if (i == 17) {
                it = pathMap.find("/controls/flight/aileron"); //// 17
                it->second = stod(values[i]);
            }
            if (i == 18) {
                it = pathMap.find("/controls/flight/elevator"); ///// 18
                it->second = stod(values[i]);
            }
            if (i == 19) {
                it = pathMap.find("/controls/flight/rudder"); ///// 19
                it->second = stod(values[i]);
            }
            if (i == 20) {
                it = pathMap.find("/controls/flight/flaps"); /// 20
                it->second = stod(values[i]);
            }
            if (i == 21) {
                it = pathMap.find("/controls/engines/engine/throttle"); // 21
                it->second = stod(values[i]);
            }
            if (i == 22) {
                it = pathMap.find("/engines/engine/rpm"); /// 22
                it->second = stod(values[i]);
            }
        }
    }

    // Split the buffer by the ',' char to get the values.
    vector<string> splitExpression(string str) {
        // If we start with a ',' get rid of it.
        if (str[0] == ',') {
            str.erase(0,1);
        }
        std::vector<std::string> splittedString;
        int startIndex = 0;
        int endIndex = 0;
        while ((endIndex = str.find(',', startIndex)) < str.size()) {
            std::string val = str.substr(startIndex, endIndex - startIndex);
            splittedString.push_back(val);
            startIndex = endIndex + 1;
        }
        if (startIndex < str.size()) {
            std::string val = str.substr(startIndex);
            splittedString.push_back(val);
        }
        return splittedString;
    }
};