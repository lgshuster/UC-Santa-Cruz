`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 05/09/2019 11:26:24 AM
// Design Name: 
// Module Name: Time_Counter
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


module Time_Counter(
    input clk,
    input enable,
    input resetTimer,
    output QuarterSec,
    output OneSec,
    output FourSecs,
    output EightSecs
    );
    
    wire [7:0] frames;
    wire [7:0] quarterSecond;
    wire frameUTC, quarterSecondUTC;
        
    // 1/4 sec = 16 = 10000

    countUD4L frameCounter1 (.clk(clk), .reset(resetTimer), .Up(enable), .Dw(1'b0), .LD(frames[4]), .Din(1'b0), .Q(frames[3:0]), .UTC(frameUTC));
    countUD4L frameCounter2 (.clk(clk), .reset(resetTimer), .Up(enable & frameUTC), .Dw(1'b0), .LD(frames[4]), .Din(1'b0), .Q(frames[7:4]));
    
    countUD4L quarterSecondCounter1 (.clk(clk), .reset(resetTimer), .Up(frames[4]), .Dw(1'b0), .LD(1'b0), .Din(1'b0), .Q(quarterSecond[3:0]), .UTC(quarterSecondUTC));
    countUD4L quarterSecondCounter2 (.clk(clk), .reset(resetTimer), .Up(frames[4] & quarterSecondUTC), .Dw(1'b0), .LD(1'b0), .Din(1'b0), .Q(quarterSecond[7:4]));
    
    
    // 1/4 sec = 1/4, 1 = 1
    // 1 sec = 4/4, 4 = 100
    // 4 secs = 16/4, 16 = 10000
    // 8 secs = 32/4, 32 = 100000
    assign QuarterSec = quarterSecond[0];
    assign OneSec = quarterSecond[2] & ~(&quarterSecond[7:3]) & ~(quarterSecond[1:0]);
    assign FourSecs = quarterSecond[4] & ~(&quarterSecond[7:5]) & ~(&quarterSecond[3:0]);
    assign EightSecs = quarterSecond[5] & ~(&quarterSecond[7:6]) & ~(&quarterSecond[4:0]);
   
    
endmodule
