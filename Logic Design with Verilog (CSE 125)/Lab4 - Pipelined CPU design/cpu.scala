// This file is where all of the CPU components are assembled into the whole CPU

package dinocpu

import chisel3._
import chisel3.util._

/**
  * The main CPU definition that hooks up all of the other components.
  *
  * For more information, see section 4.6 of Patterson and Hennessy
  * This follows figure 4.49
  */
class PipelinedCPU(implicit val conf: CPUConfig) extends Module {
  val io = IO(new CoreIO)

  // Bundles defining the pipeline registers and control structures

  // Everything in the register between IF and ID stages
  class IFIDBundle extends Bundle {
    val instruction = UInt(32.W)
    val pc          = UInt(32.W)
    val pcplusfour  = UInt(32.W)
  }

  // Control signals used in EX stage
  class EXControl extends Bundle {
    val add       = Bool()
    val immediate = Bool()
    val alusrc1   = UInt(2.W)
    val branch    = Bool()
    val jump      = UInt(2.W)
  }

  // Control signals used in MEM stage
  class MControl extends Bundle {
    val memread   = Bool()
    val memwrite  = Bool()
    // VARIABLE FROM BRANCH CONTROL UNIT BOX
  }

  // Control signals used in EB stage
  class WBControl extends Bundle {
    val regwrite = Bool()
    val toreg = UInt(2.W)
  }

  // Everything in the register between ID and EX stages
  class IDEXBundle extends Bundle {
    val excontrol = new EXControl
    val mcontrol  = new MControl
    val wbcontrol = new WBControl

    val writereg  = UInt(5.W)
    val pcplusfour  = UInt(32.W)
    val pc          = UInt(32.W)
    val sextImm     = UInt(32.W)
    val func7       = UInt(7.W)
    val func3       = UInt(3.W)
    val readdata1   = UInt(32.W)
    val readdata2   = UInt(32.W)
    val maskmode = UInt(2.W)
    val sext     = Bool()
    val rs1       = UInt(5.W)
    val rs2       = UInt(5.W)

    /* TWO INSTRUCTION VARIABLES FOR FORWARDING/HAZARDS */
  }

  // Everything in the register between ID and EX stages
  class EXMEMBundle extends Bundle {
    val mcontrol  = new MControl
    val wbcontrol = new WBControl

    val nextpc    = UInt(32.W)
    val writereg  = UInt(5.W)
    // VARIABLE FROM ADDER/BRANCH CONTROL UNIT BOX MUX
    val pcplusfour = UInt(32.W)
    val aluresult  = UInt(32.W)
    val readdata2  = UInt(32.W)
    val maskmode = UInt(2.W)
    val sext     = Bool()
    val taken    = Bool()
    // VARIABLE FROM FORWARDING MUX
    // VARIABLE FROM FUNC3
  }

  // Everything in the register between ID and EX stages
  class MEMWBBundle extends Bundle {
    val wbcontrol = new WBControl

    val pcplusfour = UInt(32.W)
    val readdata   = UInt(32.W)
    val aluresult  = UInt(32.W)
    val writereg  = UInt(5.W)
  }

  // All of the structures required
  val pc         = RegInit(0.U)
  val control    = Module(new Control())
  val branchCtrl = Module(new BranchControl())
  val registers  = Module(new RegisterFile())
  val aluControl = Module(new ALUControl())
  val alu        = Module(new ALU())
  val immGen     = Module(new ImmediateGenerator())
  val pcPlusFour = Module(new Adder())
  val branchAdd  = Module(new Adder())
  val forwarding = Module(new ForwardingUnit())  //pipelined only
  val hazard     = Module(new HazardUnit())      //pipelined only
  val (cycleCount, _) = Counter(true.B, 1 << 30)

  val if_id      = RegInit(0.U.asTypeOf(new IFIDBundle))
  val id_ex      = RegInit(0.U.asTypeOf(new IDEXBundle))
  val ex_mem     = RegInit(0.U.asTypeOf(new EXMEMBundle))
  val mem_wb     = RegInit(0.U.asTypeOf(new MEMWBBundle))

  // Remove these as you hook up each one
  //  control.io    := DontCare
  //  branchCtrl.io := DontCare
  //  registers.io := DontCare
  //  aluControl.io := DontCare
  //  alu.io := DontCare
  //  immGen.io := DontCare
  //  pcPlusFour.io := DontCare
  //  branchAdd.io := DontCare
  //  io.dmem := DontCare
  //  forwarding.io := DontCare
  //  hazard.io := DontCare

