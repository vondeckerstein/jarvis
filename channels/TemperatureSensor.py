from IOChannel import *

class TemperatureSensor(IOChannel):
	def __init__(self, id, name, link, pin=0):
		self.attr= {}
		self.attr['type']='OneWire temperture sensor'
		self.attr['pin']=pin
		self.link=link
		self.attr['id']=id
		self.attr['name']=name
		self.attr['unit']='C'
		self.attr['commands']=None
	def read(self):
		self.link.flushInput()
		self.link.write('#TR' + str(self.attr['id']) + '.')
		return float(self.link.readline()) * mult
