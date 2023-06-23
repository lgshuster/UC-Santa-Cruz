// Control logic for whether branches are taken or not

package dinocpu

import chisel3._
import chisel3.util._

class Booth8cycle extends Module {
  val io = IO(new Bundle {
    val go = Input(UInt(1.W))
    val num1 = Input(UInt(16.W))
    val num2 = Input(UInt(16.W))

    val done = Output(UInt(1.W))
    val result = Output(UInt(32.W))
  })

  // Regs hold stuff after first cycle
  val multiplicand_reg = RegInit(0.U(16.W))
  val multiplier_reg = RegInit(0.U(17.W))
  val count_reg = RegInit(0.U(4.W))
  val shift_reg = RegInit(0.U(4.W))
  val resultCurr_reg = RegInit(0.U(32.W))

  // Wires for first cycle and holding values
  val multiplicand = Wire(UInt(16.W))
  val multiplier = Wire(UInt(17.W))
  val count = Wire(UInt(4.W))
  val multiplierBits = Wire(UInt(3.W))
  val partProd = Wire(UInt(32.W))
  val partProdExt = Wire(UInt(32.W))

  multiplicand := io.num1

  // On the first cycle, can't use reg since we have to wait a cycle, so use a wire
  when (io.go === 1.U) {
    multiplier := Cat(io.num2, 0.U)
    count := "d8".U
  }
  .otherwise {
    multiplier := multiplier_reg
    count := count_reg
  }

  multiplierBits := multiplier(2,0)

  io.result := 0.U
  io.done := 0.U
  partProd := 0.U
  partProdExt := 0.U

  //printf("%d %d %d %b %b %b %b %b %b\n", io.num1, io.num2, count, multiplier, multiplierBits, multiplier_reg, partProd, partProdExt, resultCurr_reg)

  when (count =/= 0.U) {
    switch (multiplierBits) {
      is("b000".U) { // 0x multiplicand
        partProd := 0.U
      }
      is("b001".U) { // 1x multiplicand
        partProd := multiplicand
      }
      is("b010".U) { // 1x multiplicand
        partProd := multiplicand
      }
      is("b011".U) { // 2x multiplicand
        partProd := multiplicand << 1.U
      }
      is("b100".U) { // -2x multiplicand
        partProd := ~(multiplicand << 1.U) + 1.U
      }
      is("b101".U) { // -1x multiplicand
        partProd := (~multiplicand) + 1.U
      }
      is("b110".U) { // -1x multiplicand
        partProd := (~multiplicand) + 1.U
      }
      is("b111".U) { // 0x multiplicand
        partProd := 0.U
      }
    }

    partProdExt := Cat(Fill(16, partProd(15)), partProd(15, 0)) // Sign extended partial product

    resultCurr_reg := resultCurr_reg + (partProdExt << shift_reg) // Shift partial product accordingly and add to total result
    shift_reg := shift_reg + 2.U // Update shifting for next cycle
    multiplicand_reg := multiplicand
    multiplier_reg := Cat(Fill(2, multiplier(15)), multiplier(15, 2)) // Shift multiplier right twice and sign extend
    count_reg := count - 1.U // Decrease count for next cycle
  }
  .otherwise {
    io.done := 1.U
    io.result := resultCurr_reg

    // Reset regs for new multiplication
    multiplicand_reg := 0.U
    multiplier_reg := 0.U
    count_reg := 0.U
    shift_reg := 0.U
    resultCurr_reg := 0.U
  }
}