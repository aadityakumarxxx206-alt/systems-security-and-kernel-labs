# Reverse Engineering & Low-Level Engineering — Day 11

## Assembly Language Theory: Control Flow, CPU Flags, and Function Execution

### Overview

This study session focuses on fundamental x86-64 Assembly concepts used in reverse engineering and low-level program analysis. It covers comparison instructions, conditional and unconditional branching, bitwise flag testing, signed comparisons, function calls, return mechanisms, and instruction opcodes.

Understanding these concepts is essential for analyzing compiler-generated machine code, reconstructing program logic, and identifying control-flow structures during debugging and disassembly.

---

## 1. CMP — Compare Instruction

The `CMP` instruction compares two operands by internally performing subtraction for flag calculation without modifying either operand.

### Key Characteristics

- Performs subtraction-based flag calculation.
- Does not store the subtraction result in the destination operand.
- Updates relevant CPU status flags according to the subtraction result.
- Commonly used with conditional jump instructions.
- Supports both signed and unsigned comparison logic through different flag interpretations.

### Important Flags

- **ZF (Zero Flag):** Set when the subtraction result is zero.
- **SF (Sign Flag):** Reflects the most significant bit of the subtraction result.
- **CF (Carry Flag):** Indicates an unsigned borrow condition for subtraction.
- **OF (Overflow Flag):** Indicates signed arithmetic overflow.

The interpretation of these flags determines the outcome of subsequent conditional branches.

---

## 2. Conditional and Unconditional Jump Instructions

Jump instructions modify the instruction pointer and control the sequence in which machine instructions execute.

### 2.1 JNE — Jump if Not Equal

`JNE` transfers execution to its target when the Zero Flag is clear (`ZF = 0`).

- **ZF = 0:** The branch is taken.
- **ZF = 1:** The branch is not taken.

Following a comparison, this condition commonly represents inequality.

### 2.2 JE — Jump if Equal

`JE` transfers execution to its target when the Zero Flag is set (`ZF = 1`).

- **ZF = 1:** The branch is taken.
- **ZF = 0:** The branch is not taken.

Following a comparison, this condition commonly represents equality.

### 2.3 JMP — Unconditional Jump

`JMP` transfers execution directly to its specified target without evaluating a conditional flag requirement.

Key characteristics:

- Does not depend on the Zero Flag or other status flags.
- Changes the normal sequential flow of instruction execution.
- Supports control-flow transfers between basic blocks.
- Can implement loops, branch structures, and control-flow redirection.

Conditional and unconditional jumps are essential for understanding the control-flow graph of a compiled program.

---

## 3. TEST — Logical Testing Instruction

The `TEST` instruction performs a bitwise AND operation between its operands to update status flags without storing the resulting value.

### Key Characteristics

- Performs a bitwise AND for flag calculation.
- Does not modify the source operands.
- Clears the Carry Flag and Overflow Flag.
- Updates the Zero Flag and Sign Flag according to the logical result.
- The Parity Flag is also updated based on the result.
- The instruction is frequently used to examine whether specific bits are set.

When used to test a register against itself, the Zero Flag indicates whether the register contains zero.

This makes `TEST` a common instruction for analyzing zero checks, bit masks, and conditional execution in compiler-generated assembly.

---

## 4. CMP + JG — Signed Greater-Than Comparison

`JG` stands for **Jump if Greater** and is used for signed comparisons.

After a comparison, the branch is taken when both conditions are satisfied:

- `ZF = 0`
- `SF = OF`

The Zero Flag indicates whether the compared values are equal, while the Sign Flag and Overflow Flag together determine the signed ordering condition.

### Important Distinction

Signed and unsigned comparisons interpret CPU flags differently.

- **Signed comparisons:** Commonly use `JG`, `JGE`, `JL`, and `JLE`.
- **Unsigned comparisons:** Commonly use `JA`, `JAE`, `JB`, and `JBE`.

Choosing the correct conditional jump is essential when reconstructing the intended logic of a compiled program.

---

## 5. CALL and RET — Function Execution

Function calls and returns provide the fundamental mechanism for transferring execution between functions.

### 5.1 CALL Instruction

The `CALL` instruction transfers execution to a target procedure and saves a return address so that execution can resume after the call.

For a near call in x86-64 mode:

- The return address is pushed onto the stack.
- The stack pointer (`RSP`) decreases by eight bytes for this stack operation.
- Control transfers to the target function.
- The saved return address identifies the instruction following the call.

The exact encoding and target-address calculation depend on the form of the `CALL` instruction.

### 5.2 RET Instruction

The `RET` instruction returns control to the address stored at the top of the stack.

For a near return in 64-bit mode:

- The return address is retrieved from the stack.
- The instruction pointer is updated with that address.
- The stack pointer increases by eight bytes as the return address is removed.
- Execution resumes at the saved return location.

A correct return operation depends on the expected stack layout and calling convention.

---

## 6. Instruction Opcodes and Machine-Code Encoding

An opcode identifies the operation represented by a machine instruction. However, an instruction's complete machine-code encoding may contain additional fields, bytes, and operands.

### Common Opcode Encodings

| Instruction Form | Encoding |
|---|---|
| Near `RET` | `C3` |
| Near relative `CALL` | `E8` |
| Near relative `JMP` | `E9` |
| Short `JE` | `74` |
| Short `JNE` | `75` |

### Important Encoding Notes

- The listed values identify common instruction encodings, not every possible encoding of each instruction.
- Short conditional jumps use an 8-bit signed relative displacement.
- Near relative jumps and calls use a relative displacement encoded according to the instruction form and execution mode.
- `CALL` and `JMP` also support other encoding forms.
- `CMP` and `TEST` have multiple encodings depending on operand types, operand sizes, and instruction form.
- Instruction prefixes, ModR/M bytes, SIB bytes, and displacement or immediate fields may be part of the complete encoding.

In reverse engineering, opcode identification must be combined with correct instruction decoding to determine the actual operation and its operands.

---

## 7. Importance in Reverse Engineering

These instructions provide the foundation for interpreting low-level program behavior.

Understanding them helps with:

- Reconstructing conditional statements and loops.
- Identifying function boundaries and call relationships.
- Understanding stack-based return mechanisms.
- Analyzing signed and unsigned comparisons.
- Interpreting CPU status flags during debugging.
- Connecting disassembled instructions with higher-level program logic.
- Examining machine-code bytes and instruction encodings.
- Building control-flow graphs from disassembled code.

A strong understanding of these fundamentals makes it easier to analyze compiler-generated code in tools such as Ghidra and x64dbg.

---

## Conclusion

Day 11 focused on the theoretical foundations of comparison instructions, CPU status flags, branching, function calls, return operations, and machine-code encoding in x86-64 Assembly.

These concepts form an essential foundation for advanced disassembly analysis, binary inspection, and reverse engineering.

**Learning Track:** Reverse Engineering & Low-Level Engineering  
**Day:** 11  
**Focus:** x86-64 Assembly, Control Flow, CPU Flags, and Instruction Encoding
