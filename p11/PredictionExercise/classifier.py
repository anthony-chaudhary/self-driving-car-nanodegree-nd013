
from sklearn.naive_bayes import GaussianNB
from sklearn.ensemble import ExtraTreesClassifier

class GNB(object):

	def __init__(self):
		self.possible_labels = ['left', 'keep', 'right']
		#self.gnb = GaussianNB()
		self.gnb = ExtraTreesClassifier(n_estimators=20, max_depth=45, min_samples_split=4, random_state=0)

	def train(self, data, labels):
		"""
		Trains the classifier with N data points and labels.

		INPUTS
		data - array of N observations
		  - Each observation is a tuple with 4 values: s, d, 
		    s_dot and d_dot.
		  - Example : [
			  	[3.5, 0.1, 5.9, -0.02],
			  	[8.0, -0.3, 3.0, 2.2],
			  	...
		  	]

		labels - array of N labels
		  - Each label is one of "left", "keep", or "right".
		"""
		#print(data)
		#print(labels)
		#x = [[i[1], i[2], i[3], i[1]%4] for i in data]
		#print(len(x))
		#self.gnb.fit(x, labels)
		self.gnb.fit(data, labels)


	def predict(self, observation):
		"""
		Once trained, this method is called and expected to return 
		a predicted behavior for the given observation.

		INPUTS

		observation - a 4 tuple with s, d, s_dot, d_dot.
		  - Example: [3.5, 0.1, 8.5, -0.2]

		OUTPUT

		A label representing the best guess of the classifier. Can
		be one of "left", "keep" or "right".
		"""
		# TODO - complete this
		i = observation
		#prediction = self.gnb.predict([[i[1], i[2], i[3], i[1]%4]])
		prediction = self.gnb.predict([i])
		#print(prediction)

		return prediction