import serial
import time
import sys
import colors

class ArduinoIO:
	def __init__(self, link, name="unnamed link"):
		self.attr={}
		if link is not None:
			self.link = serial.Serial(str(link), baudrate=115200,timeout=.1)
		self.attr['serial'] = link
		self.attr['name'] = name
		self.attr['busy'] = False
		for i in range(50):
			self.ask('#RR')
			sys.stdout.write('.')
			sys.stdout.flush()
		print ''
		self.flushInput()
		time.sleep(1)
		self.flushInput()
		print 'link ' + link + ' initialized!'
		self.__getId()
	def readline(self):
		try:
			output = self.link.readline()
		except serial.serialutil.SerialException:
			print "Serial Error! No data received! last command received: " + self.lastCommand
			output = 0
		return output
	def __badger(self, message):
		tries = 0
		output = ''
		self.write(message)
		while (output is '' or output is None) and tries < 100:
			if tries%10 == 0:
				self.write(message)
			output = self.readline()
			tries += 1
		print 'Tried ' + str(tries) + ' times. got ' + str(output)
		return output
	def write(self, command):
		self.lastCommand = command
		while self.attr['busy']:
			time.sleep(.01)
		#print 'writing ' + command + ' to ' + self.attr['serial'] 
		self.link.write(command)
	def flushInput(self):
		self.link.flushInput()
	def getName(self):
		return self.attr['name']
	def toString(self):
		message = str(self.attr['id']) + ': ' + self.attr['name'] + ' on port ' + self.attr['serial']
		return message
	def ask(self, message, multipleLines=False):
		self.flushInput()
		self.write(message)
		if multipleLines:
			input=True
			output=""
			while input: 
				line = self.readline()
				if line !='':
					output +=line
				else:
					input=False
		else:
			return self.readline()
	def __getId(self):
		print 'getting link id'
		self.attr['id'] = self.ask('#UC')[0]
		print 'link id is ' + str(self.attr['id'])
	def getAttr(self, key=None):
		if key is None:
			return self.attr
		else:
			return self.attr[key]
	def setAttr(self, key, value):
		self.attr[key]=value

		