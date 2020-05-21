# MiniJava Compilers

This is a project for creation a MiniJava Compiler.
[![Build Status](http://img.shields.io/travis/badges/badgerbadgerbadger.svg?style=flat-square)](https://travis-ci.org/badges/badgerbadgerbadger) [![Dependency Status](http://img.shields.io/gemnasium/badges/badgerbadgerbadger.svg?style=flat-square)](https://gemnasium.com/badges/badgerbadgerbadger) [![Coverage Status](http://img.shields.io/coveralls/badges/badgerbadgerbadger.svg?style=flat-square)](https://coveralls.io/r/badges/badgerbadgerbadger) [![Code Climate](http://img.shields.io/codeclimate/github/badges/badgerbadgerbadger.svg?style=flat-square)](https://codeclimate.com/github/badges/badgerbadgerbadger) [![Github Issues](http://githubbadges.herokuapp.com/badges/badgerbadgerbadger/issues.svg?style=flat-square)](https://github.com/badges/badgerbadgerbadger/issues) [![Pending Pull-Requests](http://githubbadges.herokuapp.com/badges/badgerbadgerbadger/pulls.svg?style=flat-square)](https://github.com/badges/badgerbadgerbadger/pulls) [![Gem Version](http://img.shields.io/gem/v/badgerbadgerbadger.svg?style=flat-square)](https://rubygems.org/gems/badgerbadgerbadger) [![License](http://img.shields.io/:license-mit-blue.svg?style=flat-square)](http://badges.mit-license.org) [![Badges](http://img.shields.io/:badges-9/9-ff6799.svg?style=flat-square)](https://github.com/badges/badgerbadgerbadger)


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
./simple_tests.sh
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
