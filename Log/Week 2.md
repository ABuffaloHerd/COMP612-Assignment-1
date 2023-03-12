# 8/03/2023
My solution and word doc have disappeared so now this is on github.

# todo:
- Redo rendering queue
- Find way to render shapes in a flexible manner.

After research and consultation, i have come to the conclusion that using a linked list to contain all objects for each layer can work. I can tag each shape to find it in the list later on.

## Time: 2 hours
I have now finished a linked list implementation. It's also been tested.
![[Pasted image 20230308222208.png]]

# 9/03/2023
## Time: 3 hours
 - Finish custom render list implementation
 - Test it out

Migrated new files to project.
The shape structure now has a function pointer for custom rendering, that way if the shape's type is CUSTOM_SHAPE, the custom function will be run.
![[Pasted image 20230309180905.png]]
It's now rendering a black square on a black background again. This is because i forgot to put ``glBegin(GL_QUADS)``

It works now.
![[Pasted image 20230309202958.png]]
And now to animate it i simply search for this named square and change its attributes.
![[Pasted image 20230309203151.png]]

![[Pasted image 20230309203205.png]]

To get this far i had to change the way the parameters were inserted from inline arrays to named arguments.

Now to actually get started on the assignment I rendered a background using the rendering setup I created earlier.
![](Pasted%20image%2020230309233016.png)
![](Pasted%20image%2020230309233023.png)
![](Pasted%20image%2020230309233038.png)

Rendering triangles has some problems though. I want to add some mountains to the background.
![](Pasted%20image%2020230310001540.png)
It appears that my math is incorrect and it took some faffing about with a unit circle to figure out why.
![](Pasted%20image%2020230310001607.png)
![](Pasted%20image%2020230310001717.png)
![](Pasted%20image%2020230310001724.png)
Almost there, just needs some tweaking.
![](Pasted%20image%2020230310001803.png)

# 11/03/2023
Goals:
- render ground with randomization
Since ground has to be done randomly i have split it into a function. However, in order to pass the same arguments in i have to use a global float list. This can be passed into the function and then risky pointer arithmetic can be used.
![](Pasted%20image%2020230311212127.png)
![](Pasted%20image%2020230311212146.png)
Program seems stable, so I will safely assume that nothing bad is happening.
![](Pasted%20image%2020230311212547.png)
This is the final code. Let's see how that runs.
![](Animation.gif)
So after changing my code to include `glEnd` and fixing the `dx` calculation, this is the result:
![](Pasted%20image%2020230311213134.png)
After changing `x += dx` to `x -= dx`
![](Pasted%20image%2020230311213230.png)
![](Pasted%20image%2020230311213402.png)
`GL_QUADS` to `GL_QUAD_STRIP`
WTF IS THIS

![](Pasted%20image%2020230311213726.png)
Ok i changed it to `GL_POLYGON` which is what i was after anyway i forgot to read the docs. I also switched to drawing the bottom left and right points first before the rest of the polygon. To make a clean cut edge, i can simply put a final vertex at -1.0, -0.3. It looks very choppy so i can tweak the range of values that are randomized.

For testing I will add a key that regenerates the ground when pressed.