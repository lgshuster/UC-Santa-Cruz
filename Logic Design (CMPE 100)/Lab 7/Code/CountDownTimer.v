`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 06/04/2019 08:37:53 AM
// Design Name: 
// Module Name: CountDownTimer
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


module CountDownTimer(
    input clk,
    input reset,
    input enable,
    input startTimer,
    input [7:0] Din,
    output [7:0] timeLeft
    );
    wire DTC;        
    
    countUD4L counter1 (.clk(clk), .reset(reset), .Up(1'b0), .Dw(enable), .LD(startTimer), .Din(Din[3:0]), .Q(timeLeft[3:0]), .DTC(DTC));
    countUD4L counter2 (.clk(clk), .reset(reset), .Up(1'b0), .Dw(enable & DTC), .LD(startTimer), .Din(Din[7:4]), .Q(timeLeft[7:4]));
endmodule
