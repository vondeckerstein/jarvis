import sys
import glob
import serial

import jarvisHost
import channeltypes
import rules


CHANNELS_CONFIG_LOCATION = 'config/devices.csv'
RULES_CONFIG_LOCATION = 'config/rules.csv' 

class ConfigLoader:
	def __init__(self):
		self.jarvis = jarvisHost.JarvisHost() 
		print "finding arduinos..."
		self.ports=self.__getSerialPorts()
		self.links={}
		for connection in self.ports: #print available serial connections
			print "making connection at " + connection
			link = ''
			link = channels.ArduinoIO.ArduinoIO(connection)
			self.jarvis.addLink(link)
			self.links[link.getAttr('id')]=link
		lines = []
		print "loading channel configuration..."
		self.__parseFile(CHANNELS_CONFIG_LOCATION, self.__parseDevice)
		print "configured " + str(self.jarvis.getChannels())+ " channels."
		print "configured " + str(self.jarvis.getLinks())+ " links."
		print "loading rules..."
		self.__parseFile(RULES_CONFIG_LOCATION, self.__parseRule)
		print "loaded " + str(self.jarvis.getRules())+ " rules."
	def __parseFile(self, fileLocation, lineParseFunction):
		f = open(fileLocation, 'r')
		lines=[]
		for newline in f:
			lines.append(newline.replace('\r\n',''))
		for line in lines: 
			lineParseFunction(line)
	def __parseRule(self, line):
		args= line.split(',')
		constructor = getattr(getattr(rules, args[1]),args[1])
		kwargs = {}
		for index in range(2, len(args)):
			arg = args[index].split(':')
			print 
			if arg[1].isdigit():
				arg[1]=int(arg[1])
			kwargs[arg[0]]=arg[1]
		output = constructor(**kwargs)
		output.setAttr('outputFunction',self.jarvis.writeChannel)
		output.setAttr('outputFunctionArgs',['channel','output'])
		self.jarvis.addRule(output)
	def __parseDevice(self, line):
		args= line.split(',')
		if args[0]== '':
			pass
		elif args[0] == '#':
			pass
			#reserved for later
		else:
			try:
				if args[2]== "Arduino":
					try:
						self.links[args[1]].setAttr('name',args[4]) 
					except(KeyError):
						print 'failed to add link: "' + args[1] + '"'
				else:	#line is a channel
					try:
						constructor = getattr(getattr(channels, args[2]),args[2])
						output = constructor(args[0],args[4],self.getLink(args[1]),args[3])
						self.jarvis.addChannel(output)
					except Exception, e:
						print "valid class not found: " + args[2] 
						print e
			except(KeyError):
				print 'invalid line in configuration file:'
				print line
	def __getSerialPorts(self):
		temp_list = glob.glob ('/dev/tty[A-Za-z]*')
		result = []
		for a_port in temp_list:
			try:
				if 'USB' in a_port or 'ACM' in a_port:
					s = serial.Serial(a_port)
					s.close ()
					result.append (a_port)
			except serial.SerialException:
				pass
		return result

	def getLink(self, id):
		return self.links[id]

	def getConfiguredHost(self):
		return self.jarvis
	
if __name__=="__main__":
	config = ConfigLoader()
	if len(sys.argv) == 2:
		if 'verbose' == sys.argv[1]:
			for line in config.getConfiguredHost().toString():
				print line
	jarvis = config.getConfiguredHost()
	for line in jarvis.toString():
		print line
	sys.exit(0)

	