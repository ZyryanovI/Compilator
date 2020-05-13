# MiniJava Compilers

This is a project for creation a MiniJava Compiler.

Find out what MiniJava is, [here](http://www.cambridge.org/resources/052182060X/MCIIJ2e/grammar.htm)

## How to run
To compile:
```
make
```
It will compile `bison` and `flex` to produce final-state machine (debug is in `parser.output`), compile AST, Symbols Table and IRT.

To run:
```
./parser
```

It will read `input.txt`, parse it through final-state machine, create AST, draw in into `output.dot` file, create Symbols Table and IRT (and draw it in several files names `outputCLASS_NAME::METHOD_NAME.dot`).

To show graph: <br />
```asm
sudo apt install xdot
xdot output.dot
```

To clean:
```
make clean
```

To run static analyzer (do it only after cleaning!):
```
make static
```

## TODO
### Done
* Add flex
* Add bison
* Add grammar
* Add AST
* Add visitor
* Add printer
* Add AST visualization
* Add Symbols Table
* Add Activations Records
* Add IRT + visualization
* Add autotests
* Add statyc analyzer

### Not done yet
* Add classic tree
* Add tree linearization
