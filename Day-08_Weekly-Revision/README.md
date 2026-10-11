# Reverse Engineering & Low-Level Engineering
## Day 08 — Complete Theory Revision (Days 01–07)

**Focus:** C++ Execution, x86-64 Assembly, CPU Registers, Memory, Stack Frames, Control Flow, Bitwise Operations, and Reverse Engineering.

---

## Table of Contents

1. Program Execution Pipeline
2. Program vs. Process
3. Machine Code and Assembly Language
4. CPU Architecture and General-Purpose Registers
5. Instruction Pointer and Program Execution
6. Stack Memory and Stack Operations
7. Function Calls and Return Addresses
8. Function Prologue and Epilogue
9. Stack Frames and Local Variables
10. Decimal, Binary, and Hexadecimal Number Systems
11. Memory Addresses and Addressing Modes
12. Little-Endian Byte Ordering
13. Arithmetic Instructions
14. CMP Instruction and CPU Flags
15. TEST Instruction and Zero Detection
16. Conditional and Unconditional Jumps
17. Signed vs. Unsigned Comparisons
18. Bitwise Operations
19. Function Arguments and Calling Conventions
20. Function Return Values
21. Executables, Debugging, and Reverse Engineering
22. Connecting Source Code to Assembly
23. Complete Execution Walkthrough
24. Common Misconceptions
25. Final Revision Summary

---

# 1. Program Execution Pipeline

A C++ program does not normally execute directly from its human-readable source code. It passes through several stages before the CPU executes its machine instructions.

### Execution Pipeline

C++ Source Code
→ Preprocessing
→ Compilation
→ Assembly
→ Linking
→ Executable File
→ Operating System Loader
→ Process Creation and Initialization
→ CPU Instruction Execution

### 1.1 Source Code

Source code is the human-readable representation of a program written in a language such as C++.

Example:

```cpp
int add(int a, int b) {
    return a + b;
}
```

This code describes an operation, but the CPU does not directly execute the C++ syntax.

### 1.2 Preprocessing

The preprocessor handles directives such as:

```cpp
#include <cstdio>
#define MAX_VALUE 100
```

It processes header inclusions, macro substitutions, and conditional compilation directives before the main compilation stage.

### 1.3 Compilation and Assembly

The compiler translates the program into lower-level representations and eventually machine instructions.

An assembly representation might contain:

```asm
mov eax, 17
add eax, 25
```

These instructions are examples of x86-64 assembly. Their actual appearance depends on the compiler, target architecture, optimization settings, and calling convention.

### 1.4 Linking

The linker combines object files and resolves references between program components and external libraries.

It produces an executable or another supported binary format.

### 1.5 Loading and Execution

The operating system loads the executable, establishes the process's virtual address space, maps required executable and library sections, and performs necessary initialization.

The CPU then executes instructions according to the processor architecture and current execution state.

**Key concept:** Understanding reverse engineering requires connecting source-level behavior to the instructions and runtime state produced by the compiler and operating system.

---

# 2. Program vs. Process

A program and a process are related, but they are not the same thing.

### Program

A program is a set of instructions and associated data stored in an executable file.

Example:

`day08_lab.exe`

The executable is a file on disk.

### Process

A process is a running instance of a program managed by the operating system.

A process has execution state and operating-system-managed resources, including:

- A virtual address space
- Thread execution contexts
- Stack memory
- Mapped executable and library sections
- Handles and other operating-system resources

A process may contain multiple threads. Each thread has its own execution context and stack.

### Comparison

| Program | Process |
|---|---|
| Stored executable or program representation | Running instance |
| Exists as a file or other stored representation | Exists as an operating-system-managed execution context |
| Contains instructions and data | Has runtime state and resources |
| Can be launched multiple times | Each launch can create a separate process |

**Reverse engineering connection:** Static analysis examines executable files without requiring the program to run. Dynamic analysis examines a running process and its behavior.

---

# 3. Machine Code and Assembly Language

