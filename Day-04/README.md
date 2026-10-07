# 🔐 Low-Level Engineering — Day 04

## Topic: Understanding Control Flow — CMP, TEST & Jumps in x64

Today, I focused on understanding how programs make decisions at the assembly level.

By analyzing comparison instructions, conditional jumps, and CPU flags, I learned how to trace and predict execution paths during reverse engineering.

---

## 🎯 Learning Objectives

- Understand `CMP` and `TEST`
- Understand how CPU flags are affected
- Understand unconditional and conditional jumps
- Understand `ZF`, `CF`, `SF`, and `OF`
- Trace control flow using x64dbg
- Predict whether a conditional jump will be taken
- Verify execution flow using `RIP`

---

# 1. Assembly Instructions for Control Flow

## 1.1 `CMP` — Compare

```asm
CMP op1, op2
```

`CMP` performs a subtraction internally:

```text
op1 - op2
```

However, the result is **not stored in a register**.

Instead, the instruction updates the CPU flags in `RFLAGS`.

### Example

```asm
cmp eax, ebx
```

Conceptually:

```text
EAX - EBX
     ↓
CPU Flags (RFLAGS)
```

The result itself is discarded.

The updated flags are then used by conditional jump instructions.

---

## 1.2 `TEST` — Logical Test

```asm
TEST op1, op2
```

`TEST` performs a bitwise AND operation:

```text
op1 & op2
```

The result is not stored.

Instead, the CPU flags are updated.

### Common Example

```asm
test eax, eax
```

This is commonly used to check whether `EAX` is zero.

Conceptually:

```text
EAX & EAX
   ↓
Flags updated
```

If `EAX = 0`, the Zero Flag (`ZF`) becomes `1`.

---

# 2. Jump Instructions

## 2.1 `JMP` — Unconditional Jump

```asm
JMP <address>
```

`JMP` directly changes the execution flow to the specified address.

No condition is checked.

```text
Current Instruction
        ↓
       JMP
        ↓
Target Address
```

---

## 2.2 Conditional Jumps

Conditional jumps examine CPU flags that were previously modified by instructions such as `CMP` or `TEST`.

| Instruction | Meaning | Condition |
|-------------|---------|-----------|
| `JE` / `JZ` | Jump if Equal / Zero | `ZF = 1` |
| `JNE` / `JNZ` | Jump if Not Equal / Not Zero | `ZF = 0` |
| `JG` / `JL` | Jump if Greater / Less | Signed comparison |
| `JA` / `JB` | Jump if Above / Below | Unsigned comparison |

### Basic Control-Flow Model

```text
CMP / TEST
     ↓
RFLAGS Updated
     ↓
Conditional Jump
     ↓
Jump Taken?
   ↙     ↘
 YES      NO
  ↓        ↓
Target   Next Instruction
```

---

# 3. Understanding CPU Flags — RFLAGS

Conditional jumps depend heavily on CPU flags.

The key flags studied today were:

## `ZF` — Zero Flag

`ZF` is set to `1` when the result of an operation is zero.

For example, when comparing two equal values:

```text
5 - 5 = 0
```

The Zero Flag becomes:

```text
ZF = 1
```

This can cause:

```asm
JE
```

or

```asm
JZ
```

to take the jump.

---

## `CF` — Carry Flag

`CF` is important for unsigned comparisons and indicates an unsigned borrow/carry condition.

It is especially important when analyzing **unsigned comparisons**.

---

## `SF` — Sign Flag

`SF` indicates the sign of the result.

```text
SF reflects the most-significant bit of the result.
SF = 0 → Most-significant bit is 0
SF = 1 → Most-significant bit is 1
```

---

## `OF` — Overflow Flag

`OF` indicates a signed overflow condition.

It can occur when a signed arithmetic result cannot be represented within the available signed range.

---

# 4. Signed vs Unsigned Comparisons

Conditional jumps are not all interpreted in the same way.

### Signed Comparison

Common instructions:

```asm
JG
JL
```

These are used for signed comparisons.

### Unsigned Comparison

Common instructions:

```asm
JA
JB
```

These are used for unsigned comparisons.

This distinction is important when analyzing conditional branches in assembly.

---

# 5. Practical Analysis Workflow — x64dbg

When analyzing control flow in x64dbg, I follow this workflow:

### Step 1 — Examine Operands

First, inspect the values being compared by:

```asm
CMP
```

or:

```asm
TEST
```

These values may come from registers or memory.

---

### Step 2 — Verify Register Values

Check the current register values in x64dbg.

For example:

```text
EAX
EBX
ECX
EDX
```

Understanding these values helps predict the result of the comparison.

---

### Step 3 — Predict the Jump

Look at the upcoming conditional jump.

For example:

```asm
cmp eax, ebx
je  target
```

Determine whether the condition should be satisfied based on the compared values.

---

### Step 4 — Understand the Condition

Identify exactly what the jump is checking.

For example:

```asm
test eax, eax
je target
```

The important question is:

```text
Is EAX equal to zero?
```

---

### Step 5 — Step Through the Instruction

Use:

```text
F8 — Step Over
```

Execute the instruction and observe what happens to the execution flow.

---

### Step 6 — Verify the Execution Path

Observe the changing instruction location through `RIP`.

If the condition is satisfied, execution moves to the jump target.

If the condition is not satisfied, execution continues to the next instruction.

---

# 6. Practical Mental Model

The most important flow I learned today:

```text
Operands
   ↓
CMP / TEST
   ↓
RFLAGS Updated
   ↓
Conditional Jump
   ↓
Jump Condition Evaluated
   ↓
RIP Changes
   ↓
New Execution Path
```

This gives a useful reverse-engineering mental model:

```text
Values
  ↓
Comparison
  ↓
CPU Flags
  ↓
Branch Condition
  ↓
Control Flow
```

---

# 🧠 Key Takeaways

Today I learned that assembly-level decision making is closely connected to CPU flags.

Instead of treating a conditional jump as an isolated instruction, I can analyze it as a complete chain:

```text
CMP / TEST
     ↓
RFLAGS
     ↓
Conditional Jump
     ↓
RIP
     ↓
Execution Path
```

This helps me understand how high-level conditions are represented and traced at the assembly level.

---

# 🛠️ Tools Used

- **x64dbg**
- **x64 Assembly**
- **C++ test programs**


---

# ✅ Day 04 Complete

**Topic:** Control Flow — `CMP`, `TEST`, CPU Flags & Jumps

**Core Mental Model:**

```text
CMP / TEST
     ↓
RFLAGS
     ↓
JMP / Conditional Jump
     ↓
RIP
     ↓
Execution Path
```

🔐 One step deeper into x64 Assembly and Reverse Engineering.
```

