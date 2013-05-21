//
// Simulator.pde
//

import procontroll.*;
import java.io.*;
import processing.serial.*;

MecanumVehicle vehicle;

ControllIO controll;
ControllDevice device;
ControllStick translation;
ControllSlider rotation;
ControllSlider throttle;
ControllButton stopButton;

Serial serial;

void setup()
{
    size(500, 500);
    smooth();
    frameRate(20);

    // usually the device appears as the last serial port.
    // This is not nice at all and might not work in certain circumstances,
    // feel free to add a select box!
    serial = new Serial(this, Serial.list()[Serial.list().length-1], 115200);

    vehicle = new MecanumVehicle(width / 2, height / 2);
    setupJoystick();
}

void setupJoystick()
{
    controll = ControllIO.getInstance(this);
    device = controll.getDevice("Saitek Cyborg USB Stick");

    ControllSlider x = device.getSlider("x");
    ControllSlider y = device.getSlider("y");
    y.setMultiplier(-1.0);
    translation = new ControllStick(x, y);
    translation.setTolerance(0.05);

    rotation = device.getSlider("rz");
    rotation.setTolerance(0.05);

    throttle = device.getSlider("z");
    throttle.setTolerance(0.05);

    device.plug(this, "startEngine", ControllIO.ON_PRESS, 0);
    device.plug(this, "stopEngine", ControllIO.ON_RELEASE, 0);
}

void draw()
{
    background(200);

    vehicle.move(
        translation.getX(),
        translation.getY(),
        rotation.getValue());
    vehicle.draw();
    vehicle.sendToDevice(1 - (throttle.getValue() + 1.0) * 0.5);
}

void startEngine()
{
    serialSend("@start");
}

void stopEngine()
{
    serialSend("@stop");
    vehicle.move(0, 0, 0);
}

/**
 * Use this function to send a command to the device
 *
 * @param String cmd: The string
 */
void serialSend(String cmd)
{
    if (serial == null) return;
    serial.write(cmd);
    serial.write("\n");
    println("Sent: " + cmd);
}

/**
 * This function prints received serial input
 */
void serialEvent(Serial serial)
{
    String msg = serial.readStringUntil('\n');
    if (msg != null)
    {
        try
        {
            msg = trim(msg);
            println(msg);
        }
        catch (RuntimeException ex)
        {
            println(ex.getMessage());
        }
    }
}