  printf("Cycle=%d ", cycleCount)

  // Forward declaration of wires that connect different stages

  // From memory back to fetch. Since we don't decide whether to take a branch or not until the memory stage.
  val next_pc = Wire(UInt(32.W))
  //next_pc    := DontCare    // remove when connected

  // For wb back to other stages
  val write_data = Wire(UInt(32.W))
  //write_data    := DontCare // remove when connected

  /////////////////////////////////////////////////////////////////////////////
  // FETCH STAGE
  /////////////////////////////////////////////////////////////////////////////

  // Note: This comes from the memory stage!
  // Only update the pc if the pcwrite flag is enabled
  when (hazard.io.pcwrite === 0.U) {
    pc := pcPlusFour.io.result
  } .elsewhen (hazard.io.pcwrite === 1.U) {
    pc := next_pc
  } .elsewhen (hazard.io.pcwrite === 2.U) {
    pc := pc
  } .otherwise {
    pc := 0.U
  }

  // Send the PC to the instruction memory port to get the instruction
  io.imem.address := pc

  // Get the PC + 4
  pcPlusFour.io.inputx := 4.U
  pcPlusFour.io.inputy := pc

  // Fill the IF/ID register if we are not bubbling IF/ID
  // otherwise, leave the IF/ID register *unchanged*
  // Output: ifid_bubble, if true, we should instet a bubble in the IF/ID stage
  when (!hazard.io.ifid_bubble) {
    if_id.pc          := pc
    if_id.pcplusfour  := pcPlusFour.io.result
    if_id.instruction := io.imem.instruction
  }
  //Output: ifid_flush, if true, set the IF/ID register to 0
  // default unchanged case
  when (hazard.io.ifid_flush) {
    if_id.instruction := 0.U
    if_id.pc          := 0.U
    if_id.pcplusfour  := 0.U
  }

  printf(p"IF/ID: $if_id\n")

  /////////////////////////////////////////////////////////////////////////////
  // ID STAGE
  /////////////////////////////////////////////////////////////////////////////

  val rs1 = if_id.instruction(19,15)
  val rs2 = if_id.instruction(24,20)

  // Send input from this stage to hazard detection unit
  hazard.io.rs1 := rs1
  hazard.io.rs2 := rs2

  // Send opcode to control
  control.io.opcode := if_id.instruction(6,0)

  // Send register numbers to the register file
  registers.io.readreg1 := rs1
  registers.io.readreg2 := rs2

  // Send the instruction to the immediate generator
  immGen.io.instruction := if_id.instruction

  // FIll the id_ex register
  //  val pcplusfour = UInt(32.W)
  //  val pc         = UInt(32.W)
  //  val sextImm    = UInt(32.W)
  //  val func7      = UInt(7.W)
  //  val func3      = UInt(3.W)
  //  val readdata1  = UInt(5.W)
  //  val readdata2  = UInt(5.W)
  //  val instruction = UInt(32.W)
  /* TWO INSTRUCTION VARIABLES FOR FORWARDING/HAZARDS */

  id_ex.pcplusfour := if_id.pcplusfour
  id_ex.pc         := if_id.pc
  id_ex.sextImm    := immGen.io.sextImm
  id_ex.func7      := if_id.instruction(31,25)
  id_ex.func3      := if_id.instruction(14,12)
  id_ex.readdata1  := registers.io.readdata1
  id_ex.readdata2  := registers.io.readdata2
  id_ex.writereg   := if_id.instruction(11,7)
  id_ex.maskmode   := if_id.instruction(13,12)
  id_ex.sext       := ~if_id.instruction(14)
  id_ex.rs1        := rs1
  id_ex.rs2        := rs2
  /* TWO INSTRUCTION VARIABLES FOR FORWARDING/HAZARDS */

  // Set the execution control signals
  id_ex.excontrol.add := control.io.add
  id_ex.excontrol.immediate := control.io.immediate
  id_ex.excontrol.alusrc1 := control.io.alusrc1
  id_ex.excontrol.branch := control.io.branch
  id_ex.excontrol.jump := control.io.jump

  // Set the memory control signals
  id_ex.mcontrol.memread := control.io.memread
  id_ex.mcontrol.memwrite := control.io.memwrite

  // Set the writeback control signals
  id_ex.wbcontrol.regwrite := control.io.regwrite
  id_ex.wbcontrol.toreg := control.io.toreg

