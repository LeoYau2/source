import random

#fp = open("1.txt","r") as fp
'''with open("1.txt","w") as f:
    for i in range(6):
        f.write("{}".format(i))'''
total = str()
with open("1.txt","r") as fp:
    #print(fp.readline)
    for line in fp:
        #print(line)
        total+=line
print(total)
n = 0
ex_list = [1,2,3,7.5]
for num in ex_list:
    n+=num
print(n)
print(len(ex_list))
print(sum(ex_list))
print(max(ex_list))
print(sorted(ex_list,reverse=1))

ex2_list = list()
ex2_list.append("Hello")
print(ex2_list)
ex2_list.append(5)
print(ex2_list)
ex2_list.insert(1,"world")
print(ex2_list)
ret = ex2_list.pop(2)
print(ret)
print(ex2_list)
ex_list.remove(1)
print(ex2_list)

def add(x,y):
    return x+y

def sub(x,y):
    return x-y

def mul(x,y):
    return x*y

def div(x,y):
    return x/y

def main():
    operation_list = [add, sub, mul, div]
    x=float(input("The first number: "))
    y=float(input("The second number: "))
    operation = int(input("OP (0) ADD (1) SUB (2) MUL (3) DIV: "))

    if operation < 4 and operation >= 0:
        print("result={}".format(operation_list[operation](x,y)))
main()

ex3_list = [5,6,7]
p_list = ex3_list
ex3_list_copy = [] + ex3_list
p_list[0] = 9
print(ex3_list)
print(ex3_list_copy)
#p_list.append(p_list)
p_list.append(ex3_list)
print(p_list[3])
