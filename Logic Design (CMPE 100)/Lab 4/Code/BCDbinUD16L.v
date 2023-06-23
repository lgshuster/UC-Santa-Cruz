`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 05/02/2019 03:17:21 AM
// Design Name: 
// Module Name: BCDbinUD16L
// Project Name: 
// Target Devices: 
// Tool Versions: 
// Description: 
// 
// Dependencies: 
// 
// Revision:
// Revision 0.01 - File Created
// Additional Comments:
// 
//////////////////////////////////////////////////////////////////////////////////


module BCDbinUD16L(
    input clk,
    input Up,
    input Dw,
    input LD,
    input [15:0] Din,
    output [15:0] Q,
    output UTC,
    output DTC
//    output UTC1,
//    output UTC2,
//    output UTC3,
//    output UTC4,
//    output DTC1,
//    output DTC2,
//    output DTC3,
//    output DTC4,
//    output LD1,
//    output LD2,
//    output LD3,
//    output LD4
    );
    
    wire UTC1, UTC2, UTC3, UTC4, DTC1, DTC2, DTC3, DTC4;
//  wire LD1, LD2, LD3, LD4;

//    assign LD1 = LD;
//    assign LD2 = ~LD1 & ~UTC1 & ~DTC1;
//    assign LD3 = ~LD2 & ~UTC2 & ~DTC2;
//    assign LD4 = ~LD3 & ~UTC3 & ~DTC3;
    
    countUD4L counter1 (.clk(clk), .Up(Up), .Dw(Dw), .LD(LD), .Din(Din[3:0]), .Q(Q[3:0]), .UTC(UTC1), .DTC(DTC1));
    countUD4L counter2 (.clk(clk), .Up(Up & UTC1), .Dw(Dw & DTC1), .LD(LD), .Din(Din[7:4]), .Q(Q[7:4]), .UTC(UTC2), .DTC(DTC2));
    BCD_UD4L counter3 (.clk(clk), .Up(Up & UTC1 & UTC2), .Dw(Dw & DTC1 & DTC2), .LD(LD), .Din(Din[11:8]), .Q(Q[11:8]), .UTC(UTC3), .DTC(DTC3));
    BCD_UD4L counter4 (.clk(clk), .Up(Up & UTC1 & UTC2 & UTC3), .Dw(Dw & DTC1 & DTC2 & DTC3), .LD(LD), .Din(Din[15:12]), .Q(Q[15:12]), .UTC(UTC4), .DTC(DTC4));    
    
    assign UTC = UTC1 & UTC2 & UTC3 & UTC4;
    assign DTC = DTC1 & DTC2 & DTC3 & DTC4;
    
endmodule