  // Output: idex_bubble, if true, we should insert a bubble in the ID/EX stage
  when (hazard.io.idex_bubble) {
    id_ex.excontrol := 0.U.asTypeOf(new EXControl)
    id_ex.mcontrol  := 0.U.asTypeOf(new MControl)
    id_ex.wbcontrol := 0.U.asTypeOf(new WBControl)
  }

  printf("DASM(%x)\n", if_id.instruction)
  printf(p"ID/EX: $id_ex\n")

  /////////////////////////////////////////////////////////////////////////////
  // EX STAGE
  /////////////////////////////////////////////////////////////////////////////

  // Set the inputs to the hazard detection unit from this stage (SKIP FOR PART I)
  hazard.io.idex_memread := id_ex.mcontrol.memread
  hazard.io.idex_rd      := id_ex.writereg

  // Set the input to the forwarding unit from this stage (SKIP FOR PART I)
  forwarding.io.rs1 := id_ex.rs1
  forwarding.io.rs2 := id_ex.rs2

  // Connect the ALU control wires (line 45 of single-cycle/cpu.scala)
  aluControl.io.add       := id_ex.excontrol.add
  aluControl.io.immediate := id_ex.excontrol.immediate
  aluControl.io.funct7    := id_ex.func7
  aluControl.io.funct3    := id_ex.func3

  // Insert the forward inputx mux here (SKIP FOR PART I)
  val forward_inputx = Wire(UInt())
  when (forwarding.io.forwardA === 0.U) {
    forward_inputx  := id_ex.readdata1
  } .elsewhen (forwarding.io.forwardA === 1.U) {
    forward_inputx  := ex_mem.aluresult
  } .elsewhen (forwarding.io.forwardA === 2.U) {
    forward_inputx  := write_data
  } .otherwise {
    forward_inputx  := 0.U
  }

  // Insert the ALU inputx mux here (line 59 of single-cycle/cpu.scala)
  val alu_inputx = Wire(UInt())
  alu_inputx := DontCare
  switch(id_ex.excontrol.alusrc1) {
    is(0.U) { alu_inputx := forward_inputx }
    is(1.U) { alu_inputx := 0.U }
    is(2.U) { alu_inputx := id_ex.pc }
  }

  alu.io.inputx := alu_inputx

  // Insert forward inputy mux here (SKIP FOR PART I)
  val forward_inputy = Wire(UInt())
  when (forwarding.io.forwardB === 0.U) {
    forward_inputy  := id_ex.readdata2
  } .elsewhen (forwarding.io.forwardB === 1.U) {
    forward_inputy  := ex_mem.aluresult
  } .elsewhen (forwarding.io.forwardB === 2.U) {
    forward_inputy  := write_data
  } .otherwise {
    forward_inputy  := 0.U
  }

  val alu_inputy = Wire(UInt())
  alu_inputy := forward_inputy

  // Input y mux (line 66 of single-cycle/cpu.scala)
  alu.io.inputy := Mux( id_ex.excontrol.immediate, id_ex.sextImm, alu_inputy )

  // Connect the branch control wire (line 54 of single-cycle/cpu.scala)
  branchCtrl.io.branch := id_ex.excontrol.branch
  branchCtrl.io.funct3 := id_ex.func3
  branchCtrl.io.inputx := forward_inputx
  branchCtrl.io.inputy := forward_inputy

  // Set the ALU operation
  alu.io.operation := aluControl.io.operation

  //SHOULD THIS BE IN IF?
  // Connect the branchAdd unit
  branchAdd.io.inputx := id_ex.pc
  branchAdd.io.inputy := id_ex.sextImm

  // Set the EX/MEM register value
  // VARIABLE FROM ADDER/BRANCH CONTROL UNIT BOX MUX
  //val pcplusfour = UInt(32.W)
  //val aluresult  = UInt(32.W)
  // VARIABLE FROM FORWARDING MUX
  // VARIABLE FROM FUNC3
  //val instruction = UInt(32.W)

  // VARIABLE FROM ADDER/BRANCH CONTROL UNIT BOX MUX
  ex_mem.pcplusfour := id_ex.pcplusfour
  ex_mem.aluresult  := alu.io.result
  ex_mem.readdata2  := alu_inputy
  ex_mem.writereg   := id_ex.writereg
  ex_mem.maskmode   := id_ex.maskmode
  ex_mem.sext       := id_ex.sext

  // VARIABLE FROM FORWARDING MUX

