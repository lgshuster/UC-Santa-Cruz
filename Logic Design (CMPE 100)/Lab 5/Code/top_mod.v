`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 05/09/2019 12:43:12 PM
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
    input btnR,
    input btnU,
    input btnC,
    input sw,
    output [15:0] led,
    output [3:0] an,
    output dp,
    output [6:0] seg    
    );
    
    wire clk, digsel, qsec;
    wire FourSecs, TwoSecs, Match;
    wire HalfSec, LoadTarget, ResetTimer,  LoadNumbers, SHR, SHL, FlashBoth, FlashAlt;
    wire [7:0] LFSR_out, Numbers_out;
    wire [3:0] Target_out, sum, RingCounter_out, Selector_out;
    wire [15:0] AllNums;
    //wire KeepHalfSec;
    
    lab5_clks slowit (.clkin(clkin), .greset(btnR), .clk(clk), .digsel(digsel), .qsec(qsec));
    
    assign AllNums[15:12] = Target_out;
    assign AllNums[11:8] = sum;
    assign AllNums[7:0] = Numbers_out;
    
    assign Match = ~(|(sum ^ Target_out));

    assign an[3] = ~(RingCounter_out[3] & ((~FlashBoth & ~FlashAlt) | (FlashBoth & HalfSec) | (FlashAlt & HalfSec)));
    assign an[2] = ~(sw & RingCounter_out[2]);
    assign an[1] = ~(RingCounter_out[1] & ((~FlashBoth & ~FlashAlt) | (FlashBoth & HalfSec) | (FlashAlt & ~HalfSec)));
    assign an[0] = ~(RingCounter_out[0] & ((~FlashBoth & ~FlashAlt) | (FlashBoth & HalfSec) | (FlashAlt & ~HalfSec)));
    assign dp = 1'b1;
        
    Time_Counter counter (.clk(clk), .enable(qsec), .R(ResetTimer), .HalfSec(HalfSec), .TwoSecs(TwoSecs), .FourSecs(FourSecs));

    //FDRE  #(.INIT(1'b0) ) ff (.C(clk), .R(1'b0), .CE(HalfSec), .D(~KeepHalfSec), .Q(KeepHalfSec));
    
    StateMachine machine (.clk(clk), .Go(btnC), .Stop(btnU), .FourSecs(FourSecs), .TwoSecs(TwoSecs), .Match(Match),
            .LoadTarget(LoadTarget), .ResetTimer(ResetTimer), .LoadNumbers(LoadNumbers), .SHR(SHR), .SHL(SHL), .FlashBoth(FlashBoth), .FlashAlt(FlashAlt));
    
    LFSR randomNumGen (.clk(clk), .rnd(LFSR_out));
    Numbers numbers (.clk(clk), .D(LFSR_out), .enable(LoadNumbers), .Q(Numbers_out));
    Target target (.clk(clk), .D(LFSR_out), .enable(LoadTarget), .Q(Target_out));
    
    AddSub8 add (.A(Numbers_out[7:4]), .B(Numbers_out[3:0]), .S(sum));
    
    LED_Shifter shifter (.clk(clk), .SHR(SHR), .SHL(SHL), .Q(led));
    
    ring_counter RingCounter (.clk(clk),.Advance(digsel), .Q(RingCounter_out));
    selector Selector (.sel(RingCounter_out), .N(AllNums), .H(Selector_out));
    hex7seg display (.n(Selector_out), .e(1'b1), .seg(seg));
    
endmodule
