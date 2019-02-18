from qlearning import *
import math

alpha = 50
gamma = 0.7
epsilon = 0.15
numGames = 20000

class ball(object):
	def __init__(self):
		self.color = (255, 0, 0)
		self.x = 0.5
		self.y = 0.5
		self.vx = 0.03
		self.vy = 0.01

	def update(self):
		self.x += self.vx
		self.y += self.vy
		self.bounceWall()

	def bouncePaddle(self):
		self.x = 2 - self.x
		xRandVal = random.uniform(-0.015, 0.015)
		yRandVal = random.uniform(-0.03, 0.03)
		self.vx = -self.vx + xRandVal
		self.vy += yRandVal
		if self.vx > 0:
			self.vy = min(1.0, max(0.03, self.vy))
		elif self.vx < 0:
			self.vx = max(-1.0, min(-0.03, self.vx))
		if self.vy > 0:
			self.vy = min(1.0, self.vy)
		elif self.vy < 0:
			self.vy = max(-1.0, self.vy)

	def bounceWall(self):
		if self.y < 0:
			self.y = -self.y
			self.vy = -self.vy
		elif self.y > 1:
			self.y = 2 - self.y
			self.vy = -self.vy
		elif self.x < 0:
			self.x = -self.x
			self.vx = -self.vx

		
class paddle(object):
	""" paddle class """
	def __init__(self):
		self.color = (255, 255, 255)
		self.height = 0.2
		self.x = 1.0
		self.y = 0.5 - self.height/2
		self.v = 0.04

	def hitBoundaries(self):
		self.y = max(0, self.y)
		self.y = min(1-self.height, self.y)

	def random(self):
		actions = [0.0, -0.04, 0.04]
		move = random.choice(actions)
		self.y += move
		self.hitBoundaries()

	def qlearn(self, action):
		self.y += action*0.04
		self.hitBoundaries()

	def update(self, ball_y):
		self.qlearn(ball_y)

class pong(object):
	""" game pong """
	def __init__(self):
		self.ball = ball()
		self.paddle = paddle()
		self.qlearning = qlearning(epsilon=epsilon, alpha=alpha, gamma=gamma)
		self.state = (self.ball.x, self.ball.y, self.ball.vx, self.ball.vy, self.paddle.y)
		self.prevState = None
		self.prevAction = None
		self.score = 0
		self.bouncePaddle = False
		self.scores = []
		self.numGames = 0
		self.termination = False
		self.done = False
		self.x = [0]
		self.y = [0]

	def terminate(self):
		if len(self.scores) == 1000:
			self.scores = self.scores[1:]
		self.scores.append(self.score)
		self.score = 0
		self.termination = True
		self.numGames += 1
		total = 0
		if self.numGames%1000 == 0:
			total = float(sum(self.scores))/1000.0
			self.x.append(self.numGames)
			self.y.append(total)
			print(self.numGames)
			print(total)
		if self.numGames == numGames:
			self.done = True

	def hitPaddle(self):
		if self.ball.x > self.paddle.x:
			if self.ball.y > self.paddle.y and self.ball.y < self.paddle.y + self.paddle.height:
				self.ball.bouncePaddle()
				self.score += 1
				self.bouncePaddle = True
			else:
				self.terminate()

	def updateState(self):
		if self.termination:
			return (12,12,12,12,12)
		else:
			if self.ball.vx > 0:
				velocity_x = 1
			else:
				velocity_x = -1
			if self.ball.vy >= 0.02:
				verlocity_y = 1
			elif self.ball.vy <= 0.02:
				verlocity_y = -1
			else:
				verlocity_y = 0
			ball_x_discrete = min(11, int(math.floor(12 * self.ball.x)))
			ball_y_discrete = min(11, int(math.floor(12 * self.ball.y)))
			paddle_y_discrete = min(11, int(math.floor(12 * self.paddle.y / (1 - self.paddle.height))))
			return (ball_x_discrete, ball_y_discrete, velocity_x, verlocity_y, paddle_y_discrete)

	def update(self):
		self.hitPaddle()
		state = self.updateState()
		reward = 0.0

		if self.termination:
			reward = -1000.0
			if self.prevState is not None:
				self.qlearning.learn(self.prevState, self.prevAction, reward, state)
			self.prevState = None
			self.ball = ball()
			self.paddle = paddle()
			self.termination = False
			return

		if self.bouncePaddle:
			self.bouncePaddle = False
			reward = 1000.0

		if self.prevState is not None:
			self.qlearning.learn(self.prevState, self.prevAction, reward, state)

		state = self.updateState()
		action = self.qlearning.chooseAction(state)
		self.prevState = state
		self.prevAction = action
		self.paddle.update(action)
		self.ball.update()
		



