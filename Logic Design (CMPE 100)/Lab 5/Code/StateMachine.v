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


module StateMachine(
    input clk,
    input Go,
    input Stop,
    input FourSecs,
    input TwoSecs,
    input Match,
    output LoadTarget,
    output ResetTimer,
    output LoadNumbers,
    output SHR,
    output SHL,
    output FlashBoth,
    output FlashAlt
    );
    
    wire [2:0] PS, NS;
    
    StateMachine_Logic logic (.Go(Go), .Stop(Stop), .FourSecs(FourSecs), .TwoSecs(TwoSecs), .Match(Match), .PS(PS), .NS(NS),
        .LoadTarget(LoadTarget), .ResetTimer(ResetTimer), .LoadNumbers(LoadNumbers), .SHR(SHR), .SHL(SHL), .FlashBoth(FlashBoth), .FlashAlt(FlashAlt));
        
    FDRE #(.INIT(1'b1) ) ff1 (.C(clk), .CE(1'b1), .D(NS[0]), .Q(PS[0]));
    FDRE #(.INIT(1'b0) ) ff2[1:0] (.C({2{clk}}), .CE({2{1'b1}}), .D(NS[2:1]), .Q(PS[2:1]));
        
    
endmodule
