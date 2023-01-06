# Getting Started in Verilog

As you might have seen in the higher folder, this project serves to demonstrate how I've walked through learning verilog. I've taken a beginner's computer architecture and digital logic class, so this will be my first time working with verilog and attempting to remember concepts from both classes. My ultimate goal (which may change between the time of writing this) is to demonstrate my progress in learning verilog and cryptography and applying it to a real device.

For the first example, my main goal was to get some very simple logic working and have it uploaded and running on the FPGA board. Using the [link here](https://verilogguide.readthedocs.io/en/latest/verilog/firstproject.html#introduction), I was able to get a simple example uploaded. For the purposes of this repo, I won't go into detail on uploading for a specific board, though, I will try to detail the verilog code and demonstrate verilog on an FPGA.

The first example is a half-adder. From digital logic courses, a half adder is one that has both an XOR and an AND gate, where the AND gate is one that only outputs a logic level "HIGH" if both inputs are HIGH. An XOR gate, on the other hand, only outputs a logic level "HIGH" when either inputs are "HIGH", but not both.

Here's the code for this operation:

```verilog
// half adder verilog

module half_adder_verilog(
		input	wire	a, b,
		output wire sum, carry
);
```

The module is analogous to a "function" if you've programmed before. You can specify inputs and outputs. Here, we want our inputs to be ```a``` and ```b```, and the outputs to be ```sum``` and ```carry```.

Next, we want to *assign* these inputs and outputs to each other:
```verilog
assign sum = a ^ b;
assign carry = a & b;
```

Here, we assign the sum using the XOR operation symbol ```^```, and the AND operation using the symbol ```&```.

Next, we want to close the module:

```verilog
endmodule
```

The entire code looks like this:
```verilog
// half_adder_verilog.v

module half_adder_verilog(
		input	wire	a, b,
		output wire sum, carry
);

assign sum = a ^ b;
assign carry = a & b;

endmodule
```

Next, we'll do something a bit more complicated. A 4-bit counter.