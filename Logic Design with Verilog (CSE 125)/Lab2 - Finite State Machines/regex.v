`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 04/21/2020 07:24:05 PM
// Design Name: 
// Module Name: regex
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


module regex(
    input wire clk,
    input wire res_n,
    input wire [1:0] symbol_in,
    input wire last_symbol,
    output reg result,
    output reg done
    );
    
    // State machine states
    parameter idle = 12'b000000000001;
    parameter a = 12'b000000000010;
    parameter b = 12'b000000000100;
    parameter c = 12'b000000001000;
    parameter d = 12'b000000010000;
    parameter a2 = 12'b000000100000;
    parameter db = 12'b000001000000;
    parameter dc = 12'b000010000000;
    parameter dd = 12'b000100000000;
    parameter temp = 12'b001000000000;
    parameter right = 12'b010000000000;
    parameter wrong = 12'b100000000000;
    
    // Symbol parameters
    localparam [1:0] A = 2'b00;
    localparam [1:0] B = 2'b01;
    localparam [1:0] C = 2'b10;
    localparam [1:0] D = 2'b11;
    
    reg [11:0] state, next_state;
    reg done_reg, result_reg;
    
    always @(*) begin
        casez({last_symbol,symbol_in,state})
        
            //idle
            {1'b0, A, idle}: next_state = a; // ~last & A --> a
            {1'b0, (B || C || D), idle}: next_state = temp; // ~last & (B || C || D) --> temp
            {1'b1, (A || B || C), idle}: next_state = wrong; // last & (A || B || C) --> wrong
            {1'b1, D, idle}: next_state = right; // last & D --> right
            
            //a
            {1'b0, B, a}: next_state = b; // ~last & B --> b
            {1'b0, C, a}: next_state = c; // ~last & C --> c
            {1'b0, (A || D), a}: next_state = temp; // ~last & (A || D) --> temp
            {1'b1, 2'b?, a}: next_state = wrong; // last & -- --> wrong
            
            //b
            {1'b0, B, b}: next_state = b; // ~last & B --> b
            {1'b0, C, b}: next_state = c; // ~last & C --> c
            {1'b0, (A || D), b}: next_state = temp; // ~last & (A || D) --> temp
            {1'b1, 2'b?, b}: next_state = wrong; // last & -- --> wrong
        
            //c
            {1'b0, D, c}: next_state = d; // ~last & D --> d
            {1'b0, A, c}: next_state = a2; // ~last & A --> a2
            {1'b0, (B || C), c}: next_state = temp; // ~last & (B || C) --> temp
            {1'b1, 2'b?, c}: next_state = wrong; // last & -- --> wrong    
        
            //d
            {1'b0, B, d}: next_state = db; // ~last & B --> db
            {1'b0, C, d}: next_state = dc; // ~last & C --> dc
            {1'b0, D, d}: next_state = dd; // ~last & D --> dd
            {1'b0, A, d}: next_state = temp; // ~last & A --> temp
            {1'b1, 2'b?, d}: next_state = wrong; // last & -- --> wrong    
        
            //a2
            {1'b0, A, a2}: next_state = a; // ~last & A --> a
            {1'b0, (B || C || D), a2}: next_state = temp; // ~last & (B || C || D) --> temp
            {1'b1, (A || B || C), a2}: next_state = wrong; // last & (A || B || C) --> wrong 
            {1'b1, D, a2}: next_state = right; // last & D --> right  
        
            //db
            {1'b0, A, db}: next_state = a; // ~last & A --> a
            {1'b0, (B || C || D), db}: next_state = temp; // ~last & (B || C || D) --> temp
            {1'b1, (A || B || C), db}: next_state = wrong; // last & (A || B || C) --> wrong 
            {1'b1, D, db}: next_state = right; // last & D --> right    
            
            //dc
            {1'b0, A, dc}: next_state = a; // ~last & A --> a
            {1'b0, (B || C || D), dc}: next_state = temp; // ~last & (B || C || D) --> temp
            {1'b1, (A || B || C), dc}: next_state = wrong; // last & (A || B || C) --> wrong
            {1'b1, D, dc}: next_state = right; // last & D --> right     
        
            //dd
            {1'b0, A, dd}: next_state = a; // ~last & A --> a
            {1'b0, (B || C || D), dd}: next_state = temp; // ~last & (B || C || D) --> temp
            {1'b1, (A || B || C), dd}: next_state = wrong; // last & (A || B || C) --> wrong  
            {1'b1, D, dd}: next_state = right; // last & D --> right 
            
            //temp
            {1'b0, 2'b?, temp}: next_state = temp; // ~last & -- --> temp
            {1'b1, 2'b?, temp}: next_state = wrong; // last & -- --> wrong
     
            //right
            {1'b?, 2'b?, right}: next_state = idle; // - & -- --> idle
        
            //wrong
            {1'b?, 2'b?, wrong}: next_state = idle; // - & -- --> idle
            
            default: begin
                //$display("%b %b %b", last_symbol, symbol_in, state);
                next_state = 12'bx;
            end
        endcase
        
        // Check if done and result need to go high
        if (next_state == right) begin // If next state is right, make done 1 and result 1
            done_reg = 1'b1;
            result_reg = 1'b1;
        end
        else if (next_state == wrong) begin // Else if next state is wrong, make done 1 and result 0
            done_reg = 1'b1;
            result_reg = 1'b0;
        end
        else begin // Else if neither, keep done 0 and result unknown
            done_reg = 1'b0;
            result_reg = 1'bx;
        end
        
    end
    
    always @(posedge clk or negedge res_n) begin
        if (!res_n) begin // On reset (active low), reset FSM state to idle, done to 0, and result to unknown
            state <= idle;
            done <= 1'b0;
            result <= 1'bx;
        end
        else begin // If not reset, update FSM state, done, and result
            state <= next_state;
            done <= done_reg;
            result <= result_reg;
        end
    end
    
endmodule
