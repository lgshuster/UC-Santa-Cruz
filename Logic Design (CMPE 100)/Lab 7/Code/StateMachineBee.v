`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 06/04/2019 01:29:50 AM
// Design Name: 
// Module Name: StateMachineBee
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


module StateMachineBee(
    input clk, 
    input reset,
    input releaseBee,
    input canCaptureBees,
    input isOverlap,
    output loadBee,
    output caught,
    output captured
    );
    wire [2:0] PS, NS;
    
    StateMachineBee_Logic beeLogic (.reset(reset), .releaseBee(releaseBee), .canCaptureBees(canCaptureBees), .isOverlap(isOverlap), .PS(PS), .NS(NS), .loadBee(loadBee), .caught(caught), .captured(captured));
            
    FDRE #(.INIT(1'b1) ) ff1 (.C(clk), .CE(1'b1), .D(NS[0]), .Q(PS[0]));
    FDRE #(.INIT(1'b0) ) ff2 [1:0] (.C({2{clk}}), .CE({2{1'b1}}), .D(NS[2:1]), .Q(PS[2:1]));
            
endmodule
