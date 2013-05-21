//
// Api.ino
//

#include <SerialCommand.h>

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

void api_begin()
{
    serialCommand.addCommand("@start", api_start);
    serialCommand.addCommand("@stop", api_stop);
    serialCommand.addCommand("@v", api_v);
    serialCommand.setDefaultHandler(api_unknown);
}


// ============================================================================
// custom functions:

/**
 * Start the engines
 *
 * Usage: "@start"
 */
void api_start()
{
    for (int wheel = 0; wheel < 4; wheel++)
    {
        robot_initWheel(wheel);
    }
}

/**
 * Perform a quick stop
 *
 * Usage: "@stop"
 */
void api_stop()
{
    for (int wheel = 0; wheel < 4; wheel++)
    {
        robot_quickStop(wheel);
    }
}

/**
 * Set the desired velocity for each robot wheel.
 *
 * Usage: "@v [wheel 1] [wheel 2] [wheel 3] [wheel 4]"
 * Range: -25000...25000
 * Unit:  steps/second
 */
void api_v()
{
    int v[4] = {0, 0, 0, 0};
    for (int wheel = 0; wheel < 4; wheel++)
    {
        char *value = serialCommand.next();
        if (value == NULL)
        {
            Serial.println("!E01: Not enough arguments supplied.");
            return;
        }
        v[wheel] = atoi(value);
    }

    for (int wheel = 0; wheel < 4; wheel++)
    {
        robot_setMotorSpeed(wheel, v[wheel]);
    }
}
