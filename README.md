# alt_test


A simple CLI application for analyzing package branches in a repository.
The application downloads the package lists for the two branches and compares them.


# Build

```
su-
apt-get update
apt-get install git gcc-c++ libpoco-devel
git clone https://github.com/arxanev-g/alt_test.git
cd alt_test
make
```

# Usage

By default, the sisyphus and p10 branches are used:

```
make run
```

To use other branches, you should write make run ARGS="mybranch1 my-branch-2", syntax example:

```
make run ARGS="p9 p10"
```
The result is saved to the result.json file.

Deleting binary files, object files, and libraries

```
make clean
```
