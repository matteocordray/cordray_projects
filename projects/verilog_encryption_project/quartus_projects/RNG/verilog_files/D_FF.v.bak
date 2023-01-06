//
// Dependencies
//
// Revision: 0.01
// Additional comments:
//




module D_FF(
	input clk,reset,
	input load,D,
	output reg Q
		);
		
		
	
	always @ (posedge clk)
	begin
	if (reset)
	Q<=1'b0;
	else if (load)
	Q<=D;
	end
endmodule
	