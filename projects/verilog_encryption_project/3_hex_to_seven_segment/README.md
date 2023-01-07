# Hex to Seven Segment Decoder
This project is for a next step after the 4-bit counter, where we will decode a hexadecimal number and turn it into a number on the seven-segment display on the DE1-SOC board.

We also look into a brief introduction with modules, as they will be useful for the next project.

If we look at the `hexmodule.v` file, we can see that we have a module called `hexModule` that takes in a 4-bit input (`hexNumber`) and outputs two 7-bit outputs (`sevenSegmentActiveLow` and `sevenSegmentActiveHigh`). There are two because I was not sure if the  The 7-bit output is used to display the number on the seven-segment display.

```verilog
module hexModule(

	input [3:0] hexNumber,
	output wire[6:0] sevenSegmentActiveLow, sevenSegmentActiveHigh
	
);
```

Next, we just show the 16 possible outputs that the seven-segment display can display. The `case` statement is used to check if the input is equal to a certain value, and if it is, then the output is set to a certain value. The `default` statement is used if none of the cases are true, and is set to display the letter `f` on the display.

```verilog
always @ ( * ) begin
	case (hexNumber)
		4'b0000 : sevenSegment = 7'b1000000; // 0
		4'b0001 : sevenSegment = 7'b1111001; // 1
		4'b0010 : sevenSegment = 7'b0100100; // 2
		4'b0011 : sevenSegment = 7'b0110000; // 3
		4'b0100 : sevenSegment = 7'b0011001; // 4
		4'b0101 : sevenSegment = 7'b0010010; // 5
		4'b0110 : sevenSegment = 7'b0000010; // 6
		4'b0111 : sevenSegment = 7'b1111000; // 7
		4'b1000 : sevenSegment = 7'b0000000; // 8
		4'b1001 : sevenSegment = 7'b0010000; // 9
		4'b1010 : sevenSegment = 7'b0001000; // a
		4'b1011 : sevenSegment = 7'b0000011; // b
		4'b1100 : sevenSegment = 7'b1000110; // c
		4'b1101 : sevenSegment = 7'b0100001; // d
		4'b1110 : sevenSegment = 7'b0000110; // e
		default : sevenSegment = 7'b0001110; // f
	endcase;
end
```