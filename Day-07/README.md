
# Day 07 — x64 Assembly & Reverse Engineering

## Function Calls, Stack Operations, Memory Inspection & Control Flow

**Learning Track:** Reverse Engineering & Low-Level Engineering  
**Day:** 07  
**Focus:** x86-64 Assembly, CPU Registers, Stack Memory, and Control Flow  
**Tools:** C++, GCC, VS Code, x64dbg

---

## 1. Objectives

On Day 7, I reviewed and organized essential x86-64 assembly concepts used in debugging and reverse engineering.

The main objectives were to understand:

- Function prologues and epilogues
- Function calls using `CALL` and `RET`
- The purpose of `NOP` and `INT 3`
- Stack memory and local-variable storage
- Hexadecimal values and little-endian representation
- The `CMP` instruction and CPU flags
- Conditional and unconditional jumps
- Signed versus unsigned comparisons
- Effective memory-address calculation
- Practical debugging workflows in x64dbg

---

## 2. Function Prologue and Epilogue

### 2.1 Function Prologue

A function prologue commonly prepares a stack frame when a function begins execution.

**Example:**

```asm
push rbp
mov rbp, rsp
sub rsp, 0x20
```

**Instruction breakdown:**

| Instruction | Purpose |
|---|---|
| `push rbp` | Saves the previous RBP value on the stack. |
| `mov rbp, rsp` | Establishes RBP as a reference point for the stack frame. |
| `sub rsp, 0x20` | Reserves 32 bytes of stack space by moving RSP downward. |

This is a common pattern, not a mandatory sequence for every function. Compiler optimizations and calling conventions can change the generated code.

### 2.2 Function Epilogue

A function epilogue restores the stack frame before returning to the caller.

**Example:**

```asm
leave
ret
```

The `leave` instruction is equivalent in effect to:

```asm
mov rsp, rbp
pop rbp
```

The `ret` instruction obtains the saved return address from the stack and transfers execution back to the caller.

**Key takeaway:**

- Prologue → Establishes the function's stack frame.
- Epilogue → Restores the stack frame.
- `CALL` and `RET` → Manage function-call control flow.

---

## 3. CALL, RET, and JMP

### 3.1 CALL — Function Call

```asm
call calculate
```

`CALL` transfers execution to the target function and saves the return address for a near call.

### 3.2 RET — Return

`RET` uses the saved return address to resume execution at the caller's return point.

### 3.3 JMP — Unconditional Jump

```asm
jmp target
```

`JMP` transfers execution directly to the specified target without automatically saving a return address.

### Instruction Comparison

| Instruction | Purpose |
|---|---|
| `CALL` | Transfers control to a function and saves a return address. |
| `RET` | Returns using the saved return address. |
| `JMP` | Transfers control directly to a target. |

**Mental model:**

```text
CALL → Save return address → Enter function
RET  → Retrieve return address → Resume caller
JMP  → Transfer directly to target
```

---

## 4. NOP and INT 3

### 4.1 NOP — No Operation

```asm
nop
```

- Common one-byte opcode: `90`
- Performs no meaningful general-purpose data calculation.
- May appear as padding or alignment in executable code.

### 4.2 INT 3 — Breakpoint Instruction

```asm
int3
```

- One-byte opcode: `CC`
- Generates a breakpoint exception.
- Debuggers may use it to implement software breakpoints.

**Important:** A byte containing `CC` does not automatically prove that a debugger breakpoint is active. Its location and execution context must also be examined.

---

## 5. Memory Inspection and the Dump Window

Memory addresses identify locations where data is stored. Memory values represent the data stored at those locations.

Consider this instruction:

```asm
mov dword ptr [rbp-0x4], eax
```

Assume:

```text
RBP = 0x1000
EAX = 14 decimal = 0x0E
```

### 5.1 Calculate the Effective Address

```text
Address = RBP - 0x4
        = 0x1000 - 0x4
        = 0x0FFC
```

The instruction stores a 32-bit value at address `0x0FFC`.

### 5.2 Little-Endian Representation

Decimal `14` is hexadecimal `0E`.

Its 32-bit little-endian representation is:

```text
0E 00 00 00
```

The least significant byte is stored at the lowest memory address.

### 5.3 Data Sizes

| Data Unit | Size |
|---|---:|
| BYTE | 8 bits |
| WORD | 16 bits |
| DWORD | 32 bits |
| QWORD | 64 bits |

**Debugging reminder:** Always check the operand size before interpreting bytes in the Dump window.

---

## 6. Hexadecimal Number System

Hexadecimal is a base-16 number system.

Its digits are:

```text
0 1 2 3 4 5 6 7 8 9 A B C D E F
```

### Decimal-to-Hexadecimal Examples

| Decimal | Hexadecimal |
|---:|---:|
| 10 | `A` |
| 14 | `E` |
| 15 | `F` |
| 16 | `10` |

### Hexadecimal Addition

```text
Hexadecimal: 8 + 8 = 10
Decimal:     8 + 8 = 16
```