Machine code consists of instruction encodings that a processor can execute.

Assembly language provides a human-readable representation of those instructions.

Example:

```asm
mov eax, 17
add eax, 25
```

The first instruction places the immediate value 17 into EAX.

The second instruction adds 25 to EAX.

The final result is:

`EAX = 42 = 0x2A`

### Important distinction

Assembly instructions are not necessarily a direct, one-to-one representation of C++ statements.

A single source statement can produce several instructions. Conversely, the compiler can combine, simplify, or eliminate source-level operations.

For example, the compiler may replace an addition with a different equivalent instruction sequence.

**Reverse engineering principle:** Analyze what the instructions actually do rather than assuming that each source statement has a predictable assembly representation.

---

# 4. CPU Architecture and General-Purpose Registers

Registers are small, high-speed storage locations inside the CPU.

The x86-64 architecture provides several general-purpose registers, instruction-pointer state, flags, and other architectural state.

## 4.1 Common x86-64 Registers

| Register | Common Role |
|---|---|
| RAX | General-purpose register; frequently used for integer return values |
| RBX | General-purpose register |
| RCX | General-purpose register; argument register under the Windows x64 ABI |
| RDX | General-purpose register; argument register under the Windows x64 ABI |
| RSI | General-purpose register |
| RDI | General-purpose register |
| RBP | General-purpose register; often used as a frame pointer |
| RSP | Stack pointer |
| R8–R15 | Additional general-purpose registers |
| RIP | Instruction pointer |
| RFLAGS | Processor status and control flags |

A register's use depends on the instruction and the applicable calling convention.

## 4.2 Register Widths

Some registers have names for their smaller portions.

For example:

- RAX: 64-bit
- EAX: lower 32 bits of RAX
- AX: lower 16 bits
- AL: lower 8 bits

Similarly:

- RBX, EBX, BX, BL
- RCX, ECX, CX, CL
- RDX, EDX, DX, DL

In 64-bit mode, writing to a 32-bit general-purpose register such as EAX clears the upper 32 bits of the corresponding 64-bit register.

Example:

```asm
mov eax, 42
```

After this instruction:

`RAX = 0x000000000000002A`

This zero-extension behavior is specific to 32-bit general-purpose register writes in 64-bit mode; smaller-width writes behave differently.

---

# 5. Instruction Pointer and Program Execution

RIP is the instruction pointer in x86-64 mode.

It identifies the address associated with the next instruction to be executed.

Consider:

```asm
mov eax, 17
add eax, 25
ret
```

The CPU executes the instructions according to the control flow of the program.

RIP changes as instructions execute.

For ordinary sequential execution, it advances to the next instruction. For control-transfer instructions, such as JMP, CALL, and RET, it changes according to the instruction's semantics.

### Debugging Application

When stepping through instructions in x64dbg:

1. Observe the current instruction associated with RIP.
2. Execute one instruction.
3. Observe the updated register values.
4. Check whether execution continued sequentially or transferred elsewhere.

**Key concept:** RIP connects the CPU's current execution state to the disassembled instructions displayed by the debugger.

---

# 6. Stack Memory and Stack Operations

The stack is a memory region commonly used for function calls, saved registers, return addresses, and local storage.

On conventional x86-64 systems, the stack generally grows toward lower virtual addresses as space is allocated.

## 6.1 RSP — Stack Pointer

RSP identifies the current top of the architectural stack.

The exact interpretation depends on the operation being performed, but it is the register used by instructions such as PUSH, POP, CALL, and RET to interact with the stack.

## 6.2 PUSH

Example:

```asm
push rbp
```

This saves the previous RBP value on the stack and adjusts RSP.

In 64-bit mode, this operation normally decreases RSP by eight bytes.

Conceptually:

`RSP_new = RSP_old - 8`

The saved value is written at the new stack location.

## 6.3 POP

Example:

```asm
pop rbp
```

This loads a value from the stack into RBP and advances RSP by eight bytes in 64-bit mode.

Conceptually:

