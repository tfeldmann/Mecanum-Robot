class MecanumVehicle
{
    int width = 49;
    int height = 100;
    
    int x, y;
    // Wheel   1   2   3   4
    int[] w = {30, 30, 30, 30};
    
    MecanumVehicle()
    {
        x = 200;
        y = 200;
    }
    
    void update()
    {
        pushMatrix();
        translate(x, y);
    
        // draw body
        stroke(0);
        strokeWeight(1);
        rect(0, 0, width, height);
        
        // draw wheels
        strokeWeight(2);
        stroke(0, 100, 0);
        line(width, 0, width - w[0], - w[0]);
        line(0, 0, w[1], - w[1]);
        line(0, height, - w[2], height - w[2]);
        line(width, height, width + w[3], height - w[3]);
        
        // draw resulting forces
        strokeWeight(3);
        stroke(0, 200, 0);
        float fx = 0.7 * (-w[0] + w[1] - w[2] + w[3]);
        float fy = 0.7 * (w[0] + w[1] + w[2] + w[3]);
        line(width / 2, height / 2,
            width / 2 + round(fx), height / 2 - round(fy));
    
        popMatrix();
    }
    
    void translationTo(int _x, int _y)
    {
        int vx = _x - (x + width / 2);
        int vy = _y - (y + height / 2);
        
        float w1 = (vy - vx)/(4 * 0.7);
        float w2 = (vy + vx)/(4 * 0.7);
        
        w[0] = round(w1);
        w[1] = round(w2);
        w[2] = round(w1);
        w[3] = round(w2);
        
        /*line(x + width / 2, y + height / 2,
            x + width / 2 + round(vx),
            y + height / 2 + round(vy));*/
    }
}
