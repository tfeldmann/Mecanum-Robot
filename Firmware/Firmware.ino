//     __  ___
//    /  |/  /__  _________ _____  __  ______ ___
//   / /|_/ / _ \/ ___/ __ `/ __ \/ / / / __ `__ \
//  / /  / /  __/ /__/ /_/ / / / / /_/ / / / / / /
// /_/  /_/\___/\___/\__,_/_/ /_/\__,_/_/ /_/ /_/
//
// Serial Port:
//     Baudrate 115200, New Line
//
// Board:
//     Arduino Uno
//
// Libraries:
//     All needed additional libraries can be found in the "libraries" subfolder
//     of this sketch. Copy them to your Arduino library folder.
//

#include "types.h"

void setup()
{
    Serial.begin(115200);
    api_begin();
    robot_begin();
}

void loop()
{
    api_update();
    demo_update();
}
