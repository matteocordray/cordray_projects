






module Top_Module(
	input clk,
	input load_button,
	input reset,
	output [6:0] seg,
	output [3:0] an
		);
		
parameter zero = 4'b0000; // 255 random number will be generated

// everything that comes out of the individual modules is declared as wire
wire clk_out;
wire [3:0] mux_out;
wire [1:0] counter_out; // enables segment one by one
wire [3:0] ones, tens;
wire [1:0] hundreds;

// initiate button
wire button;

// initiate 8 D flip flops
wire [7:0] Q;
D_FF D0(clk, reset, button, Q[1], Q[0]);
D_FF D1(clk, reset, button, Q[2], Q[1]);
D_FF D2(clk, reset, button, Q[3], Q[2]);
D_FF D3(clk, reset, button, Q[4], Q[3]);
D_FF D4(clk, reset, button, Q[5], Q[4]);
D_FF D5(clk, reset, button, Q[6], Q[5]);
D_FF D6(clk, reset, button, Q[7], Q[6]);
D_FF D7(clk, reset, button, ~(Q[7] ^ Q[5] ^ Q[4] ^ Q[3]), Q[7]);

// These modules are additional in the BCD converter folder
Binary_BCD_Converter U2(Q, ones, tens, hundreds);
BCD_Seven_Segment U7(ones, seg);
BCD_Seven_Segment U7(tens, seg);
BCD_Seven_Segment U7(hundreds, seg);
endmodule 
