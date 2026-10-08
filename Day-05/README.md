# Day 5 — Assembly Language & CPU Internals

## 1. CPU Flags & Conditional Branching

CPU instructions can update several **status flags** after execution. Conditional jump instructions use these flags to determine whether a branch should be taken.

### Important CPU Flags

- **ZF (Zero Flag):** Set when the result of an operation is `0`.
- **CF (Carry Flag):** Primarily indicates a carry or borrow condition in **unsigned arithmetic**.
- **SF (Sign Flag):** Reflects the most significant bit of the result, which is used as the sign bit for signed values.
- **OF (Overflow Flag):** Indicates **signed arithmetic overflow**.

### Conditional Jumps

```asm
JE      ; Jump if Equal / ZF = 1
JZ      ; Jump if Zero / ZF = 1
JNE     ; Jump if Not Equal / ZF = 0
JNZ     ; Jump if Not Zero / ZF = 0
```

> **Important:** Conditional jumps do not perform the comparison themselves. They check flags that were previously set by instructions such as `CMP`, `TEST`, or arithmetic instructions.

---

# 2. Bitwise Operations & Instructions

## 2.1 `TEST` Instruction

`TEST` performs a **bitwise AND** between its two operands and updates CPU flags based on the result.

The AND result is **not stored** in either operand.

### Common Pattern

```asm
test eax, eax
```

Conceptually:

```text
EAX & EAX
```

The result is discarded, but the flags are updated.

This makes `TEST` particularly useful for checking whether a value is **zero or non-zero**.

### Zero Check

```asm
test eax, eax
je somewhere
```

If:

```text
EAX = 0
```

then:

```text
ZF = 1
```

and `JE` takes the branch.

### Non-Zero Check

```asm
test eax, eax
jnz somewhere
```

If:

```text
EAX ≠ 0
```

then:

```text
ZF = 0
```

and `JNZ` takes the branch.

### Lowest-Bit Check

```asm
test eax, 1
```

This can be used to examine the **least significant bit (LSB)**.

```text
LSB = 0 → ZF = 1
LSB = 1 → ZF = 0
```

Because the least significant bit distinguishes even and odd integers, this pattern can be used for an **even/odd check**.

> **Important:** The meaning of `TEST` depends on its operands and the surrounding instructions.

---

# 2.2 Logical Operations

## `AND` — `&`

A result bit is `1` only when **both corresponding input bits are `1`**.

```text
1 & 1 = 1
1 & 0 = 0
0 & 1 = 0
0 & 0 = 0
```

### Bit Masking

```asm
and eax, 0Fh
```

This preserves the **lowest 4 bits** of EAX and clears the remaining bits.

---

## `OR` — `|`

A result bit is `1` when **at least one corresponding input bit is `1`**.

```text
1 | 1 = 1
1 | 0 = 1
0 | 1 = 1
0 | 0 = 0
```

---

## `XOR` — `^`

A result bit is `1` when the corresponding input bits are **different**.

```text
1 ^ 1 = 0
1 ^ 0 = 1
0 ^ 1 = 1
0 ^ 0 = 0
```

### Common Zeroing Pattern

```asm
xor eax, eax
```

Conceptually:

```text
EAX = EAX XOR EAX
    = 0
```

Therefore:

```text
EAX = 0
```

---

# 2.3 Shift Operations

## `SHL`

`SHL` shifts bits toward the **left**.

```asm
shl eax, 1
```

For unsigned/integer bit manipulation, a left shift by one position corresponds to multiplying by `2` when no significant bits are discarded.

---

## `SHR`

`SHR` shifts bits toward the **right** and inserts zeros from the left.

```asm
shr eax, 1
```

For unsigned values, a right shift by one position corresponds to dividing by `2`, with the fractional part discarded.

---

## `SAR`

`SAR` performs an **arithmetic right shift**.

```asm
sar eax, 1
```

Unlike `SHR`, `SAR` preserves the sign by propagating the most significant bit.

### Important Difference

```text
SHR → Logical right shift
SAR → Arithmetic right shift
```

