# 16F84-shift-register
In order to make an embedded solution, sometimes we need many GPIO pins to control LEDs or Seven-segments and also we need sometimes an actuators. 
among the tips is using a shift register, takes 8-bit serial data to show it out as parallel data, to do that, I proposed an algorithm based on the timer interrupt that allows getting an optimized code.
