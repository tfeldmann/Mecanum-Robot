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
    Serial.print("!E02: Unknown command: ");
    Serial.println(command);
}

void api_init()
{
    serialCommand.addCommand("start", api_start);
    serialCommand.addCommand("stop", api_stop);
    serialCommand.addCommand("v", api_v);
    serialCommand.setDefaultHandler(api_unknown);
}


// ============================================================================
// custom functions:

/**
 * Start the engines
 *
 * Usage: "start"
 */
void api_start()
{
    for (int i = 0; i < 4; i++)
    {
        robot_initNode(i);
    }
}

/**
 * Perform a quick stop
 *
 * Usage: "stop"
 */
void api_stop()
{
    for (int i = 0; i < 4; i++)
    {
        robot_quickStop(i);
    }
}

/**
 * Set the desired velocity for each robot wheel.
 *
 * Usage: "v [wheel 1] [wheel 2] [wheel 3] [wheel 4]"
 * Range: -25000...25000
 * Unit:  steps/second
 */
void api_v()
{
    int v[4] = {0, 0, 0, 0};
    for (int i = 0; i < 4; i++)
    {
        char *value = serialCommand.next();
        if (value == NULL)
        {
            Serial.println("!E01: Not enough arguments supplied.");
            return;
        }
        v[i] = atoi(value);
    }

    for (int i = 0; i < 4; i++)
    {
        robot_setMotorSpeed(i, v[i]);
    }
}
