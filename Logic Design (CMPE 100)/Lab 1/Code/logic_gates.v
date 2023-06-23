`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 04/12/2019 04:02:06 AM
// Design Name: 
// Module Name: logic_gates
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


module logic_gates(
    input BTNC,
    input SW0,
    input SW1,
    input SW2,
    output LD0,
    output LD1,
    output LD2,
    output LD3
    );
    
    assign LD0 = ~BTNC;
    assign LD1 = SW0 & SW1;
    assign LD2 = SW0 | SW1;
    assign LD3 = SW0 ^ SW1 ^ SW2;
    
endmodule