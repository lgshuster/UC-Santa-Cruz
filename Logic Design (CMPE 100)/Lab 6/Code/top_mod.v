`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 05/16/2019 09:35:01 PM
// Design Name: 
// Module Name: top_mod
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


module top_mod(
    input clkin,
    input btnU,
    input btnL,
    input btnR,
    output [15:0] led,
    output [3:0] an,
    output dp,
    output [6:0] seg
    );
    
    wire clk, digsel, qsec;
    wire btnLEdge, btnREdge;
    wire LS, RS, TurkeyRight, TurkeyLeft, KeepRight, KeepLeft, c_in;
    wire [7:0] TurkeyCount, SignedNumber, ConvertedNumber;
    wire [11:0] AllNums;
    wire [2:0] RingCounter_out;
    wire [3:0] Selector_out;
    wire [6:0] dummyseg;

    
    lab6_clks slowit (.clkin(clkin), .greset(btnU), .clk(clk), .digsel(digsel), .qsec(qsec));
    
    assign LS = ~btnLEdge; 
    assign RS = ~btnREdge;
    
    assign SignedNumber = ({8{TurkeyCount[7]}} & ~TurkeyCount) | ({8{~TurkeyCount[7]}} & TurkeyCount);
    assign c_in = TurkeyCount[7];
    
    assign AllNums[7:0] = {1'b0, ConvertedNumber[6:0]};
    
    assign led[15] = ~btnL;
    assign led[8] = ~btnR;
        
    assign an[3] = 1'b1;
    assign an[2] = ~(RingCounter_out[2] & TurkeyCount[7]);
    assign an[1:0] = ~RingCounter_out[1:0];
    
    assign dp = 1'b1;
    
    assign seg = ({7{RingCounter_out[2]}} & {7{c_in}} & {1'b0, 1'b1, 1'b1, 1'b1, 1'b1, 1'b1, 1'b1}) | ({7{~RingCounter_out[2]}} & dummyseg); 
    
    FDRE  #(.INIT(1'b0) ) ff1 (.C(clk), .CE(1'b1),  .D(btnL), .Q(btnLEdge));
    FDRE  #(.INIT(1'b0) ) ff2 (.C(clk), .CE(1'b1),  .D(btnR), .Q(btnREdge));
      
    StateMachine machine (.clk(clk), .LS(LS), .RS(RS), .TurkeyRight(TurkeyRight), .TurkeyLeft(TurkeyLeft));
    Turkey_Counter counter (.clk(clk), .TurkeyRight(TurkeyRight), .TurkeyLeft(TurkeyLeft), .TurkeyCount(TurkeyCount));
    
    FDRE  #(.INIT(1'b0) ) ff3 (.C(clk), .R(1'b0), .CE(TurkeyRight | TurkeyLeft),  .D(TurkeyRight), .Q(KeepRight));
    FDRE  #(.INIT(1'b0) ) ff4 (.C(clk), .R(1'b0), .CE(TurkeyRight | TurkeyLeft),  .D(TurkeyLeft), .Q(KeepLeft));
    
    AddSub8 addsub (.c_in(c_in), .A(SignedNumber), .B({8{1'b0}}), .S(ConvertedNumber));
    
    LED_Shifter shifter (.clk(clk), .qsec(qsec), .LS(LS), .RS(RS), .SHR(KeepRight), .SHL(KeepLeft), .Q(led[7:0]));
        
    ring_counter RingCounter (.clk(clk), .Advance(digsel), .Q(RingCounter_out));
    selector Selector (.sel(RingCounter_out), .N(AllNums), .H(Selector_out));
    hex7seg display (.n(Selector_out), .e(1'b1), .seg(dummyseg));
    
endmodule
