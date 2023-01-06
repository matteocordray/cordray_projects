// Switch to Hex
module BCD_Seven_Segment(

	input [3:0] Y,
	output reg[6:0] sevenSegment
	
);



always @ ( * ) begin
	case (Y)
		0 : sevenSegment = 7'b1000000; // 0
		1 : sevenSegment = 7'b1111001; // 1
		2 : sevenSegment = 7'b0100100; // 2
		3 : sevenSegment = 7'b0110000; // 3
		4 : sevenSegment = 7'b0011001; // 4
		5 : sevenSegment = 7'b0010010; // 5
		6 : sevenSegment = 7'b0000010; // 6
		7 : sevenSegment = 7'b1111000; // 7
		8 : sevenSegment = 7'b0000000; // 8
		9 : sevenSegment = 7'b0010000; // 9
		default : sevenSegment = 7'b1000000; // 0
	endcase;
end

endmodule
		
			
