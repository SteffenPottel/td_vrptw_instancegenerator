# Distance matrix generator

### Introduction

The distance matrix generator computes piecewise linear duration and consumption functions for Solomon-type instances with added service stations.

### Technologies

* GCC 8.4.0

### Input files

A distance and consumpation matrix is computed using any instance file generated out of the Solomon instances with the provided script.

### Running

The computation may be started in command line via

`
<executable> <input file> <output file> <number of initial points> <maximal number of segments>
`

where the arguments passed are

* `<input file>`: The directory and filename of the instance file.
* `<output file>`: The directory and filename of the distance matrix. 
	Note that identical input and output file will overwrite the input file.
* `<number of initial points>`: An integer specifying the number of equally spaced starting times.
* `<maximal number of segments>`: An integer specifying the number of segments that the piecewise linear duration and consumption functions maximally have. 