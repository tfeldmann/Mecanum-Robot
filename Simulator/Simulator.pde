MecanumVehicle vehicle = new MecanumVehicle();

void setup()
{
    size(500, 500);
    smooth();
}

void draw()
{
    background(200);
    vehicle.update();
    vehicle.translationTo(mouseX, mouseY);
}