`RSP_new = RSP_old + 8`

### Important distinction

The stack is not the same thing as the heap.

- The stack is commonly used for call-related state and temporary storage.
- The heap is commonly used for dynamically allocated objects and memory.

Both are part of the process's virtual address space, and their implementation and management involve the operating system and runtime.

---

# 7. Function Calls and Return Addresses

The CALL and RET instructions support function-call control flow.

## 7.1 CALL

Example:

```asm
call calculateSum
```

A near CALL in 64-bit mode saves the address of the instruction following the call on the stack and transfers execution to the call target.

The saved address is the return address.

CALL is therefore both a control-transfer operation and a stack operation.

## 7.2 RET

Example:

```asm
ret
```

RET obtains the return address from the stack and transfers execution back to that address.

In a typical near return, the return address is popped into RIP.

### Function Call Sequence

Caller executes CALL
→ Return address is saved
→ Execution enters the called function
→ Function performs its operations
→ RET transfers execution to the return address
→ Caller continues

### Important distinction

RET does not normally terminate the entire program.

It returns from the current function. Program termination is a separate process involving runtime and operating-system mechanisms.

---

# 8. Function Prologue and Epilogue

A function prologue prepares the function's execution environment.

A function epilogue restores the necessary state before returning.

These are common compiler-generated patterns, not mandatory sequences for every function.

## 8.1 Common Prologue

```asm
push rbp
mov rbp, rsp
sub rsp, 0x20
```

### Instruction 1: PUSH RBP

Saves the previous RBP value.

### Instruction 2: MOV RBP, RSP

Establishes a reference point for the current stack frame.

### Instruction 3: SUB RSP, 0x20

Moves RSP downward by 32 bytes, reserving stack space.

The reserved area may support local variables, temporary values, alignment requirements, or other compiler needs.

## 8.2 Common Epilogue

```asm
mov rsp, rbp
pop rbp
ret
```

### MOV RSP, RBP

Restores RSP to the stack-frame reference point.

### POP RBP

Restores the previous RBP value.

### RET

Transfers execution to the saved return address.

An alternative common pattern is:

```asm
leave
ret
```

LEAVE performs the conventional frame teardown associated with restoring RSP and RBP.

### Compiler Variations

Optimized functions may:

- Omit the frame pointer
- Avoid allocating a conventional stack frame
- Keep local values in registers
- Use different stack-allocation sizes
- Use different epilogue sequences

**Reverse engineering principle:** Recognize common patterns, but verify the actual instructions in the binary.

---

# 9. Stack Frames and Local Variables

A stack frame is a useful conceptual description of the stack-related state associated with a function invocation.

Consider:

```cpp
int calculateSum(int a, int b) {
    int sum = a + b;
    return sum;
}
```

At the source level, the function has two parameters and one local variable.

A compiler may store `sum` in a register or on the stack.

If a local variable is stored relative to RBP, its address may look like:

```asm
mov dword ptr [rbp-4], eax
```

This writes four bytes from EAX into memory at the address RBP minus four.

Another example:

```asm
mov eax, dword ptr [rbp-8]
```

This reads a 32-bit value from memory at RBP minus eight.

## 9.1 Memory Operand Notation

Square brackets indicate a memory access in Intel-style assembly syntax.

Examples:

- `eax`: the register itself
- `[rax]`: memory at the address stored in RAX
- `[rbp-4]`: memory four bytes below the address in RBP
- `[rax+rcx*4+0x10]`: memory at a computed effective address

### Important distinction

`RBP-4` is an address calculation, not the stored value itself.

Also, local variables are not guaranteed to occupy fixed offsets. Their locations depend on the compiler, optimization settings, register allocation, and stack-frame layout.

---

# 10. Decimal, Binary, and Hexadecimal Number Systems

Reverse engineering frequently requires switching between decimal values, binary representations, and hexadecimal values.

## 10.1 Decimal

Decimal uses base 10 and the digits 0–9.

Example:

`42`

## 10.2 Binary

