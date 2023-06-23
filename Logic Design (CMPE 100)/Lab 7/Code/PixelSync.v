`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 06/01/2019 12:25:53 AM
// Design Name: 
// Module Name: PixelSync
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


module PixelSync(
    input clk,
    output [11:0] xPixel,
    output [11:0] yPixel,
    output Hsync,
    output Vsync
    );
    wire xReset, yReset;
    
    assign xReset = (xPixel == 12'd799);
    assign yReset = (yPixel == 12'd524) & xReset;
    
    countUD12L xCounter (.clk(clk), .reset(xReset), .Up(1'b1), .Dw(1'b0), .LD(1'b0), .Din(1'b0), .Q(xPixel));
    countUD12L yCounter (.clk(clk), .reset(yReset), .Up(xReset), .Dw(1'b0), .LD(1'b0), .Din(1'b0), .Q(yPixel));
    
    assign Hsync = ~((xPixel >= 12'd655) & (xPixel <= 12'd750));
    assign Vsync = ~((yPixel >= 12'd489) & (yPixel <= 12'd490));
 
endmodule
