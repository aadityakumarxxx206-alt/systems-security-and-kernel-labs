# Reverse Engineering — Day 03

## x64 Architecture & Assembly Basics

---

## 1. Key Registers Overview

### `RSP` — Stack Pointer

`RSP` points to the current position of the stack.

It is used to keep track of the current stack location during program execution.

---

### `RBP` — Base / Frame Pointer

`RBP` is used as a reference point for locations inside a function's stack frame.

It allows stack-based locations to be referenced using offsets such as:

```asm
[RBP-4]
[RBP-8]
[RBP-0x10]
```

---

### `RIP` — Instruction Pointer

`RIP` tracks the current or next instruction location being executed by the CPU.

During debugging, observing `RIP` helps track the execution flow of the program.

---

# 2. Stack Frame & Memory Addressing

## `[RBP - offset]`

`[RBP - offset]` represents a memory location at a specific offset inside the stack frame.

It describes the **location being accessed**, not the size of the data.

For example:

```asm
[RBP-4]
[RBP-8]
```

These represent different locations relative to `RBP`.

---

## Data Size Qualifiers

The instruction specifies how much data is being accessed through the data-size qualifier.

| Qualifier | Size |
|---|---:|
| `BYTE` | 1 Byte (8 bits) |
| `WORD` | 2 Bytes (16 bits) |
| `DWORD` | 4 Bytes (32 bits) |
| `QWORD` | 8 Bytes (64 bits) |

---

# 3. Function Prologue & Assembly Instructions

A function typically begins by preparing its stack frame.

## `push rbp`

```asm
push rbp
```

Saves the previous `RBP` value onto the stack.

This preserves the previous stack-frame reference before the new function establishes its own frame.

---

## `mov rbp, rsp`

```asm
mov rbp, rsp
```

Sets `RBP` to the current `RSP` position.

This establishes the reference point for the function's stack frame.

---

## `sub rsp, XX`

```asm
sub rsp, XX
```

Reserves stack space for local or temporary variables by decreasing the value of `RSP`.

For example:

```asm
sub rsp, 0x30
```

reserves:

```text
0x30 = 48 bytes
```

---

## `CALL`

```asm
CALL
```

Calls another function.

When `CALL` is executed, the return address is saved on the stack so that execution can return to the caller after the called function completes.

---

## `RET`

```asm
RET
```

Returns execution to the caller using the saved return address.

The return address that was saved during the function call is used to continue execution from the appropriate location.

---

# 4. Debugger Navigation

## `F8` — Step Over

```text
F8 → Step Over
```

`F8` executes the current instruction and then moves the debugger to the next execution point.

During practical debugging, this allows the instructions to be observed step-by-step.

It is useful for following how:

```text
Instruction
     ↓
Register / Memory Change
     ↓
Next Instruction
```

changes during program execution.

---

# 🧠 Core Mental Model

The concepts covered in this section can be connected as:

```text
RSP
 ↓
Current Stack Position

RBP
 ↓
Stack Frame Reference
 ↓
[RBP - offset]
 ↓
Memory Location

RIP
 ↓
Instruction Location
 ↓
Execution Flow
```

---

# 🔄 Function Stack-Frame Flow

```text
push rbp
    ↓
Previous RBP saved
    ↓
mov rbp, rsp
    ↓
New Stack Frame Reference
    ↓
sub rsp, XX
    ↓
Stack Space Reserved
    ↓
[RBP - offset]
    ↓
Local / Temporary Data
```

---

## 📌 Day 03 Key Takeaway

The main concepts covered were:

- `RSP` → Stack Pointer
- `RBP` → Base / Frame Pointer
- `RIP` → Instruction Pointer
- `[RBP - offset]` → Stack-frame memory location
- `BYTE / WORD / DWORD / QWORD` → Data size qualifiers
- `push rbp` → Save previous `RBP`
- `mov rbp, rsp` → Establish stack-frame reference
- `sub rsp, XX` → Reserve stack space
- `CALL` → Call a function and save the return address
- `RET` → Return to the caller
- `F8` → Step Over during debugging
```