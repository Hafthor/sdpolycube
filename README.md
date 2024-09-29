# sdpolycube

Only computes a single trivial symmetry at a time.

Run ./make.sh to build

Example Usage: ./sdpolycube 20
Computes trivial symmetry for filter=20 and outputs the full count.

Add up all the trivial symmetries for the given N and the non-trivial symmetries (which you can compute with DoddsPolycube) and divide by 24 to get the unique polycubes for the given N.

For performance, N is set as a const int in main.c. Speaking of performance, this C implementation is about 15% faster than C# version in DoddsPolycube.
