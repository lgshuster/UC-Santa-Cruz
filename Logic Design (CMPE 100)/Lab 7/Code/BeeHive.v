`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 06/01/2019 03:06:19 PM
// Design Name: 
// Module Name: BeeHive
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


module BeeHive(
    input clk,
    input [12:0] xPixel,
    input [12:0] yPixel,
    input frame,
    input loadHive,
    input startHive,
    input canMoveHive,
    input left,
    input right,
    input up,
    input down,
    output [12:0] xHiveCenter,
    output [12:0] yHiveCenter,
    output hiveInterior,
    output hiveBorders
    );
    wire [12:0] xStart, yStart, xPos, yPos, xLeft, xRight, yUp, yDown;
    wire isLeftBorder, isRightBorder, isUpBorder, isDownBorder, keepLeft, keepRight, keepUp, keepDown;
        
    assign xHiveCenter = xPos;
    assign yHiveCenter = yPos;
        
    assign xStart = 13'd319;
    assign yStart = 13'd239;
            
    assign xLeft = xPos - 13'd39;
    assign xRight = xPos + 13'd39;
    assign yUp = yPos - 13'd39;
    assign yDown = yPos + 13'd39;
    
    assign isLeftBorder = ((xLeft - 13'd9) == 13'd7);
    assign isRightBorder = ((xRight + 13'd9) == 13'd632);
    assign isUpBorder = ((yUp - 13'd9) == 13'd7);
    assign isDownBorder = ((yDown + 13'd9) == 13'd472);
    
        
    FDRE #(.INIT(1'b0) ) ff1 (.C(clk), .R(1'b0), .CE(((left & ~right) | (right & ~left) | isLeftBorder | isRightBorder) & canMoveHive), .D(left | isRightBorder), .Q(keepLeft));
    FDRE #(.INIT(1'b0) ) ff2 (.C(clk), .R(1'b0), .CE(((left & ~right) | (right & ~left) | startHive | isLeftBorder | isRightBorder) & canMoveHive), .D(right | startHive | isLeftBorder), .Q(keepRight));
    FDRE #(.INIT(1'b0) ) ff3 (.C(clk), .R(1'b0), .CE(((up & ~down) | (down & ~up) | isUpBorder | isDownBorder) & canMoveHive), .D(up | isDownBorder), .Q(keepUp));
    FDRE #(.INIT(1'b0) ) ff4 (.C(clk), .R(1'b0), .CE(((up & ~down) | (down & ~up) | startHive | isUpBorder | isDownBorder) & canMoveHive), .D(down | startHive | isUpBorder), .Q(keepDown));
    
                            
    countUD12L HiveLeftRightCounter (.clk(clk), .reset(1'b0), .Up(frame & keepRight & canMoveHive), .Dw(frame & keepLeft & canMoveHive), .LD(loadHive), .Din(xStart), .Q(xPos));
    countUD12L HiveUpDownCounter (.clk(clk), .reset(1'b0), .Up(frame & keepDown & canMoveHive), .Dw(frame & keepUp & canMoveHive), .LD(loadHive), .Din(yStart), .Q(yPos));

    assign hiveInterior = ((xPixel >= xLeft) & (xPixel <= xRight))
                            & ((yPixel >= yUp) & (yPixel <= yDown));
                            
    assign hiveBorders =  (((xPixel >= (xLeft - 13'd8)) & (xPixel <= (xRight + 13'd8))) & (((yPixel >= (yUp - 13'd8)) & (yPixel <= (yUp - 13'd1))) | ((yPixel >= (yDown + 13'd1)) & (yPixel <= (yDown + 13'd8)))))
                            | (((yPixel >= (yUp - 13'd8)) & (yPixel <= (yDown + 13'd8))) & (((xPixel >= (xLeft - 13'd8)) & (xPixel <= (xLeft - 13'd1))) | ((xPixel >= (xRight + 13'd1)) & (xPixel <= (xRight + 13'd8)))));
     
     
     
     
     
     
     
     
     
     
     
     
     
     
     
     
     
     
                                
endmodule
