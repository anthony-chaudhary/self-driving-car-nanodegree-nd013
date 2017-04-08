
import math
import matplotlib.pyplot as plot

"""
Mu = mean1 / prior measurement
v = mean2 / new measurement
Sigma^2 = variance1 / prior variance
Tau^2 = vaiance2 / new variance

"""

# Measurement update step in Kalman filter


def newMeanAndVariance(mean1, variance1, mean2, variance2):

    newMean = (1 / (variance1 + variance2)) * \
        ((variance2 * mean1) + (variance1 * mean2))
    newVariance = 1 / ((1 / variance1) + (1 / variance2))

    return newMean, newVariance

# Test case from quiz, expected answer is 12.4, 1.6
print(newMeanAndVariance(10., 8., 13., 2.))


# Test range
testRanges = [x for x in range(1, 200)]
resultsDict = {}

mean1 = 8
variance1 = 8.0
mean2 = 10

for i, x in enumerate(testRanges):
    newMean, newVariance = newMeanAndVariance(
        mean1, variance1, mean2, x)
    # print(newVariance, newMean, i + 1)
    resultsDict[i + 1] = (newMean, newVariance)

# maxResults = max(results)
# print(resultsDict)
# print("max results", maxResults)
print(resultsDict[2])
# print(results)

"""
Notice as new variance increases
mean and variance both apporach the original measurement
"""
#plot.plot(results, label=['Line 2', 'line1'])

for k, v in resultsDict.items():
    plot.plot(k, v[0], 'ro', k, v[1], 'g^', k, variance1, "bo", k, mean1, "bo")

plot.legend()  # To draw legend
plot.xlabel('variance2')
plot.ylabel('Value')
plot.title(
    'Effect of variance2. Red = newMean, Green = newVariance, original measurement = blue')
plot.show()


testDict = {'mean1': 10., 'variance1': 8., 'mean2': 13., 'variance2': 2.}


def newMeanAndVariance2(*args):

    # TODO a way to loop through and handle an arbitrary number of means
    # and variances
    newMean = (1 / (variance1 + variance2)) * \
        ((variance2 * mean1) + (variance1 * mean2))
    newVariance = 1 / ((1 / variance1) + (1 / variance2))

    return newMean, newVariance

# print(newMeanAndVariance2(testDict))
