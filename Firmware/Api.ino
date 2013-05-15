//
// Api.ino
//

#include "SerialCommand.h"

SerialCommand serialCommand;

void api_update()
{
    serialCommand.readSerial();
}

void api_unknown(const char *command)
{
    Serial.print("!Unknown command: ");
    Serial.println(command);
}

void api_init()
{
    serialCommand.addCommand("v", api_v);
    serialCommand.addCommand("stop", api_stop);
    serialCommand.addCommand("start", api_start);
    serialCommand.setDefaultHandler(api_unknown);
}


// ============================================================================
// custom functions:

void api_v()
{
    char *arg1 = serialCommand.next();

    // No arguments -> show usage
    if (arg1 == NULL)
    {
        Serial.println("# V [Steps / second]");
        return;
    }

    // there are arguments (number of steps)
    if (arg1 != NULL)
    {
        // Converts a char string to a long
        int v = atoi(arg1);
        robot_setMotorSpeed(0x01, v);
    }
}

void api_stop()
{
    robot_quickStop(0x01);
}

void api_start()
{
    robot_initNode(0x01);
}
