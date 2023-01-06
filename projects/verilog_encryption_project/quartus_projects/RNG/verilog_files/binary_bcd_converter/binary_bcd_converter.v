





//module Binary_BCD_Converter(
//	input [7:0] A,
//	output reg [3:0] ones,
//	output reg [3:0] tens,
//	output reg [1:0] hundreds
//	);
//	
//	wire [3:0] c1, c2, c3, c4, c5, c6, c7; // declare the data lines coming out of each shift_add3_module
//	wire [3:0] d1, d2, d3, d4, d5, d6, d7; // declare the data lines going into each shift_add3_module
//	
//	
//	// follow the block diagram here:
//	assign d1={1'b0, A[7:5]};
//	assign d2={c1[2:0],A[4]};
//	assign d3={c2[2:0],A[3]};
//	assign d4={c3[2:0],A[2]};
//	assign d5={c4[2:0],A[1]};
//	assign d6={1'b0,c1[3], c2[3], c3[3]};
//	assign d7={c6[2:0],c4[3]}; 
//	
//	// initiate the shift_add_algorithm
//	
//	Shift_Add3_algorithm U1(d1,c1);
//	Shift_Add3_algorithm U2(d2,c2);
//	Shift_Add3_algorithm U3(d3,c3);
//	Shift_Add3_algorithm U4(d4,c4);
//	Shift_Add3_algorithm U5(d5,c5);
//	Shift_Add3_algorithm U6(d6,c6);
//	Shift_Add3_algorithm U7(d7,c7);
//	
//	assign ones = {c5[2:0], A[0]}; // four bits that will make-up ones
//	assign tens = {c5[3], c7[2:0]}; // four bits that will make-up tens
//	assign hundreds = {c7[3], c6[3]}; // four bits that will make-up hundreds
//	
//	
//endmodule


module Binary_BCD_Converter(
	input [9:0] A,
	output reg [3:0] Hundreds,
	output reg [3:0] Tens,
	output reg [3:0] Ones
	);
	
	
	integer i;
	always @ (A)
	begin
		// set 100's, 10's, and 1's to 0
		Hundreds = 4'd0;
		Tens = 4'd0;
		Ones = 4'd0;
		
		for (i=7; i>=0; i=i-1)
		begin
			// add 3 to columns >= 5
			if (Hundreds >= 5)
				Hundreds = Hundreds + 3;
			if (Tens >= 5)
				Tens = Tens + 3;
			if (Ones >= 5)
				Ones = Ones + 3;
			
			// shift left one
			Hundreds = Hundreds << 1;
			Hundreds[0] = Tens[3];
			Tens = Tens << 1;
			Tens[0] = Ones[3];
			Ones = Ones << 1;
			Ones[0] = A[i];
		end
	end

endmodule 
		
	