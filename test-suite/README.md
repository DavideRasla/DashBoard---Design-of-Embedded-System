# Unit and Coverage Testing

This directory contains some utilities to run unit testing and and coverage testing on this Dashboard system.

To run these tests, run:
```sh
$ make clean
$ make
```

The `clean` options prepares all the needed directories by the subsequent `make` command. This will create two directories:
- `res-unit`, which will contain results of unit testing under the form of `*.xml' files, which can be opened by any browser and explored in an html-like version;
- `res-coverage`, which will contain code coverage informations, in particular in its `html` subfolder.

## Requirements

- CUnit - for unit testing
- Gcov
- Lcov - for graphical coverage measurement
