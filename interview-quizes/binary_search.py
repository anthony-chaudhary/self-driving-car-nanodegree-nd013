import time
import math

# Implment binary search
# Binary search is a divide and conquer strategy. Key assumption -> list has already been sorted

def binary_search(target, search_area):
    """
    Goal: Complete a binary search

    Inputs:
    Target: an integer target to search for
    Search_area: a list of integers to search through

    Outputs:
    Location of target in search_area
    """

    updated_search_area = search_area   # This keeps naming conventions the same in while()
    candidate_value = 0  # Prototypes for ease of printing after loop
    candidate_location = 0

    while (updated_search_area != 0):

        candidate_location = int(len(updated_search_area) / 2)
        candidate_value = updated_search_area[candidate_location]

        if candidate_value == target:
            print("Candidate_value", candidate_value)
            return

        if candidate_location == 0:
        	print(target, "not in list")
        	return

        if target < candidate_value:
            # update search area to be just to left of middle
            updated_search_area = updated_search_area[: candidate_location]

        else:
            # update search area to be just to right of middle
            updated_search_area = updated_search_area[candidate_location:]


# Testing
target = -500
search_area = [i for i in range(0, 1000, 1)]
binary_search(target, search_area)

target = 469
search_area = [i for i in range(0, 1000, 1)]
binary_search(target, search_area)

# TODO add sorting step
# TODO store "original" location?

"""
Start > end 
if start and end cross can guarantee element does not exist in array
Above, candidate_location == start, and since we already check 
If candidate_value = target, if that passes, and candidate_location ==0
it's not in the list.
This is a slightly non standard implementation I'm exploring 
more typical implementation below
"""


def binarySearch_2(target, search_area):
	start = 0
	end = len(search_area) - 1
	found = False

	while start <= end and not found:
		
		midpoint = (start + end) // 2
		
		if search_area[midpoint] == target:

			found = True
			item = search_area[midpoint]
			
		if target < search_area[midpoint]:

			end = midpoint-1

		else:

			start = midpoint+1

	print(found, item)
	return found


#found = binarySearch_2(target, search_area)
#print(found)

def test(function, target):

    times = []
    lengths = [1e3, 1e4, 1e5, 1e6, 1e7]

    for i in lengths:

        start = time.time()

        array = [i for i in range(0, int(i))]

        # binary_search(target=math.floor(i / 2), search_area=array)
        function(target, array)

        end = time.time()

        times.append(end - start)

    for i, length in enumerate(lengths):

        print("For length", length, "\tratio", times[i] / length, "\tTime:", times[i])


print("\nSpeed tests:")
test(binary_search, 23)

print("\nmethod_2")
test(binarySearch_2, 23)

for i in range(10):
	print("\n Testing", i)
	test(binary_search, i)
