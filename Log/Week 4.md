Fixed bug that caused snow particles to disappear.
``	p->mass = rand() % 11 + 1; // between 0 and 10``
This caused particles with 0 mass to start spawning. These particles would not move and would not be rendered.