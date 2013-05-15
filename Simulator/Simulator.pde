import procontroll.*;
import java.io.*;
import processing.serial.*;

MecanumVehicle vehicle;


Serial serial;

ControllIO controll;
ControllDevice device;
ControllStick translation;
ControllSlider rotation;

void setup()
{
    size(500, 500);
    smooth();

    for (int i = 0; i < Serial.list().length; i++)
    {
        println(Serial.list()[i]);
    }
    serial = new Serial(this, "/dev/tty.usbmodemfa131", 115200);


    vehicle = new MecanumVehicle(width / 2, height / 2);

    controll = ControllIO.getInstance(this);
    device = controll.getDevice("Saitek Cyborg USB Stick");

    ControllSlider x = device.getSlider("x");
    ControllSlider y = device.getSlider("y");
    y.setMultiplier(-50.0);
    x.setMultiplier(50.0);
    translation = new ControllStick(x, y);
    translation.setTolerance(0.05);

    rotation = device.getSlider("rz");
    rotation.setMultiplier(0.5);
    rotation.setTolerance(0.05);
}

void draw()
{
    background(200);

    vehicle.translation(translation.getX(), translation.getY());
    vehicle.rotate(rotation.getValue());
    vehicle.update();
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
 * This function forwards reveived serial input to
 * the currently active view.
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
        catch (Exception ex)
        {
            println(ex.getMessage());
        }
    }
}

