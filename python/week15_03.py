with open('USPopulation.txt','r') as f:
    #print(f.read())
    USPopulation = []
    #lines = f.readlines()
    #print(lines)
    
    for line in f:
        USPopulation.append(int(line))
    print(len(USPopulation))
    print(USPopulation)
    sum = 0
    max = 0
    min = 1000000
    for i in range(0,len(USPopulation)):
        sum += USPopulation[i]


    print('總人口 =',sum)