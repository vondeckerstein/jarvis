from IOChannel import *

class AnalogIn(IOChannel):
	def __init__(self, id=None, name=None, link=None, pin=0):
		self.attr={}
		self.attr['type']='AnalogIn'
		self.attr['direction']='input'
		self.link=link
		self.attr['pin']=pin
		self.attr['id']=id
		self.attr['mult']=0.0739820076
		self.attr['unit']='Amps'
		self.attr['zero']=512
		self.attr['min']=0
		self.attr['max']=1023
		self.attr['name']=name
		self.attr['commands']=None
		self.attr['rawvalue']=0
		self.attr['value']=0
	def read(self):
		self.link.flushInput()
		self.link.write('#AR' + str(self.attr['pin']) + '.')
		try:
			self.attr['rawvalue']=float(self.link.readline()) 
		except:
			print "Could not read", self.attr['name']
		self.attr['value']= (self.attr['zero'] - self.attr['rawvalue']) * self.attr['mult']