# 4 bit counter

For this section, the 4 bit counter is implemented in Verilog. We first assign a basic module to a `button_counter` as shown below:

```verilog
module button_counter (
	// Inputs
	input			wire  [1:0]		pmod,
	
	// Outputs
	output		    reg	  [3:0]		led
);
```

Here, we have two inputs, `pmod` and `led`. The `pmod` input is a 2-bit input, where the `wire` means the variable is read-only, and the `led` output is a 4-bit output, where the `reg` means the variable is R/W capable. The `pmod` input is used to read the switch inputs that are located on the board, and the `led` output is used to display the counter value on the LEDs.

Next, we'll assign new wires to the `clk` and `rst` lines of a 4-bit counter:

```verilog
wire rst;
wire clk;
// Reset is the inverse of the first button
assign rst = ~pmod[0];

// Clock signal is the inverse of the second button
assign clk = ~pmod[1];
```

Next, we will count up on just the clock's *rising* edge or we reset on a button push
```verilog
always @ (posedge clk or posedge rst) begin
    if (rst == 1'b1) begin
        led <= 4'b0;
    end else begin
        led <= led + 1'b1;
    end
end
```

The `1'b1` is a 1-bit constant, and holds the value that comes after the `b`, which in this case is `1`.

If the `rst` button is pushed, the clock will *reset* and the counter will start from 0. Otherwise, the counter will *shift* (that is what the `<=` operator does) and increment by 1 (hence the `1b'1`).

This simple but effective application shows the basics when it comes to verilog, and implementing a counter in verilog seemed like a good first step towards understanding the entirety of the application. For information on uploading the DE-1 SOC board, go to the [FPGA Designs with Verilog and SystemVerilog Website](https://verilogguide.readthedocs.io/en/latest/verilog/firstproject.html) to find out specifics on using Quartus Prime and the associated programmer.

Below is a GIF of the counter in action:

<p align="center">
   <img src="images/4_bit_counter_demo.gif" width="400" />
   <p align="center"> Figure 1. <i>The 4-bit counter</i></p>
</p>