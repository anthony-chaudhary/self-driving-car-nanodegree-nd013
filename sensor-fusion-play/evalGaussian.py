
import math
import matplotlib.pyplot as plot

mu = 60
sigmaSquared = 7
x = 8
pi = math.pi  # 3.141
# print(pi)
e = math.e  # 2.718
# print(e)


def evalGaussian(mu, sigmaSquared, x):

    a = (1 / math.sqrt(2 * pi * sigmaSquared))
    b = ((-1 / 2) * math.pow((x - mu), 2)) / sigmaSquared
    result = a * math.pow(e, b)

    # print("Start:", a)
    # print("Exponent part:", b)
    return result


# Test single
# print("\nResult:", evalGaussian(mu, sigmaSquared, x))

# Test range
testRanges = [x for x in range(100)]
results = []
resultsDict = {}

for i, x in enumerate(testRanges):
    result = evalGaussian(mu, sigmaSquared, x)
    results.append(result)
    resultsDict[result] = i
    # resultsDict.update({result: i})  #Alternate dict update method
    # print("\nResult:", x)

maxResults = max(results)
# print(resultsDict)

print(maxResults)
print(resultsDict[maxResults])
# print(results)

plot.plot(results)
plot.show()
