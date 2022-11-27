# Voronoi Diagram in C

## Usage

Compile from source code:

```bash
make
```

Execute from command line:

```bash
./voronoi
```

Program generates diagram, saved as `voronoi.ppm` file in [PPM](https://en.wikipedia.org/wiki/Netpbm#PPM_example) format. Can be viewed with:

```bash
xdg-open voronoi.ppm
eog voronoi.ppm
```

Program accepts following parameters:

- `-s #` - number of points (seeds) in diagram.
- `-p #` - parameter in Minkowski distance calculation. By default this parameter
equals to 2, which represents classical Euclidean distance. When set to 1, cells are generated with Manhattan distance. Parameter accepts float numbers.
- `-c` - dynamically generate color palette, instead of predefined 15 colors.
- `-f FILENAME` - output filename, overrides default `voronoi.ppm`.
- `-w #` - diagram canvas width.
- `-h #` - diagram canvas height.
- `-r #` - point radius.

Advanced usage example:

```bash
./voronoi -s 5 -p 1.4
./voronoi -s 40 -h 1000 -w 800 -r 3 -c -p 2.5 -f result.ppm
```

## Example
![Voronoi diagram with 15 points](./voronoi.png)

## Links

- [Voronoi diagram](https://en.wikipedia.org/wiki/Voronoi_diagram) on Wikipedia
- [Minkowski distance](https://en.wikipedia.org/wiki/Minkowski_distance) on Wikipedia
- [Manhattan distance](https://en.wikipedia.org/wiki/Taxicab_geometry) on Wikipedia

