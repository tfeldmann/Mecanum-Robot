MecanumVehicle vehicle;

float w = 0;

void setup()
{
    size(500, 500);
    smooth();
    vehicle = new MecanumVehicle(width / 2, height / 2);
}

void draw()
{
    background(200);
    textAlign(CENTER);
    text("Maus: Translation, Pfeil hoch / runter: Rotation", width / 2, 20);
    vehicle.update();
    vehicle.translationTo(mouseX, height - mouseY);
    vehicle.rotate(w);
}

void keyPressed()
{
    if (keyCode == UP)
    {
        w += 0.1;
    }
    else if (keyCode == DOWN)
    {
        w -= 0.1;
    }
}
