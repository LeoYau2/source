print("Enter layer: ", end="")
layer = int(input())
print("Enter width: ", end="")
width = int(input())
for i in range(1, layer+1):
    for j in range(0, width):
        if layer-i==0:
            print("*"*(2*i-1), end="")
        else:
            print(" "*(layer-i-1),"*"*(2*i-1)," "*(layer-i-1), end="")
    print("\n", end="")
