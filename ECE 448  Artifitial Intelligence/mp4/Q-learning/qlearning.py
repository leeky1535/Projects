import numpy as np
import random

# https://github.com/studywolf/blog/blob/master/RL/Cat%20vs%20Mouse%20exploration/qlearn.py
class qlearning(object):
	def __init__(self, epsilon, alpha, gamma):
		self.q = {}
		self.epsilon = epsilon
		self.alpha = alpha
		self.gamma = gamma
		self.explored = {}
		self.actions = [-1, 0, 1]

	def getQ(self, state, action):
		return self.q.get((state, action), 0.0)

	def learnQ(self, state, action, reward, value):
		if (state, action) not in self.explored:
			self.explored[(state, action)] = 0
		else:
			self.explored[(state, action)] += 1
		old_value = self.q.get((state, action), None)
		if old_value is None:
			self.q[(state, action)] = reward
		else:
 			self.q[(state, action)] = old_value + float(self.alpha)/float(self.alpha + self.explored[(state, action)]) * (value - old_value)
 
	def chooseAction(self, state):
		if random.random() < self.epsilon:
			return random.choice(self.actions)
		else:
			q = [self.getQ(state, a) for a in self.actions]
			maxQ = max(q)
			if q.count(maxQ) > 1:
				best = [i for i in range(3) if q[i] == maxQ]
				action = self.actions[random.choice(best)]
				return action
			else:
				return self.actions[q.index(maxQ)]

	def chooseActionRandom(self):
		return random.choice([-1.0, 0.0, 1.0])

	def learn(self, state1, action1, reward, state2):
		maxqnew = max([self.getQ(state2, a) for a in self.actions])
		self.learnQ(state1, action1, reward, reward + self.gamma*maxqnew)


		