#AlexanderMerry, decision Tree code
import random
maxValue = int(9)
starterValue= int (5)
Iterations = int(10000000)
#The first command ALWAYS starts at zone '5', which is zone 0 in literal terms
x=random.randint(1,9)
while (x==starterValue):
    x=random.randint(0,9)
if (x<starterValue):
    Fchance=1/starterValue
elif(x>starterValue):
    Fchance=1/(maxValue-(starterValue-1))
longrunner = 0
Value=x
#Loop to get an 'Iterations' number of tests to narrow the error produced
for j in range(0,Iterations):
    runner=1 # *Fchance if working off of the first one
    for i in range(0,4):
        x=random.randint(1,9)
        while (x==Value):
            x=random.randint(1,9)
        if (x<Value):
            chance=1/Value
        elif(x>Value):
            chance=1/(maxValue-(Value-1))
        runner=runner*chance
        Value=x
    longrunner = longrunner + runner
#Print the average over an 'Iterations' amount of tests
print("This is the average chance of getting all the zones correct",longrunner/Iterations)
