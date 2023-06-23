`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 05/16/2019 07:02:05 AM
// Design Name: 
// Module Name: StateMachine_Logic
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


module StateMachine_Logic(
    input LS,
    input RS,
    input [6:0] PS,
    output [6:0] NS,
    output TurkeyRight,
    output TurkeyLeft
    );
    
    wire IDLE, FROM_LEFT, FROM_RIGHT, BOTH_FROM_LEFT, BOTH_FROM_RIGHT, CONTINUE_RIGHT, CONTINUE_LEFT;
    wire Next_IDLE, Next_FROM_LEFT, Next_FROM_RIGHT, Next_BOTH_FROM_LEFT, Next_BOTH_FROM_RIGHT, Next_CONTINUE_RIGHT, Next_CONTINUE_LEFT;
    
    assign IDLE = PS[0];
    assign FROM_LEFT = PS[1];
    assign FROM_RIGHT = PS[2];
    assign BOTH_FROM_LEFT = PS[3];
    assign BOTH_FROM_RIGHT = PS[4];
    assign CONTINUE_RIGHT = PS[5];
    assign CONTINUE_LEFT = PS[6];
    
    assign NS[0] = Next_IDLE;
    assign NS[1] = Next_FROM_LEFT;
    assign NS[2] = Next_FROM_RIGHT;
    assign NS[3] = Next_BOTH_FROM_LEFT;
    assign NS[4] = Next_BOTH_FROM_RIGHT;
    assign NS[5] = Next_CONTINUE_RIGHT;
    assign NS[6] = Next_CONTINUE_LEFT;
    
    assign Next_IDLE = (IDLE & (LS & RS)) | (FROM_LEFT & LS) | (FROM_RIGHT & RS) | (CONTINUE_RIGHT & (LS & RS)) | (CONTINUE_LEFT & (RS & LS));
    assign Next_FROM_LEFT = (IDLE & ~LS) | (FROM_LEFT & (~LS & RS)) | (BOTH_FROM_LEFT & (~LS & RS));
    assign Next_FROM_RIGHT = (IDLE & ~RS) | (FROM_RIGHT & (~RS & LS)) | (BOTH_FROM_RIGHT & (~RS & LS));
    assign Next_BOTH_FROM_LEFT = (FROM_LEFT & (~LS & ~RS)) | (BOTH_FROM_LEFT & (~LS & ~RS)) | (CONTINUE_RIGHT & (~LS & ~RS));
    assign Next_BOTH_FROM_RIGHT = (FROM_RIGHT & (~RS & ~LS)) | (BOTH_FROM_RIGHT & (~RS & ~LS)) | (CONTINUE_LEFT & (~RS & ~LS));
    assign Next_CONTINUE_RIGHT = (BOTH_FROM_LEFT & (LS & ~RS)) | (CONTINUE_RIGHT & (LS & ~RS));
    assign Next_CONTINUE_LEFT = (BOTH_FROM_RIGHT & (RS & ~LS)) | (CONTINUE_LEFT & (RS & ~LS));

    assign TurkeyRight = CONTINUE_RIGHT & (LS & RS);
    assign TurkeyLeft = CONTINUE_LEFT & (RS & LS);


endmodule
