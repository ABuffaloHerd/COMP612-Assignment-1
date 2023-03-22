# 20/03/2023
Added a full snowman.
![](Pasted%20image%2020230321005828.png)

The eyes and nose were drawn with the new `render_poly` function.
![](Pasted%20image%2020230321005933.png)
This batch colour converter works but doesn't work the moment multidimensional arrays come into play.
![](Pasted%20image%2020230321010006.png)

Fixed bug that caused snow particles to disappear.
``	p->mass = rand() % 11 + 1; // between 0 and 10``
This caused particles with 0 mass to start spawning. These particles would not move and would not be rendered.