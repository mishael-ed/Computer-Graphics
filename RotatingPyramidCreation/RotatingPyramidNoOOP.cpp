#include "RotatingPyramidNoOOP.h"
#include <iostream>
#include <vector>
#include <chrono>
#include <SDL.h>
#include <cmath>

struct Point2D { float x; float y; };
struct Point3D { float x; float y; float z; };
struct Edge { int start; int end; };

int WindowSizeX;
int WindowSizeY;
SDL_Renderer* renderer;
SDL_Window* window;
float FL = 5;
float rotation = 0;
float DeltaTime;
float scaledValue = 2;

int iterations = 0;

// Creates an array of points
std::vector<Point3D> points2{
    Point3D{ 0, 0, 1 }, Point3D{ 1, -1, -1 },
    Point3D{ 1,  1, -1 }, Point3D{ -1,  1, -1 },
    Point3D{ -1, -1, -1 }
};

// Creates an array of edges (This holds indices referencing to the points array)
std::vector<Edge> edges2{
    Edge{ 0, 1 },
    Edge{ 0, 2 },
    Edge{ 0, 3 },
    Edge{ 0, 4 },
    Edge{ 4, 1 },
    Edge{ 1, 2 },
    Edge{ 2, 3 },
    Edge{ 3, 4 },
};

Point2D projection2(Point3D point)
{
    return Point2D{ WindowSizeX / 2 + (point.x * FL) / (FL + point.z) * 100, WindowSizeY / 2 + (point.y * FL) / (FL + point.z) * 100 };
}

Point3D rotateOnY(Point3D point)
{
    Point3D rotatedPoint;
    rotatedPoint.x = cos(rotation) * point.x + sin(rotation) * point.z;
    rotatedPoint.y = point.y;
    rotatedPoint.z = -sin(rotation) * point.x + cos(rotation) * point.z;
    return rotatedPoint;
}

Point3D scaleInOut(Point3D point)
{
    // Calculate the scaling factor based on time
    float scale = 1 + 0.01 * sin(iterations * 0.01); 

    Point3D scaledPoint;
    scaledPoint.x = point.x * scale;
    scaledPoint.y = point.y * scale;
    scaledPoint.z = point.z * scale;
    return scaledPoint;
}

void render2(std::vector<Point3D> points, std::vector<Edge> edges)
{
    iterations++;
    auto time1 = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration(0);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(renderer);
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);

    rotation = rotation + 1 * DeltaTime;

    for (auto& edge : edges) {
        Point3D rotatedPointStart = rotateOnY(points[edge.start]);
        Point3D rotatedPointEnd = rotateOnY(points[edge.end]);
        Point3D scaledPointStart = scaleInOut(rotatedPointStart);
        Point3D scaledPointEnd = scaleInOut(rotatedPointEnd);
        Point2D start = projection2(scaledPointStart);
        Point2D end = projection2(scaledPointEnd);

        SDL_RenderDrawLine(renderer, start.x, start.y, end.x, end.y);
    }

    SDL_RenderPresent(renderer);
    auto time2 = std::chrono::high_resolution_clock::now();
    duration = time2 - time1;
    DeltaTime = duration.count();
    time1 = time2;
}

int main(int argc, char* argv[])
{
    iterations = 0;
    window = SDL_CreateWindow("Cube Renderer No OOP", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 960, 540, 0);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    SDL_GetWindowSize(window, &WindowSizeX, &WindowSizeY);

    bool running = true;

    while (running)
    {
        if (SDL_QuitRequested()) { running = false; break; }
        render2(points2, edges2);
    }

    return 0;
}
