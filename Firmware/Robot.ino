//
// Robot.ino
//
//
//   Wheel 3               Wheel 0
//     +--------------------+
//     |                    |
//     |               Arm  | -->
//     |                    |
//     +--------------------+
//   Wheel 2               Wheel 1
//

#include <CAN.h>
#include <SPI.h>

#define DEBUGMODE 0  // Set to 1 to enable debugging output
#define CAN_BUS_SPEED 1000  // 1Mbaud

/**
 * The robot initialization routine.
 * This function will only start the basic communication, to start the wheels
 * call robot_startMotors() afterwards.
 */
void robot_begin()
{
    robot_initCAN(CAN_BUS_SPEED);
}

/**
 * Sets the motor modes to velocity mode and starts the power supply.
 * After that, the motors are ready to drive. This will take about 900 ms.
 */
void robot_startMotors()
{
    for (int i = 0; i < 4; i++)
    {
        robot_startMotor(i);
    }
}

/**
 * Convenience function for setting the speed of all four motors.
 * Please notice that this will take about 150ms.
 *
 * @example: Perform a slow, counterclockwise rotation:
 * >>    int16_t speed[4] = {-1000, 1000, -1000, 1000};
 * >>    robot_setMotorSpeed(speed);
 *
 * @param speed: An array of integers between -25000 and 25000 [steps/second]
 */
void robot_setMotorSpeed(int16_t speed[4])
{
    for (int i = 0; i < 4; i++)
    {
        robot_setSingleMotorSpeed(i, speed[i]);
    }
}

/**
 * Set the speed of a single wheel
 * @param wheel: which wheel to select (0, 1, 2 or 3)
 * @param speed: the desired speed in steps/second. Range: -25000 to 25000
 */
void robot_setSingleMotorSpeed(uint8_t wheel, int16_t speed)
{
    #if DEBUGMODE
    Serial.print("# Motor speed: ");
    Serial.println(speed);
    #endif

    int8_t physicalDirection[4] = {-1, 1, 1, -1};
    speed *= physicalDirection[wheel];

    byte lowNibble = speed;
    byte highNibble = speed >> 8;
    byte cmd[8] = {0x2B, 0x42, 0x60, 0x00, lowNibble, highNibble, 0x00, 0x00};
    robot_command(wheel, cmd);
}

/**
 * Performs a quick stop.
 * This halts the robot as fast as possible and shuts the motors down.
 * To start driving again you have to call robot_startMotors() first.
 */
void robot_quickStop()
{
    for (int i = 0; i < 4; i++)
    {
        robot_quickStopWheel(i);
    }
}

// ----------------------------------------------------------------------------
// Internal functions

/**
 * Initializes the communicatino via the CAN-Bus.
 * The baudrate should be 1 Mbaud per default.
 */
void robot_initCAN(uint16_t baudrate)
{
    CAN.begin();
    CAN.baudConfig(baudrate);
    CAN.setMode(NORMAL);
    delay(100);
}

void robot_startMotor(uint8_t wheel)
{
    robot_velocityMode(wheel);
    robot_motorPowerOn(wheel);
    robot_motorStart(wheel);
}

void robot_velocityMode(uint8_t wheel)
{
    #if DEBUGMODE
    Serial.println("# Motor velocity mode");
    #endif

    byte cmd[2][8] = {
        {0x2B, 0x40, 0x60, 0x00, 0x0F, 0x00, 0x00, 0x00},  // CONTROLWORD RES:0 RES:0 RES:0 RES:0 RES:0 RES:0 MS5:0 HAT:0 FTR:0 MS3:0 MS2:0 MS1:0 ENO:1 QUS:1 ENV:1 SWO:1
        {0x2F, 0x60, 0x60, 0x00, 0x02, 0x00, 0x00, 0x00}   // MODE OF OPERATION
    };
    for (int i = 0; i < 3; i++)
    {
        robot_command(wheel, cmd[i]);
    }
}

void robot_motorPowerOn(uint8_t wheel)
{
    #if DEBUGMODE
    Serial.println("# Motor power on");
    #endif

    byte cmd[3][8] = {
        {0x2B, 0x40, 0x60, 0x00, 0x06, 0x00, 0x00, 0x00},  // CONTROLWORD RES:0 RES:0 RES:0 RES:0 RES:0 RES:0 MS5:0 HAT:0 FTR:0 MS3:0 MS2:0 MS1:0 ENO:0 QUS:1 ENV:1 SWO:0
        {0x2B, 0x40, 0x60, 0x00, 0x07, 0x00, 0x00, 0x00},  // CONTROLWORD RES:0 RES:0 RES:0 RES:0 RES:0 RES:0 MS5:0 HAT:0 FTR:0 MS3:0 MS2:0 MS1:0 ENO:0 QUS:1 ENV:1 SWO:1
        {0x2B, 0x40, 0x60, 0x00, 0x0F, 0x00, 0x00, 0x00},  // CONTROLWORD RES:0 RES:0 RES:0 RES:0 RES:0 RES:0 MS5:0 HAT:0 FTR:0 MS3:0 MS2:0 MS1:0 ENO:1 QUS:1 ENV:1 SWO:1
    };
    for (int i = 0; i < 3; i++)
    {
        robot_command(wheel, cmd[i]);
    }
}

