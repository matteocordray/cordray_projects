// hextoSevenSegment_testCircuit

module hexToSevenSegment_test
(
	// Inputs
	input	wire[7:0] SW,
	
	// Outputs
	output [6:0] HEX0, HEX1
	
);

hexModule hexToSevenSegment0 (
					.hexNumber(SW[3:0]), .sevenSegmentActiveLow(HEX0));
					
hexModule hexToSevenSegment1 (
					.hexNumber(SW[7:4]), .sevenSegmentActiveLow(HEX1));
					
endmodule