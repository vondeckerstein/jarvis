from IOChannel import *

class DMX1(IOChannel):
	def __init__(self, id, name, link, pin=0):
		self.attr= {}
		self.attr['type']='DMX-Single'
		self.attr['pin']=pin
		self.attr['id']=id
		self.link=link
		self.attr['name']=name
		self.attr['P']=0
		self.attr['commands']=[0,2,4,8,16,32,64,100]
	def write(self, message):
		self.__writePower(int(message))
	def read(self):
		return self.attr['P']
	def __writePower(self, P, T=1):
		self.attr['P'] = P
		self.__write()
	def __write(self):
		P = int(self.attr['P']) * 255 / 100
		if P < 0:
			P = 0
		if P > 255:
			P = 255
		self.link.write('#DW' + str(self.attr['id']) + '.' + str(P) + '.')
		self.link.busy=True
		self.mode='manual'
		time.sleep(.2)
		self.link.busy=False

