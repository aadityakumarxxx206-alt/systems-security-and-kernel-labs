# Reverse Engineering & Low-Level Engineering — Day-09

# Assembly Language & CPU Architecture

## Overview

This document covers signed and unsigned comparisons, two's complement representation, CPU status flags, and essential x86-64 control-flow instructions. These concepts are fundamental to understanding assembly execution, conditional branching, debugging, and reverse engineering.

## 1. Signed and Unsigned Comparisons

The CPU processes bit patterns. Whether a bit pattern represents a signed or unsigned integer depends on how the program interprets it.

### 1.1 Unsigned Comparison

Unsigned integers represent values from zero upward. The following x86-64 conditional jump instructions are commonly used for unsigned comparisons:

- `JA` — Jump if Above (`CF = 0` and `ZF = 0`)
- `JAE` — Jump if Above or Equal (`CF = 0`)
- `JB` — Jump if Below (`CF = 1`)
- `JBE` — Jump if Below or Equal (`CF = 1` or `ZF = 1`)

### 1.2 Signed Comparison

Signed integers represent both negative and non-negative values. The following conditional jumps are used for signed comparisons:

- `JG` — Jump if Greater (`ZF = 0` and `SF = OF`)
- `JGE` — Jump if Greater or Equal (`SF = OF`)
- `JL` — Jump if Less (`SF != OF`)
- `JLE` — Jump if Less or Equal (`ZF = 1` or `SF != OF`)

### Example

```asm
mov eax, -1
mov ebx, 1
cmp eax, ebx
jl signed_less
jb unsigned_below
```

For a 32-bit comparison, `-1` is represented by the bit pattern `0xFFFFFFFF`. Interpreted as a signed integer, it is less than `1`. Interpreted as an unsigned integer, it is greater than `1`.

After `CMP`, signed and unsigned conditional jumps interpret the same status flags differently.

## 2. Two's Complement Representation

Two's complement is the standard representation used for signed integers in modern x86-64 systems.

### 2.1 Standard Method

To calculate the negative representation of a positive integer using a fixed bit width:

1. Invert every bit (`0` becomes `1`, and `1` becomes `0`).
2. Add `1` to the inverted result.

Example: Represent `-5` using 8 bits.

```text
Positive 5: 00000101
Invert:     11111010
Add 1:      11111011

Negative 5: 11111011
```

### 2.2 Shortcut Method

Starting from the rightmost bit, keep all bits unchanged through the first `1`. Invert every bit to its left.

Example:

```text
Positive 5: 00000101
Negative 5: 11111011
```

### 2.3 Signed Integer Ranges

For an n-bit two's complement integer, the range is:

`-2^(n-1)` through `2^(n-1) - 1`

| Bit Width | Minimum | Maximum |
|---|---:|---:|
| 4-bit signed | -8 | 7 |
| 8-bit signed | -128 | 127 |
| 16-bit signed | -32,768 | 32,767 |
| 32-bit signed | -2,147,483,648 | 2,147,483,647 |
| 64-bit signed | -2^63 | 2^63 - 1 |

For an n-bit unsigned integer, the range is `0` through `2^n - 1`.

**Important:** When interpreted as a signed two's complement integer, an MSB (Most Significant Bit) of `1` indicates a negative value, while an MSB of `0` indicates a non-negative value.

## 3. Important CPU Status Flags

The CPU flags register records information about certain arithmetic and logical operations. Conditional jump instructions use these flags to determine whether execution should branch to another location.

| Flag | Full Name | Description |
|---|---|---|
| `ZF` | Zero Flag | Set when the operation's result is zero. |
| `SF` | Sign Flag | Reflects the most significant bit of the result at the operation's operand width. |
| `CF` | Carry Flag | Records unsigned carry or borrow information for relevant arithmetic operations. |
| `OF` | Overflow Flag | Indicates signed arithmetic overflow when the mathematical result cannot be represented in the destination width. |

### 3.1 Example: Zero Flag

```asm
mov eax, 5
cmp eax, 5
je equal_label
```

`CMP` calculates flags as if it performed `5 - 5`. The result is zero, so `ZF = 1`, and `JE` takes the branch.

