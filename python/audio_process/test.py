x=0
x/=2
maxError = 0.0001
target = 0.5
print(x)

adjust = 0.5
print("error",abs(x-target))
y = 0
while y < 10:
    print(y)
    y += 1

testInput = 0
while abs(x-target) > maxError:
    x = testInput*0.7
    if x > target:
        testInput -= adjust
    else:
        testInput += adjust
    print("error",x-target,"adjust",adjust,"testInput",testInput,"x",x)
    adjust /= 2
print("testInput",testInput)
print("x",x)