Binary uses base 2 and the digits 0 and 1.

Example:

`42 decimal = 00101010 binary`

## 10.3 Hexadecimal

Hexadecimal uses base 16:

- 0–9 represent values zero through nine.
- A–F represent values ten through fifteen.

Example:

`42 decimal = 0x2A`

Because:

`2 × 16 + 10 = 42`

## 10.4 Common Conversions

| Decimal | Hexadecimal |
|---:|---:|
| 10 | 0x0A |
| 15 | 0x0F |
| 16 | 0x10 |
| 17 | 0x11 |
| 25 | 0x19 |
| 32 | 0x20 |
| 42 | 0x2A |
| 48 | 0x30 |
| 64 | 0x40 |
| 255 | 0xFF |

The `0x` prefix commonly indicates a hexadecimal integer in C and C++ source code.

Debugger displays frequently use hexadecimal by default for registers and memory addresses.

**Reverse engineering principle:** Be comfortable translating between decimal values and hexadecimal representations without confusing the two.

---

# 11. Memory Addresses and Addressing Modes

A memory address identifies a location in the process's virtual address space.

Assembly instructions can access memory using several addressing forms.

## 11.1 Direct Register Operand

```asm
mov eax, ecx
```

The instruction copies a value from ECX to EAX.

No memory read is required for the source operand.

## 11.2 Register-Based Memory Access

```asm
mov eax, dword ptr [rbx]
```

This reads four bytes from the memory address contained in RBX.

## 11.3 Base Plus Displacement

```asm
mov eax, dword ptr [rbp-4]
```

The effective address is calculated by adding a displacement to the base register.

## 11.4 Base Plus Index and Scale

```asm
mov eax, dword ptr [rax+rcx*4+0x10]
```

The effective address is:

`RAX + RCX × 4 + 0x10`

This type of addressing is useful for arrays, structures, and indexed data.

For example, multiplying an array index by four is common when accessing 32-bit elements.

### Structure and Object Fields

A structure's fields may be accessed using offsets from a base address.

If a field is located at offset 0x10, an instruction may access it using a base-plus-displacement memory operand.

These patterns can help identify:

- Array indexing
- Structure fields
- Object members
- Pointer-based data access

Actual field offsets depend on layout, alignment, and compiler decisions.

---

# 12. Little-Endian Byte Ordering

Endianness determines how the bytes of a multibyte value are arranged in memory.

On conventional x86-64 systems, multibyte integer values use little-endian byte ordering.

The least significant byte is stored at the lowest address.

Consider:

`42 decimal = 0x0000002A`

A 32-bit representation contains four bytes:

`00 00 00 2A`

In little-endian memory, the bytes appear in this order:

`2A 00 00 00`

Conceptually:

| Address | Byte |
|---|---|
| Base address + 0 | `2A` |
| Base address + 1 | `00` |
| Base address + 2 | `00` |
| Base address + 3 | `00` |

### Why This Matters

A debugger may display:

- EAX as `0000002A`
- A four-byte memory region as `2A 00 00 00`

These displays are consistent because the register presents the integer value while the memory view presents the individual bytes in increasing address order.

**Key concept:** Little-endian ordering applies to bytes in memory, not to the visual ordering of hexadecimal digits in a register display.

---

# 13. Arithmetic Instructions

Arithmetic instructions operate on values stored in registers, memory, or immediate operands, subject to the instruction's operand rules.

## 13.1 MOV

```asm
mov eax, 17
```

Copies the immediate value 17 into EAX.

MOV transfers data; it does not perform arithmetic addition.

## 13.2 ADD

```asm
mov eax, 17
add eax, 25
```

The result is:

`EAX = 42 = 0x2A`

ADD updates arithmetic flags according to the result.

## 13.3 SUB

```asm
mov eax, 42
sub eax, 17
```

The result is:

`EAX = 25 = 0x19`

SUB also updates flags.

## 13.4 LEA

```asm
lea eax, [rcx+rdx]
```

