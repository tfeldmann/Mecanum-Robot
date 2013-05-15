import procontroll.*;
import java.io.*;

MecanumVehicle vehicle;

ControllIO controll;
ControllDevice device;
ControllStick translation;
ControllSlider rotation;

void setup()
{
    size(500, 500);
    smooth();
    vehicle = new MecanumVehicle(width / 2, height / 2);

    controll = ControllIO.getInstance(this);
    device = controll.getDevice("Saitek Cyborg USB Stick");

    ControllSlider x = device.getSlider("x");
    ControllSlider y = device.getSlider("y");
    y.setMultiplier(-50.0);
    x.setMultiplier(50);
    translation = new ControllStick(x, y);
    translation.setTolerance(0.05);

    rotation = device.getSlider("rz");
    rotation.setTolerance(0.05);
}

void draw()
{
    background(200);

    vehicle.translation(translation.getX(), translation.getY());
    vehicle.rotate(rotation.getValue());
    vehicle.update();
}
