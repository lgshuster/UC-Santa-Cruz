`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 06/03/2019 05:55:05 AM
// Design Name: 
// Module Name: StateMachineHive
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


module StateMachineHive(
    input clk,
    input Go,
    input FourSecs,
    input EightSecs,
    input allCaught,
    input outOfTime,
    output notInPlay,
    output loadHive,
    output startHive,
    output canMoveHive,
    output releaseBees,
    output canCaptureBees,
    output flashBorders,
    output flashCount,
    output flashAmount,
    output startTimer,
    output resetTimer,
    output stopTimer,
    output reset
    );
    
    wire [5:0] PS, NS;
        
    StateMachineHive_Logic hiveLogic (.Go(Go), .FourSecs(FourSecs), .EightSecs(EightSecs), .allCaught(allCaught), .outOfTime(outOfTime),  .notInPlay(notInPlay), .PS(PS), .NS(NS), .loadHive(loadHive), .startHive(startHive), .canMoveHive(canMoveHive), .releaseBees(releaseBees), .canCaptureBees(canCaptureBees), .flashBorders(flashBorders), .flashCount(flashCount), .flashAmount(flashAmount), .startTimer(startTimer), .resetTimer(resetTimer), .stopTimer(stopTimer), .reset(reset));
        
    FDRE #(.INIT(1'b1) ) ff1 (.C(clk), .CE(1'b1), .D(NS[0]), .Q(PS[0]));
    FDRE #(.INIT(1'b0) ) ff2[4:0] (.C({5{clk}}), .CE({5{1'b1}}), .D(NS[5:1]), .Q(PS[5:1]));
        
endmodule
