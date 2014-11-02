from IOChannel import *

class Relay(IOChannel):
	def __init__(self, id, name, link,pin=0):
		self.attr= {}
		self.attr['type']='Relay'
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
		if state=='on':
			output=1
		elif state=='off':
			output=0
		else:
			output=None
		if output==0 or output==1:
			self.link.write('#RW' + str(self.attr['id']) + '.' + str(output) + '.')
		elif state=='save':
			self.__save()
		elif state=='load':
			self.__load()
	def __save(self):
		self.link.write('#RS')
	def __load(self):
		self.link.write('#RL')