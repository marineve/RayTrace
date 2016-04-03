#include "Image.h"
#include "raytrace.h"
#include "vector"

int main(int, char**)
{
    //Create a new 512x512 image
    Image image(512, 512);

    //Initialize and run the ray tracer
    RayTrace rayTracer;
    rayTracer.RayTraceSurface(&image);

    //Display and save the image
    image.show("Assignment 1 Ray Tracer");
    //image.save("305Assignment1.png");
}
