# Instance Generator

### Techonologies

* Python 3.5+
* Pandas 1.0.5

### Input files

The Solomon instances can be found for instance [here](http://web.cba.neu.edu/~msolomon/problems.htm).

### Running

The script may be run in command line via

`
<Python-Interpreter> instance_generator.py <Solomon instance folder> <number of service stations per quadrant> <charging time> <battery capacity> <Output folder>
`

where the provided arguments are

* `<Solomon instance folder>`: Directory of the input files.
* `<number of service stations per quadrant>`: An integer number specifying the number of service stations per city centre. City centres are located at the centre of each quadrant.
* `<charging time>`: A number spcifying the fixed time required for conducting a replenishment.
* `<battery capacity>`: A number specifying the maximally available amount of a single resource.
* `<Output folder>`: Directory for storing the generated files.