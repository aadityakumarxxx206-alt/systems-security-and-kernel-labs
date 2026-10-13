# Reverse Engineering & Low-Level Engineering — Day 10

## Assembly Language Basics: Stack, Functions & Flags

Today, I studied important x86-64 assembly concepts and explored how the CPU uses the stack, handles function calls, and makes decisions using status flags.

The goal was to understand how these operations work at the assembly and memory levels.

## 1. Stack and Registers

The stack is a memory area used to manage function calls, return addresses, saved registers, and local variables.

### RSP — Stack Pointer

- `RSP` holds the address of the current top of the stack.
- In x86-64, the stack typically grows toward lower memory addresses.
- `PUSH` decreases `RSP` and stores data on the stack.
- `POP` retrieves data from the top of the stack and increases `RSP`.
- The stack follows the LIFO principle: Last In, First Out.

### Example

```asm
push rax
pop rax
```

**What happens?**

1. `PUSH RAX` saves the value of `RAX` on the stack.
2. `POP RAX` retrieves the saved value.
3. The stack pointer changes as the stack is used.

On x86-64, these register operations normally move the stack pointer by 8 bytes per `PUSH` or `POP`.

## 2. Function Calls: CALL and RET

Functions allow programs to execute reusable blocks of code.

### CALL Instruction

- Saves the return address on the stack.
- Transfers execution to the target function.

### RET Instruction

- Retrieves the return address from the stack.
- Transfers execution back to the caller.

### Example

```asm
call my_function

; Execution continues here after RET

my_function:
    mov eax, 10
    ret
```

**Key concept:** `CALL` remembers where execution should continue, and `RET` returns execution to that location.

## 3. Zero Flag (ZF) and Conditional Jumps

The CPU maintains status flags to describe the results of certain operations.

### Zero Flag (ZF)

- `ZF = 1`: The result of the relevant operation is zero.
- `ZF = 0`: The result of the relevant operation is not zero.

### JNE — Jump if Not Equal

`JNE` jumps when `ZF = 0`. It does not jump when `ZF = 1`.

### Practical Example

```asm
cmp dword ptr [rbp-4], 7
jne not_equal
```

Assume `[RBP-4]` contains the value `7`.

1. `CMP` compares the stored value with `7`.
2. The comparison behaves like a subtraction for flag-setting purposes.
3. Because both values are equal, `ZF = 1`.
4. Therefore, `JNE` does not jump.

If the stored value were `5`, the comparison would set `ZF = 0`, and `JNE` would jump to `not_equal`.

**Important:** `CMP` updates flags but does not store the subtraction result in the destination operand.

## 4. Function Prologue and Stack Frame

A function may create a stack frame to manage its local variables and saved state.

### Example: Function Prologue

```asm
push rbp
mov rbp, rsp
sub rsp, 30h
```

### Explanation

- `push rbp`: Saves the previous value of `RBP`.
- `mov rbp, rsp`: Establishes the current stack-frame base.
- `sub rsp, 30h`: Reserves 48 bytes of stack space.

Here, `30h` is hexadecimal for 48 in decimal.

This is a common stack-frame pattern, although compilers may use different instructions depending on optimization and calling conventions.

### Storing a Local Variable

```asm
mov dword ptr [rbp-4], 7
```

This instruction stores the 32-bit value `7` at the memory address four bytes below `RBP`.

The notation `[RBP-4]` refers to a memory location, not a register.

## 5. Memory Dump and Little-Endian Format

A debugger can display memory contents in hexadecimal.

For example, the 32-bit integer `7` appears as:

```text
07 00 00 00
```

### Why?

x86-64 systems use little-endian byte ordering for ordinary integer values.

The least significant byte is stored at the lowest memory address.

For the value `7`:

- Lowest address: `07`
- Next byte: `00`
- Next byte: `00`
- Highest address: `00`

Together, these four bytes represent the 32-bit integer `7`.

This is useful when inspecting local variables in a debugger's Dump or Memory window.

## 6. Practical Debugging Checklist

During assembly debugging, check the following:

- [ ] Observe how `RSP` changes after `PUSH` and `POP`.
- [ ] Follow the return address created by `CALL`.
- [ ] Observe how `RET` transfers execution back to the caller.
- [ ] Inspect `ZF` after executing `CMP`.
- [ ] Determine whether `JNE` is taken or not taken.
- [ ] Inspect a local variable at `[RBP-4]`.
- [ ] Confirm how the integer `7` appears in a memory dump.

## Key Takeaways

Today, I strengthened my understanding of:

- Stack operations and the `RSP` register.
- Function calls and return addresses.
- The `CALL` and `RET` instructions.
- Zero Flag and conditional branching.
- Function prologues and local variables.
- Little-endian memory representation.
- Practical assembly debugging concepts.

These concepts are important foundations for understanding program execution, analyzing compiled code, and learning reverse engineering.

## Day 10 Summary

I learned how the stack supports function execution, how return addresses control program flow, how CPU flags influence conditional jumps, and how local variables appear in memory.

My next step is to continue practicing these concepts in a debugger and connect assembly instructions with actual register and memory changes.

---

**Journey:** Reverse Engineering & Low-Level Engineering  
**Day:** 10  
**Focus:** x86-64 Assembly, Stack Operations, Function Calls, CPU Flags, and Memory Analysis

#ReverseEngineering #AssemblyLanguage #x64dbg #LowLevelProgramming #ComputerArchitecture #LearningInPublic
