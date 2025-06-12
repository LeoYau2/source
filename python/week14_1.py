number = input()
reverse = ""
for i in range(0,len(number)):
    reverse = number[i] + reverse
print("--> {}*5 =".format(reverse),int(reverse)*5)
#print("--> {}*5 =".format(reverse),int(input()[::-1])*5)
