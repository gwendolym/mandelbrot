/*
 * A simple program that generates the mandelbrot set
 * (c) Hannah Veit 2023
 *
*/


#include <stdio.h>
#include <stdlib.h>

struct Complex {
    double a, b;
};

void printComp (struct Complex x) {
    if (x.b > 0) printf("%lf + %lfi", x.a, x.b);
    else printf("%lf - %lfi", x.a, -x.b);
}

struct Complex compAdd (struct Complex x, struct Complex y) {
    struct Complex ret;
    ret.a = x.a + y.a;
    ret.b = x.b + y.b;

    return ret;
}

struct Complex compMult (struct Complex x, struct Complex y) {
    struct Complex ret;
    ret.a = x.a * y.a - x.b * y.b;
    ret.b = x.a * y.b + y.a * x.b;

    return ret;
}


int checkBound (struct Complex c, const int depth) {

    int iteration = 0;
    struct Complex res = {0,0};
    while (res.a *res.a + res.b * res.b  <= 4 && iteration < depth) {
        res = compAdd(compMult(res, res),c);

        iteration++;
    }
    return iteration;
}



int main () {

    

    double midA = -0.75;
    double midB = 0;
    double size = 1.25;
    


    double minA = midA - size;
    double maxA = midA + size;
    double minB = midB - size;
    double maxB = midB + size;

    double pixA = 800;
    double pixB = pixA;

    const int MAX_DEPTH = 120 / 4 * 4;


    // double incJ = (maxB-minB)/pixB;
    // double incI = (maxA-minA)/pixA;
    // int countj = 0;
    // int counti = 0;

    int range = MAX_DEPTH;
    int fourth = range / 4;

    int maxSection[] = {fourth, fourth * 2, fourth * 3, MAX_DEPTH};
    int minSection[] = {0, fourth + 1, fourth * 2 + 1, fourth * 3 + 1};
    int colourPalette[120][3];
    double r = 0, g = 0, b = 0;

    for (int value = 0; value <= MAX_DEPTH; value++){
        
        if (value < maxSection[0] &&  value > minSection[0]) {
            b = (double) 255 / (maxSection[0] - minSection[0]-1) * (value);
        }
        else if (value == maxSection[0])
            b = 255;

        else if (value < maxSection[1] &&  value > minSection[1]) {
            b = 255 - 255 / (double) (maxSection[1] - minSection[1]-1) * (value % fourth -1 );
            g = 255 / (double) (maxSection[1] - minSection[1]) * (value % fourth -1);  
        }
        else if (value == maxSection[1]){
            b = 0;
            g = 255;
        }
        else if (value < maxSection[2] &&  value > minSection[2]) {
            r = 255 / (double) (maxSection[2] - minSection[2]-1) * (value % fourth -1);  
        }
        else if (value == maxSection[2]){
            r = 255;
        }
        else if (value < maxSection[3] &&  value > minSection[3]) {
            g = 255 - 255 / (double) (maxSection[3] - minSection[3]-1) * (value % fourth -1);  
        }
        else if (value == maxSection[3]){
            g = 0;
        }


        colourPalette[value][0] = (int) r;
        colourPalette[value][1] = (int) g;
        colourPalette[value][2] = (int) b;
}
      
    
    int i, j;
    FILE *fp = fopen("mandelbrot.ppm", "wb"); /* b - binary mode */
    (void) fprintf(fp, "P6\n%d %d\n255\n", (int) pixA, (int) pixB);
    
    
    for (j = 0; j < pixB; ++j)
    {
        for (i = 0; i < pixA; ++i)
        {
            double pointA = minA + (maxA-minA)/pixA * (i-1);
            double pointB = minB + (maxB-minB)/pixB * (j-1);
            struct Complex c = {pointA, pointB};
            static unsigned char color[3] = {0,0,0};
            if (pointA >- 2 && pointA < 1.2 && pointB > -1.2 && pointB < 1.2)
            
            {
                int it = checkBound(c, MAX_DEPTH);
                it = MAX_DEPTH - it;
                color[0] = colourPalette[it][0];  /* red */
                color[1] = colourPalette[it][1];  /* green */
                color[2] = colourPalette[it][2];  /* blue */
            }
            




            (void) fwrite(color, 1, 3, fp);
        }
    }
    (void) fclose(fp);
    return EXIT_SUCCESS;

    
    
}
