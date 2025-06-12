import os
import random
def print_star(layer):
    for i in range(layer, 0,-1):
        print("*" * i)
    for i in range(2, layer+1):
        print("*" * i)

again = 'y'
while again=='y':
    layer = int(input("number of layer: "))
    result = print_star(layer)
    print("".format(result))

    value = random.randint(1,10000)
    if value==input("value "):
        print("correct")
    else:
        print("value = " + str(value))

    again = input("Again? (y/n)")
