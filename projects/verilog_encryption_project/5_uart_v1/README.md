# UART v2

This folder shows the next step I took after completing some of the AES encryption algorithms. I wanted to see how feasible it is to implement UART on an FPGA, and after banging my head against a wall at why one github repo wasn't working, I found [this one](https://github.com/varmil/uart-verilog) that seems to do the job.

With this, unlike the AES encryption side of things, I was able to get this working (decently) on the FPGA. There is a GIF below demonstrating the functionality (the GIF format makes it hard to see, I recommend viewing the MP4 video located in the `images` folder to get a closer look at what is going on).

<p align="center">
<img src="images/uart_demo.gif" width="600" />
<p align="center"> Figure 1. <i>UART v2</i></p>