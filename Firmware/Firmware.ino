//     __  ___
//    /  |/  /__  _________ _____  __  ______ ___
//   / /|_/ / _ \/ ___/ __ `/ __ \/ / / / __ `__ \
//  / /  / /  __/ /__/ /_/ / / / / /_/ / / / / / /
// /_/  /_/\___/\___/\__,_/_/ /_/\__,_/_/ /_/ /_/
//

#include <CAN.h>
#include <SPI.h>

void setup()
{
    Serial.begin(115200);
    api_init();
    robot_init();
}

void loop()
{
    api_update();
    // readCanMessages();
}

/**
 * Reads all incoming CAN-Messages and dumps them on the serial port
 */
void readCanMessages()
{
    byte length = 0;
    byte rx_status = 0;
    unsigned short frame_id = 0;
    byte frame_data[8];

    // clear frame data
    for (int i = 0; i < 8; i++)
    {
        frame_data[i] = 0;
    }

    rx_status = CAN.readStatus();
    if ((rx_status & 0x01) == 0x01)
    {
        CAN.readDATA_ff_0(&length, frame_data, &frame_id);
    }
    else if ((rx_status & 0x02) == 0x02)
    {
        CAN.readDATA_ff_1(&length, frame_data, &frame_id);
    }

    if (((rx_status & 0x01) == 0x01) || ((rx_status & 0x02) == 0x02))
    {
        Serial.print("Rx:");
        Serial.print(rx_status, HEX);
        Serial.print(" ");
        Serial.print(length, HEX);

        Serial.print(" - ");
        Serial.print(frame_id, HEX);
        Serial.print(": ");

        for (int i = 0; i < 8; i++)
        {
            Serial.print(" ");
            Serial.print(frame_data[i], HEX);
        }
        Serial.println();
    }
}
