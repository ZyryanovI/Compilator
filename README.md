# MiniJava Compilers

[![GitHub release](https://img.shields.io/github/release/Naereen/StrapDown.js.svg)](https://GitHub.com/Naereen/StrapDown.js/releases/)
[![Build Status](http://img.shields.io/travis/badges/badgerbadgerbadger.svg?style=flat-square)](https://travis-ci.org/badges/badgerbadgerbadger)
[![Maintenance](https://img.shields.io/badge/Maintained%3F-yes-green.svg)](https://GitHub.com/Naereen/StrapDown.js/graphs/commit-activity)
[![Coverage Status](http://img.shields.io/coveralls/badges/badgerbadgerbadger.svg?style=flat-square)](https://coveralls.io/r/badges/badgerbadgerbadger) 
[![GitHub issues](https://img.shields.io/github/issues/Naereen/StrapDown.js.svg)](https://GitHub.com/Naereen/StrapDown.js/issues/)
[![Badges](http://img.shields.io/:badges-9/9-ff6799.svg?style=flat-square)](https://github.com/badges/badgerbadgerbadger)

### This is a MiniJava Compiler.
## How to run
To clone:
```
git clone https://github.com/ZyryanovI/Compilator.git
```
To run:
```
./init.sh
```
It will compile `bison` and `flex` to produce final-state machine (debug is in `parser.output`), compile AST, Symbols Table and IRT. Than it will read `input.txt`, parse it through final-state machine, create AST, draw in into `output.dot` file, create Symbols Table and IRT (and draw it in several files names `outputCLASS_NAME::METHOD_NAME.dot`).

To run autotests:
```
./simple_tests.sh
```

To show graph: <br />
```asm
sudo apt install xdot
xdot output.dot
```

To clean generated files:
```
make clean
```


### Done
* Added flex
* Added bison
* Added grammar
* Added AST
* Added visitor
* Added printer
* Added AST visualization
* Added Symbols Table
* Added IRT + visualization
* Added autotests
