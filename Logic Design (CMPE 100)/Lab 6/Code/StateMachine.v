`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 05/16/2019 07:01:38 AM
// Design Name: 
// Module Name: StateMachine
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


module StateMachine(
    input clk,
    input LS,
    input RS,
    output TurkeyRight,
    output TurkeyLeft
    );
    
    wire [6:0] PS, NS;
    
    StateMachine_Logic logic (.LS(LS), .RS(RS), .PS(PS), .NS(NS), .TurkeyRight(TurkeyRight), .TurkeyLeft(TurkeyLeft));
            
    FDRE #(.INIT(1'b1) ) ff1 (.C(clk), .CE(1'b1), .D(NS[0]), .Q(PS[0]));
    FDRE #(.INIT(1'b0) ) ff2[5:0] (.C({6{clk}}), .CE({6{1'b1}}), .D(NS[6:1]), .Q(PS[6:1]));
    
endmodule
