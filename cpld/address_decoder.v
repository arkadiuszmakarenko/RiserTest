`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date:    21:14:38 10/08/2020 
// Design Name: 
// Module Name:    address_decoder 
// Project Name: 
// Target Devices: 
// Tool versions: 
// Description: 
//
// Dependencies: 
//
// Revision: 
// Revision 0.01 - File Created
// Additional Comments: 
//
//////////////////////////////////////////////////////////////////////////////////
module address_decoder(
	 input clk,
    input [23:0] A,
	 input RW,
	 output BUSINT,
	 output INTSIG1,
    output INTSIG2,
    output INTSIG3,
	 output INTSIG4,
    output INTSIG5,
	 output INTSIG6,
	 output INTSIG7,
	 input INTSIG8,
	 inout [7:0] D,
	 output DSACK0,
	 output DSACK1,
	 input PUNT_IN,
	 output PUNT_OUT,
	 input DS,
	 input AS,
	 output SPI2_MISO,
	 input SPI2_MOSI,
	 input SPI2_CLK
    );

assign INTSIG2 = A[23:16]==8'b1101_1100; //RTC registers at $DC0000 - $DCFFFF ,
//assign DSACK0 = (INTSIG2&&RW&&INTSIG8)?0 : 1'bZ;
//assign DSACK1 = (INTSIG2&&RW&&INTSIG8)?1 : 1'bZ;
assign INTSIG3 = A[3];
assign INTSIG5 = A[5];

endmodule
