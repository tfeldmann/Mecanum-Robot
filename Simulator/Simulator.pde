MecanumVehicle vehicle;

void setup()
{
    size(500, 500);
    smooth();
    vehicle = new MecanumVehicle(width / 2, height / 2);
}

void draw()
{
    background(200);
    vehicle.update();
    vehicle.translationTo(mouseX, height - mouseY);
    vehicle.rotate(-1.0);
}
