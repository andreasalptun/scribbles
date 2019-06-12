
// ease out at start
t = t*t;

// ease in at end
t = 2*t - t*t;

// ease in and out
float t2 = t*t;
t = 3*t2 - 2*t*t2;
