
from sklearn.naive_bayes import GaussianNB
from sklearn.ensemble import ExtraTreesClassifier
from sklearn.neural_network import MLPClassifier
from sklearn.preprocessing import StandardScaler

class GNB(object):

	def __init__(self):
		self.possible_labels = ['left', 'keep', 'right']
		self.clf = GaussianNB()
		#self.clf = ExtraTreesClassifier(n_estimators=20, max_depth=45, min_samples_split=4, random_state=0)
		#self.clf = MLPClassifier(hidden_layer_sizes=(4000), 
			#alpha=1e-8, momentum=.7, verbose=True, tol=1e-7, max_iter=400)
		self.scaler = StandardScaler()


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
		#x = [[i[0], i[2], i[3], i[1]%4] for i in data]
		x = [[i[3]] for i in data]
		#print(len(x))
		#self.clf.fit(x, labels)
		
		#self.scaler.fit(data[0])
		#data = self.scaler.transform(data)
		self.clf.fit(x, labels)


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
	
		#i = self.scaler.transform([observation])
		i = [observation[3]]
		#prediction = self.clf.predict([[i[1], i[2], i[3], i[1]%4]])
		prediction = self.clf.predict(i)
		#print(prediction)

		return prediction