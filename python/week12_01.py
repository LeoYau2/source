a=float(input("a="))
b=float(input("b="))
c=float(input("c="))

if a+b>c and a+c>b and c+b>a:
    print("周長 = ", a+b+c)
else:
    print("無法形成三角形")