LEA calculates an effective address expression and places the resulting integer value in the destination register.

It does not read the memory located at that effective address.

Compilers may use LEA for arithmetic expressions as well as address calculations.

### Important distinction

An assembly instruction's effect must be interpreted using its operand types and instruction semantics.

For example:

- `mov eax, [rbx]` reads memory.
- `mov eax, ebx` copies a register value.
- `lea eax, [rbx+4]` calculates an address expression.

---

# 14. CMP Instruction and CPU Flags

CMP compares two operands by performing subtraction for flag-setting purposes without storing the subtraction result in the destination operand.

Example:

```asm
cmp eax, 42
```

This compares EAX against 42 and updates the relevant status flags.

Important flags include:

- ZF — Zero Flag
- CF — Carry Flag
- SF — Sign Flag
- OF — Overflow Flag

The flags are stored in the RFLAGS architectural register.

## 14.1 Zero Flag

ZF is set when the relevant result of an instruction is zero.

For CMP, equality between the operands sets ZF.

Example:

```asm
mov eax, 42
cmp eax, 42
```

Result:

`ZF = 1`

Now consider:

```asm
mov eax, 41
cmp eax, 42
```

Result:

`ZF = 0`

## 14.2 Other Important Flags

### Carry Flag (CF)

CF records carry or borrow information for relevant arithmetic operations and supports unsigned comparisons.

### Sign Flag (SF)

SF reflects the most significant bit of the relevant result.

### Overflow Flag (OF)

OF indicates signed arithmetic overflow according to the operation's rules.

### Why Flags Matter

Conditional jump instructions inspect flags to determine whether execution should branch.

The CMP instruction and a following conditional jump often form the low-level representation of an if statement.

---

# 15. TEST Instruction and Zero Detection

TEST performs a bitwise AND between its operands for flag-setting purposes without storing the result.

Example:

```asm
test eax, eax
```

The instruction tests EAX against itself.

If EAX is zero:

`ZF = 1`

If EAX is nonzero:

`ZF = 0`

This is a common way to test whether a register contains zero.

Example:

```asm
test eax, eax
je zero_case
```

The jump is taken when EAX is zero because JE checks whether ZF is set.

TEST clears CF and OF, while SF and ZF reflect the logical result. PF also reflects the result according to the instruction's rules.

### CMP vs. TEST

| CMP | TEST |
|---|---|
| Performs subtraction for flags | Performs AND for flags |
| Commonly compares values | Commonly checks bits or zero/nonzero state |
| Does not store subtraction result | Does not store AND result |

**Reverse engineering connection:** Both instructions can precede conditional branches, but their purposes and flag behavior differ.

---

# 16. Conditional and Unconditional Jumps

Jump instructions modify control flow.

## 16.1 JMP

```asm
jmp target
```

Transfers execution to the specified target unconditionally.

## 16.2 JE / JZ

```asm
cmp eax, 42
je equal_case
```

JE and JZ jump when ZF is set.

They are synonymous condition codes for this purpose.

## 16.3 JNE / JNZ

```asm
cmp eax, 42
jne not_equal
```

JNE and JNZ jump when ZF is clear.

## 16.4 Other Conditional Jumps

| Instruction | Condition |
|---|---|
| JE / JZ | ZF = 1 |
| JNE / JNZ | ZF = 0 |
| JG | Signed greater than |
| JGE | Signed greater than or equal |
| JL | Signed less than |
| JLE | Signed less than or equal |
| JA | Unsigned above |
| JAE | Unsigned above or equal |
| JB | Unsigned below |
| JBE | Unsigned below or equal |

The conditions are determined by the relevant status flags.

### Control-Flow Example

C++ source:

```cpp
if (sum == 42) {
    return 1;
} else {
    return 0;
}
```

Possible assembly:

```asm
cmp eax, 42
jne else_block

mov eax, 1
jmp end_function

else_block:
mov eax, 0

end_function:
ret
```