  ex_mem.mcontrol.memread := id_ex.mcontrol.memread
  ex_mem.mcontrol.memwrite := id_ex.mcontrol.memwrite

  ex_mem.wbcontrol.regwrite := id_ex.wbcontrol.regwrite
  ex_mem.wbcontrol.toreg := id_ex.wbcontrol.toreg

  // Calculate whether which PC we should use and set the taken flag (line 92 in single-cycle/cpu.scala)
  when (id_ex.excontrol.jump(1)) {
    when (id_ex.excontrol.jump(0) === 0.U) {
      ex_mem.nextpc := branchAdd.io.result
      ex_mem.taken := true.B
    } .otherwise {
      ex_mem.nextpc := alu.io.result & Cat(Fill(31, 1.U), 0.U)
      ex_mem.taken := true.B
    }
  } .otherwise {
    when (branchCtrl.io.taken && id_ex.excontrol.branch) {
      when (branchCtrl.io.taken) { ex_mem.nextpc := branchAdd.io.result }
        .otherwise { ex_mem.nextpc := id_ex.pcplusfour }
      ex_mem.taken := true.B
    } .otherwise {
      ex_mem.taken  := false.B
      ex_mem.nextpc := DontCare
    }
  }


  // Output: exmem_bubble, if true, we should insert a bubble in the ID/EX stage
  when (hazard.io.exmem_bubble) {
    ex_mem.mcontrol  := 0.U.asTypeOf(new MControl)
    ex_mem.wbcontrol := 0.U.asTypeOf(new WBControl)
  }


  printf(p"EX/MEM: $ex_mem\n")

  ///////////////////////////////////cv//////////////////////////////////////////
  // MEM STAGE
  /////////////////////////////////////////////////////////////////////////////

  // Set data memory IO (line 71 of single-cycle/cpu.scala)
  io.dmem.address   := ex_mem.aluresult
  io.dmem.writedata := ex_mem.readdata2
  io.dmem.memread   := ex_mem.mcontrol.memread
  io.dmem.memwrite  := ex_mem.mcontrol.memwrite
  io.dmem.maskmode  := ex_mem.maskmode
  io.dmem.sext      := ex_mem.sext

  // Send next_pc back to the fetch stage
  next_pc := ex_mem.nextpc

  // Send input signals to the hazard detection unit (SKIP FOR PART I)
  hazard.io.exmem_taken := ex_mem.taken

  // Send input signals to the forwarding unit (SKIP FOR PART I)
  forwarding.io.exmemrd := ex_mem.writereg
  forwarding.io.exmemrw := ex_mem.wbcontrol.regwrite

  // Wire the MEM/WB register
  mem_wb.wbcontrol.regwrite := ex_mem.wbcontrol.regwrite
  mem_wb.wbcontrol.toreg := ex_mem.wbcontrol.toreg

  // Set the MEM/WB register value
  //val pcplusfour = UInt(32.W)
  //val readdata   = UInt(32.W)
  //val aluresult  = UInt(32.W)
  //val instruction = UInt(32.W)

  mem_wb.pcplusfour := ex_mem.pcplusfour
  mem_wb.readdata := io.dmem.readdata
  mem_wb.aluresult := ex_mem.aluresult
  mem_wb.writereg   := ex_mem.writereg
  // VARIABLE FROM INSTRUCTION

  printf(p"MEM/WB: $mem_wb\n")

  /////////////////////////////////////////////////////////////////////////////
  // WB STAGE
  /////////////////////////////////////////////////////////////////////////////

  // Set the writeback data mux (line 78 single-cycle/cpu.scala)
  when (mem_wb.wbcontrol.toreg === 0.U) {
    write_data := mem_wb.aluresult
  } .elsewhen (mem_wb.wbcontrol.toreg === 1.U) {
    write_data := mem_wb.readdata
  } .elsewhen (mem_wb.wbcontrol.toreg === 2.U) {
    write_data := mem_wb.pcplusfour
  } .otherwise {
    write_data := mem_wb.aluresult
  }


  // Write the data to the register file
  registers.io.writedata := write_data
  registers.io.wen := mem_wb.wbcontrol.regwrite && (registers.io.writereg =/= 0.U)
  registers.io.writereg := mem_wb.writereg

  // Set the input signals for the forwarding unit (SKIP FOR PART I)
  forwarding.io.memwbrd := mem_wb.writereg
  forwarding.io.memwbrw := mem_wb.wbcontrol.regwrite

  printf("---------------------------------------------\n")
}
