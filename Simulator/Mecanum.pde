class MecanumVehicle
{
    int x, y;
    int w = 49;
    int h = 77;
    float wheel[] = {0, 0, 0, 0};  // wheel speeds

    MecanumVehicle(int _x, int _y)
    {
        x = _x - w / 2;
        y = _y - h / 2;
    }

    void draw()
    {
        pushMatrix();
        translate(x, y);

        // draw body
        stroke(0);
        fill(255);
        strokeWeight(1);
        rect(0, 0, w, h);

        // draw wheels
        strokeWeight(2);
        stroke(0, 100, 0);
        line(w, 0, w - wheel[0], - wheel[0]);
        line(0, 0, wheel[1], - wheel[1]);
        line(0, h, - wheel[2], h - wheel[2]);
        line(w, h, w + wheel[3], h - wheel[3]);

        // draw resulting forces
        strokeWeight(3);
        stroke(0, 200, 0);
        float fx = 0.7 * (-wheel[0] + wheel[1] - wheel[2] + wheel[3]);
        float fy = 0.7 * (wheel[0] + wheel[1] + wheel[2] + wheel[3]);
        line(w / 2, h / 2, w / 2 + round(fx), h / 2 - round(fy));

        // text forces
        fill(0);
        text(wheel[0], w, 0);
        text(wheel[1], 0, 0);
        text(wheel[2], 0, h);
        text(wheel[3], w, h);

        popMatrix();
    }

    void move(float vx, float vy, float omega)
    {
        float s1 = (vy + vx);
        float s2 = (vy - vx);

        wheel[0] = s1 + omega;
        wheel[1] = s2 - omega;
        wheel[2] = s1 - omega;
        wheel[3] = s2 + omega;

        // set absolute max speed to 1.0
        float fastest = 0;
        for (int i = 0; i < 4; i++)
        {
            float value = abs(wheel[i]);
            if (value > fastest)
            {
                fastest = value;
            }
        }

        if (fastest > 1.0)
        {
            for (int i = 0; i < 4; i++)
            {
                wheel[i] /= fastest;
            }
        }
    }

    void sendToDevice(float throttle)
    {
        float fullspeed = 25000 * throttle;
        String wheelstring = "";
        for (int i = 0; i < 4; i++)
        {
            wheelstring += " " + floor(wheel[i] * fullspeed);
        }

        serialSend("@v" + wheelstring);
    }
};
