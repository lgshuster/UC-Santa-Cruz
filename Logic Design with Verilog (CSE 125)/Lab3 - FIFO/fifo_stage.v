`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 04/28/2020 05:37:14 PM
// Design Name: 
// Module Name: stage
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


module fifo_stage #(parameter WIDTH=64)(
    input wire clk, 
    input wire res_n,
    input wire shift_in, 
    input wire shift_out,
    input wire in_pointer,
    input wire before_in_pointer,
    input wire [WIDTH-1:0] data_in,
    input wire [WIDTH-1:0] hold,
    input wire [WIDTH-1:0] prev_data_out,
    output reg full, 
    output reg empty,
    output reg [WIDTH-1:0] data_out
    );
    
    reg full_reg, empty_reg; // Holds fifo stage full and empty signals for next clock cycle
    reg [WIDTH-1:0] next_data_out_reg; // Holds what data_out should be for next clock cycle

    always @(*) begin
    // Start of multiplixer
        if (~shift_in && ~shift_out) begin  // Case 1: If no shift_in and no shift_out, do nothing and keep current value
            //$display("Case 1");
         
            next_data_out_reg = hold;
        end
        
        else if (~shift_in && shift_out) begin // Case 2: Else if just shift_out, insert previous fifo stage output
            //$display("Case 2");
            
            next_data_out_reg = prev_data_out; // Take in previous stage output
        end

        else if (shift_in && ~shift_out) begin // Case 3: Else if just shift_in, if fifo stage pointed to by in_pointer insert data_in, if any other fifo stage keep current stage output
            //$display("Case 3");
            
            if (in_pointer) next_data_out_reg = data_in; // If fifo stage pointed to by in_pointer, insert data_in
            else next_data_out_reg = hold; // Else if any other fifo stage, keep current stage output
        end
        
        else if (shift_in && shift_out) begin // Case 4: Else if both shift_in and shift_out, if fifo stage pointed to by before_in_pointer insert data_in, if any other fifo stage insert previous fifo stage output
            //$display("Case 4");
            
            if (before_in_pointer) next_data_out_reg = data_in; // If fifo stage pointed to by before_in_pointer, insert data_in
            else next_data_out_reg = prev_data_out; // Else if any other fifo stage, insert previous fifo stage output
        end
        
        else begin // Case 5: Error checking if shift_in or shift_out is unknown, same as case 1 do nothing and keep current value
            $display("Shift_in or shift_out unknown");
            
            next_data_out_reg = hold;     
        end
    // End of multiplixer
        
    // Check if stage is or full or empty
        if (next_data_out_reg !== {WIDTH{1'bx}}) begin // If current value is non-unknown, stage is full
            full_reg = 1'b1;
            empty_reg = 1'b0;
        end
        else begin // Else if current value is unknown, stage is empty
            full_reg = 1'b0;
            empty_reg = 1'b1;         
        end
    end 
       
    always @(posedge clk or negedge res_n) begin
        if (!res_n) begin // If reset, initialize fifo stage to empty and data_out to unknown
            full <= 1'b0;
            empty <= 1'b1;
            data_out <= {WIDTH{1'bx}};
        end
        else begin // Else if no reset, update full and empty signals and data out
            full <= full_reg;
            empty <= empty_reg;
            data_out <= next_data_out_reg;
        end
    end
endmodule