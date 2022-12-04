#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>
#include <errno.h>
#include <assert.h>
#include <math.h>
#include <getopt.h>
#include <string.h>

#define return_defer(value) do { result = (value); goto close_file; } while(0)

#define RADIUS 4
#define WIDTH 600
#define HEIGHT 400
#define NUMBER_OF_POINTS 15
#define FILENAME "voronoi.ppm"
#define P_FACTOR 2.0f

typedef int Errno;

typedef struct {
    uint8_t r;
    uint8_t g;
    uint8_t b;
} Pixel;

typedef struct {
    int x;
    int y;
    Pixel color;
} Point;

typedef struct {
    int pointsCount;
    Point* points;
    int radius;
    int width;
    int height;
    int autoGenerateColors;
    float p;
    char resultFile[100];
    Pixel* screen;
    Pixel* colors;
} Diagram;

#define COLORS 16
enum color_name {
 	Black,
  	White,
  	Red,
  	Lime,
  	Blue,
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

Pixel* defineColors()
{
    Pixel* colors = malloc(COLORS * sizeof(Pixel));
    assert(colors != NULL);

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

    return colors;
}

static inline long map(long x, long in_min, long in_max, long out_min, long out_max)
{
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

void setRandomPoints(Diagram* v)
{
    srand(time(NULL));

    v->points = (Point*) malloc(v->pointsCount * sizeof(Point));
    assert(v->points);

    for (int i = 0; i < v->pointsCount; ++i) {
        v->points[i].x = rand() % v->width;
        v->points[i].y = rand() % v->height;
        if (v->autoGenerateColors) {
            v->points[i].color.r = map(v->points[i].x, 0, v->width, 0, 255);
            v->points[i].color.g = map(v->points[i].y, 0, v->height, 0, 255);
            v->points[i].color.b = map(v->points[i].x + v->points[i].y, 0, v->width + v->height, 0, 255);
        } else
            v->points[i].color = v->colors[1 + (i % 15)];  // skip black
    }
}

void drawPoints(Diagram* v)
{
    for (int i = 0; i < v->pointsCount; ++i) {
        int xmin = v->points[i].x - v->radius;
        int xmax = v->points[i].x + v->radius;
        int ymin = v->points[i].y - v->radius;
        int ymax = v->points[i].y + v->radius;

        for (int y = ymin; y <= ymax; ++y) {
            if (y < 0 || y >= v->height) continue;

            for (int x = xmin; x <= xmax; ++x) {
                if (x < 0 ||  x >= v->width) continue;

                int dx = v->points[i].x - x;
                int dy = v->points[i].y - y;
                if (v->radius * v->radius >= dx * dx + dy * dy) {
                    v->screen[y * v->width + x] = v->colors[Black];
                }
            }
        }
    }
}

static inline float minkowskiDistance(float p, float dx, float dy)
{
    return powf(dx, p) + powf(dy, p);
}

void renderGraph(Diagram* v)
{
    for (int y = 0; y < v->height; ++y) {
        for (int x = 0; x < v->width; ++x) {
            int closest = 0;
            float min_distance = 3.40282e+038;

            for (int i = 0; i < v->pointsCount; ++i) {

                float dx = abs(x - v->points[i].x);
                if (dx > min_distance) continue;          // too far

                float dy = abs(y - v->points[i].y);
                if (dy > min_distance) continue;          // too far

                float distance = minkowskiDistance(v->p, dx, dy);
                if (min_distance > distance) {
                    min_distance = distance;
                    closest = i;
                }
            }
            v->screen[y * v->width + x] = v->points[closest].color;
        }
    }
}

Errno saveToFile(Diagram* v)
{
    Errno result = 0;
    FILE* f = NULL;

    f = fopen(v->resultFile, "wb");

    if (f == NULL) return_defer(errno);

    fprintf(f, "P6\n%d %d\n255\n", v->width, v->height);
    if (ferror(f)) return_defer(errno);

    fwrite(v->screen, sizeof(Pixel), v->width * v->height, f);   // save full screen memory
    if (ferror(f)) return_defer(errno);

 close_file:
    if (f) fclose(f);
    return result;
}

void usage(char* prog)
{
        printf("Usage: %s [OPTION]...\n", prog);
        printf("Generates Voronoi diagram as PPM file.\n");
        printf("\n");
        printf("-s #        - number of points in diagram, default %d\n", NUMBER_OF_POINTS);
        printf("-p #        - parameter in Minkowski distance calculation, default %.1f\n", P_FACTOR);
        printf("-c          - automatically generate color palette\n");
        printf("-f FILENAME - optional output filename, default %s\n", FILENAME);
        printf("-w #        - diagram width, default %d\n", WIDTH);
        printf("-h #        - diagram height, default %d\n", HEIGHT);
        printf("-r #        - point radius, default %d\n", RADIUS);
        printf("\n");
        printf("Example: %s -s 9 \n", prog);
        printf("         %s -s 44 -p 1.5 -c\n", prog);
        exit(1);
}

void readParams(int argc, char** argv, Diagram* d)
{
    char opt;
    while ((opt = getopt(argc, argv, "-s:-p:-f:-c-w:-h:-r:")) != -1 ) {
        switch (opt) {
            case 's':
                d->pointsCount = atoi(optarg);
                if (d->pointsCount < 1) {
                    printf("Wrong number of points: %s\n", optarg);
                    exit(1);
                }
                break;

            case 'p':
                d->p = atof(optarg);
                if (d->p <= 0) {
                    printf("Positive FACTOR expected: %s\n", optarg);
                    exit(1);
                }
                break;

            case 'c':
                d->autoGenerateColors = 1;
                break;

            case 'f':
                strcpy(d->resultFile, optarg);
                break;
            case 'w':
                d->width = atoi(optarg);
                if (d->width > 5000 || d->width <= 0 ) {
                    printf("Expected width in range 0 - 5000\n");
                    exit(1);
                }
                break;
            case 'h':
                d->height = atoi(optarg);
                if (d->height > 4000 || d->height <= 0 ) {
                    printf("Expected height in range 0 - 4000\n");
                    exit(1);
                }
                break;
            case 'r':
                d->radius = atoi(optarg);
                if (d->radius > 100 || d->height <= 0 ) {
                    printf("Expected radius in range 0 - 100\n");
                    exit(1);
                }
                break;
            default:
                usage(argv[0]);
            }
    }
}

Diagram* initDiagram(int argc, char** argv)
{
    static Diagram voronoi = {
        .pointsCount = NUMBER_OF_POINTS,
        .points = NULL,
        .radius = RADIUS,
        .width = WIDTH,
        .height = HEIGHT,
        .autoGenerateColors  = 0,
        .p = P_FACTOR,
        .resultFile = FILENAME,
        .screen = NULL,
        .colors = NULL,
    };

    readParams(argc, argv, &voronoi);

    voronoi.screen =  (Pixel*) malloc(sizeof(Pixel) * voronoi.height * voronoi.width);
    assert(voronoi.screen != NULL);

    voronoi.colors = defineColors();

    return &voronoi;
}

int main(int argc, char** argv)
{
    Errno result = 0;

    Diagram* v = initDiagram(argc, argv);
    setRandomPoints(v);
    renderGraph(v);
    drawPoints(v);
    Errno err = saveToFile(v);
    if (err != 0) {
        fprintf(stderr, "Error: could not write to file %s: %s\n", v->resultFile, strerror(err));
        result = 1;
    }

    return result;;
}
