import sys
import threading
import time
import subprocess

from pynput import keyboard
import pyautogui as bot
import mouse as bot2

from PIL import Image, ImageEnhance, ImageFilter
import pytesseract
from pytesseract import Output



# set these to your screen's specs
left = 510
top = 240
unit = 49.5
resetx = 535
resety = 798
playx = 706
playy = 502



# 2 different f'ns to perform 1 click-and-drag-move (idk which one is better yet)
def selectRect(l,t,r,b):
	bot.moveTo(left+l*unit, top+t*unit)
	bot.mouseDown()
	bot.moveRel((r-l+1)*unit,(b-t+1)*unit)
	bot.mouseUp()

def selectRect2(l,t,r,b):
	bot2.move(left+l*unit, top+t*unit, absolute=True, duration=0)
	bot2.press(button='left')
	bot2.move(left+(r+1)*unit, top+(b+1)*unit, absolute=True, duration=0.1)
	bot2.release(button='left')

def doOCR():
	img = Image.open('grid-picture.png')
	img = img.convert('RGBA')
	pix = img.load()
	for y in range(img.size[1]):
		for x in range(img.size[0]):
			r = pix[x, y][0]
			g = pix[x, y][1]
			b = pix[x, y][2]
			if r+g+b > 680 and g<=r and g<=b:
				pix[x, y] = (0, 0, 0, 255)
			else:
				pix[x, y] = (255, 255, 255, 255)

	file = open("input-grid","w") # 'w' will create a new file if it doesn't exist, and overwrite everything when writing to it

	text = pytesseract.image_to_string(img, config='-c tessedit_char_whitelist=0123456789 --psm 6 --oem 0')
	file.write(text)

	file.close()
	img.save('temp.png')

# main game loop
def playGame():
	time.sleep(2) # to give time to switch to browser

	while (1):
		# click 'reset' and 'play'
		bot2.move(resetx,resety,absolute=True,duration=0)
		bot2.click(button='left')
		bot2.move(playx,playy,absolute=True,duration=0)
		bot2.click(button='left')

		# take screenshot of game board
		time.sleep(0.2)
		boardImg = bot.screenshot('grid-picture.png', region=(left,top,17*unit,10*unit))

		# do OCR to get text
		doOCR()

		# run C++ exec and wait for it to finish
		subprocess.call('algorithm.exe')

		# read from C++ generated file and make moves
		file = open("output-moves","r")
		score = int(file.readline()) # score is on first line of file
		if score <= 155:
			continue
		for line in file.readlines():
			nums = line.split(' ')
			selectRect(int(nums[0]),int(nums[1]),int(nums[2]),int(nums[3]))
		file.close()

		time.sleep(5)

		highScore = bot.screenshot('highscore.png')
		break



# start game logic on separate thread
t = threading.Thread(target=playGame)
t.daemon = True
t.start()



# while game is playing, add keyboard listener that exits program when 'esc' is pressed
def on_press(key):
    pass

def on_release(key):
    if key == keyboard.Key.esc:
        sys.exit()

with keyboard.Listener(
        on_press=on_press,
        on_release=on_release) as listener:
    listener.join()
