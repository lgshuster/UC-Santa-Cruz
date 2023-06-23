`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 06/01/2019 06:56:42 PM
// Design Name: 
// Module Name: EdgeDetector
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


module EdgeDetector(
    input clk,
    input btn,
    output z
    );
    
    wire [1:0] Q;
    
    FDRE #(.INIT(1'b0) ) flipflop0 (.C(clk), .R(1'b0), .CE(1'b1), .D(btn), .Q(Q[0]));
    FDRE #(.INIT(1'b0) ) flipflop1 (.C(clk), .R(1'b0), .CE(1'b1), .D(Q[0]), .Q(Q[1]));

    assign z = ~Q[1] & Q[0];
endmodule
