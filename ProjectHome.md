# About #
This is a simple C++ library that optimizes 3D indexed triangle meshes for use with a vertex caching renderer. The library is implemented and distributed as a single header file.

# Features #
  * Linear speed.
  * No installation required: just #include vcacheopt.h
  * Easy to use: all you need to do is pass your index buffer and triangle count to the "Optimize" function.
  * Free, as in [freedom](http://en.wikipedia.org/wiki/Gratis_versus_Libre).
  * Thread-safe: allowing for multiple optimizers running in different threads.

# The algorithm #
[Here](http://home.comcast.net/~tom_forsyth/papers/fast_vert_cache_opt.html) is a description of the algorithm implemented.


# License #
The library is free software, licensed under the MIT license.