# Day 6 — x64 Assembly & Reverse Engineering Notes

# x64 Assembly & Reverse Engineering Notes

## 1. Memory, Structures, and Pointers

### 1.1 Structure and Object Fields
A structure or object may contain multiple fields stored at different memory offsets. The compiler determines their layout, including padding and alignment.

### 1.2 Base + Offset Addressing
An instruction may access data using a base address plus an offset.

Example:

```asm
mov eax, [rcx+10h]
```

This instruction reads a 4-byte value from the memory address calculated as `RCX + 10h`.

Repeated accesses using different offsets may indicate fields of a structure or object. However, this must be verified by examining the surrounding instructions and program context.

### 1.3 Registers: EAX and RAX
- **EAX:** A 32-bit register (4 bytes).
- **RAX:** A 64-bit register (8 bytes).

In 64-bit mode, writing to `EAX` automatically clears the upper 32 bits of `RAX`.

Example:

```asm
mov rax, 0xFFFFFFFFFFFFFFFF
mov eax, 5
```

After the second instruction:

```text
EAX = 0x00000005
RAX = 0x0000000000000005
```

### 1.4 Pointers
A pointer is a value used to represent a memory address.

Example:

```asm
mov rax, [rbx]
```

This instruction reads a 64-bit value from the memory address stored in `RBX` and places it in `RAX`.

Not every 8-byte value is a pointer. Its meaning must be verified using the surrounding code and memory context.

### 1.5 Pointer Chains
A pointer chain is a sequence in which one memory address contains another address that can be followed to reach additional data.

Example:

```asm
mov rax, [rcx]
mov rax, [rax+10h]
```

The first instruction reads a value from the address stored in `RCX`. The second instruction uses the resulting value as a base address and reads another value at offset `10h`.

Whether these values are pointers depends on the program's actual memory layout.

---

## 2. Windows x64 Calling Convention

The standard Windows x64 calling convention uses registers for the first four argument positions.

### 2.1 Integer and Pointer Arguments

| Argument Position | Register |
|---|---|
| 1st argument | RCX |
| 2nd argument | RDX |
| 3rd argument | R8 |
| 4th argument | R9 |

Additional arguments are generally passed on the stack.

For floating-point arguments in the first four positions, the corresponding registers may be `XMM0`, `XMM1`, `XMM2`, and `XMM3`, depending on the argument positions and types.

The caller also reserves 32 bytes of shadow space for the callee.

### 2.2 Return Values

Common return-value locations include:

- **RAX:** Commonly used for 64-bit integer and pointer return values.
- **EAX:** Commonly used for 32-bit integer return values.
- **XMM0:** Commonly used for floating-point return values.

The exact return mechanism depends on the return type and calling convention.

### 2.3 Reverse-Engineering Tip

When analyzing a function call, inspect the argument registers immediately before the call and the relevant return-value register after the call.

These registers provide useful clues, but their contents must be interpreted in the context of the instructions and the function's behavior.

---

## 3. Key Instructions and Data Sizes

### 3.1 MOV — Move Data

`MOV` copies data from a source operand to a destination operand.

Example:

```asm
mov rax, [rbx+20h]
```

This instruction reads an 8-byte value from the memory address `RBX + 20h` and stores it in `RAX`.

The operand size determines how many bytes are read or written.

### 3.2 LEA — Load Effective Address

`LEA` calculates an effective address and stores the calculated result in the destination register. It does not read the memory contents at that address.

Example:

```asm
lea rax, [rbx+20h]
```

If `RBX = 1000h`, then:

```text
RAX = 1020h
```

Compare the instructions:

```asm
mov rax, [rbx+20h]
lea rax, [rbx+20h]
```

- `MOV` reads the value stored at the calculated memory address.
- `LEA` calculates the address and stores the result.

`LEA` can also perform efficient arithmetic using base registers, index registers, and scale factors.

### 3.3 MOVZX — Move with Zero Extension

`MOVZX` copies a smaller unsigned value into a larger destination and fills the upper bits with zeros.

Example:

```asm
movzx eax, byte ptr [rcx+10h]
```

This instruction reads one byte and zero-extends it to 32 bits.

If the source byte is `FFh`:

```text
EAX = 000000FFh
```

Because the destination is `EAX`, the upper 32 bits of `RAX` are also cleared.

### 3.4 MOVSX — Move with Sign Extension

`MOVSX` copies a smaller signed value into a larger destination while preserving its sign.

Example:

```asm
movsx eax, byte ptr [rcx+10h]
```

If the source byte is `FFh`, interpreted as signed 8-bit data:

```text
EAX = FFFFFFFFh
RAX = 00000000FFFFFFFFh
```

