// Find Max

// D=RAM[0]-RAM[1]
    @0
D=M
@1
D=D-M

// if(RAM[0]>=RAM[1]) goto IF
@IF
D ;JGE

// else goto IF
@ELSE
0 ;JMP

(IF)
// max=RAM[0]
@0
D=M
@max
M=D
// goto COND_END
@COND_END
0 ;JMP

(ELSE)
// max=RAM[1]
@1
D=M
@max
M=D

(COND_END)