This is an illustrative sequence. A real compiler may use different registers, memory operands, instruction ordering, or branch structures.

### Control-Flow Analysis

To analyze a branch:

1. Identify the instruction that sets the flags.
2. Determine the condition tested by the jump.
3. Inspect the actual flag state.
4. Identify the jump target.
5. Trace the path taken by the program.

Do not assume that a jump is taken merely because the instruction appears in the disassembly.

---

# 17. Signed vs. Unsigned Comparisons

The same bit pattern can represent either a signed or unsigned integer.

For example, a 32-bit value with all bits set can be interpreted as:

- Unsigned: 4,294,967,295
- Signed two's-complement: -1

The comparison instruction does not independently know the programmer's intended data type. Conditional jumps interpret the relevant flags according to signed or unsigned comparison rules.

## Signed Conditions

- JG: greater than
- JGE: greater than or equal
- JL: less than
- JLE: less than or equal

## Unsigned Conditions

- JA: above
- JAE: above or equal
- JB: below
- JBE: below or equal

### Example

Suppose a comparison tests a 32-bit value against zero.

For a signed negative number, the signed comparison can indicate that it is less than zero.

For an unsigned integer, the same 32-bit pattern may represent a large positive number.

**Reverse engineering principle:** Identify the data interpretation and branch condition before concluding what a comparison means.

---

# 18. Bitwise Operations

Bitwise operations manipulate individual bits.

They are widely used in flags, masks, permissions, low-level arithmetic, and binary data processing.

## 18.1 AND

The output bit is 1 only when both input bits are 1.

Example:

```text
  0110  (6)
& 0011  (3)
------
  0010  (2)
```

## 18.2 OR

The output bit is 1 when at least one input bit is 1.

```text
  0110  (6)
| 0011  (3)
------
  0111  (7)
```

## 18.3 XOR

The output bit is 1 when the input bits differ.

```text
  0110  (6)
^ 0011  (3)
------
  0101  (5)
```

## 18.4 SHL — Shift Left

Example:

```asm
shl eax, 1
```

Shifts the bits to the left and inserts zero bits on the right.

For an unsigned value, a left shift by one corresponds to multiplication by two when the result fits the destination width.

## 18.5 SHR — Logical Shift Right

Example:

```asm
shr eax, 1
```

Shifts bits to the right and inserts zeros on the left.

For unsigned integers, a right shift by one is equivalent to integer division by two, discarding the remainder.

### Important Shift Considerations

- The destination width matters.
- Bits shifted out are discarded.
- Shift operations affect flags according to their instruction semantics.
- Signed arithmetic and overflow must be considered when relating shifts to multiplication or division.
- In C++, invalid shift counts and signed overflow require particular care.

### Practical Example

For the value 42:

| Operation | Result |
|---|---:|
| `42 & 15` | 10 |
| `42 \| 3` | 43 |
| `42 ^ 7` | 45 |
| `42 << 1` | 84 |
| `42 >> 1` | 21 |

These operations connect high-level bitwise expressions to low-level register manipulation.

---

# 19. Function Arguments and Calling Conventions

A calling convention defines how functions receive arguments, return results, and preserve required register state.

It is part of the application binary interface (ABI).

Different operating systems and toolchains can use different calling conventions.

## 19.1 Windows x64 Calling Convention

Under the standard Windows x64 ABI, the first four integer or pointer arguments are passed in:

1. RCX
2. RDX
3. R8
4. R9

Additional arguments are passed through the stack according to the ABI.

The caller also reserves the required shadow space for the called function.

## 19.2 System V AMD64 ABI

On common Linux x86-64 systems, the first six integer or pointer arguments are passed in:

1. RDI
2. RSI
3. RDX
4. RCX
5. R8
6. R9

Additional arguments are generally passed on the stack according to the ABI.

## 19.3 Why This Matters

For a function:

```cpp
int calculateSum(int a, int b);
```

The locations of `a` and `b` depend on the calling convention and how the function was compiled.

