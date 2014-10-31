from IOChannel import *

class LED(IOChannel):
	def __init__(self, id, name, link,pin=0):
		self.attr= {}
		self.attr['type']='LED'
		self.link=link
		self.attr['id']=id
		self.attr['name']=name
		self.attr['state']=0
		self.attr['pin']=pin
		self.attr['commands']=['on','off']
	def read(self):
		self.link.flushInput()
		self.link.write('#RE' + str(self.attr['id']) + '.')
		state= self.link.readline()
		if state == '0':
			self.attr['state'] = '0'
		elif state == '1':
			self.attr['state'] = '1'
		return self.attr['state']
	def write(self, state):
		if int(state)==0 or int(state)== 1:
			self.link.write('#RW' + str(self.attr['id']) + '.' + str(state) + '.')