void robot_motorStart(uint8_t wheel)
{
    #if DEBUGMODE
    Serial.println("# Motor start");
    #endif

    byte cmd[11][8] = {
        {0x23, 0x46, 0x60, 0x01, 0x00, 0x00, 0x00, 0x00},  // VL VELOCITY MIN AMOUNT
        {0x23, 0x46, 0x60, 0x02, 0xA8, 0x61, 0x00, 0x00},  // VL VELOCITY MAX AMOUNT
        {0x2B, 0x40, 0x60, 0x00, 0x0F, 0x01, 0x00, 0x00},  // CONTROLWORD RES:0 RES:0 RES:0 RES:0 RES:0 RES:0 MS5:0 HAT:1 FTR:0 MS3:0 MS2:0 MS1:0 ENO:1 QUS:1 ENV:1 SWO:1
        {0x2B, 0x42, 0x60, 0x00, 0x00, 0x00, 0x00, 0x00},  // VL TARGET VELOCITY
        {0x23, 0x48, 0x60, 0x01, 0x20, 0x4E, 0x00, 0x00},  // VL ACCEL DELTA SPEED
        {0x2B, 0x48, 0x60, 0x02, 0x01, 0x00, 0x00, 0x00},  // VL ACCEL DELTA TIME
        {0x23, 0x49, 0x60, 0x01, 0x20, 0x4E, 0x00, 0x00},  // VL DECEL DELTA SPEED
        {0x2B, 0x49, 0x60, 0x02, 0x01, 0x00, 0x00, 0x00},  // VL DECEL DELTA TIME
        {0x23, 0x4A, 0x60, 0x01, 0xA0, 0x86, 0x01, 0x00},  // VL QUICK STOP DELTA SPEED
        {0x2B, 0x4A, 0x60, 0x02, 0x01, 0x00, 0x00, 0x00},  // VL QUICK STOP DELTA TIME
        {0x2B, 0x40, 0x60, 0x00, 0x0F, 0x00, 0x00, 0x00}   // CONTROLWORD RES:0 RES:0 RES:0 RES:0 RES:0 RES:0 MS5:0 HAT:0 FTR:0 MS3:0 MS2:0 MS1:0 ENO:1 QUS:1 ENV:1 SWO:1
    };
    for (int i = 0; i < 11; i++)
    {
        robot_command(wheel, cmd[i]);
    }
}

void robot_quickStopWheel(uint8_t wheel)
{
    #if DEBUGMODE
    Serial.println("# Motor quickstop");
    #endif

    byte cmd[3][8] = {
        {0x2B, 0x40, 0x60, 0x00, 0x0F, 0x01, 0x00, 0x00},  // CONTROLWORD RES:0 RES:0 RES:0 RES:0 RES:0 RES:0 MS5:0 HAT:1 FTR:0 MS3:0 MS2:0 MS1:0 ENO:1 QUS:1 ENV:1 SWO:1
        {0x2B, 0x40, 0x60, 0x00, 0x0B, 0x01, 0x00, 0x00},  // CONTROLWORD RES:0 RES:0 RES:0 RES:0 RES:0 RES:0 MS5:0 HAT:1 FTR:0 MS3:0 MS2:0 MS1:0 ENO:1 QUS:0 ENV:1 SWO:1
        {0x2B, 0x40, 0x60, 0x00, 0x0F, 0x01, 0x00, 0x00}   // CONTROLWORD RES:0 RES:0 RES:0 RES:0 RES:0 RES:0 MS5:0 HAT:1 FTR:0 MS3:0 MS2:0 MS1:0 ENO:1 QUS:1 ENV:1 SWO:1
    };
    for (int i = 0; i < 3; i++)
    {
        robot_command(wheel, cmd[i]);
    }
}

byte robot_nodeAdress(uint8_t wheel)
{
    byte nodeAdress[4] = {0x01, 0x02, 0x03, 0x04};
    return nodeAdress[wheel];
}

void robot_command(uint8_t wheel, byte data[8])
{
    uint16_t COB = 0x600;
    uint16_t frameId = COB + robot_nodeAdress(wheel);
    CAN.load_ff_0(8, frameId, data);
    CAN.send_0();
    delay(10);

    #if DEBUGMODE
    for (int i = 0; i < 8; i++)
    {
        Serial.print(data[i], HEX);
        Serial.print(" ");
    }
    Serial.println();
    #endif
}
