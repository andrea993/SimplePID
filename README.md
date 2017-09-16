# SimplePID
Fast [PID controller](https://en.wikipedia.org/wiki/PID_controller) implemented in parallel form.

## Derivation

Analog PID transfer function:

<img src="images/G_a.png" width="350">

Discrete PID transfer function calculated by Euler method:

<img src="images/G_d.png" width="600">

Transfer function comparsion (G_a: analog, G_d discrete):

<img src="images/cmp.png" width="700">

Parallel decomposition:

<img src="images/para.png" width="600">
