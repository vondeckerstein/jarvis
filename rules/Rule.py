class Rule:
	attr= {}
	def setAttr(self, key, value):
		self.attr[key]=value
	def getAttr(self, key=None):
		if key is None:
			return self.attr
		else:
			return self.attr[key]
	def run(self):
		pass
	def toString(self):
		message = ""
		for key in self.attr: 
			message+= key + ": " + str(self.attr[key]) + "    "
		return 	message + '\n'