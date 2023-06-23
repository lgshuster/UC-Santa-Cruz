`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 05/09/2019 04:18:35 AM
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
    input Go,
    input Stop,
    input FourSecs,
    input TwoSecs,
    input Match,
    input [2:0] PS,
    output [2:0] NS,
    output LoadTarget,
    output ResetTimer,
    output LoadNumbers,
    output SHR,
    output SHL,
    output FlashBoth,
    output FlashAlt
    );
    
    wire IDLE, START, END;
    wire Next_IDLE, Next_START, Next_END;
    
    assign IDLE = PS[0];
    assign START = PS[1];
    assign END = PS[2];
    
    assign NS[0] = Next_IDLE;
    assign NS[1] = Next_START;
    assign NS[2] = Next_END;
    
    assign Next_IDLE = (IDLE & ~Go) | (END & FourSecs);
    assign Next_START = (IDLE & Go) | (START & ~Stop);
    assign Next_END = (START & Stop) | (END & ~FourSecs);
    
    assign LoadTarget = IDLE & Go;
    assign ResetTimer = (IDLE & Go) | (START & ~Stop & TwoSecs) | (START & Stop) | (END & FourSecs);
    assign LoadNumbers = (IDLE & Go) | (START & ~Stop & TwoSecs);
    assign SHR = END & FourSecs & ~Match;
    assign SHL = END & FourSecs & Match;
    assign FlashBoth = END & ~FourSecs & ~Match;
    assign FlashAlt = END & ~FourSecs & Match;
    
endmodule
