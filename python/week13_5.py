print("Enter layer: ", end="")
layer = int(input())
print("Enter width: ", end="")
width = int(input())
k = 0
for i in range(1, layer):
    for j in range(0, int((width+1)/2)):
        print(" "*(layer-i-1),"*"*(2*i-1)," "*(layer-i-2+2*layer), end="")
    print("\n", end="")

print("*"*(2*layer*width-width), end="\n")
for i in range(1, layer):
    for j in range(0, int((width)/2)):
        print(" "*(i-2+2*layer),"*"*(2*(layer-i)-1)," "*(i-1), end="")
    print("\n", end="")
