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

## Example
![Voronoi diagram with 15 points](./voronoi.jpg)

## Links

- [Voronoi diagram](https://en.wikipedia.org/wiki/Voronoi_diagram) on Wikipedia

