print("hello")

a_int = 3
b_float = 2.2
c_str = "6"
d_array = [1,5,6]
input_int = int(input("input"))
layer = int(input("Enter layer:"))
for i in range(1,layer+1):
    print("*"*i)
print("\n")
for i in range(1,layer+1):
    print(" "*(layer-i),"*"*(i))


for num in range(1,10):
    for mul in range(1,10):
        print("{}x{}={}".format(num,mul,num*mul))

while input_int<10:
    print (input_int," ",end="")
    input_int += 1
print("\n")

for num in d_array:
    print(num," ",end="")
print("\n")

for num in range(-10,10,3):
    print(num," ",end="")
print("\n")

print(a_int * c_str, b_float,c_str,end="\t",sep="|||")
print(type(a_int))
print("value_a: {}, value_b: {}\n{}".format(a_int, b_float, c_str))

if a_int>b_float and b_float<int(c_str):
    print("b>a")
else:
    print("a = b")
print("c")
