`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 04/10/2019 11:15:20 PM
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
    input a0,
    input a1,
    input a2,
    input b0,
    input b1,
    input b2,
    output s0,
    output s1,
    output s2,
    output s3
    );
    
    wire c0, c1; // c1 and c2 are carry outs for adder1 and adder2
    
    // Full adder takes in two inputs and a carry in and outputs a sum and a carry out
    full_adder adder1 (.c_in(c_in), .a(a0), .b(b0), .c_out(c0), .s(s0));
    full_adder adder2 (.c_in(c0), .a(a1), .b(b1), .c_out(c1), .s(s1));
    full_adder adder3 (.c_in(c1), .a(a2), .b(b2), .c_out(s3), .s(s2));
    

endmodule
