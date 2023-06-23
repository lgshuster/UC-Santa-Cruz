`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 06/01/2019 12:12:15 AM
// Design Name: 
// Module Name: countUD12L
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


module countUD12L(
    input clk,
    input reset,
    input Up,
    input Dw,
    input LD,
    input [12:0] Din,
    output [12:0] Q
    );
    
    wire UTC1, DTC1, UTC2, DTC2;
    
    countUD4L counter1 (.clk(clk), .reset(reset), .Up(Up), .Dw(Dw), .LD(LD), .Din(Din[3:0]), .Q(Q[3:0]), .UTC(UTC1), .DTC(DTC1));
    countUD4L counter2 (.clk(clk), .reset(reset), .Up(Up & UTC1), .Dw(Dw & DTC1), .LD(LD), .Din(Din[7:4]), .Q(Q[7:4]), .UTC(UTC2), .DTC(DTC2));
    countUD4L counter3 (.clk(clk), .reset(reset), .Up(Up & UTC1 & UTC2), .Dw(Dw & DTC1 & DTC2), .LD(LD), .Din(Din[11:8]), .Q(Q[11:8]));
    
    
    
endmodule
