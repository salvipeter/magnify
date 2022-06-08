# Magnify

A simple applet using [GLUT](http://freeglut.sourceforge.net/)
and [Magick++](https://imagemagick.org/Magick++/) for trying out a
mapping on a 2D image.

Keys:

- `q` quits
- `d` and `f` change the mapping constant
- `j` and `k` change the radius

If `r` is the radius and `d` the distance of a point from the center of the map,
the new distance is computed as `r * d / (r - d)`, multiplied by a constant.
This gives `0` for the center point (when `d = 0`), and infinity when `d = r`.
