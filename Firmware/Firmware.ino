//     __  ___
//    /  |/  /__  _________ _____  __  ______ ___
//   / /|_/ / _ \/ ___/ __ `/ __ \/ / / / __ `__ \
//  / /  / /  __/ /__/ /_/ / / / / /_/ / / / / / /
// /_/  /_/\___/\___/\__,_/_/ /_/\__,_/_/ /_/ /_/
//

// You need to copy this libraries to your arduino library path
#include <CAN.h>
#include <SPI.h>

#define BUS_SPEED 1000  // 1Mbps

void setup()
{
    Serial.begin(115200);

    // initialize CAN bus class
    // this class initializes SPI communications with MCP2515
    CAN.begin();
    CAN.baudConfig(BUS_SPEED);
    CAN.setMode(NORMAL);
    //CAN.setMode(LOOPBACK);  // set to "NORMAL" for standard com

    //Wait 10 seconds so that I can still upload even
    //if the previous iteration spams the serial port
    delay(100);
}

void loop()
{
    static byte val = 0;
    byte length,rx_status,i;
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

    CAN.load_ff_0(length,frame_id,frame_data);
    CAN.load_ff_1(1,0x1d5,frame_data);
    CAN.send_0();
    CAN.send_1();

    val++;
    delay(100);
}
