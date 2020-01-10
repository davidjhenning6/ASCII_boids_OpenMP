David Henning
Basic ascii parallel implementation of a Boids simulation.

This implementation uses the OpenMP parallel library.
The program can run in two modes, one without ascii graphics for displaying the time taken to complete the specified number of iterations and the other to display the boids behaviour.

CIS 3090 A3

Code tested compiled and executed on ginny server
Please grade on the ginny socs server.


        Processes     1                 2               3               4
Iterations            
> 1000	            1.593000        0.153000        0.115000        0.108000

> 2000              2.252000        0.295000        0.226000        0.243000

> 3000	            3.002000        0.443000        0.354000        0.310000

> 10000	            11.46200        1.466000        1.339000        1.141000

> 20000             20.95000        3.095000        2.468000        2.216000

> 30000             30.45500        5.351000        3.772000        3.325000

> 100000            98.50000        16.522000       12.76000        10.525000

> 200000            186.9710        29.901000       23.91100        21.324000

> 300000            265.5250        48.235000       37.255000       30.940000

