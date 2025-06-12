import os
##os.system("pause")
dir = "t:\Pictures\Nikon D5600\135D5600\test"
#fp = open("name.txt")
for filename in os.listdir(dir):
    print(os.path.join(filename))
