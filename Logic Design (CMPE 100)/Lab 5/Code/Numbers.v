`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 05/08/2019 03:24:22 AM
// Design Name: 
// Module Name: Numbers
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


module Numbers(
    input clk,
    input [7:0] D,
    input enable,
    output [7:0] Q
    );    
    
    FDRE #(.INIT(1'b0) ) ff[7:0] (.C({8{clk}}), .R({8{1'b0}}), .CE({8{enable}}), .D(D), .Q(Q));

    
    
    
endmodule
