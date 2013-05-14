//     __  ___
//    /  |/  /__  _________ _____  __  ______ ___
//   / /|_/ / _ \/ ___/ __ `/ __ \/ / / / __ `__ \
//  / /  / /  __/ /__/ /_/ / / / / /_/ / / / / / /
// /_/  /_/\___/\___/\__,_/_/ /_/\__,_/_/ /_/ /_/
//

// You need to copy this libraries to your arduino library path
#include <CAN.h>
#include <SPI.h>

#define BUS_SPEED 1000  // 1Mbaud

void setup()
{
    Serial.begin(115200);

    // initialize CAN bus class
    // this class initializes SPI communications with MCP2515
    CAN.begin();
    CAN.baudConfig(BUS_SPEED);
    CAN.setMode(NORMAL);
    // CAN.setMode(LOOPBACK);  // set to "NORMAL" for standard com

    // Wait a second so that I can still upload even
    // if the previous iteration spams the serial port
    delay(1000);

    startNode(0x01); // @todo: variable auch nutzen!
}

void loop()
{
    receive();
    delay(100);
}

void startNode(byte nodeNr)
{
    // Nanocan Referenzhandbuch S.14
    byte startnodeCommand[2] = {0x01, 0x01};
    CAN.load_ff_0(2, 0x00, startnodeCommand);
    CAN.send_0();
}

void sendTest()
{
    static byte val = 0;
    val++;

    byte length;
    unsigned short frame_id;
    byte frame_data[8];

    frame_data[0] = val;
    frame_data[1] = 0xAA;
    frame_data[2] = 0x00;
    frame_data[3] = 0xFF;
    frame_data[4] = 0x1A;
    frame_data[5] = 0xCF;
    frame_data[6] = 0xFC;
    frame_data[7] = 0x1D;

    frame_id = 0x1d4;
    length = 8;

    CAN.load_ff_0(length, frame_id, frame_data);
    CAN.load_ff_1(1, 0x1d5, frame_data);
    CAN.send_0();
    CAN.send_1();
}

void receive()
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
