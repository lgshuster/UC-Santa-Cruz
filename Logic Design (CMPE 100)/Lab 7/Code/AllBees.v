`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 06/03/2019 03:53:00 AM
// Design Name: 
// Module Name: AllBees
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


module AllBees(
    input clk,
    input [12:0] xPixel,
    input [12:0] yPixel,
    input [12:0] xHiveCenter,
    input [12:0] yHiveCenter,
    input reset,
    input frame,
    input notInPlay,
    input releaseBees,
    input startBees,
    input canCaptureBees,
    output [3:0] beesCaught,
    output allBees
    );
    wire bee1Pos, bee2Pos, bee3Pos, bee4Pos, bee5Pos, bee6Pos, bee7Pos, bee8Pos;
    wire caught1, caught2, caught3, caught4, caught5, caught6, caught7, caught8;
    wire [15:0] randomNumber;
    
    countUD4L beesCaughtCounter (.clk(clk), .reset(releaseBees), .Up(caught1 | caught2 | caught3 | caught4 | caught5 | caught6 | caught7 | caught8), .Dw(1'b0), .LD(notInPlay), .Din(8'd8), .Q(beesCaught));
    
    LFSR randomNumGen (.clk(clk), .rnd(randomNumber));
    
    Bee bee1 (.clk(clk), .reset(reset), .xPixel(xPixel), .yPixel(yPixel), .xHiveCenter(xHiveCenter), .yHiveCenter(yHiveCenter), .xHivePos(xHiveCenter), .yHivePos(yHiveCenter), 
                .frame(frame),  .releaseBee(releaseBees), 
                .canCaptureBees(canCaptureBees), .randomNumber(randomNumber[1:0]), .caught(caught1), .bee(bee1Pos));
                
    Bee bee2 (.clk(clk), .reset(reset), .xPixel(xPixel), .yPixel(yPixel), .xHiveCenter(xHiveCenter), .yHiveCenter(yHiveCenter), .xHivePos(xHiveCenter + 13'd10), .yHivePos(yHiveCenter + 13'd5), 
                .frame(frame),  .releaseBee(releaseBees), 
                .canCaptureBees(canCaptureBees), .randomNumber(randomNumber[3:2]), .caught(caught2), .bee(bee2Pos));
                
    Bee bee3 (.clk(clk), .reset(reset), .xPixel(xPixel), .yPixel(yPixel), .xHiveCenter(xHiveCenter), .yHiveCenter(yHiveCenter), .xHivePos(xHiveCenter + 13'd25), .yHivePos(yHiveCenter + 13'd25), 
                .frame(frame),  .releaseBee(releaseBees), 
                 .canCaptureBees(canCaptureBees), .randomNumber(randomNumber[5:4]), .caught(caught3), .bee(bee3Pos));
    
    Bee bee4 (.clk(clk), .reset(reset), .xPixel(xPixel), .yPixel(yPixel), .xHiveCenter(xHiveCenter), .yHiveCenter(yHiveCenter), .xHivePos(xHiveCenter - 13'd10), .yHivePos(yHiveCenter + 13'd5), 
               .frame(frame),   .releaseBee(releaseBees), 
                .canCaptureBees(canCaptureBees), .randomNumber(randomNumber[7:6]), .caught(caught4), .bee(bee4Pos));
                               
    Bee bee5 (.clk(clk), .reset(reset), .xPixel(xPixel), .yPixel(yPixel), .xHiveCenter(xHiveCenter), .yHiveCenter(yHiveCenter), .xHivePos(xHiveCenter - 13'd25), .yHivePos(yHiveCenter + 13'd25), 
               .frame(frame),  .releaseBee(releaseBees), 
                .canCaptureBees(canCaptureBees), .randomNumber(randomNumber[9:8]), .caught(caught5), .bee(bee5Pos));
               
    Bee bee6 (.clk(clk), .reset(reset), .xPixel(xPixel), .yPixel(yPixel), .xHiveCenter(xHiveCenter), .yHiveCenter(yHiveCenter), .xHivePos(xHiveCenter + 13'd10), .yHivePos(yHiveCenter - 13'd25), 
                .frame(frame),   .releaseBee(releaseBees), 
                .canCaptureBees(canCaptureBees), .randomNumber(randomNumber[11:10]), .caught(caught6), .bee(bee6Pos));
               
    Bee bee7 (.clk(clk), .reset(reset), .xPixel(xPixel), .yPixel(yPixel), .xHiveCenter(xHiveCenter), .yHiveCenter(yHiveCenter), .xHivePos(xHiveCenter + 13'd25), .yHivePos(yHiveCenter - 13'd25), 
              .frame(frame), .releaseBee(releaseBees), 
              .canCaptureBees(canCaptureBees), .randomNumber(randomNumber[13:12]), .caught(caught7), .bee(bee7Pos));
                          
    Bee bee8 (.clk(clk), .reset(reset), .xPixel(xPixel), .yPixel(yPixel), .xHiveCenter(xHiveCenter), .yHiveCenter(yHiveCenter), .xHivePos(xHiveCenter - 13'd20), .yHivePos(yHiveCenter - 13'd20), 
              .frame(frame),  .releaseBee(releaseBees), 
             .canCaptureBees(canCaptureBees), .randomNumber(randomNumber[15:14]), .caught(caught8), .bee(bee8Pos));

                
    

    assign allBees = bee1Pos | bee2Pos | bee3Pos | bee4Pos | bee5Pos | bee6Pos | bee7Pos | bee8Pos;
endmodule
