#hilbert.hpp

##Introduction

hilbert.hpp contains two implementations of the hilbert curve encoding &
decoding algorithm described by John Skilling in his paper "Programming the
Hilbert Curve".

These implementations expose the same interface, and reside in the namespaces
`hilbert::v1` and `hilbert::v2`.

`hilbert::v1` implements the algorithm as described in the paper.
`hilbert::v2` uses template metaprogramming to unroll various loops.

In most cases v2 will be faster than v1, at the expense of code size.  It is
possible that at some point v1 will overtake v2 for higher dimensional/high
precision data due to cache limits but I have not observed this yet.

Performance is radically different between optimised and unoptimised builds of
the code.  Performance tests on my laptop give the following results:

~~~~
$ g++ test.cpp -O0
$ ./a.out
v1 took 259668 ms
v2 took 280517 ms
$
$ g++ test.cpp -O3
$ ./a.out
v1 took 23182 ms
v2 took 9387 ms
~~~~

##Building

This is a header only library.  Simply add `#include "hilbert.hpp"` to your code and use it.

##Interface

Two functions are supported:

~~~~
template<typename T, size_t N>
std::array<T, N>
IndexToPosition(std::array<T, N> const &in)`
~~~~

and

~~~~
template<typename T, size_t N>
std::array<T, N>
PositionToIndex(std::array<T, N> const &in)
~~~~

`IndexToPosition` takes an index packed into an N element vector of type T, and
returns an N dimensional position.

`PositionToIndex` is the inverse of `IndexToPosition`.

Indices are encoded in lexographic order, with the most significant component
first.  If the T is of type `uint8_t` then this is equivalent to big endian
encoding.

An example of how to do pack and unpack data for simple cases can be found in 
`test.cpp`.

T should be an unsigned integer type.

