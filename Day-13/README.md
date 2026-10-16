# Day 13: Memory Access & Data Modification

Reverse Engineering & Low-Level Engineering · Theory Notes

## 1. What Is Memory?

Memory is the place where a running program stores the data it needs.

For example, a C++ program can store variables such as `health`, `damage`, and `score` in memory.

```
int health = 100;
int damage = 25;
```

Here:

- `health` stores the value `100`.
- `damage` stores the value `25`.
- The program uses these values while it executes.

Important: A variable's value and its memory address are different things. The value is the data stored; the address tells us where that data is stored.

## 2. What Is a Memory Address?

A memory address identifies a location in the computer's memory.

For example:

```
Address: 000000439FDFFB8C
Value:   64 00 00 00
```

The address identifies the location, while the bytes represent the stored data.

In a typical x86/x64 system, addresses are displayed in hexadecimal.

## 3. What Is Hexadecimal?

Hexadecimal, or hex, is a number system with 16 symbols:

`0 1 2 3 4 5 6 7 8 9 A B C D E F`

Computers and debugging tools frequently display addresses, register values, and memory bytes in hexadecimal.

| Hexadecimal | Decimal |
| ----------- | ------- |
| `19`        | 25      |
| `37`        | 55      |
| `50`        | 80      |
| `64`        | 100     |

For example:

- `0x19` = 25
- `0x50` = 80

The `0x` prefix indicates a hexadecimal number.

## 4. What Is Stack Memory?

The stack is a region of memory used during function execution. It commonly holds local variables, saved registers, and return addresses.

Example:

```
int main() {
    int health = 100;
    int damage = 25;
}
```

When the compiler places these local variables on the stack, the program can access them through addresses relative to a register such as `RBP` or `RSP`.

In our debugging example:

```
mov dword ptr ss:[rbp-4], 64
mov dword ptr ss:[rbp-8], 19
```

These instructions store 32-bit values at stack-relative memory locations.

- `[RBP-4]` was the location used for `health`.
- `[RBP-8]` was the location used for `damage`.

These offsets depend on the compiler and the program; they are not universal rules for every program.

## 5. Understanding the `RBP` Register

`RBP` is a 64-bit general-purpose register often used as a frame pointer.

It can help a debugger identify stack locations relative to the current function's stack frame.

For example:

```
mov eax, dword ptr ss:[rbp-4]
```

This instruction reads a 32-bit value from the memory address `RBP-4` and copies it into `EAX`.

Remember: `RBP-4` means an address four bytes below the address held in `RBP`. It does not mean subtract four from the variable's value.

## 6. Understanding `EAX` and `RAX`

`RAX` is a 64-bit register. `EAX` refers to its lower 32 bits.

For example:

```
mov eax, dword ptr ss:[rbp-8]
```

If the memory location contains hexadecimal `19`, then:

```
EAX = 00000019
RAX = 0000000000000019
```

The value is 25 in decimal.

On x86-64, writing to `EAX` also clears the upper 32 bits of `RAX`.

## 7. What Is Memory Reading?

Memory reading means retrieving a value from a memory location without changing that location.

Example:

```
mov eax, dword ptr ss:[rbp-4]
```

Suppose `[RBP-4]` contains `80`.

After executing the instruction:

```
EAX = 80
```

The instruction copies the value into a register. It does not change the original memory value.

## 8. What Is Memory Modification?

Memory modification means changing the data stored at a memory location.

For example, the original health value was:

```
100 decimal = 64 hexadecimal
```

We changed the four bytes in the debugger's memory view to:

```
50 00 00 00
```

On a little-endian system, these bytes represent the 32-bit integer `0x00000050`, which is 80 in decimal.

The modified value was therefore:

```
Before: 100
After:   80
```

This changed the value in the running process's memory. It did not rewrite the original C++ source code.

## 9. What Is Little-Endian?

Little-endian is a way of storing multi-byte values in memory. The least significant byte is stored at the lowest address.

For example, the 32-bit integer `0x00000064` is stored as:

```
Address increases →

64 00 00 00
```

The bytes appear in the reverse order compared with how we normally write the hexadecimal number.

This is why the memory dump may look different from the register value.

## 10. Understanding the `MOV` Instruction

`MOV` copies data from a source to a destination.

Examples:

```
mov edx, eax
```

Copies the value of `EAX` into `EDX`.

```
mov eax, dword ptr ss:[rbp-8]
```

Reads four bytes from memory into `EAX`.

```
mov dword ptr ss:[rbp-4], eax
```

Writes the lower 32 bits of `EAX` into memory.

`MOV` itself does not perform addition or subtraction.

## 11. Understanding the `SUB` Instruction

`SUB` subtracts the source operand from the destination operand.

Our program used:

```
health = health - damage;
```

The corresponding operation in assembly can look like:

```
sub dword ptr ss:[rbp-4], eax
```

In our practical, `health` had been changed to 80 and `EAX` contained 25.

\\[ 80-25=55 \\]

Therefore:

```
Health before subtraction: 80
Damage:                    25
Health after subtraction:  55
```

The result is stored back in the destination memory location.

## 12. How We Verified the Result in x64dbg

During our practical, we observed these values:

| Item                     | Hexadecimal | Decimal |
| ------------------------ | ----------- | ------- |
| Modified health          | `50`        | 80      |
| Damage                   | `19`        | 25      |
| Health after subtraction | `37`        | 55      |

The memory dump displayed the adjacent 32-bit values together as:

```
0000003700000019
```

Read the four bytes at each address separately:

- At `[RBP-8]`: `19 00 00 00` → damage = 25.
- At `[RBP-4]`: `37 00 00 00` → health = 55.

This is a useful example of why we must consider the address, byte order, and data size when interpreting a memory dump.

## 13. What Is the Difference Between Source Code and Process Memory?

| Source code                                                                   | Process memory                                                  |
| ----------------------------------------------------------------------------- | --------------------------------------------------------------- |
| Contains the program's written instructions.                                  | Contains data used by the running program.                      |
| Edited in a code editor.                                                      | Inspected in a debugger's memory view.                          |
| Changing source code usually requires recompilation to affect the executable. | Changing a variable in memory can affect the current execution. |
| Describes the program's intended operations.                                  | Shows the program's current runtime state.                      |

Memory changes made in a debugger are generally temporary. Restarting the program normally initializes its variables again, unless the program loads or saves the changed data elsewhere.

## 14. Key Takeaways

- Memory stores data used by running programs.
- A memory address identifies where data is stored.
- Hexadecimal is commonly used to display addresses and bytes.
- `RBP` can help locate local variables in a stack frame.
- `MOV` copies data; `SUB` performs subtraction.
- Reading memory does not necessarily modify it.
- Little-endian storage explains the order of bytes in a dump.
- A debugger can inspect and, when permitted, modify a process's runtime memory.

## Quick Revision Questions

Try answering these without looking at the notes.

1. What is the difference between a memory address and a value?
2. What is `0x19` in decimal?
3. What does `mov eax, dword ptr ss:[rbp-8]` do?
4. Why can a memory dump display bytes in a different order from a hexadecimal integer?
5. If health is 80 and damage is 25, what is the result of `SUB`?
6. Does changing a variable in the debugger automatically rewrite the original C++ source code?
