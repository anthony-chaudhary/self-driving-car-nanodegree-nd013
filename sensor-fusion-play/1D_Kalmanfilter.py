# Write a program that will iteratively update and
# predict based on the location measurements
# and inferred motions shown below.


def update(mean1, var1, mean2, var2):
    new_mean = float(var2 * mean1 + var1 * mean2) / (var1 + var2)
    new_var = 1. / (1. / var1 + 1. / var2)
    return [new_mean, new_var]


def predict(mean1, var1, mean2, var2):
    new_mean = mean1 + mean2
    new_var = var1 + var2
    return [new_mean, new_var]


measurements = [5., 6., 7., 9., 15.]
motion = [1., 1., 2., 5., 1.]
measurement_sig = [20., 4000., 20., 50000., 100.]
motion_sig = [2., 4., 2., .5, 1.]
mu = 0.
sig = 10000.

"""
Iterate through measruements array
(assumes measurements array length = motion length)
Assign mu and sig = new_mean and new_var return of update function
Use initial mu and sig for update
measurement_sig (variance) is how confidnent we are of our measurements
"""

print("(Start) position:", mu, "variance", sig, "\n")

for i in range(len(measurements)):
    mu, sig = update(mu, sig, measurements[i], measurement_sig[i])
    print("(Update ) position:", mu, "variance:", sig)  # debugging statement

    mu, sig = predict(mu, sig, motion[i], motion_sig[i])
    print("(Predict) position:", mu, "variance:",
          sig, "\n")  # debugging statement


print(mu, sig)
