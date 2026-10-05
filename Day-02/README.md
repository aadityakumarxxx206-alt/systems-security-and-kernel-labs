# Day 2 — Reverse Engineering & Low-Level Memory Basics

## Key Concepts Learned

### 1. Hexadecimal System & Decimal Conversion

Hexadecimal is a **base-16 number system** commonly used in reverse engineering, assembly, debugging, memory analysis, and low-level programming.

It uses:

```text
0–9, A–F
```

where:

```text
A = 10
B = 11
C = 12
D = 13
E = 14
F = 15
```

Hexadecimal values are commonly written with the `0x` prefix.

#### Decimal → Hexadecimal Conversion

The standard manual method is to repeatedly divide the decimal number by `16` and record the remainders.

**Process:**

1. Divide the decimal number by `16`.
2. Record the remainder.
3. Divide the quotient by `16` again.
4. Repeat until the quotient becomes `0`.
5. Read the remainders **from bottom to top**.

#### Example: Decimal `42` → Hexadecimal

```text
42 ÷ 16 = 2   remainder 10 → A
 2 ÷ 16 = 0   remainder 2
```

Reading the remainders from bottom to top:

```text
2A
```

Therefore:

```text
42₁₀ = 0x2A
```

#### Important Boundary Example

```text
15₁₀  = 0xF
16₁₀  = 0x10

255₁₀ = 0xFF
256₁₀ = 0x100
```

These boundaries are important because hexadecimal is base-16 and bytes are commonly represented using two hexadecimal digits.

---

### 2. Function Stack Frame & Stack Registers

The **stack** is a region of memory used by functions for temporary storage and function-related data, such as local variables, saved registers, and other execution state.

Two important x86-64 stack registers are:

#### RSP — Stack Pointer

`RSP` points to the current top of the stack.

Stack operations such as `push`, `pop`, `call`, and `ret` affect the stack and normally change `RSP`.

#### RBP — Base Pointer

`RBP` can be used as a stable reference point for a function's stack frame.

A traditional x64 stack-frame setup commonly looks like:

```asm
push rbp
mov  rbp, rsp
sub  rsp, 0x30
```

#### Stack Setup

1. **`push rbp`**  
   Saves the previous `RBP` value on the stack.

2. **`mov rbp, rsp`**  
   Makes the current `RSP` value the reference point for the new stack frame.

3. **`sub rsp, 0x30`**  
   Moves `RSP` downward and reserves `0x30` bytes of stack space.

Since:

```text
0x30 = 48 decimal
```

the instruction reserves **48 bytes**.

> **Note:** Modern compilers may omit the traditional `RBP` frame pointer or use `RBP` as a general-purpose register, so this pattern is common for learning and debugging but is not mandatory in every compiled function.

---

### 3. RBP-Relative Addressing & Data Sizes

Once a stack frame is established, memory can be accessed using offsets relative to `RBP`.

For example:

```asm
[rbp - 4]
[rbp - 8]
[rbp - 0x10]
```

These expressions represent **memory addresses calculated from the current value of `RBP`**.

For example:

```asm
mov dword ptr [rbp-4], 0x11
```

means:

```text
Target Address = RBP - 4
Value          = 0x11
Size           = 4 bytes
```

The `-4` is an **address offset**, not the value being stored.

#### Data Sizes

| Operand | Size | Bits |
|---|---:|---:|
| `byte ptr` | 1 byte | 8 bits |
| `word ptr` | 2 bytes | 16 bits |
| `dword ptr` | 4 bytes | 32 bits |
| `qword ptr` | 8 bytes | 64 bits |

For example:

```asm
mov byte ptr  [rbp-1], 0x11
```

writes 1 byte, while:

```asm
mov dword ptr [rbp-4], 0x11
```

writes 4 bytes.

---

### 4. Memory Dump & Little-Endian

x86-64 systems use **Little-Endian** byte ordering.

Little-Endian means that the **Least Significant Byte (LSB)** of a multi-byte value is stored at the lowest memory address.

#### Example

Consider the 32-bit value:

```text
0x00000011
```

As four bytes, this is:

```text
00 00 00 11
```

In Little-Endian memory, the bytes are stored as:

```text
11 00 00 00
```

Therefore, after executing:

```asm
mov dword ptr [rbp-4], 0x11
```

the memory dump should show:

```text
11 00 00 00
```

#### Another Example

For:

```text
0x12345678
```

Little-Endian memory representation is:

```text
78 56 34 12
```

> **Important:** Little-Endian reverses the **order of bytes**, not the individual bits inside each byte.

---

# x64dbg Practice Protocol

The concepts above were verified practically using **x64dbg**.

### 1. Set Breakpoint

Load:

```text
challenge.exe
```

Find the relevant function and set a breakpoint at:

```asm
push rbp
```

Press:

```text
F2
```

Then continue execution with:

```text
F9
```

---

### 2. Inspect Stack Allocation

Step through the instructions using:

```text
F8
```

Observe the changes in the `RBP` and `RSP` registers while executing:

```asm
push rbp
mov  rbp, rsp
sub  rsp, 0x30
```

Pay attention to the fact that:

```text
0x30 = 48 bytes
```

and that the stack grows toward lower memory addresses on x86-64.

---

### 3. Calculate the Memory Address

For:

```asm
mov dword ptr [rbp-4], 0x11
```

calculate:

```text
Target Address = RBP - 4
```

For example, if:

```text
RBP = 0x0000000000123450
```

then:

```text
RBP - 4 = 0x000000000012344C
```

This is the actual memory address where the 4-byte value is written.

---

### 4. Inspect the Memory Dump

Open the relevant Dump window in x64dbg.

Use:

```text
Ctrl + G
```

and navigate to:

```text
RBP - 4
```

After executing:

```asm
mov dword ptr [rbp-4], 0x11
```

verify that the memory contains:

```text
11 00 00 00
```

This confirms the Little-Endian representation of the 32-bit value.

---

### 5. Verify Registers

Step over the instruction and inspect the relevant registers.

Pay particular attention to:

```text
RAX
EAX
RBP
RSP
```

Remember:

```text
EAX = lower 32 bits of RAX
```

The important goal is to connect the assembly instruction with the resulting **register state, memory address, and raw memory bytes**.

---

# Day 2 Summary

Today I built a practical foundation in low-level memory analysis.

I learned how to:

- Convert decimal values to hexadecimal manually.
- Understand hexadecimal as a base-16 number system.
- Understand `RSP` and `RBP` in a traditional x64 stack frame.
- Follow the stack-frame setup using `push rbp`, `mov rbp, rsp`, and `sub rsp, 0x30`.
- Calculate memory addresses using `RBP`-relative offsets.
- Understand `byte`, `word`, `dword`, and `qword` memory sizes.
- Understand Little-Endian byte ordering.
- Verify memory representation directly in x64dbg.

The key connection from this lesson is:

```text
Assembly Instruction
        ↓
Register State
        ↓
Memory Address
        ↓
Raw Memory Bytes
```

This foundation will be used in the next stages of **x64 Assembly, Stack Analysis, CALL/RET, Return Addresses, and deeper Reverse Engineering**.
