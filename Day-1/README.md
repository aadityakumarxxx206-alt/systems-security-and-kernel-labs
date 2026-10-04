# Day 1 — Low-Level Computing, Memory & Reverse Engineering Fundamentals

## 1. The Execution Chain

- **C++ Execution Flow:**  
  `C++ Source Code` → `Compiler` → `Machine Code` → `Executable` → `OS (Process)` → `CPU Instructions`

- **Compilation:**  
  The process of translating human-readable source code into lower-level machine code that can be executed by a CPU.

- **Program (Static):**  
  An executable file stored on a storage device, such as a `.exe` file.

- **Process (Dynamic):**  
  A running instance of a program that has been loaded into memory and is managed by the Operating System.

- **Reverse Engineering:**  
  The practice of analyzing compiled software or binary code to understand its original logic, structure, and behavior.

---

## 2. CPU Architecture & Memory Storage

- **CPU Registers:**  
  Small, extremely fast storage locations inside the CPU that temporarily hold data, addresses, and intermediate results during execution.

- **RIP (Instruction Pointer):**  
  A 64-bit register used by the CPU to track the address associated with the current instruction execution and the normal sequential flow of instructions.

- **RFLAGS Register:**  
  A status register that contains CPU flags describing the results or state of certain operations.

- **Variable Storage Lifecycle:**  
  High-level variables exist as programming concepts. At runtime, depending on the generated code, their values may be stored in CPU registers, stack memory, or heap memory.

---

## 3. Core Objectives in Binary Reverse Engineering

1. **Function Identification:**  
   Locating functions and their boundaries to understand the structure of a compiled program.

2. **Assembly Parsing:**  
   Reading and understanding disassembled binary instructions represented as assembly language.

3. **Control Flow Analysis:**  
   Understanding how program execution moves through different instructions and code paths.

4. **Data Isolation:**  
   Identifying constants, variables, and memory locations, including stack offsets such as `[RBP-0x4]`.

5. **Behavior Reconstruction:**  
   Using low-level instructions and data relationships to understand the behavior of the original program.

---

## 4. Memory Locations & Stack Offsets

- **Stack Allocation:**  
  Local function variables may be stored in stack memory and can be accessed using offsets relative to a base register such as `RBP` when an RBP-based stack frame is used.

- **Offset Distinction:**  
  `[RBP-0x4]` represents a memory operand based on an offset from `RBP`. It does **not** mean that the stored value is `-4`.

For example:

```text
RBP = 00000091F7FFFDB0

RBP - 0x4
= 00000091F7FFFDAC
```

Therefore:

```text
[RBP-0x4]
```

refers to the memory location calculated from `RBP - 0x4`.

The actual value stored at that location is a separate concept.

---

## 5. Data Units & Hexadecimal Mathematics

### Data Sizes Breakdown

- **Byte:** 1 Byte = 8 Bits
- **WORD:** 2 Bytes = 16 Bits
- **DWORD (Double Word):** 4 Bytes = 32 Bits
- **QWORD (Quad Word):** 8 Bytes = 64 Bits

In our Day-1 experiment, the `DWORD` memory operands represented 4-byte integer values.

For example:

```text
05 00 00 00
```

represents a 4-byte value.

---

### Hexadecimal System (Base-16)

Hexadecimal is a base-16 number system.

It uses:

```text
0 1 2 3 4 5 6 7 8 9 A B C D E F
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

---

### 2-Digit Hex Conversion Formula (`0xXY`)

```text
(X × 16) + Y
```

Examples:

```text
0x11
= (1 × 16) + 1
= 17
```

```text
0x19
= (1 × 16) + 9
= 25
```

```text
0x2A
= (2 × 16) + 10
= 42
```

```text
0x2F
= (2 × 16) + 15
= 47
```

---

### 3-Digit Hex Conversion Formula (`0xXYZ`)

```text
(X × 16²) + (Y × 16) + Z
```

Examples:

```text
0x100
= (1 × 16²) + (0 × 16) + 0
= 256
```

```text
0x200
= (2 × 16²) + (0 × 16) + 0
= 512
```

---

### 4-Digit Advanced Hex Conversion (`0xXYZW`)

```text
(X × 16³) + (Y × 16²) + (Z × 16) + W
```

#### Example 1 — `0x1A2F`

```text
(1 × 4096)
+ (10 × 256)
+ (2 × 16)
+ 15

= 6,703
```

#### Example 2 — `0xDEAD`

```text
(13 × 4096)
+ (14 × 256)
+ (10 × 16)
+ 13

= 57,005