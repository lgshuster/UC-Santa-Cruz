`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 04/18/2019 12:45:10 PM
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
    
    wire e;
    wire [3:0] mux1_in = {1'b0, c_in, c_in, 1'b1};
    wire [3:0] mux2_in = {c_in, ~c_in, ~c_in, c_in};
    wire [1:0] mux_sel = {a, b};
    
    assign e = 1;
    
    m4_1e mux1 (.in(mux1_in), .sel(mux_sel), .e(e), .o(c_out)); // mux for carry out
    m4_1e mux2 (.in(mux2_in), .sel(mux_sel), .e(e), .o(s)); // mux for sum
    
endmodule
