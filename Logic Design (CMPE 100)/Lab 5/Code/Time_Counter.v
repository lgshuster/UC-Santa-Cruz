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
    input R,
    output HalfSec,
    output TwoSecs,
    output FourSecs
    );
    
    wire [7:0] Q;
    wire UTC;
    
    countUD4L counter1 (.clk(clk), .R(R), .Up(enable), .Q(Q[3:0]), .UTC(UTC));
    countUD4L counter2 (.clk(clk), .R(R), .Up(enable & UTC), .Q(Q[7:4]));

    // 10 = 2
    // 1000 = 8
    // 10000 = 16
    
    assign HalfSec = Q[1] & ~(&Q[7:2]) & ~Q[0];
    assign TwoSecs = Q[3] & ~(&Q[7:4]) & ~(&Q[2:0]);
    assign FourSecs = Q[4] & ~(&Q[7:5]) & ~(&Q[3:0]);
   
    
endmodule
