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
    serialCommand.addCommand("v1", api_v1);
    serialCommand.addCommand("v2", api_v2);
    serialCommand.addCommand("v3", api_v3);
    serialCommand.addCommand("v4", api_v4);
    serialCommand.addCommand("stop", api_stop);
    serialCommand.addCommand("start", api_start);
    serialCommand.setDefaultHandler(api_unknown);
}


// ============================================================================
// custom functions:

void api_v()
{
    int v[4] = {0, 0, 0, 0};
    for (int i = 0; i < 4; i++)
    {
        char *value = serialCommand.next();
        if (value == NULL)
        {
            Serial.println("!Not enough arguments");
            return;
        }
        v[i] = atoi(value);
    }

    for (int i = 0; i < 4; i++)
    {
        robot_setMotorSpeed(i, v[i]);
    }
}

void api_v1()
{
    char *arg1 = serialCommand.next();

    // No arguments -> show usage
    if (arg1 == NULL)
    {
        Serial.println("#v1 [Steps / second]");
        return;
    }

    // there are arguments (number of steps)
    if (arg1 != NULL)
    {
        // Converts a char string to a long
        int v = atoi(arg1);
        robot_setMotorSpeed(1, v);
    }
}

void api_v2()
{
    char *arg1 = serialCommand.next();

    // No arguments -> show usage
    if (arg1 == NULL)
    {
        Serial.println("#v2 [Steps / second]");
        return;
    }

    // there are arguments (number of steps)
    if (arg1 != NULL)
    {
        // Converts a char string to a long
        int v = atoi(arg1);
        robot_setMotorSpeed(2, v);
    }
}

void api_v3()
{
    char *arg1 = serialCommand.next();

    // No arguments -> show usage
    if (arg1 == NULL)
    {
        Serial.println("#v3 [Steps / second]");
        return;
    }

    // there are arguments (number of steps)
    if (arg1 != NULL)
    {
        // Converts a char string to a long
        int v = atoi(arg1);
        robot_setMotorSpeed(3, v);
    }
}

void api_v4()
{
    char *arg1 = serialCommand.next();

    // No arguments -> show usage
    if (arg1 == NULL)
    {
        Serial.println("#v4 [Steps / second]");
        return;
    }

    // there are arguments (number of steps)
    if (arg1 != NULL)
    {
        // Converts a char string to a long
        int v = atoi(arg1);
        robot_setMotorSpeed(4, v);
    }
}

void api_stop()
{
    for (int i = 1; i < 5; i++)
    {
        robot_quickStop(i);
    }
}

void api_start()
{
    for (int i = 1; i < 5; i++)
    {
        robot_initNode(i);
    }
}
