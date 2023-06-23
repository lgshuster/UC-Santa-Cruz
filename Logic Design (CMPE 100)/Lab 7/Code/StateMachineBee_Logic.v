`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 06/04/2019 01:30:06 AM
// Design Name: 
// Module Name: StateMachineBee_Logic
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


module StateMachineBee_Logic(
    input reset,
    input releaseBee,
    input canCaptureBees,
    input isOverlap,
    input [2:0] PS, 
    input [2:0] NS,
    output loadBee,
    output caught,
    output captured
    );
    wire IDLE, MOVE, CAPTURED;
    wire Next_IDLE, Next_MOVE, Next_CAPTURED;
    
    assign IDLE = PS[0];
    assign MOVE = PS[1];
    assign CAPTURED = PS[2];
    
    assign NS[0] = Next_IDLE;
    assign NS[1] = Next_MOVE;
    assign NS[2] = Next_CAPTURED;
    
    assign Next_IDLE = (IDLE & ~releaseBee) | (MOVE & reset) | (CAPTURED & reset);
    assign Next_MOVE = (IDLE & releaseBee) | (MOVE & ((~isOverlap | ~canCaptureBees) & ~reset));
    assign Next_CAPTURED = (MOVE & isOverlap & canCaptureBees) | (CAPTURED & ~reset);
    
    assign loadBee = (IDLE & ~releaseBee) | (MOVE & isOverlap & canCaptureBees) | (CAPTURED & ~reset);
    assign caught = (MOVE & isOverlap & canCaptureBees);
    assign captured = (IDLE & ~releaseBee) | (MOVE & isOverlap & canCaptureBees) | (CAPTURED & ~reset) | (MOVE & reset) | (CAPTURED & reset);

    
endmodule
