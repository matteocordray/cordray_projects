


module two_bit_counter(
	input clk, // slow clock
	output [1:0] Q
	);
	reg [1:0] temp = 0; // initially counter is set to 0
	
	always @(posedge clk) // when positive edge of the clock arrives, counter goes up by 1
	begin
		temp=temp+1;
	end
	
	assign Q = temp; // values of the counter gets stored in Q
	
endmodule 