### 3.2 Carry Flag vs. Overflow Flag

- `CF` is primarily used to reason about unsigned arithmetic.
- `OF` is used to reason about signed arithmetic.

For an 8-bit example:

```text
Unsigned: 255 + 1 produces 0, with carry.
Signed:   127 + 1 produces the bit pattern 10000000, with overflow.
```

The stored 8-bit result is `0x00` in the unsigned example and `0x80` in the signed example. The underlying bit-level addition is the same, but the interpretation and relevant flags differ.

## 4. Essential Assembly Instructions

### 4.1 CMP — Compare Operands

`CMP A, B` updates the status flags as if it calculated `A - B`. It does not store the subtraction result in either operand.

Example:

```asm
mov eax, 10
mov ebx, 20
cmp eax, ebx
jl less_label
```

Since signed `10` is less than signed `20`, `JL` takes the branch.

### 4.2 JMP — Unconditional Jump

`JMP` transfers execution directly to a specified destination. It does not automatically save a return address.

```asm
jmp target
```

### 4.3 Conditional Jump Instructions

Conditional jumps use the current status flags to decide whether to branch.

- `JE` / `JZ` — Jump if Equal / Zero
- `JNE` / `JNZ` — Jump if Not Equal / Not Zero
- `JG`, `JGE`, `JL`, `JLE` — Signed comparisons
- `JA`, `JAE`, `JB`, `JBE` — Unsigned comparisons

`JE` and `JNE` are commonly used after `CMP` to check equality or inequality.

### 4.4 CALL — Function Call

`CALL` transfers execution to a function or procedure and saves the return address on the stack. A normal matching `RET` can use that address to resume execution at the instruction following the call.

```asm
call my_function
```

A `CALL` instruction does not automatically create a complete stack frame or save every register. Those responsibilities depend on the function's implementation and calling convention.

### 4.5 RET — Return from Function

`RET` retrieves the return address from the stack and transfers execution back to that address. In normal near-call usage, the return address is removed from the stack as part of the return operation.

```asm
ret
```

### 4.6 CALL vs. JMP vs. RET

| Instruction | Purpose | Stack Behavior |
|---|---|---|
| `CALL` | Calls a function or procedure | Pushes a return address onto the stack. |
| `JMP` | Transfers execution to another location | Does not automatically push a return address. |
| `RET` | Returns to the saved return address | Pops the return address from the stack. |

## 5. Practical Debugging with x64dbg

Use the following workflow to understand comparisons and conditional branching:

1. Set a breakpoint before the `CMP` instruction.
2. Inspect the registers containing the operands.
3. Execute the `CMP` instruction.
4. Inspect `ZF`, `SF`, `CF`, and `OF` in the flags panel.
5. Identify the next conditional jump instruction.
6. Determine whether the jump uses signed or unsigned conditions.
7. Step through the instruction and confirm the actual execution path.
8. For `CALL` and `RET`, inspect the instruction pointer and stack to understand function control flow.

## 6. Key Takeaways

- The same bit pattern can represent different values under signed and unsigned interpretation.
- Two's complement is a standard method of representing signed integers.
- `CMP` updates flags without storing a subtraction result.
- Signed conditional jumps use relationships involving `SF`, `OF`, and `ZF`.
- Unsigned conditional jumps primarily use `CF` and `ZF`.
- `CALL` saves a return address, `JMP` does not, and `RET` uses the return address on the stack.
- Understanding CPU flags, registers, stack memory, and control flow is essential for reverse engineering.

## Day 09 Summary

**Track:** Reverse Engineering & Low-Level Engineering

**Day:** 09

**Topics Covered:**
- Signed and unsigned comparisons
- Two's complement representation
- Signed and unsigned integer ranges
- CPU status flags: `ZF`, `SF`, `CF`, and `OF`
- `CMP` and conditional jump instructions
- `JMP`, `CALL`, and `RET`
- Practical assembly debugging with x64dbg

**Learning Objective:** Understand how the CPU represents signed integers, updates status flags, evaluates conditional branches, and manages function control flow.