This distinction becomes especially important when working with signed values.

---

# 2.4 Bit Masks

A **bit mask** is a value used to selectively manipulate individual bits or groups of bits.

Bit masks can be used to:

- isolate specific bits,
- check specific bits,
- set specific bits,
- clear specific bits.

Example:

```asm
and eax, 0Fh
```

Conceptually:

```text
EAX
AND
0000000Fh
```

This isolates the lowest 4 bits.

---

# 3. Pointers & Memory Addressing

## Pointer

A **pointer** is a value that represents the address of a memory location.

A register can contain such a value.

```asm
mov rax, 1000h
```

Here, RAX contains:

```text
1000h
```

If that value represents a valid memory address, RAX is holding that address.

> **Important:** A register itself is not inherently a pointer. Its current value may represent an address, an integer, or another type of bit pattern depending on how the program uses it.

---

# 3.1 `RAX` vs `[RAX]`

This distinction is fundamental in reverse engineering.

### `RAX`

```asm
RAX
```

means:

> The value currently stored in the RAX register.

That value may represent:

- an integer,
- a pointer/address,
- or another bit pattern.

### `[RAX]`

```asm
[RAX]
```

means:

> Access the memory located at the address contained in RAX.

Conceptually:

```text
RAX
↓
Value stored in the register
```

```text
[RAX]
↓
Memory at the address contained in RAX
```

---

# 3.2 Memory Read

```asm
mov eax, [rbp - 4]
```

Conceptually:

```text
Memory[RBP - 4] → EAX
```

The CPU:

1. Calculates the effective address `RBP - 4`.
2. Reads the value from that memory location.
3. Places the value into EAX.

---

# 3.3 Memory Write

```asm
mov [rax], ecx
```

Conceptually:

```text
ECX → Memory[RAX]
```

The value in RAX is treated as the memory address, and the value in ECX is written to that location.

---

# 4. Memory Addressing Formula

A common x86-64 memory-addressing form is:

```text
[BASE + INDEX × SCALE + OFFSET]
```

Where:

- **BASE** → Base register
- **INDEX** → Index register
- **SCALE** → Multiplication factor
- **OFFSET** → Constant displacement

The CPU uses these components to calculate an **effective memory address**.

---

## 4.1 Direct Register Address

```asm
[RAX]
```

Conceptually:

```text
Address = RAX
```

The value contained in RAX is used as the memory address.

---

## 4.2 Base + Offset

```asm
[RAX + 10h]
```

Conceptually:

```text
Address = RAX + 0x10
```

The offset modifies the base address by `0x10` bytes.

---

## 4.3 Base + Index × Scale

```asm
[RAX + RCX * 4]
```

Conceptually:

```text
Address = RAX + (RCX × 4)
```

This addressing pattern is commonly encountered when accessing **arrays and indexed data structures**.

---

# 4.4 Valid Scale Values

For x86/x64 memory addressing, the scale factor can be:

```text
1
2
4
8
```

For example:

```asm
[RAX + RCX * 8]
```

means:

```text
Address = RAX + (RCX × 8)
```

A scale of `8` commonly appears when working with **8-byte elements**, such as pointers or 64-bit values.

---

# 5. Core Rules to Remember

```text
RAX
↓
Value stored in the RAX register
```

```text
[RAX]
↓
Memory at the address contained in RAX
```

```text
test eax, eax
↓
Check whether EAX is zero or non-zero
```

```text
xor eax, eax
↓
EAX = 0
```

```text
and eax, 0Fh
↓
Isolate the lowest 4 bits
```

```text
SHL
↓
Logical left shift
```

```text
SHR
↓
Logical right shift
```

```text
SAR
↓
Arithmetic right shift
```

```text
[BASE + INDEX × SCALE + OFFSET]
↓
General x86-64 effective-address form
```

# Day 5 — Key Takeaway

> **Registers hold values. A value can represent an address. Square brackets indicate memory access using that value as an address. CPU flags describe the results of operations, and conditional jumps use those flags to control program flow.**