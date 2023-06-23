`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 05/16/2019 09:10:26 PM
// Design Name: 
// Module Name: Turkey_Counter
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


module Turkey_Counter(
    input clk, 
    input TurkeyRight,
    input TurkeyLeft,
    output [7:0] TurkeyCount
    );
    
    wire UTC, DTC;
    
    countUD4L counter1 (.clk(clk), .Up(TurkeyLeft), .Dw(TurkeyRight), .Q(TurkeyCount[3:0]), .UTC(UTC), .DTC(DTC));
    countUD4L counter2 (.clk(clk), .Up(TurkeyLeft & UTC), .Dw(TurkeyRight & DTC), .Q(TurkeyCount[7:4]));
    
endmodule
