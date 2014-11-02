import os
import channeltypes

class Configurator:
	def __init__(self, filename):
		self.device = self.loadDeviceConfiguration(filename)
		
		print ""
		print "Configured device as follows:"
		print self.device.attr
		for channel in self.device.channels:
			print channel.attr
		print ""
		
	def generateFirmware(self, filename):
		print "loading firmware block files..."
		sourceFiles = ["firmwareblocks/basetemplate.h","firmwareblocks/AnalogIn.h",]
		files = []
		dicts = []		
		for sourceFile in sourceFiles:
			files.append(self.loadFile(sourceFile))
		
		#parse sourcefiles into dictionaries
		for index, file in enumerate(files):
			print ""
			print "parsing file: " + sourceFiles[index]
			file = file.split("//{{{")
			dict = {"header comments" : file.pop(0)}
			for object in file:
				print "splitting object:"
				print object
				object = object.split("}}}")
				print "Adding " + object[0] + " to dictionary"
				dict[object[0]]=object[1]
			print "file contents:"
			print file
			print "dict contents:"
			print dict
			dicts.append(dict)
			
		#build output file
		outputFile = ""
		fields = [ 	'header comments', 'inclusions', 'definitions', 'declarations', 
					'setup header', 'setup', 'setup footer', 'loop header', 'loop',
					'commands header', 'commands', 'commands footer', 'loop footer',
					'functions' ]
		for field in fields:
			for index, dict in enumerate(dicts):
				if field in dict:
					if dict[field] not in [None, "", "\n"]: #check for actual content
						if index != 0:
							outputFile += "\n// " + field + " from " + sourceFiles[index] + "\n"
						outputFile += dict[field]
		
		#save output file to <filename>.ino
		print outputFile
		
	def loadDeviceConfiguration(self, configurationName):
		channels = []
		device = channeltypes.ArduinoInterface.ArduinoInterface()
		print "Getting configuration file for" + configurationName + "..."
		
		#build file in memory
		file = self.loadFile("devices/" + configurationName + ".cfg")
		
		#break file into constituent items:
		file = file.split("#iolocation:")
		for index, line in enumerate(file): #because line is NOT passed by reference
			line=line.replace('\t', "")
			line=line.split('\n')
			for subIndex, item in enumerate(line):
				if item.count(":")>0:
					item = item.split(":")
					line[subIndex] = item
			file[index]=line
		
		# populate device attributes
		self.addAttributesFromList(device, file.pop(0))
		
		# build list of channels from list of items
		for object in file:
			if isinstance(object[0], str):
				type = object.pop(0)
				print "Building channel: '" + type +"'"
				print [type]
				module = getattr(channeltypes, type)
				constructor = getattr(module, type)
				channel = constructor()
				self.addAttributesFromList(channel, object)
				device.channels.append(channel)
		return device
		
	def addAttributesFromList(self, item, attributesList):
		for attribute in attributesList:
			if isinstance(attribute, list):
				item.attr[attribute[0]] = attribute[1]
					
	def loadFile(self, fileLocation):
		f=open(fileLocation, "r")
		file=""
		for line in f:
			file += line
		f.close()
		file = file.replace("\r","")
		return file

if __name__ == "__main__":
	c = Configurator("ArduinoUno")
	
	c.generateFirmware("ArduinoUno")
	