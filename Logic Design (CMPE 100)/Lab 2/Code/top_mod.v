`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 04/11/2019 03:29:11 AM
// Design Name: 
// Module Name: top_mod
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


module top_mod(
    input c_in,
    input a0,
    input a1,
    input a2,
    input b0,
    input b1,
    input b2,
    output DP,
    output AN0,
    output AN1,
    output AN2,
    output AN3,
    output CA,
    output CB,
    output CC,
    output CD,
    output CE,
    output CF,
    output CG,

    output tmp_n0,
    output tmp_n1,
    output tmp_n2,
    output tmp_n3
    );
    
    wire n3, n2, n1, n0, DP, AN0, AN1, AN2, AN3;
    assign DP = 1;
    assign AN0 = 0;
    assign AN1 = 1;
    assign AN2 = 1;
    assign AN3 = 1;
    assign tmp_n3 = n3;
    assign tmp_n2 = n2;
    assign tmp_n1 = n1;
    assign tmp_n0 = n0;

    top_adder adder(.c_in(c_in), .a0(a0), .a1(a1), .a2(a2), .b0(b0), .b1(b1), .b2(b2), .s0(n0), .s1(n1), .s2(n2), .s3(n3));
    segment_display display(.n0(n0), .n1(n1), .n2(n2), .n3(n3), .CA(CA), .CB(CB), .CC(CC), .CD(CD), .CE(CE), .CF(CF), .CG(CG));
    
endmodule
