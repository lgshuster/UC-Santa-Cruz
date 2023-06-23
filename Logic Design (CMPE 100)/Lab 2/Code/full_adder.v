`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 04/10/2019 11:07:25 PM
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


module full_adder(
    input a,
    input b,
    input c_in,
    output c_out,
    output s
    );
    
    assign c_out = ((~a & b & c_in) | (a & ~b & c_in) | (a & b & ~c_in) | (a & b & c_in));
    assign s = ((~a & ~b & c_in) | (~a & b & ~c_in) | (a & ~b & ~c_in) | (a & b & c_in));
    
endmodule
