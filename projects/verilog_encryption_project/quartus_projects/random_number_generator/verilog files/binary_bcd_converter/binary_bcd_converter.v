





module Binary_BCD_Converter(
	input [7:0] A,
	output [3:0] ones,
	output [3:0] tens,
	output [1:0] hundreds
	);
	
	wire [3:0] c1, c2, c3, c4, c5, c6, c7; // declare the data lines coming out of each shift_add3_module
	wire [3:0] d1, d2, d3, d4, d5, d6, d7; // declare the data lines going into each shift_add3_module
	
	
	// follow the block diagram here:
	assign d1={1'b0, A[7:5]};
	assign d2={c1[2:0],A[4]};
	assign d3={c2[2:0],A[3]};
	assign d4={c3[2:0],A[2]};
	assign d5={c4[2:0],A[1]};
	assign d6={1'b0,c1[3], c2[3], c3[3]};
	assign d7={c6[2:0],c4[3]};
	
	// initiate the shift_add_algorithm
	
	Shift_Add3_algorithm U1(d1,c1);
	Shift_Add3_algorithm U2(d2,c2);
	Shift_Add3_algorithm U3(d3,c3);
	Shift_Add3_algorithm U4(d4,c4);
	Shift_Add3_algorithm U5(d5,c5);
	Shift_Add3_algorithm U6(d6,c6);
	Shift_Add3_algorithm U7(d7,c7);
	
	assign units = {c5[2:0], A[0]}; // four bits that will make-up ones
	assign tens = {c5[3], c7[2:0]}; // four bits that will make-up tens
	assign hundreds = {c7[3], c6[3]}; // four bits that will make-up hundreds
	
	
endmodule 