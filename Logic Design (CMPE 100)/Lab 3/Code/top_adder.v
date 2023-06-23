`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 04/18/2019 03:33:00 AM
// Design Name: 
// Module Name: top_adder
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

module top_adder(
    input c_in,
    input [7:0] A,
    input [7:0] B,
    output [3:0] S
    );
    
    wire c0, c1, c2, c3, c4, c5, c6; // carry outs for adders
    
    // Full adder takes in two inputs and a carry in and outputs a sum and a carry out
    full_adder adder1 (.c_in(c_in), .a(A[0]), .b(B[0]), .c_out(c0), .s(S[0]));
    full_adder adder2 (.c_in(c0), .a(A[1]), .b(B[1]), .c_out(c1), .s(S[1]));
    full_adder adder3 (.c_in(c1), .a(A[2]), .b(B[2]), .c_out(c2), .s(S[2]));
    full_adder adder4 (.c_in(c2), .a(A[3]), .b(B[3]), .s(S[3]));

endmodule