For a typical Windows x64 call, the first two integer arguments arrive through ECX and EDX.

For a typical System V AMD64 call, they arrive through EDI and ESI.

A compiler may then copy the incoming arguments into stack locations or other registers.

**Reverse engineering principle:** Determine the target platform and ABI before interpreting function arguments.

---

# 20. Function Return Values

A function's return value is communicated according to the calling convention.

For ordinary integer return values on the standard Windows x64 and System V AMD64 ABIs, EAX/RAX is commonly used.

Example:

```cpp
int calculateSum(int a, int b) {
    return a + b;
}
```

Possible assembly:

```asm
mov eax, 17
add eax, 25
ret
```

At the point of return:

`EAX = 42`

The caller receives the integer result according to the ABI.

### Important distinction

A function can calculate a result in a register, store it in memory, or use intermediate locations before returning.

The source-level return statement does not require a single fixed instruction sequence.

---

# 21. Executables, Debugging, and Reverse Engineering

Reverse engineering studies how software behaves and how its implementation can be understood from available artifacts.

Two major approaches are static analysis and dynamic analysis.

## 21.1 Static Analysis

Static analysis examines a binary without executing it.

Common activities include:

- Inspecting executable sections
- Reading disassembly
- Identifying function boundaries
- Examining strings and references
- Analyzing control-flow structures
- Investigating imported functions

Tools such as Ghidra can help with disassembly, decompilation, and program structure analysis.

## 21.2 Dynamic Analysis

Dynamic analysis examines a program while it runs.

Common activities include:

- Setting breakpoints
- Stepping through instructions
- Inspecting registers
- Examining stack memory
- Following function calls
- Observing conditional branches
- Inspecting memory at runtime

x64dbg is a Windows debugger commonly used for dynamic analysis of supported Windows executables.

## 21.3 The Debugging Workflow

A disciplined workflow is:

1. Build a small test program.
2. Identify the function of interest.
3. Set a breakpoint.
4. Run until the breakpoint is reached.
5. Inspect the current instruction and registers.
6. Step through instructions.
7. Inspect memory and flags when relevant.
8. Record observations.
9. Compare observations against the expected behavior.
10. Repeat with changed inputs.

This process develops the ability to move from source-level reasoning to instruction-level evidence.

---

# 22. Connecting Source Code to Assembly

Consider the following function:

```cpp
int checkNumber(int a, int b) {
    int sum = a + b;

    if (sum == 42) {
        return 1;
    }

    return 0;
}
```

This function combines several concepts.

### Stage 1: Function Arguments

The function receives two integers.

Their initial locations depend on the target ABI.

### Stage 2: Arithmetic

The program calculates:

`sum = a + b`

A compiler may use ADD or an equivalent instruction sequence.

### Stage 3: Comparison

The sum is compared against 42.

A CMP instruction is a common implementation.

### Stage 4: Flag Update

If the comparison operands are equal, ZF is set.

### Stage 5: Conditional Branch

A conditional jump determines whether execution follows the matching or non-matching path.

### Stage 6: Return Value

The function returns either 1 or 0.

For a normal integer return under common x86-64 ABIs, the value is returned through EAX/RAX.

### Stage 7: Return to Caller

RET transfers control to the saved return address.

This function connects arguments, arithmetic, registers, flags, control flow, and return behavior into one execution path.

---

# 23. Complete Execution Walkthrough

Consider the following values:

```cpp
int a = 17;
int b = 25;
int sum = a + b;
```

## 23.1 Arithmetic

`17 + 25 = 42`

## 23.2 Hexadecimal Representation

`42 decimal = 0x2A`

## 23.3 Comparison

The program compares `sum` with 42.

If both values are equal, the comparison sets ZF.

## 23.4 Conditional Branch

A following JE or JNE instruction uses the flag state to determine the execution path.

## 23.5 Return Value

The matching path returns 1.

The non-matching path returns 0.

## 23.6 Memory Representation

On x86-64, a 32-bit integer with value 42 is represented in little-endian memory as:

