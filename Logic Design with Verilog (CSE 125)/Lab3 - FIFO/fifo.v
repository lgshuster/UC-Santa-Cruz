`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 04/28/2020 05:36:06 PM
// Design Name: 
// Module Name: fifo
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


module fifo #(parameter WIDTH=64, parameter DEPTH=8)(
    input wire clk, 
    input wire res_n, 
    input wire shift_in, 
    input wire shift_out, 
    input wire [WIDTH-1:0] data_in,  
    output reg full, 
    output reg empty,
    output reg [WIDTH-1:0] data_out
    );

    wire [WIDTH-1:0] data_out_arr [DEPTH-1:0]; // Holds fifo stage outputs
    wire [DEPTH-1:0] full_bus, empty_bus; // Holds fifo stage empty and full signals
    reg [DEPTH:0] in_pointer;  // Keeps track of where to insert data_in
    reg [DEPTH:0] before_in_pointer; // Keeps track of where to insert data_in in case of shift_in and shift_out

    genvar i;
        generate
            for (i = 0; i < DEPTH; i = i + 1) begin: FIFO
                if (i == DEPTH - 1) begin // If last fifo stage, instantiate it but give no previous data
                    fifo_stage #(.WIDTH(WIDTH)) stage 
                        (.clk(clk), .res_n(res_n), .shift_in(shift_in), .shift_out(shift_out), .in_pointer(in_pointer[i]), .before_in_pointer(before_in_pointer[i]), .data_in(data_in), 
                        .hold(data_out_arr[i]), .prev_data_out({WIDTH{1'bx}}), .full(full_bus[i]), .empty(empty_bus[i]), .data_out(data_out_arr[i]));
                end
                
                else begin // Else if every other fifo stage, feed in previous fifo stage output
                    fifo_stage #(.WIDTH(WIDTH)) stage 
                        (.clk(clk), .res_n(res_n), .shift_in(shift_in), .shift_out(shift_out), .in_pointer(in_pointer[i]), .before_in_pointer(before_in_pointer[i]), .data_in(data_in), 
                        .hold(data_out_arr[i]), .prev_data_out(data_out_arr[i+1]), .full(full_bus[i]), .empty(empty_bus[i]), .data_out(data_out_arr[i]));
                end
            end
    endgenerate

    always @(*) begin
        data_out = data_out_arr[0]; // Set data_out always equal to first fifo stage
        
        // And all fifo stage full/empty signals to see overall fifo fullness/emptiness
        full = &full_bus;
        empty = &empty_bus;
    end
 
    always @(posedge clk or negedge res_n) begin
        if (!res_n) begin // On reset, initialize fifo to empty, data_out to unknown, and in_pointer to first fifo stage and before_in_pointer to nothing
            full <= 1'b0;
            empty <= 1'b1;
            data_out <= 'bx;
            in_pointer <= 'b1;
            before_in_pointer <= 'b0;
        end
        else begin        
            if (~shift_in && shift_out) begin // If just shift_out, move in_pointer and before_in_pointer to previous fifo stages
                in_pointer <= in_pointer >> 1;
                before_in_pointer <= before_in_pointer >> 1;
            end
            else if (shift_in && ~shift_out) begin // Else if just shift_in, move in_pointer to next fifo stage and make before_in_pointer to what in_pointer used to be
                in_pointer <= in_pointer << 1;
                before_in_pointer <= in_pointer;
            end
        end
    end
    
endmodule
