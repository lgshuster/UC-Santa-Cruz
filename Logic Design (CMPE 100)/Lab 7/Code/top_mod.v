`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 06/01/2019 12:34:05 AM
// Design Name: 
// Module Name: top_mod
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


module top_mod(
    input clkin,
    input btnC,
    input btnL,
    input btnR,
    input btnU,
    input btnD,
    input [15:0] sw,
    output [3:0] vgaRed, 
    output [3:0] vgaBlue,
    output [3:0] vgaGreen,
    output Hsync,
    output Vsync,
    output dp,
    output [3:0] an,
    output [6:0] seg,
    output [15:0] led
    );
    wire clk, digsel;
    wire [12:0] xPixel, yPixel;
    wire frame, loadHive, startHive, releaseBees;
    wire [12:0] xHiveCenter, yHiveCenter;
    wire wallBorders, hiveInterior, hiveBorders, allBees;
    wire canMoveHive, canCaptureBees, flashBorders, flashCount, flashAmount;
    wire startTimer, stopTimer, countDown, resetTimer, reset, QuarterSec, OneSec, FourSecs,  EightSecs, allCaught, outOfTime, notInPlay;
    wire [7:0] timeLeft;
    wire [3:0] beesCaught;
    wire [15:0] AllNums;
    wire [3:0] RingCounter_out;
    wire [3:0] Selector_out;

            
    assign wallBorders = (((xPixel >= 0) & (xPixel <= 639)) & (((yPixel >= 0) & (yPixel <= 7)) | ((yPixel >= 472) & (yPixel <= 479))))
                            | (((yPixel >= 0) & (yPixel <= 479)) & (((xPixel >= 0) & (xPixel <= 7)) | ((xPixel >= 632) & (xPixel <= 639))));
                           
                           
    assign vgaRed = {4{wallBorders | (hiveInterior & ~allBees)}};
    assign vgaBlue = {4{hiveBorders & ((flashBorders & QuarterSec) | (~flashBorders))}};   
    assign vgaGreen = {4{allBees}};  
    
    assign AllNums[15:12] = beesCaught;
    assign AllNums[7:0] = timeLeft;
    
    assign allCaught = ~notInPlay & (beesCaught == 4'd8);
    assign outOfTime = (timeLeft == 8'd0);
    
    assign an[3] = ~((RingCounter_out[3] & ((flashAmount & QuarterSec) | (~flashAmount))));
    assign an[2] = 1'b1;
    assign an[1] = ~((RingCounter_out[1] & (canCaptureBees | (stopTimer & (~flashCount | (flashCount & QuarterSec))))));
    assign an[0] = ~((RingCounter_out[0] & (canCaptureBees | (stopTimer & ~flashCount) | (stopTimer & (flashCount & QuarterSec)))));
    
    
    assign dp = 1'b1;
                            
    lab7_clks not_so_slow (.clkin(clkin), .greset(sw[0]), .clk(clk), .digsel(digsel));

    PixelSync sync (.clk(clk), .xPixel(xPixel), .yPixel(yPixel), .Hsync(Hsync), .Vsync(Vsync));
    
    EdgeDetector pixelPerFrame (.clk(clk), .btn(Vsync), .z(frame));
    EdgeDetector oneSecondDetector (.clk(clk), .btn(QuarterSec), .z(countDown));
        
    Time_Counter upTimer (.clk(clk), .enable(frame), .resetTimer(resetTimer), .QuarterSec(QuarterSec), .OneSec(OneSec), .FourSecs(FourSecs), .EightSecs(EightSecs));
    CountDownTimer downTimer (.clk(clk), .reset(reset), .enable(countDown & ~stopTimer), .startTimer(startTimer), .Din(sw[15:8]), .timeLeft(timeLeft));
    
    StateMachineHive hiveMachine (.clk(clk), .Go(btnC), .FourSecs(FourSecs), .EightSecs(EightSecs), 
                                    .allCaught(allCaught), .outOfTime(outOfTime), .notInPlay(notInPlay), 
                                    .loadHive(loadHive), .startHive(startHive), .canMoveHive(canMoveHive), .releaseBees(releaseBees), 
                                    .canCaptureBees(canCaptureBees), .flashBorders(flashBorders), .flashCount(flashCount), .flashAmount(flashAmount), .startTimer(startTimer), .resetTimer(resetTimer), .stopTimer(stopTimer), .reset(reset));
        
    BeeHive hive (.clk(clk), .xPixel(xPixel), .yPixel(yPixel), .frame(frame), 
                    .loadHive(loadHive), .startHive(startHive), .canMoveHive(canMoveHive),
                    .left(btnL), .right(btnR), .up(btnU), .down(btnD), 
                    .xHiveCenter(xHiveCenter), .yHiveCenter(yHiveCenter), 
                    .hiveInterior(hiveInterior), .hiveBorders(hiveBorders));
                    
    AllBees bees (.clk(clk), .xPixel(xPixel), .yPixel(yPixel), .xHiveCenter(xHiveCenter), .yHiveCenter(yHiveCenter), .reset(reset), .frame(frame), .notInPlay(notInPlay), .releaseBees(releaseBees),  .canCaptureBees(canCaptureBees), .beesCaught(beesCaught), .allBees(allBees));
    
    ring_counter RingCounter (.clk(clk), .Advance(digsel), .Q(RingCounter_out));
    selector Selector (.sel(RingCounter_out), .N(AllNums), .H(Selector_out));
    hex7seg display (.n(Selector_out), .e(1'b1), .seg(seg));
    
endmodule
