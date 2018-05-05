Controls:-	
	LEFT KEY: 	Move your player to left
	RIGHT KEY: 	Move your player to right
	SPACE BAR:	Make your player jump
	DOWN KEY:	Apply brake to the horizontal movement.

GamePlay:-
->	Initially the ball will be stationary.
->	On pressing any of the Left or Right key a specific velocity is imparted to the ball in that direction.The Jump height is adjusted such that it can jump
	over only one level of blocks at a time.
->	On Landing on the top of blocks points are rewarded and some vertical push is given.On hitting/collinding with any other side the ball will be reflected back
	with some reduced velocity(co-efficient of restitution)
->	The ball is unable to jump from water.Due to the drag the horizantal velocity os ball is reduced.
->	The triangular shaped blocks are traps/spikes. Landing on them will reduce your life.You have 3 life chances in the begining.
->	The teal color rectangular blocks are trampolines.Once again, Landing on top only will give you certain velocity.
->	Two levels are implemented.In the second level the enemies/scoring_blocks will be mobile as well the number of traps will increase.
->	On hitting all the blocks of level One you can move forward to the next level.
->	Current Score,Lives remaining and Current level will be displayed in title bar.
