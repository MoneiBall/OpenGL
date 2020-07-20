# OpenGL
## Computer Graphics with OpenGL

### Tasks

Specify normal vectors for each face, such that normals are perpendicular to the triangle.

Create a plane quad (rectangular geometry using VAO s and VBO s or gVertexBuffer) in
the y=0 plane, of dimensions 50×50 and place it down as the ‘ground’. Specify correct
texture coordinates and normals for the plane quad. Put water.bmp texture on
the plane quad.

Load the fish.obj from file (1 point) and put fish.jpg texture on it. Don't change
the texture of the ground. Put it in the center (0,0,0) in a way that only half of the fish is above
the planequad. Scale the fish to be 0.2 of its original size.

Set the color of the octahedron inside the fragment shader to be constant RGB(0.73, 0.95,
1.0) instead of texturing (use uniforms). Put five octahedrons around the fish in a circle
with a radius of five, alternatively put only one for 1 point. Have them (it) rotate
around the fish, it should take 5 seconds to take one rotation. Make them (it)
rotating around their own Y axis.

Let the user control the fish with the arrow keys, so that it always stays on the water and
can move in four directions. (E.g. when pressing the right key, it moves a little in the +X
direction, when pressing the up key, a little in the -Z direction, etc.). Make the fish to
turn in the direction of the movement.

Create multiple (at least 4) fishes with random distances from the center (so that they are
still in the 50×50 ground) (you can just use rand() to generate random integers). They
should all rotate around the center in the XZ plane. Let them have different speeds, the
period time of the circular movement should be a random duration between 3 and 10 seconds.

Let the light source, illuminating the scene, be a point light with blue diffuse color. The
point should be above the ground by 10 units.
