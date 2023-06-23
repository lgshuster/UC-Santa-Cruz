`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 06/03/2019 05:59:45 AM
// Design Name: 
// Module Name: StateMachineHive_Logic
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


module StateMachineHive_Logic(
    input Go,
    input FourSecs,
    input EightSecs,
    input allCaught,
    input outOfTime,
    input [5:0] PS,
    output [5:0] NS,
    output notInPlay,
    output loadHive,
    output startHive,
    output canMoveHive,
    output releaseBees,
    output canCaptureBees,
    output flashBorders,
    output flashCount,
    output flashAmount,
    output startTimer,
    output resetTimer,
    output stopTimer,
    output reset
    );
    wire IDLE, IDLE_MOVE, STOP_RELEASE, MOVE_CAPTURE, WIN, LOSE;
    wire Next_IDLE, Next_IDLE_MOVE, Next_STOP_RELEASE, Next_MOVE_CAPTURE, Next_WIN, Next_LOSE;
    
    assign IDLE = PS[0];
    assign IDLE_MOVE = PS[1];
    assign STOP_RELEASE = PS[2];
    assign MOVE_CAPTURE = PS[3];
    assign WIN = PS[4];
    assign LOSE = PS[5];

    assign NS[0] = Next_IDLE;
    assign NS[1] = Next_IDLE_MOVE;
    assign NS[2] = Next_STOP_RELEASE;
    assign NS[3] = Next_MOVE_CAPTURE;
    assign NS[4] = Next_WIN;
    assign NS[5] = Next_LOSE;

    
    assign Next_IDLE = (IDLE & ~FourSecs);
    assign Next_IDLE_MOVE = (IDLE & FourSecs) | (IDLE_MOVE & ~Go) | (LOSE & EightSecs);
    assign Next_STOP_RELEASE = (IDLE_MOVE & Go) | (STOP_RELEASE & ~EightSecs) | (WIN & Go);
    assign Next_MOVE_CAPTURE = (STOP_RELEASE & EightSecs) | (MOVE_CAPTURE & ~allCaught & ~outOfTime);
    assign Next_WIN = (MOVE_CAPTURE & allCaught) | (WIN & ~Go);
    assign Next_LOSE = (MOVE_CAPTURE & outOfTime) | (LOSE & ~EightSecs);

    assign notInPlay = IDLE | IDLE_MOVE;
    assign loadHive = (IDLE & ~FourSecs);
    assign startHive = (IDLE & FourSecs);
    assign canMoveHive = (IDLE & FourSecs) | (IDLE_MOVE & ~Go) | (STOP_RELEASE & EightSecs) | (MOVE_CAPTURE & ~allCaught & ~outOfTime) | (LOSE & ~EightSecs) | (LOSE & EightSecs) | (WIN & ~Go);
    assign releaseBees = (IDLE_MOVE & Go) | (WIN & Go);
    assign canCaptureBees = (STOP_RELEASE & EightSecs) | (MOVE_CAPTURE & ~allCaught & ~outOfTime);
    assign flashBorders = (STOP_RELEASE & EightSecs) | (MOVE_CAPTURE & ~allCaught & ~outOfTime);
    assign flashCount = (MOVE_CAPTURE & allCaught) | (WIN & ~Go);
    assign flashAmount = (MOVE_CAPTURE & outOfTime) | (LOSE & ~EightSecs);
    assign startTimer = (STOP_RELEASE & EightSecs);
    assign resetTimer = (IDLE_MOVE & Go) | (MOVE_CAPTURE & outOfTime) | (WIN & Go);
    assign stopTimer = (MOVE_CAPTURE & allCaught)  | (WIN & ~Go) | (MOVE_CAPTURE & outOfTime) | (LOSE & ~EightSecs);        
    assign reset = (WIN & Go) | (LOSE & EightSecs);
    
endmodule
