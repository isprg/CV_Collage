import sys
import os
import subprocess
import time
import pyautogui


def send_esc():
	pyautogui.press('esc')

def lena_run(args):
	#print(args)
	if(len(args) == 2):
		os.chdir(args[1])
		
	#print(os.getcwd())	
	subprocess.Popen("AR_Lena.exe")

args = sys.argv
lena_run(args)
time.sleep(10)
send_esc()
