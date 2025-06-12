/*module abcd (A,B,C,F);
input A,B,C;
output F;
wire D,E;
and a1 (D,A,B);
not n1 (E,C);
or o1 (F,D,E);
//assign out = in1 & in2;
endmodule

module decoder (A,B,Y);
input A,B;
output [3:0]Y;

reg[3:0]Y
always @(A or B)
	case({A,B})
		2'b00:Y = {4'b1110};
		2'b01:Y = {4'b1101};
		2'b10:Y = {4'b1011};
	default:Y = {4'b0111};
	endcase
endmodule*/
/*
module Q1 (A,B,C,D,out);
input A,B,C,D;
output out;
wire E,F,G;
or o1 (E,A,B);
and a1 (F,C,D);
xor x1 (G,~E,F);
not n1 (out,G);
endmodule
*/
module Q2 (A,B,C,D,E,F,G);
input A,B,C,D;
output E,F,G;
and a1 (E,A,B);
not n1 (F,C);
not n2 (G,D);
endmodule