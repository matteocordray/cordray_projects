






module seven_segment_display_driver(
	
	input wire [1:0] KEY,
	output [6:0] HEX0, HEX1, HEX2
		);

// everything that comes out of the individual modules is declared as wire
wire [3:0] ones, tens, hundreds;


// initiate 8 D flip flops
wire [10:0] Q;
D_FF D0(.clk(KEY[0]), .reset(KEY[1]), .D(Q[1]), .Q(Q[0]));
D_FF D1(KEY[0], KEY[1], Q[2], Q[1]);
D_FF D2(KEY[0], KEY[1],  Q[3], Q[2]);
D_FF D3(KEY[0], KEY[1], Q[4], Q[3]);
D_FF D4(KEY[0], KEY[1], Q[5], Q[4]);
D_FF D5(KEY[0], KEY[1], Q[6], Q[5]);
D_FF D6(KEY[0], KEY[1], Q[7], Q[6]);
D_FF D7(KEY[0], KEY[1], Q[8], Q[7]);
D_FF D8(KEY[0], KEY[1], Q[9], Q[8]);
D_FF D10(KEY[0], KEY[1], ~(Q[7] ^ Q[5] ^ Q[4] ^ Q[3]), Q[9]);



// These modules are additional in the BCD converter folder
Binary_BCD_Converter U1(.A(Q), .Hundreds(hundreds), .Tens(tens), .Ones(ones));

BCD_Seven_Segment U2(ones, HEX0);
BCD_Seven_Segment U3(tens, HEX1);
BCD_Seven_Segment U4(hundreds, HEX2);
endmodule 
