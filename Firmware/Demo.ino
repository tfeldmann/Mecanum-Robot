//
// Demo.ino
//

CurveResult circle_translation_only(float t)
{
    // Original: speed multiplier: 5000, timeStretch 0.093
    float timeStretch = 0.093;

    CurveResult result;
    result.finished = false;
    result.omega = 0;
    result.vx = -cos(t * timeStretch);
    result.vy = sin(t * timeStretch);

    stop after a quarter circle
    if (result.vy < 0)
    {
        result.finished = true;
    }

    return result;
}

CurveResult circle_with_rotation_forward(float t)
{
    float timeStretch = 0.1;

    CurveResult result;
    result.finished = false;
    result.omega = -0.305;
    result.vx = 0;
    result.vy = 1;

    // todo: t>160000 stop

    return result;
}

CurveResult circle_with_rotation_sideways(float t)
{
    float timeStretch = 0.1;

    CurveResult result;
    result.finished = false;
    result.omega = -0.3;
    result.vx = -1;
    result.vy = 0;

    // todo: time>16

    return result;
}


// =============================================================================
// Demo execution code

boolean active = false;
unsigned long starttime;
CurveResult (*demo)(float);

void demo_start()
{
    starttime = millis();
    active = true;
}

void demo_update()
{
    if (!active || demo == NULL) return;

    float t = (millis() - starttime) / 1000.0;
    CurveResult result = (*demo)(t);

    // stop when demo is finished
    if (result.finished) demo_stop();

    float velocities[4] = {0, 0, 0, 0};
    wheelVelocities(velocities, result.vx, result.vy, result.omega);

    for (int i = 0; i < 4; i++)
    {
        Serial.print((int)(velocities[i] * 10000));
        Serial.print("\t");
        robot_setSingleMotorSpeed(i, (int)(velocities[i] * 10000));
    }
    Serial.println();
}

void demo_stop()
{
    active = false;
    demo = NULL;
    robot_quickStop();
}

void demo_1()
{
    demo = &circle_translation_only;
    demo_start();
}

void demo_2()
{
    demo = &circle_with_rotation_forward;
    demo_start();
}

void demo_3()
{
    demo = &circle_with_rotation_sideways;
    demo_start();
}

void wheelVelocities(float *velocities, float vx, float vy, float omega)
{
    float s1 = vy - vx;
    float s2 = vy + vx;

    velocities[0] = s1 + omega;
    velocities[1] = s2 - omega;
    velocities[2] = s1 - omega;
    velocities[3] = s2 + omega;
}
