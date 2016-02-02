# Attractors

![Small example](https://raw.githubusercontent.com/cawhitworth/Attractors/master/examples/small_example.png)

Drawing attractor functions (at the moment, Pickover, or Clifford, and Peter de
Jong attractors) in C++. Should build on Windows (Visual Studio), OS X and
Linux (clang++, SCons). Uses a (very slightly) modified
[lodepng](https://github.com/lvandeve/lodepng) for writing out images.

As well as rendering the images using custom gradient palettes and configurable
gamma and exposure, the code attempts to find aesthetically interesting sets of
coefficients automatically.

Initial inspiration came from [this](http://paulbourke.net/fractals/clifford/)
popping up on Hacker News.

## Attractor functions?

Iterative or derivative functions that tend towards a particular set. Sometimes
this is a single value, or a cycle of a handful of values. Sometimes, it's a
large set of values that when plotted, draw a pretty pattern. This project
tries to find and draw the latter.

## Finding interesting patterns

The code essentially looks for sets of coefficients that, when the attractor
function is iterated, produce a large number of different values quickly. The
way it does this probably isn't the most efficient.

This process repeats with randomly chosen coefficients until an 'interesting'
set is found.

First, it runs 10,000 iterations of the pure function, keeping track of the
minimum and maximum x and y values - this firstly gives us the bounds for the
function so we can scale when rendering, and secondly, if any of the bounds is
still zero, we can take a reasonable guess at this not being an interesting set
of coefficients.

Next, it runs another 10,000 iterations of an exposure pass (see below) on a
640x512 bitmap. If the maximum exposure recorded during this pass is greater
than 10 - i.e., if any single pixel in the bitmap is visited more than 10
times - we consider this set of coefficients uninteresting.

## Rendering

This is a two-stage process. First, the image is *exposed*, and then
*developed*.

### Exposure

The function is repeatedly evaluated; after each iteration, the resulting x
and y values are mapped back onto the bitmap (using the bounds we calculated
in the evaluation stage). The value in the corresponding cell in the bitmap
is incremented by one, and the maximum value seen in any cell is tracked -
the `maxExposure` value in the code.

### Developing

Once the image is exposed, we have a grid of values between `0` and
`maxExposure`. The develop stage maps these values onto the range from 0-1
using the `maxExposure` and a gamma correction (i.e., `p -> p ^ 1/gamma`). The
result is clamped to the range 0-1, so using a value lower than `maxExposure`
will increase the effective exposure of the image and the expense of blowing
out the brighter end.

Once the adjusted exposure value has been calculated, it is mapped onto a
colour gradient defined by a set of values from 0-1 and corresponding RGB
values; the colours are linearly interpolated appropriately.

## Comments on the code

It's pretty messy. The style is kind of all over the place because I've not
had much chance to really properly drill down into modern C++. There's a kind
of mishmash of `#defines` and inline functions and things and the main
Attractors.cpp has way too much in it and there's no tests and... sorry,
basically.

I quite like the functional stuff and the iterating over the `Bitmap` part in
`develop()` though.

There shouldn't be any warnings using the standard C++ compiler in Visual
Studio 2015.

# Examples

## Clifford, A = -1.52331, B = 1.28192, C = 1.45112, D = -0.975345

Rendered at 1920x1080, 500m iterations, gamma 1.5, exposure clamp 0.8,
Hot palette.

![Example 1](https://raw.githubusercontent.com/cawhitworth/Attractors/master/examples/example_1.png)

## Peter de Jong, A = -1.99727, B = -1.60524, C = 1.72766, D = -1.88512)

Rendered at 1920x1080, 500m iterations, gamma 1.5, exposure clamp 0.8,
WhiteOrange palette.

![Example 2](https://raw.githubusercontent.com/cawhitworth/Attractors/master/examples/example_2.png)
