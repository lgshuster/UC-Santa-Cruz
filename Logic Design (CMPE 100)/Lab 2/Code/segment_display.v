`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 04/11/2019 02:09:37 AM
// Design Name: 
// Module Name: segment_display
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


module segment_display(
    input n0,
    input n1,
    input n2,
    input n3,
    output CA,
    output CB,
    output CC,
    output CD,
    output CE,
    output CF,
    output CG
    );
    
    assign CA = ((~n3 & ~n2 & ~n1 & n0) | (~n3 & n2 & ~n1 & ~n0) | (n3 & ~n2 & n1 & n0) | (n3 & n2 & ~n1 & n0));
    assign CB = ((~n3 & n2 & ~n1 & n0) | (~n3 & n2 & n1 & ~n0) | (n3 & ~n2 & n1 & n0) | (n3 & n2 & ~n1 & ~n0) | (n3 & n2 & n1 & ~n0) | (n3 & n2 & n1 & n0));
    assign CC = ((~n3 & ~n2 & n1 & ~n0) | (n3 & n2 & ~n1 & ~n0) | (n3 & n2 & n1 & ~n0) | (n3 & n2 & n1 & n0));
    assign CD = ((~n3 & ~n2 & ~n1 & n0) | (~n3 & n2 & ~n1 & ~n0) | (~n3 & n2 & n1 & n0) | (n3 & ~n2 & n1 & ~n0) | (n3 & n2 & n1 & n0));
    assign CE = ((~n3 & ~n2 & ~n1 & n0) | (~n3 & ~n2 & n1 & n0) | (~n3 & n2 & ~n1 & ~n0) | (~n3 & n2 & ~n1 & n0) | (~n3 & n2 & n1 & n0) | (n3 & ~n2 & ~n1 & n0));
    assign CF = ((~n3 & ~n2 & ~n1 & n0) | (~n3 & ~n2 & n1 & ~n0) | (~n3 & ~n2 & n1 & n0) | (~n3 & n2 & n1 & n0) | (n3 & n2 & ~n1 & n0));
    assign CG = ((~n3 & ~n2 & ~n1 & ~n0) | (~n3 & ~n2 & ~n1 & n0) | (~n3 & n2 & n1 & n0) | (n3 & n2 & ~n1 & ~n0));
    
endmodule
