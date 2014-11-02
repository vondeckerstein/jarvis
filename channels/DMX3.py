from IOChannel import *
import time

colors = { 	'red' : [255, 0, 0] , 'green' : [0, 255, 0], 
			'blue' : [0, 0, 255] , 'cyan' : [0, 255, 255] , 
			'yellow' : [255, 255, 0] ,	'incandescent' : [255, 120, 60],  
			'purple' : [255, 0, 255], 'white' : [255, 255, 255]}

class DMX3(IOChannel):
	def __init__(self, id, name, link, pin=0):
		self.attr= {}
		self.attr['type']='DMX3'
		self.attr['id']=id
		self.link=link
		self.attr['name']=name
		self.attr['pin']=pin
		self.attr['R']=255 
		self.attr['G']=255
		self.attr['B']=255
		self.attr['P']=0
		self.attr['K']=0
		self.attr['color']='white'
		self.attr['colors']=colors
		self.attr['mode']='manual'
		self.attr['commands']= [0,2,5,10,25,50,75,100]
		for color in colors:
			self.attr['commands'].append(color)
	def read(self):
		return {'R':self.attr['R'],'G':self.attr['G'],'B':self.attr['B'],'P':self.attr['P']}
	def write(self, message):
		print 'writing: "' + str(message) + '" on ' + self.attr['name'] + '" - link: ' + self.link.getAttr('name')
		if isinstance(message,basestring):
			self.__writeColor(message)
		elif isinstance(message, list):
			self.__manualWrite(message[0],message[1],message[2])
		elif isinstance(message, int):
			self.__writePower(message)
		else:
			print 'unrecognized message type: ' + str(type(message))
	def __manualWrite(self, R, G, B, T=1):
		self.attr['R'] = R
		self.attr['G'] = G
		self.attr['B'] = B
		self.attr['mode']= 'manual'
		self.__write()
	def __writeTemp(self, K, P=0, T=1):
		if P==0:
			P=self.attr['P']
		if K <= 6600:
			R = 255 
			G = 99.4708025861 * math.log(K/100) - 161.1195681661
			if G < 0:
				G = 0
			if G > 255:
				G = 255
		else:
			R = 329.698727446 * math.pow((K/100-60), -0.1332047592)
			if R < 0:
				R = 0
			if R > 255:
				R = 255
			G = 288.1221695283 * math.pow(((K/100)-60),-0.0755148492)
			if G < 0:
				G = 0
			if G > 255:
				G = 255
		if K >= 6600:
			B = 255
		else:
			if K <= 1900:
				B = 0
			else:
				B = 138.5177312231 * math.log((K/100)-10) - 305.0447927307
				if B < 0:
					B = 0
				if B > 255:
					B = 255
		self.attr['R'] = R
		self.attr['G'] = G
		self.attr['B'] = B
		self.attr['P'] = P
		self.__write()
	def __writeColor(self, color, P=0, T=1):
		self.attr['R'] = colors[color][0]
		self.attr['G'] = colors[color][1]
		self.attr['B'] = colors[color][2]
		self.attr['mode']='color'
		self.attr['color']=color
		self.__write()
	def __writePower(self, P, T=1):
		self.attr['P'] = P
		self.__write()
	def __write(self):
		R = int(self.attr['R']) * int(self.attr['P']) / 100
		G = int(self.attr['G']) * int(self.attr['P']) / 100
		B = int(self.attr['B']) * int(self.attr['P']) / 100
		if G < 0:
			G = 0
		if G > 255:
			G = 255
		if R < 0:
			R = 0
		if R > 255:
			R = 255
		if B < 0:
			B = 0
		if B > 255:
			B = 255
		self.link.write('#DW' + str(self.attr['pin']) + '.' + str(R) + '.' + str(G) + '.' + str(B) + '.1.')
		self.link.busy=True
		time.sleep(.2)
		self.link.busy=False
