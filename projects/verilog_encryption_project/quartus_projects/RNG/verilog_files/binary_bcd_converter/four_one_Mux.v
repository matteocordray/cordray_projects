




module four_one_Mux(
	input wire [3:0] A,
	input wire [3:0] B,
	input wire [3:0] C,
	input wire [3:0] D,
	input wire [1:0] SS,
	output reg [3:0] Y
	); 
	
	always @(A or B or C or D or SS)
		begin
		case(SS)
		2'b00: Y = A;
		2'b01: Y = B;
		2'b10: Y = C;
		2'b11: Y = D;
		endcase
		end
		
endmodule 
		