`2A 00 00 00`

## 23.7 Function Return

RET transfers execution back to the caller using the saved return address.

### Unified Conceptual Flow

Arguments
→ Registers or other ABI-defined locations
→ Arithmetic
→ Intermediate result
→ Comparison
→ Flags
→ Conditional branch
→ Return value
→ RET
→ Caller resumes execution

This is a useful conceptual model, but actual compiler output may use different registers, instructions, and memory layouts.

---

# 24. Common Misconceptions

### Misconception 1: Every C++ statement becomes one assembly instruction.

**Correction:** Compilers can expand, combine, optimize, or eliminate operations.

### Misconception 2: Every function has a standard RBP-based stack frame.

**Correction:** Optimized functions may omit a frame pointer or avoid a conventional stack frame.

### Misconception 3: Local variables always exist in stack memory.

**Correction:** Local variables may reside in registers, stack memory, or optimized representations.

### Misconception 4: CMP stores the subtraction result.

**Correction:** CMP updates flags without storing the subtraction result in its destination operand.

### Misconception 5: TEST stores the AND result.

**Correction:** TEST updates flags without storing the logical result.

### Misconception 6: JNE always jumps after CMP.

**Correction:** JNE jumps only when ZF is clear.

### Misconception 7: Little-endian reverses the hexadecimal digits of a number.

**Correction:** Little-endian changes the order of bytes in memory, not the order of hexadecimal digits within each byte.

### Misconception 8: RBP always points to the stack frame.

**Correction:** RBP is a general-purpose register and is only used as a frame pointer when the compiled code chooses to do so.

### Misconception 9: RET terminates the entire program.

**Correction:** RET normally returns to the caller. Program termination is a separate operation.

### Misconception 10: The same calling convention is used by all x86-64 operating systems.

**Correction:** Windows x64 and System V AMD64 use different conventions for many argument-passing details.

### Misconception 11: Register values and memory bytes must look identical.

**Correction:** Registers display values, while a memory dump displays bytes at increasing addresses.

### Misconception 12: Disassembly always reveals the original C++ source exactly.

**Correction:** Compilation removes or transforms source-level information. Disassembly reveals machine instructions, while decompilation provides an approximation of higher-level logic.

---

# 25. Final Revision Summary

The concepts from Days 01–07 connect through the execution of a compiled program.

- The compiler transforms C++ into machine code.
- The operating system loads the executable and establishes a process.
- The CPU executes instructions using registers and memory.
- RIP identifies the current instruction location.
- RSP tracks the stack's current top.
- RBP may provide a reference point for a conventional stack frame.
- CALL saves a return address and transfers control.
- PUSH and POP manipulate stack state.
- Function prologues and epilogues commonly establish and restore stack-related state.
- Decimal, binary, and hexadecimal representations describe the same numerical values in different bases.
- Memory operands use effective addresses to access data.
- Little-endian ordering determines how multibyte values appear in memory.
- Arithmetic instructions calculate values and update flags as specified.
- CMP compares operands through flag updates.
- TEST performs a logical AND for flag-setting purposes.
- Conditional jumps use flags to control execution.
- Signed and unsigned comparisons use different condition rules.
- Bitwise operations manipulate individual bits.
- Calling conventions define argument passing, return values, and register-preservation rules.
- RET transfers execution back to the caller.
- Static analysis examines binaries, while dynamic analysis observes runtime behavior.

## Core Reverse Engineering Principle

**Do not memorize assembly instructions in isolation. Understand the relationship between an instruction, the CPU state before execution, the state after execution, and the resulting control flow.**

That relationship is the foundation for analyzing compiled programs, understanding disassembly, investigating runtime behavior, and progressing toward advanced reverse engineering and low-level systems engineering.

---

**Day 08 Status:** Complete Theory Revision — Days 01–07

**Next Step:** Apply these concepts in a practical C++ program, inspect the generated x86-64 assembly in a debugger, document the actual observations, and publish the lab to GitHub.