Hexadecimal `10` represents decimal `16`, not decimal `10`.

Understanding hexadecimal is essential when examining registers, instruction bytes, memory addresses, and debugger output.

---

## 7. CMP and CPU Flags

The `CMP` instruction compares two operands by performing a conceptual subtraction and updating the relevant CPU flags.

**Example:**

```asm
cmp eax, ebx
```

Conceptually:

```text
EAX - EBX
```

`CMP` does not store the subtraction result in EAX. Instead, it updates flags according to the comparison.

### 7.1 Zero Flag (ZF)

When both operands are equal:

```text
EAX = 5
EBX = 5

cmp eax, ebx

ZF = 1
```

When the operands differ:

```text
EAX = 5
EBX = 3

cmp eax, ebx

ZF = 0
```

**Important:** `CMP` can compare two registers, a register and an immediate value, or other permitted operand combinations. It does not always compare a value against zero.

### 7.2 Important CPU Flags

| Flag | Meaning |
|---|---|
| ZF | Set when the conceptual subtraction result is zero. |
| CF | Indicates an unsigned carry or borrow condition. |
| SF | Reflects the sign bit of the result. |
| OF | Indicates signed arithmetic overflow. |

The interpretation of these flags depends on the instruction and the condition being evaluated.

---

## 8. Effective Memory Address Calculation

x86-64 instructions can calculate memory addresses using a base register, an index register, a scale factor, and an optional displacement.

**Example:**

```asm
mov eax, dword ptr [rsi + rax*8]
```

The effective address is calculated as:

```text
RSI + (RAX × 8)
```

Assume:

```text
RSI = 0x1000
RAX = 2
```

Calculation:

```text
Address = 0x1000 + (2 × 8)
        = 0x1000 + 0x10
        = 0x1010
```

The Dump window can be used to inspect memory at address `0x1010`.

**Important distinction:**

Calculating an effective address identifies a memory location. It does not, by itself, reveal the value stored there.

---

## 9. Conditional and Unconditional Jumps

### 9.1 Unconditional Jump

```asm
jmp target
```

Execution transfers to the target without checking a condition.

### 9.2 JE — Jump if Equal

```asm
cmp eax, ebx
je equal_label
```

Condition:

```text
ZF = 1
```

The jump is taken when the Zero Flag is set.

### 9.3 JNE — Jump if Not Equal

```asm
cmp eax, ebx
jne not_equal_label
```

Condition:

```text
ZF = 0
```

The jump is taken when the Zero Flag is clear.

### 9.4 Signed Comparison Jumps

These instructions interpret the comparison as a signed comparison.

| Instruction | Meaning |
|---|---|
| `JG` | Signed greater than |
| `JGE` | Signed greater than or equal |
| `JL` | Signed less than |
| `JLE` | Signed less than or equal |

### 9.5 Unsigned Comparison Jumps

These instructions interpret the comparison as an unsigned comparison.

| Instruction | Meaning |
|---|---|
| `JA` | Unsigned above |
| `JAE` | Unsigned above or equal |
| `JB` | Unsigned below |
| `JBE` | Unsigned below or equal |

**Key distinction:** Signed and unsigned comparisons interpret the same bit patterns differently. The correct jump depends on how the operands are intended to be interpreted.

---

## 10. Practical Debugging Checklist

When analyzing assembly in x64dbg:

1. Locate the instruction being analyzed.
2. Inspect the relevant CPU registers.
3. Calculate effective memory addresses when necessary.
4. Inspect the corresponding bytes in the Dump window.
5. Confirm the operand size.
6. Check CPU flags after comparison instructions.
7. Follow execution to understand the control-flow path.
8. Compare the assembly behavior with the original C++ code.

### Debugging Workflow

```text
Assembly Instruction
        ↓
Register State
        ↓
Effective Memory Address
        ↓
Memory Bytes
        ↓
CPU Flags
        ↓
Control Flow
        ↓
Program Behavior
```

---

## 11. Key Learnings — Day 7

Today, I reviewed and organized the relationship between assembly instructions, stack operations, memory representation, CPU flags, and control flow.

Key takeaways:

- `CALL` and `RET` manage function-call control flow.
- Function prologues and epilogues may manage stack frames.
- `NOP` commonly uses opcode `90`.
- `INT 3` uses opcode `CC`.
- Little-endian storage places the least significant byte first.
- `CMP` updates flags without storing its subtraction result.
- `JE` and `JNE` evaluate equality-related flag conditions.
- Signed and unsigned comparisons use different conditional jumps.
- The Dump window helps inspect memory at calculated addresses.
- Register values, memory bytes, and control flow must be interpreted together.

---

## 12. Next Step

Continue with practical debugging exercises involving:

- `CMP` and conditional jumps
- CPU register values and flag changes
- Effective memory-address calculations
- Stack-memory inspection
- Comparing C++ conditions with their assembly-level behavior

The goal is to connect each assembly instruction to actual program execution in a debugger.

---

**Day 7 — Review & Documentation Complete**  
**Learning Track:** Reverse Engineering & Low-Level Engineering
