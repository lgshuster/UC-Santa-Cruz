`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 04/18/2019 03:29:15 AM
// Design Name: 
// Module Name: full_adder
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

module all_mux(
    );
endmodule


module m4_1e(
    input [3:0] in,
    input [1:0] sel,
    input e,
    output o
    );
    
    wire out3,out2,out1,out0;

    assign out3 = in[3] & ~sel[1] & ~sel[0];
    assign out2 = in[2] & ~sel[1] & sel[0];
    assign out1 = in[1] & sel[1] & ~sel[0];
    assign out0 = in[0] & sel[1] & sel[0];

    assign o = (out3 | out2 | out1 | out0) & e;
endmodule

module m8_1e(
    input [7:0] in,
    input [2:0] sel,
    input e,
    output o
    );
 
    wire out7,out6,out5,out4,out3,out2,out1,out0;
    
    assign out7 = in[7] & ~sel[2] & ~sel[1] & ~sel[0];
    assign out6 = in[6] & ~sel[2] & ~sel[1] & sel[0];
    assign out5 = in[5] & ~sel[2] & sel[1] & ~sel[0];
    assign out4 = in[4] & ~sel[2] & sel[1] & sel[0];
    assign out3 = in[3] & sel[2] & ~sel[1] & ~sel[0];
    assign out2 = in[2] & sel[2] & ~sel[1] & sel[0];
    assign out1 = in[1] & sel[2] & sel[1] & ~sel[0];
    assign out0 = in[0] & sel[2] & sel[1] & sel[0];
    
    assign o = (out7 | out6 | out5 | out4 | out3 | out2 | out1 | out0) & e;
endmodule

module m2_1x8(
    input [7:0] in0,
    input [7:0] in1,
    input s,
    output [7:0] o
    );
    
    wire [7:0] out0,out1;
    
    assign out0 = in0 & {8{~s}};
    assign out1 = in1 & {8{s}};
    
    assign o = out0 | out1;
endmodule