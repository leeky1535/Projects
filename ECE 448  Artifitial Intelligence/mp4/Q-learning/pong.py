import pygame, sys
from pygame.locals import *
from structs import *
import signal
import sys
import math
from matplotlib import pyplot

analyze = 1  #change this to 0 to see the actual game
fps = 80
windowWidth = 800
windowHeight = 800
paddleWidth = 20
ballWidth = 10

black = (0,0,0)
blue = (0,0,255)

game = pong()

def draw():
	line_x = [0, numGames]
	line_y = [9,9]
	f,ax = pyplot.subplots()
	ax.plot(game.x, game.y, 'b', label='Average Bounce')
	pyplot.xlabel('# of games')
	pyplot.ylabel('Avg # of bounce')
	pyplot.title('alpha = ' + str(alpha) + '/(' + str(alpha) + '+N(s,a)), gamma = ' + str(gamma) + ', epsilon = ' + str(epsilon))
	pyplot.grid(True)
	pyplot.show()


def main():
	if analyze == 1:
		while True:
			game.update()
			if game.done:
				draw()
				break
				
	elif analyze == 0:
		pygame.init()
		global DISPLAYSURF
		
		fpsClock = pygame.time.Clock()
		DISPLAYSURF = pygame.display.set_mode((windowWidth,windowHeight)) 
		pygame.display.set_caption('Pong')
		
		ball = pygame.Rect(game.state[0]*windowWidth- ballWidth, game.state[1]*windowWidth - ballWidth, paddleWidth, paddleWidth)
		paddle = pygame.Rect(1.0*windowWidth - paddleWidth, game.state[4]*windowWidth - paddleWidth, paddleWidth, game.paddle.height*windowHeight)
		wall = pygame.Rect(0, 0, paddleWidth, windowHeight)

		DISPLAYSURF.fill(black)
		ball.x = game.ball.x*windowWidth - ballWidth
		ball.y = game.ball.y*windowWidth - ballWidth
		pygame.draw.rect(DISPLAYSURF, game.ball.color, ball)

		paddle.y =  game.paddle.y*windowWidth - ballWidth
		pygame.draw.rect(DISPLAYSURF, game.paddle.color, paddle)
		
		pygame.draw.rect(DISPLAYSURF, blue, wall)

		while True:
			for event in pygame.event.get():
				if event.type == QUIT:
					pygame.quit()
					sys.exit()

			game.update()
			DISPLAYSURF.fill(black)
			ball.x = game.ball.x*windowWidth - ballWidth
			ball.y = game.ball.y*windowWidth - ballWidth
			pygame.draw.rect(DISPLAYSURF, game.ball.color, ball)
			paddle.y = game.paddle.y*windowWidth - ballWidth
			pygame.draw.rect(DISPLAYSURF, game.paddle.color, paddle)
			pygame.draw.rect(DISPLAYSURF, blue, wall)
			pygame.display.update()
			fpsClock.tick(fps)

main()
