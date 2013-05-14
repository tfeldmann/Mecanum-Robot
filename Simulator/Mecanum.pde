class MecanumVehicle
{
    int x, y;
    int w = 49;
    int h = 77;
    float r;

    int speed[] = {0, 0, 0, 0};  // wheel speeds

    MecanumVehicle(int _x, int _y)
    {
        x = _x - w / 2;
        y = _y - h / 2;

        r = sqrt(w*w/4.0 + h*h/4.0);
        println("r: "+r);
    }

    void update()
    {
        pushMatrix();
        translate(x, y);

        // draw body
        stroke(0);
        strokeWeight(1);
        rect(0, 0, w, h);

        // draw wheels
        strokeWeight(2);
        stroke(0, 100, 0);
        line(w, 0, w - speed[0], - speed[0]);
        line(0, 0, speed[1], - speed[1]);
        line(0, h, - speed[2], h - speed[2]);
        line(w, h, w + speed[3], h - speed[3]);

        // draw resulting forces
        strokeWeight(3);
        stroke(0, 200, 0);
        float fx = 0.7 * (-speed[0] + speed[1] - speed[2] + speed[3]);
        float fy = 0.7 * (speed[0] + speed[1] + speed[2] + speed[3]);
        line(w / 2, h / 2,
            w / 2 + round(fx), h / 2 - round(fy));

        popMatrix();
    }

    void translationTo(int _x, int _y)
    {
        int vx = _x - (x + w / 2);
        int vy = _y - (y + h / 2);

        float w1 = (vy - vx)/(4 * 0.7);
        float w2 = (vy + vx)/(4 * 0.7);

        speed[0] = round(w1);
        speed[1] = round(w2);
        speed[2] = round(w1);
        speed[3] = round(w2);
    }

    void rotate(float w)
    {

    }
};
