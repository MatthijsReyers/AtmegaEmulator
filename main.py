
import curses, random

class brick:
    def __init__(self, shape):
        self.shape = shape
        self.sh0 = [
            [1,1,1,1],
            [0,0,0,0]]
        self.sh1 = [
            [0,1,0,0],
            [1,1,1,0]]
        self.sh2 = [
            [1,0,0,0],
            [1,1,1,0]]
        self.sh3 = [
            [0,0,0,1],
            [0,1,1,1]]
        self.sh4 = [
            [1,1,0,0],
            [0,1,1,0]]
        self.sh5 = [
            [0,0,1,1],
            [0,1,1,0]]

    def getShape(self):
        if self.shape == 0: return self.sh0
        elif self.shape == 1: return self.sh1
        elif self.shape == 2: return self.sh2
        elif self.shape == 3: return self.sh3
        elif self.shape == 4: return self.sh4
        elif self.shape == 5: return self.sh5

def moveLeft():
    pass

def moveRight():
    pass

def smashDown():
    pass

def switchHold():
    pass

def DetectCollision():
    pass

def BlockRenderer(screen, BaseY, BaseX, block):
    ShapeToRender = block.getShape()
    for y in [0,1]:
        for x in [0,1,2,3]:
            print(ShapeToRender[y][x])
            if ShapeToRender[y][x] == 1:
                screen.addch(BaseY+y, BaseX+x, '#')

def UIrenderer(screen):
    PlayingFieldWidth = 10
    PlayingFieldHeight = 20

    screen.addstr(0, 0, ''.ljust(26,'═'))
    screen.addstr(21,8, ''.ljust(10,'═'))
    screen.addstr(5, 1, ''.ljust(6,'═'))
    screen.addstr(14,19,''.ljust(6,'═'))
    
    for y in range(1,5): screen.addch(y,0,'║')
    for y in range(1,21): screen.addch(y,7,'║')
    for y in range(1,21): screen.addch(y,18,'║')
    for y in range(1,14): screen.addch(y,25,'║')

    screen.addch(0, 0, '╔')
    screen.addch(5, 0, '╚')
    screen.addch(21,7, '╚')
    screen.addch(0, 25,'╗')
    screen.addch(21,18,'╝')
    screen.addch(14,25,'╝')

    screen.addch(5, 7, '╣')
    screen.addch(0, 7, '╦')
    screen.addch(0, 18,'╦')
    screen.addch(14,18,'╠')

def MainRenderer(screen):

    # Get proper curses settings.
    screen.nodelay(True)

    # Get display size.
    MaxHeight, MaxWidth = screen.getmaxyx()

    # Generate block cue.
    BlockCue = []
    for i in range(5):
        rm = random.randint(0,6)
        BlockCue.append(brick(rm))

    while True:
        screen.clear()
    
        # Render interface lines.
        UIrenderer(screen)

        # Render blocks in block cue.
        for i in range(5):
            BlockRenderer(screen, 2+(i*3), 20, BlockCue[i])

        screen.refresh()

        key = screen.getch()
        if key in ['\x1b[C', ord('d')]: moveRight()
        elif key in ['\x1b[D', ord('a')]: moveLeft()
        elif key in ['\x1b[B', ord('s')]: moveDown()
        elif key == ord('q'):
            break


if __name__ == '__main__':

    curses.wrapper(MainRenderer)
