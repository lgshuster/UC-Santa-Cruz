`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 04/16/2019 04:48:12 PM
// Design Name: 
// Module Name: AddSub8
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

module AddSub8(
  input c_in,
  input [7:0] A,
  input [7:0] B,
  output [7:0] S
  );
  
  
  wire c0, c1, c2, c3, c4, c5, c6; // carry outs for adders
  
  // Full adder takes in two inputs and a carry in and outputs a sum and a carry out
  full_adder adder1 (.c_in(c_in), .a(A[0]), .b(B[0]), .c_out(c0), .s(S[0]));
  full_adder adder2 (.c_in(c0), .a(A[1]), .b(B[1]), .c_out(c1), .s(S[1]));
  full_adder adder3 (.c_in(c1), .a(A[2]), .b(B[2]), .c_out(c2), .s(S[2]));
  full_adder adder4 (.c_in(c2), .a(A[3]), .b(B[3]), .c_out(c3), .s(S[3]));
  full_adder adder5 (.c_in(c3), .a(A[4]), .b(B[4]), .c_out(c4), .s(S[4]));
  full_adder adder6 (.c_in(c4), .a(A[5]), .b(B[5]), .c_out(c5), .s(S[5]));
  full_adder adder7 (.c_in(c5), .a(A[6]), .b(B[6]), .c_out(c6), .s(S[6]));
  full_adder adder8 (.c_in(c6), .a(A[7]), .b(B[7]), .s(S[7]));
  

endmodule
