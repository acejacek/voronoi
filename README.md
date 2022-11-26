# Voronoi Diagram in C

## Usage

Compile from source code:

```bash
make
```

Execute from command line:

```bash
./voronoi 10
```

The parameter is number of points (or seeds) on the diagram.

Program generates diagram, saved as `voronoi.ppm` file in [PPM](https://en.wikipedia.org/wiki/Netpbm#PPM_example) format. Can be viewed with:

```bash
xdg-open voronoi.ppm
eog voronoi.ppm
```

Program accepts optional second parameter, which is a factor in calculating distance to the seeds. By default this parameter
equals to 2, which represents classical Euclidean distance. When set to 1, cells are generated with Manhattan distance.

```bash
./voronoi 5 1.4
```

Parameter is expected to be greater than 0 (zero).

## Example
![Voronoi diagram with 15 points](./voronoi.png)

## Links

- [Voronoi diagram](https://en.wikipedia.org/wiki/Voronoi_diagram) on Wikipedia
- [Minkowski distance](https://en.wikipedia.org/wiki/Minkowski_distance) on Wikipedia
- [Manhattan distance](https://en.wikipedia.org/wiki/Taxicab_geometry) on Wikipedia

