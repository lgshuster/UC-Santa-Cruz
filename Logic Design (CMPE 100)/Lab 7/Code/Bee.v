`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 06/03/2019 03:53:12 AM
// Design Name: 
// Module Name: Bee
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


module Bee(
    input clk,
    input [12:0] xPixel,
    input [12:0] yPixel,
    input [12:0] xHiveCenter,
    input [12:0] yHiveCenter,
    input [12:0] xHivePos,
    input [12:0] yHivePos,
    input frame,
    input reset,
    input releaseBee,
    input canCaptureBees,
    input [1:0] randomNumber,
    output caught,
    output bee
    );
    wire [12:0] xBeePos, yBeePos, xBeeStart, yBeeStart, xBeeLeft, xBeeRight, yBeeUp, yBeeDown, xHiveLeft, xHiveRight, yHiveUp, yHiveDown;
    wire upLeftDirection, upRightDirection, downLeftDirection, downRightDirection;
    wire isLeftBorder, isRightBorder, isUpBorder, isDownBorder; 
    wire loadBee, isOverlap, captured, keepLeft, keepRight, keepUp, keepDown;

    assign xBeeStart = xHivePos;
    assign yBeeStart = yHivePos;

    assign xBeeLeft = xBeePos - 13'd3;
    assign xBeeRight = xBeePos + 13'd3;
    assign yBeeUp = yBeePos - 13'd3;
    assign yBeeDown = yBeePos + 13'd3;
    
    assign xHiveLeft = xHiveCenter - 13'd47;
    assign xHiveRight = xHiveCenter + 13'd47;
    assign yHiveUp = yHiveCenter - 13'd47;
    assign yHiveDown = yHiveCenter + 13'd47;
    
    assign isOverlap = ~((xBeeLeft > xHiveRight) | (xBeeRight < xHiveLeft) | (yBeeUp > yHiveDown) | (yBeeDown < yHiveUp));
        
    assign isLeftBorder = ((xBeeLeft - 13'd1) == 13'd7);
    assign isRightBorder = ((xBeeRight + 13'd1) == 13'd632);
    assign isUpBorder = ((yBeeUp - 13'd1) == 13'd7);
    assign isDownBorder = ((yBeeDown + 13'd1) == 13'd472);
    
    assign upLeftDirection = ~randomNumber[1] & ~randomNumber[0];
    assign upRightDirection = ~randomNumber[1] & randomNumber[0];
    assign downLeftDirection = randomNumber[1] & ~randomNumber[0];
    assign downRightDirection = randomNumber[1] & randomNumber[0];   
    
    StateMachineBee beeMachine (.clk(clk), .reset(reset), .releaseBee(releaseBee), .loadBee(loadBee), .canCaptureBees(canCaptureBees), .isOverlap(isOverlap), .caught(caught), .captured(captured));
     
    FDRE #(.INIT(1'b0) ) ff1 (.C(clk), .R(loadBee), .CE(((upLeftDirection | downLeftDirection) & releaseBee) | ((isLeftBorder | isRightBorder) & ~captured)), .D(releaseBee | isRightBorder), .Q(keepLeft));
    FDRE #(.INIT(1'b0) ) ff2 (.C(clk), .R(loadBee), .CE(((upRightDirection | downRightDirection) & releaseBee) | ((isLeftBorder | isRightBorder) & ~captured)), .D(releaseBee | isLeftBorder), .Q(keepRight));
    FDRE #(.INIT(1'b0) ) ff3 (.C(clk), .R(loadBee), .CE(((upLeftDirection | upRightDirection) & releaseBee) | ((isDownBorder | isUpBorder) & ~captured)), .D(releaseBee | isDownBorder), .Q(keepUp));
    FDRE #(.INIT(1'b0) ) ff4 (.C(clk), .R(loadBee), .CE(((downLeftDirection  | downRightDirection) & releaseBee) | ((isDownBorder | isUpBorder) & ~captured)), .D(releaseBee | isUpBorder), .Q(keepDown));
    
                            
    countUD12L BeeLeftRightCounter (.clk(clk), .reset(1'b0), .Up(frame & keepRight), .Dw(frame & keepLeft), .LD(loadBee), .Din(xBeeStart), .Q(xBeePos));
    countUD12L BeeUpDownCounter (.clk(clk), .reset(1'b0), .Up(frame & keepDown), .Dw(frame & keepUp), .LD(loadBee), .Din(yBeeStart), .Q(yBeePos));

    
    assign bee = ((xPixel >= xBeeLeft) & (xPixel <= xBeeRight))
                    & ((yPixel >= yBeeUp) & (yPixel <= yBeeDown));
















endmodule