The value in `EAX` represents `-1` as a signed 32-bit integer.

### 3.5 Common Data Sizes

| Data Type | Size in Bits | Size in Bytes |
|---|---:|---:|
| Byte | 8 | 1 |
| Word | 16 | 2 |
| Dword (Doubleword) | 32 | 4 |
| Qword (Quadword) | 64 | 8 |

These names describe operand sizes commonly encountered in x86/x64 assembly.

---

## 4. Stack Operations and Stack Frames

### 4.1 RSP — Stack Pointer

`RSP` points to the current top of the stack.

In the usual x64 stack convention, the stack grows toward lower memory addresses.

- `PUSH` decreases `RSP`.
- `POP` increases `RSP`.

For ordinary 64-bit general-purpose register operations, `PUSH` and `POP` typically adjust `RSP` by 8 bytes.

### 4.2 PUSH — Store a Value on the Stack

Example:

```asm
push rax
```

This instruction decreases `RSP` by 8 bytes and stores the value of `RAX` at the new stack-top address.

### 4.3 POP — Retrieve a Value from the Stack

Example:

```asm
pop rax
```

This instruction retrieves the value at the top of the stack into `RAX` and increases `RSP` by 8 bytes.

### 4.4 RBP — Frame Pointer

`RBP` is often used as a stable reference to a function's stack frame.

A typical function prologue may look like this:

```asm
push rbp
mov rbp, rsp
sub rsp, 20h
```

Explanation:

1. `push rbp` saves the previous value of `RBP`.
2. `mov rbp, rsp` establishes a frame reference.
3. `sub rsp, 20h` reserves 32 bytes of stack space.

This is a common pattern, not a mandatory layout for every function. Optimized code may omit the frame pointer or use a different stack layout.

### 4.5 Negative and Positive Offsets

Examples:

```asm
mov eax, [rbp-4]
mov ecx, [rbp-8]
```

In a conventional frame, these accesses may refer to local variables or other stack data.

A positive offset, such as:

```asm
mov rax, [rbp+10h]
```

may refer to a stack-passed argument or another item in the stack frame, depending on the function's prologue and layout.

**Important:** Never assume that an offset always represents a particular variable or argument. Verify it by examining the instructions and runtime memory.

---

## 5. CALL and RET Instructions

### 5.1 CALL — Call a Function

Example:

```asm
call function_name
```

`CALL` saves the return address on the stack and transfers execution to the target function.

The return address identifies where execution should continue after the function returns.

### 5.2 RET — Return from a Function

Example:

```asm
ret
```

`RET` retrieves the return address from the stack and transfers execution back to that address.

A normal `RET` does not retrieve the function's return value. The return value is handled separately, commonly through `RAX` or an appropriate subregister.

### 5.3 Simple Execution Flow

```text
Caller Function
      |
      v
     CALL
      |
      v
 Target Function
      |
      v
     RET
      |
      v
Caller Continues
```

**Reverse-engineering tip:** While debugging, observe the stack before and after `CALL` and `RET` to understand how control flow changes.

---

## 6. Practical Reverse-Engineering Rules

1. Identify the instruction and operand size before interpreting a memory access.
2. Distinguish between reading memory with `MOV` and calculating an address with `LEA`.
3. Treat register contents as evidence of possible arguments or return values, not as guaranteed meanings.
4. Verify suspected pointers and pointer chains using runtime memory and surrounding instructions.
5. Analyze stack offsets in the context of the function's actual stack frame.
6. Remember that compiler optimizations can change register usage, stack layout, and instruction sequences.
7. Use a debugger such as x64dbg to inspect registers, memory, the stack, and instruction execution.
8. Confirm conclusions using multiple pieces of evidence rather than relying on a single instruction.

---

## 7. Quick Revision

| Concept | Key Point |
|---|---|
| RAX | 64-bit general-purpose register |
| EAX | 32-bit register; writes clear the upper 32 bits of RAX |
| Pointer | A value representing a memory address |
| `[RAX]` | Access memory at the address stored in RAX |
| `MOV` | Copies data between operands |
| `LEA` | Calculates an effective address |
| `MOVZX` | Extends a value with zeros |
| `MOVSX` | Extends a signed value while preserving its sign |
| RSP | Tracks the top of the stack |
| RBP | Often used as a stack-frame reference |
| PUSH | Stores a value on the stack |
| POP | Retrieves a value from the stack |
| CALL | Saves a return address and transfers control |
| RET | Returns to the saved return address |
| RCX, RDX, R8, R9 | First four integer/pointer argument registers in Windows x64 |

---

**Final Principle:** Reverse engineering is not just about reading assembly instructions. It involves understanding memory, registers, calling conventions, stack behavior, control flow, and runtime evidence together.
