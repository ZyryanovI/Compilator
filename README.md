# MiniJava Compilers

This is a project for creation a MiniJava Compiler.


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

To run autotests:
```
./simple_test.sh
```

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
