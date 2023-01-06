//
// Dependencies
//
// Revision: 0.01
// Additional comments:
//




module D_FF(
	
	input clk,reset,
	input D,
	output reg Q
	);
	
	always @ (negedge clk or negedge reset)
	begin
		if (reset==1'b0)
			Q <= 1'b0;
		else if (clk==1'b0)
			Q <= D;
		end
endmodule
	