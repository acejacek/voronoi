#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>
#include <assert.h>
#include <math.h>

#define WIDTH 800
#define HEIGHT 600
#define RADIUS 4

typedef struct {
    int x;
    int y;
} Point;

Point* points;

typedef struct {
    uint8_t r;
    uint8_t g;
    uint8_t b;
} Pixel;

static Pixel screen[HEIGHT][WIDTH] = {0};

#define COLORS 16
enum color_name {
 	Black,
  	White,
  	Red,
  	Lime,
  	Blue ,
  	Yellow,
  	Cyan, 
  	Magenta,
  	Silver,
  	Gray,
  	Maroon,
  	Olive,	
  	Green,
  	Purple,
  	Teal,
  	Navy,
};

static Pixel colors[COLORS];

void defineColors()
{
    colors[0] = (Pixel){0,0,0};
    colors[1] = (Pixel){255,255,255};
    colors[2] = (Pixel){255,0,0};
    colors[3] = (Pixel){0,255,0};
    colors[4] = (Pixel){0,0,255};
    colors[5] = (Pixel){255,255,0};
    colors[6] = (Pixel){0,255,255};
    colors[7] = (Pixel){255,0,255};
    colors[8] = (Pixel){192,192,192};
    colors[9] = (Pixel){128,128,128};
    colors[10] = (Pixel){128,0,0};
    colors[11] = (Pixel){128,128,0};
    colors[12] = (Pixel){0,128,0};
    colors[13] = (Pixel){128,0,128};
    colors[14] = (Pixel){0,128,128};
    colors[15] = (Pixel){0,0,128};
}

void setRandomPoints(const int pointsCount)
{
    srand(time(NULL));

    points = (Point*) malloc(pointsCount * sizeof(Point));
    assert(points);

    for (int i = 0; i < pointsCount; ++i) {
        points[i].x = rand() % WIDTH;
        points[i].y = rand() % HEIGHT;
    }
}

void drawPoints(const int pointsCount)
{
    for (int i = 0; i < pointsCount; ++i) {
        int xmin = points[i].x - RADIUS;
        int xmax = points[i].x + RADIUS;
        int ymin = points[i].y - RADIUS;
        int ymax = points[i].y + RADIUS;

        for (int y = ymin; y <= ymax; ++y) {
            for (int x = xmin; x <= xmax; ++x) {
                int dx = points[i].x - x;
                int dy = points[i].y - y;
                if (RADIUS * RADIUS >= dx * dx + dy * dy) {
                    if (x >= 0 && x < WIDTH && y >= 0 && y < HEIGHT)
                        screen[y][x] = colors[Black];
                }
            }
        }
    }
}

float minkowskiDistance(float p, Point n, int x, int y)
{
    int dx = abs(x - n.x);
    int dy = abs(y - n.y);

    return powf(dx, p) + powf(dy, p);
}

void renderGraph(const int pointsCount, float p)
{
    for (int y = 0; y < HEIGHT; ++y) {
        for (int x = 0; x < WIDTH; ++x) {
            int closest = 0;
            float min_distance = 3.402823466e+38f;

            for (int i = 0; i < pointsCount; ++i) {
                float distance = minkowskiDistance(p, points[i], x, y);
                if (min_distance > distance) {
                    min_distance = distance;
                    closest = i;
                }
            }
            screen[y][x] = colors[1 + (closest % 15)]; // skip black
        }
    }
}

void saveToFile(const char* filename) 
{
    FILE* f = fopen(filename, "wb");
    if (!f) {
        printf("ERROR: Can't create file %s\n", filename);
        exit(1);
    }
    fprintf(f, "P6\n%d %d\n255\n", WIDTH, HEIGHT);
    for (int y = 0; y < HEIGHT; ++y){
        for (int x = 0; x < WIDTH; ++x) {
            fwrite(&screen[y][x].r, sizeof(char), 1, f);
            fwrite(&screen[y][x].g, sizeof(char), 1, f);
            fwrite(&screen[y][x].b, sizeof(char), 1, f);
        }
    }
    fclose(f);
}

int readParams(int argc, char** argv, float* p)
{
    if (argc < 2) {
        printf("Usage: %s <# of points> [FACTOR] \n", argv[0]);
        printf("Generates Voronoi diagram as PPM file\n");
        printf("Optional FACTOR represents parameter in Minkowski distance calculation.\n");

        printf("Example: %s 16 \n", argv[0]);
        printf("         %s 10 1.5\n", argv[0]);
        exit(1);
    }

    int pointsCount = atoi(argv[1]);
    if (pointsCount < 1) {
        printf("Wrong number of points: %s\n", argv[1]);
        exit(1);
    }

    if (argc == 3) {
        *p = atof(argv[2]);
        if (*p <= 0) {
            printf("Positive FACTOR expected: %f\n", *p);
            exit(1);
        }
    }

    return pointsCount;
}

int main(int argc, char** argv)
{
    float p = 2.0f;
    int pointsCount = readParams(argc, argv, &p);

    defineColors();
    setRandomPoints(pointsCount);
    renderGraph(pointsCount, p);
    drawPoints(pointsCount);
    saveToFile("voronoi.ppm");

    free(points);
    return 0;
}
