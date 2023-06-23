`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 05/02/2019 06:31:28 AM
// Design Name: 
// Module Name: ring_counter
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


module ring_counter(
    input clk,
    input Advance,
    output [3:0] Q
    );
    
    wire dummy;
        
    FDRE  #(.INIT(1'b0) ) ff1 (.C(clk), .R(1'b0), .CE(Advance), .D(Q[3]), .Q(Q[0]));
    FDRE  #(.INIT(1'b0) ) ff2 (.C(clk), .R(1'b0), .CE(Advance), .D(Q[2]), .Q(Q[3]));
    FDRE  #(.INIT(1'b0) ) ff3 (.C(clk), .R(1'b0), .CE(Advance), .D(Q[1]), .Q(Q[2]));
    FDRE  #(.INIT(1'b1) ) ff4 (.C(clk), .R(1'b0), .CE(Advance), .D(Q[0]), .Q(Q[1]));
        
endmodule
