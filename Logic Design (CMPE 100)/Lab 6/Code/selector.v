`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 05/02/2019 06:25:56 AM
// Design Name: 
// Module Name: selector
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


module selector(
    input [2:0] sel,
    input [11:0] N,
    output [3:0] H
    );
    
    assign H = (N[11:8]&{4{sel[2]}}) 
                | (N[7:4]&{4{sel[1]}}) 
                | (N[3:0]&{4{sel[0]}});
    
endmodule
