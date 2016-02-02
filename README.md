# Attractors

Drawing some attractor functions. Should build on Windows (Visual Studio),
OS X and Linux (clang++, SCons). Uses a (very slightly) modified lodepng for
writing out images.

## Attractor functions?

Iterative functions that tend towards a particular set. Sometimes this is a
single value, or a cycle of a handful of values. Sometimes, it's a large
set of values that when plotted, draw a pretty pattern. This project tries
to find and draw the latter.

# Examples

## A = -1.52331, B = 1.28192, C = 1.45112, D = -0.975345

Rendered at 1920x1080, 500 million iterations, gamma 1.5, exposure clamp 0.8.

![Example 1](https://raw.githubusercontent.com/cawhitworth/Attractors/master/examples/example_1.png)
