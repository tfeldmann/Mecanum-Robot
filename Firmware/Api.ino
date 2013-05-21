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

void api_start()
{
    for (int i = 0; i < 4; i++)
    {
        robot_initNode(i);
    }
}

void api_stop()
{
    for (int i = 0; i < 4; i++)
    {
        robot_quickStop(i);
    }
}

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
