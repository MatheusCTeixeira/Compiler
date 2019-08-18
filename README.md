```
  _    _ _    _ _    _    _____                      _ _
 | |  | | |  | | |  | |  / ____|                    (_) |
 | |__| | |__| | |__| | | |     ___  _ __ ___  _ __  _| | ___ _ __
 |  __  |  __  |  __  | | |    / _ \| '_ ` _ \| '_ \| | |/ _ \ '__|
 | |  | | |  | | |  | | | |___| (_) | | | | | | |_) | | |  __/ |
 |_|  |_|_|  |_|_|  |_|  \_____\___/|_| |_| |_| .__/|_|_|\___|_|
                                              | |
                                              |_|
```

# Como compilar o compilador

```bash
git clone https://github.com/MatheusCTeixeira/Compiler.git
cd Compiler
make
```

# Sintaxe
```c
{
    int var1 = 10;
    while (var > 0) {
        var = var - 1;
    } endwhile

    if (var1 == 0) {
        print("var1_eh_igual_a_zero");
    } else {
        print("var1_nao_eh_igual_a_zero");
    } endif
}
```

# Compilar o código
```bash
    ./hhh arquivo.hhh output.asm
```
O arquivo "arquivo.hhh" é o arquivo de entrada, o códido fonte.

O arquivo "output.asm" é o arquivo de saída. Diferente do gcc/g++, não é necessário utilizar flags como ```-o```.

# Exemplos

O arquivo ```/test``` contém diversos exemplos de códigos na linguagem HHH.

# Amostras
### Entrada
```c
{
    int a = 10;
    int b = 15;
    int c = 0;

    while (a != b) {
        c = c + 1;
        if ( c % 2 == 0)
            a = a + 1;
        else
            b = b - 1;
        endif

        print(a);
        print(b);

        print("____________________________________");
    } endwhile


    if (a==b)
    {
        print("a_igual_a_b");
    }
    else
    {
        print("a_diferente_de_b");
    }
    endif

    if (a != 0 && b != 0)
        print("a_e_b_sao_diferentes_de_0.");
    endif
}
```

### Saída
```asm
SECTION	.data
_a_0	DD	0
_b_1	DD	0
_c_2	DD	0
str_0	DB	"____________________________________", 0xA, 0
str_1	DB	"a_igual_a_b", 0xA, 0
str_2	DB	"a_diferente_de_b", 0xA, 0
str_3	DB	"a_e_b_sao_diferentes_de_0.", 0xA, 0
print_int	DB	"%d" , 0xA, 0
print_string	DB	"%s", 0xA, 0
scan_int	DB	"%d"

; nasm -f elf file.hhh
; gcc-6 -m32 file.asm -o file
SECTION	.text
global	main
extern	printf
extern	scanf


main:
MOV	dword EBX,	10
MOV	[_a_0],	EBX

MOV	dword EBX,	15
MOV	[_b_1],	EBX

MOV	dword EBX,	0
MOV	[_c_2],	EBX

while_0:

MOV	EBX,	[_b_1]
MOV	EAX,	[_a_0]
CMP	EAX,	EBX
LAHF
NOT	EAX
PUSH	EAX


POP	EAX
AND	AH,	64
CMP	AH,	0
JE	endwhile_0

MOV	dword EBX,	1
MOV	EAX,	[_c_2]
ADD	EAX,	EBX
PUSH	EAX

POP	EBX
MOV	[_c_2],	EBX

if_0:

MOV	dword EBX,	2
MOV	EAX,	[_c_2]
XOR	EDX,	EDX
IDIV	EBX
MOV	EAX,	EDX
PUSH	EAX


MOV	dword EBX,	0
POP	EAX
CMP	EAX,	EBX
LAHF
PUSH	EAX


POP	EAX
AND	AH,	64
CMP	AH,	0
JE	else_0

MOV	dword EBX,	1
MOV	EAX,	[_a_0]
ADD	EAX,	EBX
PUSH	EAX

POP	EBX
MOV	[_a_0],	EBX

JMP	endif_0
else_0:

MOV	dword EBX,	1
MOV	EAX,	[_b_1]
SUB	EAX,	EBX
PUSH	EAX

POP	EBX
MOV	[_b_1],	EBX

endif_0:
PUSH	dword [_a_0]
PUSH	print_int
CALL	printf
POP	EAX
POP	EAX
PUSH	dword [_b_1]
PUSH	print_int
CALL	printf
POP	EAX
POP	EAX
PUSH	print_string
PUSH	str_0
CALL	printf
POP	EAX
POP	EAX
JMP	while_0
endwhile_0:
if_1:

MOV	EBX,	[_b_1]
MOV	EAX,	[_a_0]
CMP	EAX,	EBX
LAHF
PUSH	EAX


POP	EAX
AND	AH,	64
CMP	AH,	0
JE	else_1
PUSH	print_string
PUSH	str_1
CALL	printf
POP	EAX
POP	EAX
JMP	endif_1
else_1:
PUSH	print_string
PUSH	str_2
CALL	printf
POP	EAX
POP	EAX
endif_1:
if_2:

MOV	dword EBX,	0
MOV	EAX,	[_a_0]
CMP	EAX,	EBX
LAHF
NOT	EAX
PUSH	EAX


MOV	dword EBX,	0
MOV	EAX,	[_b_1]
CMP	EAX,	EBX
LAHF
NOT	EAX
PUSH	EAX


POP	EBX
POP	EAX
AND	EAX,	EBX
PUSH	EAX


POP	EAX
AND	AH,	64
CMP	AH,	0
JE	else_2
PUSH	print_string
PUSH	str_3
CALL	printf
POP	EAX
POP	EAX
JMP	endif_2
else_2:
endif_2:

RET

```
