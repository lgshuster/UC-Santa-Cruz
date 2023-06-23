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


module Target(
    input clk,
    input [7:0] D,
    input enable,
    output [3:0] Q
    );
    wire [7:0] dummy;
    
    
    FDRE #(.INIT(1'b0) ) ff[7:0] (.C({8{clk}}), .R({8{1'b0}}), .CE({8{enable}}), .D(D), .Q(dummy));

    assign Q[3] = dummy[7];
    assign Q[2] = dummy[6];
    assign Q[1] = dummy[5];
    assign Q[0] = dummy[0];
    
    
endmodule
